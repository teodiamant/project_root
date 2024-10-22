#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/draw_constrained_triangulation_2.h>
#include "utils.hpp"

#include <cassert>
#include <iostream>

using namespace CGAL;
using namespace std;

typedef Exact_predicates_inexact_constructions_kernel K;
typedef Exact_predicates_tag Itag;
typedef Constrained_Delaunay_triangulation_2<K, CGAL::Default, Itag> CDT;
typedef CDT::Point Point;
typedef CDT::Edge Edge;

class Graphics {
public:
    Graphics(const InputData& instance_data);
    void drawTriangulation();

private:
    const InputData& instance_data_;
};

#endif // GRAPHICS_HPP
