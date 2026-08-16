#pragma once

#include <iostream>
#include <fstream>
#include <conio.h>
#include "console_func.h"
using namespace std;


class File_Not_Found_Exception : public exception
{
private:
	string massage = "Error: file not found!!!";
public:
	File_Not_Found_Exception()
	{

	}

	const char* what() const noexcept override
	{
		return massage.c_str();
	}
};

class Cant_Remove_File : public exception
{
private:
	string massage = "Error: can't remove file!!!";
public:
	Cant_Remove_File()
	{

	}

	const char* what() const noexcept override
	{
		return massage.c_str();
	}
};


struct position
{
	uint32_t x, y;
	position() : x(0), y(0)
	{

	}
	position(size_t x_, size_t y_) : x(x_), y(y_)
	{

	}
	position(uint32_t x_, uint32_t y_) : x(x_), y(y_)
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
	//
	friend istream& operator>>(istream& in, position& pos)
	{
		in.read(reinterpret_cast<char*>(&pos.x), sizeof(pos.x));
		in.read(reinterpret_cast<char*>(&pos.y), sizeof(pos.y));
		return in;
	}
	friend ofstream& operator<<(ofstream& out, const position& pos)
	{
		out.write(reinterpret_cast<const char*>(&pos.x), sizeof(pos.x));
		out.write(reinterpret_cast<const char*>(&pos.y), sizeof(pos.y));
		return out;
	}
	//
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
	uint32_t x1, y1, x2, y2;
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
	window4(uint32_t x1_, uint32_t y1_, uint32_t x2_, uint32_t y2_) : x1(x1_), y1(y1_), x2(x2_), y2(y2_)
	{
		check_window4();
	}
	window4(position left_up_angle, position right_down_angle) : x1(left_up_angle.x), y1(left_up_angle.y), x2(right_down_angle.x), y2(right_down_angle.y)
	{
		check_window4();
	}
	window4(initializer_list<uint32_t> list)
	{
		x1 = *list.begin();
		y1 = *(list.begin() + 1);
		x2 = *(list.begin() + 2);
		y2 = *(list.begin() + 3);
	}
	//
	friend istream& operator>>(istream& in, window4& window)
	{
		in.read(reinterpret_cast<char*>(&window.x1), sizeof(window.x1));
		in.read(reinterpret_cast<char*>(&window.y1), sizeof(window.y1));
		in.read(reinterpret_cast<char*>(&window.x2), sizeof(window.x2));
		in.read(reinterpret_cast<char*>(&window.y2), sizeof(window.y2));		
		return in;
	}
	friend ofstream& operator<<(ofstream& out, const window4& window)
	{
		out.write(reinterpret_cast<const char*>(&window.x1), sizeof(window.x1));
		out.write(reinterpret_cast<const char*>(&window.y1), sizeof(window.y1));
		out.write(reinterpret_cast<const char*>(&window.x2), sizeof(window.x2));
		out.write(reinterpret_cast<const char*>(&window.y2), sizeof(window.y2));
		return out;
	}
	//
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

class File_Name_Exception : public exception
{
private:
	string message = "Error: wrong file name or folder path!!!";
public:
	File_Name_Exception()
	{

	}
	const char* what() const noexcept override
	{
		return message.c_str();
	}
};

void update_path(string& folder, string& name)
{
	int seporator_position = -1;
	string new_picture_name;
	for (int i = name.size() - 1; i >= 0; --i)
	{
		if (name[i] == '/')
		{
			seporator_position = i;
			break;
		}
	}
	if (seporator_position > 0)
	{
		folder += '/';
	}
	for (int i = 0; i < name.size(); ++i)
	{
		if (i < seporator_position)
		{
			name += name[i];
		}
		else if (seporator_position < i)
		{
			new_picture_name += name[i];
		}
	}
	name = new_picture_name;
}

bool exist(string file_name)
{
	ifstream in(file_name);
	bool answer = in.is_open();
	in.close();
	return answer;
}
void build_file(string file_name)
{
	ofstream out(file_name);
	out.close();
}

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
	T* pixel_table[300][300];
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
	void dounload()
	{
		ifstream in(get_file_name());
		if (!in.is_open())
		{
			throw File_Not_Found_Exception();
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
		in.close();
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
	//
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
		update_path(folder_name, picture_name);
		if (!exist(get_file_name()))
		{
			throw File_Not_Found_Exception();
		}
		build_pixel_table();
		dounload();
	}
	Picture(string new_name, size_t x, size_t y) : size_x(x), size_y(y), picture_name(new_name)
	{
		update_pixel_size();
		update_path(folder_name, picture_name);
		build_file(get_file_name());
		build_pixel_table();
	}
	//
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
	//
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
	//
	T& get_pixel(position pic_pos)
	{
		check_picture_position(pic_pos);
		return *(pixel_table[pic_pos.x][pic_pos.y]);
	}
	//
	string get_file_name() const
	{
		return folder_name + '/' + picture_name + end_name;
	}
	void remove()
	{
		if (exist(get_file_name()))
		{
			if (std::remove(get_file_name().c_str()) != 0)
			{
				throw Cant_Remove_File();
			}
		}
	}
	string get_name() const
	{
		return picture_name;
	}
	string get_folder() const
	{
		return folder_name;
	}
	void rename(string new_name)
	{
		for (size_t i = 0; i < new_name.size(); i++)
		{
			if (new_name[i] == '/')
			{
				throw File_Name_Exception();
			}
		}
		remove();
		picture_name = new_name;
		upload();
	}
	void change_folder(string new_folder)
	{
		create_folders(new_folder);
		remove();
		folder_name = new_folder;
		upload();
	}
	//
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

template<typename T>
class Roll
{
private:
	Picture<T> picture;
	uint32_t delay;//in milliseconds
	position pic_pos;
	window4 cur_pos;
	//
	string folder_name = "./Pictures";
	string roll_name;
	const string end_name = ".pic";
	//
	string get_file_name() const
	{
		return folder_name + '/' + roll_name + end_name;
	}
	void dounload()
	{
		ifstream in(get_file_name());
		if (!in.is_open())
		{
			throw File_Not_Found_Exception();
		}
		in.read(reinterpret_cast<*char>(&delay), sizeof(delay));
		in >> pic_pos >> cur_pos;
		in >> picture.get_file_name();
		in.close();
	}
	void upload() const
	{
		ofstream out(get_file_name());
		out.write(reinterpret_cast<const char*>(&delay), sizeof(delay));
		out << pic_pos << cur_pos;
		string picture_name;
		getline(in, picture_name);
		out.close();
		return;
	}
public:
	Roll(string picture_name, uint32_t delay_) : picture(picture_name), delay(delay_), pic_pos(0, 0), cur_pos(0, 0, 0, 0)
	{
		roll_name(picture.get_name());
		update_path(folder_name, roll_name);

	}
	Roll(string roll_name_, string picture_name, size_t delay_) : roll_name(roll_name), picture(picture_name), delay(delay_), pic_pos(0, 0), cur_pos(0, 0, 0, 0)
	{
		update_path(folder_name, roll_name);

	}
	//
	void set_delay(size_t new_delay)
	{
		delay = new_delay;
	}
	void set_pic_pos(position new_pic_pos)
	{
		pic_pos = new_pic_pos;
	}
	void set_cur_pos(window4 new_cur_pos)
	{
		cur_pos = new_cur_pos;
	}
	//
	size_t& get_delay() const
	{
		return delay;
	}
	position& get_pic_pos() const
	{
		return pic_pos;
	}
	window4& get_pic_pos() const
	{
		return cur_pos;
	}
	//
	~Roll()
	{
		picture.~Picture();
	}
};