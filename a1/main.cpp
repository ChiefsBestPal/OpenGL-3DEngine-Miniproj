/*
 * Authors: Antoine Cantin 40211205
 *          Etienne Plante 40236785
 * Assignment: COMP371 Assignment 1
 * Date: February 2025
 */
#include "Part1.h"
#include "Part2.h"

int main()
{

    std::cout << "Part 1" << std::endl;
    part1_main();

    std::cout << "\n\r>> \033[1mClick Any Key to Proceed to Part 2\033[0m\n";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer
    std::cin.get();

    std::cout << "Part 2" << std::endl;
    Part2Driver::Driver::run();

    return 0;
}
