//
// Created by stanislav_tomash on 20.10.2019.
//

#include "Interface.h"
#include <algorithm>

using namespace BigInt;

void Interface::acceptLoop() {
    while (true) {
        std::string line;
        std::getline(inputStream, line);

        std::vector<std::string> parsedArgs = parse(line);
        if (parsedArgs.empty()) continue;
        bool exitFlag = false;

        if (labNum == LAB_1) {
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
                    default:
                        processInvalid(parsedArgs);
                        break;
                }
            } catch (std::exception const &e) {
                outputStream << e.what() << "\n";
            }
            if (exitFlag)
                break;
        } else if (labNum == LAB_2) {
            try {
                switch (parseFunctionName(parsedArgs[0])) {
                    case FACTORIZE:
                        processFactorize(parsedArgs);
                        break;
                    case LOG:
                        processLog(parsedArgs);
                        break;
                    case EULER:
                        processEuler(parsedArgs);
                        break;
                    case MOBIUS:
                        processMobius(parsedArgs);
                        break;
                    case LEGENDRE:
                        processLegendre(parsedArgs);
                        break;
                    case JACOBI:
                        processJacobi(parsedArgs);
                        break;
                    case SQRTMOD:
                        processSqrtMod(parsedArgs);
                        break;
                    case IS_PRIME:
                        processIsPrime(parsedArgs);
                        break;
                    case EXIT:
                        exitFlag = true;
                        break;
                    default:
                        processInvalid(parsedArgs);
                        break;
                }
            } catch (std::exception const &e) {
                outputStream << e.what() << "\n";
            }
            if (exitFlag)
                break;
        }
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
    } else if (operationType == "factorize") {
        return OPERATION_TYPE::FACTORIZE;
    } else if (operationType == "log") {
        return OPERATION_TYPE::LOG;
    } else if (operationType == "euler") {
        return OPERATION_TYPE::EULER;
    } else if (operationType == "mobius") {
        return OPERATION_TYPE::MOBIUS;
    } else if (operationType == "legendre") {
        return OPERATION_TYPE::LEGENDRE;
    } else if (operationType == "jacobi") {
        return OPERATION_TYPE::JACOBI;
    } else if (operationType == "sqrtmod") {
        return OPERATION_TYPE::SQRTMOD;
    } else if (operationType == "prime") {
        return OPERATION_TYPE::IS_PRIME;
    }
    else {
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

void Interface::processFactorize(const std::vector<std::string> &args) {
    if (args.size() != 2)
        throw InvalidConsoleArguments();

    BigInteger arg;
    parseArgsToBigInts(args, arg);
    std::vector<BigInt::BigInteger> factors = BigInt::factorize(arg);
    for (const auto& factor : factors) {
        outputStream << factor << " ";
    }
    outputStream << "\n";
}

void Interface::processLog(const std::vector<std::string> &args) {
    if (args.size() != 4)
        throw InvalidConsoleArguments();

    BigInteger base, arg, modulo;
    parseArgsToBigInts(args, arg, base, modulo);
    BigInteger ans = BigInt::log(arg, base, modulo);
    if (ans == BigInteger(-1)) {
        outputStream << "No solution\n";
    } else {
        outputStream << ans << "\n";
    }
}

void Interface::processEuler(const std::vector<std::string> &args) {
    if (args.size() != 2)
        throw InvalidConsoleArguments();

    BigInteger arg;
    parseArgsToBigInts(args, arg);
    BigInteger ans = BigInt::eulerFunc(arg);
    outputStream << ans << "\n";
}

void Interface::processMobius(const std::vector<std::string> &args) {
    if (args.size() != 2)
        throw InvalidConsoleArguments();

    BigInteger arg;
    parseArgsToBigInts(args, arg);
    BigInteger ans = BigInt::mobiusFunc(arg);
    outputStream << ans << "\n";
}

void Interface::processLegendre(const std::vector<std::string> &args) {
    if (args.size() != 3)
        throw InvalidConsoleArguments();

    BigInteger a, p;
    parseArgsToBigInts(args, a, p);
    BigInteger ans = BigInt::legendreSymbol(a, p);
    outputStream << ans << "\n";
}

void Interface::processJacobi(const std::vector<std::string> &args) {
    if (args.size() != 3)
        throw InvalidConsoleArguments();

    BigInteger a, p;
    parseArgsToBigInts(args, a, p);
    BigInteger ans = BigInt::jacobiSymbol(a, p);
    outputStream << ans << "\n";
}
void Interface::processSqrtMod(const std::vector<std::string> &args) {
    if (args.size() != 3)
        throw InvalidConsoleArguments();

    BigInteger n, p;
    parseArgsToBigInts(args, n, p);
    BigInteger ans = BigInt::sqrtMod(n, p);
    if (ans == BigInteger(-1)) {
        outputStream << "No solution\n";
    } else {
        if (p - ans == ans) {
            outputStream << ans << "\n";
        } else {
            outputStream << ans << ", " << p - ans << "\n";
        }
    }
}

void Interface::processIsPrime(const std::vector<std::string> &args){
    if (args.size() != 2)
        throw InvalidConsoleArguments();

    BigInteger arg;
    parseArgsToBigInts(args, arg);
    bool ans = BigInt::isPrime(arg);
    outputStream << ans << "\n";
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

void Interface::chooseLab() {
    while (labNum == UNDEFINED) {
        int num;
        outputStream << "Please enter lab number: \n";
        inputStream >> num;
        if (num == 1) {
            labNum = LAB_1;
        } else if (num == 2) {
            labNum = LAB_2;
        } else {
            outputStream << "You've entered invalid number. Should be 1 or 2.\n";
        }
    }
}


