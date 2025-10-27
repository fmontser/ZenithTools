#include "SoundGenerator.hpp"
#include "Exceptions.hpp"
#include <cmath>

#include <thread>

using namespace zenith;

SoundGenerator::SoundGenerator() : sf::SoundStream() {
	initialize(2, 48000);
}

void SoundGenerator::PlayPeriodBell() {
	

	double frequency = 500.0f;
	double amplitude = 16000.0f;
	double phase = 0.0f;
	double delta = (frequency * 2 * 3.14159265358979323846) / 48000.0f;

	//TODO crear funcion para automatizar la duracion
	int duration = 55;


	_soundFunction = [=](const std::size_t frameCount) mutable {
		if (duration <= 0) {
			_soundFunction = nullptr;
			return;
		}
		duration--;

		for (std::size_t i = 0; i < frameCount; ++i) {
			sf::Int16 sample = static_cast<sf::Int16>(amplitude * std::sin(phase));
			_buffer[i * 2]     = sample; //L-channel
			_buffer[i * 2 + 1] = sample; //R-channel
			phase += delta;
			if (phase > 2 * 3.14159265358979323846)
				phase -= 2 * 3.14159265358979323846;

			frequency -= 0.02f;
			if (frequency < 0)
				frequency = 0;
			delta = (frequency * 2 * 3.14159265358979323846) / 48000.0f;
		}
	};
	play();
}

bool SoundGenerator::onGetData(Chunk &data) {
	const std::size_t frameCount = 1024;
	const std::size_t sampleCount = frameCount * 2;

	_buffer.clear();
	_buffer.resize(sampleCount);

	if (_soundFunction != nullptr)
		_soundFunction(frameCount);
	else {
		return false;
	}

	data.samples = _buffer.data();
	data.sampleCount = sampleCount;
	return true;
}

void SoundGenerator::onSeek(sf::Time timeOffset) {}

/* 

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


*/