#include "BigInteger.h"

#include <iostream>
#include <cmath>
#include <algorithm>

BigInteger::BigInteger() {
    this -> sign = PLUS;
    this -> data = std::vector<int>();
}

BigInteger::BigInteger(int x) {
    this -> sign = x >= 0 ? PLUS : MINUS;
    this -> data = std::vector<int>();
    this -> data.push_back(std::abs(x));
    this -> cleanup_zeroes();
}

BigInteger BigInteger::operator+() const {
    return BigInteger(*this);
}

BigInteger BigInteger::operator-() const {
    BigInteger result = BigInteger(*this);
    result.inverse_sign();
    result.cleanup_zeroes();
    return result;
}

BigInteger BigInteger::abs() const {
    BigInteger result = BigInteger(*this);
    result.sign = PLUS;
    return result;
}

bool BigInteger::operator==(const BigInteger &rhs) const {
    return (sign == rhs.sign && data == rhs.data);
}

bool BigInteger::operator!=(const BigInteger &rhs) const {
    return !((*this) == rhs);
}

bool BigInteger::operator<(const BigInteger &rhs) const {
    if (sign != rhs.sign) {
        return sign == MINUS;
    }
    int len1 = data.size();
    int len2 = rhs.data.size();
    if (len1 != len2) {
        return sign == PLUS ? len1 < len2 : len1 > len2;
    }
    for (auto pos1 = data.end() - 1, pos2 = rhs.data.end() - 1; pos1 >= data.begin(); pos1--, pos2--) {
        if (*pos1 != *pos2) {
            return sign == PLUS ? *pos1 < *pos2 : *pos1 > *pos2;
        }
    }
    return false;
}

bool BigInteger::operator>(const BigInteger &rhs) const {
    return !((*this) == rhs) && !((*this) < rhs);
}

bool BigInteger::operator<=(const BigInteger &rhs) const {
    return ((*this) == rhs || (*this) < rhs);
}

bool BigInteger::operator>=(const BigInteger &rhs) const {
    return ((*this) == rhs || (*this) > rhs);
}

BigInteger BigInteger::operator+(const BigInteger& rhs) const {
    BigInteger result;
    if (this -> sign == rhs.sign) {
        auto pos1 = this -> data.begin();
        auto pos2 = rhs.data.begin();
        int rem = 0;
        while (pos1 != this -> data.end() || pos2 != rhs.data.end() || rem > 0) {
            int left_add = pos1 != this -> data.end() ? *pos1 : 0;
            int right_add = pos2 != rhs.data.end() ? *pos2 : 0;
            int cur_res = left_add + right_add + rem;

            rem = cur_res / MODULO;
            result.data.push_back(cur_res % MODULO);

            if (pos1 != this -> data.end()) pos1++;
            if (pos2 != rhs.data.end()) pos2++;
        }
        result.sign = this -> sign;
    } else {
        BigInteger smaller, bigger;
        if ((*this).abs() < rhs.abs()) {
            smaller = *this;
            bigger = rhs;
        } else {
            smaller = rhs;
            bigger = *this;
        }
        auto pos1 = bigger.data.begin();
        auto pos2 = smaller.data.begin();
        int rem = 0;
        while (pos1 != bigger.data.end() || pos2 != smaller.data.end() || rem > 0) {
            int left_operand = pos1 != bigger.data.end() ? *pos1 : 0;
            int right_operand = pos2 != smaller.data.end() ? *pos2 : 0;
            int cur_res = left_operand - right_operand - rem;

            if (cur_res >= 0) {
                rem = 0;
                result.data.push_back(cur_res);
            } else {
                rem = (std::abs(cur_res) + MODULO - 1) / MODULO;
                result.data.push_back(cur_res + rem * MODULO);
            }

            if (pos1 != bigger.data.end()) pos1++;
            if (pos2 != smaller.data.end()) pos2++;
        }
        result.sign = bigger.sign;
        result.cleanup_zeroes();
    }
    return result;
}

BigInteger BigInteger::operator-(const BigInteger& rhs) const {
    return (*this) + (-rhs);
}

void BigInteger::cleanup_zeroes() {
    while (!data.empty() && data.back() == 0) {
        data.pop_back();
    }
    if (data.empty()) {
        sign = PLUS;
    }
}

void BigInteger::inverse_sign() {
    if (this -> sign == MINUS) {
        this -> sign = PLUS;
    } else {
        this -> sign = MINUS;
    }
}

