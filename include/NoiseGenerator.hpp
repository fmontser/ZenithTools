/**
 * @file NoiseGenerator.hpp
 * @author https://github.com/fmontser
 * @brief NoiseGenerator class definition.
 */

#pragma once

#include "SFML/Audio/SoundStream.hpp"
#include <vector>
#include <functional>
#include <array>
#include <string>

namespace zenith {

	class NoiseGenerator : public sf::SoundStream {
		public:
			float volume;
			bool muted;
			static float masterVolume;
			static bool masterMuted;
				
			NoiseGenerator(float band);
			static const std::array<float, 10> GetDefaultBands();
			const std::string GetBandText() const;
		
		private:
			using FilterState = std::array<float, 6>;
			using FilterFunction = std::function<float (float, FilterState&)>;
		
			enum FilterIndex {
				IN, IN_PREV, IN_PREV2, OUT_PREV, OUT_PREV2, BAND
			};

			std::vector<sf::Int16> _buffer;
			FilterState _fState;
			std::string _bandText;

			bool onGetData(Chunk& data) override;
			void onSeek(sf::Time timeOffset) override;
			
			void SetBandText();

			void GenerateNoise();
			float BandFilter(float white);
			std::array<float,5> CalculateBiquadCoeffs();
			float CalculatePerceptualGain();
	};
}