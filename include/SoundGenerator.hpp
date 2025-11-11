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
			void PlayRestBell();
			void PlayWorkBell();

		private:
			bool onGetData(Chunk& data) override;
			void onSeek(sf::Time timeOffset) override;

			void GenerateRestBell();
			void GenerateWorkBell();

			std::vector<sf::Int16> _buffer;
			std::vector<sf::Int16> _restBellbuffer;
			std::vector<sf::Int16> _workBellbuffer;
			SoundFunction _soundFunction;
	};

}