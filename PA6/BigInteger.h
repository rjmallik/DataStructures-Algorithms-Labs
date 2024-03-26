#include "BigInteger.h"
#include "List.h"
#include <stdexcept> // For std::invalid_argument
#include <algorithm> // For std::reverse
#include <iostream>
#include <iomanip> // for std::setw and std::setfill
using namespace std;

// Global constants
const long base = 1000000000; // 1 billion
const int power = 9; // Base = 10^power

// Helper functions (implementations will follow after BigInteger member functions)
void negateList(List& L);
void sumList(List& S, List A, List B, int sgn);
int normalizeList(List& L);
void shiftList(List& L, int p);
void scalarMultList(List& L, ListElement m);

// BigInteger Constructors & Destructors
BigInteger::BigInteger() : signum(0), digits() {}

BigInteger::BigInteger(long x) : signum(x >= 0 ? 1 : -1), digits() {
    if (x == 0) return;
    while (x != 0) {
        digits.insertBefore(x % base);
        x /= base;
    }
}

BigInteger::BigInteger(std::string s) : signum(1), digits() {
    int start = 0;
    if (!s.empty() && s[0] == '-') {
        signum = -1;
        start = 1;
    }
    for (std::string::size_type i = start; i < s.length(); i++) {
        if (!isdigit(s[i]) && s[i]!= '-' && s[i]!= '+') throw std::invalid_argument("Invalid input string");
        digits.insertAfter(s[i] - '0'); // Use insertAfter() to add digits to the end
    }
}


BigInteger::BigInteger(const BigInteger& N) : signum(N.signum), digits(N.digits) {}

// Access functions
int BigInteger::sign() const {
    return signum;
}

int BigInteger::compare(const BigInteger& N) const {
    if (digits.isEmpty() && N.digits.isEmpty()) return 0;
    if (digits.isEmpty()) return -1;
    if (N.digits.isEmpty()) return 1;
	
    if (digits.length() != N.digits.length()) {
        return digits.length() > N.digits.length() ? 1 : -1;
    }

    // Compare digit by digit
    List::Node* currentThis = digits.frontDummy->next; // Start from the least significant digit
    List::Node* currentN = N.digits.frontDummy->next;

    while (currentThis != digits.backDummy && currentN != N.digits.backDummy) {
        if (currentThis->data != currentN->data) {
            return currentThis->data > currentN->data ? 1 : -1;
        }
        currentThis = currentThis->next; // Move to the next digit
        currentN = currentN->next;
    }

    return 0; // If all digits are equal
}



void BigInteger::makeZero() {
    signum = 0;
    digits.clear();
}

void BigInteger::negate() {
    if (signum !=0) {
    signum *=-1;
    }
}


BigInteger BigInteger::add(const BigInteger& N) const {
    BigInteger result;
    
    int carry = 0;
    List A = digits;
    List B = N.digits;
    A.moveBack();
    B.moveBack();
    //int base = 10;

    while (!A.isEmpty() || !B.isEmpty()) {
        int sum = (A.isEmpty() ? 0 : A.peekPrev()) + (B.isEmpty() ? 0 : B.peekPrev()) + carry;
        result.digits.insertAfter(sum % base);
        carry = sum / base;
        if (!A.isEmpty()) A.eraseBefore(); // Move the cursor in list A to the previous element
        if (!B.isEmpty()) B.eraseBefore(); // Move the cursor in list B to the previous element
    }
    if (carry != 0) result.digits.insertBefore(carry);
    return result;
}


BigInteger BigInteger::sub(const BigInteger& N) const {
    BigInteger result;
    //std::cout << "DIGITS  :\n " << digits << std::endl;
    //std::cout << "N DIGITS  :\n " << N.digits << std::endl;
    if(this->signum < 0 && N.signum >0){
        int signB = 1;
        sumList(result.digits, this->digits, N.digits, signB);
        normalizeList(result.digits);
        result.signum = -1;
        return result;
    }
      if(this->signum < 0 && N.signum <0){
        int signB = -1;
        sumList(result.digits, this->digits, N.digits, signB);
        //normalizeList(result.digits);
        result.signum = normalizeList(result.digits);
        return result;
    }
    int signB = -N.sign(); // Negate the sign of B for subtraction
    sumList(result.digits, this->digits, N.digits, signB); // Use the negated sign of B
    //std::cout << "before NORMALIZE result (S): " << result << std::endl;
    result.signum = normalizeList(result.digits);
    //std::cout << "SIGNUM  :\n " << result.signum << std::endl;
    //std::cout << "Final result (S): " << result << std::endl;
    return result;
}


