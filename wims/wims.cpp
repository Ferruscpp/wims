#include "console_func.h"
#include "pic.h"
using namespace std;

int main()
{
	init_console_func();
	switch_to_small_screen();
	set_color_16(get_foreground_basic_color(), get_background_basic_color());
	while (!is_hit_())
	{

	}
	clear_in_buffer();
	try
	{
		bool build, move = false;
		//Picture<Pixel<Console_Pixel_16>> a("Second_picture"); build = false;
		//Picture<Pixel<Console_Pixel_16>> a("Try_new_func", 5, 8); build = true;
		Picture<Pixel<Console_Pixel_16>> a("New_folder/Try_new_func"); build = false;
		//Picture<Pixel<Console_Pixel_16>> a("Second_picture", 5, 8); build = true;
		if (build)
		{
			for (int x = 0; x < 5; x++)
			{
				for (int y = 0; y < 8; y++)
				{
					//a.get_pixel({ x, y }).pixel.set(15, 6);
					if (x % 2 == 0 && y % 2 == 0)
					{
						a.get_pixel({ x, y }).pixel.set(15, 6);
					}
					else if (x % 2 == 1 && y % 2 == 0)
					{
						a.get_pixel({ x, y }).pixel.set(15, 5);
					}
					else if (x % 2 == 0 && y % 2 == 1)
					{
						a.get_pixel({ x, y }).pixel.set(15, 4);
					}
					else
					{
						a.get_pixel({ x, y }).pixel.set(15, 3);
					}
				}
			}
		}
		position pic_pos(0, 3);
		putstr_("\033[?25h");
		window4 cur_pos = { 15, 4, 19, 7 };
		a.seg_draw(pic_pos, cur_pos);
		set_cursor_pos(0, 10);
		a.draw_from(pic_pos);
		set_cursor_pos(7, 10);
		a.draw();
		set_cursor_pos(50, 6);
		//a.expanded_draw(position(69, 25));
		a.expanded_draw({ 0, 0 }, { 50, 6, 69, 25 });
		//
		if (move)
		{
			//a.rename("Try_new_func");
			a.change_folder("./Pictures/New_folder");
		}
		a.~Picture();
	}
	catch (exception &ex)
	{
		set_color_16(15, 0);
		cout << ex.what() << endl;
	}
	while (!is_hit_())
	{

	}
	end_of_work_console_func();
}