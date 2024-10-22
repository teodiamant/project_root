#include "utils.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <fstream>

using namespace boost::property_tree;

std::vector<Point> read_points_from_json(const std::string& filename) {
    ptree pt;
    read_json(filename, pt);

    std::vector<Point> points;
    auto x = pt.get_child("points_x");
    auto y = pt.get_child("points_y");
    for (auto it_x = x.begin(), it_y = y.begin(); it_x != x.end() && it_y != y.end(); ++it_x, ++it_y) {
        points.push_back(Point(it_x->second.get_value<double>(), it_y->second.get_value<double>()));
    }

    return points;
}

void save_solution_to_json(const std::string& filename, const std::vector<std::pair<int, int>>& edges, const std::vector<Point>& steiner_points) {
    ptree pt;
    pt.put("content_type", "CG_SHOP_2025_Solution");

    ptree edges_node;
    for (const auto& edge : edges) {
        ptree edge_node;
        edge_node.push_back(ptree::value_type("", std::to_string(edge.first)));
        edge_node.push_back(ptree::value_type("", std::to_string(edge.second)));
        edges_node.push_back(ptree::value_type("", edge_node));
    }
    pt.add_child("edges", edges_node);

    ptree steiner_x, steiner_y;
    for (const auto& point : steiner_points) {
        steiner_x.push_back(ptree::value_type("", std::to_string(point.x())));
        steiner_y.push_back(ptree::value_type("", std::to_string(point.y())));
    }

    pt.add_child("steiner_points_x", steiner_x);
    pt.add_child("steiner_points_y", steiner_y);

    write_json(filename, pt);
}
