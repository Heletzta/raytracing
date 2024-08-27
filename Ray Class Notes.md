Ray Class Notes: Ch 4


4.1 

Ray class -- all ray tracers have it
All ray tracers have a computation of what color is seen along a ray

Think of a ray as a function 

P(t) = A + tb

Here P is a 3D position along a line in 3D

A is the ray origin and b is the ray direction

The ray parameter t is a real number (double in the code but can be a float)

A and b are vectors, t is a scalar
P is a vector

We can represent the idea of a ray as a calss, and represent the function P(t) as a function that we call ray::at(t)




4.2

Ready to make a ray tracer

Ray tracer sends rays through pixels and computes the color seen in the direction of those rays

Steps:
1. calculate the ray from the "eye" through the pixel
2. Determine which objects the ray intersects
3. compute a color for the closest intersection point

We want to make a camera to "see" the rays through

We'll use a non-square image -- 16:9 aspect ratio -- very common

the ratio of image width to image height is 16:9

width/height = 16/9 = 1.7778

Easier to set the width and the aspect ratio, and then calculate the height, but need to make sure that the height calculation is >= 1.

In addition to setting up the pixel dimensions for the rendered image, we also need to set up a virtual viewport through which to pass our scene rays. 

The viewport is a virtual rectangle in the 3D world that contains the grid of image pixel locations.

If pixels are spaced the same distance horizontally as they are vertically, the viewport that bounds them will have the same aspect ratio as the rendered image. The distance between two adjacent pixels is called the pixel spacing, and square pixels is the standard.

Choose an arbitrary viewport height of 2.0

scale the viewport width to give us the desired aspect ratio

Why don't we use aspect_ratio when computing viewport_width?

The value set to aspect ratio is the ideal ratio, it may not be the *actual* ratio between image_width and image_height

Since image_height is not a real value, but an integer

*
aspect_ratio is an ideal ratio, which we approximare as best as possible with the integer-based ratio of image width over image height!
*

In order for our viewport proportions to exaxtly match our image proportions, we use the calculated image aspect ratio to determine our final viewport width.




Next: 

camera center: a point in 3D space from which all scene rays will originate

(referred to as the eye point)

The vector from the camera center to the viewport center will be orthogonal to the viewport

Distance between the viewport and the camera center point referred to as "focal length"

Set to 1 unit for now

Start with camera center at (0, 0, 0)

Y axis goes up, x axis goes right, negative z axis pointing in viewing direction -- right handed coordinates



While our 3D space has the conventions above, this conflicts with our image coordinates, where we wnat to have the zeroth pixel in the top left and increase until you get to the bottom right

Our image coordinate Y-axes is inverted: Y increases going down the image

Check out drawings in notebook page 1

As we scan the image, we will start at the upper left pixel (0, 0), scan left to right across each row, then scan row by row top to bottom

To help navigate the pixel grid, we'll use a vector from the left edge to the right edge (Vu -- viewport_u) and a vector from the upper edge to the lower edge (Vv -- viewport_v)

Our pixel grid wil will be inset from the viewport edges by half teh pizel-to-pixel distance. This way, out viewport area is evenly divided into width x height identical regions

Look at page 2 in notebook for image







Section 5. Adding a Sphere

Let's add a single object to our ray tracer. Sphere -- calculating whether a ray hits a sphere is relatively simple

5.1 Ray-Sphere Intersection

The equation for a sphere of radius r that is centered at the origin:

x^2 + y^2 + z^2 = r^2

If a given point is inside the sphere, then:
x^2 + y^2 + z^2 < r^2

If outside, then x^2 + y^2 + z^2 > r^2

Allow the center to be arbitrary point (Cx, Cy, Cz), then the equation is less nice: 

(Cx - x)^2 + (Cy - y)^2 + (Cz - z)^2 = r^2

Graphics: Want your formulas to be in terms of vectors so that all the x/y/z stuff can be simply represented using a vec3 class


Note: the vector from from point P = (x, y, z)to center C = (Cx, Cy, Cz) is (C - P)

Use definition of the dot product:

