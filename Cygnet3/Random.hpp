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
#pragma once

#include <random>
#include <chrono>

namespace cg {

/**
 A random number helper.
*/
class Random
{
public:
	Random();
	/**Flip a coin.
	\return True half the time, false the other half.*/
	static bool Coin();
	/**Get an unisiged 64bit integer with an upper limit.
	[0,uint64_max]
	\return An uint64 that is the reandom number.*/
	static uint64_t UInt();
	/**Get an unisiged 64bit integer with an upper limit.
	[0,max]
	\param max The max value for any number generated.
	\return An uint64 that is the reandom number.*/
	static uint64_t UInt(
		uint64_t max);
	/**Get an unisiged 64bit integer with an upper limit and a lower limit.
	[min,max]
	\param min The minimum value of the generated number.
	\param max The max value for any number generated.
	\return An uint64 that is the reandom number.*/
	static uint64_t UInt(
		uint64_t min,
		uint64_t max);
	/**Get an 64bit integer with an upper limit.
	[int64_min,int64_max]
	\return An int64 that is the reandom number.*/
	static int64_t Int();
	/**Get an nisiged 64bit integer with an upper limit.
	[int64_min,max]
	\param max The max value for any number generated.
	\return An int64 that is the reandom number.*/
	static int64_t Int(
		int64_t max);
	/**Get an unisiged 64bit integer with an upper limit and a lower limit.
	[min,max]
	\param min The minimum value of the generated number.
	\param max The max value for any number generated.
	\return An int64 that is the reandom number.*/
	static int64_t Int(
		int64_t min,
		int64_t max);
	/**Reseed the twister used for integer numbers.*/
	static void ReSeedTwister();
	/**Generate a long double number. [min,max]
	\param min The smalles value.
	\param max The largest value.
	\return the random number.*/
	static long double Double(
		long double min,
		long double max);
	/**Generate a long double number. [0,max]
	\param max The largest value.
	\return the random number.*/
	static long double Double(
		long double max);
	/**Get a random upper case alpha character.
	\return a char.*/
	static char UpperAlphaChar();
	/**Get a random upper case HEX character.
	\return a char.*/
	static char UpperHexChar();
	/**Get a random upper case alphanum character.
	\return a char.*/
	static char UpperAlphaNumChar();
	/**Get a random lower case alphanum character.
	\return a char.*/
	static char LowerAlphaChar();
	/**Get a random mixed case alpha character.
	\return a char.*/
	static char MixedAlphaChar();
	/**Get a random lower case hex character.
	\return a char.*/
	static char LowerHexChar();
	/**Get a random lower case alphanum character.
	\return a char.*/
	static char LowerAlphaNumChar();
	/**Get a random mixed case alphanum character.
	\return a char.*/
	static char MixedAlphaNumChar();
	/**Get a random char that is a digit.
	\return a char.*/
	static char NumChar();
private:
	/**Get the single twister instance to use. Its in a functions so that 
	if the twister is never needed, it will never be instanciated.
	\return a reference to the single allowed twister engine.*/
	static std::mt19937_64& GetTwister();
};

}
