/*
 * Authors: Antoine Cantin 40211205
 *          Etienne Plante 40236785
 * Assignment: COMP371 Assignment 1
 * Date: February 2025
 */
#ifndef PART2_H
#define PART2_H

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <tuple>
#include <cmath>
#include <string>
#include <algorithm>
namespace Part2Geometry {
    class Point {
        int x, y, z;

    public:
        // Constructor
        Point(int x = 0, int y = 0, int z = 0);
        // Copy constructor
        Point(const Point &other);
        // Destructor
        ~Point();

        // Getters
        int getX() const;

        int getY() const;

        int getZ() const;

        // Translate function
        int translate(int d, char axis);

        // Display function
        std::string toString() const;

        // Distance between this point and another point
        inline double distanceTo(const Point &other) const;

        // Operator overloads for vector operations
        Point operator+(const Point &other) const;

        Point operator-(const Point &other) const;

        Point operator*(int scalar) const;

        Point operator/(int scalar) const;

        // Comparison operators
        bool operator==(const Point &other) const;

        bool operator!=(const Point &other) const;

        // Convert to tuple
        inline std::tuple<int, int, int> as_tuple() const;

        // Stream output operator
        friend std::ostream &operator<<(std::ostream &os, const Point &point);
    };

    class Triangle {
        Point *vertex_1;
        Point *vertex_2;
        Point *vertex_3;

    public:
        // Constructors
        explicit Triangle(const Point &v1, const Point &v2, const Point &v3);

        Triangle();

        // Prevent Copy constructor
        // Each Triangle is a uniquely labeled mesh in 3D space
        // (In favor of move semantics or deep copy clone)
        Triangle(const Triangle &);


        // Destructor
        ~Triangle();

        // Clone function
        Triangle *clone() const;

        // Getters and Setters for vertices
        Point getVertex(int index) const;

        void setVertex(int index, const Point &v);

        // Translate function
        void translate(int d, char axis);

        // Calculate area
        double calcArea() const;

        // Display function
        std::string toString() const;

        // Stream output operator
        friend std::ostream &operator<<(std::ostream &os, const Triangle &tri);

        // Utility function to check for a valid triangle
        static bool isValidTrianglePoints(const Point &v1, const Point &v2, const Point &v3);

        // Safety function to check if the vertices are valid pointers
        bool areVerticesValidPointers() const;

        Triangle& operator=(const Triangle& other);
    };
}

namespace Part2Driver {
     class Driver {
        public:
            static void run();
            static Part2Geometry::Point getVertexCoordInput(const std::string &vertex_name);
    };
}

#endif // PART2_H
