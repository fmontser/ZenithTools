#pragma once

#include "gmock/gmock.h"
#include "ISoundGenerator.hpp"

namespace zenith {

	class MockSoundGenerator : public ISoundGenerator {
		public:
			MOCK_METHOD(void, PlayRestBell, (), (override));
			MOCK_METHOD(void, PlayWorkBell, (), (override));
	};
}