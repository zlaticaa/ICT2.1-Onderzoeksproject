#pragma once
#include <string>
#include <sstream>
#include <chrono>
class ping {
private:
	long long _millsRaw;
	long long _mills;
public:
	ping(std::string rawMsg) {
		std::stringstream ss(rawMsg);
		auto now = std::chrono::system_clock::now();
		auto duration = now.time_since_epoch();
		auto millisNow = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
		ss >> _millsRaw;
		_mills = millisNow.count() - _millsRaw;
	}
	long long GetMillis() { return _mills; };
};
