#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <vector>
#include <CGAL/draw_constrained_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;

class TriangulationViewer : public QWidget {
    std::vector<std::pair<int, int>> edges;
    std::vector<Point> points;

public:
    TriangulationViewer(std::vector<Point> points, std::vector<std::pair<int, int>> edges);
    
    void draw(QPainter &painter);

    void paintEvent(QPaintEvent *event) override;
};

void display_triangulation(int argc, char *argv[], const std::vector<Point>& points, const std::vector<std::pair<int, int>>& edges);

#endif
