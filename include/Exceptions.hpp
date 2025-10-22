#pragma once
#include <stdexcept>

namespace zenith {

	struct NotImplementedException : public std::logic_error {
		NotImplementedException();
	};

	struct InvalidModeException : public std::logic_error {
		InvalidModeException();
	};

}