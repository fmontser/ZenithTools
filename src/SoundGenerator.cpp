#include "SoundGenerator.hpp"
#include "Exceptions.hpp"
#include <cmath>
#include <cstdint>
#include <algorithm>
#include <limits>

using namespace zenith;

constexpr double PI = 3.141592653589793;
constexpr uint32_t CHANNELS = 2;
constexpr uint32_t SAMPLE_RATE = 48000;
const sf::Int16 MAX_AMPLITUDE =  32767;
const sf::Int16 MIN_AMPLITUDE = -32767;

SoundGenerator::SoundGenerator() : sf::SoundStream() {
	volume = 50.0f;
	initialize(2, SAMPLE_RATE);
	GenerateRestBell();
	GenerateWorkBell();
	setVolume(volume);
}

SoundGenerator::~SoundGenerator() {
	stop();
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
	data.samples = _buffer.data();
	data.sampleCount = _buffer.size();
	_buffer.clear();
	return true;
}

void SoundGenerator::onSeek(sf::Time timeOffset) {}

void SoundGenerator::GenerateRestBell() {
	double frequency = 500.0f;
	double amplitude = MAX_AMPLITUDE;
	double phase = 0.0f;
	double delta = (frequency * 2 * PI) / SAMPLE_RATE;
	double duration = 1.0f;
	const size_t totalFrames = SAMPLE_RATE * duration;
	const float fade_start = 0.16f;
	const size_t fade_frames = totalFrames - (totalFrames * fade_start);
	const float fade_delta = amplitude / (totalFrames * fade_start);

	_restBellbuffer.resize(totalFrames * CHANNELS);

	for (std::size_t i = 0; i < totalFrames; ++i) {
		
		if (i >= fade_frames)
			amplitude -= fade_delta;

		sf::Int16 sample = std::clamp(
			static_cast<sf::Int16>(amplitude * std::sin(phase) * 0.5f),
			MIN_AMPLITUDE,
			MAX_AMPLITUDE);
		
		_restBellbuffer[i * CHANNELS]     = sample; //L
		_restBellbuffer[i * CHANNELS + 1] = sample; //R
			
		phase += delta;
		if (phase > 2 * PI)
			phase -= 2 * PI;

		frequency -= 0.02f;
		if (frequency < 0)
			frequency = 0;

		delta = (frequency * 2 * PI) / SAMPLE_RATE;
	}
}

void SoundGenerator::GenerateWorkBell() {
	double frequency = 0.0f;
	sf::Int16 amplitude = MAX_AMPLITUDE;
	double phase = 0.0f;
	double delta = (frequency * 2 * PI) / SAMPLE_RATE;
	double duration = 1.0f;
	const size_t totalFrames = SAMPLE_RATE * duration;
	const float fade_start = 0.16f;
	const size_t fade_frames = totalFrames - (totalFrames * fade_start);
	const float fade_delta = amplitude / (totalFrames * fade_start);

	_workBellbuffer.resize(totalFrames * CHANNELS);

	for (std::size_t i = 0; i < totalFrames; ++i) {
		
		if (i >= fade_frames)
			amplitude -= fade_delta;

		sf::Int16 sample = std::clamp(
			static_cast<sf::Int16>(amplitude * std::sin(phase)),
			MIN_AMPLITUDE,
			MAX_AMPLITUDE);

		_workBellbuffer[i * CHANNELS]     = sample; //L
		_workBellbuffer[i * CHANNELS + 1] = sample; //R
			
		phase += delta;
		if (phase > 2 * PI)
			phase -= 2 * PI;

		frequency += 0.01f;

		delta = (frequency * 2 * PI) / SAMPLE_RATE;
	}
}