//////////////////////////////////////////////////
//		   Written by Michael Hennessy			//
//	For Dr. David Adams's Computing III Class	//
//			UML Spring Semester 2015			//
//////////////////////////////////////////////////

#include "BigInt.h"

std::string cust_to_string( int n )
{
	char tmp[20];
	sprintf( tmp, "%d", n );
	return std::string( tmp );
}
std::string cust_to_string( long n )
{
	char tmp[20];
	sprintf( tmp, "%ld", n );
	return std::string( tmp );
}
std::string cust_to_string( long long n )
{
	char tmp[20];
	sprintf( tmp, "%lld", n );
	return std::string( tmp );
}
std::string cust_to_string( unsigned n )
{
	char tmp[20];
	sprintf( tmp, "%u", n );
	return std::string( tmp );
}
std::string cust_to_string( unsigned long n )
{
	char tmp[20];
	sprintf( tmp, "%lu", n );
	return std::string( tmp );
}
std::string cust_to_string( unsigned long long n )
{
	char tmp[20];
	sprintf( tmp, "%llu", n );
	return std::string( tmp );
}

BigInt::BigInt()
{
	data.push_back( '0' );

	isNegative = false;
}

BigInt::BigInt( int x )
{
	if ( x < 0 )
	{
		x = -x;
		isNegative = true;
	}
	else
		isNegative = false;

	data = cust_to_string( x );
}

BigInt::BigInt( string x )
{
	if ( x.size() < 1 )
	{
		cerr << "Tried to make a BigInt with an empty string, exiting...\n";
		exit( 1 );
	}

	if ( x.size() == 1 && isdigit( x[0] ) )
	{
		isNegative = false;
		data = x;
		return;
	}

	unsigned index = 0, firstValidIndex = 0, lastValidIndex = 0;

	// find the position of the first char that is a number, -, or +
	while ( !isdigit( x[index] ) && x[index] != '-' && x[index] != '+' )
	{
		index++;
		if ( index == x.size() - 1 )
		{
			cout << "Tried to construct BigInt from invalid string:\n" << x << "\nExiting...\n";
			exit( 1 );
		}
	}
	firstValidIndex = index++; // caution here, I know what I'm doing

	// if char after initial valid char is not a digit, exit
	if ( !isdigit( x[firstValidIndex + 1] ) )
	{
		cout << "Tried to construct BigInt from invalid string:\n" << x << "\nExiting...\n";
		exit( 1 );
	}

	// initialize isNegative according to first valid char ( it will either be minus (neg), or a number or plus (pos) )
	isNegative = x[firstValidIndex] == '-';

	// find the index of the last digit/last valid char
	while ( isdigit( x[index] ) )
		index++;
	lastValidIndex = index;

	// prune string so data is only the number, since isNegative is a separate variable
	if ( !isdigit( x[firstValidIndex] ) )
		x = x.substr( firstValidIndex + 1, lastValidIndex - firstValidIndex - 1 );
	else
		x = x.substr( firstValidIndex, lastValidIndex - firstValidIndex );

	// account for leading 0s
	if ( x.size() > 0 && x[0] == '0' )
		removeLeadingZeroes( x );

	data = x;
}

ostream& operator<<( ostream& out, const BigInt& right )
{
	if ( right.isNegative )
		out << "-" << right.data;
	else
		out << right.data;
	return out;
}

bool operator<( const BigInt& left, const BigInt& right )
{
	if ( left.isNegative )
	{
		if ( right.isNegative ) // left and right are negative
		{
			if ( left.data.size() == right.data.size() )
				return left.data > right.data;
			else
				return left.data.size() > right.data.size();
		}
		else // left is negative, right is positive
		{
			return true;
		}
	}
	else
	{
		if ( right.isNegative ) // left is positive, right is negative
		{
			return false;
		}
		else // left and right are positive
		{
			if ( left.data.size() == right.data.size() )
				return left.data < right.data;
			else
				return left.data.size() < right.data.size();
		}
	}
}

