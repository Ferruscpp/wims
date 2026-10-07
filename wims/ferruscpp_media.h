#pragma once

#include <iostream>
#include <fstream>
#include <conio.h>
#include "console_func.h"

namespace ferruscpp
{
	
	void scan_raw(std::istream& in, uint32_t& value)
	{
		in.read(reinterpret_cast<char*>(&value), sizeof(value));
	}
	void print_raw(std::ostream& out, const uint32_t& value)
	{
		out.write(reinterpret_cast<const char*>(&value), sizeof(value));
	}

	namespace exceptions
	{
		class File_Not_Found_Exception : public std::exception
		{
		private:
			std::string massage = "Error: file not found!!!";
		public:
			File_Not_Found_Exception()
			{

			}

			const char* what() const noexcept override
			{
				return massage.c_str();
			}
		};

		class Cant_Remove_File_Exception : public std::exception
		{
		private:
			std::string massage = "Error: can't remove file!!!";
		public:
			Cant_Remove_File_Exception()
			{

			}

			const char* what() const noexcept override
			{
				return massage.c_str();
			}
		};

		class Download_Exception : public std::exception
		{
		private:
			std::string massage = "Error: can't download file!!!";
		public:
			Download_Exception()
			{

			}

			const char* what() const noexcept override
			{
				return massage.c_str();
			}
		};

		class File_Name_Exception : public std::exception
		{
		private:
			std::string message = "Error: wrong file name or folder path!!!";
		public:
			File_Name_Exception()
			{

			}
			const char* what() const noexcept override
			{
				return message.c_str();
			}
		};
	}

