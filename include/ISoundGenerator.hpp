#pragma once

namespace zenith {

	class ISoundGenerator {
		public:
			virtual ~ISoundGenerator() = default;
			virtual void PlayRestBell() = 0;
			virtual void PlayWorkBell() = 0;
	};
}