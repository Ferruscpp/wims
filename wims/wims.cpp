#include "console_func.h"
#include "ferruscpp_media.h"
#pragma comment(linker, "/STACK:16777216")
using namespace std;
using namespace ferruscpp;

namespace ferruscpp
{
	namespace wims_src
	{
		enum wims_mode
		{
			move,
			write,
			draw,
			pixel_select,
			text_select
		};

		enum direction
		{
			up,
			down,
			left,
			right
		};
	}

	namespace symbols
	{
		bool is_normal_symbol(const char& ch)
		{
			return 32 <= ch && ch <= 126;
		}

		bool is_enter(const char& ch)
		{
			return ch == '\n' || ch == '\r';
		}

		bool is_backspace(const char& ch)
		{
#if _WIN32
			return ch == 8;
#elif __linux__
			return ch == '\x7c'
#endif
		}

		char is_arrow(const char& ch, wims_src::direction*& d)
		{
			using namespace wims_src;
#if _WIN32
			if (ch == -32 || ch == 224)
			{
				if (!io::is_hit_())
				{
					return 0;
				}
				char ch_ = io::getch_();
				if (ch_ == 72)
				{
					d = new direction(direction::up);
				}
				else if (ch_ == 75)
				{
					d = new direction(direction::left);
				}
				else if (ch_ == 80)
				{
					d = new direction(direction::down);
				}
				else if (ch_ == 77)
				{
					d = new direction(direction::right);
				}
				else
				{
					return ch_;
				}
			}
			return 0;
#elif __linux__
			if (ch == 27)
			{
				if (!io::is_hit_())
				{
					return 0;
				}
				char ch_ = io::getch_();
				if (ch == '[')
				{
					ch_ = io::getch_();
					if (ch_ == 'A')
					{
						d = new direction(direction::up);
					}
					if (ch_ == 'B')
					{
						d = new direction(direction::down);
					}
					if (ch_ == 'C')
					{
						d = new direction(direction::right);
					}
					if (ch_ == 'D')
					{
						d = new direction(direction::left);
					}
					else
					{
						return ch_;
					}
				}
				else
				{
					return ch_;
				}
			}
			return 0;	
#endif
		}

		bool is_w(const char& ch)
		{
			return ch == 'w' || ch == 'W';
		}
		bool is_a(const char& ch)
		{
			return ch == 'a' || ch == 'A';
		}
		bool is_s(const char& ch)
		{
			return ch == 's' || ch == 'S';
		}
		bool is_d(const char& ch)
		{
			return ch == 'd' || ch == 'D';
		}
		bool is_moving_leter(const char& ch)
		{
			return is_w(ch) || is_a(ch) || is_s(ch) || is_d(ch);
		}
		bool is_moving_number(const char& ch)
		{
			return '1' <= ch && ch <= '9';
		}
	}

	namespace io
	{
		void print_colored_line(size_t y, colors::c16 background_color)
		{
			colors::set_color_16(background_color, background_color);
			cursor::set_cursor_pos(0, y);
			size_t max_x = screen::get_right_down_angle().first;
			std::string line;
			for (size_t x = 0; x < max_x; ++x)
			{
				line += ' ';
			}
			io::putstr_(line);
		}

		void read_and_write(std::string& line)
		{
			char ch = io::getch_();
			while (!symbols::is_enter(ch))
			{
				if (symbols::is_normal_symbol(ch))
				{
					line += ch;
					io::putstr_(std::string() + ch);
				}
				else if (symbols::is_backspace(ch))
				{
					if (!line.empty())
					{
						line.pop_back();
						auto cursor_pos = cursor::get_cursor_pos();
						cursor::set_cursor_pos(cursor_pos.first - 1, cursor_pos.second);
						io::putstr_(" ");
						cursor::set_cursor_pos(cursor_pos.first - 1, cursor_pos.second);
					}
				}
				ch = io::getch_();
			}
		}
	}

	namespace wims_src
	{

		template<typename Picture_>
		class wims
		{
		private:
			Picture_& picture;
			std::pair<size_t, size_t> pixel_size;
			std::pair<size_t, size_t> screen_size;
			wims_mode mode = wims_mode::move;
			std::pair<size_t, size_t> start_pos = { 1, 1 };
			using pixel_type = typename Picture_::pixel_type;
			bool need_set_foreground = false, need_set_background = false;
			pixel_type paint;


			std::pair<size_t, size_t> get_picture_pos(std::pair<size_t, size_t> cursor_pos)
			{
				return { (cursor_pos.first - start_pos.first) / pixel_size.first, (cursor_pos.second - start_pos.second) / pixel_size.second };
			}

