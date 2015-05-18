#include <ctime>
#include <fstream>
#include <Windows.h>

#include "BigInt.h"

int main( int argc, char* argv[] )
{
	BigInt x;
	BigInt y;
	BigInt result;

	x = 0;
	y = 1;

	ofstream out;
	out.open( "fib_out.txt" );
	time_t first;

	while ( 1 )
	{
		first = clock();

		out << x << "\n";

		result = x + y;
		x = y;
		y = result;

		if ( GetAsyncKeyState( VK_ESCAPE ) )
			break;

		cout << (float)( clock() - first ) / (float)CLOCKS_PER_SEC << "\n";
	}

	out.close();

	return 0;
}