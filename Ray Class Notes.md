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

Recall: 