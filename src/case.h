#pragma once
#include <vector>

#include "pipe.h"

/*
Class Case:
Reads input file, validates inputs, sets up the problem and solves.
*/

class Case : public BaseObj {

public:

	Case(std::string input_file) : _filepath(input_file) {};

	void ReadInput();
	bool ValidateInputs();
	void SolveProblem();

private:

	enum Input {
		inlet_p,
		oil_rate,
		water_rate,
		gas_rate,
		diameter,
		length,
		angle,
		oil_visc,
		gas_visc,
		water_visc
	};

	std::vector<std::string> _valid_units = { 
		"Pa",
		"m3/d",
		"m3/d",
		"m3/d",
		"m",
		"m",
		"degrees",
		"kg/m.s",
		"kg/m.s",
		"kg/m.s"
	};

	std::string _filepath;
	std::vector<double> _temp_value;
	std::vector<std::string> _temp_units;
	
};