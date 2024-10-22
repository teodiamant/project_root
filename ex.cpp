#include "utils.hpp"
#include "graphics.hpp"

#include <iostream>

using namespace std;

int main() {
    // Load the instance data from JSON file
    string filename = "instance_data.json";
    InputData instance_data = readJsonFile(filename);

    // Display the loaded data
    cout << "Instance UID: " << instance_data.getInstanceUid() << endl;
    cout << "Number of points: " << instance_data.getNumPoints() << endl;
    cout << "Points X: ";
    for (const auto& x : instance_data.getPointsX()) {
        cout << x << " ";
    }
    cout << endl;

    cout << "Points Y: ";
    for (const auto& y : instance_data.getPointsY()) {
        cout << y << " ";
    }
    cout << endl;

    cout << "Region Boundary: ";
    for (const auto& boundary : instance_data.getRegionBoundary()) {
        cout << boundary << " ";
    }
    cout << endl;

    cout << "Number of constraints: " << instance_data.getNumConstraints() << endl;

    cout << "Additional Constraints: " << endl;
    for (const auto& constraint : instance_data.getAdditionalConstraints()) {
        cout << "(" << constraint.first << ", " << constraint.second << ")" << endl;
    }

    // Draw the triangulation using the loaded data
    Graphics graphics(instance_data);
    graphics.drawTriangulation();

    return 0;
}
