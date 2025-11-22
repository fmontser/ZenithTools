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

	/**
	 * @brief Generates and plays sound effects for the gui
	 */
	class SoundGenerator : public sf::SoundStream, public ISoundGenerator {
		public:
			/**
			 * @brief Default constructor
			 */
			SoundGenerator();

			/**
			 * @brief Default destructor
			 */
			~SoundGenerator();

			/**
			 * @brief Play the rest period sound effect
			 */
			void PlayRestBell() override;

			/**
			 * @brief Play the work period sound effect
			 */
			void PlayWorkBell() override;

			float volume;  ///< Holds the volume value for this generator on SFML scale 0.0f-100.0f

		private:
			bool onGetData(Chunk& data) override;
			void onSeek(sf::Time timeOffset) override;
			
			void GenerateRestBell();
			void GenerateWorkBell();

			std::vector<sf::Int16> _buffer;
			std::vector<sf::Int16> _restBellbuffer;
			std::vector<sf::Int16> _workBellbuffer;
	};
}