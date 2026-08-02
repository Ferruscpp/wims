#pragma once

#include <iostream>
#include <fstream>
#include "console_func.h"
using namespace std;


class File_not_found_exception : public exception
{
private:
	string massage = "Error: file not found.!!!";
public:
	File_not_found_exception()
	{

	}

	const char* what() const noexcept override
	{
		return massage.c_str();
	}
};

struct position//this is shit
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
	T pixel;
	Pixel() : pixel(' ', get_foreground_bassic_color(), get_background_bassic_color())
	{
		
	}
	Pixel(T pixel_) : pixel(pixel_)
	{

	}
	void draw() const
	{
		pixel.draw();
		pixel.draw();
	}
	friend istream& operator>>(istream& in, Pixel& pixel)
	{
		in >> pixel.pixel;
		return in;
	}
	friend ostream& operator<<(ostream& out, const Pixel& pixel)
	{
		out << pixel.pixel;
		return out;
	}
	~Pixel()
	{

	}
};

template<typename T>
class Picture
{
private:
	string folder_name = "./Pictures";
	string picture_name;
	const string end_name = ".pic";
	size_t size_x = 80;
	size_t size_y = 25;
	T* pixel_table[100][100];
	//
	class Picture_Exception : public exception
	{
	private:
		string message = "Exite out of picture range!!!";
	public:
		Picture_Exception();
		const char* what() const noexcept override
		{
			return message.c_str();
		}
	};
	//
	string get_file_name() const
	{
		return folder_name + '/' + picture_name + end_name;
	}
	void dounload() const
	{
		ifstream in(get_file_name());
		if (!in.is_open())
		{
			throw File_not_found_exception();
		}
		for (size_t y = 0; y < size_y; ++y)
		{
			for (size_t x = 0; x < size_x; ++x)
			{
				in >> *pixel_table[x][y];
			}
		}
	}
	void upload()
	{
		ofstream out(get_file_name());
		for (size_t y = 0; y < size_y; ++y)
		{
			for (size_t x = 0; x < size_x; ++x)
			{
				out << *pixel_table[x][y];
			}
		}
		out.close();
		return;
	}
	bool exist()
	{
		ifstream in(get_file_name());
		bool answer = in.is_open();
		in.close();
		return answer;
	}
	void build_file()
	{
		ofstream out(get_file_name());
		out.close();
	}
	//
	void update_path()
	{
		int seporator_position = -1;
		string new_picture_name;
		for (int i = picture_name.size() - 1; i >= 0; --i)
		{
			if (picture_name[i] == '/')
			{
				seporator_position = i;
				break;
			}
		}
		if (seporator_position > 0)
		{
			folder_name += '/';
		}
		for (int i = 0; i < picture_name.size(); ++i)
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
	//
	void check_picture_position(position pic_pos)
	{
		if (pic_pos.x < 0 || size_x <= pic_pos.x)
		{
			throw Picture_Exception();
		}
		if (pic_pos.y < 0 || size_y <= pic_pos.y)
		{
			throw Picture_Exception();
		}
	}
public:
	Picture(string name_) : picture_name(name_)
	{
		update_path();
		if (!exist())
		{
			throw File_not_found_exception();
		}
		dounload();
	}
	Picture(string new_name, size_t x, size_t y) : size_x(x), size_y(y), picture_name(new_name)
	{
		update_path();
		build_file();
		for (size_t y = 0; y < size_y; ++y)
		{
			for (size_t x = 0; x < size_x; ++x)
			{
				pixel_table[x][y] = new T;
			}
		}
	}
	void draw()
	{
		check_position(get_cursor_x() + size_x, get_cursor_y() + size_y);
		for (size_t y = 0; y < size_y; ++y)
		{
			for (size_t x = 0; x < size_x; ++x)
			{
				pixel_table[x][y]->draw();
			}
			if (y != size_y - 1)
			{
				putstr_("\n");
			}
		}
	}
	void draw(position cur_pos)
	{
		set_cursor_pos(cur_pos.x, cur_pos.y);
		draw();
	}
	T& get_pixel(position pic_pos)
	{
		//check_picture_position(pic_pos);
		return *(pixel_table[pic_pos.x][pic_pos.y]);
	}
	//TODO
	void seg_draw(position pic_pos, window4 cur_pos)
	{
		
	}
	void rename(string new_name)
	{

	}
	void change_folder(string new_folder)
	{

	}
	//DONE
	~Picture()
	{
		upload();
		for (size_t y = 0; y < size_y; ++y)
		{
			for (size_t x = 0; x < size_x; ++x)
			{
				pixel_table[x][y]->~T();
			}
		}
	}
};