(C - P) * (C - P) = (Cx - x)^2 + (Cy - y)^2 + (Cz - z)^2

therefore:

(C-P) * (C-P) = r^2

We can read this as "any point P that satisfies this equation is on the sphere"

We want to know if our ray P(t) = Q + td ever hits the sphere anywhere. 

If it does hit the sphere -- There is some t for which P(t) satisfies the sphere equation. 

Looking for t where this is true:

(C - P(t)) * (C - P(t)) = r^2

which can be found by replacing P(t) with its expanded form:

(C - (Q + td)) * (C - (Q + td)) = r^2

We have 3 vectors on the left dotted by 3 vectors on the right. If you solve fully -- get 9 vectors. 

We can separate terms based on if it includes t or not:

(-td + (C - Q)) * (-td + (C - Q)) = r^2
t^2d - 2td * (C - Q) + (C - Q)(C - Q) = r^2

Move the radius:

t^2d - 2td * (C - Q) + (C - Q)(C - Q) - r^2 = 0

It's a quadratic (of t)! No more vectors because dot product cancels them!

Use quadratic equation:

a = d*d
b = -2d * (C - Q)
c = (C - Q) * (C - Q) - r^2

You can solve for the t that will create the circle!

There is a square root part that can be + (two real solutions), 0 (1 solution), or -(no real solutions)





5.2 Creating our First Raytraced Image

Take the math and hardcode it into our program, test teh code by placing a small sphere at -1 on the z axis and then coloring red any pixel that intersects it.

This example lacks shading, reflection rays, and more than one object, but still cool!

Note: we are testing to see if a ray intersects with the sphere by solving the quadratic equation to see if a solution exists, but solutions with negative values of t work just fine.

If you change your sphere center to x = +1. you will get the same picture because this solution doesn't distinguish between objects in front of the camera and objects behind the camera.

Not a feature! A bug! Fix it next.








6. Surface Normals and Multiple Objects

We can shade with surface normals
Surface normal: vector that is perpendicular to hte surface at the point of intersection

Design decision: whether normal vectors will have arbitrary length, or will be normalized to unit length

In practice: unit length is good

1. some places might require unit vector, better to do operation once than in every place it's needed
2. We do require unit-lenghth normal vectors in several places
3. if you require normal vectors to be unit length, then you can often efficiently generate that vector with an undertanding of the specific geometry class, in its constructor, or in the hit() function
    Ex: sphere normals can be made unit length by dividing by the sphere radius, avoiding the square root entirely

POLICY: all normal vectors will be of unit length

For a sphere: outward normal is in the direction of the hit point minus the center (P - C)

We can put that into the code, and shade it!

We can visualize the normals with a color map since we don't have lights yet

Common trick: map each component to the interval from 0 to 1, and then map (x, y, z) to (red, green, blue)

For the normal -- need hit point, not just whether we hit or not (what we calced before)

Only have one sphere in the scene -- directly in front of the camera -- don't need to worry about negative values of t yet

Assume closest hit point (smallest t) is the one that we want. 





6.2 Simplifying the Ray-Sphere Intersection Code

Recall: a vector dotted with itself is equal to the squared length of that vector

Notice: equation for b has a factor of -2 in it. 
Consider what happens to the quadratic equation if b = -2h:

b = -2.0 * dot(r.direction(), oc)

b = -2h

h = dot(r.direction(), oc)

t = h +- sqrt(h^2 - ac) / a

Nice simplification!

b = -2d * (C - Q)
b = -2h
h = b/ -2 = d * (C - Q)








6.3 An Abstraction for Hittable Objects

How about more than one sphere? Tempting to have an array of spheres, clean solution is to make an "abstract class" for anything a ray might hit, and make both a sphere and a list of spheres just somethnig that can be hit. Call these classes "hittables"

**This "hittable" abstract class will have a hit function that takes in a ray**

Most ray tracers have found it convenient to add a valid interval for hits tmin and tmax -- hit only "counts" if tmin < t < tmax

