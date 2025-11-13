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
	_loopMode = false;
	initialize(2, SampleRate);
	GenerateRestBell();
	GenerateWorkBell();
	GenerateNoise();
	setVolume(50.0f);
}

void SoundGenerator::PlayRestBell() {
	_loopMode = false;
	_buffer = _restBellbuffer;
	play();
}

void SoundGenerator::PlayWorkBell() {
	_loopMode = false;
	_buffer = _workBellbuffer;
	play();
}

void SoundGenerator::PlayNoise(NoiseColor color) {
	_loopMode = true;
	switch (color)
	{
		case SoundGenerator::NoiseColor::BROWN:
			_buffer = _brownNoiseBuffer;
			break;
		case SoundGenerator::NoiseColor::PINK:
			_buffer = _pinkNoiseBuffer;
			break;
		case SoundGenerator::NoiseColor::WHITE:
			_buffer = _whiteNoiseBuffer;
			break;
		case SoundGenerator::NoiseColor::BLUE:
			_buffer = _blueNoiseBuffer;
			break;
		case SoundGenerator::NoiseColor::VIOLET:
			_buffer = _violetNoiseBuffer;
			break;
		default:
			InvalidModeException();
			_loopMode = false;
			return;
	}
	play();
}

bool SoundGenerator::onGetData(Chunk &data) {
	if (_buffer.empty())
		return false;
	data.samples = _buffer.data();
	data.sampleCount = _buffer.size();
	if (!_loopMode)
		_buffer.clear();
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

void SoundGenerator::GenerateNoise() {
	

/* 	_pinkNoiseBuffer.resize(totalFrames * Channels);
	_blueNoiseBuffer.resize(totalFrames * Channels);
	_violetNoiseBuffer.resize(totalFrames * Channels); */

	GenerateBrown(60.0f);
	//GeneratePink(60.0f);
	GenerateWhite(60.0f);
	//GenerateBlue(60.0f);
	//GenerateViolet(60.f);

}

void SoundGenerator::GenerateBrown(double duration) {
	sf::Int16 amplitude = MaxAmplitude;
	const size_t totalFrames = SampleRate * duration;
	float lastSample = 0.0f;

	_brownNoiseBuffer.resize(totalFrames * Channels);

	for (std::size_t i = 0; i < totalFrames; ++i) {
		float stepSize = 0.02f;
		float noise = (float(rand()) / RAND_MAX - 0.5f) * stepSize;
 		lastSample += noise;
		lastSample = std::clamp(lastSample, -1.f, 1.f);
		sf::Int16 newSample = static_cast<sf::Int16>(lastSample * amplitude);
		_brownNoiseBuffer[i * Channels]     = newSample; // L
		_brownNoiseBuffer[i * Channels + 1] = newSample; // R
	}
}

void SoundGenerator::GenerateWhite(double duration) {
	sf::Int16 amplitude = MaxAmplitude;
	const size_t totalFrames = SampleRate * duration;

	_whiteNoiseBuffer.resize(totalFrames * Channels);

	float stepSize = 0.2f;
	for (std::size_t i = 0; i < totalFrames; ++i) {
		float noise = (float(rand()) / RAND_MAX - 0.5f) * stepSize;
		sf::Int16 newSample = static_cast<sf::Int16>(noise * amplitude);
		_whiteNoiseBuffer[i * Channels]     = newSample; // L
		_whiteNoiseBuffer[i * Channels + 1] = newSample; // R
	}
}


/* 
void SoundGenerator::GenerateNoise() {
	sf::Int16 amplitude = MaxAmplitude;
	double duration = 1.0f;
	const size_t totalFrames = SampleRate * duration;
	static sf::Int16 last_sample = 0;

	_noiseBuffer.resize(totalFrames * Channels);

	for (std::size_t i = 0; i < totalFrames; ++i) {

	
		
		sf::Int16 random = amplitude * ((rand() % 65536));
		sf::Int16 sample = std::clamp(random, MinAmplitude, MaxAmplitude);

		_noiseBuffer[i * Channels]     = sample; //L-channel
		_noiseBuffer[i * Channels + 1] = sample; //R-channel

	}
}


*/