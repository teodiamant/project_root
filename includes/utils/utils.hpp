#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>
using namespace std;

class InputData {
private:
    string instance_uid;
    int num_points;
    vector<int> points_x;
    vector<int> points_y;
    vector<int> region_boundary;
    int num_constraints;
    vector<pair<int, int>> additional_constraints;
public:
    // Getters
    string getInstanceUid() const { return instance_uid; }
    int getNumPoints() const { return num_points; }
    const vector<int>& getPointsX() const { return points_x; }
    const vector<int>& getPointsY() const { return points_y; }
    const vector<int>& getRegionBoundary() const { return region_boundary; }
    int getNumConstraints() const { return num_constraints; }
    const vector<pair<int, int>>& getAdditionalConstraints() const { return additional_constraints; }

    // Setters
    void setInstanceUid(const string& uid) { instance_uid = uid; }
    void setNumPoints(int points) { num_points = points; }
    void setPointsX(const vector<int>& points) { points_x = points; }
    void setPointsY(const vector<int>& points) { points_y = points; }
    void setRegionBoundary(const vector<int>& boundary) { region_boundary = boundary; }
    void setNumConstraints(int constraints) { num_constraints = constraints; }
    void setAdditionalConstraints(const vector<pair<int, int>>& constraints) { additional_constraints = constraints;} 
};

// Function to read JSON file and parse the data
InputData readJsonFile(const string& filename);

#endif // UTILS_HPP
