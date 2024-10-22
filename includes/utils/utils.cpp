#include "utils.hpp"
#include <fstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>

#include <iostream>

using namespace boost::property_tree;
using namespace std;
using namespace CGAL;

InputData readJsonFile(const string& filename) {
    InputData inputData;

    // Create a property tree and parse the JSON file
    ptree pt;
    try {
        read_json(filename, pt);
    } catch (const json_parser_error& e) {
        cerr << "Error reading JSON file: " << e.what() << endl;
        return inputData;
    }

    // Extract data from property tree
    inputData.setInstanceUid(pt.get<string>("instance_uid", ""));
    inputData.setNumPoints(pt.get<int>("num_points", 0));

    // Points X
    vector<int> points_x;
    for (const auto& item : pt.get_child("points_x")) {
        points_x.push_back(item.second.get_value<int>());
    }
    inputData.setPointsX(points_x);

    // Points Y
    vector<int> points_y;
    for (const auto& item : pt.get_child("points_y")) {
        points_y.push_back(item.second.get_value<int>());
    }
    inputData.setPointsY(points_y);

    // Region Boundary
    vector<int> region_boundary;
    for (const auto& item : pt.get_child("region_boundary")) {
        region_boundary.push_back(item.second.get_value<int>());
    }
    inputData.setRegionBoundary(region_boundary);

    inputData.setNumConstraints(pt.get<int>("num_constraints", 0));

    // Additional Constraints
    vector<vector<int>> additional_constraints;
    for (const auto& constraint : pt.get_child("additional_constraints")) {
        vector<int> pair;
        for (const auto& value : constraint.second) {
            pair.push_back(value.second.get_value<int>());
        }
        additional_constraints.push_back(pair);
    }
    inputData.setAdditionalConstraints(additional_constraints);

    return inputData;
}

int countObtuseAngles(const CDT& cdt) {
    int obtuse_count = 0;

    // Iterate over all finite faces (triangles) in the CDT
    for (auto face_iter = cdt.finite_faces_begin(); face_iter != cdt.finite_faces_end(); ++face_iter) {
        // Get the three vertices of the triangle
        Point p1 = face_iter->vertex(0)->point();
        Point p2 = face_iter->vertex(1)->point();
        Point p3 = face_iter->vertex(2)->point();

        // Calculate the three angles of the triangle using CGAL::angle
        if (CGAL::angle(p2, p1, p3) == CGAL::OBTUSE) {
            ++obtuse_count;
        }
        if (CGAL::angle(p1, p2, p3) == CGAL::OBTUSE) {
            ++obtuse_count;
        }
        if (CGAL::angle(p1, p3, p2) == CGAL::OBTUSE) {
            ++obtuse_count;
        }
    }

    return obtuse_count;
}

