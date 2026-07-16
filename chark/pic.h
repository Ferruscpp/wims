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
	int x, y;
	position(int x_, int y_) : x(x_), y(y_)
	{

	}
};

struct window4
{
	int x1, y1, x2, y2;
	window4(int x1_, int y1_, int x2_, int y2_) : x1(x1_), y1(y1_), x2(x2_), y2(y2_)
	{
		check_position(x1, y1);
		check_position(x2, y2);
		if (x2 < x1 || y2 < y1)
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
	size_t size_x = 80;
	size_t size_y = 25;
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
	//
	string get_symbol_file_name() const;
	string get_foreground_file_name() const;
	string get_background_file_name() const;
	// pure vertual
	virtual void draw_pixel(int x, int y) = 0;
	virtual void dounload() const = 0;
	virtual void upload() = 0;
	//
	void build_files();
	//
	void update_path();
public:
	Picture(string name_);//find and auto open
	Picture(string new_name, size_t x, size_t y);//create new picture
	void draw();
	void draw(position cur_pos);
	void seg_draw(position pic_pos, window4 cur_pos);//position in picture and window on screen
	void rename(string new_name);
	void change_folder(string new_folder);
	~Picture();
};

struct RGB
{
	char red, green, blue;
	RGB() : red(0), green(0), blue(0)
	{

	}
	RGB(char r, char g, char b) : red(r), green(g), blue(b)
	{

	}
};

class Picture_RGB : public Picture
{
protected:
	string* symbol;
	RGB** foreground;
	RGB** background;
	void draw_pixel() override;
	void dounload() const override;
	void upload() override;
public:
	Picture_RGB(string name_);
	Picture_RGB(string name_, int x, int y);
};

class Picture_8bit
{

};

class Picture_16
{

};