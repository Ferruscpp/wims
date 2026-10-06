#include "console_func.h"
#include "ferruscpp_media.h"
#pragma comment(linker, "/STACK:16777216")
using namespace std;
using namespace ferruscpp;
using namespace ferruscpp::pixels;

namespace ferruscpp
{
	namespace wims
	{
		template<typename picture_type>
		void move_up(picture_type& picture)
		{
			auto cur_pos = console::get_cursor_pos();
			--cur_pos.second;
			if (picture.is_in_picture({ cur_pos.first, cur_pos.second }))
			{
				console::set_cursor_pos(cur_pos.first, cur_pos.second);
			}
		}

		template<typename picture_type>
		void move_left(picture_type& picture)
		{
			auto cur_pos = console::get_cursor_pos();
			--cur_pos.first;
			if (picture.is_in_picture({ cur_pos.first, cur_pos.second }))
			{
				console::set_cursor_pos(cur_pos.first, cur_pos.second);
			}
		}

		template<typename picture_type>
		void move_down(picture_type& picture)
		{
			auto cur_pos = console::get_cursor_pos();
			++cur_pos.second;
			if (picture.is_in_picture({ cur_pos.first, cur_pos.second }))
			{
				console::set_cursor_pos(cur_pos.first, cur_pos.second);
			}
		}

		template<typename picture_type>
		void move_right(picture_type& picture)
		{
			auto cur_pos = console::get_cursor_pos();
			++cur_pos.first;
			if (picture.is_in_picture({ cur_pos.first, cur_pos.second }))
			{
				console::set_cursor_pos(cur_pos.first, cur_pos.second);
			}
		}

		template<typename picture_type>
		void start(picture_type& picture)
		{
			picture.draw();
			console::set_color_16(console::get_foreground_basic_color(), console::get_background_basic_color());
			while (true)
			{
				char ch = console::getch_();
				if (ch == '\033')// парсинг комманд 
				{

				}
				if (ch == 'w' || ch == 'W')
				{
					move_up(picture);
				}
				if (ch == 'a' || ch == 'A')
				{
					move_left(picture);
				}
				if (ch == 's' || ch == 'S')
				{
					move_down(picture);
				}
				if (ch == 'd' || ch == 'D')
				{
					move_right(picture);
				}
			}
		}
	}
}

int main(int argc, char* argv[])
{
	console::init_console_func();
	while (!console::is_hit_())
	{

	}
	console::clear_in_buffer();
	//
	if (argc == 1)
	{
		cout << "Hello, I'm builder of wims";
	}
	else
	{
		try
		{
			std::string name = argv[argc - 1];
			bool need_create = false;
			size_t width = 40, height = 40;
			bool use_Pixel = false;//true if use Pixel
			size_t pixel_type = 0;// 0 - 16 colors, 1 - 8 bit, 3 - rgb
			for (int i = 1; i < argc - 1; i++)
			{
				std::string arg = argv[i];
				if (arg == "-w" || arg == "--width")
				{
					if (argc - 1 == i + 1)
					{
						console::putstr_("Error! Wrong flags using!");
						std::exit(1);
					}
					width = (size_t)std::stoi(argv[++i]);
					need_create = true;
				}
				else if (arg == "-h" || arg == "--height")
				{
					if (argc - 1 == i + 1)
					{
						console::putstr_("Error! Wrong flags using!");
						std::exit(1);
					}
					height = (size_t)std::stoi(argv[++i]);
					need_create = true;
				}
				else if (arg == "-d" || arg == "--double")
				{
					use_Pixel = true;
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
					console::putstr_("Error! Wrong flags using!");
					std::exit(1);
				}
			}
			//
			if (use_Pixel)
			{
				switch (pixel_type)
				{
				case 0:
					if (need_create)
					{
						Picture<Pixel<Console_Pixel_16>> a(name, width, height);
						wims::start(a);
					}
					else
					{
						Picture<Pixel<Console_Pixel_16>> a(name);
						wims::start(a);
					}
				/*case 1:
					if (need_create)
					{
						Picture<Pixel<Console_Pixel_8bit>> a(name, width, height);
						wims::start(a);
					}
					else
					{
						Picture<Pixel<Console_Pixel_8bit>> a(name);
						wims::start(a);
					}
				case 2:
					if (need_create)
					{
						Picture<Pixel<Console_Pixel_rgb>> a(name, width, height);
						wims::start(a);
					}
					else
					{
						Picture<Pixel<Console_Pixel_rgb>> a(name);
						wims::start(a);
					}*/
				default:
					console::putstr_("Error! Wrong flags using!");
					std::exit(1);
				}
			}
			else
			{
				switch (pixel_type)
				{
				case 0:
					if (need_create)
					{
						Picture<Console_Pixel_16> a(name, width, height);
						wims::start(a);
					}
					else
					{
						Picture<Console_Pixel_16> a(name);
						wims::start(a);
					}
				/*case 1:
					if (need_create)
					{
						Picture<Console_Pixel_8bit> a(name, width, height);
						wims::start(a);
					}
					else
					{
						Picture<Console_Pixel_8bit> a(name);
						wims::start(a);
					}
				case 2:
					if (need_create)
					{
						Picture<Console_Pixel_rgb> a(name, width, height);
						wims::start(a);
					}
					else
					{
						Picture<Console_Pixel_rgb> a(name);
						wims::start(a);
					}*/
				default:
					console::putstr_("Error! Wrong flags using!");
					std::exit(1);
				}
			}
		}
		catch (std::exception& ex)
		{
			console::putstr_(ex.what());
		}
	}
	//
	console::end_of_work_console_func();
}