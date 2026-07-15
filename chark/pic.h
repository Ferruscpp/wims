#pragma once

#include <iostream>
#include <fstream>
#include "console_func.h"
using namespace std;

class File_not_found_exception : public exception
{
private:
	string massage = "Error: file not found.\nFunction that called this exception: ";
public:
	File_not_found_exception(string func)
	{
		massage += func;
	}

	const char* what() const noexcept override
	{
		return massage.c_str();
	}
};

struct position
{
	int cur_pos_x, cur_pos_y;
	position(int cur_pos_x_, int cur_pos_y_) : cur_pos_x(cur_pos_x_), cur_pos_y(cur_pos_y_)
	{
		check_position(cur_pos_x, cur_pos_y);
	}
};

struct window4
{
	int cur_pos_x1, cur_pos_y1, cur_pos_x2, cur_pos_y2;
	window4(int cur_pos_x1_, int cur_pos_y1_, int cur_pos_x2_, int cur_pos_y2_) : cur_pos_x1(cur_pos_x1_), cur_pos_y1(cur_pos_y1_), cur_pos_x2(cur_pos_x2_), cur_pos_y2(cur_pos_y2_)
	{
		check_position(cur_pos_x1, cur_pos_y1);
		check_position(cur_pos_x2, cur_pos_y2);
		if (cur_pos_x2 < cur_pos_x1 || cur_pos_y2 < cur_pos_y1)
		{
			throw Screen_Exception();
		}
	}
};

class Picture
{
protected:
	string picture_name;
	string folder_name = "./Pictures/";
	const string symbol_name = "_symbol.pic";
	const string color_foreground_name = "_color_foreground.pic";
	const string color_background_name = "_color_background.pic";
	int size_x = 80;
	int size_y = 25;
	// pure vertual
	virtual void draw_pixel() = 0;
	virtual void save_online() = 0;
	virtual void save_ofline() = 0;
	//
	string get_symbol_file_name() const;
	string get_foreground_file_name() const;
	string get_background_file_name() const;
	//
	void update_path();
public:
	Picture(string name_);//find and auto open
	Picture(string new_name, int x, int y);//create new picture
	void draw();
	void draw(position cur_pos);
	void seg_draw(position pic_pos, window4 cur_pos);
	void rename(string new_name);
	void change_folder(string new_folder);
	~Picture();
};

class Picture_RGB
{

};

class Picture_8bit
{

};

class Picture_16
{

};