std::ostream &operator<<(std::ostream &os, const BigInteger &rhs) {
    if (rhs.sign == MINUS) {
        os << '-';
    }
    if (rhs.data.empty()) {
        os << 0;
    }
    for (auto it = rhs.data.end() - 1; it >= rhs.data.begin(); it--) {
        std::string current_str = std::to_string(*it);
        if (it != rhs.data.end() - 1) {
            while (current_str.length() < BigInteger::CELL_LENGTH) {
                current_str = '0' + current_str; // NOLINT(performance-inefficient-string-concatenation)
            }
        }
        os << current_str;
    }
    return os;
}

BigInteger::BigInteger(const std::string& s) {
    if (s.length() == 0) {
        throw InvalidBigIntegerStringException();
    }
    auto pos = s.begin();
    if (*pos == '-') {
        sign = MINUS;
        pos++;
    } else if (*pos == '+') {
        sign = PLUS;
        pos++;
    } else {
        sign = PLUS;
    }
    int current_num = 0, current_pow = 1;
    for (auto it = s.end() - 1; it >= pos; it--) {
        if (current_pow == MODULO) {
            data.push_back(current_num);
            current_num = 0;
            current_pow = 1;
        }
        if (!isdigit(*it)) {
            throw InvalidBigIntegerStringException();
        }
        current_num += current_pow * int(*it - '0');
        current_pow *= 10;
    }
    if (current_num > 0) {
        data.push_back(current_num);
    }
}

std::istream &operator>>(std::istream &is, BigInteger &rhs) {
    std::string s;
    is >> s;
    rhs = BigInteger(s);
    return is;
}

BigInteger BigInteger::operator*(const BigInteger &rhs) const {
    BigInteger result;

    std::vector<long long> result_vec;
    std::vector<fft_base> left_arg(data.begin(), data.end()), right_arg(rhs.data.begin(), rhs.data.end());
    size_t n = 1;
    while (n < std::max(data.size(), rhs.data.size())) {
        n *= 2;
    }
    n *= 2;
    left_arg.resize(n);
    right_arg.resize(n);
    fft(left_arg, false);
    fft(right_arg, false);
    for (size_t i = 0; i < n; i++) {
        left_arg[i] *= right_arg[i];
    }
    fft(left_arg, true);
    result_vec.resize(n);
    for (size_t i = 0; i < n; i++) {
        result_vec[i] = (long long)(floor(left_arg[i].real() + 0.5));
    }
    int rem = 0;
    auto pos = result_vec.begin();
    while (rem > 0 || pos != result_vec.end())
    {
        long long add = (pos != result_vec.end() ? *pos : 0);
        long long cur_res = add + rem;

        result.data.push_back(cur_res % MODULO);
        rem = cur_res / MODULO;

        if (pos != result_vec.end()) pos++;
    }

    if (sign == rhs.sign) {
        result.sign = PLUS;
    } else {
        result.sign = MINUS;
    }

    result.cleanup_zeroes();
    return result;
}

BigInteger BigInteger::pow(BigInteger exp, const BigInteger& modulo) const {
    BigInteger mult = *this;
    BigInteger result = BigInteger(1);
    while (exp != BigInteger(0)) {
        if (exp.data[0] % 2 == 1) {
            result *= mult;
            if (modulo != BigInteger(0)) {
                result %= modulo;
            }
        }
        mult *= mult;
        exp /= BigInteger(2);
    }
    return result;
}

BigInteger &BigInteger::operator*=(const BigInteger &rhs) {
    BigInteger result = (*this) * rhs;
    this -> sign = result.sign;
    this -> data = result.data;
    return *this;
}

BigInteger BigInteger::getIntOfLen(int len) {
    BigInteger res(1);
    return res.scale(len - 1);
}

