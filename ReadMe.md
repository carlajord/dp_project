# Homogeneous pressure drop calculator

## Intro

This repository creates an executable file to read inputs and calculate pressure drop in a pipe.<br>
The file must have a specific format:<br>

__________________________________________
Inlet Pressure = 1.234 Pa<br>
Inlet Oil Flow Rate = 1.234 m3/d<br>
Inlet Water Flow Rate = 1.234 m3/d<br>
Inlet Gas Flow Rate = 1.234 m3/d<br>
Diameter = 1.234 m<br>
Length = 1.234 m<br>
Inclination Angle = 1.234 degrees<br>
Oil Viscosity = 1.234 kg/m.s<br>
Gas Viscosity = 1.234 kg/m.s<br>
Water Viscosity = 1.234 kg/m.s<br>
__________________________________________

The presented ordering of the variables and units are required to successfully read the inputs.<br>
The pressure drop is calculated using fluid properties obtained at average pressure.

## Requirements

This project successfully builds with:
  * CMake v. 3.23.1 (https://cmake.org/documentation/)
  * Visual Studio 2022 (https://visualstudio.microsoft.com/vs)

## Build

In the project folder, create a build directory
```
mkdir build && build
```
Run cmake and build
```
cmake ..
cmake --build .
```
