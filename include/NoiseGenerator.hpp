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

namespace zenith {

	class NoiseGenerator : public sf::SoundStream {
		public:
			using FilterState = std::array<float, 4>;
			using FilterFunction = std::function<float (float, FilterState&)>;

			float volume;
			bool muted;
			
			NoiseGenerator(FilterFunction filterFunction);
			
		private:

			enum class FilterIndex {
				IN1, IN2, OUT1, OUT2
			};

			std::vector<sf::Int16> _buffer;
			std::function<float (float, FilterState&)> _filter;
			FilterState _fState;

			bool onGetData(Chunk& data) override;
			void onSeek(sf::Time timeOffset) override;
			
			void GenerateNoise();
			static float BandFilter63Hz(float white, FilterState& fState);
			static float BandFilter125Hz(float white, FilterState& fState);
			static float BandFilter250Hz(float white, FilterState& fState);
			static float BandFilter500Hz(float white, FilterState& fState);
			static float BandFilter1KHz(float white, FilterState& fState);
			static float BandFilter2KHz(float white, FilterState& fState);
			static float BandFilter4KHz(float white, FilterState& fState);
			static float BandFilter8KHz(float white, FilterState& fState);

	};
}