#include "SoundGenerator.hpp"
#include "Exceptions.hpp"
#include <cmath>
#include <algorithm>
#include <limits>

using namespace zenith;

constexpr double Pi = 3.14159265358979323846;
constexpr uint Channels = 2;
constexpr uint SampleRate = 48000;
const sf::Int16 MaxAmplitude =  32767 * 0.5;
const sf::Int16 MinAmplitude = -32767 * 0.5;

SoundGenerator::SoundGenerator() : sf::SoundStream() {
	volume = 50.0f;
	muted = true;
	initialize(2, SampleRate);
	GenerateRestBell();
	GenerateWorkBell();
	setVolume(volume);
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
	return true;
}

void SoundGenerator::onSeek(sf::Time timeOffset) {}

void SoundGenerator::GenerateRestBell() {
	double frequency = 500.0f;
	double amplitude = MaxAmplitude;
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

		sf::Int16 sample = std::clamp(
			static_cast<sf::Int16>(amplitude * std::sin(phase)),
			MinAmplitude,
			MaxAmplitude);
		
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
	sf::Int16 amplitude = MaxAmplitude;
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

		sf::Int16 sample = std::clamp(
			static_cast<sf::Int16>(amplitude * std::sin(phase)),
			MinAmplitude,
			MaxAmplitude);

		_workBellbuffer[i * Channels]     = sample; //L-channel
		_workBellbuffer[i * Channels + 1] = sample; //R-channel
			
		phase += delta;
		if (phase > 2 * Pi)
			phase -= 2 * Pi;

		frequency += 0.01f;

		delta = (frequency * 2 * Pi) / SampleRate;
	}
}