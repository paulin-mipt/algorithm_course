#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <queue>

const double EPS = 1e-5;

struct point {
    double x, y, z;
    size_t index;
    point() { }
    point(double _x, double _y, double _z, size_t _index) {
        x = _x;
        y = _y;
        z = _z;
        index = _index;
    }
    void print() const {
        std::cout << x << " " << y << " " << z << std::endl;
    }
};

struct vect {
    double x, y, z;
    vect(const point& a, const point& b) {
        x = b.x - a.x;
        y = b.y - a.y;
        z = b.z - a.z;
    }

    vect(double _x, double _y, double _z) {
        x = _x;
        y = _y;
        z = _z;
    }

    double length() {
        return sqrt(x*x + y*y + z*z);
    }

    void print() const {
        std::cout << x << " " << y << " " << z << std::endl;
    }
};

struct face {
    std::vector<point> points;
    face() { }
    face(const point& p0, const point& p1, const point& p2) {
        points.push_back(p0);
        points.push_back(p1);
        points.push_back(p2);
    }

    void print() {
        std::cout << points.size() << " ";
        for (size_t i = 0; i < 3; i++) {
            std::cout << points[i].index << " ";
        }
        std::cout << std::endl;
    }

    bool operator<(const face& b) const {
        return (points[0].index < b.points[0].index) || ((points[0].index == b.points[0].index)
                    && (points[1].index < b.points[1].index)) || ((points[0].index == b.points[0].index) &&
                    (points[1].index == b.points[1].index) && (points[2].index < b.points[2].index));
    }

    void shift() {
        if (points[0].index > points[1].index || points[0].index > points[2].index) {
            if (points[1].index < points[2].index) {
                point p0 = points[0];
                points[0] = points[1];
                points[1] = points[2];
                points[2] = p0;
            } else {
                point p0 = points[0];
                points[0] = points[2];
                points[2] = points[1];
                points[1] = p0;
            }
        }
    }
};

struct plane {
    double a, b, c, d;
    plane() { }
    plane(const point& p1, const point& p2, const point& p3) {
        a = (p2.y - p1.y)*(p3.z - p1.z) - (p3.y - p1.y)*(p2.z - p1.z);
        b = (p2.z - p1.z)*(p3.x - p1.x) - (p3.z - p1.z)*(p2.x - p1.x);
        c = (p2.x - p1.x)*(p3.y - p1.y) - (p3.x - p1.x)*(p2.y - p1.y);
        d = -(p1.x *  a + p1.y * b + p1.z * c);
    }

    double evaluate(const point& p) {
        return a*p.x + b*p.y + c*p.z + d;
    }

    vect normalVect() {
        return vect(a, b, c);
    }
};

vect vectProduct(const vect& v1, const vect& v2) {
    return vect(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
                                v1.x * v2.y - v1.y * v2.x);
}

double scalarProduct(const vect& v1, const vect& v2) {
    double ans = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    return ans;
}


double absAngleBtwVect(const vect& a, const vect& b) {
    //a.print();
    //b.print();
    double vp = vectProduct(a, b).length();
    double sp = scalarProduct(a, b);
    if (fabs(sp) < EPS && fabs(vp) < EPS) {
        return 2 * M_PI;
    }
    double ans = atan2(vp, sp);
    //std::cout << ans << std::endl;
    return ans;
}

//reading data and choosing the first face
void init(std::vector<point>& points, std::vector<face>& faces) {
    size_t n;
    std::cin >> n;
    for (size_t i = 0; i < n; i++) {
        double x, y, z;
        std::cin >> x >> y >> z;
        points.push_back(point(x, y, z, i));
    }

    sort(points.begin(), points.end(),
                    [](const point& a, const point& b) {return a.z < b.z;});
    point p1 = points[0], p2, p3;
    double maxAngle = -10;
    vect vertical(0, 0, 1);

    for (size_t i = 1; i < n; i++) {
        double newAngle = absAngleBtwVect(vect(p1, points[i]), vertical);
        if (newAngle > maxAngle) {
            maxAngle = newAngle;
            p2 = points[i];
        }
    }

    bool foundFace = false;
    size_t i = 1;
    if (p2.index == points[1].index) {
        i = 2;
    }
    double sign = 0;
    vect v1(p1, p2);
    while (!foundFace) {
        foundFace = true;
        size_t j = 1;
        while (points[j].index == p2.index || j == i) {
            j++;
        }

        plane p(p1, p2, points[i]);
        size_t k = j;
        double checkValue = p.evaluate(points[j]);
        if (fabs(checkValue) > EPS) {
            sign = checkValue;
        }
        while (k < n) {
            if (p.evaluate(points[k]) * checkValue < 0) {
                foundFace = false;
            }
            k++;
        }
        if (foundFace) {
            p3 = points[i];
            break;
        }

        i++;
        if (points[i].index == p2.index) {
            i++;
        }
    }

    face newFace;
    if (sign > 0) {
        newFace = face(p1, p3, p2);
    } else {
        newFace = face(p1, p2, p3);
    }
    newFace.shift();
    faces.push_back(newFace);
}

void addEdge(std::vector< std::vector<size_t> >& used, const point& p1, const point& p2) {
    used[p1.index][p2.index]++;
    used[p2.index][p1.index]++;
}

void addFace(std::vector< std::vector<size_t> >& used, const face& f) {
    for (size_t i = 0; i < 3; i++) {
        addEdge(used, f.points[i], f.points[(i+1) % 3]);
    }
}

bool canAdd(const std::vector< std::vector<size_t> >& used, const point& p1, const point& p2, const point& p3) {
    if (used[p1.index][p2.index] == 2) {
        return false;
    }
    if (used[p2.index][p3.index] == 2) {
        return false;
    }
    if (used[p1.index][p3.index] == 2) {
        return false;
    }
    return true;
}

int main() {
    freopen("input.txt", "r", stdin);
    std::vector<face> faces;
    std::vector<point> points;
    init(points, faces);
    std::vector< std::vector<size_t> > usedEdge(points.size());
    for (size_t i = 0; i < points.size(); i++) {
        for (size_t j = 0; j < points.size(); j++) {
            usedEdge[i].push_back(0);
        }
    }

    face f0 = faces[0];
    addFace(usedEdge, f0);

    size_t k = 0;
    while (k < faces.size()) {
        for (size_t i = 0; i < 3; i++) {
            point p1 = faces[k].points[i];
            point p2 = faces[k].points[(i + 1) % 3];
            plane currentFacePlane(faces[k].points[0], faces[k].points[1], faces[k].points[2]);
            if (usedEdge[p1.index][p2.index] < 2) {
                double minAngle = 2 * M_PI;
                int bestPointInd = -1;
                for (size_t p = 0; p < points.size(); p++) {
                    plane tempPlane(p1, points[p], p2);
                    double angle = absAngleBtwVect(tempPlane.normalVect(), currentFacePlane.normalVect());
                    if (angle < minAngle && fabs(angle) > EPS) {
                        bestPointInd = p;
                        minAngle = angle;
                    }
                }

                if (minAngle + EPS < 2 * M_PI && canAdd(usedEdge, points[bestPointInd], p1, p2)) {
                    face newFace(p2, p1, points[bestPointInd]);
                    addFace(usedEdge, newFace);
                    newFace.shift();
                    faces.push_back(newFace);
                }
            }
        }
        k++;
    }

    sort(faces.begin(), faces.end());
    std::cout << faces.size() << std::endl;
    for (size_t i = 0; i < faces.size(); i++) {
        faces[i].print();
    }

    return 0;
}
