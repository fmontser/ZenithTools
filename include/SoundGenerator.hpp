/**
 * @file SoundGenerator.hpp
 * @author https://github.com/fmontser
 * @brief SoundGenerator class definition.
 */

#pragma once

#include "SFML/Audio/SoundStream.hpp"
#include <vector>

namespace zenith {

	class SoundGenerator : public sf::SoundStream {
		public:
			SoundGenerator();

		private:
			bool onGetData(Chunk& data) override;
			void onSeek(sf::Time timeOffset) override;

			double _freq;
			double _amp;
			double _phase;
			double _delta;
			std::vector<sf::Int16> _buffer;

	};

}