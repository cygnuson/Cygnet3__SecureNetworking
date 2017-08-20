/*

(C) Matthew Swanson

This file is part of Cygnet3.

Cygnet3 is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

Cygnet3 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Cygnet3.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "Random.hpp"

namespace cg {

Random::Random()
{
	
}
bool Random::Coin()
{
	return (UInt() & 1) == 0;
}
uint64_t Random::UInt()
{
	return GetTwister()();
}
uint64_t Random::UInt(
	uint64_t max)
{
	max += 1; //to include the max value.
	return GetTwister()() % max;
}
uint64_t Random::UInt(
	uint64_t min, 
	uint64_t max)
{
	max += 1; //to include the max value.
	return (GetTwister()() % (max - min)) + min;
}

int64_t Random::Int()
{
	return (int64_t) UInt();
}

int64_t Random::Int(int64_t max)
{
	if (max < std::numeric_limits<int64_t>::max())
		++max;
	else
		return UInt();
	return ((int64_t)UInt()) % max;
}

int64_t Random::Int(int64_t min, int64_t max)
{
	return ((int64_t)UInt() % (max - min)) + min;
}

void Random::ReSeedTwister()
{
	GetTwister().seed(
		std::chrono::system_clock::now().time_since_epoch().count());
}

std::mt19937_64 & Random::GetTwister()
{
	static std::mt19937_64 tw;
	return tw;
}

long double Random::Double(
	long double min,
	long double max)
{
	if (min < 0)
		min = 0.0;
	std::uniform_real_distribution<long double> urd(min, max);
	return urd(GetTwister());
}
long double Random::Double(
	long double max)
{
	std::uniform_real_distribution<long double> urd(0.0, max);
	return urd(GetTwister());
}
char Random::UpperAlphaChar()
{
	uint64_t letter = (UInt(0, 25));
	letter += 'A';
	return (char)letter;
}
char Random::UpperHexChar()
{
	auto num = UInt(0, 15);
	if (num < 10)
		return (char) num + '0';
	else
		return (char)(num - 10) + 'A';
}
char Random::UpperAlphaNumChar()
{
	uint64_t letter = (UInt(0, 35));
	if (letter < 26) {
		letter += 'A';
	}
	else {
		letter = (letter - 26) + '0';
	}
	return (char)letter;
}
char Random::LowerAlphaChar()
{
	uint64_t letter = (UInt(0, 25));
	letter += 'a';
	return (char)letter;
}
char Random::MixedAlphaChar()
{
	bool coin = (UInt() % 2) == 0;
	uint64_t letter = (UInt(0, 25));
	letter += coin ? 'a' : 'A';
	return (char)letter;
}
char Random::LowerHexChar()
{
	auto num = UInt(0, 15);
	if (num < 10)
		return (char)num + '0';
	else
		return (char)(num - 10) + 'a';
}
char Random::LowerAlphaNumChar()
{
	uint64_t letter = (UInt(0, 35));
	if (letter < 26) {
		letter += 'a';
	}
	else {
		letter = (letter - 26) + '0';
	}
	return (char)letter;
}
char Random::MixedAlphaNumChar()
{
	bool coin = (UInt() % 2) == 0;
	uint64_t letter = (UInt(0, 35));
	if (letter < 26) {
		letter += coin?'a':'A';
	}
	else {
		letter = (letter - 26) + '0';
	}
	return (char)letter;
}
char Random::NumChar()
{
	return (char)UInt(0, 9) + '0';
}

}