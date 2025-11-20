#include "Exceptions.hpp"

using namespace zenith;

NotImplementedException::NotImplementedException()
	: std::logic_error("Not implemented") {}

InvalidModeException::InvalidModeException()
	: std::logic_error("Invalid state") {}

MissingClassInstanceException::MissingClassInstanceException()
	: std::logic_error("Missing class Instance") {}
