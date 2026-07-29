#include "pic.h"

//File_not_found_exception
File_not_found_exception::File_not_found_exception()
{

}

const char* File_not_found_exception::what() const noexcept
{
	return massage.c_str();
}

//position
position::position(int x_, int y_) : x(x_), y(y_)
{

}

//window4
window4::window4(int x1_, int y1_, int x2_, int y2_) : x1(x1_), y1(y1_), x2(x2_), y2(y2_)
{
	check_position(x1, y1);
	check_position(x2, y2);
	if (x2 < x1 || y2 < y1)
	{
		throw Screen_Exception();
	}
}

//Console_Pixel_16
Console_Pixel_16::Console_Pixel_16() : symbol(' '), foreground(7), background(0)
{
	
}

Console_Pixel_16::Console_Pixel_16(char symbol_, c16 foreground_, c16 background_) : symbol(symbol_), foreground(foreground_), background(background_)
{

}

void Console_Pixel_16::draw() const
{
	set_color_16(foreground, background);
	string h;
	h += symbol;
	putstr_(h);
}

void Console_Pixel_16::set(char symbol_)
{
	symbol = symbol_;
}

void Console_Pixel_16::set(c16 foreground_, c16 background_)
{
	foreground = foreground_;
	background = background_;
}

void Console_Pixel_16::set(char symbol_, c16 foreground_, c16 background_)
{
	symbol = symbol_;
	foreground = foreground_;
	background = background_;
}

istream& operator>>(istream& in, Console_Pixel_16& pixel)
{
	in >> noskipws;
	in >> pixel.symbol;
	in >> skipws;
	in >> pixel.foreground >> pixel.background;
	return in;
}

ostream& operator<<(ostream& out, const Console_Pixel_16& pixel)
{
	out << pixel.symbol << pixel.background << pixel.foreground;
	return out;
}

//Pixel
template<typename T>
Pixel<T>::Pixel() : first(), second()
{
	
}

template<typename T>
Pixel<T>::Pixel(T first_, T second_) : first(first_), second(second_)
{
	
}

template<typename T>
void Pixel<T>::draw() const
{
	first.draw();
	second.draw();
}

template<typename T>
istream& operator>>(istream& in, Pixel<T>& pixel)
{
	in >> noskipws;
	in >> pixel.symbol;
	in >> skipws;
	in >> pixel.foreground >> pixel.background;
	return in;
}

template<typename T>
ostream& operator<<(ostream& out, const Pixel<T>& pixel)
{
	out << pixel.symbol << pixel.background << pixel.foreground;
	return out;
}


//Picture_Exception
template<typename T>
Picture<T>::Picture_Exception::Picture_Exception()
{

}

template<typename T>
const char* Picture<T>::Picture_Exception::what() const noexcept
{
	return message.c_str();
}

//Picture protected
template<typename T>
string Picture<T>::get_symbol_file_name() const
{
	return folder_name + picture_name + symbol_name;
}

template<typename T>
string Picture<T>::get_foreground_file_name() const
{
	return folder_name + picture_name + color_foreground_name;
}

template<typename T>
string Picture<T>::get_background_file_name() const
{
	return folder_name + picture_name + color_background_name;
}

template<typename T>
void Picture<T>::build_files()
{
	ofstream symbol_out(get_symbol_file_name());
	ofstream foreground_out(get_foreground_file_name());
	ofstream background_out(get_background_file_name());
	symbol_out.close();
	foreground_out.close();
	background_out.close();
}

template<typename T>
void Picture<T>::update_path()
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

//Picture public
template<typename T>
Picture<T>::Picture(string name_) : picture_name(name_)
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

template<typename T>
Picture<T>::Picture(string new_name, size_t x, size_t y) : size_x(x), size_y(y), picture_name(new_name)
{
	update_path();
	build_files();
}

template<typename T>
void Picture<T>::draw()
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
template<typename T>
void Picture<T>::draw(position cur_pos)
{
	set_cursor_pos(cur_pos.x, cur_pos.y);
	draw();
}

template<typename T>
void Picture<T>::seg_draw(position pic_pos, window4 cur_pos)
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

template<typename T>
Picture<T>::~Picture()
{

}