			void write_markings()
			{
				cursor::set_cursor_pos(0, 0);
				colors::set_color_16(colors::c16(14), colors::c16(0));
				io::putstr_("\\");
				//write_vertical_numbers
				colors::set_color_16(colors::c16(13), colors::c16(0));
				cursor::set_cursor_pos(0, pixel_size.second);
				std::pair<size_t, size_t> n_cursor_pos = { 1, 1 };
				size_t counter = 0;
				while (picture.is_in_picture(points::position(get_picture_pos(n_cursor_pos))))
				{
					io::putstr_(std::to_string(counter % 10));
					++counter;
					n_cursor_pos.second += pixel_size.second;
					cursor::set_cursor_pos(0, pixel_size.second * (counter + 1));
				}
				//write_horizontal_numbers
				colors::set_color_16(colors::c16(3), colors::c16(0));
				cursor::set_cursor_pos(pixel_size.first, 0);
				n_cursor_pos = { 1, 1 };
				counter = 0;
				while (picture.is_in_picture(points::position(get_picture_pos(n_cursor_pos))))
				{
					io::putstr_(std::to_string(counter % 10));
					++counter;
					n_cursor_pos.first += pixel_size.first;
					cursor::set_cursor_pos(pixel_size.first * (counter + 1), 0);
				}
			}
			void draw_picture()
			{
				picture.draw(points::position(start_pos));
				cursor::set_cursor_pos(start_pos.first, start_pos.second);
				colors::set_color_16(colors::get_foreground_basic_color(), colors::get_background_basic_color());
			}
			void clear_console_line()
			{
				io::print_colored_line(screen_size.second - 1, colors::c16(0));
				cursor::set_cursor_pos(0, screen_size.second - 1);
				colors::set_color_16(colors::c16(15), colors::c16(0));
			}
			void open_console_mode()
			{
				io::print_colored_line(screen_size.second - 2, colors::c16(15));
				clear_console_line();
			}
			void close_console_mode()
			{
				io::print_colored_line(screen_size.second - 2, colors::c16(0));
				io::print_colored_line(screen_size.second - 1, colors::c16(0));
			}
			void draw_ui()
			{
				write_markings();
				draw_picture();
			}

			bool move_cursor(const direction& d)
			{
				auto cur_pos = cursor::get_cursor_pos();
				switch (d)
				{
				case direction::up: cur_pos.second -= pixel_size.second; break;
				case direction::down: cur_pos.second += pixel_size.second; break;
				case direction::left: cur_pos.first -= pixel_size.first; break;
				case direction::right: cur_pos.first += pixel_size.first; break;
				}
				if (picture.is_in_picture(points::position(get_picture_pos(cur_pos))))
				{
					cursor::set_cursor_pos(cur_pos.first, cur_pos.second);
					return true;
				}
				else
				{
					cur_pos.first -= pixel_size.first;
					if (picture.is_in_picture(points::position(get_picture_pos(cur_pos))))
					{
						cursor::set_cursor_pos(cur_pos.first + pixel_size.first, cur_pos.second);
						return true;
					}
				}
				return false;
			}
			void enter_move()
			{
				if (move_cursor(direction::down))
				{
					while (move_cursor(direction::left))
					{

					}
				}
			}

			template<typename Pixel_Type>
			void set_paint()
			{
				
			}
			void paint_pixel()
			{
				auto cursor_pos = cursor::get_cursor_pos();
				points::position picture_pos(get_picture_pos(cursor_pos));
				if (picture.is_in_picture(picture_pos))
				{
					pixel_type& pixel = picture.get_pixel(picture_pos);
					if (need_set_foreground)
					{
						if (need_set_background)
						{
							pixel.set(paint.get_foreground(), paint.get_background());
						}
						else
						{
							pixel.set(paint.get_foreground(), pixel.get_background());
						}
					}
					else
					{
						if (need_set_background)
						{
							pixel.set(pixel.get_foreground(), paint.get_background());
						}
					}
					pixel.draw();
					move_cursor(direction::left);
				}
			}

