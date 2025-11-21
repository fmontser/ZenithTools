/**
 * @file Fonts.hpp
 * @author https://github.com/fmontser
 * @brief Fonts class definition.
 */

#pragma once
#include "imgui.h"
#include "imgui-SFML.h"

//TODO document class
namespace zenith {
	class Fonts {
		public:
			Fonts();
			static ImFont* GetNormalFont();
			static ImFont* GetLargeFont();
			
		private:
			static ImFont* _normalFont;
			static ImFont* _largeFont;
	};
};

