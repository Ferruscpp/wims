#pragma once

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdint>
#include <initializer_list>
#include <string>
#include <utility>
#include <chrono>

#if _WIN32
#include <windows.h>
#include <conio.h>
#elif __linux__
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#else
#error "Unknown OS"
#endif

#define DEBUG_MODE_FOR_LINUX 0
#if DEBUG_MODE_FOR_LINUX
#define _WIN32 0
#define __linux__ 1
#endif

namespace ferruscpp
{
	namespace screen
	{
		void check_position(int x, int y);

		bool is_in_screen(int x, int y);

		std::pair<size_t, size_t> get_right_down_angle();

		void open_new_screen();

		void close_screen();

		void clear_screen();

		void switch_to_big_screen();

		void switch_to_small_screen();
	}

	namespace colors
	{
		//cRGB
		struct crgb
		{
			char red, green, blue;
			crgb();
			crgb(char r, char g, char b);
			friend std::istream& operator>>(std::istream& in, crgb& color);
			friend std::ostream& operator<<(std::ostream& out, const crgb& color);
		};
		//c8bit
		struct c8bit
		{
			char color;
			c8bit();
			c8bit(char color_);
			friend std::istream& operator>>(std::istream& in, c8bit& color);
			friend std::ostream& operator>>(std::ostream& out, const c8bit& color);
		};
		//c16
		struct c16
		{
			char color;
			c16();
			c16(int color_);
			c16(char color_);
			c16& operator=(int color_);
			c16& operator=(char color_);
			friend std::istream& operator>>(std::istream& in, c16& color);
			friend std::ostream& operator<<(std::ostream& out, const c16& color);
		};

		//set colors
		void set_color_rgb(crgb f_color, crgb b_color);

		void set_color_8bit(c8bit f_color, c8bit b_color);

		void set_color_16(c16 f_color, c16 b_color);

		//get basic colors
		c16 get_foreground_basic_color();

		c16 get_background_basic_color();
	}

	namespace details
	{
		class Screen_Controller
		{
		private:
			int bottom_x = 0;
			int top_x = 201;
			int bottom_y = 0;
			int top_y = 51;
			colors::c16 foreground_basic = 15;
			colors::c16 background_basic = 0;
		public:
			Screen_Controller() = default;

			void switch_to_small();

			void switch_to_big();

			std::pair<size_t, size_t> get_right_down_angle() const;

			colors::c16 get_foreground_basic_color() const;

			colors::c16 get_background_basic_color() const;

			friend void screen::check_position(int x, int y);
			friend bool screen::is_in_screen(int x, int y);
		};

		extern Screen_Controller* sc_;

#if _WIN32
		class Terminal_Controller
		{
		public:
			Terminal_Controller() = default;
			~Terminal_Controller() = default;
		};
#elif __linux__
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

		extern Terminal_Controller* tc_;
#else
#error "Unknown OS"
#endif
	}

	namespace exceptions
	{
		class Screen_Exception : public std::exception
		{
		private:
			std::string massage = "Error: exite out of screen!!!";
		public:
			Screen_Exception() = default;
			const char* what() const noexcept override;
		};

		class ANSI_Doesnt_Supported_Exception : public std::exception
		{
		private:
			std::string massage = "Error: ANSI doesn't supported in this console!!!";
		public:
			ANSI_Doesnt_Supported_Exception() = default;
			const char* what() const noexcept override;
		};

		class Color_Exception : public std::exception
		{
		private:
			std::string massage = "Error: color convertation error!!!";
		public:
			Color_Exception() = default;
			const char* what() const noexcept override;
		};

#if __linux__
		class Not_Init_Console_Error : public std::exception
		{
		private:
			std::string message = "Error: console can be crashed!!!";
		public:
			Not_Init_Console_Error() = default;
			const char* what() const noexcept override;
		};
#endif
	}

	namespace io
	{
		int putstr_(std::string str);
		
		int getch_();
		
		bool is_hit_();

		void clear_in_buffer();
	}

	namespace files
	{
		void create_folder(std::string folder);
	}

	namespace console
	{
		void start_for_all_OS();
		void end_for_all_OS();

		void init_console_func();
		void end_of_work_console_func();

		void wait(size_t milliseconds);
	}

	namespace cursor
	{
		std::pair<size_t, size_t> get_cursor_pos();

		size_t get_cursor_x();

		size_t get_cursor_y();

		void set_cursor_pos(size_t x, size_t y);
	}
}