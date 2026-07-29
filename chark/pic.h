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
	File_not_found_exception();

	const char* what() const noexcept override;
};

struct position
{
	int x, y;
	position(int x_, int y_);
};

struct window4
{
	int x1, y1, x2, y2;
	window4(int x1_, int y1_, int x2_, int y2_);
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
	Console_Pixel_16();
	Console_Pixel_16(char symbol_, c16 foreground_, c16 background_);
	void draw() const;
	void set(char symbol_);
	void set(c16 foreground_, c16 background_);
	void set(char symbol_, c16 foreground_, c16 background_);
	friend istream& operator>>(istream& in, Console_Pixel_16& pixel);
	friend ostream& operator<<(ostream& out, const Console_Pixel_16& pixel);
};

template<typename T>
class Pixel
{
public:
	T first, second;
	Pixel();
	Pixel(T first_, T second_);
	void draw() const;
	friend istream& operator>>(istream& in, Pixel& pixel);
	friend ostream& operator<<(ostream& out, Pixel& pixel);
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
		const char* what() const noexcept override;
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