			int scan_number_in_console_mode_for_c16()
			{
				std::string s_number;
				io::read_and_write(s_number);
				if (s_number.empty())
				{
					return -1;
				}
				int number = -1;
				bool was_error = false;
				try
				{
					number = std::stoi(s_number);
				}
				catch (const std::exception& ex)
				{
					was_error = true;
				}
				if (number < 0 || 16 <= number || was_error)
				{
					clear_console_line();
					io::putstr_("Invalid argument");
					console::wait(500);
					return -1;
				}
				return number;
			}
			template<>
			void set_paint<pixels::Console_Pixel_16>()
			{
				io::putstr_("foreground: ");
				int foreground = scan_number_in_console_mode_for_c16();
				if (foreground != -1)
				{
					paint.set(colors::c16(foreground), paint.get_foreground());
					need_set_foreground = true;
				}
				else
				{
					need_set_foreground = false;
				}
				clear_console_line();
				io::putstr_("background: ");
				int background = scan_number_in_console_mode_for_c16();
				if (background != -1)
				{
					paint.set(paint.get_background(), colors::c16(background));
					need_set_background = true;
				}
				else
				{
					need_set_background = false;
				}
			}

			void exe_moving_leters(const char& ch)
			{
				if (symbols::is_w(ch))//up
				{
					move_cursor(direction::up);
				}
				if (symbols::is_a(ch))//left
				{
					move_cursor(direction::left);
				}
				if (symbols::is_s(ch))//down
				{
					move_cursor(direction::down);
				}
				if (symbols::is_d(ch))//right
				{
					move_cursor(direction::right);
				}
			}
			void exe_moving_numbers(const char& ch)
			{
				switch (ch)
				{
				case '8': move_cursor(direction::up); break;
				case '9': move_cursor(direction::up); move_cursor(direction::right); break;
				case '6': move_cursor(direction::right); break;
				case '3': move_cursor(direction::right); move_cursor(direction::down); break;
				case '2': move_cursor(direction::down); break;
				case '1': move_cursor(direction::down); move_cursor(direction::left); break;
				case '4': move_cursor(direction::left); break;
				case '7': move_cursor(direction::left); move_cursor(direction::up); break;
				case '5': //just stay here
					break;
				}
			}

		public:
			
			wims(Picture_& picture_) : picture(picture_)
			{
				pixel_size = picture.get_pixel_size();
				screen_size = screen::get_right_down_angle();
				draw_ui();
			}

			void start()
			{
				io::clear_in_buffer();
				char ch;
				char buffer = 0;
				while (true)
				{
					if (buffer == 0)
					{
						ch = io::getch_();
					}
					else
					{
						ch = buffer;
						buffer = 0;
					}
					//
					direction* d = nullptr;
					buffer = symbols::is_arrow(ch, d);
					if (d != nullptr)
					{
						move_cursor(*d);
						continue;
					}
					//
					if (ch == 27)//need to switch modes
					{
						auto cursor_pos = cursor::get_cursor_pos();
						open_console_mode();
						if (buffer == 0)
						{
							ch = io::getch_();
						}
						else
						{
							ch = buffer;
							buffer = 0;
						}
						while (ch != 27)
						{
							if (ch == ':')
							{
								clear_console_line();
								io::putstr_(":");
								std::string command;
								io::read_and_write(command);
								//
								if (command == "move")
								{
									mode = wims_mode::move;
								}
								else if (command == "write")
								{
									mode = wims_mode::write;
								}
								else if (command == "draw")
								{
									mode = wims_mode::draw;
								}
								else if (command == "s" || command == "save")
								{
									picture.upload();
								}
								else if (command == "q" || command == "quite")
								{
									return;
								}
								else if (command == "sq" || command == "save&quite" || command == "save quite" || command == "save and quite")
								{
									picture.upload();
									return;
								}
								else if (command == "d" || command == "download")
								{
									clear_console_line();
									io::putstr_("Do you really want to download picture? It will delete unsave changes[Y/N]: ");
									char ch_ = io::getch_();
									if (ch_ == 'Y')
									{
										picture.download();
										draw_ui();
									}
								}
								else if (command == "set_color" || command == "set color")
								{
									clear_console_line();
									set_paint<pixel_type>();
								}
								//
								clear_console_line();
							}
							else
							{
								clear_console_line();
								io::putstr_("[ESC]");
							}
							ch = io::getch_();
						}
						close_console_mode();
						draw_picture();
						cursor::set_cursor_pos(cursor_pos.first, cursor_pos.second);
					}
					else
					{
						if (mode == wims_mode::move)	
						{
							exe_moving_numbers(ch);
							exe_moving_leters(ch);
						}
						if (mode == wims_mode::write)
						{
							if (symbols::is_enter(ch))
							{
								enter_move();
							}
							else if(symbols::is_normal_symbol(ch))
							{
								auto picture_pos = get_picture_pos(cursor::get_cursor_pos());
								if (!picture.is_in_picture(points::position(picture_pos)))
								{
									enter_move();
								}
								if (picture.is_in_picture(points::position(picture_pos)))
								{
									picture_pos = get_picture_pos(cursor::get_cursor_pos());
									auto& pixel = picture.get_pixel(points::position(picture_pos));
									pixel.set(ch);
									pixel.draw();
								}
							}
							else if (symbols::is_backspace(ch))
							{
								if (!move_cursor(direction::left))
								{
									if (move_cursor(direction::up))
									{
										while (move_cursor(direction::right))
										{

										}
										move_cursor(direction::left);
									}
								}
								auto picture_pos = get_picture_pos(cursor::get_cursor_pos());
								auto& pixel = picture.get_pixel(points::position(picture_pos));
								pixel.set(' ');
								pixel.draw();
								move_cursor(direction::left);
							}
						}
						if (mode == wims_mode::draw)
						{
							if (symbols::is_moving_leter(ch) || symbols::is_moving_number(ch))
							{
								paint_pixel();
								exe_moving_numbers(ch);
								exe_moving_leters(ch);
							}
						}
					}
				}
			}
		};
	}
}

