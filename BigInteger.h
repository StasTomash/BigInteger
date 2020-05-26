#ifndef BIG_INTEGER_BIG_INTEGER_H
#define BIG_INTEGER_BIG_INTEGER_H

#include <vector>
#include <iostream>
#include <complex>

namespace BigInt {

    enum BigIntegerSign {
        MINUS,
        PLUS
    };

    typedef std::complex<double> fft_base;

    void fft(std::vector<fft_base> &arg, bool invert = false);

    struct InvalidBigIntegerStringException : public std::exception {
        const char *what() const noexcept override {
            return "Invalid string value for BigInteger cast";
        }
    };
    struct InvalidConsoleArguments : public std::exception {
        const char* what () const noexcept override {
            return "Invalid arguments. See \'help\' command for reference";
        }
    };

    class BigInteger {
    protected:
        static const int CELL_LENGTH = 6;
        static const int MODULO = 1000000;

        BigIntegerSign sign;
        std::vector<int> data;

        void inverse_sign();
        void cleanup_zeroes();
        BigInteger scale(int n) const;
        BigInteger scalar_mult(int n) const;
        static BigInteger getIntOfLen(int len);
        friend std::ostream &operator<<(std::ostream &os, const BigInteger &rhs);
        friend std::istream &operator>>(std::istream &is, BigInteger &rhs);

    public:
        explicit BigInteger(int x);
        explicit BigInteger(const std::string &s);
        int toInt() const;
        bool isOdd() const;
        BigInteger(const BigInteger &) = default;
        BigInteger();
        BigInteger abs() const;
        static BigInteger rand(const BigInteger& maxVal);

        bool operator==(const BigInteger &rhs) const;
        bool operator!=(const BigInteger &rhs) const;
        bool operator<(const BigInteger &rhs) const;
        bool operator<=(const BigInteger &rhs) const;
        bool operator>(const BigInteger &rhs) const;
        bool operator>=(const BigInteger &rhs) const;

        BigInteger operator+() const;
        BigInteger operator-() const;
        BigInteger operator+(const BigInteger &rhs) const;
        BigInteger operator-(const BigInteger &rhs) const;
        BigInteger operator*(const BigInteger &rhs) const;
        BigInteger operator/(const BigInteger &rhs) const;
        BigInteger operator%(const BigInteger &rhs) const;
        BigInteger &operator*=(const BigInteger &rhs);
        BigInteger &operator+=(const BigInteger &rhs);
        BigInteger &operator-=(const BigInteger &rhs);
        BigInteger &operator/=(const BigInteger &rhs);
        BigInteger &operator%=(const BigInteger &rhs);
        const BigInteger operator++(int);

        BigInteger pow(BigInteger exp, const BigInteger &modulo = BigInteger(0)) const;
        BigInteger sqrt() const;

        static BigInteger
            restoreFromModuloes(const std::vector<BigInteger> &remainders, const std::vector<BigInteger> &primes);

        static BigInteger getRandOfLen(int len);
        static BigInteger getRandOfBitLen(int len);
    };

    std::ostream &operator<<(std::ostream &os, const BigInteger &rhs);
    std::istream &operator>>(std::istream &is, BigInteger &rhs);
}

#endif //BIG_INTEGER_BIG_INTEGER_H