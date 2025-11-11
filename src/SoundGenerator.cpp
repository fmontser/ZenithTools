#include "SoundGenerator.hpp"
#include "Exceptions.hpp"
#include <cmath>

using namespace zenith;

constexpr double Pi = 3.14159265358979323846;
constexpr uint Channels = 2;
constexpr uint SampleRate = 48000;

//TODO borrar?
constexpr uint FrameCount = 1024;
const std::size_t SampleCount = FrameCount * Channels;

SoundGenerator::SoundGenerator() : sf::SoundStream() {
	initialize(2, SampleRate);
	GenerateRestBell();
	GenerateWorkBell();
}


void SoundGenerator::PlayRestBell() {
	_buffer = _restBellbuffer;
	play();
}

void SoundGenerator::PlayWorkBell() {
	_buffer = _workBellbuffer;
	play();
}

bool SoundGenerator::onGetData(Chunk &data) {
	if (_buffer.empty())
		return false;
	data.samples = _buffer.data();
	data.sampleCount = _buffer.size();
	_buffer.clear();
	return true;
}

void SoundGenerator::onSeek(sf::Time timeOffset) {}

void SoundGenerator::GenerateRestBell() {
	double frequency = 500.0f;
	double amplitude = 16000.0f;
	double phase = 0.0f;
	double delta = (frequency * 2 * Pi) / SampleRate;
	double duration = 1.0f;
	const size_t totalFrames = SampleRate * duration;
	const float fade_start = 0.16f;
	const size_t fade_frames = totalFrames - (totalFrames * fade_start);
	const float fade_delta = amplitude / (totalFrames * fade_start);

	_restBellbuffer.resize(totalFrames * Channels);

	for (std::size_t i = 0; i < totalFrames; ++i) {
		
		if (i >= fade_frames)
			amplitude -= fade_delta;

		sf::Int16 sample = static_cast<sf::Int16>(amplitude * std::sin(phase));

		_restBellbuffer[i * Channels]     = sample; //L-channel
		_restBellbuffer[i * Channels + 1] = sample; //R-channel
			
		phase += delta;
		if (phase > 2 * Pi)
			phase -= 2 * Pi;

		frequency -= 0.02f;
		if (frequency < 0)
			frequency = 0;

		delta = (frequency * 2 * Pi) / SampleRate;
	}
}

void SoundGenerator::GenerateWorkBell() {
	double frequency = 0.0f;
	double amplitude = 16000.0f;
	double phase = 0.0f;
	double delta = (frequency * 2 * Pi) / SampleRate;
	double duration = 1.0f;
	const size_t totalFrames = SampleRate * duration;
	const float fade_start = 0.16f;
	const size_t fade_frames = totalFrames - (totalFrames * fade_start);
	const float fade_delta = amplitude / (totalFrames * fade_start);

	_workBellbuffer.resize(totalFrames * Channels);

	for (std::size_t i = 0; i < totalFrames; ++i) {
		
		if (i >= fade_frames)
			amplitude -= fade_delta;

		sf::Int16 sample = static_cast<sf::Int16>(amplitude * std::sin(phase));

		_workBellbuffer[i * Channels]     = sample; //L-channel
		_workBellbuffer[i * Channels + 1] = sample; //R-channel
			
		phase += delta;
		if (phase > 2 * Pi)
			phase -= 2 * Pi;

		frequency += 0.01f;

		delta = (frequency * 2 * Pi) / SampleRate;
	}
}


/* 

		for (std::size_t i = 0; i < frameCount; ++i) {
			sf::Int16 sample = _amp * static_cast<sf::Int16>((rand() % 65536) - 32768);
			
			//TODO tono simple ...static_cast<sf::Int16>(_amp * std::sin(_phase));

			_buffer[i * 2]     = sample; // canal izquierdo
			_buffer[i * 2 + 1] = sample; // canal derecho
			_phase += _delta;
			if (_phase > 2 * 3.14159265358979323846)
				_phase -= 2 * 3.14159265358979323846;

			_delta = (_freq * 2 * 3.14159265358979323846) / SampleRate;
		}


*/