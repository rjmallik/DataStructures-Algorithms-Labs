#include "BigInteger.h"

#define power 9
#define base  1000000000

// #define power 1
// #define base  10

//* Class Constructors & Destructors ---------------------------------------
// BigInteger()
// Constructor that creates a new BigInteger in the zero state:
// signum=0, digits=().
BigInteger::BigInteger() {
    signum = 0;
    digits = List();
}

// BigInteger()
// Constructor that creates a new BigInteger from the long value x.
BigInteger::BigInteger(long x) {
    if (x < 0) {
        signum = -1;
        x = -x;
    } else if (x > 0) {
        signum = 1;
    } else {
        signum = 0;
        digits.insertBefore(0);
        return;
    }

    // Extract digits from the positive value of x and insert them into digits vector
    while (x > 0) {
        digits.insertBefore(x % base);
        x /= base;
    }
}

// BigInteger()
// Constructor that creates a new BigInteger from the string s.
// Pre: s is a non-empty string consisting of (at least one) base 10 digit
// {0,1,2,3,4,5,6,7,8,9}, and an optional sign {+,-} prefix.
BigInteger::BigInteger(std::string s) {
    if (s.length() == 0) {
        throw std::invalid_argument("BigInteger: Constructor: empty string");
    }
    digits.clear();
    signum = 0;

    // If we have an optional sign of -
    if (s[0] == '-') {
        signum = -1;
        s = s.substr(1);
    }
    // If we have an optional sign of +
    else if (s[0] == '+') {
        signum = 1;
        s = s.substr(1);
    }
    // We assume sign is positive, but its not there
    else {
        signum = 1;
    }
    // Move 0's on left away, dont matter
    while (s[0] == '0') {
        s = s.substr(1);
    }
    if (s.length() == 0) {
        throw std::invalid_argument("BigInteger: Constructor: non-numeric string");
    }
    long unsigned int i = 0;
    while (i < s.length()) {
        int loop;
        long num = 0;
        if (digits.length() == 0) {
            if (s.length() % power != 0) {
                loop = s.length() % power;
            } else {
                loop = power;
            }
        } else {
            loop = power;
        }
        for (int j = 0; j < loop; j++) {
            if (i < 0)
                break;
            if (!std::isdigit(s[i])) {
                throw std::invalid_argument("BigInteger: Constructor: non-numeric string");
            }
            num = num * 10 + (s[i] - '0');
            i++;
        }
        digits.insertBefore(num);
    }
}

// BigInteger()
// Constructor that creates a copy of N.
BigInteger::BigInteger(const BigInteger &N) {
    signum = N.signum;
    digits = N.digits;
}

//* Access functions --------------------------------------------------------

// sign()
// Returns -1, 1 or 0 according to whether this BigInteger is positive,
// negative or 0, respectively.
int BigInteger::sign() const {
    return signum;
}

// compare()
// Returns -1, 1 or 0 according to whether this BigInteger is less than N,
// greater than N or equal to N, respectively.
int BigInteger::compare(const BigInteger &N) const {
    List A = this->digits;
    List B = N.digits;
    A.moveFront();
    B.moveFront();

    if (signum < N.signum) {
        return -1;
    } else if (signum > N.signum) {
        return 1;
    } else if (this->signum == 0) {
        return 0;
    }
    // This has more digits than N
    else if (N.digits.length() > this->digits.length()) {
        if (this->signum == 1) {
            return -1;
        } else {
            return 1;
        }
        return 0;
    }
    // N has more digits than this
    else if (N.digits.length() < this->digits.length()) {
        if (this->signum == 1) {
            return 1;
        } else {
            return -1;
        }
    }

    // move past 0's
    while (A.position() != A.length()) {
        if (A.peekNext() == 0)
            A.eraseAfter();
        else {
            break;
        }
    }
    while (B.position() != B.length()) {
        if (B.peekNext() == 0)
            B.eraseAfter();
        else {
            break;
        }
    }
    while (A.position() < A.length()) {
        if (A.peekNext() > B.peekNext()) {
            if (this->signum == -1) {
                return -1;
            } else {
                return 1;
            }
        } else if (A.peekNext() < B.peekNext()) {
            if (this->signum == -1) {
                return 1;
            } else {
                return -1;
            }
        }
        A.moveNext();
        B.moveNext();
    }
    return 0;
}

//* Manipulation procedures -------------------------------------------------

// makeZero()
// Re-sets this BigInteger to the zero state.
void BigInteger::makeZero() {
    signum = 0;
    digits.clear();
}

// negate()
// If this BigInteger is zero, does nothing, otherwise reverses the sign of
// this BigInteger positive <--> negative.
void BigInteger::negate() {
    if (signum == 0) {
        return;
    }
    signum = -signum;
}

//* Helper Functions ------------------------------------------------------------
// negateList()
// Changes the sign of each integer in List L. Used by sub().
void negateList(List &L) {
    L.moveFront();
    while (L.position() < L.length()) {
        long num = L.peekNext();
        L.eraseAfter();
        L.insertAfter(-num);
        L.moveNext();
    }
}

