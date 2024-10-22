#include "graphics.hpp"

Graphics::Graphics(const InputData& instance_data) : instance_data_(instance_data) {}

void Graphics::drawTriangulation() {
    // Initialize Constrained Delaunay Triangulation
    CDT cdt;

    cout << "Inserting a grid of " << instance_data_.getPointsX().size() << "x" << instance_data_.getPointsY().size() << " constraints " << endl;

    // Create vertical constraints
    for (const auto& x : instance_data_.getPointsX()) {
        cdt.insert_constraint(Point(x, 0), Point(x, instance_data_.getPointsY().size())); // Use the size of Y for the end point
    }

    // Create horizontal constraints
    for (const auto& y : instance_data_.getPointsY()) {
        cdt.insert_constraint(Point(0, y), Point(instance_data_.getPointsX().size(), y)); // Use the size of X for the end point
    }

    assert(cdt.is_valid());

    int count = 0;
    for (const Edge& e : cdt.finite_edges()) {
        if (cdt.is_constrained(e)) {
            ++count;
        }
    }

    cout << "The number of resulting constrained edges is " << count << endl;
    CGAL::draw(cdt);
}
