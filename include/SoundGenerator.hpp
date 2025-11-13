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

namespace zenith {

	class SoundGenerator : public sf::SoundStream, public ISoundGenerator {
		public:
		
		SoundGenerator();
		void PlayRestBell() override;
		void PlayWorkBell() override;
	
		private:
		bool onGetData(Chunk& data) override;
		void onSeek(sf::Time timeOffset) override;
		
		void GenerateRestBell();
		void GenerateWorkBell();
		void SetAmplitude(int16_t *amplitude, float *volume);
		

		std::vector<sf::Int16> _buffer;
		std::vector<sf::Int16> _restBellbuffer;
		std::vector<sf::Int16> _workBellbuffer;
		
	};

}