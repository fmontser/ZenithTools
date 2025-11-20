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

	/**
	 * @brief Triggers when an static method is called but no instance of class was created.
	 */
	struct MissingClassInstanceException : public std::logic_error {
		MissingClassInstanceException();
	};

}