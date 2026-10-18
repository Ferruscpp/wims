#pragma once
#include "console_func.h"
#include <vector>
#include <deque>
#include <functional>
#include "ferruscpp_media.h"
#include <type_traits>
#pragma comment(linker, "/STACK:16777216")

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

		bool is_tab(const char& ch)
		{
			return ch == '\t';
		}

		bool is_esc(char ch)
		{
			return ch == 27;
		}

		bool is_backspace(const char& ch)
		{
#if  defined(_WIN32) || defined(_WIN64)
			return ch == 8;
#elif defined(__linux__)
			return ch == 127;
#endif
		}

		bool is_backtick(const char& ch)
		{
			return ch == '`';
		}

		bool is_arrow(const char& ch, wims_src::direction*& d)//can delete somesymbols
		{
			using namespace wims_src;
#if  defined(_WIN32) || defined(_WIN64)
			if (ch == -32 || ch == 224)
			{
				if (!io::is_hit_())
				{
					return false;
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
					return false;
				}
				return true;
			}
			return false;
#elif defined(__linux__)
			if (ch == 27)
			{
				if (!io::is_hit_())
				{
					return false;
				}
				char ch_ = io::getch_();
				if (ch_ == 91)
				{
					if (!io::is_hit_())
					{
						return false;
					}
					ch_ = io::getch_();
					if (ch_ == 65)
					{
						d = new direction(direction::up);
					}
					if (ch_ == 66)
					{
						d = new direction(direction::down);
					}
					if (ch_ == 67)
					{
						d = new direction(direction::right);
					}
					if (ch_ == 68)
					{
						d = new direction(direction::left);
					}
					else
					{
						return false;
					}
					return true;
				}
				else
				{
					return false;
				}
			}
			return false;
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

	namespace figures
	{
		void draw_frame(points::position cur_pos, size_t size_x, size_t size_y, colors::c16 color, bool is_double = false)
		{
			std::string single_elem = " ";
			if (is_double)
			{
				single_elem += ' ';
			}
			colors::set_color_16(colors::get_foreground_basic_color(), color);
			cursor::set_cursor_pos(cur_pos.x, cur_pos.y);
			for (size_t x = 0; x < size_x; ++x)
			{
				io::putstr_(single_elem);
			}
			for (size_t y = 1; y <= size_y - 2; y++)
			{
				cursor::set_cursor_pos(cur_pos.x, cur_pos.y + y);
				io::putstr_(single_elem);
				cursor::set_cursor_pos(cur_pos.x + (size_x - 1) * single_elem.size(), cur_pos.y + y);
				io::putstr_(single_elem);
			}
			cursor::set_cursor_pos(cur_pos.x, cur_pos.y + size_y - 1);
			for (size_t x = 0; x < size_x; ++x)
			{
				io::putstr_(single_elem);
			}
		}

	}

	namespace wims_src
	{
		template<typename Picture_>
		class wims
		{
		private:
			using pixel_type = typename Picture_::pixel_type;

			class Base_Panel
			{
			protected:
				enum base_panel_objects : size_t
				{
					brushes_switch,
					counter
				};
				//parent
				wims<Picture_>& parent;
				//panel
				Picture<pixels::Console_Pixel_16> panel{ "brushes_panel" };
				size_t panel_x_size, panel_y_size;
				//vectors
				std::vector<size_t> y_pos;
				std::deque<std::function<bool(char)>> processes;
				//mode of wotk
				bool is_open = false, is_focusing_on_panel = false;
				size_t setting_mode = base_panel_objects::brushes_switch;
				size_t brushes_type_switch = 0;

				virtual void draw_setting_switch() const
				{
					using namespace colors;
					set_color_16(get_foreground_basic_color(), c16{ 14 });
					if (setting_mode == base_panel_objects::brushes_switch)
					{
						cursor::set_cursor_pos(parent.screen_size.first - panel_x_size + 1, y_pos[brushes_switch]);
						io::putstr_(" ");
					}
				}
				void draw_frame_for_brushes(bool is_delete = false) const
				{
					size_t x = parent.screen_size.first - panel_x_size + 2;
					colors::c16 color{ colors::get_background_basic_color() };
					if (!is_delete)
					{
						color =	13;
					}
					figures::draw_frame(points::position{ x + (brushes_type_switch % 4) * 10, y_pos[base_panel_objects::brushes_switch] + (brushes_type_switch / 4) * 5 }, 6, 6, color, true);
				}

				virtual bool processing_switch_setting_mode(char ch)
				{
					if (symbols::is_backspace(ch))
					{
						/*
						if (setting_mode == base_panel_objects::brushes_switch)
						{
							//do nothing because now we have only one mode in base panel
						}
						else
						{
							return false;
						}
						draw_setting_switch();
						return true;
						*/
					}
					return false;
				}
				virtual bool processing_switch(char ch)
				{
					if (symbols::is_enter(ch))
					{
						if (setting_mode == base_panel_objects::brushes_switch)
						{
							draw_frame_for_brushes(true);
							++brushes_type_switch;
							if (brushes_type_switch == 3)
							{
								brushes_type_switch = 0;
							}
							draw_frame_for_brushes();
						}
						else
						{
							return false;
						}
						return true;

					}
					return false;
				}
			public:

				Base_Panel(wims<Picture_>& parent_) : parent(parent_)
				{
					y_pos.push_back(2);
					panel_x_size = panel.get_size().first;
					panel_y_size = panel.get_size().second;
					processes.push_back([this](char c)
					{
						return this->processing_switch_setting_mode(c);
					});
					processes.push_back([this](char c)
					{
						return this->processing_switch(c);
					});
				}

				bool get_is_open() const
				{
					return is_open;
				}
				bool get_is_focusing_on_panel() const
				{
					return is_focusing_on_panel;
				}

				virtual void draw() const
				{
					panel.draw(points::position{ parent.screen_size.first - panel_x_size, 0U });
					//draw setting switch
					draw_setting_switch();
					//draw frame for brushes
					draw_frame_for_brushes();
				}

				void run()
				{
					is_open = true;
					is_focusing_on_panel = true;
					parent.draw_ui();
					char ch = io::getch_();
					while (!symbols::is_tab(ch))
					{
						for (auto& proc : processes)
						{
							if (proc(ch))
								break;
						}
						ch = io::getch_();
					}
					if (symbols::is_tab(ch))
					{
						close();
					}
				}

				void close()
				{
					is_open = false;
					parent.draw_ui();
				}
			};

			Picture_& picture;
			Base_Panel panel{ *this };
			std::pair<size_t, size_t> pixel_size;
			std::pair<size_t, size_t> screen_size;
			wims_mode mode = wims_mode::move;
			std::pair<size_t, size_t> start_pos = { 1, 1 };
			bool need_set_foreground = false, need_set_background = false;
			pixel_type paint;
			std::pair<size_t, size_t> get_picture_pos(std::pair<size_t, size_t> cursor_pos)
			{
				return { (cursor_pos.first - start_pos.first) / pixel_size.first, (cursor_pos.second - start_pos.second) / pixel_size.second };
			}

			void console_clear_line()
			{
				io::print_colored_line(screen_size.second - 1, colors::c16(0));
				cursor::set_cursor_pos(0, screen_size.second - 1);
				colors::set_color_16(colors::c16(15), colors::c16(0));
			}
			void console_open()
			{
				io::print_colored_line(screen_size.second - 2, colors::c16(15));
				console_clear_line();
			}
			void console_close()
			{
				io::print_colored_line(screen_size.second - 2, colors::c16(0));
				io::print_colored_line(screen_size.second - 1, colors::c16(0));
			}
			bool console_run_single_command(char& ch)
			{
				if (ch == ':')
				{
					console_clear_line();
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
					else if (command == "q" || command == "quit")
					{
						return true;
					}
					else if (command == "sq" || command == "save&quit" || command == "save quit" || command == "save and quit")
					{
						picture.upload();
						return true;
					}
					else if (command == "d" || command == "download")
					{
						console_clear_line();
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
						console_clear_line();
						set_paint<pixel_type>();
					}
					else if (command == "show_colors" || command == "show colors")
					{
						console_clear_line();
						colors::c16 showing_color;
						for (int i = 0; i < 16; i++)
						{
							showing_color = colors::c16(i);
							colors::set_color_16(colors::c16(15), showing_color);
							io::putstr_(" ");
							colors::set_color_16(colors::c16(15), colors::c16(0));
							io::putstr_(std::to_string(i) + " ");
						}
						colors::set_color_16(colors::c16(15), colors::c16(0));
						io::getch_();
					}
					//
					console_clear_line();
				}
				else
				{
					console_clear_line();
					io::putstr_("[ESC]");
				}
				ch = io::getch_();
				return false;
			}
			bool console_start()
			{
				char ch;
				console_open();
				ch = io::getch_();
				while (ch != 27)
				{
					if (console_run_single_command(ch))
					{
						return true;
					}
				}
				console_close();
				draw_ui();
				return false;
			}


			void draw_markings()
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
			void draw_ui()
			{
				screen::clear_screen();
				draw_markings();
				draw_picture();
				if (panel.get_is_open())
				{
					panel.draw();
				}
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
			void move_enter()
			{
				if (move_cursor(direction::down))
				{
					while (move_cursor(direction::left))
					{

					}
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
					console_clear_line();
					io::putstr_("Invalid argument");
					console::wait(500);
					return -1;
				}
				return number;
			}
			template<typename Pixel_Type>
			void set_paint()
			{
				if constexpr (std::is_same_v<Pixel_Type, pixels::Console_Pixel_16>)
				{
					io::putstr_("foreground: ");
					int foreground = scan_number_in_console_mode_for_c16();
					if (foreground != -1)
					{
						paint.set(colors::c16(foreground), paint.get_background());
						need_set_foreground = true;
					}
					else
					{
						need_set_foreground = false;
					}
					console_clear_line();
					io::putstr_("background: ");
					int background = scan_number_in_console_mode_for_c16();
					if (background != -1)
					{
						paint.set(paint.get_foreground(), colors::c16(background));
						need_set_background = true;
					}
					else
					{
						need_set_background = false;
					}
				}
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
				while (true)
				{
					ch = io::getch_();
					direction* d = nullptr;
					if (symbols::is_arrow(ch, d))
					{
						move_cursor(*d);
					}
					else
					{
						if (symbols::is_esc(ch) || symbols::is_tab(ch))
						{
							auto cursor_pos = cursor::get_cursor_pos();
							if (symbols::is_esc(ch))
							{
								if (console_start())
								{
									return;
								}
							}
							else if (symbols::is_tab(ch))
							{
								panel.run();
							}
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
									move_enter();
								}
								else if (symbols::is_normal_symbol(ch))
								{
									auto picture_pos = get_picture_pos(cursor::get_cursor_pos());
									if (!picture.is_in_picture(points::position(picture_pos)))
									{
										move_enter();
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
			}
		};
	}
}