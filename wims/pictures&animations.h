#pragma once

#include <iostream>
#include <fstream>
#include "msoftcon.h"
#include <cstdlib>
#include <string>
using namespace std;

color int_to_color(int color_index);

class Picture
{
protected:
	static int picture_cnt;
	const string picture_name;
	const string _symbol_name = " symbol.txt";
	const string _color_foreground_name = " color foreground.txt";
	const string _color_background_name = " color background.txt";
	int size_x = 80;
	int size_y = 25;
	string _pic_symbol[182];
	int _pic_c_f[182][200];
	int	_pic_c_b[182][200];
	void save_online();
	class Picture_Exception : public exception
	{
	private:
		string message = "Exite out of picture range\n";
	public:
		Picture_Exception()
		{
		}
		const char* what() const noexcept override
		{
			return message.c_str();
		}
	};
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
	class Time_Exception : public exception
	{
	private:
		string massage = "Exite out of time\n";
	public:
		Time_Exception()
		{
		}
		const char* what() const noexcept override
		{
			return massage.c_str();
		}
	};
public:

	Picture();
	Picture(int x, int y);
	Picture(string new_name, int x, int y);
	Picture(string new_name);
	void draw();
	void draw(int cur_pos_x, int cur_pos_y);
	void seg_draw(int x, int y, int cur_pos_x1, int cur_pos_y1, int cur_pos_x2, int cur_pos_y2);
	void rotate_vertically(int cur_pos_x1, int cur_pos_y1, int cur_pos_x2, int cur_pos_y2, int milliseconds, int times);
	void rotate_vertically(int x, int y, int cur_pos_x1, int cur_pos_y1, int cur_pos_x2, int cur_pos_y2, int milliseconds, int times);
	void rotate_horizontally(int cur_pos_x1, int cur_pos_y1, int cur_pos_x2, int cur_pos_y2, int milliseconds, int times);
	void rotate_horizontally(int x, int y, int cur_pos_x1, int cur_pos_y1, int cur_pos_x2, int cur_pos_y2, int milliseconds, int times);
};

class Roll : public Picture
{
	friend void two_rotate_vertically(Roll& a, Roll& b, int milliseconds, int times);
	friend void two_rotate_vertically(Roll& a, Roll& b, int milliseconds_1, int times_1, int milliseconds_2, int times_2);
	friend void tree_rotate_vertically(Roll& a, Roll& b, Roll& c, int milliseconds_1, int times_1, int milliseconds_2, int times_2, int milliseconds_3, int times_3);
private:
	int pic_x;
	int pic_y;
	int cur_pos_x1;
	int cur_pos_y1;
	int cur_pos_x2;
	int cur_pos_y2;
public:
	Roll();
	Roll(Picture new_pic, int x, int y);
	Roll(string new_pic, int x, int y, int _cur_pos_x1, int _cur_pos_y1, int _cur_pos_x2, int _cur_pos_y2);
	Roll(Picture new_pic, int x, int y, int _cur_pos_x1, int _cur_pos_y1, int _cur_pos_x2, int _cur_pos_y2);
	void seg_draw2();
	void seg_draw2(int x, int y);
	void change_x_y(int x, int y);
};

class Time_Exception : public exception
{
private:
	string massege = "Exite out of time\n";
public:
	Time_Exception()
	{
	}
	const char* what() const noexcept override
	{
		return massege.c_str();
	}
};

int gsd(int a, int b);

void two_rotate_vertically(Roll& a, Roll& b, int milliseconds, int times);

void two_rotate_vertically(Roll& a, Roll& b, int milliseconds_1, int times_1, int milliseconds_2, int times_2);

void tree_rotate_vertically(Roll& a, Roll& b, Roll& c, int milliseconds_1, int times_1, int milliseconds_2, int times_2, int milliseconds_3, int times_3);