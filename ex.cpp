#include "utils.hpp"
#include "graphics.hpp"

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include "custom.hpp"
//#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/draw_constrained_triangulation_2.h>
#include <CGAL/Polygon_2.h> 

#include <iostream>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Exact_predicates_tag Itag;
typedef Custom_Constrained_Delaunay_triangulation_2<K, CGAL::Default, Itag> CDT;
typedef CDT::Point Point;

int main() {
    // Load the instance data from JSON file
    string filename = "instance_data.json";
    InputData instance_data = readJsonFile(filename);

//  // Display the loaded data
    //instance_data.display();

    // Initialize Constrained Delaunay Triangulation
    CDT cdt;

    //////////////////// Insert Instance ////////////////////
    // Insert points
    for (size_t i = 0; i < instance_data.getPointsX().size(); ++i) {
        cdt.insert(Point(instance_data.getPointsX()[i], instance_data.getPointsY()[i]));
    }

    // Insert constraints from region boundary
    const auto& region_boundary = instance_data.getRegionBoundary();
    for (size_t i = 0; i < region_boundary.size(); ++i) {
        int idx1 = region_boundary[i];
        int idx2 = region_boundary[(i + 1) % region_boundary.size()];
        cdt.insert_constraint(Point(instance_data.getPointsX()[idx1], instance_data.getPointsY()[idx1]),
                              Point(instance_data.getPointsX()[idx2], instance_data.getPointsY()[idx2]));
    }

    // Insert additional constraints
    for (const auto& constraint : instance_data.getAdditionalConstraints()) {
        if (constraint.size() == 2) {
            cdt.insert_constraint(Point(instance_data.getPointsX()[constraint[0]], instance_data.getPointsY()[constraint[0]]),
                                  Point(instance_data.getPointsX()[constraint[1]], instance_data.getPointsY()[constraint[1]]));
        }
    }

    //check how many obtuse angles are in the initial polygon
    int initialObtuseCount = countObtuseAngles(cdt);
    cout << "Initial number of obtuse angles: " << initialObtuseCount << endl;
    
    //////////////////// Improve Triangulation using Edge Flips ////////////////////
    performEdgeFlips(cdt);

    int afterEdgeFlipsObtuseCount = countObtuseAngles(cdt);
    cout << "Number of obtuse angles after Edge Flips: " << afterEdgeFlipsObtuseCount << endl;


    // Draw the constrained Delaunay triangulation
    draw(cdt);

    return 0;
};