bool operator>( const BigInt& left, const BigInt& right )
{
	if ( left.isNegative )
	{
		if ( right.isNegative ) // left and right are negative
		{
			if ( left.data.size() == right.data.size() )
				return left.data < right.data;
			else
				return left.data.size() < right.data.size();
		}
		else // left is negative, right is positive
		{
			return false;
		}
	}
	else
	{
		if ( right.isNegative ) // left is positive, right is negative
		{
			return true;
		}
		else // left and right are positive
		{
			if ( left.data.size() == right.data.size() )
				return left.data > right.data;
			else
				return left.data.size() > right.data.size();
		}
	}
}

bool operator==( const BigInt& left, const BigInt& right )
{
	return left.data == right.data && left.isNegative == right.isNegative;
}

BigInt BigInt::operator+( const BigInt& right )
{
	BigInt result;

	if ( isNegative )
	{
		if ( right.isNegative ) // neg + neg
		{
			result = BigInt( sum( data, right.data ) );
			result.isNegative = true;
			return result;
		}
		else // neg + pos
		{
			result = BigInt( difference( data, right.data ) );
			result.isNegative = BigInt( right.data ) < BigInt( data );
		}
	}
	else
	{
		if ( right.isNegative ) // pos + neg
		{
			result = BigInt( difference( data, right.data ) );
			result.isNegative = BigInt( data ) < BigInt( right.data );
		}
		else // pos + pos
		{
			result = BigInt( sum( data, right.data ) );
		}
	}

	return result;
}

BigInt BigInt::operator-( const BigInt& right )
{
	BigInt result;

	if ( isNegative )
	{
		if ( right.isNegative ) // neg - neg
		{
			result = BigInt( difference( data, right.data ) );
			result.isNegative = BigInt( right.data ) < BigInt( data );
		}
		else // neg - pos
		{
			result = BigInt( sum( data, right.data ) );
			result.isNegative = true;
		}
	}
	else
	{
		if ( right.isNegative ) // pos - neg
		{
			result = BigInt( sum( data, right.data ) );
		}
		else // pos - pos
		{
			result = BigInt( difference( data, right.data ) );
			result.isNegative = BigInt( data ) < BigInt( right.data );
		}
	}

	return result;
}

BigInt BigInt::operator*( const BigInt& right )
{
	if ( data == "0" || right.data == "0" )
		return BigInt( "0" );

	BigInt result;
	string tempLeft = data;
	string tempRight = right.data;
	makeLengthsEqual( tempLeft, tempRight );

	int length = tempLeft.size();

	for ( int rightIndex = length - 1; rightIndex >= 0; rightIndex-- )
	{
		string curLine;
		int carry = 0;

		for ( int i = 0; i < length - 1 - rightIndex; i++ ) // add zeroes to end of subsequent lines after first
			curLine.insert( curLine.begin(), '0' );

		for ( int leftIndex = length - 1; leftIndex >= 0; leftIndex-- )
		{
			string curProduct = singleDigitProduct( tempLeft[leftIndex], tempRight[rightIndex] );
			char &lDig = curProduct[0], &rDig = curProduct[1]; // references to left and right digit of current product

			if ( curProduct.size() == 1 ) // add 0 as left digit of curProduct if it is only 1 digit in size
				curProduct.insert( curProduct.begin(), '0' );

			rDig += carry;
			if ( rDig > '9' ) // add excess value from right to left, subtract from right
			{
				int remainder = rDig % '9' - 1;
				rDig = intToChar( remainder );
				lDig += 1;
			}

			carry = charToInt( lDig );
			curLine.insert( curLine.begin(), rDig );
		}

 		if ( carry > 0 )
			curLine.insert( curLine.begin(), intToChar( carry ) );

		result = result + BigInt( curLine );
	}

	if ( isNegative != right.isNegative )
		result.isNegative = true;

	return result;
}

