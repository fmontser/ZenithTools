#include "NoiseGenerator.hpp"
#include "Exceptions.hpp"
#include <cmath>
#include <algorithm>
#include <limits>
#include <sstream>
#include <iomanip>
#include <map>

using namespace zenith;

constexpr uint CHANNELS = 2;
constexpr uint FRAME_COUNT = 1024;
constexpr uint SAMPLE_RATE = 48000;
constexpr float QUALITY = 0.707f;
const sf::Int16 MAX_AMPLITUDE =  32767;
const sf::Int16 MIN_AMPLITUDE = -32767;

float NoiseGenerator::masterVolume = 0.5f;
bool NoiseGenerator::masterMuted = false;

NoiseGenerator::NoiseGenerator(float band) : sf::SoundStream() {
	volume = 50.0f;
	muted = true;
	initialize(2, SAMPLE_RATE);
	setVolume(volume * masterVolume);
	_fState.fill(0.0f);
	_fState[BAND] = band;
	SetBandText();
}

const std::array<float, 10> NoiseGenerator::GetDefaultBands() {
	return { 63.0, 125.0f, 250.0f, 500.0f,
			 1000.0f, 2000.0f, 4000.0f, 8000.0f, 16000.0f, 24000.0f };
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
	_buffer.resize(FRAME_COUNT * CHANNELS);
	
	for (size_t i = 0; i < FRAME_COUNT; ++i) {
		float white = (float(rand()) / RAND_MAX - 0.5f);
	

		white = BandFilter(white);

		white = white * MAX_AMPLITUDE;
		sf::Int16 normalized = static_cast<sf::Int16>(
			std::clamp(white, float(MIN_AMPLITUDE), float(MAX_AMPLITUDE))
		);

		_buffer[i * CHANNELS] = normalized; // L
		_buffer[i * CHANNELS + 1] = normalized; // R
	}
}

float NoiseGenerator::BandFilter(float white) {
	std::array<float, 5> coeffs = CalculateBiquadCoeffs();
	float perceptualGain = CalculatePerceptualGain();

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

	return filtered * perceptualGain;
}

std::array<float,5> NoiseGenerator::CalculateBiquadCoeffs()
{
	std::array<float,5> coeffs;

	// Angular frequency
	float w0 = 2.0f * static_cast<float>(M_PI) * _fState[BAND] / SAMPLE_RATE;
	float alpha = std::sin(w0) / (2.0f * QUALITY);
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

float NoiseGenerator::CalculatePerceptualGain()
{
	//TODO switch from hardcoded to log curve strong on bass

	static std::map<float, float> gain = {
		{63.0f, 10.0f},
		{125.0f, 5.0f},
		{250.0f, 2.0f},
		{500.0f, 1.0f},
		{1000.0f, 0.6f},
		{2000.0f, 0.3f},
		{4000.0f, 0.15f},
		{8000.0f, 0.1f},
		{16000.0f, 0.085f},
		{24000.0f, 0.075f}
	};

	try {
		return gain[_fState[BAND]];
	}
	catch(const std::exception& e) {
		return 1.0f;
	}
}