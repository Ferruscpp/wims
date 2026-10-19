#include "wims.h"
using namespace std;
using namespace ferruscpp;

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
	io::putstr_("Hit to start...");
	while (!io::is_hit_())
	{

	}
	io::clear_in_buffer();
	screen::clear_screen();
	screen::update_screen_size();
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
			colors::set_color_16(colors::get_foreground_basic_color(), colors::get_background_basic_color());
			io::putstr_(ex.what());
			console::wait(10000);
		}
	}
	//
	screen::close_screen();
	console::end_of_work_console_func();
}