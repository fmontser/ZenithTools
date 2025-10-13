#pragma once
#include <stdexcept>

namespace zenith {

	struct NotImplementedException : public std::logic_error {
		NotImplementedException();
	};

	struct InvalidStateException : public std::logic_error {
		InvalidStateException();
	};

}