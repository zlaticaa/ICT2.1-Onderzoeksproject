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
		auto now = std::chrono::system_clock::now();
		auto duration = now.time_since_epoch();
		auto microsNow = std::chrono::duration_cast<std::chrono::microseconds>(duration);
		ss >> _microsRaw;
		_micros = microsNow.count() - _microsRaw;
	}
	long long GetMicros() { return _micros; };
};
