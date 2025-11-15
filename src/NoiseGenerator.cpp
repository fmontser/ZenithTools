#include "NoiseGenerator.hpp"
#include "Exceptions.hpp"
#include <cmath>
#include <algorithm>
#include <limits>

using namespace zenith;

constexpr double Pi = 3.14159265358979323846;
constexpr uint Channels = 2;
constexpr uint FrameCount = 1024;
constexpr uint SampleRate = 48000;
const sf::Int16 MaxAmplitude =  32767;
const sf::Int16 MinAmplitude = -32767;

NoiseGenerator::NoiseGenerator(FilterFunction filterFunction) : sf::SoundStream() {
	volume = 50.0f;
	muted = true;
	initialize(2, SampleRate);
	setVolume(volume);
	_filter = filterFunction;
}

bool NoiseGenerator::onGetData(Chunk &data) {
	GenerateNoise();
	data.samples = _buffer.data();
	data.sampleCount = _buffer.size();
	_buffer.clear();
	return true;
}

void NoiseGenerator::onSeek(sf::Time timeOffset) {}

void NoiseGenerator::GenerateNoise() {
	_buffer.resize(FrameCount * Channels);
	float volumeNorm = 0.5f;

	for (size_t i = 0; i < FrameCount; ++i) {
		float white = (float(rand()) / RAND_MAX - 0.5f);

		white = white * MaxAmplitude;
		sf::Int16 normalized = static_cast<sf::Int16>(
			std::clamp(white * volumeNorm, float(MinAmplitude), float(MaxAmplitude))
		);

		_buffer[i * Channels] = normalized; // L
		_buffer[i * Channels + 1] = normalized; // R
	}
}


float NoiseGenerator::BandFilter63Hz(float white, FilterState& fState) {
	// TODO
	throw NotImplementedException();
	return white;
}

float NoiseGenerator::BandFilter125Hz(float white, FilterState& fState) {
	// TODO
	throw NotImplementedException();
	return white;
}

float NoiseGenerator::BandFilter250Hz(float white, FilterState& fState) {
	// TODO
	throw NotImplementedException();
	return white;
}

float NoiseGenerator::BandFilter500Hz(float white, FilterState& fState) {
	// TODO
	throw NotImplementedException();
	return white;
}

float NoiseGenerator::BandFilter1KHz(float white, FilterState& fState) {
	// TODO
	throw NotImplementedException();
	return white;
}

float NoiseGenerator::BandFilter2KHz(float white, FilterState& fState) {
	// TODO
	throw NotImplementedException();
	return white;
}

float NoiseGenerator::BandFilter4KHz(float white, FilterState& fState) {
	// TODO
	throw NotImplementedException();
	return white;
}

float NoiseGenerator::BandFilter8KHz(float white, FilterState& fState) {
	// TODO
	throw NotImplementedException();
	return white;
}
