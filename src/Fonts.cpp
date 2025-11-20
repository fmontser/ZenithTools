#include "Fonts.hpp"
#include "Exceptions.hpp"
#include "Roboto-Bold.h"

using namespace zenith;

ImFont* Fonts::_normalFont = nullptr;
ImFont* Fonts::_largeFont = nullptr;

Fonts::Fonts() {
	ImGuiIO& io = ImGui::GetIO();

	_normalFont = io.Fonts->AddFontFromMemoryTTF(
		Roboto_Bold_ttf,
		Roboto_Bold_ttf_len,
		14.0f
	);
	io.FontDefault = _normalFont;

	_largeFont = io.Fonts->AddFontFromMemoryTTF(
		Roboto_Bold_ttf,
		Roboto_Bold_ttf_len,
		64.0f
	);

	ImGui::SFML::UpdateFontTexture();
}

ImFont* Fonts::GetNormalFont()
{
	if (_normalFont)
		return _normalFont;
	throw MissingClassInstanceException();
}

ImFont* Fonts::GetLargeFont()
{
	if (_largeFont)
		return _largeFont;
	throw MissingClassInstanceException();
}
