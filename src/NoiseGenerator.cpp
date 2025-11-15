#include "NoiseGenerator.hpp"
#include "Exceptions.hpp"
#include <cmath>
#include <algorithm>
#include <limits>
#include <sstream>
#include <iomanip>


using namespace zenith;

constexpr uint Channels = 2;
constexpr uint FrameCount = 1024;
constexpr uint SampleRate = 48000;
constexpr float Quality = 0.707f;
const sf::Int16 MaxAmplitude =  32767;
const sf::Int16 MinAmplitude = -32767;

NoiseGenerator::NoiseGenerator(float band) : sf::SoundStream() {
	volume = 50.0f;
	muted = true;
	initialize(2, SampleRate);
	setVolume(volume);
	_fState.fill(0.0f);
	_fState[BAND] = band;
	SetBandText();
}

const std::array<float, 8> NoiseGenerator::GetDefaultBands() {
	return { 63.0, 125.0f, 250.0f, 500.0f, 1000.0f, 2000.0f, 4000.0f, 8000.0f };
}

void NoiseGenerator::SetBandText() {
	std::ostringstream oss;

	if (_fState[BAND] >= 1000.0f)
		oss << std::fixed << (int)_fState[BAND] / 1000 << "KHz";
	else
		oss << std::fixed << (int)_fState[BAND] << "Hz";
	_bandText = oss.str();
}


const std::string NoiseGenerator::GetBandText() const {
	return _bandText;
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
	
	for (size_t i = 0; i < FrameCount; ++i) {
		float white = (float(rand()) / RAND_MAX - 0.5f);
	

		white = BandFilter(white);

		white = white * MaxAmplitude;
		sf::Int16 normalized = static_cast<sf::Int16>(
			std::clamp(white, float(MinAmplitude), float(MaxAmplitude))
		);

		_buffer[i * Channels] = normalized; // L
		_buffer[i * Channels + 1] = normalized; // R
	}
}

float NoiseGenerator::BandFilter(float white) {
	std::array<float, 5> coeffs = CalculateBiquadCoeffs();

	_fState[IN] = white;
	float filtered = coeffs[0] * _fState[IN] +
					 coeffs[1] * _fState[IN_PREV] +
					 coeffs[2] * _fState[IN_PREV2] -
					 coeffs[3] * _fState[OUT_PREV] -
					 coeffs[4] * _fState[OUT_PREV2];
	
	_fState[IN_PREV2] = _fState[IN_PREV];
	_fState[IN_PREV] = _fState[IN];
	_fState[OUT_PREV2] = _fState[OUT_PREV];
	_fState[OUT_PREV] = filtered;

	return filtered;
}

std::array<float,5> NoiseGenerator::CalculateBiquadCoeffs()
{
	std::array<float,5> coeffs;

	// Angular frequency
	float w0 = 2.0f * static_cast<float>(M_PI) * _fState[BAND] / SampleRate;
	float alpha = std::sin(w0) / (2.0f * Quality);
	float cos_w0 = std::cos(w0);

	// Coefficients for a low-pass / peak filter
	float b0 = (1.0f - cos_w0) / 2.0f;
	float b1 = 1.0f - cos_w0;
	float b2 = (1.0f - cos_w0) / 2.0f;
	float a0 = 1.0f + alpha;
	float a1 = -2.0f * cos_w0;
	float a2 = 1.0f - alpha;

	// Normalize
	coeffs[0] = b0 / a0;
	coeffs[1] = b1 / a0;
	coeffs[2] = b2 / a0;
	coeffs[3] = a1 / a0;
	coeffs[4] = a2 / a0;

	return coeffs;
}