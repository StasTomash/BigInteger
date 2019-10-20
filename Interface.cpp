//
// Created by stanislav_tomash on 20.10.2019.
//

#include "Interface.h"
#include <algorithm>

void Interface::acceptLoop() {
    while (true) {
        std::string line;
        std::getline(inputStream, line);

        std::vector<std::string> parsedArgs = parse(line);
        if (parsedArgs.empty()) continue;
        bool exitFlag = false;

        try {
            switch (parseFunctionName(parsedArgs[0])) {

                case ADDITION:
                    processAddition(parsedArgs);
                    break;
                case SUBTRACTION:
                    processSubtraction(parsedArgs);
                    break;
                case MULTIPLICATION:
                    processMultiplication(parsedArgs);
                    break;
                case DIVISION:
                    processDivision(parsedArgs);
                    break;
                case REMAINDER:
                    processRemainder(parsedArgs);
                    break;
                case POWER:
                    processPower(parsedArgs);
                    break;
                case COMPARISON:
                    processComparison(parsedArgs);
                    break;
                case SQRT:
                    processSqrt(parsedArgs);
                    break;
                case SOLVE_SYSTEM:
                    processSolveSystem(parsedArgs);
                    break;
                case HELP:
                    processHelp(parsedArgs);
                    break;
                case INVALID:
                    processInvalid(parsedArgs);
                    break;
                case EXIT:
                    exitFlag = true;
                    break;
            }
        } catch (std::exception const& e) {
            outputStream << e.what() << "\n";
        }
        if (exitFlag)
            break;
    }
}

Interface::OPERATION_TYPE Interface::parseFunctionName(std::string operationType) {
    std::transform(operationType.begin(), operationType.end(), operationType.begin(),
                   [](unsigned char c) { return std::tolower(c);});

    if (operationType == "add") {
        return OPERATION_TYPE::ADDITION;
    } else if (operationType == "sub") {
        return OPERATION_TYPE::SUBTRACTION;
    } else if (operationType == "mult") {
        return OPERATION_TYPE::MULTIPLICATION;
    } else if (operationType == "div") {
        return OPERATION_TYPE::DIVISION;
    } else if (operationType == "mod") {
        return OPERATION_TYPE::REMAINDER;
    } else if (operationType == "pow") {
        return OPERATION_TYPE::POWER;
    } else if (operationType == "cmp") {
        return OPERATION_TYPE::COMPARISON;
    } else if (operationType == "sqrt") {
        return OPERATION_TYPE::SQRT;
    } else if (operationType == "solve") {
        return OPERATION_TYPE::SOLVE_SYSTEM;
    } else if (operationType == "help") {
        return OPERATION_TYPE::HELP;
    } else if (operationType == "exit") {
        return OPERATION_TYPE::EXIT;
    } else {
        return OPERATION_TYPE::INVALID;
    }
}

std::vector<std::string> Interface::parse(const std::string& rawString) {
    std::vector<std::string> result = std::vector<std::string>();
    std::stringstream ss;
    ss << rawString;
    std::string token;
    while (ss >> token) {
        result.push_back(token);
    }
    return result;
}

void parseArgsToBigInts(
        std::vector<std::string> args,
        BigInteger& leftArg,
        BigInteger& rightArg,
        BigInteger& modulo) {

    leftArg = BigInteger(args[1]);
    rightArg = BigInteger(args[2]);
    if (args.size() > 3)
        modulo = BigInteger(args[3]);
    else
        modulo = BigInteger(0);
}

void parseArgsToBigInts(
        std::vector<std::string> args,
        BigInteger& leftArg,
        BigInteger& rightArg) {

    leftArg = BigInteger(args[1]);
    rightArg = BigInteger(args[2]);
}

void parseArgsToBigInts(
        std::vector<std::string> args,
        BigInteger& argument) {

    argument = BigInteger(args[1]);
}

void Interface::processAddition(const std::vector<std::string>& args) {
    if (args.size() != 3 && args.size() != 4)
        throw InvalidConsoleArguments();

    BigInteger leftArg, rightArg, modulo;
    parseArgsToBigInts(args, leftArg, rightArg, modulo);
    BigInteger result = leftArg + rightArg;
    if (modulo != BigInteger(0)) {
        outputStream << result % modulo;
    } else {
        outputStream << result;
    }
    outputStream << "\n";
}

