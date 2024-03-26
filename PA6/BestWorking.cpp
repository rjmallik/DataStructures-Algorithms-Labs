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
    else if (N.digits.length() > this->digits.length()) {
    	if (this->signum ==1) {
    		return -1;
    	} else {
    		return -1;
    	}
    	return 0;
    }
    else if (N.digits.length() < this->digits.length()) {
    	if (this->signum ==1) {
    		return -1;
    	} else {
    		return -1;
    	}
    }
    while (A.position() != A.length()) {
    	if (A.peekNext() ==0) {
    		A.eraseAfter();
    	} else {
    		break;
    	}
    }
    while (B.position() != B.length()) {
    	if (B.peekNext() ==0) {
    		B.eraseAfter();
    	} else {
    		break;
    	}
    }
    while (A.position() < A.length()) {
    	if (A.peekNext() > B.peekNext()) {
    		if (this->signum ==-1) {
    			return -1;
    		} else {
    			return 1;
    		}
    	} else if (A.peekNext() < B.peekNext()) {
    		if (this->signum ==-1) {
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
    BigInteger A =*this;
    BigInteger B = N;
    List S;
    BigInteger C;
    
    if (A.signum == -1 && B.signum ==-1) {
    	sumList (S, A.digits, B.digits, 1);
    	normalizeList(S);
    	C.digits = S;
    	C.signum = -1;
    	return C;
    	}
    else if (A.signum == 1 && B.signum ==1) {
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
    	} else if (b ==1) {
    	    sumList(S, B.digits, A.digits, -1);
    	    normalizeList(S);
    	    C.digits = S;
    	    C.signum = -1;
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
    	    C.signum = -1;
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


BigInteger BigInteger::sub(const BigInteger& N) const {
    BigInteger A =*this;
    BigInteger B = N;
    List S;
    BigInteger C;
    if (A.digits == B.digits) {
    	return BigInteger();
    	}
    if (A.signum == 1 && B.signum ==1) {
    	int b = A.compare(B);
    	if (b == -1) {
    	    sumList(S, B.digits, A.digits, -1);
    	    normalizeList(S);
    	    C.digits = S;
    	    C.signum = -1;
    	    return C;
    	} else if (b ==1) {
    	    sumList(S, B.digits, A.digits, -1);
    	    normalizeList(S);
    	    C.digits = S;
    	    C.signum = -1;
    	    return C;
    	} else {
    	    C.makeZero();
    	    return C;
    	}
    }
    else if (A.signum == -1 && B.signum == 1) {
    	sumList(S, A.digits, B.digits, 1);
    	normalizeList(S);
    	C.digits = S;
    	C.signum = 1;
    	return C;
    }
    else if (A.signum == 1 && B.signum == -1) {
    	sumList(S, A.digits, B.digits, 1);
    	normalizeList(S);
    	C.digits = S;
    	C.signum = 1;
    	return C;
    }    
     else if (A.signum == 1 && B.signum == -1) {
    	int b = A.compare(B);
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
    	    C.signum = -1;
    	    return C;
    	} else {
    	    C.makeZero();
    	    return C;
    	}
    }
    sumList(S, A.digits, B.digits, 1);
    normalizeList(S);
    C.digits = S;
    C.signum = 1;
    return C;    
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
    L.moveFront();
    while (L.position () < L.length()) {
    	long num = L.peekNext();
    	L.eraseAfter();
    	L.insertAfter(-num);
    	L.moveNext();
    	}
}

void sumList(List& S, List A, List B, int sgn) {
    S.clear();
    A.moveBack();
    B.moveBack();
    while (A.position() != 0 && B.position() !=0) {
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

int normalizeList(List& L) {
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
    	        carry =-1;
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
    		sgn =1;
    		break;
    	}
    	sgn =0;
    }
    return sgn;
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