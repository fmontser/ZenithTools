#include "SoundGenerator.hpp"
#include "Exceptions.hpp"
#include <cmath>


using namespace zenith;

SoundGenerator::SoundGenerator() : sf::SoundStream() {

	_freq = 440.0f;
	_amp = 3000.0f;
	_phase = 100.0f;
	_delta = (_freq * 2 * 3.14159265358979323846) / 48000.0f;
	initialize(2, 48000);

}


bool SoundGenerator::onGetData(Chunk &data) {
	const std::size_t frameCount = 1024;
	const std::size_t sampleCount = frameCount * 2;

	_buffer.resize(sampleCount);

	for (std::size_t i = 0; i < frameCount; ++i) {
		sf::Int16 sample = _amp * static_cast<sf::Int16>((rand() % 65536) - 32768);
		
		//TODO tono simple ...static_cast<sf::Int16>(_amp * std::sin(_phase));

		_buffer[i * 2]     = sample; // canal izquierdo
		_buffer[i * 2 + 1] = sample; // canal derecho
		_phase += _delta;
		if (_phase > 2 * 3.14159265358979323846)
			_phase -= 2 * 3.14159265358979323846;

		_delta = (_freq * 2 * 3.14159265358979323846) / 48000.0f;
	}

	data.samples = _buffer.data();
	data.sampleCount = sampleCount;
	return true;
}

void SoundGenerator::onSeek(sf::Time timeOffset) {}