BigInteger BigInteger::operator/(const BigInteger &rhs) const {
    std::vector<int> resultVec;

    if (this->data.size() < rhs.data.size())
        resultVec = std::vector<int>(0, 1);
    else {

        BigInteger curPow = getIntOfLen(this->data.size() - rhs.data.size() + 1);
        BigInteger currentSum = BigInteger(0);

        while (curPow != BigInteger(0)) {
            int lower_bound = 0, upper_bound = MODULO - 1;
            while (upper_bound != lower_bound) {
                int middle = (lower_bound + upper_bound + 1) / 2;
                BigInteger middleMultiplier = curPow.scalar_mult(middle);
                if (middleMultiplier * rhs.abs() + currentSum <= this -> abs())
                    lower_bound = middle;
                else
                    upper_bound = middle - 1;
            }
            resultVec.push_back(lower_bound);
            currentSum += curPow.scalar_mult(lower_bound) * rhs.abs();
            curPow = curPow.scale(-1);
        }
        std::reverse(resultVec.begin(), resultVec.end());
    }
    BigInteger result;
    result.sign = (this->sign == rhs.sign ? BigIntegerSign::PLUS : BigIntegerSign::MINUS);
    if (result.sign == BigIntegerSign::PLUS) {
        result.data = resultVec;
    } else {
        result.data = resultVec;
        if (result.abs() * rhs.abs() != this->abs())
            result -= BigInteger(1);
    }
    result.cleanup_zeroes();
    return result;
}

BigInteger BigInteger::scale(int n) const {
    BigInteger result = *this;
    if (n >= 0) {
        std::vector<int> zeroes(n, 0);
        result.data.reserve(result.data.size() + zeroes.size());
        result.data.insert(result.data.begin(), zeroes.begin(), zeroes.end());
    } else {
        result.data = std::vector<int>(result.data.begin() - n, result.data.end());
    }
    return result;
}

BigInteger BigInteger::scalar_mult(int n) const {
    BigInteger result = *this;
    for (auto& x : result.data) {
        x *= n;
    }
    return result;
}

BigInteger &BigInteger::operator+=(const BigInteger &rhs) {
    *this = *this + rhs;
    return *this;
}

BigInteger &BigInteger::operator-=(const BigInteger &rhs) {
    *this = *this - rhs;
    return *this;
}

BigInteger BigInteger::operator%(const BigInteger &rhs) const {
    BigInteger result;
    return (*this) - rhs * (*this / rhs);
}

BigInteger &BigInteger::operator/=(const BigInteger &rhs) {
    *this = *this / rhs;
    return *this;
}

BigInteger BigInteger::sqrt() const {
    BigInteger result = getIntOfLen(this->data.size() / 2);
    bool revert = false;
    while (true) {
        BigInteger nextResult = (*this / result + result) / BigInteger(2);
        if (result == nextResult || (result < nextResult && revert)) {
            break;
        }
        revert = nextResult < result;
        result = nextResult;
    }
    return result;
}

BigInteger BigInteger::restoreFromModuloes(const std::vector<BigInteger>& remainders,
                                           const std::vector<BigInteger>& primes) {

    if (remainders.size() != primes.size()) {
        throw InvalidBigIntegerStringException();
    }

    BigInteger result;
    std::vector<std::vector<BigInteger> > reciprocals(primes.size(), std::vector<BigInteger>(primes.size()));

    for (int i=0; i<primes.size(); i++)
        for (int j=0; j<primes.size(); j++)
            reciprocals[i][j] = primes[i].pow(primes[j] - BigInteger(2), primes[j]);

    BigInteger curMultiplier(1);
    std::vector<BigInteger> answerVec(primes.size());
    for (int i=0; i<primes.size(); i++) {
        answerVec[i] = remainders[i];
        for (int j = 0; j < i; j++) {
            answerVec[i] = reciprocals[j][i] * (answerVec[i] - answerVec[j]);
            answerVec[i] %= primes[i];
        }
        result += curMultiplier * answerVec[i];
        curMultiplier *= primes[i];
    }
    return result;
}

BigInteger &BigInteger::operator%=(const BigInteger &rhs) {
    return *this = *this % rhs;
}

void fft(std::vector<fft_base> &arg, bool invert) {
    int n = arg.size();
    if (n == 1) {
        return;
    }
    std::vector<fft_base> a0(n / 2), a1(n / 2);
    for (int i = 0, j = 0; i < n; i += 2, j++) {
        a0[j] = arg[i];
        a1[j] = arg[i+1];
    }
    fft(a0, invert);
    fft(a1, invert);

    double angle = 2 * M_PI / n * (invert ? -1 : 1);
    fft_base w(1), wn(cos(angle), sin(angle));
    for (int i = 0; i < n / 2; i++) {
        arg[i] = a0[i] + w * a1[i];
        arg[i + n / 2] = a0[i] -  w * a1[i];
        if (invert) {
            arg[i] /= 2;
            arg[i + n / 2] /= 2;
        }
        w *= wn;
    }
}