Design question: compute the normal if we hit something?

    Might end up hitting something closer as we do our search
    Only need the normal of the closest thing
    
    Simple solution: compute a bundle of things, store it in some structure



6.4 Front Faces Versus Back Faces

The second design decision for normals is whether they should always point out

Presently: normal found will always be in the direction of the center to the intersection point (pointing out)

If ray intersects the sphere from the outside, the normal points against the ray!

If the ray intersects the sphere from the inside, the normal points with the ray!

We can have the normal always point against the ray. If the ray is outside the sphere, the normal will point outward, but if the ray is inside the sphere, the normal will point inward.


Need to choose one of the possibilities -- will later want to determine which side of the surface a ray is coming from.

Important for objects that are rendered differently on each side -- text on a two-sided sheet of paper, or for objects that have an inside and an outside, like glass balls

If we decide that normal always points out: need to determine which side the ray is on when we color it
Can figure it out by comparing the ray with the normal:
    If ray and normal are in the same direction, the ray is inside the object
    If the the ray and the normal face opposite directions, then the ray is outside of the object

This can be determined by taking the dot product of the two vectors, where if their dot is +, the ray is inside the sphere, and -, then outside

if (dot(ray_direction, outward_normal) > 0.0) {
    // ray is inside the sphere
    ...
} else {
    // ray is outside the sphere
    ...
}

If decide to have the normals point against the ray, won't be able to user the dot product to determine which side of the surface the ray is on! Need to store that information instead: 

bool front_face;
if (dot(ray_direction, outward_normal) > 0.0) {
    // ray is inside the sphere
    normal = -outward_normal;
    front_face = false;
} else {
    // ray is outside the sphere
    normal = outward_normal;
    front_face = true;
}

Set things up so that normals always point "outward" from the surface, or always point against the incident ray. 

Decision is determined by whether you want to determine the side of the surface at the time of geometry intersection or at the time of coloring.

More material types than geo types in this project --> put the determination at geometry time!

Add the front_face boolean to the hit_record class

Add a function to solve the calculation for us: set_face_normal()
    Assume vector passed to the function is of unit length

Add surface side determination to the class





6.5 A List of Hittable Objects

Now have a generic object called hittable that the ray can intersect with. 

Add a class that stores a list of hittables!







6.6 Some New C++ Features

Hittable_list class code uses some C++ features that you might not know if you're not a C++ programmer: vector, shared_ptr, and make_shared

shared_ptr<type> is a pointer to some allocated type, with reference-counting semantics. Every time you assign its value to another shared pointer, the reference count is incremented. As shared pointers go out of scope, the reference count is decremented. Once the count goes to 0, the object is safely deleted

shared pointer is first initialized with a newly allocated object -- like this:

shared_ptr<double> double_ptr = make_shared<double>(0.37);
shared_ptr<vec3>   vec3_ptr   = make_shared<vec3>(1.414214, 2.718281, 1.618034);
shared_ptr<sphere> sphere_ptr = make_shared<sphere>(point3(0,0,0), 1.0);


make_shared<thing>(thing_constructor params...) allocates a new instance of type "thing", using the constructor parameteres. It returns a shared_ptr<thing>

Since the typee can be automatically deduced by the return type of make_shared<type>(...), the above lines can be more simply expressed using c++'s auto type:

auto double_ptr = make_shared<double>(0.37);
auto vec3_ptr   = make_shared<vec3>(1.414214, 2.718281, 1.618034);
auto sphere_ptr = make_shared<sphere>(point3(0,0,0), 1.0);

We'll use shared pointers in our vode because it allows multiple geometries to share a common instance (for example, a bunch of spheres that all use the same color material), and because it makes memory management automatic and easier to reason about

std::shared_ptr is included with the <memory> header



2ND C++ FEATURE

std::vector

A generic array-like collection of an arbitary type. Use a collection of pointers to hittable. 

std::vector automatically grows as more values are added: objects.push_back(object) adds a value to the end of the std::vector member variable objects

std::vector is included with the <vector> header

the using statements tell the compiler that we'll be getting shared_ptr and make_shared from the std library, so we don't need to prefix these with std:: every time we reference them