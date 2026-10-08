#include "console_func.h"
#include "ferruscpp_media.h"
#pragma comment(linker, "/STACK:16777216")
using namespace std;
using namespace ferruscpp;

namespace ferruscpp
{
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

		bool is_normal_symbol(const char& ch)
		{
			return 32 <= ch && ch <= 126;
		}

		void read_and_write(std::string& line)
		{
			char ch = io::getch_();
			while (ch != '\n' && ch != '\r')
			{
				if (is_normal_symbol(ch))
				{
					line += ch;
					io::putstr_(std::string() + ch);
				}
				ch = io::getch_();
			}
		}
	}

	namespace wims
	{
		enum wims_mode
		{
			move,
			write,
			draw
		};

		enum direction
		{
			up,
			down,
			left,
			right
		};

		template<typename Picture_>
		class wims
		{
		private:
			Picture_& picture;
			std::pair<size_t, size_t> pixel_size;
			std::pair<size_t, size_t> screen_size;
			wims_mode mode = wims_mode::move;
			bool is_console_open;

			std::pair<size_t, size_t> get_picture_pos(std::pair<size_t, size_t> cursor_pos)
			{
				return { cursor_pos.first / pixel_size.first, cursor_pos.second / pixel_size.second };
			}

			void draw_picture()
			{
				picture.draw(points::position(0, 0));
				cursor::set_cursor_pos(pixel_size.first - 1, pixel_size.second - 1);
				colors::set_color_16(colors::get_foreground_basic_color(), colors::get_background_basic_color());
			}
			void open_console_mode()
			{
				io::print_colored_line(screen_size.second - 2, colors::c16(15));
				io::print_colored_line(screen_size.second - 1, colors::c16(0));
				cursor::set_cursor_pos(0, screen_size.second - 1);
				colors::set_color_16(colors::c16(15), colors::c16(0));
			}
			void close_console_mode()
			{
				io::print_colored_line(screen_size.second - 2, colors::c16(0));
				io::print_colored_line(screen_size.second - 1, colors::c16(0));
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
				return false;
			}

			void draw_ui()
			{
				if (is_console_open)
				{
					open_console_mode();
				}
				else
				{
					close_console_mode();
				}
				draw_picture();
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
				char ch;
				while (true)
				{
					ch = io::getch_();
					if (ch == 27)//need to switch modes
					{
						auto cursor_pos = cursor::get_cursor_pos();
						open_console_mode();
						ch = io::getch_();
						while (ch != 27)
						{
							if (ch == ':')
							{
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
								//
								open_console_mode();
							}
							else
							{
								open_console_mode();
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
							if (ch == 'w' || ch == 'W')//up
							{
								move_cursor(direction::up);
							}
							if (ch == 'a' || ch == 'A')//left
							{
								move_cursor(direction::left);
							}
							if (ch == 's' || ch == 'S')//down
							{
								move_cursor(direction::down);
							}
							if (ch == 'd' || ch == 'D')//right
							{
								move_cursor(direction::right);
							}
						}
						if (mode == wims_mode::write)
						{
							if (ch == '\n' || ch == '\r')
							{
								if (move_cursor(direction::down))
								{
									while (move_cursor(direction::left))
									{

									}
								}
							}
							else if(io::is_normal_symbol(ch))
							{
								
							}
						}
					}
				}
			}
		};
	}
}

int main(int argc, char* argv[])
{
	console::init_console_func();
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
						wims::wims<Picture<pixels::Double_Pixel<pixels::Console_Pixel_16>>> program(a);
						program.start();
					}
					else
					{
						Picture<pixels::Double_Pixel<pixels::Console_Pixel_16>> a(name);
						wims::wims<Picture<pixels::Double_Pixel<pixels::Console_Pixel_16>>> program(a);
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
						wims::wims<Picture<pixels::Console_Pixel_16>> program(a);
						program.start();
					}
					else
					{
						Picture<pixels::Console_Pixel_16> a(name);
						wims::wims<Picture<pixels::Console_Pixel_16>> program(a);
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
	console::end_of_work_console_func();
}