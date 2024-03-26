#include "BigInteger.h"
#include <iostream>
#include <cassert>

void testConstruction() {
    BigInteger zero;
    assert(zero.to_string() == "0");

    BigInteger fromString("-123456789");
    assert(fromString.to_string() == "-123456789");

    BigInteger fromLong(123456789);
    assert(fromLong.to_string() == "123456789");

    BigInteger copyConstructed(fromLong);
    assert(copyConstructed.to_string() == "123456789");
}

void testArithmeticOperations() {
    BigInteger a("12345");
    BigInteger b("67890");
    BigInteger sum = a + b;
    assert(sum.to_string() == "80235");

    BigInteger diff = a - b;
    assert(diff.to_string() == "-55545");

    BigInteger prod = a * b;
    assert(prod.to_string() == "838102050");

    BigInteger zero;
    BigInteger zeroProd = a * zero;
    assert(zeroProd.to_string() == "0");
}

void testComparisons() {
    BigInteger a("12345");
    BigInteger b("67890");
    assert(a < b);
    assert(b > a);
    assert(a <= b);
    assert(b >= a);
    assert(a != b);

    BigInteger c("12345");
    assert(a == c);
}

void testEdgeCases() {
    BigInteger large("99999999999999999999999999999999999999");
    BigInteger one("1");
    BigInteger largePlusOne = large + one;
    assert(largePlusOne.to_string() == "100000000000000000000000000000000000000");

    BigInteger negative("-12345");
    BigInteger negativePlusA = negative + a;
    assert(negativePlusA.to_string() == "0");
}

void testToString() {
    BigInteger a("-123456789");
    assert(a.to_string() == "-123456789");

    BigInteger zero;
    assert(zero.to_string() == "0");
}

int main() {
    testConstruction();
    testArithmeticOperations();
    testComparisons();
    testEdgeCases();
    testToString();

    std::cout << "All BigInteger tests passed successfully.\n";
    return 0;
}
