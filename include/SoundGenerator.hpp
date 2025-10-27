/**
 * @file SoundGenerator.hpp
 * @author https://github.com/fmontser
 * @brief SoundGenerator class definition.
 */

#pragma once

#include "SFML/Audio/SoundStream.hpp"
#include <vector>
#include <functional>

namespace zenith {

	using SoundFunction = std::function<void (const std::size_t frameCount)>;

	class SoundGenerator : public sf::SoundStream {
		public:
			SoundGenerator();
			void PlayPeriodBell();

		private:
			bool onGetData(Chunk& data) override;
			void onSeek(sf::Time timeOffset) override;

			std::vector<sf::Int16> _buffer;
			SoundFunction _soundFunction;
	};

}