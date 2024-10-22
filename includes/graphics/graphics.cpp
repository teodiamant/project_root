#include "graphics.hpp"

TriangulationViewer::TriangulationViewer(std::vector<Point> points, std::vector<std::pair<int, int>> edges)
    : points(std::move(points)), edges(std::move(edges)) {}

void TriangulationViewer::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    draw(painter);  
}

void TriangulationViewer::draw(QPainter &painter) {
    for (const auto& edge : edges) {
        auto p1 = points[edge.first];
        auto p2 = points[edge.second];

        painter.drawLine(QPointF(p1.x(), p1.y()), QPointF(p2.x(), p2.y()));
    }
}

void display_triangulation(int argc, char *argv[], const std::vector<Point>& points, const std::vector<std::pair<int, int>>& edges) {
    QApplication app(argc, argv);
    TriangulationViewer viewer(points, edges);
    viewer.resize(800, 600);  
    viewer.show();
    app.exec();  
}
