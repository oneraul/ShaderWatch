#pragma once

#include <Windows.h>
#include <iostream>

namespace rmkl {
	class ColoredConsole
	{
	public:
		enum class Color {
			Black, Blue, Green, Cyan, Red, Purple, Yellow, White, Grey
		};

	public:
		ColoredConsole()
			: hConsole(GetStdHandle(STD_OUTPUT_HANDLE))
		{
		}

		~ColoredConsole()
		{
			SetConsoleTextAttribute(hConsole, getColor(Color::White));
		}

		void print(std::string str, Color color) const
		{
			SetConsoleTextAttribute(hConsole, getColor(color));
			std::cout << str;
		}

	private:
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
			case Color::Grey:	return 8;
			}
		}

	private:
		HANDLE hConsole = nullptr;
	};
}