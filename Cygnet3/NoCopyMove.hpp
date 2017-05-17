#pragma once


namespace cg {
/**Disable the ability to copy an object.*/
struct NoCopy
{
	NoCopy() {};
	NoCopy(const NoCopy& other) = delete;
	void operator=(const NoCopy& other) = delete;
};
/**Disable the ability to move an object.*/
struct NoMove
{
	NoMove() {};
	NoMove(NoMove&& other) = delete;
	void operator=(NoMove&& other) = delete;
};

}