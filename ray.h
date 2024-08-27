#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
    public:
        // initializes blank ray
        ray() {}

        //initializes orig and dir as origin and direction
        ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}


        // ray::origin() and ray::direction both return an immutable reference to their members
        // callers can use the reference directly, or make a mutable copy for their needs
        const point3& origin() const {
            return orig;
        }
        const vec3& direction() const {
            return dir;
        }

        // implement the P(t) function that says what point we are at 
        // given the time step and the origin point and direction vector
        point3 at(double t) const {
            return orig + t*dir;
        }

        private:
            point3 orig;
            vec3 dir;
};

#endif