BigInt BigInt::operator/( const BigInt& right )
{
	if ( BigInt( data ) < BigInt( right.data ) )
		return BigInt( "0" );
	else if ( BigInt( right.data ) == BigInt( 0 ) )
	{
		cout << "Division by 0, exiting...\n";
		exit( 1 );
	}

	string strResult;
	size_t divisions;
	string currentComparison = data.substr( 0, 1 );

	for ( size_t i = 0; i < data.length(); i++ )
	{
		divisions = numberOfDivisions( currentComparison, right.data );

		strResult.push_back( intToChar( divisions ) );

		currentComparison = BigInt( BigInt( currentComparison ) - ( BigInt( divisions ) * BigInt( right.data ) ) ).data;
		
		currentComparison.push_back( data[ i + 1 ] );
	}

	BigInt result( strResult );
	result.isNegative = isNegative != right.isNegative ;
	return result;
}

void BigInt::makeLengthsEqual( string& a, string& b )
{
	if ( a.size() == b.size() )
		return;
	else if ( a.size() > b.size() )
	{
		int dif = a.size() - b.size();
		for ( int i = 0; i < dif; i++ )
			b.insert( b.begin(), '0' );
	}
	else
	{
		int dif = b.size() - a.size();
		for ( int i = 0; i < dif; i++ )
			a.insert( a.begin(), '0' );
	}
}

void BigInt::removeLeadingZeroes( string& str )
{
	size_t index;
	for ( index = 0; str[index] == '0'; index++ );
	str = str.substr( index, str.size() - 1 );

	if ( str.size() == 0 )
		str.append( "0" );
}

string BigInt::sum( string left, string right )
{
	string result;
	string tempLeft = left;
	string tempRight = right;
	makeLengthsEqual( tempLeft, tempRight );

	int size = tempLeft.size();

	int curLeft = 0;
	int curRight = 0;
	int remain = 0;
	int carry = 0;

	for ( int i = 0; i < size; i++ ) // same as addition pos + pos
	{
		curLeft = charToInt( tempLeft[ size - 1 - i ] ) + carry;
		curRight = charToInt( tempRight[ size - 1 - i ] );

		carry = ( curLeft + curRight ) / 10;
		remain = ( curLeft + curRight ) % 10;

		result.insert( result.begin(), intToChar( remain ) );
	}

	if ( carry > 0 )
		result.insert( result.begin(), intToChar( carry ) );

	return result;
}

string BigInt::difference( string left, string right )
{
	string result;
	string tempLeft;
	string tempRight;

	int curLeft = 0;
	int curRight = 0;
	int remain = 0;

	// make sure the bigger number is the "left" number
	{
		// l and r are so temporary that they should have their own scope

		string l = left;
		string r = right;
		makeLengthsEqual( l, r );

		if ( l > r ) 
		{
			tempLeft = l;
			tempRight = r;
		}
		else if ( l < r )
		{
			tempLeft = r;
			tempRight = l;
		}
		else
			return "0";
	}

	int size = tempLeft.size();

	for ( int i = 0; i < size; i++ )
	{
		int index = size - 1 - i;
		curLeft = charToInt( tempLeft[ index ] );// + carry;
		curRight = charToInt( tempRight[ index ] );

		if ( curLeft < curRight ) // borrow
		{
			int j, count = 0;

			for ( j = index - 1; tempLeft[j] == '0'; j--, count++ ); // find first non-zero to the left
			tempLeft[ j ]--;
			for ( int l = j + 1; l < index; l++ ) // setting each counted 0 equal to 9, but as a character
				tempLeft[ l ] = '9';

			curLeft += 10;
		}
		remain = ( curLeft - curRight );

		result.insert( result.begin(), intToChar( remain ) );
	}

	return result;
}

int BigInt::numberOfDivisions( const string& numerator, const string& denominator )
{
	if ( BigInt( numerator ) < BigInt( denominator ) )
		return 0;
	else
	{
		BigInt l = BigInt( numerator ), r = BigInt( denominator );

		for ( int i = 0;; i++ )
		{
			l = l - r;

			if ( l.isNegative )
				return i;
			else if ( l == BigInt( "0" ) )
				return i + 1;

			if ( i > 9 )
			{
				cout << l.data << " was left\n";
				cout << "Division math was done incorrectly, exiting...\n";
				exit( 1 );
			}
		}
	}
}