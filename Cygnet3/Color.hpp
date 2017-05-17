#pragma once

#include <cstdint>

#include "OSInclude.hpp"


namespace cg {
namespace colors {

#ifdef _WIN32
enum class ForegroundColor : uint8_t {
	Black = 0,
	DarkBlue = FOREGROUND_BLUE,
	DarkGreen = FOREGROUND_GREEN,
	DarkCyan = FOREGROUND_GREEN | FOREGROUND_BLUE,
	DarkRed = FOREGROUND_RED,
	DarkMagenta = FOREGROUND_RED | FOREGROUND_BLUE,
	DarkYellow = FOREGROUND_RED | FOREGROUND_GREEN,
	DarkGrey = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
	Grey = FOREGROUND_INTENSITY,
	Blue = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
	Green = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
	Cyan = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
	Red = FOREGROUND_INTENSITY | FOREGROUND_RED,
	Magenta = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
	Yellow = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
	White = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
};
enum class BackgroundColor : uint8_t {
	Black = 0,
	DarkBlue = BACKGROUND_BLUE,
	DarkGreen = BACKGROUND_GREEN,
	DarkCyan = BACKGROUND_GREEN | BACKGROUND_BLUE,
	DarkRed = BACKGROUND_RED,
	DarkMagenta = BACKGROUND_RED | BACKGROUND_BLUE,
	DarkYellow = BACKGROUND_RED | BACKGROUND_GREEN,
	DarkGrey = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
	Grey = BACKGROUND_INTENSITY,
	Blue = BACKGROUND_INTENSITY | BACKGROUND_BLUE,
	Green = BACKGROUND_INTENSITY | BACKGROUND_GREEN,
	Cyan = BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE,
	Red = BACKGROUND_INTENSITY | BACKGROUND_RED,
	Magenta = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE,
	Yellow = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN,
	White = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
};

#else
/*linux colors here.*/

enum class ForegroundColor : uint8_t {
	Black = 30,
	Blue = 94,
	Green = 92,
	Cyan = 96,
	Red = 91,
	Magenta = 95,
	Yellow = 93,
	DarkGrey = 90,
	Grey = 37,
	DarkBlue = 34,
	DarkGreen = 32,
	DarkCyan = 36,
	DarkRed = 31,
	DarkMagenta = 35,
	DarkYellow = 33,
	White = 97,
};
enum class BackgroundColor : uint8_t {
	Black = 40,
	DarkBlue = 44,
	DarkGreen = 42,
	DarkCyan = 46,
	DarkRed = 41,
	DarkMagenta = 45,
	DarkYellow = 43,
	DarkGrey = 100,
	Grey = 47,
	Blue = 104,
	Green = 102,
	Cyan = 106,
	Red = 101,
	Magenta = 105,
	Yellow = 103,
	White = 107,
};
#endif

/**A pair of colors with easy or options.*/
struct ColorPair
{
	/**Create the pair
	\param fore The forground color.
	\param back The background color.*/
	ColorPair(ForegroundColor fore, BackgroundColor back)
        :m_back(back), m_fore(fore){}
	/**Create a default color pair.*/
	ColorPair()
        :m_back(BackgroundColor::Black),m_fore(ForegroundColor::Green) {}
	/**The background color.*/
	BackgroundColor m_back;
	/**The foreground color.*/
	ForegroundColor m_fore;
	/**Or both colors together.
	\return The result.*/
	uint8_t Or()
	{
		return uint8_t(m_fore) | uint8_t(m_back) ;
	}
};

}
}
