#include "console_func.h"

#if DEBUG_MODE_FOR_LINUX
#define _WIN32 0
#define __linux__ 1
#endif

//crgb
crgb::crgb() : red(0), green(0), blue(0)
{

}

crgb::crgb(char r, char g, char b) : red(r), green(g), blue(b)
{

}

istream& operator>>(istream& in, crgb& color)
{
	in >> color.red >> color.green >> color.blue;
	return in;
}

ostream& operator<<(ostream& out, const crgb& color)
{
	out << color.red << color.green << color.blue;
	return out;
}

//c8bit
c8bit::c8bit() : color(0)
{

}

c8bit::c8bit(char color_) : color(color_)
{

}

istream& operator>>(istream& in, c8bit& color)
{
	in >> color.color;
	return in;
}

ostream& operator<<(ostream& out, const c8bit& color)
{
	out << color.color;
	return out;
}

//c16
c16::c16() : color(0)
{

}

c16::c16(int color_) : color(color_)
{
	if (color < 0 || 16 <= color)
	{
		throw Color_Exception();
	}
}

c16::c16(char color_) : color(color_)
{
	if (16 <= (int)color)
	{
		throw Color_Exception();
	}
}

c16& c16::operator=(int color_)
{
	if (color_ < 0 || 16 <= color_)
	{
		throw Color_Exception();
	}
	color = color_;
	return *this;
}

c16& c16::operator=(char color_)
{
	if (16 <= (int)color_)
	{
		throw Color_Exception();
	}
	color = color_;
	return *this;
}

istream& operator>>(istream& in, c16& color)
{
	in >> noskipws;
	in >> color.color;
	in >> skipws;
	return in;
}

ostream& operator<<(ostream& out, const c16& color)
{
	out << color.color;
	return out;
}

//Screen_Controller
Screen_Controller::Screen_Controller()
{

}

void Screen_Controller::switch_to_small()
{
	bottom_x = 0;
	top_x = 80;
	bottom_y = 0;
	top_y = 25;
}

void Screen_Controller::switch_to_big()
{
	bottom_x = 0;
	top_x = 201;
	bottom_y = 0;
	top_y = 51;
}

c16 Screen_Controller::get_foreground_basic_color() const
{
	return foreground_basic;
}

c16 Screen_Controller::get_background_basic_color() const
{
	return background_basic;
}

void check_position(int x, int y)
{
	if (x < sc_->bottom_x || sc_->top_x < x)
	{
		throw Screen_Exception();
	}
	if (y < sc_->bottom_y || sc_->top_y < y)
	{
		throw Screen_Exception();
	}
}

Screen_Controller* sc_;

c16 get_foreground_bassic_color()
{
	return sc_->get_foreground_basic_color();
}

c16 get_background_bassic_color()
{
	return sc_->get_background_basic_color();
}


void start_for_all_OS()
{
	sc_ = new Screen_Controller;
	//putstr_("\033[12l\033[?25l");
}

void end_for_all_OS()
{
	set_color_16(sc_->get_foreground_basic_color(), sc_->get_background_basic_color());
	delete sc_;
	//putstr_("\033[?25h");
}

#if _WIN32
//Terminal_Controller
Terminal_Controller::Terminal_Controller()
{

}
Terminal_Controller::~Terminal_Controller()
{

}
//putstr_
int putstr_(string str)
{
	for (int i = 0; i < str.size(); ++i)
	{
		if (!_putch(str[i]))
		{
			return EOF;
		}
	}
	return str[str.size() - 1];
}
//getch_
int getch_()
{
	return _getch();
}
//is_hit_
bool is_hit_()
{
	return _kbhit();
}
//wait
void wait(size_t milliseconds)
{
	Sleep(milliseconds);
}
//clear_inbuffer
void clear_in_buffer()
{
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	FlushConsoleInputBuffer(hStdin);
}
//init_console_fun
void init_console_func()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor_info;
	GetConsoleCursorInfo(hConsole, &cursor_info);
	cursor_info.bVisible = FALSE;
	SetConsoleCursorInfo(hConsole, &cursor_info);
	start_for_all_OS();
}
//end_of_work_console_func
void end_of_work_console_func()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor_info;
	GetConsoleCursorInfo(hConsole, &cursor_info);
	cursor_info.bVisible = FALSE;
	SetConsoleCursorInfo(hConsole, &cursor_info);
	end_for_all_OS();
}
#elif __linux__
//Terminal_Controller
int Terminal_Controller::counter = 0;
Terminal_Controller::Terminal_Controller()
{
	counter++;
	tcgetattr(STDIN_FILENO, &oldin);
	newin = oldin;
	newin.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &newin);
}
int Terminal_Controller::get_info()
{
	return counter;
}
Terminal_Controller::~Terminal_Controller()
{
	--counter;
	tcsetattr(STDIN_FILENO, TCSANOW, &oldin);
}
//Not_Init_Console_Error
Not_Init_Console_Error::Not_Init_Console_Error()
{

}
const char* Not_Init_Console_Error::what() const noexcept
{
	return message.c_str();
}
//putstr_
int putstr_(string str)
{
	return write(1, str.c_str(), str.size());
}
//getch_
int getch_()
{
	if (Terminal_Controller::get_info() != 1)
	{
		throw Not_Init_Console_Error();
	}
	char ch;
	read(STDIN_FILENO, &ch, 1);
	return (int)ch;
}
//is_hit_
bool is_hit_()
{
	if (Terminal_Controller::get_info() != 1)
	{
		throw Not_Init_Console_Error();
	}
	int bit_cnt;
	ioctl(STDIN_FILENO, FIONREAD, &bit_cnt);
	return bit_cnt > 0;
}
//wait
void wait(int milliseconds)
{
	usleep(milliseconds * 1000);
}
//clear_in_buffer
void clear_in_buffer()
{
	tcflush(STDIN_FILENO, TCIFLUSH);
}
//tc_
Terminal_Controller* tc_;
//init_console_func
void init_console_func()
{
	tc_ = new Terminal_Controller;
	putstr_("\033[?25l");
	start_for_all_OS();
}
//end_of_work_console_func
void end_of_work_console_func()
{
	end_for_all_OS();
	putstr_("\033[?25h");
	delete tc_;
}
#endif


