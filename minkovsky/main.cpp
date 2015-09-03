#include <iostream>
#include <vector>
#include <cstdio>
#include <algorithm>

struct point {
    double x, y;
    point() { }
    point(double _x, double _y) {
        x = _x;
        y = _y;
    }
    bool operator<(const point& b) const {
        return x < b.x || (x == b.x &&  y < b.y);
    }
    point operator+(const point& b) const {
        return point(x + b.x, y + b.y);
    }
};

typedef std::vector<point> polygon;

struct vect {
    double x, y;
    vect(const point& a, const point& b) {
        x = b.x - a.x;
        y = b.y - a.y;
    }
};

double vectProduct(const vect& v1, const vect& v2) {
    return v1.x * v2.y - v1.y * v2.x;
}

bool isRightTurn(const point& p1, const point& p2, const point& p3) {
    vect v1(p1, p2), v2(p1, p3);
    return vectProduct(v1, v2) >= 0;
}

void inputPolygon(polygon& p) {
    size_t vertNum;
    std::cin >> vertNum;
    p.resize(vertNum);
    for (size_t i = 0; i < vertNum; i++) {
        std::cin >> p[i].x >> p[i].y;
    }
    p.push_back(p[0]);
    std::reverse(p.begin(), p.end());
}

void invertPolygon(polygon& p) {
    for (size_t i = 0; i < p.size(); i++) {
        p[i].x *= -1;
        p[i].y *= -1;
    }
}

//works for convex polygons only!
void buildMinkSum(const polygon& p1, const polygon& p2, polygon& sum) {
    size_t vertex1 = 0, vertex2 = 0;
    //finding most left-bottom points
    for (size_t i = 0; i < p1.size(); i++) {
        if (p1[i] < p1[vertex1]) {
            vertex1 = i;
        }
    }
    for (size_t i = 0; i < p2.size(); i++) {
        if (p2[i] < p2[vertex2]) {
            vertex2 = i;
        }
    }

    size_t usedNum1 = 1, usedNum2 = 1;
    vect currentEdge1(p1[vertex1], p1[vertex1 + 1]), currentEdge2(p1[vertex2], p1[vertex2 + 1]);
    //sorting by polar angles of the edges
    do {
        sum.push_back(p1[vertex1] + p2[vertex2]);
        bool isFirstBetter = vectProduct(currentEdge1, currentEdge2) < 0;
        if (usedNum1 == p1.size() || (usedNum2 < p2.size() && isFirstBetter)) {
            usedNum2++;
            vertex2++;
            vertex2 %= p2.size() - 1;
        } else {
            usedNum1++;
            vertex1++;
            vertex1 %= p1.size() - 1;
        }
    } while (usedNum1 < p1.size() || usedNum2 < p2.size());
    sum.push_back(p1[vertex1] + p2[vertex2]);
}

//works for convex polygons only!
bool isInsidePolygon(const point& goal, const polygon& p) {
    for (size_t i = 0; i < p.size() - 1; i++) {
        if (!isRightTurn(p[i], p[i + 1], goal)) {
            return false;
        }
    }
    return true;
}

int main() {
    //freopen("input.txt", "r", stdin);
    polygon p1, p2, sum;
    inputPolygon(p1);
    inputPolygon(p2);
    invertPolygon(p2);

    buildMinkSum(p1, p2, sum);

    if (isInsidePolygon(point(0, 0), sum)) {
        std::cout << "YES" << std::endl;
    } else {
        std::cout << "NO" << std::endl;
    }
    return 0;
}