// sumList()
// Overwrites the state of S with A + sgn*B (considered as vectors).
// Used by both sum() and sub().
void sumList(List &S, List A, List B, int sgn) {
    S.clear();
    A.moveBack();
    B.moveBack();
    while (A.position() != 0 && B.position() != 0) {
        S.insertAfter(A.peekPrev() + sgn * B.peekPrev());
        A.movePrev();
        B.movePrev();
    }
    while (A.position() != 0) {
        S.insertAfter(A.peekPrev());
        A.movePrev();
    }
    while (B.position() != 0) {
        S.insertAfter(sgn * B.peekPrev());
        B.movePrev();
    }
}
// normalizeList()
// Performs carries from right to left (least to most significant
// digits), then returns the sign of the resulting integer. Used
// by add(), sub() and mult().
int normalizeList(List &L) {
    int sgn = 1;
    long carry = 0;
    L.moveBack();
    while (L.position() != 0) {
        long num = L.peekPrev() + carry;
        carry = 0;
        if (num < 0) {
            if (L.position() != 1) {
                num += base;
            }
            carry = -1;
        } else if (num >= base) {
            carry = num / base;
            num %= base;
        }
        L.eraseBefore();
        L.insertBefore(num);
        L.movePrev();
    }
    if (carry > 0) {
        sgn = 1;
        L.insertBefore(carry);
    }
    if (L.front() < 0) {
        negateList(L);
        sgn = -1;
        normalizeList(L);
        return sgn;
    }
    for (L.moveFront(); L.position() != L.length(); L.moveNext()) {
        if (L.peekNext() != 0) {
            sgn = 1;
            break;
        }
        sgn = 0;
    }
    return sgn;
}

// shiftList()
// Prepends p zero digits to L, multiplying L by base^p. Used by mult().
void shiftList(List &L, int p) {
    L.moveFront();
    int i = 0;
    while (i < p) {
        L.insertBefore(0);
        i++;
    }
}

// scalarMultList()
// Multiplies L (considered as a vector) by m. Used by mult().
void scalarMultList(List &L, ListElement m) {
    L.moveFront();
    while (L.position() < L.length()) {
        long num = L.peekNext();
        L.eraseAfter();
        L.insertAfter(num * m);
        L.moveNext();
    }
}

//* BigInteger Arithmetic operations ----------------------------------------
// add()
// Returns a BigInteger representing the sum of this and N.
BigInteger BigInteger::add(const BigInteger &N) const {
    BigInteger A = *this;
    BigInteger B = N;
    List S;
    BigInteger C;
    if (A.signum == -1 && B.signum == -1) {
        sumList(S, A.digits, B.digits, 1);
        normalizeList(S);
        C.digits = S;
        C.signum = -1;
        return C;
    } else if (A.signum == 1 && B.signum == 1) {
        sumList(S, A.digits, B.digits, 1);
        normalizeList(S);
        C.digits = S;
        C.signum = 1;
        return C;
    } else if (A.signum == -1 && B.signum == 1) {
        C = A;
        C.negate();
        int b = B.compare(C);
        if (b == -1) {
            sumList(S, B.digits, A.digits, -1);
            normalizeList(S);
            C.digits = S;
            C.signum = -1;
            return C;
        } else if (b == 1) {
            sumList(S, B.digits, A.digits, -1);
            normalizeList(S);
            C.digits = S;
            C.signum = 1;
            return C;
        } else {
            C.makeZero();
            return C;
        }
    } else if (A.signum == 1 && B.signum == -1) {
        C = B;
        C.negate();
        int b = A.compare(C);
        if (b == -1) {
            sumList(S, A.digits, C.digits, -1);
            normalizeList(S);
            C.digits = S;
            C.signum = -1;
            return C;
        } else if (b == 1) {
            sumList(S, A.digits, C.digits, -1);
            normalizeList(S);
            C.digits = S;
            C.signum = 1;
            return C;
        } else {
            C.makeZero();
            return C;
        }
    } else {
        sumList(S, A.digits, B.digits, 1);
        normalizeList(S);
        C.digits = S;
        C.signum = 1;
        return C;
    }
}
// sub()
// Returns a BigInteger representing the difference of this and N.
BigInteger BigInteger::sub(const BigInteger &N) const {
    BigInteger A = *this;
    BigInteger B = N;
    List S;
    BigInteger C;
    if (A.digits == B.digits) {
        return BigInteger();
    }
    // Both signs are positive
    if (A.signum == 1 && B.signum == 1) {
        int b = A.compare(B);
        // If B is bigger than A
        if (b == -1) {
            sumList(S, B.digits, A.digits, -1);
            normalizeList(S);
            C.digits = S;
            C.signum = -1;
            return C;
        }
        // If A is bigger than B
        else if (b == 1) {
            sumList(S, A.digits, B.digits, -1);
            normalizeList(S);
            C.digits = S;
            C.signum = 1;
            return C;
        } else {
            C.makeZero();
            return C;
        }
    }
    // B is positive and A is negative
    else if (B.signum == 1 && A.signum == -1) {
        sumList(S, A.digits, B.digits, 1);
        normalizeList(S);
        C.digits = S;
        C.signum = -1;
        return C;
    }

    // A is positive and B is negative
    else if (A.signum == 1 && B.signum == -1) {
        sumList(S, A.digits, B.digits, 1);
        normalizeList(S);
        C.digits = S;
        C.signum = 1;
        return C;
    }
    // If A is negative and B is negative
    else if (A.signum == -1 && B.signum == -1) {
        int b = A.compare(B);
        // B is bigger than A
        if (b == -1) {
            sumList(S, B.digits, A.digits, -1);
            normalizeList(S);
            C.digits = S;
            C.signum = -1;
            return C;
            // A is bigger than B
        } else if (b == 1) {
            sumList(S, B.digits, A.digits, -1);
            normalizeList(S);
            C.digits = S;
            C.signum = 1;
            return C;
        } else {
            C.makeZero();
            return C;
        }
    }
    sumList(S, A.digits, B.digits, -1);
    normalizeList(S);
    C.digits = S;
    C.signum = -1;
    return C;
}

