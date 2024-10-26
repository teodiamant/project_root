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

    //////////////////// Improve CDT using Edge Flips ////////////////////
    performEdgeFlips(cdt);

    cout << "Number of obtuse angles after Edge Flips: " << countObtuseAngles(cdt) << endl;
    
    // I make CDT the temporary cdt and final CDT will be the optimal cdt after the steiner functions
    CDT finalCDT = cdt;
    TriangulationData data;
    
    bool improvement = true;
    int bestObtuseCount = countObtuseAngles(finalCDT);
    string bestFunction;

    while (improvement) {
        improvement = false;

        // Iterate over all finite faces in the triangulation
        for (auto face_iter = finalCDT.finite_faces_begin(); face_iter != finalCDT.finite_faces_end(); ++face_iter) {
            CDT::Face_handle face = face_iter;

            if (!obtuseFace(face, finalCDT)) {
                continue;
            }

            // Variables to track the best Steiner point option
            int currentBestObtuseCount = bestObtuseCount;
            Point bestPoint;
            bool pointAdded = false;

            // Test Steiner point at circumcenter or centroid
            CDT tempCDT = finalCDT;  // Create a temporary copy of finalCDT
            TriangulationData tempData;
            steinerCircumcenterCentroid(face, tempCDT, tempData);  // Add Steiner point at circumcenter or centroid
            int obtuseCircumcenterCount = countObtuseAngles(tempCDT);
            if (obtuseCircumcenterCount < currentBestObtuseCount) {
                currentBestObtuseCount = obtuseCircumcenterCount;
                bestPoint = *tempData.steiner_points.rbegin();  // Get the last added point
                pointAdded = true;
                bestFunction = "Circumcenter/Centroid";
            }

            // Test Steiner point at median
            tempCDT = finalCDT;  // Reset to original CDT
            tempData.steiner_points.clear();
            steinerMedian(face, tempCDT, tempData);  // Add Steiner point at median of the obtuse edge
            int obtuseMedianCount = countObtuseAngles(tempCDT);
            if (obtuseMedianCount < currentBestObtuseCount) {
                currentBestObtuseCount = obtuseMedianCount;
                bestPoint = *tempData.steiner_points.rbegin();
                pointAdded = true;
                bestFunction = "Median";
            }

            // Test Steiner point at projection
            tempCDT = finalCDT;  // Reset to original CDT
            tempData.steiner_points.clear();
            steinerProjection(face, tempCDT, tempData);  // Add Steiner point at the projection of the obtuse vertex
            int obtuseProjectionCount = countObtuseAngles(tempCDT);
            if (obtuseProjectionCount < currentBestObtuseCount) {
                currentBestObtuseCount = obtuseProjectionCount;
                bestPoint = *tempData.steiner_points.rbegin();
                pointAdded = true;
                bestFunction = "Projection";
            }
        
    // Apply the best Steiner point insertion to finalCDT
            if (pointAdded && currentBestObtuseCount < bestObtuseCount) {
                finalCDT.insert(bestPoint);  // Insert the best point found
                data.steiner_points.push_back(bestPoint);  // Track the inserted Steiner point
                bestObtuseCount = currentBestObtuseCount;
                improvement = true;  // Mark that an improvement was made
            }
        }
    }

    // Display the final count of obtuse angles after optimization
    int finalObtuseCount = countObtuseAngles(finalCDT);
    cout << "Final number of obtuse angles after Steiner point insertion: " << finalObtuseCount << endl 
    << "Function Used: " << bestFunction << endl;
    
    // Draw the optimized constrained Delaunay triangulation
    draw(finalCDT);

    return 0;
}