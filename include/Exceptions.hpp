#pragma ONCE
#include <stdexcept>

namespace zenith {

	struct NotImplementedException : public std::logic_error {
		NotImplementedException();
	};

}