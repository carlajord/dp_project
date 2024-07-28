#pragma once
#include "fluid.h"

Fluid::Fluid(double pressure) {
	
	/* Pressure initialization */

	UpdatePressure(pressure);
}

void Fluid::SetFluidProperties(double oil_visc, double gas_visc, double water_visc) {

	_mu[OIL] = oil_visc;
	_mu[GAS] = gas_visc;
	_mu[WATER] = water_visc;

}

void Fluid::UpdatePressure(double pressure) {

	/* Recalculates fluid properties given pressure */

	auto P = (pressure / psia_to_Pa);
	_Rs = 0.0461 * P / bbl_to_scf;
	_Bo = 0.972 + pow((_Rs * 0.00048 + 0.1026), 1.175);
	_Bg = 16.0 / P;
	
	for (int fluidInt = GAS; fluidInt != NUMTYPES; fluidInt++) {
		FluidType fluid = static_cast<FluidType>(fluidInt);
		CalculateDensity(fluid, P);
	}
}

void Fluid::CalculateDensity(Fluid::FluidType fluid_type, double pressure) {

	/* Calculates density in Field and converts to SI units */

	double value = 62.4;
	switch (fluid_type) {
	case GAS:
		value = 0.0028 * pressure;
		break;
	case OIL:
		value = (53.04 + 0.01254 * _Rs) / _Bo;
		break;
	case WATER:
		value = 62.4;
		break;
	}
	value = value / (ft3_to_m3 * kg_to_lbm);
	SetDensity(fluid_type, value);

}
