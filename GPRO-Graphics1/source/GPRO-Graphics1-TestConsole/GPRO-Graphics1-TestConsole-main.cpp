/*
   Copyright 2020 Daniel S. Buckstein

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

/*
	GPRO-Graphics1-TestConsole-main.c/.cpp
	Main entry point source file for a Windows console application.

	Modified by: Jared C. Midgett
	Modified because: Class - Lab 1
*/


#include <stdio.h>
#include <stdlib.h>

//#include "gpro/gpro-math/gproVector.h"
//#include "MathConst.h"
#include "constMath.h"
//#include "Hittables.h"
#include "Hittable.h"



// My own includes below
#include <iostream>


// Prototype for a function(s)
void writeColor(std::ostream& out, vec3 pixel_color);
vec3 rayBackground(vec3 ray_direction, const vec3 origin, const hittable& world);
double hit_sphere(const vec3 origin, const vec3 center, double radius, const vec3 c_direction);


void testVector()
{
	// test array vector initializers and functions
	float3 av, bv, cv, dv;
	vec3default(av);								// -> a = (0, 0, 0)
	vec3init(bv, 1.0f, 2.0f, 3.0f);					// -> b = (1, 2, 3)
	vec3copy(dv, vec3init(cv, 4.0f, 5.0f, 6.0f));	// -> d = c = (4, 5, 6)
	vec3copy(av, dv);								// a = d			-> a = (4, 5, 6)
	vec3add(dv, bv);								// d += b			-> d = (4 + 1, 5 + 2, 6 + 3) = (5, 7, 9)
	vec3sum(dv, bv, bv);							// d = b + b		-> d = (1 + 1, 2 + 2, 3 + 3) = (2, 4, 6)
	vec3add(vec3sum(dv, cv, bv), av);				// d = c + b + a	-> d = (4 + 1 + 4, 5 + 2 + 5, 6 + 3 + 6) = (9, 12, 15)

#ifdef __cplusplus
	// test all constructors and operators
	vec3 a, b(1.0f, 2.0f, 3.0f), c(cv), d(c);		// default; init; copy array; copy
	a = d;											// assign						-> a = (4, 5, 6)
	d += b;											// add assign					-> d = (5, 7, 9)
	d = b + b;										// sum, init, assign			-> d = (2, 4, 6)
	d = c + b + a;									// sum, init, sum, init, assign	-> d = (9, 12, 15)
#endif	// __cplusplus
}

/// [  NOTE  ] 
/// Seems like I have to manually run the program before running it in command line to get it to update...
/// I am refrencing tutorial (Ray Tracing in One Weekend:Peter Shirley) pretty heavily here. Alot of code is mimicked / adapted from his.

int main(int const argc, char const* const argv[])
{
	// I'm not 100% sure what this piece of code does, but I'll leave it in since it seems to just be checking for errors in the provided functions.
	testVector();



	// Okay, So to start I'm gonna construct an image.
	// Since my time management is **** and I'm starting this project FAR later than I should, I'll do the entire thing in C++

	/*       // Image size will be 256 x 256 as reccomended by the tutorial (Ray Tracing in One Weekend:Peter Shirley)
	const int imageSize_x = 256;
	const int imageSize_y = 256;      */ // This is now *mostly irrelevent






	/// IMAGE
	// Create an aspect ratio to automatically determine height based on width.
	const double aspect_ratio = 16.9 / 9.0;
	const int imageSize_x = 400;
	const int imageSize_y = static_cast<int>(imageSize_x / aspect_ratio);


	/// WORLD
	// Create two opbjects for the environment
	hittable_list world;
	world.add(make_shared<sphere>(vec3(0, 0, -1), 0.5));
	world.add(make_shared<sphere>(vec3(0, -100.5, -1), 100));





	/// CAMERA
	double viewport_y = 2.0; // The viewports are obvious enough for the camera- but I'm not quite sure how the focal_length will work...
	double viewport_x = aspect_ratio * viewport_y;
	double focal_length = 1.0;

	vec3 origin(0, 0, 0); // These are the basic 'locations' for certain parts of the camera.
	vec3 horizontal(float(viewport_x), 0, 0);
	vec3 vertical(0, float(viewport_y), 0);
	vec3 lowerLeftCorner(-(float(viewport_x)/2), -(float(viewport_y)/2), -float(focal_length)); // I do not know why the guy in the tutorial seemed to have overcomplicated this...


	// Sphere data! //// THIS IS NOW IRRELEVENT
	//vec3 sphere_center(0, 0, -1);



	// The tutorial is outputting text to the console; though I'm not sure why I'll copy it for now.
	// It seems to be just a bit of information about the image- but what the heck is the P3 for?
	std::cout << "P3\n" << imageSize_x << ' ' << imageSize_y << "\n255\n";

	// Double For-Loop that seems to go through each pixel if I understand it correctly. 
	// Colors each of the pixels too by normalizing a value and passing it in as color. 
	for (int i = imageSize_y; i >= 0; i--) {
		for (int j = 0; j < imageSize_x; j++) {

			// This is creating a vector3 for the normalized color values
			/*      vec3 pixel_color(static_cast<float>(double(i) / (double(imageSize_x) - double(1))), 
				static_cast<float>(double(j) / (double(imageSize_y) - double(1))), 
				static_cast<float>(0.25));      */ // This is now mostly* irrelevent

			double u = double(j) / double(double(imageSize_x) - double(1)); // Create the basic uv- which is just the current pixel basically. At least atm...
			double v = double(i) / double(double(imageSize_y) - double(1));

			vec3 c_x(float(viewport_x) * float(u), 0, 0); // I am creating the CURRENT x and y's in vec3's because I don't have a * operator override and am too lazy to make one.
			vec3 c_y(0, float(viewport_y) * float(v), 0);

			vec3 c_ray = lowerLeftCorner + c_x + c_y;
			vec3 pixel_color = rayBackground(c_ray, origin, world);

			

			// This is writing the colors calculated above to the console
			writeColor(std::cout, pixel_color);

			

			/* 

			// Normalize numbers for the colors later? Also make everythings double to avoid using autos and warnings. There's probably a better way to do this...
			double r = double(i) / double(imageSize_x - double(1));
			double g = double(j) / double(imageSize_y - double(1));
			double b = 0.25;
			
			// Actually creating the colors from the math above
			int ir = static_cast<int>(255.999 * r);
			int ig = static_cast<int>(255.999 * g);
			int ib = static_cast<int>(255.999 * b); 

			// Outputs the color datato the console? Ooooh, because I'm using commandline. Duh.
			std::cout << ir << ' ' << ig << ' ' << ib << "\n";

			*/ // This is now obsolete upon creationg of the WriteColor function
		}


	}

	// Output to ... something I don't recongnize to note that progress is complete.
	// Oh! Seems to tell the command line.
	std::cerr << "\nDone\n";

	// I can leave this as it probably does not have any effect on the end result. ** Seems it may be? Commenting out...
	//printf("\n\n");
	
	// Comment this out for now.
	//system("pause");
}













