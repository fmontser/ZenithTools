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
	_noiseColorName = "NONE";
	volume = 50.0f;
	muted = true;
	initialize(2, SampleRate);
	GenerateRestBell();
	GenerateWorkBell();
	GenerateNoise();
	setVolume(volume);
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
			_noiseColorName = "BROWN";
			_buffer = _brownNoiseBuffer;
			break;
		case SoundGenerator::NoiseColor::PINK:
			_noiseColorName = "PINK";
			_buffer = _pinkNoiseBuffer;
			break;
		case SoundGenerator::NoiseColor::WHITE:
			_noiseColorName = "WHITE";
			_buffer = _whiteNoiseBuffer;
			break;
		case SoundGenerator::NoiseColor::BLUE:
			_noiseColorName = "BLUE";
			_buffer = _blueNoiseBuffer;
			break;
		case SoundGenerator::NoiseColor::VIOLET:
			_noiseColorName = "VIOLET";
			_buffer = _violetNoiseBuffer;
			break;
		default:
			InvalidModeException();
			_noiseColorName = "NONE";
			_loopMode = false;
			return;
	}
	play();
}

const std::string SoundGenerator::GetNoiseColorName() const {
	return _noiseColorName;
}

const SoundGenerator::NoiseColor SoundGenerator::GetNoiseColor() const {
	return _noiseColor;
}

void SoundGenerator::SetNoiseColor(NoiseColor color) {
	_noiseColor = color;
	switch (color)
	{
		case SoundGenerator::NoiseColor::BROWN:
			_noiseColorName = "BROWN";
			break;
		case SoundGenerator::NoiseColor::PINK:
			_noiseColorName = "PINK";
			break;
		case SoundGenerator::NoiseColor::WHITE:
			_noiseColorName = "WHITE";
			break;
		case SoundGenerator::NoiseColor::BLUE:
			_noiseColorName = "BLUE";
			break;
		case SoundGenerator::NoiseColor::VIOLET:
			_noiseColorName = "VIOLET";
			break;
		default:
			_noiseColorName = "NONE";
			return;
	}
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
	GenerateBrown(60.0f);
	GeneratePink(60.0f);
	GenerateWhite(60.0f);
	GenerateBlue(60.0f);
	GenerateViolet(60.f);

}

void SoundGenerator::GenerateBrown(double duration) {
	sf::Int16 amplitude = MaxAmplitude;
	const size_t totalFrames = SampleRate * duration;
	float lastSample = 0.0f;

	_brownNoiseBuffer.resize(totalFrames * Channels);

	for (std::size_t i = 0; i < totalFrames; ++i) {
		float stepSize = 0.02f;
		float white = (float(rand()) / RAND_MAX - 0.5f) * stepSize;
 		lastSample += white;
		lastSample = std::clamp(lastSample, -1.0f, 1.0f);
		sf::Int16 brown = static_cast<sf::Int16>(lastSample * amplitude);
		_brownNoiseBuffer[i * Channels] = brown; // L
		_brownNoiseBuffer[i * Channels + 1] = brown; // R
	}
}

void SoundGenerator::GeneratePink(double duration) {
/* 	sf::Int16 amplitude = MaxAmplitude;
	const size_t totalFrames = SampleRate * duration;

	_pinkNoiseBuffer.resize(totalFrames * Channels);

	float stepSize = 0.2f;
	for (std::size_t i = 0; i < totalFrames; ++i) {
		float noise = (float(rand()) / RAND_MAX - 0.5f) * stepSize;
		sf::Int16 newSample = static_cast<sf::Int16>(noise * amplitude);
		_pinkNoiseBuffer[i * Channels] = newSample; // L
		_pinkNoiseBuffer[i * Channels + 1] = newSample; // R
	} */
}

void SoundGenerator::GenerateWhite(double duration) {
	sf::Int16 amplitude = MaxAmplitude;
	const size_t totalFrames = SampleRate * duration;

	_whiteNoiseBuffer.resize(totalFrames * Channels);

	float stepSize = 0.02f;
	for (size_t i = 0; i < totalFrames; ++i) {
		float white = (float(rand()) / RAND_MAX - 0.5f) * stepSize;

		float scaled = white * amplitude;
		sf::Int16 newSample = static_cast<sf::Int16>(
			std::clamp(scaled, float(-amplitude), float(amplitude))
		);

		_whiteNoiseBuffer[i * Channels] = newSample; // L
		_whiteNoiseBuffer[i * Channels + 1] = newSample; // R
	}
}


void SoundGenerator::GenerateBlue(double duration) {
/* 	sf::Int16 amplitude = MaxAmplitude;
	const size_t totalFrames = SampleRate * duration;

	_blueNoiseBuffer.resize(totalFrames * Channels);

	float stepSize = 0.2f;
	for (std::size_t i = 0; i < totalFrames; ++i) {
		float noise = (float(rand()) / RAND_MAX - 0.5f) * stepSize;
		sf::Int16 newSample = static_cast<sf::Int16>(noise * amplitude);
		_blueNoiseBuffer[i * Channels]     = newSample; // L
		_blueNoiseBuffer[i * Channels + 1] = newSample; // R
	} */
}

void SoundGenerator::GenerateViolet(double duration) {
	sf::Int16 amplitude = MaxAmplitude;
	const size_t totalFrames = SampleRate * duration;

	_violetNoiseBuffer.resize(totalFrames * Channels);

	float lastWhite = 0.0f;
	for (size_t i = 0; i < totalFrames; ++i) {
		float white = (float(rand()) / RAND_MAX - 0.5f);
		float violet = white - lastWhite;
		lastWhite = white;

		sf::Int16 newSample = static_cast<sf::Int16>(
			std::clamp(violet * amplitude, float(-amplitude), float(amplitude))
		);

		_violetNoiseBuffer[i * Channels] = newSample; // L
		_violetNoiseBuffer[i * Channels + 1] = newSample; // R
	}
}