#pragma once

#include "fluid.h"

/*
Class Pipe:
Creates Pipe objects and contains functions to calculate homogeneous pressure drop.
*/

class Pipe : public BaseObj {

public:

    Pipe() = default;

    void SetGeometryDetails(double diam, double length, double angle);
    void SetFlowProperties(double pin, double oil_rate, double water_rate, double gas_rate);
    void SetFluid(Fluid* fluid) { _fluid = fluid; };
    const double& GetHoldup() const { return _holdup; };
    const double& GetPressureDrop() const { return _pressure_drop; };
    const double& GetOutletPressure() const { return _p_out; };

    void CalculateHoldUp();
    std::vector<double> CalculateNoSlipFluidProperties();
    void CalculateTotalPresDrop();
    void InnerSolve(double pressure);
    bool Solve();

private:

    // input data
    double _diam;
    double _length;
    double _angle;
    double _p_in;
    std::map<int, double> _flow{ };

    // calculated values
    double _vmix;
    double _holdup;
    double _pressure_drop;
    double _p_out;

    Fluid* _fluid;
};