/// [ NOTE ] These functions are generally heavily adapted from the tutorial I am referencing, that is mentioned above.

// Creating a function to calculate and write out the colors for me.
void writeColor(std::ostream& out, vec3 pixel_color) {

	out << static_cast<int>(255.999 * pixel_color.x) << ' '
		<< static_cast<int>(255.999 * pixel_color.y) << ' '
		<< static_cast<int>(255.999 * pixel_color.z) << '\n';

}



// creating a function to mimic the 'ray' class shown in the tutorial, cause I don't want to bother making a class.
vec3 rayBackground(vec3 ray_direction, const vec3 origin, const hittable& world) {

	/*  if (hit_sphere(origin, center, 0.5, ray_direction)) {
		return vec3(1, 0, 0); // The tutorial has this be red. I want blue
	}  

	float t = static_cast<float>(hit_sphere(origin, center, 0.5, ray_direction)); // Run function to set t (incase of hitting sphere)
	if (t > 0.0) { // Check if ray hit sphere

		vec3 N = ((t * ray_direction) - vec3(0, 0, -1)); // This calculates the distance through the sphere I believe... I think... That or the offset of a UV. Maybe.
		return 0.5 * vec3(N.x + 1.0f, N.y + 1.0f, N.z + 1.0f); // This will return a color map based on N for the point in question.

	} */ // This bit is now irrelevent


	hit_record rec; // Create a hit_record struct

	if (world.hit(origin, ray_direction, 0, infinity, rec)) { // Checks whether the 'world' was hit by the ray

		return 0.5 * (rec.normal + vec3(1, 1, 1)); // Gives a color for the world

	}




	// The tutorial is convuluted at this point, and now I understand why autos are so hated. 
	// I am attempting to construct something close to what he has for ray_color() in the tutorial

	float t = 0.5f * (ray_direction.y + 1.0f);

	float t_sub = ((1.0f - float(t)) * 1.0f); // These three lines are getting the vec3's he adds together to create a color for the ray background
	vec3 t_p1(t_sub, t_sub, t_sub);
	vec3 t_p2(t * 0.5f, t * 0.7f, t * 1.0f);
	
	vec3 t_sum = t_p1 + t_p2; // adding the rays together

	return t_sum; // return the sum 

}



// Let's go physically abuse spheres by shooting them with rays everybody! 
double hit_sphere(const vec3 origin, const vec3 center, double radius, const vec3 c_direction) {

	// Vec3 that has where the sphere is going to be placed in regards to the camera position (origin of world atm)
	vec3 oc = (origin - center); // The former is the camera and the latter is the sphere in regards to it

	// Create variable needed for function
	/*  double a = dot(c_direction, c_direction);
	double b = dot(oc, c_direction);
	double c = dot(oc, oc) - (radius * radius); */


	double a = c_direction.length_squared();
	double half_b = dot(oc, c_direction);
	double c = oc.length_squared() - (radius * radius);

	// Actual equation telling us if we hit the sphere depending on its outcome
	double discriminant = (half_b * half_b) - (4 * a * c); // (I am calling it the same thing the tutorial does)

	//std::cerr << discriminant << '\n'; // Using this to check outcomes to try and figure out why sphere doesn't appear

	// Originally returned true/false depending on value. Now is an if statement returning a value

	discriminant += 3;// For some reason, 0 does not work, but -3 works as intended. No idea how/why exactly this is occuring. I'm just adding 3 here so I don't get imaginary numbers
	if (discriminant < 0) { // This returns -1, which acts as a false
		return -1.0;
	
	} else { 
		return (-half_b - sqrt(discriminant) / a); // This value will be used as t.

	}

}