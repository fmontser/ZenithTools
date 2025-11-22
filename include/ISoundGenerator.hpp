/**
 * @file ISoundGenerator.hpp
 * @author https://github.com/fmontser
 * @brief ISoundGenerator interface definition.
 */

#pragma once

namespace zenith {
	/**
	 * @brief Interface for SoundGenerator class to allow mocking
	 */
	class ISoundGenerator {
		public:
			virtual ~ISoundGenerator() = default;
			virtual void PlayRestBell() = 0;
			virtual void PlayWorkBell() = 0;
	};
}