#include "vec3.hpp"
#include "primitives.hpp"
#include "bvh.hpp"
#include "utils.hpp"

#include <iostream>

using namespace std;

void height_map(float * vertices, unsigned int vsize, float * height, unsigned int x, unsigned int y, float defau) {

    size_t num_tri = vsize / 9;
    Triangle ** triangles = new Triangle*[num_tri];
    size_t filled = 0;

    for (size_t i = 0; i < vsize; i += 9)
    {
        triangles[filled++] = new Triangle(vec3(vertices[i], vertices[i + 1], vertices[i + 2]),
                                         vec3(vertices[i + 3], vertices[i + 4], vertices[i + 5]),
                                         vec3(vertices[i + 6], vertices[i + 7], vertices[i + 8]));
    }
    

    TDBVH bvh;
    BBox bounds = bvh.build(triangles, num_tri);
    float step = (bounds.high.x - bounds.low.x) / x;
    Ray ray;

    //always point down
    ray.dir = vec3(0, 0, -1);

    for (size_t i = 0; i < x; i++)
    {
        cout << i << "/" << x << endl;
        for (size_t j = 0; j < y; j++)
        {
            ray.origin = vec3(bounds.low.x + (i + 0.5) * step, bounds.low.y + (y + 0.5) * step, TOP_HEIGHT); 
            bvh.traceRegualarRay(ray, false);

            if (ray.t == RTINFINITY)
                height[j * y + i] =  defau;
            else
                height[j * y + i] = TOP_HEIGHT - ray.t;
        }
    }

    for (size_t i = 0; i < num_tri; ++i)
        delete triangles[i];
    delete [] triangles;
}