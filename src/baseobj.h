#pragma once

// If using concurrency, consider replacing localtime for a thread safe function
#pragma warning( disable : 4996 )

#include <iostream>
#include <format>
#include <string>
#include <map>
#include <chrono>

#define Log(status, msg) { MyLogging(status, msg); }

/*
Class BaseObj:
Base class for all created objects.
Includes shared functions and variables.
*/

class BaseObj {

public:

	BaseObj() = default;

	static constexpr double ft3_to_m3 = 0.028316847; // 0.3048^3
	static constexpr double kg_to_lbm = 2.205;
	static constexpr double bbl_to_scf = 5.615;
	static constexpr double psia_to_Pa = 6894.8;
	static constexpr double s_to_day = (24.0 * 3600.0);

	enum status { INFO, WARN, ERROR };
	std::map<int, std::string> _status_map{ {INFO, "Info"}, {WARN, "Warning"}, {ERROR, "Error"} };
	
	void MyLogging(int status, std::string msg) {

		/* Time, log level and logging message written to console.
		Note: Alternatively, a function can be added to log to a file
		*/

		std::time_t now_time = std::time(NULL);
		std::cout << std::put_time(std::localtime(&now_time), "%Y-%m-%d %X") << " " << _status_map[status] << ": " << msg << std::endl;

	}
};