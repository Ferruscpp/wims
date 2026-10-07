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
		enum direction
		{
			up,
			down,
			left,
			right
		};

		template<typename picture_type>
		bool move_cursor(const picture_type& picture, const std::pair<size_t, size_t>& pixel_size, const direction& d)
		{
			auto cur_pos = console::get_cursor_pos();
			switch (d)
			{
			case direction::up: cur_pos.second -= pixel_size.second; break;
			case direction::down: cur_pos.second += pixel_size.second; break;
			case direction::left: cur_pos.first -= pixel_size.first; break;
			case direction::right: cur_pos.first += pixel_size.first; break;
			}
			if (picture.is_in_picture({ cur_pos.first / pixel_size.first, cur_pos.second / pixel_size.second}))
			{
				console::set_cursor_pos(cur_pos.first, cur_pos.second);
				return true;
			}
			return false;
		}

		template<typename picture_type>
		void start(picture_type& picture)
		{
			std::pair<size_t, size_t> pixel_size = picture.get_pixel_size();
			picture.draw();
			console::set_cursor_pos(pixel_size.first - 1, pixel_size.second - 1);
			console::set_color_16(console::get_foreground_basic_color(), console::get_background_basic_color());
			enum wims_mode
			{
				move,
				write,
			};
			wims_mode mode = wims_mode::move;
			while (true)
			{
				char ch = console::getch_();
				if (ch == '\033')//парсинг комманд 
				{

				}
				if (mode == wims_mode::move)
				{
					if (ch == 'w' || ch == 'W')//up
					{
						move_cursor(picture, pixel_size, direction::up);
					}
					if (ch == 'a' || ch == 'A')//left
					{
						move_cursor(picture, pixel_size, direction::left);
					}
					if (ch == 's' || ch == 'S')//down
					{
						move_cursor(picture, pixel_size, direction::down);
					}
					if (ch == 'd' || ch == 'D')//right
					{
						move_cursor(picture, pixel_size, direction::right);
					}
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
			bool use_Double_Pixel = false;//true if use Double_Pixel
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
					console::putstr_("Error! Wrong flags using!");
					std::exit(1);
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
						Picture<Double_Pixel<Console_Pixel_16>> a(name, width, height);
						wims::start(a);
					}
					else
					{
						Picture<Double_Pixel<Console_Pixel_16>> a(name);
						wims::start(a);
					}
					break;
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
					break;
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