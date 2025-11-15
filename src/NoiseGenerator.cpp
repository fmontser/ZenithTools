#include "NoiseGenerator.hpp"
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

NoiseGenerator::NoiseGenerator() : sf::SoundStream() {
	_loopMode = true;
	volume = 50.0f;
	muted = true;
	initialize(2, SampleRate);
	GenerateNoise();
	setVolume(volume);
}

void NoiseGenerator::PlayNoise(NoiseColor color) {
	_loopMode = true;
	play();
}

bool NoiseGenerator::onGetData(Chunk &data) {
	if (_buffer.empty())
		return false;
	data.samples = _buffer.data();
	data.sampleCount = _buffer.size();
	if (!_loopMode)
		_buffer.clear();
	return true;
}

void NoiseGenerator::onSeek(sf::Time timeOffset) {}

void NoiseGenerator::GenerateNoise() {
	double duration = 60.0f;
	sf::Int16 amplitude = MaxAmplitude;
	const size_t totalFrames = SampleRate * duration;

	_buffer.resize(totalFrames * Channels);

	float stepSize = 0.5f;
	for (size_t i = 0; i < totalFrames; ++i) {
		float white = (float(rand()) / RAND_MAX - 0.5f) * stepSize;

		float scaled = white * amplitude;
		sf::Int16 newSample = static_cast<sf::Int16>(
			std::clamp(scaled, float(-amplitude), float(amplitude))
		);

		_buffer[i * Channels] = newSample; // L
		_buffer[i * Channels + 1] = newSample; // R
	}
}