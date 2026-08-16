#pragma once

#include <iostream>
#include <fstream>
#include "console_func.h"
using namespace std;


class File_not_found_exception : public exception
{
private:
	string massage = "Error: file not found!!!";
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
	size_t x, y;
	position() : x(0), y(0)
	{

	}
	position(size_t x_, size_t y_) : x(x_), y(y_)
	{

	}
	position(int x_, int y_) 
	{
		if (x_ < 0 || y_ < 0)
		{
			clog << "Warning: x or y is negative number!" << endl;
		}
		x = x_;
		y = y_;
	}
	explicit position(pair<size_t, size_t> pos) : x(pos.first), y(pos.second)
	{

	}
	operator pair<size_t, size_t>() const
	{
		return make_pair((size_t)x, (size_t)y);
	}
};

struct window4
{
	size_t x1, y1, x2, y2;
private:
	class Window_Exception : exception
	{
	private:
		string massage = "Error: wrong angle position!!!";
	public:
		Window_Exception()
		{

		}

		const char* what() const noexcept override
		{
			return massage.c_str();
		}
	};
	void check_window4()
	{
		if (x2 < x1 || y2 < y1)
		{
			throw Window_Exception();
		}
	}
public:
	window4(size_t x1_, size_t y1_, size_t x2_, size_t y2_) : x1(x1_), y1(y1_), x2(x2_), y2(y2_)
	{
		check_window4();
	}
	window4(position left_up_angle, position right_down_angle) : x1(left_up_angle.x), y1(left_up_angle.y), x2(right_down_angle.x), y2(right_down_angle.y)
	{
		check_window4();
	}
	window4(initializer_list<size_t> list)
	{
		x1 = *list.begin();
		y1 = *(list.begin() + 1);
		x2 = *(list.begin() + 2);
		y2 = *(list.begin() + 3);
	}
	void screen_check() const
	{
		check_position(x1, y1);
		check_position(x2, y2);
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
	Console_Pixel_16(char symbol_, c16 foreground_, c16 background_) : symbol(' '), foreground(get_foreground_basic_color()), background(get_background_basic_color())
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
	static pair<size_t, size_t> get_size()
	{
		return make_pair((size_t)1, (size_t)1);
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
		out << pixel.symbol << pixel.foreground << pixel.background;
		return out;
	}
};

template<typename T>
class Pixel
{
public:
	T pixel;
	Pixel() : pixel(' ', get_foreground_basic_color(), get_background_basic_color())
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
	static pair<size_t, size_t> get_size()
	{
		pair<size_t, size_t> answer(T::get_size());
		answer.first *= 2;
		return answer;
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
	uint32_t size_x = 80;
	uint32_t size_y = 25;
	position pixel_size;
	T* pixel_table[100][100];
	//
	class Picture_Exception : public exception
	{
	private:
		string message = "Error: exite out of picture range!!!";
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
	string get_file_name() const
	{
		return folder_name + '/' + picture_name + end_name;
	}
	void dounload()
	{
		ifstream in(get_file_name());
		if (!in.is_open())
		{
			throw File_not_found_exception();
		}
		in.read(reinterpret_cast<char*>(&size_x), sizeof(size_x));
		in.read(reinterpret_cast<char*>(&size_y), sizeof(size_y));
		for (size_t y = 0; y < size_y; ++y)
		{
			for (size_t x = 0; x < size_x; ++x)
			{
				in >> *pixel_table[x][y];
			}
		}
	}
	void upload() const
	{
		ofstream out(get_file_name());
		out.write(reinterpret_cast<const char*>(&size_x), sizeof(size_x));
		out.write(reinterpret_cast<const char*>(&size_y), sizeof(size_y));
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
	bool exist() const
	{
		ifstream in(get_file_name());
		bool answer = in.is_open();
		in.close();
		return answer;
	}
	void build_file() const
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
	void update_pixel_size()
	{
		pixel_size = (position) T::get_size();
	}
	//
	void check_picture_position(position pic_pos) const
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
	//
	void build_pixel_table()
	{
		for (size_t y = 0; y < size_y; ++y)
		{
			for (size_t x = 0; x < size_x; ++x)
			{
				pixel_table[x][y] = new T;
			}
		}
	}
public:
	Picture(string name_) : picture_name(name_)
	{
		update_pixel_size();
		update_path();
		if (!exist())
		{
			throw File_not_found_exception();
		}
		build_pixel_table();
		dounload();
	}
	Picture(string new_name, size_t x, size_t y) : size_x(x), size_y(y), picture_name(new_name)
	{
		update_pixel_size();
		update_path();
		build_file();
		build_pixel_table();
	}

	void seg_draw(position pic_pos, window4 cur_pos) const
	{
		check_picture_position(pic_pos);
		cur_pos.screen_check();
		check_picture_position({ pic_pos.x + (cur_pos.x2 - cur_pos.x1) / pixel_size.x, pic_pos.y + (cur_pos.y2 - cur_pos.y1) / pixel_size.y });
		set_cursor_pos(cur_pos.x1, cur_pos.y1);
		//y is position in picture
		for (size_t y = pic_pos.y; (y - pic_pos.y) * pixel_size.y + cur_pos.y1 <= cur_pos.y2; ++y)
		{
			for (size_t x = pic_pos.x; (x - pic_pos.x) * pixel_size.x + cur_pos.x1 <= cur_pos.x2; ++x)
			{
				pixel_table[x][y]->draw();
			}
			size_t next_position_in_console_y = (y - pic_pos.y + 1) * pixel_size.y + cur_pos.y1;
			if (next_position_in_console_y <= cur_pos.y2)
			{
				set_cursor_pos(cur_pos.x1, next_position_in_console_y);
			}
		}
	}
	void draw_from(position pic_pos) const
	{
		position cur_pos(get_cursor_pos());
		position sec_cur_pos(cur_pos.x + (size_x - pic_pos.x) * pixel_size.x - 1, cur_pos.y + (size_y - pic_pos.y) * pixel_size.y - 1);
		window4 window(cur_pos, sec_cur_pos);
		seg_draw(pic_pos, window);
	}
	void draw(position cur_pos) const
	{
		seg_draw(position(0, 0), window4(cur_pos, position(cur_pos.x + size_x * pixel_size.x - 1, cur_pos.y + size_y * pixel_size.y - 1)));
	}
	void draw() const
	{
		draw((position)get_cursor_pos());
	}

	void expanded_draw(position pic_pos, window4 cur_pos) const
	{
		cur_pos.screen_check();
		set_cursor_pos(cur_pos.x1, cur_pos.y1);
		pic_pos.x %= size_x;
		pic_pos.y %= size_y;
		for (size_t y = pic_pos.y, pos_y = cur_pos.y1; pos_y <= cur_pos.y2; y = (y + 1) % size_y, pos_y += pixel_size.y)
		{
			for (size_t x = pic_pos.x, pos_x = cur_pos.x1; pos_x <= cur_pos.x2; x = (x + 1) % size_x, pos_x += pixel_size.x)
			{
				pixel_table[x][y]->draw();
			}
			size_t next_position_in_console_y = pos_y + pixel_size.y;
			if (next_position_in_console_y <= cur_pos.y2)
			{
				set_cursor_pos(cur_pos.x1, next_position_in_console_y);
			}
		}
	}
	void expanded_draw(window4 cur_pos) const
	{
		expanded_draw({ 0, 0 }, cur_pos);
	}
	void expanded_draw(position right_down_angle) const
	{
		window4 cur_pos((position) get_cursor_pos(), right_down_angle);
		expanded_draw(cur_pos);
	}
	T& get_pixel(position pic_pos)
	{
		check_picture_position(pic_pos);
		return *(pixel_table[pic_pos.x][pic_pos.y]);
	}
	void rename(string new_name)
	{

	}
	void change_folder(string new_folder)
	{

	}
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