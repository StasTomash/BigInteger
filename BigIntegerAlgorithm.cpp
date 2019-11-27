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

std::tuple<BigInt::KeyPair, BigInt::EllipticCurve::Point, BigInt::EllipticCurve::Point> BigInt::encode(const BigInt::BigInteger &arg) {
    if (arg > BigInteger(255) || arg < BigInteger(0)) {
        throw BigInt::InvalidConsoleArguments();
    }

    auto encoder = new EllipticCurve();
    auto seed = BigInteger::rand(encoder->p);
    auto basePoint = encoder->createPoint();
    auto Q = basePoint * seed;

    auto keyPair = BigInt::KeyPair({Q, seed});

    auto sessionKey = BigInteger::rand(encoder->p);
    auto M = Q * arg;
    auto a = basePoint * sessionKey;
    auto b = Q * sessionKey + M;

    auto ans = std::tuple{keyPair, a, b};
    return ans;
}

BigInt::BigInteger
BigInt::decode(const BigInt::EllipticCurve::Point &a,
               const BigInt::EllipticCurve::Point &b,
               const BigInt::KeyPair& keyPair) {
    auto M = b + (a * keyPair.privateKey).inverse();
    for (BigInteger m(0); m < BigInteger(256); m++) {
        if (keyPair.publicKey * m == M) {
            return m;
        }
    }
    return BigInteger(-1);
}

std::ostream &BigInt::operator<<(std::ostream &os, const BigInt::EllipticCurve::Point& pt) {
    os << "(" << pt.x << ", " << pt.y << ")";
    return os;
}

std::ostream &BigInt::operator<<(std::ostream &os, const BigInt::EllipticCurve &pt) {
    os << "a = " << pt.a << ", b = " << pt.b << ", p = " << pt.p;
    return os;
}

BigInt::BigInteger BigInt::gcdExtended(const BigInt::BigInteger &a, const BigInt::BigInteger &b, BigInt::BigInteger &x,
                                       BigInt::BigInteger &y) {
    if (a == BigInteger(0)) {
        x = BigInteger(0);
        y = BigInteger(1);
        return b;
    }
    BigInteger x1, y1, d;
    d = gcdExtended(b % a, a, x1, y1);
    x = y1 - (b / a) * x1;
    y = x1;
    return d;
}

BigInt::BigInteger BigInt::inverseInCircle(const BigInt::BigInteger &arg, const BigInteger& modulo) {
    BigInteger x, y, g;
    g = BigInt::gcdExtended(arg, modulo, x, y);
    if (g != BigInteger(1)) {
        throw;
    }
    return (x + modulo) % modulo;
}

BigInt::EllipticCurve::Point
BigInt::EllipticCurve::createPoint(const BigInt::BigInteger &x, const BigInt::BigInteger &y) {
    return BigInt::EllipticCurve::Point(x, y, this);
}

BigInt::EllipticCurve::Point BigInt::EllipticCurve::createPoint() {
    return BigInt::EllipticCurve::Point(this);
}

BigInt::EllipticCurve::Point BigInt::EllipticCurve::Point::operator+(const BigInt::EllipticCurve::Point &other) const {
    Point ans(BigInteger(0), BigInteger(0), this->parent);
    if (this->x == other.x && this->y != other.y) {
        return ans;
    }
    if (this->x == BigInteger(0) && this->y == BigInteger(0)) {
        return other;
    }
    if (other.x == BigInteger(0) && other.y == BigInteger(0)) {
        return *this;
    }
    BigInteger slope;
    if (*this == other) {
        slope = BigInteger(3) * this->x * this->x + this->parent->a
                * BigInt::inverseInCircle(BigInteger(2) * this->y , this->parent->p);
        slope %= this->parent->p;
    } else {
        slope = (this->y + this->parent->p - other.y)
                * BigInt::inverseInCircle(this->x + this->parent->p - other.x, this->parent->p);
        slope %= this->parent->p;
    }
    ans.x = (slope * slope + BigInteger(2) * this->parent->p - this->x - other.x) % this->parent->p;
    ans.y = (slope * (this->x - ans.x + this->parent->p) - this->y + this->parent->p) % this->parent->p;
    return ans;
}

bool BigInt::EllipticCurve::Point::operator==(const BigInt::EllipticCurve::Point& other) const {
    return (this->x == other.x && this->y == other.y);
}

BigInt::EllipticCurve::Point BigInt::EllipticCurve::Point::operator*(const BigInt::BigInteger &other) const {
    if (other == BigInteger(1)) {
        return *this;
    }
    if (other == BigInteger(0)) {
        return Point(BigInteger(0), BigInteger(0), this->parent);
    }
    Point ans(BigInteger(0), BigInteger(0), this->parent);
    Point cp(*this);
    BigInteger exp(other);
    while (exp != BigInteger(0)) {
        if (exp.isOdd()) {
            ans = ans + cp;
        }
        cp = cp + cp;
        exp /= BigInteger(2);
    }
    return ans;
}

BigInt::EllipticCurve::Point BigInt::EllipticCurve::Point::inverse() const {
    return BigInt::EllipticCurve::Point(x, parent->p - y, parent);
}
