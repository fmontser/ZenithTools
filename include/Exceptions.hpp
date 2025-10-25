/**
 * @file Exceptions.hpp
 * @author https://github.com/fmontser
 * @brief Exceptions class definition.
 */

#pragma once
#include <stdexcept>

namespace zenith {

	/**
	 * @brief Stub for non implemented code.
	 */
	struct NotImplementedException : public std::logic_error {
		NotImplementedException();
	};

	/**
	 * @brief Triggers when an invalid MODE or STATE is provided.
	 */
	struct InvalidModeException : public std::logic_error {
		InvalidModeException();
	};

}