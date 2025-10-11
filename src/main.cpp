#include "Timer.hpp"
#include <thread>
#include <iostream>

using namespace zenith;
using std::cout;

int main(int argc, char* argv[]){

	auto testTimer = Timer(Minutes(0), Seconds(10));

	testTimer.Start();

	while(testTimer.GetStatus().state != Timer::State::Ended) {

		std::this_thread::sleep_for(Seconds(2));
		cout << testTimer.GetStatus().elapsed << "\n";
	}

	return 0;
}