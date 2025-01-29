#include "Part2.h"

using namespace std;

namespace Part2Geometry {
// Point class implementation

// Constructor
    Point::Point(int x, int y, int z) : x(x), y(y), z(z) {}

// Destructor (default, as no dynamic memory needs to be freed from heap)
    Point::~Point() {}

// Getters
    int Point::getX() const { return x; }

    int Point::getY() const { return y; }

    int Point::getZ() const { return z; }

// Translate function to move point along specified axis
    int Point::translate(int d, char axis) {
        switch (axis) {
            case 'x':
                x += d;
                break;
            case 'y':
                y += d;
                break;
            case 'z':
                z += d;
                break;
            default:
                return -1;  // Invalid axis value
        }
        return 0;  // Successful translation
    }

// Display function to show point coordinates
    string Point::toString() const {
        stringstream ss;
        ss << "Point(" << x << "," << y << "," << z << ")";
        return ss.str();
    }

// Distance between this point and another point
    inline double Point::distanceTo(const Point &other) const {
        return sqrt(pow(x - other.x, 2) + pow(y - other.y, 2) + pow(z - other.z, 2));
    }

// Operator overload for vector operations
    Point Point::operator+(const Point &other) const { return {x + other.x, y + other.y, z + other.z}; }

    Point Point::operator-(const Point &other) const { return {x - other.x, y - other.y, z - other.z}; }

    Point Point::operator*(int scalar) const { return {x * scalar, y * scalar, z * scalar}; }

    Point Point::operator/(int scalar) const { return {x / scalar, y / scalar, z / scalar}; }

// Operator overload for boolean comparison operations
    bool Point::operator==(const Point &other) const { return x == other.x && y == other.y && z == other.z; }

