#pragma once



/*


I was having an error where MathConst.h couldn't be opened. I made this file as a copy to see if I could get it to open- and then the error dissapeared.
That's why this file exists. I have no idea what the hell fixed it.

Nvm. More errors



*/






/// [ NOTE ] : This is pretty much all mimicked from the Ray Tracing in One Weekend tutorial by Peter Shirley

#ifndef MATHCONST_H	 // This is what the tutorial calls it, but I named the header file without thinking... oh well.
#define MATHCONST_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>
//#include "gpro/gpro-math/gproVector.h"





// Usings 

using std::shared_ptr;
using std::make_shared;
using std::sqrt;


// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;


// Utitlity Functions

inline double degrees_to_radians(double degrees) {

	return degrees * pi / 180.0; // Simple calculation to turn degrees to radians

}

// Couple Common Headers *One, actually due to the differences between my code and the tutorial I'm following
//#include "gpro/gpro-math/gproVector.h";

#endif // !MATHCONST_H
