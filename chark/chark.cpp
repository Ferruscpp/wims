#include "console_func.h"
#include "pic.h"
using namespace std;

int main()
{
	init_console_func();
	open_new_screen();
	switch_to_small_screen();
	Pixel<Console_Pixel_16> a;
	a.first.set('5');
	a.second.set('5');
	//a.draw();
	wait(10'000);
	close_screen();
	end_of_work_console_func();
}