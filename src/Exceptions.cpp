#include "Exceptions.hpp"

using namespace zenith;

NotImplementedException::NotImplementedException()
	: std::logic_error("Not implemented") {}

InvalidModeException::InvalidModeException()
	: std::logic_error("Invalid state") {}