    bool Point::operator!=(const Point &other) const { return !(*this == other); }

// Tuple conversion for easy unpacking of coordinates
    inline tuple<int, int, int> Point::as_tuple() const { return make_tuple(x, y, z); }

// Non-member friend function ostream overload for easy printing of Point objects
    ostream &operator<<(ostream &os, const Point &point) { return os << point.toString(); }



// Triangle class implementation

// Explicit Constructor with three Point objects
    Triangle::Triangle(const Point &v1, const Point &v2, const Point &v3) {
        if (!Triangle::isValidTrianglePoints(v1, v2, v3)) {
            throw logic_error("The points do not form a valid triangle.");
        }
        // Dynamically allocate a copy of the Point object referenced and assign new pointer to it
        vertex_1 = new Point(v1);
        vertex_2 = new Point(v2);
        vertex_3 = new Point(v3);
    }

// Default constructor, setting pointers to null
    Triangle::Triangle() : vertex_1(nullptr), vertex_2(nullptr), vertex_3(nullptr) {}

// Move constructor
//    Triangle(Triangle&& other) noexcept
//            : vertex_1(other.vertex_1), vertex_2(other.vertex_2), vertex_3(other.vertex_3) {
//        other.vertex_1 = other.vertex_2 = other.vertex_3 = nullptr;
//    }

// Clone function to create a deep copy of the triangle
    Triangle *Triangle::clone() const {
        return new Triangle(*vertex_1, *vertex_2, *vertex_3);
    }

// Prevent Copy constructor
// Each Triangle is a uniquely labeled mesh in 3D space
// (In favor of move semantics or deep copy clone)
// Triangle(const Triangle&) = delete;
// Move ptr assignment constructor if need remove: Triangle& operator=(const Triangle&) = delete;

// Destructor (freeing dynamically allocated memory i.e. the 3 raw vertices pointers)
    Triangle::~Triangle() {
        delete vertex_1;
        delete vertex_2;
        delete vertex_3;
    }

// Getter (Use point index for less verbosity)
    Point *Triangle::getVertex(int index) const {
        switch (index) {
            case 1:
                return vertex_1;
            case 2:
                return vertex_2;
            case 3:
                return vertex_3;
            default:
                return nullptr;
        }
    }

// Setters (Use point index for less verbosity)
    void Triangle::setVertex(int index, const Point &v) {
        Point *vertices[] = {vertex_1, vertex_2, vertex_3};
        int v1 = (index + 1) % 3, v2 = (index + 2) % 3;
        if (!Triangle::isValidTrianglePoints(v, *vertices[v1], *vertices[v2])) {
            throw invalid_argument("The new point does not form a valid triangle.");
        }
        *vertices[index] = v;
    }

// Translate function for the entire triangle
    void Triangle::translate(int d, char axis) {
        if (vertex_1) vertex_1->translate(d, axis);
        if (vertex_2) vertex_2->translate(d, axis);
        if (vertex_3) vertex_3->translate(d, axis);

        if (!Triangle::isValidTrianglePoints(*vertex_1, *vertex_2, *vertex_3)) {
            throw logic_error("The points after transform do not form a valid triangle.");
        }
    }

/*
 * Returns the area of the triangle using the cross product magnitude method in 3D Euclidean space
 *
 * Area of a triangle with vertices A(x1, y1, z1), B(x2, y2, z2), C(x3, y3, z3) is given by:
 * 0.5 * |AB x AC|
 */
    double Triangle::calcArea() const {
        if (!vertex_1 || !vertex_2 || !vertex_3) return 0.0;

        // Using the cross product method to calculate area of triangle in R3 space
        auto [x1, y1, z1] = vertex_1->as_tuple();
        auto [x2, y2, z2] = vertex_2->as_tuple();
        auto [x3, y3, z3] = vertex_3->as_tuple();

        // Vectors AB and AC
        int ABx = x2 - x1, ABy = y2 - y1, ABz = z2 - z1;
        int ACx = x3 - x1, ACy = y3 - y1, ACz = z3 - z1;

        // Cross product of AB and AC by 3x3 matrix determinant
        long long cross_x = static_cast<long long>(ABy) * ACz - static_cast<long long>(ABz) * ACy;
        long long cross_y = static_cast<long long>(ABz) * ACx - static_cast<long long>(ABx) * ACz;
        long long cross_z = static_cast<long long>(ABx) * ACy - static_cast<long long>(ABy) * ACx;

        // Area of triangle is half the magnitude of the cross product AB x AC
        // (Since that cross product is a normal vector to the triangle plane and that vector's magnitude is the area of the parallelogram formed by the sides vectors)
        long long cross_product_magnitude_squared = cross_x * cross_x + cross_y * cross_y + cross_z * cross_z;
        if (cross_product_magnitude_squared < 0) {
            throw overflow_error("Overflow occurred during area calculation.");
        }

        double area = 0.5 * sqrt(static_cast<double>(cross_product_magnitude_squared));
        if (isinf(area) || isnan(area)) {
            throw runtime_error("Invalid numerical area calculation.");
        }

        return area;
    }

// Display function to show triangle coordinates
    string Triangle::toString() const {
        stringstream ss;
        ss << "Triangle: {\n  Vertex1: (" << vertex_1->getX() << "," << vertex_1->getY() << "," << vertex_1->getZ()
           << ")\n"
           << "  Vertex2: (" << vertex_2->getX() << "," << vertex_2->getY() << "," << vertex_2->getZ() << ")\n"
           << "  Vertex3: (" << vertex_3->getX() << "," << vertex_3->getY() << "," << vertex_3->getZ() << ")\n}";
        return ss.str();
    }

// Non-member friend function ostream overload for easy printing of Triangle objects
    ostream &operator<<(ostream &os, const Triangle &tri) { return os << tri.toString(); }

// Utility static Function to check if the points form a valid triangle using inequality theorem
    bool Triangle::isValidTrianglePoints(const Point &v1, const Point &v2, const Point &v3) {
        constexpr double EPSILON = 1e-9;
        double a = v1.distanceTo(v2);
        double b = v2.distanceTo(v3);
        double c = v3.distanceTo(v1);
        return a + b > c + EPSILON && b + c > a + EPSILON && c + a > b + EPSILON;
    }
}
namespace Part2Driver {
    void Driver::run() {
        Part2Geometry::Triangle triangle(Part2Geometry::Point(0, 0), Part2Geometry::Point(1, 0), Part2Geometry::Point(0, 1));
        bool exit = false;

        while (!exit) {
            std::cout << "\n--- Triangle Operations ---\n";
            std::cout << "1. Display triangle coordinates\n";
            std::cout << "2. Translate triangle\n";
            std::cout << "3. Exit\n";
            std::cout << "Choose an option: ";
            int choice;
            std::cin >> choice;

            switch (choice) {
                case 1: {
                    std::cout << triangle.toString() << std::endl;
                    break;
                }
                case 2: {
                    float dx, dy;
                    std::cout << "Enter translation distance for x and y: ";
                    std::cin >> dx >> dy;
                    triangle.translate(dx, dy);
                    std::cout << "Triangle translated by (" << dx << ", " << dy << ")\n";
                    break;
                }
                case 3:
                    exit = true;
                    break;
                default:
                    std::cout << "Invalid option. Please try again.\n";
                    break;
            }
        }
    }
}

// Driver code
// TODO Make better menu system with options to retry
// TODO Assignment mentions "Driver Class" so maybe make a class for this
int part2_main() {
    Triangle t;
    int choice;
    do {
        cout << "Menu: \n";
        cout << "0. Create Triangle\n";
        cout << "1. Display Triangle Coordinates\n";
        cout << "2. Translate Triangle\n";
        cout << "3. Calculate Triangle Area\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch(choice) {
            case 0:
                int x1, y1, z1, x2, y2, z2, x3, y3, z3;
                cout << "Enter coordinates of vertex 1 (x, y, z): ";
                cin >> x1 >> y1 >> z1;
                cout << "Enter coordinates of vertex 2 (x, y, z): ";
                cin >> x2 >> y2 >> z2;
                cout << "Enter coordinates of vertex 3 (x, y, z): ";
                cin >> x3 >> y3 >> z3;
                t = Triangle(Point(x1, y1, z1), Point(x2, y2, z2), Point(x3, y3, z3));
                cout << "Triangle created successfully.\n";
                break;
            case 1:
                cout << t << endl;
                break;
            case 2:
                int d;
                char axis;
                cout << "Enter translation distance (d) and axis (x, y, z): ";
                cin >> d >> axis;
                t.translate(d, axis);
                break;
            case 3:
                cout << "Area of the triangle: " << t.calcArea() << endl;
                break;
            case 4:
                cout << "Exiting...\n";
                return 0;
            default:
                cout << "Invalid choice! Try again.\n";
                break;
        }
    } while(true);

    return 0;
}