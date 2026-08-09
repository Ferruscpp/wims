#include "console_func.h"
#include "pic.h"
using namespace std;

int main()
{
	init_console_func();
	open_new_screen();
	switch_to_small_screen();
	//c16 a(15), b(6);
	//cout << ' ' << a << b;
	while (!is_hit_())
	{
		
	}
	clear_in_buffer();
	try
	{
		bool build;
		Picture<Pixel<Console_Pixel_16>> a("Second_picture"); build = false;
		//Picture<Pixel<Console_Pixel_16>> a("Second_picture", 5, 8); build = true;
		if (build)
		{
			for (int x = 0; x < 5; x++)
			{
				for (int y = 0; y < 8; y++)
				{
					a.get_pixel({ x, y }).pixel.set(15, 6);
				}
			}
		}
		position pic_pos(0, 3);
		window4 cur_pos = {15, 4, 20, 7};
		a.seg_draw(pic_pos, cur_pos);
		//a.draw();
		a.~Picture();
		//putstr_("This was done!!!");
	}
	catch (exception)
	{
		cout << "ERROR!!!\n";
	}
	while (!is_hit_())
	{

	}
	close_screen();
	end_of_work_console_func();
}