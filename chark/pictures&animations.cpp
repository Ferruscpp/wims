#include "pictures&animations.h"

/*размер экрана по x 201 символов в развернутом варианте*/
/*размер экрана по y 52 символа в развернутом вариате*/

color int_to_color(int color_index)
{
	/* 0  Черный          8  Темно-серый
   1  Темно-синий     9  Синий
   2  Темно-зеленый   10 Зеленый
   3  Темно-голубой   11 Алый
   4  Темно-красный   12 красный
   5  Темно-алый      13 алый
   6  Коричневый      14 Желтый
   7  Светло-серый    15 Белый
   */
	color_index = color_index % 16;
	switch (color_index)
	{
	case 0:
		return color::cBLACK;
	case 1:
		return color::cDARK_BLUE;
	case 2:
		return color::cDARK_GREEN;
	case 3:
		return color::DARK_CYAN;
	case 4:
		return color::cDARK_RED;
	case 5:
		return color::cDARK_MAGENTA;
	case 6:
		return color::cBROWN;
	case 7:
		return color::cLIGHT_GRAY;
	case 8:
		return color::cDARK_GRAY;
	case 9:
		return color::cBLUE;
	case 10:
		return color::cGREEN;
	case 11:
		return color::cCYAN;
	case 12:
		return color::cRED;
	case 13:
		return color::cMAGENTA;
	case 14:
		return color::cYELLOW;
	case 15:
		return color::cWHITE;
	}
}

int Picture::picture_cnt = 0;

void Picture::save_online()
{
	ifstream pic_s(picture_name + _symbol_name);
	ifstream pic_c_f(picture_name + _color_foreground_name);
	ifstream pic_c_b(picture_name + _color_background_name);
	string symbol_s;
	int color_f, color_b;
	for (int i = 0; i < size_y; i++)
	{
		getline(pic_s, symbol_s);
		_pic_symbol[i] = symbol_s;
		for (int j = 0; j < size_x; j++)
		{
			pic_c_f >> color_f;
			pic_c_b >> color_b;
			_pic_c_f[i][j] = color_f;
			_pic_c_b[i][j] = color_b;
		}
	}
	pic_s.close();
	pic_c_f.close();
	pic_c_b.close();
}

Picture::Picture() : picture_name("New_picture_" + to_string(++picture_cnt))
{
	ofstream pic_s(picture_name + _symbol_name);
	ofstream pic_c_f(picture_name + _color_foreground_name);
	ofstream pic_c_b(picture_name + _color_background_name);
	for (int i = 0; i < size_y; i++)
	{
		for (int j = 0; j < size_x; j++)
		{
			pic_s << ' ';
			pic_c_f << int(cWHITE) << ' ';
			pic_c_b << int(cBLACK) << ' ';
		}
		pic_s << '\n';
		pic_c_f << '\n';
		pic_c_b << '\n';
	}
	pic_s.close();
	pic_c_f.close();
	pic_c_b.close();
	this->save_online();
}

Picture::Picture(int x, int y) : picture_name("New_picture_" + to_string(++picture_cnt))
{
	size_x = x;
	size_y = y;
	if (size_x <= 0 || size_x > 200 || size_y <= 0 || size_y > 66)
	{
		throw Picture_Exception();
	}
	ofstream pic_s(picture_name + _symbol_name);
	ofstream pic_c_f(picture_name + _color_foreground_name);
	ofstream pic_c_b(picture_name + _color_background_name);
	for (int i = 0; i < size_y; i++)
	{
		for (int j = 0; j < size_x; j++)
		{
			pic_s << ' ';
			pic_c_f << int(cWHITE) << ' ';
			pic_c_b << int(cBLACK) << ' ';
		}
		pic_s << '\n';
		pic_c_f << '\n';
		pic_c_b << '\n';
	}
	pic_s.close();
	pic_c_f.close();
	pic_c_b.close();
	this->save_online();
}

Picture::Picture(string new_name, int x, int y) : picture_name(new_name)
{
	picture_cnt++;
	size_x = x;
	size_y = y;
	if (size_x <= 0 || size_x > 200 || size_y <= 0 || size_y > 182)
	{
		throw Picture_Exception();
	}
	ofstream pic_s(picture_name + _symbol_name);
	ofstream pic_c_f(picture_name + _color_foreground_name);
	ofstream pic_c_b(picture_name + _color_background_name);
	for (int i = 0; i < size_y; i++)
	{
		for (int j = 0; j < size_x; j++)
		{
			pic_s << ' ';
			pic_c_f << int(cWHITE) << ' ';
			pic_c_b << int(cBLACK) << ' ';
		}
		pic_s << '\n';
		pic_c_f << '\n';
		pic_c_b << '\n';
	}
	pic_s.close();
	pic_c_f.close();
	pic_c_b.close();
	this->save_online();
}