	namespace points
	{
		struct position
		{
			uint32_t x = 0, y = 0;
			position() = default;
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
					std::clog << "Warning: x or y is negative number!" << std::endl;
				}
				x = x_;
				y = y_;
			}
			//
			friend std::istream& operator>>(std::istream& in, position& pos)
			{
				scan_raw(in, pos.x);
				scan_raw(in, pos.y);
				return in;
			}
			friend std::ofstream& operator<<(std::ofstream& out, const position& pos)
			{
				print_raw(out, pos.x);
				print_raw(out, pos.y);
				return out;
			}
			//
			explicit position(std::pair<size_t, size_t> pos) : x(pos.first), y(pos.second)
			{

			}
			operator std::pair<size_t, size_t>() const
			{
				return std::make_pair((size_t)x, (size_t)y);
			}
		};

		struct window4
		{
			uint32_t x1 = 0, y1 = 0, x2 = console::get_right_down_angle().first, y2 = console::get_right_down_angle().second;
		private:
			class Window_Exception : std::exception
			{
			private:
				std::string massage = "Error: wrong angle position!!!";
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
			window4() = default;
			window4(uint32_t x1_, uint32_t y1_, uint32_t x2_, uint32_t y2_) : x1(x1_), y1(y1_), x2(x2_), y2(y2_)
			{
				check_window4();
			}
			window4(position left_up_angle, position right_down_angle) : x1(left_up_angle.x), y1(left_up_angle.y), x2(right_down_angle.x), y2(right_down_angle.y)
			{
				check_window4();
			}
			window4(std::initializer_list<uint32_t> list)
			{
				x1 = *list.begin();
				y1 = *(list.begin() + 1);
				x2 = *(list.begin() + 2);
				y2 = *(list.begin() + 3);
			}
			//
			friend std::istream& operator>>(std::istream& in, window4& window)
			{
				scan_raw(in, window.x1);
				scan_raw(in, window.y1);
				scan_raw(in, window.x2);
				scan_raw(in, window.y2);
				return in;
			}
			friend std::ofstream& operator<<(std::ofstream& out, const window4& window)
			{
				print_raw(out, window.x1);
				print_raw(out, window.y1);
				print_raw(out, window.x2);
				print_raw(out, window.y2);
				return out;
			}
			//
			void screen_check() const
			{
				console::check_position(x1, y1);
				console::check_position(x2, y2);
			}
		};
	}

	namespace pixels
	{
		//need realizatin
		class Console_Pixel_rgb
		{
		private:

		public:

		};

		//need realizetion
		class Console_Pixel_8bit
		{
		private:

		public:

		};

		class Console_Pixel_16
		{
		private:
			char symbol;
			console::c16 foreground, background;
		public:
			Console_Pixel_16() : symbol(' '), foreground(console::get_foreground_basic_color()), background(console::get_background_basic_color())
			{

			}
			Console_Pixel_16(char symbol_, console::c16 foreground_, console::c16 background_) : symbol(symbol_), foreground(foreground_), background(background_)
			{

			}
			void draw() const
			{
				set_color_16(foreground, background);
				std::string h;
				h += symbol;
				console::putstr_(h);
			}
			void set(char symbol_)
			{
				symbol = symbol_;
			}
			void set(console::c16 foreground_, console::c16 background_)
			{
				foreground = foreground_;
				background = background_;
			}
			void set(char symbol_, console::c16 foreground_, console::c16 background_)
			{
				symbol = symbol_;
				foreground = foreground_;
				background = background_;
			}
			static std::pair<size_t, size_t> get_size()
			{
				return std::make_pair((size_t)1, (size_t)1);
			}
			friend std::istream& operator>>(std::istream& in, Console_Pixel_16& pixel)
			{
				in >> std::noskipws;
				in >> pixel.symbol;
				in >> std::skipws;
				in >> pixel.foreground >> pixel.background;
				return in;
			}
			friend std::ostream& operator<<(std::ostream& out, const Console_Pixel_16& pixel)
			{
				out << pixel.symbol << pixel.foreground << pixel.background;
				return out;
			}
		};

		template<typename Pixel_>
		class Double_Pixel : public Pixel_
		{
		public:
			Double_Pixel() : Pixel_(' ', console::get_foreground_basic_color(), console::get_background_basic_color())
			{

			}
			Double_Pixel(Pixel_ pixel_) : Pixel_(pixel_)
			{

			}
			void draw() const
			{
				Pixel_::draw();
				Pixel_::draw();
			}
			static std::pair<size_t, size_t> get_size()
			{
				std::pair<size_t, size_t> answer(Pixel_::get_size());
				answer.first *= 2;
				return answer;
			}
			friend std::istream& operator>>(std::istream& in, Double_Pixel& pixel)
			{
				in >> static_cast<Pixel_&>(pixel);
				return in;
			}
			friend std::ostream& operator<<(std::ostream& out, const Double_Pixel& pixel)
			{
				out << static_cast<const Pixel_&>(pixel);
				return out;
			}
			~Double_Pixel()
			{

			}
		};
	}

	namespace files
	{
		//functions for files
		template<typename O>
		void update_path(O& object)
		{
			//delete base folder
			int base_folder_length = object.folder_name.size();
			if (base_folder_length <= object.name.size())
			{
				bool is_need_to_delete = true;
				for (int i = 0; i < base_folder_length; ++i)
				{
					if (object.name[i] != object.folder_name[i])
					{
						is_need_to_delete = false;
						break;
					}
				}
				if (is_need_to_delete)
				{
					object.name.erase(0, base_folder_length);
				}
			}
			//delete extension
			int last_position = object.name.size() - object.end_name.size();
			if (0 <= last_position)
			{
				if (object.name[last_position] == '.')
				{
					object.name.erase(last_position, object.end_name.size());
				}
			}
			//clean first '/'
			if (object.name[0] == '/')
			{
				object.name.erase(0, 1);
			}
			//devide on folder and name
			int seporator_position = -1;
			std::string new_name;
			for (int i = object.name.size() - 1; i >= 0; --i)
			{
				if (object.name[i] == '/')
				{
					seporator_position = i;
					break;
				}
			}
			if (seporator_position > 0)
			{
				object.folder_name += '/';
			}
			for (int i = 0; i < object.name.size(); ++i)
			{
				if (i < seporator_position)
				{
					object.folder_name += object.name[i];
				}
				else if (seporator_position < i)
				{
					new_name += object.name[i];
				}
			}
			object.name = new_name;
		}
		template<typename O>
		std::string get_file_name(O& object)
		{
			return object.folder_name + '/' + object.name + object.end_name;
		}
		template<typename O>
		bool exist(O& object)
		{
			std::ifstream in(get_file_name(object));
			bool answer = in.is_open();
			in.close();
			return answer;
		}
		template<typename O>
		void build_file(O& object)
		{
			std::ofstream out(get_file_name(object));
			out.close();
		}
		template<typename O>
		void remove(O& object)
		{
			if (exist(object))
			{
				if (std::remove(get_file_name(object).c_str()) != 0)
				{
					throw exceptions::Cant_Remove_File_Exception();
				}
			}
		}
		template<typename O>
		void rename(O& object, std::string new_name)
		{
			for (size_t i = 0; i < new_name.size(); i++)
			{
				if (new_name[i] == '/')
				{
					throw exceptions::File_Name_Exception();
				}
			}
			remove(object);
			object.name = new_name;
			object.upload();
		}
		template<typename O>
		void change_folder(O& object, std::string new_folder)
		{
			console::create_folder(new_folder);
			remove(object);
			object.folder_name = new_folder;
			object.upload();
		}
	}

	template<typename T>
	class Picture
	{
	private:
		std::string folder_name = "./Pictures";
		std::string name;
		const std::string end_name = ".pic";
		uint32_t size_x = 80;
		uint32_t size_y = 25;
		points::position pixel_size;
		T* pixel_table[300][300];
		//
		class Picture_Exception : public std::exception
		{
		private:
			std::string message = "Error: exite out of picture range!!!";
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
		template<typename O> friend void files::update_path(O& object);
		template<typename O> friend std::string files::get_file_name(O& object);
		template<typename O> friend bool files::exist(O& object);
		template<typename O> friend void files::build_file(O& object);
		//
		void update_pixel_size()
		{
			pixel_size = (points::position)T::get_size();
		}
		//
		void check_picture_position(points::position pic_pos) const
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
		Picture(std::string name_) : name(name_)
		{
			update_pixel_size();
			files::update_path(*this);
			if (!files::exist(*this))
			{
				throw exceptions::File_Not_Found_Exception();
			}
			build_pixel_table();
			download();
		}
		Picture(std::string new_name, size_t x, size_t y) : size_x(x), size_y(y), name(new_name)
		{
			update_pixel_size();
			files::update_path(*this);
			files::build_file(*this);
			build_pixel_table();
		}
		//
		void seg_draw(points::position pic_pos, points::window4 cur_pos) const
		{
			check_picture_position(pic_pos);
			cur_pos.screen_check();
			check_picture_position({ pic_pos.x + (cur_pos.x2 - cur_pos.x1) / pixel_size.x, pic_pos.y + (cur_pos.y2 - cur_pos.y1) / pixel_size.y });
			console::set_cursor_pos(cur_pos.x1, cur_pos.y1);
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
					console::set_cursor_pos(cur_pos.x1, next_position_in_console_y);
				}
			}
		}
		void draw_from(points::position pic_pos) const
		{
			points::position cur_pos(console::get_cursor_pos());
			points::position sec_cur_pos(cur_pos.x + (size_x - pic_pos.x) * pixel_size.x - 1, cur_pos.y + (size_y - pic_pos.y) * pixel_size.y - 1);
			points::window4 window(cur_pos, sec_cur_pos);
			seg_draw(pic_pos, window);
		}
		void draw(points::position cur_pos) const
		{
			seg_draw(points::position(0, 0), points::window4(cur_pos, points::position(cur_pos.x + size_x * pixel_size.x - 1, cur_pos.y + size_y * pixel_size.y - 1)));
		}
		void draw() const
		{
			draw((points::position)console::get_cursor_pos());
		}
		//
		void expanded_draw(points::position pic_pos, points::window4 cur_pos) const
		{
			cur_pos.screen_check();
			console::set_cursor_pos(cur_pos.x1, cur_pos.y1);
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
		void expanded_draw(points::window4 cur_pos) const
		{
			expanded_draw({ 0, 0 }, cur_pos);
		}
		void expanded_draw(points::position right_down_angle) const
		{
			points::window4 cur_pos((points::position)console::get_cursor_pos(), right_down_angle);
			expanded_draw(cur_pos);
		}
		//
		bool is_in_picture(points::position pic_pos) const
		{
			if (pic_pos.x < 0 || size_x <= pic_pos.x)
			{
				return false;
			}
			if (pic_pos.y < 0 || size_y <= pic_pos.y)
			{
				return false;
			}
			return true;
		}
		const std::pair<size_t, size_t> get_pixel_size() const
		{
			return pixel_size;
		}
		T& get_pixel(points::position pic_pos)
		{
			check_picture_position(pic_pos);
			return *(pixel_table[pic_pos.x][pic_pos.y]);
		}
		//
		void download()
		{
			std::ifstream in(files::get_file_name(*this));
			if (!in.is_open())
			{
				throw exceptions::File_Not_Found_Exception();
			}
			scan_raw(in, size_x);
			scan_raw(in, size_y);
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
			std::ofstream out(files::get_file_name(*this));
			print_raw(out, size_x);
			print_raw(out, size_y);
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
		template<typename O> friend void files::remove(O& object);
		//
		template<typename O> friend void files::rename(O& object, std::string new_name);
		template<typename O> friend void files::change_folder(O& object, std::string new_folder);
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
		T* picture;
		uint32_t delay;//in milliseconds
		points::position pic_pos;
		points::window4 cur_pos;
		uint32_t time;
		//
		std::string folder_name = "./Rolls";
		std::string name;
		const std::string end_name = ".roll";
		//
		template<typename O> friend void files::update_path(O& object);
		template<typename O> friend std::string files::get_file_name(O& object);
		template<typename O> friend bool files::exist(O& object);
		template<typename O> friend void files::build_file(O& object);
	public:
		Roll(std::string name_) : name(name_), picture(nullptr)
		{
			download();
		}
		Roll(std::string name_, std::string picture_name, uint32_t delay_, points::position pic_pos_, points::window4 cur_pos_, uint32_t time_)
			: name(name_), picture(new T(picture_name)), delay(delay_), pic_pos(pic_pos_), cur_pos(cur_pos_), time(time_)
		{
			files::update_path(*this);
			upload();
		}
		Roll(std::string name_, std::string picture_name, size_t delay_, size_t time_) : Roll(name_, picture_name, delay_, points::position(), points::window4(), time_)
		{

		}
		//
		void set_delay(size_t new_delay)
		{
			delay = new_delay;
		}
		void set_pic_pos(points::position new_pic_pos)
		{
			pic_pos = new_pic_pos;
		}
		void set_cur_pos(points::window4 new_cur_pos)
		{
			cur_pos = new_cur_pos;
		}
		void set_time(size_t new_time)
		{
			time = new_time;
		}
		//
		size_t& get_delay() const
		{
			return delay;
		}
		points::position& get_pic_pos() const
		{
			return pic_pos;
		}
		points::window4& get_cur_pos() const
		{
			return cur_pos;
		}
		size_t& get_time() const
		{
			return time;
		}
		//
		void download()
		{
			std::ifstream in(files::get_file_name(*this));
			if (!in.is_open())
			{
				throw exceptions::File_Not_Found_Exception();
			}
			scan_raw(in, delay);
			in >> pic_pos >> cur_pos;
			scan_raw(in, time);
			std::string path_to_picture;
			getline(in, path_to_picture);
			if (picture == nullptr)
			{
				picture = new T(path_to_picture);
			}
			else
			{
				if (files::get_file_name(*picture) != path_to_picture)
				{
					throw exceptions::Download_Exception();
				}
				else
				{
					picture->download();
				}
			}
			in.close();
		}
		void upload() const
		{
			std::ofstream out(get_file_name(*this));
			print_raw(out, delay);
			out << pic_pos << cur_pos;
			print_raw(out, time);
			out << get_file_name(*picture);
			out.close();
			return;
		}
		template<typename O> friend void files::remove(O& object);
		//
		template<typename O> friend void files::rename(O& object, std::string new_name);
		template<typename O> friend void files::change_folder(O& object, std::string new_folder);
		//
		~Roll()
		{
			picture->~Picture();
		}
	};

}