BigInteger BigInteger::mult(const BigInteger& N) const {
    BigInteger result;
    const List& A = digits;
    const List& B = N.digits;
    int base = 10;

    for (int i = 0; i < A.length(); ++i) {
        int carry = 0;
        BigInteger temp;
        List::Node* currentNode = A.frontDummy->next; // Start from the least significant digit
        for (int k = 0; k < i; ++k) {
            temp.digits.insertBefore(0); // Insert trailing zeros according to the current position
            currentNode = currentNode->next;
        }
        List::Node* currentB = B.frontDummy->next;
        while (currentB != B.backDummy) {
            int product = currentNode->data * currentB->data + carry;
            temp.digits.insertBefore(product % base);
            carry = product / base;
            currentB = currentB->next;
        }
        if (carry != 0) temp.digits.insertBefore(carry);
        result = result.add(temp);
    }
    return result;
}



std::string BigInteger::to_string() {
    std::stringstream ss;
    List temp(digits); // Make a copy of digits

    temp.moveFront(); // Move the cursor to the front of the list

    while (temp.position() != temp.length()) { // Iterate until the cursor reaches the end
        ss << temp.peekNext(); // Peek at the next element without moving the cursor
        temp.moveNext(); // Move the cursor to the next element
    }
    
    std::string result = ss.str();
    if (signum == -1 && result != "0") {
        result = "-" + result; // Add negative sign if necessary
    }
    
    return result;
}


// Helper function implementations
void negateList(List& L) {
    for (L.moveFront(); L.position() < L.length(); L.moveNext()) {
        // Retrieve the current node's data and negate it
        L.beforeCursor->data = -L.beforeCursor->data;
    }
}

void sumList(List& S, List A, List B, int sgn) {
    A.moveFront();
    B.moveFront();
    S.clear();
    long sum = 0;
    while (A.position() < A.length() || B.position() < B.length()) {
        if (A.position() < A.length()) sum += A.peekNext(); // Accessing data from list A
        if (B.position() < B.length()) sum += sgn * B.peekNext(); // Accessing data from list B
        S.insertBefore(sum % base);
        sum /= base;
        A.moveNext();
        B.moveNext();
    }
    if (sum != 0) S.insertBefore(sum);
}

int normalizeList(List& L) {
    long carry = 0;
    for (L.moveFront(); L.position() < L.length(); L.moveNext()) {
        long temp = L.peekNext() + carry; // Accessing data directly using peekNext()
        L.afterCursor->data = temp % base; // Setting data directly
        carry = temp / base;
    }
    if (carry > 0) L.insertAfter(carry); // Inserting after the cursor
    // Determine the sign
    L.moveBack();
    return L.isEmpty() ? 0 : (L.afterCursor->data < 0 ? -1 : 1); // Accessing data directly
}

void shiftList(List& L, int p) {
    for (int i = 0; i < p; i++) {
        L.insertAfter(0);
    }
}

void scalarMultList(List& L, ListElement m) {
    long carry = 0;
    for (L.moveFront(); L.position() < L.length(); L.moveNext()) {
        long temp = m * L.peekNext() + carry; // Accessing data using peekNext()
        L.insertBefore(temp % base); // Inserting the result before the cursor
        carry = temp / base;
        L.eraseBefore(); // Erase the previous cursor position
    }
    if (carry > 0) L.insertBefore(carry); // Inserting carry at the beginning if needed
}


// Overloading the arithmetic operators based on BigInteger arithmetic operations

bool operator==(const BigInteger& A, const BigInteger& B) {
    return A.compare(B) == 0;
}

bool operator<(const BigInteger& A, const BigInteger& B) {
    return A.compare(B) < 0;
}

bool operator<=(const BigInteger& A, const BigInteger& B) {
    return A.compare(B) <= 0;
}

bool operator>(const BigInteger& A, const BigInteger& B) {
    return A.compare(B) > 0;
}

bool operator>=(const BigInteger& A, const BigInteger& B) {
    return A.compare(B) >= 0;
}

BigInteger operator+(const BigInteger& A, const BigInteger& B) {
    return A.add(B);
}

BigInteger operator+=(BigInteger& A, const BigInteger& B) {
    A = A.add(B);
    return A;
}

BigInteger operator-(const BigInteger& A, const BigInteger& B) {
    return A.sub(B);
}

BigInteger operator-=(BigInteger& A, const BigInteger& B) {
    A = A.sub(B);
    return A;
}

BigInteger operator*(const BigInteger& A, const BigInteger& B) {
    return A.mult(B);
}

BigInteger operator*=(BigInteger& A, const BigInteger& B) {
    A = A.mult(B);
    return A;
}

// Stream insertion operator for output
std::ostream& operator<<(std::ostream& stream, BigInteger N) {
    return stream << N.BigInteger::to_string();
}