#include "Exception.hpp"

namespace cg {
namespace ex {

std::string ExNullPointer::ToString() const
{
	return "The poitner is null.";
}
std::string ExNotImplemented::ToString() const
{
	return "The function or class is not implemented.";
}
std::string ExIndexBounds::ToString() const
{
	return "The index is out of bounds.";
}
std::string ExDoesNotOwn::ToString() const
{
	return "The data is not owned by the object.";
}


std::string ExStillRunning::ToString() const
{
	return "The object is still running.";
}

std::string ExNotRunning::ToString() const
{
	return "The object is not running.";
}

}
}