Picture::Picture(string new_name) : picture_name(new_name)
{
	picture_cnt++;
	ifstream pic_s(picture_name + _symbol_name);
	ifstream pic_c_f(picture_name + _color_foreground_name);
	ifstream pic_c_b(picture_name + _color_background_name);
	string size_cnt;
	getline(pic_s, size_cnt);
	size_x = size_cnt.size();
	size_y = 0;
	while (!pic_s.eof())
	{
		getline(pic_s, size_cnt);
		size_y++;
	}
	pic_s.close();
	pic_c_f.close();
	pic_c_b.close();
	if (size_x <= 0 || size_x > 200 || size_y <= 0 || size_y > 182)
	{
		throw Picture_Exception();
	}
	this->save_online();
}

void Picture::draw()
{
	set_cursor_pos(1, 1);
	for (int i = 0; i < size_y; i++)
	{
		for (int j = 0; j < size_x; j++)
		{
			set_color(int_to_color(_pic_c_f[i][j]), int_to_color(_pic_c_b[i][j]));
			_putch(static_cast<char>(_pic_symbol[i][j]));
		}
		set_color(cWHITE, cBLACK);
		cout << endl;
	}
	set_color(cWHITE, cBLACK);
}

void Picture::draw(int cur_pos_x, int cur_pos_y)
{
	cur_pos_x++;
	cur_pos_y++;
	if (cur_pos_x <= 0 || cur_pos_x > 200 || cur_pos_y <= 0 || cur_pos_y > 50)
	{
		throw Screen_Exception();
	}
	if (cur_pos_x + size_x > 200 || cur_pos_y + size_y > 50)
	{
		throw Screen_Exception();
	}
	set_cursor_pos(cur_pos_x, cur_pos_y);
	for (int i = 0; i < size_y; i++)
	{
		for (int j = 0; j < size_x; j++)
		{
			set_color(int_to_color(_pic_c_f[i][j]), int_to_color(_pic_c_b[i][j]));
			_putch(static_cast<char>(_pic_symbol[i][j]));
		}
		set_color(cWHITE, cBLACK);
		set_cursor_pos(cur_pos_x, cur_pos_y + i + 1);
		//cout << endl;
	}
	set_color(cWHITE, cBLACK);
}

void Picture::seg_draw(int x, int y, int cur_pos_x1, int cur_pos_y1, int cur_pos_x2, int cur_pos_y2)
{
	cur_pos_x1++;
	cur_pos_y1++;
	cur_pos_x2++;
	cur_pos_y2++;
	if (cur_pos_x1 <= 0 || cur_pos_x1 > 200 || cur_pos_y1 <= 0 || cur_pos_y1 > 50)
	{
		throw Screen_Exception();
	}
	if (cur_pos_x2 <= 0 || cur_pos_x2 > 200 || cur_pos_y2 <= 0 || cur_pos_y2 > 50)
	{
		throw Screen_Exception();
	}
	if (cur_pos_x1 > cur_pos_x2 || cur_pos_y1 > cur_pos_y2)
	{
		throw Screen_Exception();
	}
	if (x < 0 || x >= size_x || y < 0 || y >= size_y)
	{
		throw Picture_Exception();
	}
	if (cur_pos_x2 - cur_pos_x1 + 1 > size_x)
	{
		throw Picture_Exception();
	}
	if (cur_pos_y2 - cur_pos_y1 + 1 > size_y)
	{
		throw Picture_Exception();
	}
	set_cursor_pos(cur_pos_x1, cur_pos_y1);
	int y_cnt = 0, x_cnt = 0;
	for (int i = y; y_cnt < cur_pos_y2 - cur_pos_y1 + 1; i = (i + 1) % size_y, y_cnt++)
	{
		x_cnt = 0;
		for (int j = x; x_cnt < cur_pos_x2 - cur_pos_x1 + 1; j = (j + 1) % size_x, x_cnt++)
		{
			set_color(int_to_color(_pic_c_f[i][j]), int_to_color(_pic_c_b[i][j]));
			_putch(static_cast<char>(_pic_symbol[i][j]));
		}
		set_color(cWHITE, cBLACK);
		set_cursor_pos(cur_pos_x1, cur_pos_y1 + y_cnt + 1);
	}
	set_color(cWHITE, cBLACK);
}

