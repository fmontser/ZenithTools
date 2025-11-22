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
	/**
	 * @brief Generates and filters a stream of white noise
	 */
	class NoiseGenerator : public sf::SoundStream {
		public:
			float volume;  ///< Holds the volume value for this generator on SFML scale 0.0f-100.0f
			bool muted;    ///< Holds mute state for this generator
			
			static float masterVolume;  ///< Holds the master volume for all noise generators
			static bool masterMuted;    ///< Holds the mute state for all noise generators (exclusive)
			
			/**
			 * @brief Constructs a new noise generator
			 * @param band Noise will be generate arround that band frequency
			 * @note Audible frequencies on most hardware is about 63hz-24KHz
			 */
			NoiseGenerator(float band);

			/**
			 * @brief Destructor
			 * @note It will stop the sound thread before destroying the object
			 */
			~NoiseGenerator();

			/**
			 * @brief Obtain the default ten bands for noise generation
			 */
			static const std::array<float, 10> GetDefaultBands();

			/**
			 * @brief Obtain this object band on text format
			 * @example "63Hz"
			 */
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