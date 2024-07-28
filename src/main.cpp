#pragma once
#include <assert.h>

#include "case.h"

void Run() {

	std::cout << "===================================================================================\n\n";
	std::cout << "*** Welcome to the Homogeneous Pressure Drop Calculator ***\n\n";
	std::cout << "To execute this program, a file path (ex.: 'A:/File/Path.txt') must be given.\n";
	std::cout << "The file must contain the following information:\n\n";
	std::cout << "___________________________________\n";
	std::cout << "Inlet Pressure = 1.234 Pa\n";
	std::cout << "Inlet Oil Flow Rate = 1.234 m3/d\n";
	std::cout << "Inlet Water Flow Rate = 1.234 m3/d\n";
	std::cout << "Inlet Gas Flow Rate = 1.234 m3/d\n";
	std::cout << "Diameter = 1.234 m\n";
	std::cout << "Length = 1.234 m\n";
	std::cout << "Inclination Angle = 1.234 degrees\n";
	std::cout << "Oil Viscosity = 1.234 kg/m.s\n";
	std::cout << "Gas Viscosity = 1.234 kg/m.s\n";
	std::cout << "Water Viscosity = 1.234 kg/m.s\n";
	std::cout << "___________________________________\n\n";
	std::cout << "Flow is given in m3/d, angle is given in degrees and the remaining units are in SI.\n\n";
	std::cout << "===================================================================================\n\n";
	std::cout << "Please, enter a file path (or press 'q' to quit):" << std::endl;

	std::string file_path;
	std::cin >> file_path;
	bool is_valid = true;

	if (file_path != "q") {
		Case theCase = Case::Case(file_path);
		theCase.ReadInput();
		is_valid = theCase.ValidateInputs();
		if (is_valid) {
			theCase.SolveProblem();
		}
	}

	system("pause");
}

void Test() {

	std::cout << "___________________________________\n"; 
	std::cout << "\nStarting tests ...\n" << std::endl;

	std::string file_path;
	bool is_valid;
	std::string testdir = DATA_DIR;

	// Test input file validation normal
	file_path = testdir + "test_normal_solve.txt";
	Case theCase = Case::Case(file_path);
	theCase.ReadInput();
	is_valid = theCase.ValidateInputs();
	assert(is_valid == true);

	// Test input file validation fail
	file_path = "none";
	theCase = Case::Case(file_path);
	theCase.ReadInput();
	is_valid = theCase.ValidateInputs();
	assert(is_valid == false);

	// Fake input data to test Fluid and Pipe
	std::vector<double> temp_value = { 7239540.0, 158.99, 100.0, 28316.85, 0.050673, 152.4, 30, 0.002, 0.000015, 0.001 };

	Fluid theFluid = Fluid::Fluid(temp_value[0]);
	theFluid.SetFluidProperties(temp_value[7], temp_value[8], temp_value[9]);

	Pipe thePipe = Pipe::Pipe();
	thePipe.SetGeometryDetails(temp_value[3], temp_value[5], temp_value[6]);
	thePipe.SetFlowProperties(temp_value[0], temp_value[1], temp_value[2], temp_value[3]);
	thePipe.SetFluid(&theFluid);

	// Test fluid and pipe normal solve

	assert(abs(theFluid.GetDensity(Fluid::FluidType::GAS) - 47.0862) < 1.0e-4);
	assert(abs(theFluid.GetDensity(Fluid::FluidType::OIL) - 815.2086) < 1.0e-4);
	assert(abs(theFluid.GetDensity(Fluid::FluidType::WATER) - 999.381) < 1.0e-4);

	assert((theFluid.GetOilFormationVolFactor() - 1.0442) < 1.0e-4);
	assert((theFluid.GetGasFormationVolFactor() - 0.01524) < 1.0e-4);
	assert((theFluid.GetSolutionGOR() - 8.6207) < 1.0e-4);
	
	assert((thePipe.GetHoldup() - 0.0898) < 1.0e-4);
	assert((thePipe.GetPressureDrop() - 93485.089) < 1.0e-3);

	std::cout << "\nAll tests passed.\n";
	std::cout << "___________________________________\n" << std::endl;

}

int main(int argc, char **argv) {
	
	//Test();
	Run();
}