void Picture::rotate_vertically(int cur_pos_x1, int cur_pos_y1, int cur_pos_x2, int cur_pos_y2, int milliseconds, int times)
{
	cur_pos_x1++;
	cur_pos_y1++;
	cur_pos_x2++;
	cur_pos_y2++;
	if (cur_pos_x1 <= 0 || cur_pos_x1 > 200 || cur_pos_y1 <= 0 || cur_pos_y1 > 50)
	{
		throw Screen_Exception();
	}
	if (cur_pos_x2 <= 0 || cur_pos_x2 > 200 || cur_pos_y2 <= 0 || cur_pos_y2 > 50)
	{
		throw Screen_Exception();
	}
	if (cur_pos_x1 > cur_pos_x2 || cur_pos_y1 > cur_pos_y2)
	{
		throw Screen_Exception();
	}
	if (cur_pos_x2 - cur_pos_x1 + 1 > size_x)
	{
		throw Picture_Exception();
	}
	if (cur_pos_y2 - cur_pos_y1 + 1 > size_y)
	{
		throw Picture_Exception();
	}
	if (times <= 0 || milliseconds < 0)
	{
		throw Time_Exception();
	}
	for (int _t_times = 0; _t_times < times; _t_times++)
	{
		this->seg_draw(0, _t_times % size_y, cur_pos_x1 - 1, cur_pos_y1 - 1, cur_pos_x2 - 1, cur_pos_y2 - 1);
		wait(milliseconds);
	}
}

void Picture::rotate_vertically(int x, int y, int cur_pos_x1, int cur_pos_y1, int cur_pos_x2, int cur_pos_y2, int milliseconds, int times)
{
	cur_pos_x1++;
	cur_pos_y1++;
	cur_pos_x2++;
	cur_pos_y2++;
	if (cur_pos_x1 <= 0 || cur_pos_x1 > 200 || cur_pos_y1 <= 0 || cur_pos_y1 > 50)
	{
		throw Screen_Exception();
	}
	if (cur_pos_x2 <= 0 || cur_pos_x2 > 200 || cur_pos_y2 <= 0 || cur_pos_y2 > 50)
	{
		throw Screen_Exception();
	}
	if (cur_pos_x1 > cur_pos_x2 || cur_pos_y1 > cur_pos_y2)
	{
		throw Screen_Exception();
	}
	if (x < 0 || x >= size_x || y < 0 || y >= size_y)
	{
		throw Picture_Exception();
	}
	if (cur_pos_x2 - cur_pos_x1 + 1 > size_x - x)
	{
		throw Picture_Exception();
	}
	if (cur_pos_y2 - cur_pos_y1 + 1 > size_y)
	{
		throw Picture_Exception();
	}
	if (times <= 0 || milliseconds < 0)
	{
		throw Time_Exception();
	}
	for (int _t_times = 0; _t_times < times; _t_times++)
	{
		this->seg_draw(x, (y + _t_times) % size_y, cur_pos_x1 - 1, cur_pos_y1 - 1, cur_pos_x2 - 1, cur_pos_y2 - 1);
		wait(milliseconds);
	}
}

void Picture::rotate_horizontally(int cur_pos_x1, int cur_pos_y1, int cur_pos_x2, int cur_pos_y2, int milliseconds, int times)
{
	cur_pos_x1++;
	cur_pos_y1++;
	cur_pos_x2++;
	cur_pos_y2++;
	if (cur_pos_x1 <= 0 || cur_pos_x1 > 200 || cur_pos_y1 <= 0 || cur_pos_y1 > 50)
	{
		throw Screen_Exception();
	}
	if (cur_pos_x2 <= 0 || cur_pos_x2 > 200 || cur_pos_y2 <= 0 || cur_pos_y2 > 50)
	{
		throw Screen_Exception();
	}
	if (cur_pos_x1 > cur_pos_x2 || cur_pos_y1 > cur_pos_y2)
	{
		throw Screen_Exception();
	}
	if (cur_pos_x2 - cur_pos_x1 + 1 > size_x)
	{
		throw Picture_Exception();
	}
	if (cur_pos_y2 - cur_pos_y1 + 1 > size_y)
	{
		throw Picture_Exception();
	}
	if (times <= 0 || milliseconds < 0)
	{
		throw Time_Exception();
	}
	for (int _t_times = 0; _t_times < times; _t_times++)
	{
		this->seg_draw(_t_times % size_x, 0, cur_pos_x1 - 1, cur_pos_y1 - 1, cur_pos_x2 - 1, cur_pos_y2 - 1);
		wait(milliseconds);
	}
}

