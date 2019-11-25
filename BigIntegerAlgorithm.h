//
// Created by stanislav_tomash on 05.11.2019.
//

#ifndef BIGINTEGERLAB_BIGINTEGERALGORITHM_H
#define BIGINTEGERLAB_BIGINTEGERALGORITHM_H

#include "BigInteger.h"

namespace BigInt {
    static const std::vector<BigInteger> smallPrimes({BigInteger(2), // NOLINT(cert-err58-cpp)
                                                      BigInteger(3),
                                                      BigInteger(5),
                                                      BigInteger(7),
                                                      BigInteger(11),
                                                      BigInteger(13)});

    BigInteger gcd(const BigInteger&, const BigInteger&);
    bool isPrime(const BigInteger&);
    bool smallIsPrime(const BigInteger&);
    std::vector<BigInteger> factorize(const BigInteger&);
    std::vector<BigInteger> smallFactorize(const BigInteger&);
    BigInteger log(const BigInteger& arg, const BigInteger& base, const BigInteger& modulo);
    BigInteger eulerFunc(const BigInteger& arg);
    BigInteger mobiusFunc(const BigInteger& arg);
    BigInteger legendreSymbol(const BigInteger& a, const BigInteger& p);
    BigInteger jacobiSymbol(const BigInteger& a, const BigInteger& n);
    BigInteger sqrtMod(const BigInteger& arg, const BigInteger& modulo);
}

#endif //BIGINTEGERLAB_BIGINTEGERALGORITHM_H

