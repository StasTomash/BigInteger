//
// Created by stanislav_tomash on 05.11.2019.
//

#include "BigIntegerAlgorithm.h"
#include <map>
#include <algorithm>
#include <set>

std::vector<BigInt::BigInteger> BigInt::factorize(const BigInt::BigInteger& arg) {
    // Choosing F(X) = X^2 + 1
    //

    static const int NUMBER_OF_STEPS = 1000;
    std::vector<BigInt::BigInteger> ans;
    if (BigInt::isPrime(arg)) {
        ans.emplace_back(arg);
        return ans;
    }
    if (arg <= BigInteger(10000000)) {
        return smallFactorize(arg);
    }
    for (const auto& x : smallPrimes) {
        std::vector<BigInt::BigInteger> allNumbers;
        allNumbers.emplace_back(x);
        for (int i=1; i<NUMBER_OF_STEPS; i++) {
            allNumbers.emplace_back((allNumbers[i-1] * allNumbers[i-1] + BigInteger(1)) % arg);
            BigInt::BigInteger d = BigInt::gcd(arg, (allNumbers[i]-allNumbers[i/2]).abs());
            if (d != BigInteger(1)) {
                std::vector<BigInt::BigInteger> factors = factorize(d);
                ans.insert(ans.end(), factors.begin(), factors.end());
                std::vector<BigInt::BigInteger> additionalFactors = factorize(arg / d);
                ans.insert(ans.end(), additionalFactors.begin(), additionalFactors.end());
                std::sort(ans.begin(), ans.end());
                return ans;
            }
        }
    }
    ans.emplace_back(arg);
    std::sort(ans.begin(), ans.end());
    return ans;
}

std::vector<BigInt::BigInteger> BigInt::smallFactorize(const BigInt::BigInteger& arg) {
    int cp = arg.toInt();
    std::vector<BigInt::BigInteger> ans;
    for (int i=2; i*i <= cp; i++)
        while (cp % i == 0) {
            ans.emplace_back(BigInteger(i));
            cp /= i;
        }
    if (cp != 1) ans.emplace_back(BigInteger(cp));
    return ans;
}

bool BigInt::isPrime(const BigInt::BigInteger& arg) {
    if (arg < BigInteger(1000000)) {
        return smallIsPrime(arg);
    }
    BigInteger curPower(1), curExp(0);
    while ((arg - BigInteger(1)) % (curPower * BigInteger(2)) == BigInteger(0)) {
        curPower *= BigInteger(2);
        curExp++;
    }
    BigInteger rem((arg - BigInteger(1)) / curPower);

    for (const auto& x : smallPrimes) {
        if (x.pow(rem, arg) == BigInteger(1)) {
            continue;
        }
        curPower = BigInteger(1);
        bool isEvidence = false;
        for (BigInteger i(0); i < curExp; i++) {
            if (x.pow(curPower * rem, arg) == arg - BigInteger(1)) {
                isEvidence = true;
                break;
            }
            curPower *= BigInteger(2);
        }
        if (!isEvidence) {
            return false;
        }
    }
    return true;
}

bool BigInt::smallIsPrime(const BigInt::BigInteger& arg) {
    int cp = arg.toInt();
    for (int i=2; i*i <= cp; i++) {
        if (cp % i == 0) {
            return false;
        }
    }
    return true;
}

BigInt::BigInteger BigInt::gcd(const BigInt::BigInteger& a, const BigInt::BigInteger& b) {
    if (a == BigInteger(0) || b == BigInteger(0)) {
        return a + b;
    }
    return gcd(b, a % b);
}

BigInt::BigInteger BigInt::log(const BigInteger& arg, const BigInteger& base, const BigInteger& modulo) {
    BigInteger m = modulo.sqrt() + BigInteger(1);
    BigInteger multiplier = base.pow(m, modulo);
    BigInteger curPow = multiplier;
    std::map<BigInteger, BigInteger> table;
    for (BigInteger i(1); i <= m; i++) {
        if (table.find(curPow) == table.end())
            table[curPow] = i;
        curPow = (curPow * multiplier) % modulo;
    }
    BigInteger curCheck = arg;
    for (BigInteger j(0); j < m; j++) {
        if (table.find(curCheck) != table.end()) {
            return table[curCheck] * m - j;
        }
        curCheck = (curCheck * base) % modulo;
    }
    return BigInteger(0);
}

BigInt::BigInteger BigInt::eulerFunc(const BigInt::BigInteger &arg) {
    std::vector<BigInt::BigInteger> factors = BigInt::factorize(arg);
    std::set<BigInt::BigInteger> uniqueFactors(factors.begin(), factors.end());
    BigInteger ans = arg;
    for (const auto& factor : uniqueFactors) {
        ans -= ans / factor;
    }
    return ans;
}

BigInt::BigInteger BigInt::mobiusFunc(const BigInt::BigInteger &arg) {
    std::vector<BigInt::BigInteger> factors = BigInt::factorize(arg);
    std::set<BigInt::BigInteger> uniqueFactors(factors.begin(), factors.end());
    if (factors.size() != uniqueFactors.size()) {
        return BigInteger(0);
    } else if (factors.size() % 2 == 0) {
        return BigInteger(1);
    } else {
        return BigInteger(-1);
    }
}

BigInt::BigInteger BigInt::legendreSymbol(const BigInt::BigInteger &a, const BigInt::BigInteger &p) {
    BigInteger cp = a;
    while (cp < BigInteger(0)) {
        cp = a + p;
    }
    BigInteger ans = cp.pow((p - BigInteger(1)) / BigInteger(2), p);
    if (ans == p - BigInteger(1)) {
        return BigInteger(-1);
    } else {
        return ans;
    }
}

BigInt::BigInteger BigInt::jacobiSymbol(const BigInt::BigInteger &a, const BigInt::BigInteger &n) {
    std::vector<BigInt::BigInteger> factors = BigInt::factorize(n);
    BigInteger ans(1);
    for (const auto& factor : factors) {
        ans *= legendreSymbol(a, factor);
        if (ans == BigInteger(0)) {
            break;
        }
    }
    return ans;
}

BigInt::BigInteger BigInt::sqrtMod(const BigInt::BigInteger &arg, const BigInt::BigInteger &modulo) {
    if (BigInt::legendreSymbol(arg, modulo) != BigInteger(1)) {
        return BigInteger(-1);
    }

    BigInteger a(0);
    while (true) {
        if (BigInt::legendreSymbol(a * a - arg, modulo) == BigInteger(-1)) {
              break;
        } else {
            a++;
        }
    }
    auto multiplyPair = [omega = a * a - arg, p = modulo](const std::pair<BigInteger, BigInteger>& lhs,
                                                          const std::pair<BigInteger, BigInteger>& rhs) {
        return std::make_pair((lhs.first * rhs.first + lhs.second * rhs.second * omega) % p,
                              (lhs.first * rhs.second + lhs.second * rhs.first));
    };
    auto powResult = std::make_pair(BigInteger(1), BigInteger(0));
    auto base = std::make_pair(a, BigInteger(1));
    auto exp = (modulo + BigInteger(1)) / BigInteger(2);

    while (exp > BigInteger(0)) {
        if (exp.isOdd()) {
            powResult = multiplyPair(powResult, base);
        }
        base = multiplyPair(base, base);
        exp /= BigInteger(2);
    }
    if (powResult.second == BigInteger(0)) return BigInteger(-1);
    BigInteger ans = powResult.first;
    if ((ans * ans) % modulo != arg) return BigInteger(-1);
    return ans;
}

