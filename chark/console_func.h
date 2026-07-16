#pragma once

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

using namespace std;

#if _WIN32
#include <windows.h>
#include <conio.h>
int put(string& str)
{
	for (int i = 0; i < str.size(); i++)
	{
		if (!_putch(str[i]))
		{
			return EOF;
		}
	}
	return str[str.size() - 1];
}
int get()
{
	return _getch();
}
class TerminalController
{
public:
	TerminalController()
	{
		
	}
	~TerminalController()
	{

	}
};

void init_console_func()
{

}

void end_of_work_console_func()
{

}
#elif __linux__
#include <unistd.h>
#include <termios.h>
//------------------------------------------------------
class TerminalController
{
private:
	inline static int counter = 0;
	termios newin, oldin;
public:
	TerminalController()
	{
		counter++;
		tcgetattr(STDIN_FILENO, &oldin);
		newin = oldin;
		newin.c_lflag &= ~(ICANON | ECHO);
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &newin);
	}
	static int get_info()
	{
		return counter;
	}
	~TerminalController()
	{
		tcsetattr(STDIN_FILENO, TCSANOW, &oldin);
	}
};
//-----------------------------------------------------
class Not_Init_Console_Error : public exception
{
private:
	string massage = "Error: console can be crashed!!!";
public:
	Not_Init_Console_Error()
	{

	}
	const char* what() const noexcept override
	{
		return massage.c_str();
	}
};
//-----------------------------------------------------
int put(string& str)
{
	return write(1, str.c_str(), str.size());
}
//-----------------------------------------------------
int get()
{
	if (TerminalController::get_info() != 1)
	{
		throw Not_Init_Console_Error();
	}
	char ch;
	read(STDIN_FILENO, &ch, 1);
	return (int)ch;
}
//-----------------------------------------------------
TerminalController* tc_;

void init_console_func()
{
	tc_ = new TerminalController;
}

void end_of_work_console_func()
{
	tc_->~TerminalController();
}
//-----------------------------------------------------
#else
#error: "Unknown OS"
#endif

const int low_screen_x(0);
const int up_screen_x(201);
const int low_screen_y(0);
const int up_screen_y(52);

class Screen_Exception : public exception
{
private:
	string massage = "Exite out of screen\n";
public:
	Screen_Exception()
	{
	}
	const char* what() const noexcept override
	{
		return massage.c_str();
	}
};

size_t get_cursor_x()
{
	string h = "\033[6n";
	put(h);
	
}

size_t get_cursor_y()
{

}

void check_position(int x, int y)
{
	if (x < low_screen_x || up_screen_x < x)
	{
		throw Screen_Exception();
	}
	if (y < low_screen_y || up_screen_y < y)
	{
		throw Screen_Exception();
	}
}

void print_rgb(char r, char g, char b)
{

}

void print_8bit()
{

}

void print_16()
{

}

void set_cursor_pos(size_t x, size_t y)
{
	check_position(x, y);
	string h = "\033[" + to_string(x + 1) + ';' + to_string(y + 1) + 'H';
	put(h);
}