#pragma once
#include <math.h>

#include "baseobj.h"

/*
Class Fluid:
Creates Black Oil fluid objects that can be used by equipments (i.e., Pipes)
*/

class Fluid : public BaseObj {

public:
    
    Fluid(double pressure);

    enum FluidType { GAS, OIL, WATER, NUMTYPES };

    const double& GetDensity(FluidType fluid_type) const { return _rho.at(fluid_type); };
    const double& GetViscosity(FluidType fluid_type) const { return _mu.at(fluid_type); };
    const double& GetOilFormationVolFactor() const { return _Bo; };
    const double& GetGasFormationVolFactor() const { return _Bg; };
    const double& GetSolutionGOR() const { return _Rs; };

    void SetDensity(FluidType fluid_type, double density) { _rho[fluid_type] = density; };
    void SetFluidProperties(double oil_visc, double gas_visc, double water_visc);

    void UpdatePressure(double pressure);
    void CalculateDensity(FluidType fluid_type, double pressure);

private:

    double _Bo;
    double _Bg;
    double _Rs;

    std::map<int, double> _rho { };
    std::map<int, double> _mu { };

};