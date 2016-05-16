//////////////////////////////////////////////////
//         Written by Michael Hennessy          //
//  For Dr. David Adams's Computing III Class   //
//          UML Spring Semester 2015            //
//////////////////////////////////////////////////

#ifndef BIGINT_H
#define BIGINT_H

#include <cstdio> // for sprintf (maintains compatibility with g++)
#include <cstdlib> // for std::exit
#include <iostream>
#include <string>

// this BS maintains most functionality;
// weird edge-case behavior at -(2^32) numbers,
// but you shouldn't be inlining huge (tiny) number initialization anyway
std::string cust_to_string( int n );
std::string cust_to_string( long n );
std::string cust_to_string( long long n );
std::string cust_to_string( unsigned n );
std::string cust_to_string( unsigned long n );
std::string cust_to_string( unsigned long long n );

using namespace std;

struct DivData
{
	int divisions;
	int remainder;
};

class BigInt
{
public:
	BigInt();
	BigInt( int x );
	explicit BigInt( string x );

	friend ostream& operator<<( ostream& out, const BigInt& right );
	friend bool operator<( const BigInt& left, const BigInt& right );
	friend bool operator>( const BigInt& left, const BigInt& right );
	friend bool operator==( const BigInt& left, const BigInt& right );
	BigInt operator+( const BigInt& right );
	BigInt operator-( const BigInt& right );
	BigInt operator*( const BigInt& right );
	BigInt operator/( const BigInt& right );

private:
	string data;
	bool isNegative;

	void makeLengthsEqual( string& a, string& b );
	void removeLeadingZeroes( string& str );

	string sum( string left, string right );
	string difference( string left, string right );

	string singleDigitProduct( char left, char right ) { return cust_to_string( (left - '0') * (right - '0') ); }

	int charToInt( char c ) { return c - '0'; }
	char intToChar( int n ) { return n + '0'; }

	int numberOfDivisions( const string& numerator, const string& denominator );
};

#endif