//Screen_Exception
Screen_Exception::Screen_Exception()
{

}

const char* Screen_Exception::what() const noexcept
{
	return massage.c_str();
}


//ANSI_Doesnt_Supported_Exception
ANSI_Doesnt_Supported_Exception::ANSI_Doesnt_Supported_Exception()
{

}

const char* ANSI_Doesnt_Supported_Exception::what() const noexcept
{
	return massage.c_str();
}


//cursor position
pair<size_t, size_t> get_cursor_pos()
{
	clear_in_buffer();
	putstr_("\033[6n");
	auto start = chrono::steady_clock::now();
	while (!is_hit_())
	{
		auto now_time = chrono::steady_clock::now();
		if (chrono::duration_cast<chrono::milliseconds>(now_time - start).count() > 30)
		{
			throw ANSI_Doesnt_Supported_Exception();
		}
	}
	char ch;
	while (true)
	{
		ch = getch_();
		if (ch == '\033')
		{
			string answer;
			while ((ch = getch_()) != 'R' || answer.size() > 100)
			{
				answer += ch;
				if (ch == '\033')
				{
					answer = "";
				}
			}
			if (ch == 'R')
			{
				//answer == "[number;number"
				if (answer[0] == '[')
				{
					string first_num, second_num;
					int x, y, i = 1;
					for (; i < answer.size() && answer[i] != ';'; ++i)
					{
						first_num += answer[i];
					}
					++i;
					for (; i < answer.size(); ++i)
					{
						second_num += answer[i];
					}
					bool is_ok = true;
					try
					{
						x = stoi(first_num);
						y = stoi(second_num);
					}
					catch (const invalid_argument& e)
					{
						is_ok = false;
					}
					if (is_ok)
					{
						return make_pair(x - 1, y - 1);
					}
				}
			}
		}
	}
}

size_t get_cursor_x()
{
	return get_cursor_pos().first;
}

size_t get_cursor_y()
{
	return get_cursor_pos().second;
}

void set_cursor_pos(size_t x, size_t y)
{
	check_position(x, y);
	string h = "\033[" + to_string(y + 1) + ';' + to_string(x + 1) + 'H';
	putstr_(h);
}

//screen
void open_new_screen()
{
	putstr_("\033[?1049h");
	putstr_("\033[H");
	putstr_("\033[2J");
}

void close_screen()
{
	putstr_("\033[?1049l");
}

void clear_screen()
{
	putstr_("\033[2J");
}

void switch_to_big_screen()
{
	sc_->switch_to_big();
}

void switch_to_small_screen()
{
	sc_->switch_to_small();
}

//Color_Exception
Color_Exception::Color_Exception()
{

}

const char* Color_Exception::what() const noexcept
{
	return massage.c_str();
}

/*шпаргалка
—тара€ классика(упоротые 16 цветов):
	\033[30m ... \033[37m Ч “екст(8 базовых)
	\033[90m ... \033[97m Ч “екст(8 €рких)
	\033[40m ... \033[47m Ч ‘он(8 базовых)
	\033[100m ... \033[107m Ч ‘он(8 €рких)
—овременный 8 - bit(256 цветов) Ч всегда через; 5;:
	\033[38; 5; Xm - “екст
	\033[48; 5; Xm - ‘он
‘он—овременный 24 - bit(RGB) Ч всегда через; 2;:
	\033[38; 2; R; G; Bm - “екст
	\033[48; 2; R; G; Bm Ч ‘он
*/

//set colors
void set_color_rgb(crgb f_color, crgb b_color)
{
	string f_command = "\033[38;2;" + to_string((int)f_color.red) + ';' + to_string((int)f_color.green) + ';' + to_string((int)f_color.blue) + 'm';
	string b_command = "\033[48;2;" + to_string((int)b_color.red) + ';' + to_string((int)b_color.green) + ';' + to_string((int)b_color.blue) + 'm';
	putstr_(f_command);
	putstr_(b_command);
}

void set_color_8bit(c8bit f_color, c8bit b_color)
{
	string f_command = "\033[38;5;" + to_string((int)f_color.color) + 'm';
	string b_command = "\033[48;5;" + to_string((int)b_color.color) + 'm';
	putstr_(f_command);
	putstr_(b_command);
}

void set_color_16(c16 f_color, c16 b_color)
{
	string f_command = "\033[";
	if (f_color.color < 8)
	{
		f_command += to_string(30 + (int)f_color.color);
	}
	else
	{
		f_command += to_string(90 + (int)f_color.color);
	}
	f_command += 'm';
	putstr_(f_command);
	string b_command = "\033[";
	if (b_color.color < 8)
	{
		b_command += to_string(40 + (int)b_color.color);
	}
	else
	{
		b_command += to_string(100 + (int)b_color.color);
	}
	b_command += 'm';
	putstr_(b_command);
}