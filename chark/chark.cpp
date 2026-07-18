#include "console_func.h"
using namespace std;

int main()
{
	init_console_func();
	open_new_screen();
	switch_to_small_screen();
	crgb a((char)255, (char)0, (char)0);
	crgb b((char)0, (char)0, (char)255);
	set_rgb(a, b);
	putstr_("shit, it's work");
	wait(10'000);
	close_screen();
	end_of_work_console_func();
}