#include <iostream>
#include "BigInteger.h"
#include "Interface.h"

int main() {
    Interface mainInterface(std::cin, std::cout);
    mainInterface.acceptLoop();
}