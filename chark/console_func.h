#pragma once

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <chrono>

using namespace std;

#define DEBUG_MODE_FOR_LINUX 0
#if DEBUG_MODE_FOR_LINUX
#define _WIN32 0
#define __linux__ 1
#endif

//cRGB
struct crgb
{
	char red, green, blue;
	crgb();
	crgb(char r, char g, char b);
	friend ifstream& operator>>(ifstream& fin, crgb& color);
	friend ostream& operator<<(ostream& fout, const crgb& color);
};
//c8bit
struct c8bit
{
	char color;
	c8bit();
	c8bit(char color_);
	friend ifstream& operator>>(ifstream& fin, c8bit& color);
	friend ostream& operator>>(ostream& fout, const c8bit& color);
};
//c16
struct c16
{
	int color;
	c16();
	c16(int color_);
	c16& operator=(int color_);
	friend ifstream& operator>>(ifstream& fin, c16& color);
	friend ofstream& operator<<(ofstream& out, const c16& color);
};

class Screen_Controller
{
private:
	int bottom_x = 0;
	int top_x = 201;
	int bottom_y = 0;
	int top_y = 51;
	c16 foreground_basic = 15;
	c16 background_basic = 0;
public:
	Screen_Controller();

	void switch_to_small();

	void switch_to_big();

	c16 get_foreground_basic_color() const;

	c16 get_background_basic_color() const;

	friend void check_position(int x, int y);
};

void check_position(int x, int y);

c16 get_foreground_bassic_color();

c16 get_background_bassic_color();

extern Screen_Controller* sc_;

void start_for_all_OS();
void end_for_all_OS();

#if _WIN32
#include <windows.h>
#include <conio.h>
//-----------------------------------------------------
class Terminal_Controller
{
public:
	Terminal_Controller();
	~Terminal_Controller();
};
//-----------------------------------------------------
int putstr_(string str);
//-----------------------------------------------------
int getch_();
//-----------------------------------------------------
bool is_hit_();
//-----------------------------------------------------
void wait(int milliseconds);
//-----------------------------------------------------
void clear_in_buffer();
//-----------------------------------------------------
void init_console_func();
//-----------------------------------------------------
void end_of_work_console_func();
#elif __linux__
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
//-----------------------------------------------------
class Terminal_Controller
{
private:
	static int counter;
	termios newin, oldin;
public:
	Terminal_Controller();
	static int get_info();
	~Terminal_Controller();
};
//-----------------------------------------------------
class Not_Init_Console_Error : public exception
{
private:
	string message = "Error: console can be crashed!!!";
public:
	Not_Init_Console_Error();
	const char* what() const noexcept override;
};
//-----------------------------------------------------
int putstr_(string str);
//-----------------------------------------------------
int getch_();
//-----------------------------------------------------
bool is_hit();
//-----------------------------------------------------
void wait(int milliseconds);
//-----------------------------------------------------
void clear_in_buffer();
//-----------------------------------------------------
extern Terminal_Controller* tc_;

void init_console_func();

void end_of_work_console_func();
//-----------------------------------------------------
#else
#error "Unknown OS"
#endif

class Screen_Exception : public exception
{
private:
	string massage = "Exite out of screen!!!";
public:
	Screen_Exception();
	const char* what() const noexcept override;
};

class ANSI_Doesnt_Supported_Exception : public exception
{
private:
	string massage = "ANSI doesn't supported in this console!!!";
public:
	ANSI_Doesnt_Supported_Exception();
	const char* what() const noexcept override;
};

//cursor position
pair<size_t, size_t> get_cursor_pos();

size_t get_cursor_x();

size_t get_cursor_y();

void set_cursor_pos(size_t x, size_t y);

//screen
void open_new_screen();

void close_screen();

void clear_screen();

void switch_to_big_screen();

void switch_to_small_screen();

//Color_Exception
class Color_Exception : public exception
{
private:
	string massage = "Error with color convertation!!!";
public:
	Color_Exception();
	const char* what() const noexcept override;
};

//set colors
void set_color_rgb(crgb f_color, crgb b_color);

void set_color_8bit(c8bit f_color, c8bit b_color);

void set_color_16(c16 f_color, c16 b_color);