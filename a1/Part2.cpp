#include "Part2.h"

using namespace std;

// Point class
class Point {
private:
    int x, y, z;

public:

    // Constructor
    Point(int x = 0, int y = 0, int z = 0) : x(x), y(y), z(z) {}

    // Destructor (default, as no dynamic memory needs to be freed from heap)
    ~Point() {}

    // Getters
    int getX() const { return x; }
    int getY() const { return y; }
    int getZ() const { return z; }
    // Setters
    void setX(int x) { this->x = x; }
    void setY(int y) { this->y = y; }
    void setZ(int z) { this->z = z; }

    // Translate function to move point along specified axis
    int translate(int d, char axis) {
        switch(axis) {
            case 'x': x += d; break;
            case 'y': y += d; break;
            case 'z': z += d; break;
            default: return -1;  // Invalid axis value
        }
        return 0;  // Successful translation
    }

    // Display function to show point coordinates
    string toString() const {
        stringstream ss;
        ss << "Point(" << x << "," << y << "," << z << ")";
        return ss.str();
    }

    // Distance between this point and another point
    inline double distanceTo(const Point& other) const {
        return sqrt(pow(x - other.x, 2) + pow(y - other.y, 2) + pow(z - other.z, 2));
    }

    // Operator overload for vector operations
    Point operator+(const Point& other) const {return {x + other.x, y + other.y, z + other.z};}
    Point operator-(const Point& other) const {return {x - other.x, y - other.y, z - other.z};}
    Point operator*(int scalar) const {return {x * scalar, y * scalar, z * scalar};}
    Point operator/(int scalar) const { return {x / scalar, y / scalar, z / scalar};}
    // Operator overload for boolean comparison operations
    bool operator==(const Point& other) const { return x == other.x && y == other.y && z == other.z; }
    bool operator!=(const Point& other) const { return !(*this == other); }

    // Tuple conversion for easy unpacking of coordinates
    tuple<int, int, int> as_tuple() const {return make_tuple(x, y, z);}

    // Non-member function ostream overload for easy printing of Point objects
    friend ostream& operator<<(ostream& os, const Point& point) { return os << point.toString(); }
};

// Triangle class
class Triangle {
private:
    Point *vertex_1;
    Point *vertex_2;
    Point *vertex_3;
public:
    // Constructor with three Point objects
    explicit Triangle(const Point &v1, const Point &v2, const Point &v3) {
        if (!Triangle::isValidTrianglePoints(v1, v2, v3)) {
            throw logic_error("The points do not form a valid triangle.");
        }
        // Dynamically allocate a copy of the Point object referenced and assign new pointer to it
        vertex_1 = new Point(v1);
        vertex_2 = new Point(v2);
        vertex_3 = new Point(v3);
    }

    // Default constructor, setting pointers to null
    Triangle() : vertex_1(nullptr), vertex_2(nullptr), vertex_3(nullptr) {}

    // Move constructor
//    Triangle(Triangle&& other) noexcept
//            : vertex_1(other.vertex_1), vertex_2(other.vertex_2), vertex_3(other.vertex_3) {
//        other.vertex_1 = other.vertex_2 = other.vertex_3 = nullptr;
//    }

    // Clone function to create a deep copy of the triangle
    Triangle *clone() const {
        return new Triangle(*vertex_1, *vertex_2, *vertex_3);
    }

    // Prevent Copy constructor
    // Each Triangle is a uniquely labeled mesh in 3D space
    // (In favor of move semantics or deep copy clone)
    Triangle(const Triangle &) = delete;
    // Move ptr assignment constructor if need remove: Triangle& operator=(const Triangle&) = delete;

    // Destructor (freeing dynamically allocated memory i.e. the 3 raw vertices pointers)
    ~Triangle() {
        delete vertex_1;
        delete vertex_2;
        delete vertex_3;
    }

    // Utility Function to check if the points form a valid triangle using inequality theorem
    static bool isValidTrianglePoints(const Point& v1, const Point& v2, const Point& v3) {
        constexpr double EPSILON = 1e-9;
        double a = v1.distanceTo(v2);
        double b = v2.distanceTo(v3);
        double c = v3.distanceTo(v1);
        return a + b > c + EPSILON && b + c > a + EPSILON && c + a > b + EPSILON;
    }
};