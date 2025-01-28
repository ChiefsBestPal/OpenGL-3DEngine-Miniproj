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

};
