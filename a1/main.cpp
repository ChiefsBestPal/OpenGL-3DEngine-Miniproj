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
