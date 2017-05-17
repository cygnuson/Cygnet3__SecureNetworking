#pragma once

#include "StringHelpers.hpp"
#include "ArrayView.hpp"

#include <istream>
#include <sstream>
#include <memory>
#include <cctype>

namespace cg {


/**Advance a istream past all X type chars at the front of the stream.
\param strm The istream to advance to the first instance of stopChar.
\param stopChar The char to stop advancing when encountered.
\param keepLast True to keep the stopChar in the stream.
\return The amount of units that were removed from the stream.*/
inline std::size_t SkipUntil(
	std::istream& strm,
	char stopChar,
	bool keepLast = true)
{
	std::size_t hits = 0;
	while (strm.good() && strm.get() != stopChar)
		++hits;
	if (keepLast)
		strm.putback(stopChar);
	return hits;
}
/**Advance a istream past all X type chars at the front of the stream.
\param strm The istream to advance to the first instance of stopChar.
\param stopChars The chars to stop advancing when encountered.
\param keepLast True to keep the stopChar in the stream.
\return The amount of units that were removed from the stream.*/
inline std::size_t SkipUntil(
	std::istream& strm,
	const std::string& stopChars,
	bool keepLast = true)
{
	char ch = 0;
	std::size_t hits = 0;
	while (strm.good())
	{
		ch = (char)strm.get();
		if (stopChars.find(ch) != std::string::npos)
		{
			break;
		}
		++hits;
	}
	if (keepLast)
		strm.putback(ch);
	return hits;
}

/**Advance the stream untill the `pred` parameter callable object returns
`predStop` on a char.
\param strm The istream to advance..
\param pred A callable that returns `predStop` when its time to stop skipping.
\param predStop The value that pred should return when its time to stop.
\return The amount of units that were removed from the stream.*/
template<typename Pred>
inline std::size_t SkipUntil(
	std::istream& strm,
	Pred pred,
	bool predStop)
{
	std::size_t hits = 0;
	while (strm.good() && (pred(strm.peek()) != 0) != predStop)
		strm.ignore();

	return hits;
}

/**Get the contents of a stream up till `pred` returns `predStop` on a char.
\param strm The stream to get from.
\param pred The callable object that will be called with each char.
\param predStop The value that when `pred` returns it, the oepration stopss.
\return A string containing the cahrs that were extracted.*/
template<typename Pred>
std::string ExtractUntil(
	std::istream& strm,
	Pred pred,
	bool predStop)
{
	std::string ret;
	while (strm.good() && (pred(strm.peek()) != 0) != predStop)
		ret += strm.get();
	return ret;
}
/**Extract chars from a stream untill `stopChar` is seen.
\param strm The istream to extract from.
\param stopChar The char to stop extracting when encountered.  The stopChar
will remain in the stream when the function returns.
\param keepLast True to keep the stopChar in the stream.
\param skipAmount The amount of stop chars to skip before returning.
\return A string that is the chars removed.*/
inline std::string ExtractUntil(
	std::istream& strm,
	char stopChar,
	bool keepLast = true,
	int skipAmount = 0)
{
	std::string ret;
	while (strm.good())
	{
		ret += (char)strm.get();
		if (strm.peek() == stopChar && skipAmount != 0)
			--skipAmount;
		if (strm.peek() == stopChar && skipAmount == 0)
			break;
	}
	if (!keepLast)
		ret += (char)strm.get();
	return ret;
}

/**Extract chars from a stream untill one of the `stopChars` is seen.
\param strm The istream to extract from.
\param stopChars The char to stop extracting when encountered.  The stopChar
will remain in the stream when the function returns.
\param keepLast True to keep the stopChar in the stream.
\param skipAmount The amount of stop chars to skip before returning.
\return A string that is the chars removed.*/
inline std::string ExtractUntil(
	std::istream& strm,
	const std::string& stopChars,
	bool keepLast = true,
	int skipAmount = 0)
{
	std::string ret;
	char ch = 0;
	while (strm.good())
	{
		ch = (char)strm.get();
		if (stopChars.find(ch) != std::string::npos)
		{
			if (skipAmount == 0)
			{
				strm.putback(ch);
				break;
			}
			else
				--skipAmount;
		}
		ret += ch;
	}
	if (!keepLast)
		ret += (char)strm.get();
	return ret;
}
/**Extract chars from a stream untill one of the `stopChars` is seen.
\param strm The istream to extract from.
\param stopChars The char to stop extracting when encountered.  The stopChar
will remain in the stream when the function returns.
\param keepLast True to keep the stopChar in the stream.
\param skipAmount The amount of stop chars to skip before returning.
\return A string that is the chars removed.*/
inline std::string ExtractUntil(
	std::istream& strm,
	const char* stopChars,
	bool keepLast = true,
	int skipAmount = 0)
{
	return ExtractUntil(strm, std::string(stopChars), keepLast, skipAmount);
}
/**Get the remainter of the stream as a string.
\param strm The stream to get the letters from.
\return The string that is whatever is left in the stream.*/
inline std::string ExtractRest(std::istream& strm)
{
	std::stringstream ss;
	ss << strm.rdbuf();
	return ss.str();
}
/**Determine which char comes first in a stream.
\param strm The stream.
\param chars The chars to look for.
\return The char that comes first in the stream or 0 if no chars occure.*/
inline char FirstOccurance(std::istream& strm, const std::string& chars)
{
	auto sPos = strm.tellg();
	while (strm.good())
	{
		char ch = strm.peek();
		if (chars.find_first_of(ch) != std::string::npos)
		{
			strm.seekg(sPos);
			return ch;
		}
		else
			strm.ignore();
	}
	strm.seekg(sPos);
	return 0;
}
/**Trim all white space from the front of the stream.*/
inline void TrimWhitespace(std::istream& strm)
{
	SkipUntil(strm, std::isspace, false);
}

/**Seek to a position in the file (write). This will open the file.
\param pos The postion to seek to.*/
inline void SeekW(std::ostream& stream, std::streamoff pos)
{
	stream.seekp(pos, std::ios::beg);
}
/**Seek to a position in the file (read). This will open the file.
\param pos The postion to seek to.*/
inline void SeekR(std::istream& stream, std::streamoff pos)
{
	stream.seekg(pos, std::ios::beg);
}
/**Seek to a position in the file (read.write) This will open the file.
\param pos The postion to seek to.*/
inline void Seek(std::iostream& stream, std::streamoff pos)
{
	SeekR(stream, pos);
	SeekW(stream, pos);
}
/**Seek to a specific line.
\param expand True to expand the file with lines untill the line is
reached.
\param pos The list to seek to.  If `expand` is false, and `pos` is greater
than the last line, then the stream will seek to the end of the stream.
\return true if the line was found, false if the stream is at the end.*/
inline bool SeekLine(std::iostream& stream, std::streamoff pos,
	bool expand = false)
{
	bool expanded = false;
	for (std::streamoff i = 0; i < pos - 1; ++i)
	{
		std::string line;
		if (!getline(stream, line))
		{
			if (expand)
			{
				expanded = true;
				stream.clear();
				stream.seekp(0, stream.end);
				while (i++ < pos)
					stream << std::endl;
				stream.seekg(0, stream.end);
				break;
			}
			return false;
		}
	}
	Seek(stream, stream.tellg());
	return true;
}

/**offset the stream position (write) reletive to its current position.
\param amt The amount to offset.
\param fillByte The byte to fill empty space with when passing the EOF.*/
inline void OffsetW(std::ostream& stream, std::ptrdiff_t amt,
	char fillByte = ' ')
{
	auto cPos = stream.tellp();
	stream.seekp(0, std::ios::end);
	auto size = stream.tellp();
	auto nCPos = cPos;
	nCPos += amt;
	if (nCPos > size)
	{
		auto addSize = nCPos - size;
		stream.seekp(0, std::ios::end);
		while (addSize-- > 0)
			stream << fillByte;
		stream.seekp(0, std::ios::end);
	}
	else
	{
		stream.seekp(cPos, std::ios::beg);
		stream.seekp(amt, std::ios::cur);
	}
}
/**offset the stream position (read) reletive to its current position.
\param amt The amount to offset.*/
inline void OffsetR(std::istream& stream, std::ptrdiff_t amt)
{
	stream.seekg(amt, std::ios::cur);
}
/**offset the stream position (write/read) reletive to its current
position.  The read position will stop at the end of the file, but the
write position will expand the file after the read position has stopped at
the old-end-of-file.
\param amt The amount to offset.
\param fillByte The byte to fill empty space with when passing the EOF.*/
inline void Offset(std::iostream& stream, std::ptrdiff_t amt,
	char fillByte = ' ')
{
	OffsetR(stream, amt);
	OffsetW(stream, amt, fillByte);
}
/**Forward the stream untill a specific string is reached.
\param stream The stream to search.
\param term The search term.
\return True if the term was found.*/
inline bool SkipUntilString(std::istream& stream,
	const std::string& term)
{
	while (stream.good())
	{
		if (!stream.ignore(std::uint32_t(1) << 31, term[0]).good())
		{
			/*read EOF without findind the delim.*/
			return false;
		}
		if (!stream.good())
			break;
		stream.putback(term[0]);
		std::string data = term;
		stream.read(&data[0], data.size());
		if (data == term)
		{
			std::ptrdiff_t off = data.size();
			off = -off;
			stream.seekg(off, std::ios::cur);
			return true;
		}
		else
		{
			std::ptrdiff_t off = data.size() - 1;
			off = -off;
			stream.seekg(off, std::ios::cur);
		}
	}
	return false;
}

/**Forward the stream untill a specific string is reached.
\param stream The stream to search.
\param term The search term.
\return True if the term was found.*/
inline bool SkipPastString(std::istream& stream,
	const std::string& term)
{
	if (!SkipUntilString(stream, term))
		return false;
	else
		stream.seekg(term.size(), std::ios::cur);
	return true;
}

/**Forward the stream to the first line with the term on it.
\param stream The stream to search.
\param term The search term.
\param pastLine True to end on the line after the line for which `term` was
found.
\return True if the term was found.*/
inline bool SkipToLine(std::istream& stream,
	const std::string& term, bool pastLine = false)
{
	while (stream.good())
	{
		std::string tStr;
		std::getline(stream, tStr);
		auto pos = tStr.find(term);
		if (pos != std::string::npos)
		{
			if (!pastLine)
			{
				std::ptrdiff_t move = tStr.size();
				stream.seekg(-move, std::ios::cur);
				uint64_t pos = stream.tellg();
				while (stream.peek() != '\n' && pos != 0)
				{
					stream.seekg(-1, std::ios::cur);
					pos = stream.tellg();
				}
				if (pos != 0)
					stream.ignore();
			}
			return true;
		}
	}
	return false;
}

/**Forward the stream to the first line with the term on it.
\param stream The stream to search.
\param term The search term.
\return True if the term was found.*/
inline bool SkipPastLine(std::istream& stream,
	const std::string& term)
{
	return SkipToLine(stream, term, true);
}

/**Shift the data to the right in the file.
\param pos The position to shift from.
\param amt The amount to shift.
\param stream The stream to shift.
\param fillByte The byte that will fill in for the emptied space.
\param arrayLike False if the data shifted off the right edge should
expand the file to keep the data.*/
inline void ShiftRight(std::iostream& stream, std::size_t pos, std::size_t amt,
	char fillByte = 0, bool arrayLike = false)
{
	stream.seekg(0, std::ios::end);
	std::size_t size = std::size_t(stream.tellg()) - pos;
	cg::ArrayView data(size);
	stream.seekg(pos);
	stream.read(data.data(), size);
	stream.seekp(pos + amt);
	if (arrayLike)
		stream.write(data.data(), size - amt);
	else
		stream.write(data.data(), size);
	stream.seekp(pos);
	while (amt-- > 0)
		stream.write(&fillByte, 1);
}
/**Shift the data to the left in the file.
\param pos The position to shift from.
\param stream The stream to shift.
\param amt The amount to shift.
\param fillByte The byte that will fill in for the emptied space.*/
inline void ShiftLeft(std::iostream& stream, std::size_t pos, std::size_t amt,
	char fillByte = 0)
{
	auto size = pos;
	cg::ArrayView data(size);
	stream.seekg(amt);
	stream.read(data.data(), size - amt);
	stream.seekp(0);
	stream.write(data.data(), size - amt);
	stream.seekp(pos);
	std::string s(amt, fillByte + 1);
	stream.write(s.c_str(), amt);
}

/**Forward the stream untill a specific string is reached.
\param stream The stream to search.
\param term The search term.
\param keepLast Keep the `term` in the stream.
\return Everythign in the stream untill the term was found. An empty string if
it was not found.*/
inline std::string ExtractUntilString(std::istream& stream,
	const std::string& term, bool keepLast = true)
{
	std::string ret;
	while (stream.good())
	{
		std::string str;
		std::getline(stream, str, term[0]);
		if (!stream.good())
			break;
		stream.putback(term[0]);
		std::string data = term;
		stream.read(&data[0], data.size());
		ret += str;
		if (data == term)
		{
			if (keepLast)
			{
				auto pos = stream.tellg();
				pos -= data.size();
				stream.seekg(pos,std::ios::beg);
			}
			break;

		}
		else
		{
			ret += term[0];
			std::ptrdiff_t off = data.size() - 1;
			off = -off;
			stream.seekg(off, std::ios::cur);
		}

	}
	return ret;
}
/**Extract the next available peice of data between two brackets.  The stream
will be moved so that the 'lBracket' is the next thing to come out.
\param stream The stream to get from.
\param lBracket the left type of bracket.
\param rBracket the right type of bracket.
\param keepLast True to keep the brackets and its contents in the stream.
\param keepBrackets True to have to return value keep its brackets.
\return The next thing between an 'lBracket' and an 'rBracket' or a blank
string in the event that no bracket pair exists or just one of the brackets
does not exist. If the string returned is  blank, then the stream will be
moved back to the begining.*/
inline std::string ExtractTag(std::istream& stream,
	const std::string& lBracket, const std::string& rBracket,
	bool keepLast = false, bool keepBrackets = false)
{
	cg::ExtractUntilString(stream, lBracket, false);
	if (!stream.good())
	{
		stream.seekg(0, std::ios::beg);
		return "";
	}
	auto str = cg::ExtractUntilString(stream, rBracket, true);
	if (!stream.good())
	{
		stream.seekg(0, std::ios::beg);
		return "";
	}
	if (keepLast)
	{
		std::ptrdiff_t strmSeek = (str.size() + lBracket.size());
		stream.seekg(-strmSeek, std::ios::cur);
	}
	else
		/*toss the final bracket.*/
		stream.ignore(rBracket.size());
	if (keepBrackets)
		return lBracket + str + rBracket;
	return str;
}
}
