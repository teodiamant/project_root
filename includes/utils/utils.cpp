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

bool shouldFlip(CDT::Face_handle face, int edge_index, const CDT& cdt) {
    // Check if the edge is flipable
    if (!cdt.is_flipable(face, edge_index)) {
        //cout << "Edge at index " << edge_index << " is not flipable." << endl;
        return false;
    }

    // Get the neighboring face that shares the edge
    CDT::Face_handle opposite_face = face->neighbor(edge_index);

    // Ensure the neighboring face is valid (not infinite)
    if (cdt.is_infinite(opposite_face)) {
        //cout << "Neighboring face is infinite for edge " << edge_index << "." << endl;
        return false;
    }

    // Get the vertices involved in the shared edge and opposite vertices
    Point p1 = face->vertex((edge_index + 1) % 3)->point();
    Point p2 = face->vertex((edge_index + 2) % 3)->point();
    Point p3 = face->vertex(edge_index)->point(); // Opposite vertex in face1
    Point p4 = opposite_face->vertex(opposite_face->index(face))->point(); // Opposite vertex in face2

    // Check if p4 lies inside the circumcircle of the triangle (p1, p2, p3)
    auto side = CGAL::side_of_bounded_circle(p1, p2, p3, p4);
    if (side == CGAL::ON_BOUNDED_SIDE) {
        // If p4 lies inside the circumcircle, flipping will improve the triangulation
        //cout << "Edge at index " << edge_index << " should be flipped to improve Delaunay condition." << endl;
        return true;
    } else {
        //cout << "Edge at index " << edge_index << " does not need to be flipped." << endl;
    }

    return false;
}

void performEdgeFlips(CDT& cdt) {
    bool flipped = true;

    // Continue flipping edges as long as we are able to make improvements
    while (flipped) {
        flipped = false;

        // Iterate over all finite edges in the CDT
        for (auto edge_iter = cdt.finite_edges_begin(); edge_iter != cdt.finite_edges_end(); ++edge_iter) {
            CDT::Edge edge = *edge_iter;
            CDT::Face_handle face = edge.first;
            int edge_index = edge.second;

            // Skip constrained edges (such as region boundaries or user-defined constraints)
            if (cdt.is_constrained(edge)) {
                //cout << "Skipping constrained edge at index " << edge_index << endl;
                continue;
            }

            // Check if the edge should be flipped (is flipable and improves triangulation quality)
            if (shouldFlip(face, edge_index, cdt)) {
                // Perform the edge flip
                //cout << "Flipping edge at index " << edge_index << endl;
                cdt.flip(face, edge_index);
                flipped = true; // Mark that at least one edge was flipped in this iteration
            }
        }
    }
}

bool obtuseFace(CDT::Face_handle face, const CDT& cdt) {
    Point p1 = face->vertex(0)->point();
    Point p2 = face->vertex(1)->point();
    Point p3 = face->vertex(2)->point();
 
    // Calculate the three angles of the triangle using CGAL::angle
    if (CGAL::angle(p2, p1, p3) == CGAL::OBTUSE || CGAL::angle(p1, p2, p3) == CGAL::OBTUSE || CGAL::angle(p1, p3, p2) == CGAL::OBTUSE) {
        return true;
    }
    return false;
}

void steinerCircumcenterCentroid(CDT::Face_handle face, CDT& cdt, TriangulationData &data) {
    Point p1 = face->vertex(0)->point();
    Point p2 = face->vertex(1)->point();
    Point p3 = face->vertex(2)->point();
    Point circumcenterPoint = CGAL::circumcenter( p1, p2, p3);

    Triangle triangulate = cdt.triangle(face);

    //check if circumcenterPoint is inside the triangle
    if (triangulate.bounded_side(circumcenterPoint) == CGAL::ON_BOUNDED_SIDE) {
        cdt.insert(circumcenterPoint);
        data.steiner_points.push_back(circumcenterPoint);
        return;
    }
    
    // if circumcenterPoint is outside the triangle
    Point centroidPoint = CGAL::centroid( p1, p2, p3);
    cdt.insert(centroidPoint);
    data.steiner_points.push_back(centroidPoint);
}

void steinerMedian(CDT::Face_handle face, CDT& cdt, TriangulationData &data) {
    Point p1 = face->vertex(0)->point();
    Point p2 = face->vertex(1)->point();
    Point p3 = face->vertex(2)->point();
    Point p4;
    Point p5;

    if (CGAL::angle( p2, p1, p3) == CGAL::OBTUSE) {
        p4 = p2;
        p5 = p3;   
    }
    else if (CGAL::angle(p1, p2, p3) == CGAL::OBTUSE) {
        p4 = p1;
        p5 = p3;
    }
    else if (CGAL::angle(p1, p3, p2) == CGAL::OBTUSE) {
        p4 = p1;
        p5 = p2;
    }

    Point medianPoint = CGAL::midpoint( p4, p5);
    cdt.insert(medianPoint);
    data.steiner_points.push_back(medianPoint);
}

void steinerProjection(CDT::Face_handle face, CDT& cdt, TriangulationData &data) {
    Point p1 = face->vertex(0)->point();
    Point p2 = face->vertex(1)->point();
    Point p3 = face->vertex(2)->point();
    Point p4, p5, p6;

    //p6 is the obtuse angle

    if (CGAL::angle( p2, p1, p3) == CGAL::OBTUSE) {
        p4 = p2;
        p5 = p3;   
        p6 = p1;
    }
    else if (CGAL::angle(p1, p2, p3) == CGAL::OBTUSE) {
        p4 = p1;
        p5 = p3;
        p6 = p2;
    }
    else if (CGAL::angle(p1, p3, p2) == CGAL::OBTUSE) {
        p4 = p1;
        p5 = p2;
        p6 = p3;
    }

    //projection of the obtuse angle
    K::Line_2 line( p4, p5);
    Point projectionPoint = line.projection(p6);

    cdt.insert(projectionPoint);
    data.steiner_points.push_back(projectionPoint);
}
