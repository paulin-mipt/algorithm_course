#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cmath>

struct point {
    double x, y;
    point() { }
    point(double _x, double _y) {
        x = _x;
        y = _y;
    }
};

struct vect {
    double x, y;
    vect(double _x, double _y) {
        x = _x;
        y = _y;
    }

    vect(point& a, point& b) {
        x = b.x - a.x;
        y = b.y - a.y;
    }

    double length() {
        return sqrt(x * x + y * y);
    }
};

double vectProduct(const vect& v1, const vect& v2) {
    return v1.x * v2.y - v1.y * v2.x;
}

double scalarProduct(const vect& v1, const vect& v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

bool comparePoints(const point& p1, const point& p2) {
	return (p1.x < p2.x) || (p1.x == p2.x && p1.y < p2.y);
}

bool isRightTurn(point& p1, point& p2, point& p3) {
    vect v1(p1, p2), v2(p1, p3);
    return vectProduct(v1, v2) > 0;
}

double angleBtwVect(const vect& v1, const vect& v2) {
    return atan2(vectProduct(v1, v2), scalarProduct(v1, v2));
}

void buildConvexHull(std::vector<point>& pointsList, std::vector<point>& convexHull) {
	std::sort(pointsList.begin(), pointsList.end(), &comparePoints);
	point p1 = pointsList[0], p2 = pointsList.back();
	std::vector<point> upper, lower;
	upper.push_back(p1);
	lower.push_back(p1);
	for (size_t i = 1; i < pointsList.size(); ++i) {
		if (i == pointsList.size() - 1 || isRightTurn(p1, pointsList[i], p2)) {
			while (upper.size() > 1 && !isRightTurn(upper[upper.size() - 2], upper[upper.size() - 1], pointsList[i])) {
				upper.pop_back();
            }
			upper.push_back (pointsList[i]);
		}
		if (i == pointsList.size() - 1 || !isRightTurn(p1, pointsList[i], p2)) {
			while (lower.size() > 1 && isRightTurn(lower[lower.size() - 2], lower[lower.size() - 1], pointsList[i])) {
				lower.pop_back();
            }
			lower.push_back (pointsList[i]);
		}
	}

	for (size_t i = 0; i < upper.size(); ++i)
		convexHull.push_back(upper[i]);
	for (size_t i = lower.size() - 2; i > 0; --i)
		convexHull.push_back(lower[i]);
}

int main() {
    //freopen("input.txt", "r", stdin);
    size_t pointNum, radius;
    std::vector<point> pointsList, convexHull;
    std::cin >> pointNum >> radius;
    pointsList.resize(pointNum);
    for (size_t i = 0; i < pointNum; ++i) {
        std::cin >> pointsList[i].x >> pointsList[i].y;
    }

    buildConvexHull(pointsList, convexHull);

    convexHull.push_back(convexHull[0]);
    convexHull.push_back(convexHull[1]);
    double wallLength = 0;
    for (size_t i = 0; i < convexHull.size() - 2; ++i) {
        vect v1 = vect(convexHull[i], convexHull[i + 1]);
        vect v2 = vect(convexHull[i + 1], convexHull[i + 2]);
        wallLength += v1.length();
        wallLength += radius * angleBtwVect(v1, v2);
    }

    std::cout << floor(wallLength + 0.5);
    return 0;
}
