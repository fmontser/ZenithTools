/**
 * @file SoundGenerator.hpp
 * @author https://github.com/fmontser
 * @brief SoundGenerator class definition.
 */

#pragma once

#include "ISoundGenerator.hpp"
#include "SFML/Audio/SoundStream.hpp"
#include <vector>
#include <functional>
#include <string>

namespace zenith {

	class SoundGenerator : public sf::SoundStream, public ISoundGenerator {
		public:

			enum class NoiseColor {
				BROWN, PINK, WHITE, BLUE, VIOLET
			};

			SoundGenerator();
			void PlayRestBell() override;
			void PlayWorkBell() override;
			void PlayNoise(NoiseColor color);
			
			const std::string GetNoiseColorName() const;
			const NoiseColor GetNoiseColor() const;
			void SetNoiseColor(NoiseColor color);

			float volume;
			bool muted;

		private:
			bool onGetData(Chunk& data) override;
			void onSeek(sf::Time timeOffset) override;
			
			void GenerateRestBell();
			void GenerateWorkBell();
			void GenerateNoise();

			void GenerateWhite(double duration);
			void GenerateBrown(double duration);

			bool _loopMode;
			NoiseColor _noiseColor;
			std::string _noiseColorName;
			std::vector<sf::Int16> _buffer;
			std::vector<sf::Int16> _restBellbuffer;
			std::vector<sf::Int16> _workBellbuffer;
			std::vector<sf::Int16> _brownNoiseBuffer;
			std::vector<sf::Int16> _pinkNoiseBuffer;
			std::vector<sf::Int16> _whiteNoiseBuffer;
			std::vector<sf::Int16> _blueNoiseBuffer;
			std::vector<sf::Int16> _violetNoiseBuffer;
		
	};
}