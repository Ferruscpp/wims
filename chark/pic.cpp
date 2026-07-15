#include "pic.h"


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

Picture::Picture(string name_) : picture_name(name_)
{
	update_path();
	ifstream in(get_symbol_file_name());
	if (in.is_open())
	{
		throw File_not_found_exception("Picture::Picture(string name_);");
	}
	
}