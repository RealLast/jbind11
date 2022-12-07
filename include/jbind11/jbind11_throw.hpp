#pragma once
#include <exception>
#include <sstream>


#define JBIND_THROW(msg)\
{\
	std::ostringstream ex_str;\
	ex_str << msg;\
	ex_str << "(" << __FILE__ << " " << __LINE__ << ")";\
	throw std::runtime_error(ex_str.str()); \
}