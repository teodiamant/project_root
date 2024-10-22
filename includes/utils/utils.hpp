#ifndef UTILS_HPP
#define UTILS_HPP

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <vector>
#include <string>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;

std::vector<Point> read_points_from_json(const std::string& filename);
void save_solution_to_json(const std::string& filename, const std::vector<std::pair<int, int>>& edges, const std::vector<Point>& steiner_points);

#endif
