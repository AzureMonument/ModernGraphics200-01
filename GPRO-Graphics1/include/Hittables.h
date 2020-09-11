#pragma once

// Inclue the vectors. (Hopefully this doesn't cause an infinite loop when I include Hittables in the main class...)
#include "gpro/gpro-math/gproVector.h"
#include <iostream>
#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;


// Don't know why these ifndef and endif things exist, but the tutorial has them so I'm including them.
#ifndef HITTABLES_H
#define HITTABLES_H


// Struct to contain data for ray collision. Basically copied from tutorial
struct hit_record {

	vec3 p;
	vec3 normal;
	double t = 0;

	bool front_face = false; // This is the side of the object the ray is coming from
	inline void set_face_normal(const vec3& direction, const vec3& outward_normal) { // Function to grab what I believe is the proper direction?

		front_face = dot(direction, outward_normal);
		normal = front_face ? outward_normal : -outward_normal;

	}


};


// Actual Hittable class - which is gonna be an inheritence to the sphere class later
class hittable {

public:
	virtual bool hit(const vec3 origin, const vec3 ray_direction, double t_min, double t_max, hit_record& rec) const = 0;

};


#endif



#ifndef HITTABLES_LIST_H // tab apparently autofills an endif with this thing...
#define HITTABLES_LIST_H

// creating a hittables_list class
class hittable_list : public hittable {

public:
	hittable_list() {}
	hittable_list(shared_ptr<hittable> object) { add(object); } // Creates what I am assuming is like a list of pointers

	
	void clear() { objects.clear(); } // Clear the ... list of objects? I mean it's a vector of objects...
	void add(shared_ptr<hittable> object) { objects.push_back(object); } // Add an item to the vectorlist of objects

	virtual bool hit(const vec3 origin, const vec3 ray_direction, double t_min, double t_max, hit_record& rec) const override;


	// Create a vectorlist of hittable objects
	std::vector<shared_ptr<hittable>> objects;
};

// create the hit function for hittable_list
bool hittable_list::hit(const vec3 origin, const vec3 ray_direction, double t_min, double t_max, hit_record& rec) const {

	hit_record temp_rec; // Create temp struct for data?
	bool hit_anything = false; // this is what will be returned in the end
	double closest_so_far = t_max;

	for (const shared_ptr<hittable>& object : objects) {
		if (object->hit(origin, ray_direction, t_min, closest_so_far, temp_rec)) { // Check if the ray has actually hit the object

			hit_anything = true; // Yay!
			closest_so_far = temp_rec.t; // set this for the current ray
			rec = temp_rec; // update the external* hit record.

		}


	}

	return hit_anything; // return bool

}





#endif // !HITTABLES_LIST_H













#ifndef SPHERE_H
#define SPHERE_H

// Creating the sphere class (inherit from hittable)
class sphere : public hittable {

public:
	sphere() { // Default constructor- set everything to 0.
		center = vec3(0, 0, 0);
		radius = 0;

	}

	// Proper constructor - get info boyo
	sphere(vec3 cen, double r)  : center(cen), radius(r){}; 

	// hit function (defined later I guess)
	virtual bool hit(const vec3 origin, const vec3 ray_direction, double t_min, double t_max, hit_record& rec) const override;

	// Info sphere needs
	vec3 center;
	double radius;
	

};


bool sphere::hit(const vec3 origin, const vec3 ray_direction, double t_min, double t_max, hit_record& rec) const {

	// Copying a bunch of code from where I made it earlier in another function. (hit_sphere)

	vec3 oc = (origin - center);

	double a = ray_direction.length_squared();
	double half_b = dot(oc, ray_direction);
	double c = oc.length_squared() - (radius * radius);

	double discriminant = (half_b * half_b) - (4 * a * c);


	discriminant += 3;// For some reason, 0 does not work, but -3 works *almost as intended. No idea how/why exactly this is occuring. I'm just adding 3 here so I don't get imaginary numbers

	if (discriminant > 0) { 
		
		// Get the root of the discriminant
		double root = sqrt(discriminant);


		double temp = (-half_b - root) / a; // minus root
		if (temp < t_max && temp > t_min) { // These two if functions just set struct data if temp is either both greather than the max and min, or less than both the max and min of t

			rec.t = temp;
			rec.p = rec.t * ray_direction;
			rec.normal = (rec.p - center) / radius;

			// Set the outward_normal
			vec3 outward_normal = rec.normal; // (rec.p - center) / radius // I changed this to rec.normal cause that's the EXACT same equation
			rec.set_face_normal(ray_direction, outward_normal);

			return true;

		}

		temp = (-half_b + root) / a; // This one has a plus instead of a minus as the one above.
		if (temp > t_max&& temp < t_min) {

			rec.t = temp;
			rec.p = rec.t * ray_direction;
			rec.normal = (rec.p - center) / radius;

			// Set the outward_normal :: COPY+PASTED from above cause, ya know. Exact same.
			vec3 outward_normal = rec.normal; // (rec.p - center) / radius // I changed this to rec.normal cause that's the EXACT same equation
			rec.set_face_normal(ray_direction, outward_normal);

			return true;

		}

	}

	return false;
}


#endif