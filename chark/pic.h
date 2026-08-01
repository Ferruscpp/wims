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
	File_not_found_exception()
	{

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

//need realizatin
class Pixel_rgb
{
private:

public:

};

//need realizetion
class Pixel_8bit
{
private:

public:

};

class Console_Pixel_16
{
private:
	char symbol;
	c16 foreground, background;
public:
	Console_Pixel_16()
	{

	}
	Console_Pixel_16(char symbol_, c16 foreground_, c16 background_) : symbol(' '), foreground(get_foreground_bassic_color()), background(get_background_bassic_color())
	{

	}
	void draw() const
	{
		set_color_16(foreground, background);
		string h;
		h += symbol;
		putstr_(h);
	}
	void set(char symbol_)
	{
		symbol = symbol_;
	}
	void set(c16 foreground_, c16 background_)
	{
		foreground = foreground_;
		background = background_;
	}
	void set(char symbol_, c16 foreground_, c16 background_)
	{
		symbol = symbol_;
		foreground = foreground_;
		background = background_;
	}
	friend istream& operator>>(istream& in, Console_Pixel_16& pixel)
	{
		in >> noskipws;
		in >> pixel.symbol;
		in >> skipws;
		in >> pixel.foreground >> pixel.background;
		return in;
	}
	friend ostream& operator<<(ostream& out, const Console_Pixel_16& pixel)
	{
		out << pixel.symbol << pixel.background << pixel.foreground;
		return out;
	}
};

template<typename T>
class Pixel
{
public:
	T first, second;
	Pixel() : first(' ', get_foreground_bassic_color(), get_background_bassic_color()), second(' ', get_foreground_bassic_color(), get_background_bassic_color())
	{
		
	}
	Pixel(T first_, T second_) : first(first_), second(second_)
	{

	}
	void draw() const
	{
		first.draw();
		second.draw();
	}
	friend istream& operator>>(istream& in, Pixel& pixel)
	{
		in >> noskipws;
		in >> pixel.symbol;
		in >> skipws;
		in >> pixel.foreground >> pixel.background;
		return in;
	}
	friend ostream& operator<<(ostream& out, const Pixel& pixel)
	{
		out << pixel.symbol << pixel.background << pixel.foreground;
		return out;
	}
	~Pixel()
	{

	}
};

template<typename T>
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
	//надо добавить всякие там таблицы
	class Picture_Exception : public exception
	{
	private:
		string message = "Exite out of picture range\n";
	public:
		Picture_Exception();
		const char* what() const noexcept override
		{
			return message.c_str();
		}
	};
	//
	string get_symbol_file_name() const
	{
		return folder_name + picture_name + symbol_name;
	}
	string get_foreground_file_name() const
	{
		return folder_name + picture_name + color_foreground_name;
	}
	string get_background_file_name() const
	{
		return folder_name + picture_name + color_background_name;
	}
	//save
	void dounload() const;
	void upload();
	//
	void build_files()
	{
		ofstream symbol_out(get_symbol_file_name());
		ofstream foreground_out(get_foreground_file_name());
		ofstream background_out(get_background_file_name());
		symbol_out.close();
		foreground_out.close();
		background_out.close();
	}
	//
	void update_path()
	{
		int seporator_position = 0;
		string new_picture_name;
		for (int i = picture_name.size() - 1; i >= 0; i--)
		{
			if (picture_name[i] == '/')
			{
				seporator_position = i;
				break;
			}
		}
		for (int i = 0; i < picture_name.size(); i++)
		{
			if (i < seporator_position)
			{
				folder_name += picture_name[i];
			}
			else if (seporator_position < i)
			{
				new_picture_name += picture_name[i];
			}
		}
		picture_name = new_picture_name;
	}
public:
	Picture(string name_) : picture_name(name_)
	{
		update_path();
		ifstream in(get_symbol_file_name());
		if (!in.is_open())
		{
			throw File_not_found_exception();
		}
		in.close();
		upload();
	}
	Picture(string new_name, size_t x, size_t y) : size_x(x), size_y(y), picture_name(new_name)
	{
		update_path();
		build_files();
	}
	void draw()
	{
		check_position(get_cursor_x() + size_x, get_cursor_y() + size_y);
		for (size_t y = 0; y < size_y; ++y)
		{
			for (size_t x = 0; x < size_x; ++x)
			{
				draw_pixel(x, y);
			}
		}
	}
	void draw(position cur_pos)
	{
		set_cursor_pos(cur_pos.x, cur_pos.y);
		draw();
	}
	void seg_draw(position pic_pos, window4 cur_pos)
	{
		if (size_x <= pic_pos.x || size_y <= pic_pos.y)
		{
			throw Picture_Exception();
		}
		set_cursor_pos(cur_pos.x1, cur_pos.y1);
		for (size_t i = 0; i < 0; i++)
		{

		}
	}
	void rename(string new_name)
	{

	}
	void change_folder(string new_folder)
	{

	}
	~Picture()
	{

	}
};