void Picture::rotate_horizontally(int x, int y, int cur_pos_x1, int cur_pos_y1, int cur_pos_x2, int cur_pos_y2, int milliseconds, int times)
{
	cur_pos_x1++;
	cur_pos_y1++;
	cur_pos_x2++;
	cur_pos_y2++;
	if (cur_pos_x1 <= 0 || cur_pos_x1 > 200 || cur_pos_y1 <= 0 || cur_pos_y1 > 50)
	{
		throw Screen_Exception();
	}
	if (cur_pos_x2 <= 0 || cur_pos_x2 > 200 || cur_pos_y2 <= 0 || cur_pos_y2 > 50)
	{
		throw Screen_Exception();
	}
	if (cur_pos_x1 > cur_pos_x2 || cur_pos_y1 > cur_pos_y2)
	{
		throw Screen_Exception();
	}
	if (x < 0 || x >= size_x || y < 0 || y >= size_y)
	{
		throw Picture_Exception();
	}
	if (cur_pos_x2 - cur_pos_x1 + 1 > size_x - x)
	{
		throw Picture_Exception();
	}
	if (cur_pos_y2 - cur_pos_y1 + 1 > size_y)
	{
		throw Picture_Exception();
	}
	if (times <= 0 || milliseconds < 0)
	{
		throw Time_Exception();
	}
	for (int _t_times = 0; _t_times < times; _t_times++)
	{
		this->seg_draw((x + _t_times) % size_x, y, cur_pos_x1 - 1, cur_pos_y1 - 1, cur_pos_x2 - 1, cur_pos_y2 - 1);
		wait(milliseconds);
	}
}

Roll::Roll() : Picture()
{
	pic_x = 0;
	pic_y = 0;
	cur_pos_x1 = 0;
	cur_pos_y1 = 0;
	cur_pos_x2 = 0;
	cur_pos_y2 = 0;
}

Roll::Roll(Picture new_pic, int x, int y) : Picture(new_pic)
{
	pic_x = x;
	pic_y = y;
}

Roll::Roll(string new_pic, int x, int y, int _cur_pos_x1, int _cur_pos_y1, int _cur_pos_x2, int _cur_pos_y2) : Picture(new_pic)
{
	pic_x = x;
	pic_y = y;
	cur_pos_x1 = _cur_pos_x1;
	cur_pos_y1 = _cur_pos_y1;
	cur_pos_x2 = _cur_pos_x2;
	cur_pos_y2 = _cur_pos_y2;
}

Roll::Roll(Picture new_pic, int x, int y, int _cur_pos_x1, int _cur_pos_y1, int _cur_pos_x2, int _cur_pos_y2) : Picture(new_pic)
{
	pic_x = x;
	pic_y = y;
	cur_pos_x1 = _cur_pos_x1;
	cur_pos_y1 = _cur_pos_y1;
	cur_pos_x2 = _cur_pos_x2;
	cur_pos_y2 = _cur_pos_y2;
}

void Roll::seg_draw2()
{
	this->seg_draw(pic_x, pic_y, cur_pos_x1, cur_pos_y1, cur_pos_x2, cur_pos_y2);
}

void Roll::seg_draw2(int x, int y)
{
	pic_x = x;
	pic_y = y;
	this->seg_draw(pic_x, pic_y, cur_pos_x1, cur_pos_y1, cur_pos_x2, cur_pos_y2);
}

void Roll::change_x_y(int x, int y)
{
	pic_x = x;
	pic_y = y;
}

int gsd(int a, int b)
{
	if (b == 0)
	{
		return a;
	}
	else
	{
		return gsd(b, a % b);
	}
}

void two_rotate_vertically(Roll& a, Roll& b, int milliseconds, int times)
{
	bool cross = true;
	if (a.cur_pos_y1 > b.cur_pos_y2 || b.cur_pos_y1 > a.cur_pos_y2)
	{
		cross = false;
	}
	if (a.cur_pos_x1 > b.cur_pos_x2 || b.cur_pos_x1 > a.cur_pos_x2)
	{
		cross = false;
	}
	if (!cross)
	{
		if (times <= 0 || milliseconds < 0)
		{
			throw Time_Exception();
		}
		for (int _t_times = 0; _t_times < times; _t_times++)
		{
			a.seg_draw2(a.pic_x, (a.pic_y + _t_times) % a.size_y);
			b.seg_draw2(b.pic_x, (b.pic_y + _t_times) % b.size_y);
			wait(milliseconds);
		}
	}
	else
	{
		exit(0);
	}
}

