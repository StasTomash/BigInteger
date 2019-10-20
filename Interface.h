//
// Created by stanislav_tomash on 20.10.2019.
//

#ifndef BIGINTEGERLAB_INTERFACE_H
#define BIGINTEGERLAB_INTERFACE_H

#include "BigInteger.h"
#include <iostream>

struct InvalidConsoleArguments : public std::exception {
    const char* what () const noexcept override {
        return "Invalid arguments. See \'help\' command for reference";
    }
};

class Interface {
private:
    enum OPERATION_TYPE {
        ADDITION,
        SUBTRACTION,
        MULTIPLICATION,
        DIVISION,
        REMAINDER,
        POWER,
        COMPARISON,
        SQRT,
        SOLVE_SYSTEM,

        HELP,
        INVALID,
        EXIT
    };

    std::istream& inputStream;
    std::ostream& outputStream;

    void processAddition(const std::vector<std::string>& args);
    void processSubtraction(const std::vector<std::string>& args);
    void processMultiplication(const std::vector<std::string>& args);
    void processDivision(const std::vector<std::string>& args);
    void processRemainder(const std::vector<std::string>& args);
    void processPower(const std::vector<std::string>& args);
    void processComparison(const std::vector<std::string>& args);
    void processSqrt(const std::vector<std::string>& args);
    void processSolveSystem(const std::vector<std::string>& args);
    void processHelp(const std::vector<std::string>& args);
    void processInvalid(const std::vector<std::string>& args);

    static std::vector<std::string> parse(const std::string& rawString);
    static OPERATION_TYPE parseFunctionName(std::string operationType);

public:
    Interface(std::istream& is, std::ostream& os) : inputStream(is), outputStream(os) {};
    void acceptLoop();
};


#endif //BIGINTEGERLAB_INTERFACE_H
