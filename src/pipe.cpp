#pragma once
#include <numbers>

#include "pipe.h"

static const double g = 9.81;

void Pipe::SetGeometryDetails(double diam, double length, double angle) {
	
	/* Sets input geometrical parameters and converts angle to rad  */

	_diam = diam;
	_length = length;
	_angle = angle*std::numbers::pi/180.0;

};

void Pipe::SetFlowProperties(double p_in, double oil_rate, double water_rate, double gas_rate) {

	_p_in = p_in;
	_flow[Fluid::FluidType::OIL] = oil_rate;
	_flow[Fluid::FluidType::GAS] = gas_rate;
	_flow[Fluid::FluidType::WATER] = water_rate;
}

bool Pipe::Solve() {
	
	/*
	Iterates to calculate pressure drop updating fluid properties with average pressure
	x = outlet pressure
	f = pressure drop error
	*/
	
	double x0 = _p_in, x1 = _p_in, x = _p_in;
	double f0 = 1.0, f1 = 1.0, f = 1.0;
	
	// Parameters
	double tol = 1.0e-5;
	int cnt = 0;
	auto p_avg = _p_in;
	auto isValid = true;

	// initial estimates
	x0 = _p_in;
	InnerSolve(p_avg);
	f0 = _pressure_drop;
	

	while (abs(f) > tol) {

		// Validate some calculated values
		if (_holdup < 0.0 || _holdup > 1.0) isValid = false;

		if (cnt > 100 || !isValid) {
			// Did not converge, warn user and quit
			std::string msg = std::format("Failed to converge oulet pressure calculation, please review input data.");
			Log(ERROR, msg);
			return false;
		}

		// 1st iteration, use successive substitution
		if (cnt == 0) {
			x = _p_in + _pressure_drop;
		}
		else { // subsequent iterations, regula falsi
			x = x0 - (x0 - x1) * f0 / (f0 - f1);
		}

		// Update fluid properties and recalculate pressure drop
		p_avg = (_p_in + x) / 2.0;
		InnerSolve(p_avg);
		f = _p_in - x - _pressure_drop;

		// Update lower and upper bound according to error
		if (cnt == 0 || f0 * f > 0) {
			x1 = x;
			f1 = f;
		}
		else {
			x0 = x;
			f0 = f;
		}
		cnt++;
	}
	
	_p_out = x;

	std::string msg = std::format("Finished pressure drop calculation.");
	Log(INFO, msg);
	return true;
}

void Pipe::InnerSolve(double p_avg) {

	/* Updates fluid properties and recalculates pressure drop */

	_fluid->UpdatePressure(p_avg);
	CalculateHoldUp();
	CalculateTotalPresDrop();
}

void Pipe::CalculateHoldUp() {

	/* Calculates velocities and holdup */

	auto Bo = _fluid->GetOilFormationVolFactor();
	auto Rs = _fluid->GetSolutionGOR();
	auto Bg = _fluid->GetGasFormationVolFactor();
	auto GOR = _flow[Fluid::FluidType::GAS] / _flow[Fluid::FluidType::OIL];
	auto area = std::numbers::pi * pow(_diam, 2.0);;

	auto fo = _flow[Fluid::FluidType::OIL] / (_flow[Fluid::FluidType::OIL] + _flow[Fluid::FluidType::WATER]);
	auto vsw = _flow[Fluid::FluidType::WATER] / (area * s_to_day);
	auto vso = _flow[Fluid::FluidType::OIL] * Bo / (area * s_to_day);
	auto vsg = _flow[Fluid::FluidType::OIL] * (GOR - Rs) * Bg / (area * s_to_day);
	auto vsl = vso * fo + vsw * (1 - fo);
	_vmix = vsl + vsg;
	_holdup = vsl / _vmix;

}

std::vector<double> Pipe::CalculateNoSlipFluidProperties() {

	/* Calculates no-slip density and viscosity */

	auto rho_o = _fluid->GetDensity(Fluid::FluidType::OIL);
	auto rho_g = _fluid->GetDensity(Fluid::FluidType::GAS);
	auto rho_w = _fluid->GetDensity(Fluid::FluidType::WATER);

	auto mu_o = _fluid->GetViscosity(Fluid::FluidType::OIL);
	auto mu_g = _fluid->GetViscosity(Fluid::FluidType::GAS);
	auto mu_w = _fluid->GetViscosity(Fluid::FluidType::WATER);

	auto fo = _flow[Fluid::FluidType::OIL] / (_flow[Fluid::FluidType::OIL] + _flow[Fluid::FluidType::WATER]);

	auto rho_l = rho_o * fo + rho_w * (1 - fo);
	auto rho_ns = rho_l * _holdup + rho_g * (1 - _holdup);
	auto mu_l = mu_o * fo + mu_w * (1 - fo);
	auto mu_ns = mu_l * _holdup + mu_g * (1 - _holdup);

	return { rho_ns , mu_ns };
}

void Pipe::CalculateTotalPresDrop() {

	/* Calculates pressure drop with the gravitational and frictional components */

	std::vector<double> no_slip_props;
	no_slip_props = CalculateNoSlipFluidProperties();

	auto Re = _vmix * no_slip_props[0] * _diam / no_slip_props[1];
	auto f = 0.0056 + 0.5 * pow(Re, -0.32);

	auto fric_dp_dl = f * no_slip_props[0] * _vmix * abs(_vmix) / (2.0 * _diam);
	auto grav_dp_dl = no_slip_props[0] * g * sin(_angle);
	
	_pressure_drop = (grav_dp_dl + fric_dp_dl)*_length;

}

