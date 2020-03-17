#pragma once

#include <Windows.h>
#include <iostream>


#define intense FOREGROUND_INTENSITY |
#define background * 16
#define black 0
#define blue 1
#define green 2
#define cyan 3
#define red 4
#define purple 5
#define yellow 6
#define white 7
#define grey intense black
#define underscore | COMMON_LVB_UNDERSCORE
#define top_line | COMMON_LVB_GRID_HORIZONTAL
#define left_line | COMMON_LVB_GRID_LVERTICAL

struct ColoredConsole
{
	enum class Color {
		Black, Blue, Green, Cyan, Red, Purple, Yellow, White, Grey
	};

	HANDLE hConsole = nullptr;

	ColoredConsole()
		: hConsole(GetStdHandle(STD_OUTPUT_HANDLE))
	{
	}

	~ColoredConsole()
	{
		SetConsoleTextAttribute(hConsole, getColor(Color::White));
	}

	void cout(std::string str, Color color) const
	{
		SetConsoleTextAttribute(hConsole, getColor(color));
		std::cout << str << "\n";
	}

	int getColor(Color color) const
	{
		switch (color)
		{
		case Color::Black:	return 0;
		case Color::Blue:	return 1;
		case Color::Green:	return 2;
		case Color::Cyan:	return 3;
		case Color::Red:	return 4;
		case Color::Purple:	return 5;
		case Color::Yellow:	return 6;
		case Color::White:	return 7;
		case Color::Grey:	return grey;
		}
	}
};