// mult()
// Returns a BigInteger representing the product of this and N.
BigInteger BigInteger::mult(const BigInteger &N) const {
    BigInteger A = *this;
    BigInteger B = N;
    List S;
    // If either of the lengths are 0
    if (B.digits.length() == 0 || A.digits.length() == 0) {
        return BigInteger();
    }
    B.digits.moveBack();
    // While we can still multiply
    while (B.digits.position() != 0) {
        List C = A.digits;
        shiftList(C, B.digits.position() - 1);
        scalarMultList(C, B.digits.peekPrev());
        C.moveBack();
        for (int i = 0; i < B.digits.length() - B.digits.position(); i++) {
            C.insertAfter(0);
        }
        sumList(S, S, C, 1);
        normalizeList(S);
        B.digits.movePrev();
    }
    BigInteger K;
    K.digits = S;
    // Get the proper sign
    if (A.signum == 1 && B.signum == 1) {
        K.signum = 1;
    } else if (A.signum == -1 && B.signum == -1) {
        K.signum = 1;
    } else {
        K.signum = -1;
    }
    return K;
}

//* Other Functions ---------------------------------------------------------

// to_string()
// Returns a string representation of this BigInteger consisting of its
// base 10 digits. If this BigInteger is negative, the returned string
// will begin with a negative sign '-'. If this BigInteger is zero, the
// returned string will consist of the character '0' only.
std::string BigInteger::to_string() {
    if (digits.length() == 0) {
        return "0";
    }
    std::string s = "";
    if (signum == -1) {
        s += "-";
    }
    digits.moveFront();
    // skip 0's
    while (digits.position() != digits.length()) {
        if (digits.peekNext() != 0) {
            break;
        }
        digits.moveNext();
    }
    // add 0
    if (digits.position() == digits.length()) {
        s += "0";
        return s;
    }
    bool expanded = false;
    while (digits.position() != digits.length()) {
        std::string ns = std::to_string(digits.peekNext());
        if (expanded) {
            long unsigned int i = 0;
            while (i < power - ns.length()) {
                s += "0";
                i++;
            }
        }
        expanded = true;
        s += ns;
        digits.moveNext();
    }
    return s;
}

//* Overriden Operators -----------------------------------------------------
// operator<<()
// Inserts string representation of N into stream.
std::ostream &operator<<(std::ostream &stream, BigInteger N) {
    return stream << N.to_string();
}

// operator==()
// Returns true if and only if A equals B.
bool operator==(const BigInteger &A, const BigInteger &B) {
    return A.compare(B) == 0;
}

// operator<()
// Returns true if and only if A is less than B.
bool operator<(const BigInteger &A, const BigInteger &B) {
    return A.compare(B) == -1;
}

// operator<=()
// Returns true if and only if A is less than or equal to B.
bool operator<=(const BigInteger &A, const BigInteger &B) {
    return A < B || A == B;
}

// operator>()
// Returns true if and only if A is greater than B.
bool operator>(const BigInteger &A, const BigInteger &B) {
    return !(A <= B);
}

// operator>=()
// Returns true if and only if A is greater than or equal to B.
bool operator>=(const BigInteger &A, const BigInteger &B) {
    return !(A < B);
}

// operator+()
// Returns the sum A+B.
BigInteger operator+(const BigInteger &A, const BigInteger &B) {
    return A.add(B);
}

// operator+=()
// Overwrites A with the sum A+B.
BigInteger operator+=(BigInteger &A, const BigInteger &B) {
    A = A.add(B);
    return A;
}

// operator-()
// Returns the difference A-B.
BigInteger operator-(const BigInteger &A, const BigInteger &B) {
    return A.sub(B);
}

// operator-=()
// Overwrites A with the difference A-B.
BigInteger operator-=(BigInteger &A, const BigInteger &B) {
    BigInteger C = A.sub(B);
    A = C;
    return A;
}

// operator*()
// Returns the product A*B.
BigInteger operator*(const BigInteger &A, const BigInteger &B) {
    return A.mult(B);
}

// operator*=()
// Overwrites A with the product A*B.
BigInteger operator*=(BigInteger &A, const BigInteger &B) {
    A = A.mult(B);
    return A;
}