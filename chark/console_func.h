#pragma once

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

using namespace std;

#if _WIN32
	#include <windows.h>
#elif __linux__

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

void print_rgb(int r, int g, int b)
{

}

void print_8bit()
{

}

void print_16()
{

}