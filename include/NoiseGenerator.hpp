/**
 * @file NoiseGenerator.hpp
 * @author https://github.com/fmontser
 * @brief NoiseGenerator class definition.
 */

#pragma once

#include "SFML/Audio/SoundStream.hpp"
#include <vector>
#include <functional>
#include <string>

namespace zenith {

	class NoiseGenerator : public sf::SoundStream {
		public:

			enum class NoiseColor {
				BROWN, PINK, WHITE, BLUE, VIOLET
			};

			NoiseGenerator();
			void PlayNoise(NoiseColor color);
			
			float volume;
			bool muted;

		private:
			bool onGetData(Chunk& data) override;
			void onSeek(sf::Time timeOffset) override;
			
			void GenerateNoise();

			bool _loopMode;
			std::vector<sf::Int16> _buffer;
	};
}