void two_rotate_vertically(Roll& a, Roll& b, int milliseconds_1, int times_1, int milliseconds_2, int times_2)
{
	bool cross = true;
	if (a.cur_pos_y1 > b.cur_pos_y2 || b.cur_pos_y1 > a.cur_pos_y2)
	{
		cross = false;
	}
	if (a.cur_pos_x1 > b.cur_pos_x2 || b.cur_pos_x1 > a.cur_pos_x2)
	{
		cross = false;
	}
	if (!cross)
	{
		if (times_1 < 0 || milliseconds_1 < 0 || times_2 < 0 || milliseconds_2 < 0)
		{
			throw Time_Exception();
		}
		int _gsd = gsd(milliseconds_1, milliseconds_2);
		int cnt_s = 0, cnt_f = 0;
		int _t_times = 0;
		while (cnt_s < times_1 || cnt_f < times_2)
		{
			if (_t_times % milliseconds_1 == 0 && cnt_f < times_1)
			{
				a.seg_draw2(a.pic_x, (a.pic_y + _t_times / milliseconds_1) % a.size_y);
				cnt_f++;
			}
			if (_t_times % milliseconds_2 == 0 && cnt_s < times_2)
			{
				b.seg_draw2(b.pic_x, (b.pic_y + _t_times / milliseconds_2) % b.size_y);
				cnt_s++;
			}
			wait(_gsd);
			_t_times += _gsd;
		}
	}
	else
	{
		exit(0);
	}
}

void tree_rotate_vertically(Roll& a, Roll& b, Roll& c, int milliseconds_1, int times_1, int milliseconds_2, int times_2, int milliseconds_3, int times_3)
{
	bool cross_1_2 = true;
	bool cross_1_3 = true;
	bool cross_2_3 = true;
	if (a.cur_pos_y1 > b.cur_pos_y2 || b.cur_pos_y1 > a.cur_pos_y2)
	{
		cross_1_2 = false;
	}
	if (a.cur_pos_x1 > b.cur_pos_x2 || b.cur_pos_x1 > a.cur_pos_x2)
	{
		cross_1_2 = false;
	}
	if (a.cur_pos_y1 > c.cur_pos_y2 || c.cur_pos_y1 > a.cur_pos_y2)
	{
		cross_1_3 = false;
	}
	if (a.cur_pos_x1 > c.cur_pos_x2 || c.cur_pos_x1 > a.cur_pos_x2)
	{
		cross_1_3 = false;
	}
	if (c.cur_pos_y1 > b.cur_pos_y2 || b.cur_pos_y1 > c.cur_pos_y2)
	{
		cross_2_3 = false;
	}
	if (c.cur_pos_x1 > b.cur_pos_x2 || b.cur_pos_x1 > c.cur_pos_x2)
	{
		cross_2_3 = false;
	}
	if (!cross_1_2 && !cross_1_3 && !cross_2_3)
	{
		if (times_1 < 0 || milliseconds_1 < 0 || times_2 < 0 || milliseconds_2 < 0 || times_3 < 0 || milliseconds_3 < 0)
		{
			throw Time_Exception();
		}
		int _gsd = gsd(gsd(milliseconds_1, milliseconds_2), milliseconds_3);
		int cnt_s = 0, cnt_f = 0, cnt_t = 0;
		int _t_times = 0;
		while (cnt_f < times_1 || cnt_s < times_2 || cnt_t < times_3)
		{
			if (_t_times % milliseconds_1 == 0 && cnt_f < times_1)
			{
				a.seg_draw2(a.pic_x, (a.pic_y + /*_t_times / milliseconds_1*/ 13) % a.size_y);
				cnt_f++;
			}
			if (_t_times % milliseconds_2 == 0 && cnt_s < times_2)
			{
				b.seg_draw2(b.pic_x, (b.pic_y + /*_t_times / milliseconds_2*/ 13) % b.size_y);
				cnt_s++;
			}
			if (_t_times % milliseconds_3 == 0 && cnt_t < times_3)
			{
				c.seg_draw2(c.pic_x, (c.pic_y + /*_t_times / milliseconds_3*/ 13) % c.size_y);
				cnt_t++;
			}
			//wait(500);
			_t_times += _gsd;
		}
	}
	else
	{
		exit(0);
	}
}