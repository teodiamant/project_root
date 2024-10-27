#ifndef UTILS_HPP
#define UTILS_HPP

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
//#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include "custom.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <CGAL/Polygon_2.h>
#include <stack>
#include <set>
#include <CGAL/centroid.h>
#include <regex>

using namespace std;

// Define the Kernel and CDT type
typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Exact_predicates_tag Itag;
typedef Custom_Constrained_Delaunay_triangulation_2<K, CGAL::Default, Itag> CDT;
typedef CDT::Point Point;

typedef CDT::Triangle Triangle;
typedef K::Line_2 Line_2;
typedef CGAL::Polygon_2<K> Polygon_2;

struct TriangulationData {
    vector<Point> steiner_points;
    vector<pair<Point, Point>> edges;
};

class InputData {
private:
    string instance_uid_;
    int num_points_;
    vector<int> points_x_;
    vector<int> points_y_;
    vector<int> region_boundary_;
    int num_constraints_;
    vector<vector<int>> additional_constraints_;
public:
    // Setters
    void setInstanceUid(const string& uid) { instance_uid_ = uid; }
    void setNumPoints(int num_points) { num_points_ = num_points; }
    void setPointsX(const vector<int>& points_x) { points_x_ = points_x; }
    void setPointsY(const vector<int>& points_y) { points_y_ = points_y; }
    void setRegionBoundary(const vector<int>& region_boundary) { region_boundary_ = region_boundary; }
    void setNumConstraints(int num_constraints) { num_constraints_ = num_constraints; }
    void setAdditionalConstraints(const vector<vector<int>>& constraints) { additional_constraints_ = constraints; }

    // Getters
    string getInstanceUid() const { return instance_uid_; }
    int getNumPoints() const { return num_points_; }
    const vector<int>& getPointsX() const { return points_x_; }
    const vector<int>& getPointsY() const { return points_y_; }
    const vector<int>& getRegionBoundary() const { return region_boundary_; }
    int getNumConstraints() const { return num_constraints_; }
    const vector<vector<int>>& getAdditionalConstraints() const { return additional_constraints_; }

    // Display function
    void display() const {
        cout << "Instance UID: " << instance_uid_ << endl;
        cout << "Number of points: " << num_points_ << endl;
        cout << "Points X: ";
        for (const auto& x : points_x_) {
            cout << x << " ";
        }
        cout << endl;

        cout << "Points Y: ";
        for (const auto& y : points_y_) {
            cout << y << " ";
        }
        cout << endl;

        cout << "Region Boundary: ";
        for (const auto& boundary : region_boundary_) {
            cout << boundary << " ";
        }
        cout << endl;

        cout << "Number of constraints: " << num_constraints_ << endl;

        cout << "Additional Constraints: " << endl;
        for (const auto& constraint : additional_constraints_) {
            if (constraint.size() == 2) {
                cout << "(" << constraint[0] << ", " << constraint[1] << ")" << endl;
            } else {
                cerr << "Error: Constraint does not have exactly 2 elements." << endl;
            }
        }
    }
};

// Declaration of additional utility functions
InputData readJsonFile(const string& filename);
int countObtuseAngles(const CDT& cdt);

// Flip Functions
bool shouldFlip(CDT::Face_handle face, int edge_index, const CDT& cdt);
void performEdgeFlips(CDT& cdt);

// Steiner Function

bool obtuseFace(CDT::Face_handle face, const CDT& cdt);

void steinerCircumcenterCentroid(CDT::Face_handle face, CDT& cdt, TriangulationData &data);
void steinerMedian(CDT::Face_handle face, CDT& cdt, TriangulationData &data);
void steinerProjection(CDT::Face_handle face, CDT& cdt, TriangulationData &data);
void centroidPolygon(CDT::Face_handle face, CDT& cdt, TriangulationData &data);

// Output the data to a JSON file
string print_rational(const K::FT& coord);
void writeJsonOutput(const string& output_filename, const string& instance_uid, const vector<Point>& steiner_points, const CDT& finalCDT);

#endif // UTILS_HPP