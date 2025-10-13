#include "Exceptions.hpp"

using namespace zenith;

NotImplementedException::NotImplementedException()
	: std::logic_error("Not implemented") {}

InvalidStateException::InvalidStateException()
	: std::logic_error("Invalid state") {}