void symbols_finder()
{
	while (true)
	{
		char ch = io::getch_();
		io::putstr_(to_string((int)ch) + ' ');
	}
}

int main(int argc, char* argv[])
{
	console::init_console_func();
	screen::open_new_screen();
	while (!io::is_hit_())
	{

	}
	io::clear_in_buffer();
	//
	if (argc == 1)
	{
		io::putstr_("Hello, I'm builder of wims");
	}
	else
	{
		try
		{
			std::string name = argv[argc - 1];
			bool need_create = false;
			size_t width = 40, height = 40;
			bool use_Double_Pixel = false;//true if use Double_Pixel
			size_t pixel_type = 0;// 0 - 16 colors, 1 - 8 bit, 3 - rgb
			for (int i = 1; i < argc - 1; i++)
			{
				std::string arg = argv[i];
				if (arg == "-w" || arg == "--width")
				{
					if (argc - 1 == i + 1)
					{
						io::putstr_("Error! Wrong flags using!");
						std::exit(1);
					}
					width = (size_t)std::stoi(argv[++i]);
					need_create = true;
				}
				else if (arg == "-h" || arg == "--height")
				{
					if (argc - 1 == i + 1)
					{
						io::putstr_("Error! Wrong flags using!");
						std::exit(1);
					}
					height = (size_t)std::stoi(argv[++i]);
					need_create = true;
				}
				else if (arg == "-d" || arg == "--double")
				{
					use_Double_Pixel = true;
				}
				else if (arg == "-rgb" || arg == "-8bit" || arg == "-16")
				{
					if (arg == "-16")
					{
						pixel_type = 0;
					}
					else if (arg == "-8bit")
					{
						pixel_type = 1;
					}
					if (arg == "-rgb")
					{
						pixel_type = 2;
					}
				}
				else
				{
					io::putstr_("Error! Wrong flags using!");
					return 1;
				}
			}
			//
			if (use_Double_Pixel)
			{
				switch (pixel_type)
				{
				case 0:
					if (need_create)
					{
						Picture<pixels::Double_Pixel<pixels::Console_Pixel_16>> a(name, width, height);
						wims_src::wims<Picture<pixels::Double_Pixel<pixels::Console_Pixel_16>>> program(a);
						program.start();
					}
					else
					{
						Picture<pixels::Double_Pixel<pixels::Console_Pixel_16>> a(name);
						wims_src::wims<Picture<pixels::Double_Pixel<pixels::Console_Pixel_16>>> program(a);
						program.start();
					}
					break;
				default:
					io::putstr_("Error! Wrong flags using!");
					return 1;
				}
			}
			else
			{
				switch (pixel_type)
				{
				case 0:
					if (need_create)
					{
						Picture<pixels::Console_Pixel_16> a(name, width, height);
						wims_src::wims<Picture<pixels::Console_Pixel_16>> program(a);
						program.start();
					}
					else
					{
						Picture<pixels::Console_Pixel_16> a(name);
						wims_src::wims<Picture<pixels::Console_Pixel_16>> program(a);
						program.start();
					}
					break;
				default:
					io::putstr_("Error! Wrong flags using!");
					return 1;
				}
			}
		}
		catch (std::exception& ex)
		{
			io::putstr_(ex.what());
		}
	}
	//
	screen::close_screen();
	console::end_of_work_console_func();
}