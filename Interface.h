//
// Created by stanislav_tomash on 20.10.2019.
//

#ifndef BIGINTEGERLAB_INTERFACE_H
#define BIGINTEGERLAB_INTERFACE_H

#include "BigInteger.h"
#include "BigIntegerAlgorithm.h"
#include <iostream>

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

        FACTORIZE,
        LOG,
        EULER,
        MOBIUS,
        LEGENDRE,
        JACOBI,
        SQRTMOD,
        IS_PRIME,
        ENCODE,

        HELP,
        INVALID,
        EXIT
    };

    enum LAB_NUM {
        UNDEFINED,
        LAB_1,
        LAB_2
    };

    std::istream& inputStream;
    std::ostream& outputStream;
    LAB_NUM labNum;

    void processAddition(const std::vector<std::string>& args);
    void processSubtraction(const std::vector<std::string>& args);
    void processMultiplication(const std::vector<std::string>& args);
    void processDivision(const std::vector<std::string>& args);
    void processRemainder(const std::vector<std::string>& args);
    void processPower(const std::vector<std::string>& args);
    void processComparison(const std::vector<std::string>& args);
    void processSqrt(const std::vector<std::string>& args);
    void processSolveSystem(const std::vector<std::string>& args);
    void processFactorize(const std::vector<std::string>& args);
    void processLog(const std::vector<std::string>& args);
    void processEuler(const std::vector<std::string>& args);
    void processMobius(const std::vector<std::string>& args);
    void processLegendre(const std::vector<std::string>& args);
    void processJacobi(const std::vector<std::string>& args);
    void processSqrtMod(const std::vector<std::string>& args);
    void processEncode(const std::vector<std::string>& args);
    void processIsPrime(const std::vector<std::string>& args);
    void processHelp(const std::vector<std::string>& args);
    void processInvalid(const std::vector<std::string>& args);

    static std::vector<std::string> parse(const std::string& rawString);
    static OPERATION_TYPE parseFunctionName(std::string operationType);

public:
    Interface(std::istream& is, std::ostream& os) : inputStream(is), outputStream(os), labNum(UNDEFINED) {};
    void acceptLoop();
    void chooseLab();
};


#endif //BIGINTEGERLAB_INTERFACE_H
