#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
//#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include "custom.hpp"
#include <CGAL/draw_constrained_triangulation_2.h>
#include "utils.hpp"

#include <cassert>
#include <iostream>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Exact_predicates_tag Itag;
typedef Custom_Constrained_Delaunay_triangulation_2<K, CGAL::Default, Itag> CDT;
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
