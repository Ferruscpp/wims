#include "pic.h"

//Picture protected
string Picture::get_symbol_file_name() const
{
	return folder_name + picture_name + symbol_name;
}

string Picture::get_foreground_file_name() const
{
	return folder_name + picture_name + color_foreground_name;
}

string Picture::get_background_file_name() const
{
	return folder_name + picture_name + color_background_name;
}

void Picture::build_files()
{
	ofstream symbol_out(get_symbol_file_name());
	ofstream foreground_out(get_foreground_file_name());
	ofstream background_out(get_background_file_name());
	symbol_out.close();
	foreground_out.close();
	background_out.close();
}

void Picture::update_path()
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
Picture::Picture(string name_) : picture_name(name_)
{
	update_path();
	ifstream in(get_symbol_file_name());
	if (in.is_open())
	{
		throw File_not_found_exception("Picture::Picture(string name_);");
	}
	in.close();
	upload();
}

Picture::Picture(string new_name, size_t x, size_t y) : size_x(x), size_y(y), picture_name(new_name)
{
	update_path();
	build_files();
}

void Picture::draw()
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

void Picture::draw(position cur_pos)
{
	set_cursor_pos(cur_pos.x, cur_pos.y);
	draw();
}

void Picture::seg_draw(position pic_pos, window4 cur_pos)
{
	if (size_x <= pic_pos.x || size_y <= pic_pos.y)
	{
		throw Picture_Exception();
	}
	set_cursor_pos(cur_pos.x1, cur_pos.y1);
	for (size_t i = 0; i < ; i++)
	{

	}
}

Picture::~Picture()
{

}


//Picture_RGB
