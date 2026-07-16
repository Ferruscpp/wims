#include "console_func.h"
using namespace std;

int main()
{
	init_console_func();
	string a = "Is this shit work? ";
	a += get();
	a += '\n';
	put(a);
	string b = "it is realy intresting: ";
	b += get();
	string c = "realy? ";
	c += get();
	c += '\n';
	put(c);
	put(b);
	end_of_work_console_func();
}