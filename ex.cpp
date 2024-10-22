#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/draw_constrained_triangulation_2.h>

#include <cassert>
#include <iostream> 

using namespace CGAL;
using namespace std;

typedef Exact_predicates_inexact_constructions_kernel K;
typedef Exact_predicates_tag                               Itag;
typedef Constrained_Delaunay_triangulation_2<K, CGAL::Default, Itag> CDT;
typedef CDT::Point Point;
typedef CDT::Edge  Edge;

int main() {
    
}