void Interface::processSubtraction(const std::vector<std::string> &args) {
    if (args.size() != 3 && args.size() != 4)
        throw InvalidConsoleArguments();

    BigInteger leftArg, rightArg, modulo;
    parseArgsToBigInts(args, leftArg, rightArg, modulo);
    BigInteger result = leftArg - rightArg;
    if (modulo != BigInteger(0)) {
        outputStream << result % modulo;
    } else {
        outputStream << result;
    }
    outputStream << "\n";
}

void Interface::processMultiplication(const std::vector<std::string> &args){
    if (args.size() != 3 && args.size() != 4)
        throw InvalidConsoleArguments();

    BigInteger leftArg, rightArg, modulo;
    parseArgsToBigInts(args, leftArg, rightArg, modulo);
    BigInteger result = leftArg * rightArg;
    if (modulo != BigInteger(0)) {
        outputStream << result % modulo;
    } else {
        outputStream << result;
    }
    outputStream << "\n";
}

void Interface::processDivision(const std::vector<std::string> &args){
    if (args.size() != 3 && args.size() != 4)
        throw InvalidConsoleArguments();

    BigInteger leftArg, rightArg, modulo;
    parseArgsToBigInts(args, leftArg, rightArg, modulo);
    BigInteger result = leftArg / rightArg;
    if (modulo != BigInteger(0)) {
        outputStream << result % modulo;
    } else {
        outputStream << result;
    }
    outputStream << "\n";
}

void Interface::processRemainder(const std::vector<std::string> &args){
    if (args.size() != 3)
        throw InvalidConsoleArguments();

    BigInteger leftArg, rightArg;
    parseArgsToBigInts(args, leftArg, rightArg);
    BigInteger result = leftArg % rightArg;
    outputStream << result;
    outputStream << "\n";
}

void Interface::processPower(const std::vector<std::string> &args) {
    if (args.size() != 3 && args.size() != 4)
        throw InvalidConsoleArguments();

    BigInteger leftArg, rightArg, modulo;
    parseArgsToBigInts(args, leftArg, rightArg, modulo);
    if (modulo != BigInteger(0)) {
        outputStream << leftArg.pow(rightArg, modulo);
    } else {
        outputStream << leftArg.pow(rightArg);
    }
    outputStream << "\n";
}

void Interface::processComparison(const std::vector<std::string> &args) {
    if (args.size() != 3)
        throw InvalidConsoleArguments();

    BigInteger leftArg, rightArg;
    parseArgsToBigInts(args, leftArg, rightArg);
    if (leftArg < rightArg) {
        outputStream << "Less";
    } else if (leftArg == rightArg) {
        outputStream << "Equal";
    } else {
        outputStream << "Greater";
    }
    outputStream << "\n";
}

void Interface::processSqrt(const std::vector<std::string> &args) {
    if (args.size() != 2)
        throw InvalidConsoleArguments();

    BigInteger arg;
    parseArgsToBigInts(args, arg);
    BigInteger result = arg.sqrt();
    outputStream << result;
    outputStream << "\n";
}

void Interface::processSolveSystem(const std::vector<std::string> &args) {
    if (args.size() != 2)
        throw InvalidConsoleArguments();

    int cnt;
    try {
        cnt = std::stoi(args[1]);
    } catch (std::invalid_argument const& e) {
        throw InvalidConsoleArguments();
    } catch (std::out_of_range const& e) {
        throw InvalidConsoleArguments();
    }

    std::vector<BigInteger> rem(cnt), mod(cnt);

    for (int i=0; i<cnt; i++) {
        outputStream << "A_" << i << " and P_" << i << ": \n";
        std::vector<std::string> curInput(3);
        inputStream >> curInput[1] >> curInput[2];
        parseArgsToBigInts(curInput, rem[i], mod[i]);
    }

    outputStream << BigInteger::restoreFromModuloes(rem, mod);
    outputStream << "\n";
}

void Interface::processHelp(const std::vector<std::string> &args) {
    outputStream << "Following commands are allowed: \n";
    outputStream << "\tadd x y [mod]\n";
    outputStream << "\tsub x y [mod]\n";
    outputStream << "\tmult x y [mod]\n";
    outputStream << "\tdiv x y [mod]\n";
    outputStream << "\tmod x y\n";
    outputStream << "\tpow x y [mod]\n";
    outputStream << "\tcmp x y\n";
    outputStream << "\tsqrt x\n";
    outputStream << "\tsolve cnt   [will prompt you for additional input]\n";
    outputStream << "\thelp\n";
}

void Interface::processInvalid(const std::vector<std::string>& /*args*/) {
    outputStream << "Invalid function provided";
}

