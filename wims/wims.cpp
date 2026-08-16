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