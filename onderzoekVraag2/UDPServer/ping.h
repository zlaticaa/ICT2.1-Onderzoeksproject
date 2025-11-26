#pragma once
#include <string>
#include <sstream>
#include <chrono>
class ping {
private:
	long long _microsRaw;

	long long _micros;
public:
	ping(std::string rawMsg) {
		
		std::stringstream ss(rawMsg);
		//haalt de huidige tijd in microseconden op
		auto now = std::chrono::system_clock::now();
		auto duration = now.time_since_epoch();
		auto microsNow = std::chrono::duration_cast<std::chrono::microseconds>(duration);
		//haalt de tijd uit het bericht
		ss >> _microsRaw;
		//slaat het verschil in tijd op tussen aankomst en verstuur
		_micros = microsNow.count() - _microsRaw;
	}
	//geeft de tijd in micro seconden terug
	long long GetMicros() { return _micros; };
};
