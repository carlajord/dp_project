#pragma once
#include <algorithm>
#include <fstream>
#include <sstream>

#include "case.h"

void Case::ReadInput() {

	/* Reads inputs given a file path */

	std::ifstream in(_filepath);
	std::string msg;
	std::string text;
	double value;
	std::string unit;

	// Make sure file could be opened, otherwise quit
	if (!in.is_open()) {
		msg = std::format("Could not open file {}.", _filepath);
		Log(ERROR, msg);
		return;
	}
	while (std::getline(in, text))
	{
		// skip blank lines
		if (std::all_of(text.begin(), text.end(), isspace)) {
			continue;
		}

		// Read each line and store values and units in temp vectors
		size_t pos = text.find('=');
		if (pos != std::string::npos)
		{
			text = text.substr(pos + 1);
			std::istringstream text_stream(text);
			text_stream >> value >> unit;

			// some error handling for improper parsing of file
			if (text_stream.fail()) {
				value = 0.0;
				unit = "";
			}

			_temp_value.push_back(value);
			_temp_units.push_back(unit);
		}
	}
}

bool Case::ValidateInputs() {

	/* Make sure collected inputs are valid */

	std::string msg;

	// Variable count should correspond to the number of inputs expected
	if (_temp_value.size() == 0) {
		msg = std::format("Failed to read variables from the input file.", _valid_units.size(), _temp_value.size());
		Log(ERROR, msg);
		return false;
	}
	if (_temp_value.size() != _valid_units.size()) {
		msg = std::format("Incorrect input, {} input variables are required, but {} were read from the input file.", _valid_units.size(), _temp_value.size());
		Log(ERROR, msg);
		return false;
	}

	// Make sure all units are declared, and all are SI
	for (int i = 0; i < _valid_units.size(); i++) {
		if (_temp_units[i].empty()) {
			msg = std::format("Missing unit, not all variables could be parsed from the input file. Please review inputs.");
			Log(ERROR, msg);
			return false;
		}
		if (_temp_units[i] != _valid_units[i]) {
			msg = std::format("Incorrect unit detected. Flow should be given in m3/d, angle should be given in degrees and remaining units should be in SI.");
			Log(ERROR, msg);
			return false;
		}
	}

	// Make sure values are positive and > 0.0, where applicable
	if (_temp_value[diameter] <= 0.0 || _temp_value[length] <= 0.0 || _temp_value[oil_visc] <= 0.0 || _temp_value[water_visc] <= 0.0 || _temp_value[gas_visc] <= 0.0) {
		msg = std::format("Diameter, length and phase viscosities should be positive and non-zero, please review the inputs.");
		Log(ERROR, msg);
		return false;
	}

	// Angle must be between 0 and 180
	if (_temp_value[angle] < 0.0 || _temp_value[angle] > 180.0) {
		msg = std::format("Angle should be between 0 and 180 degrees, please review the inputs.");
		Log(ERROR, msg);
		return false;
	}

	// Length should be > than diameter
	if (_temp_value[length] < _temp_value[diameter]) {
		msg = std::format("Length is shorter than diameter, please review the inputs.");
		Log(ERROR, msg);
		return false;
	}
	
	return true;
}

void Case::SolveProblem() {

	/* Set up the problem and solve */

	std::string msg;
	msg = std::format("Calculating pressure drop ...");
	Log(INFO, msg);

	// Create Fluid object
	Fluid blackOil = Fluid::Fluid(_temp_value[inlet_p]);
	blackOil.SetFluidProperties(_temp_value[oil_visc], _temp_value[gas_visc], _temp_value[water_visc]);

	// Create Pipe object
	Pipe thePipe = Pipe::Pipe();
	thePipe.SetGeometryDetails(_temp_value[diameter], _temp_value[length], _temp_value[angle]);
	thePipe.SetFlowProperties(_temp_value[inlet_p], _temp_value[oil_rate], _temp_value[water_rate], _temp_value[gas_rate]);
	thePipe.SetFluid(&blackOil);

	// Solve
	auto solved = thePipe.Solve();
	
	// Wrapping up
	if (!solved) {
		msg = std::format("Terminating due to failed calculation.");
		Log(ERROR, msg);
		return;
	}
	msg = std::format("Displaying results...\n\n*** Results ***\nHoldup: {:.4f}\nTotal Pressure Drop: {:.2f} Pa\nOutlet Pressure: {:.2f} Pa\n", thePipe.GetHoldup(), thePipe.GetPressureDrop(), thePipe.GetOutletPressure());
	Log(INFO, msg);


}