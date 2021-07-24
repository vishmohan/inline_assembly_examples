#include <iostream>
#include <cstdint>
using namespace std;

int main()
{

	// Unsigned Multiplication 
	// Multiply by 2 => Shift left by 1

	uint32_t value1 = 37;
	uint32_t value1times2 = 0;

	asm volatile
	(
		"shll $1, %%eax"
		:"=a"(value1times2)
		:"a"(value1)
		:"cc"

	);

	cout << value1 << " multiplied by 2 is " << value1times2 << endl;

	// Unsigned Division
	// Divide by 2 => Shift right by 1

	uint32_t value2 = 336;
	uint32_t oldvalue2 = value2;

	asm volatile
	(
		"shrl $1, %0"
		:
		:"m"(value2)
		:"cc"

	);
	
	cout << oldvalue2 << " divided by 2 is " << value2 << endl;

	// signed Multiplication 
	// Multiply by 4 => Shift left by 2

	int32_t value3 = -17;
	int32_t value3times4 = 0;

	asm volatile
	(
		"sall $2, %1"
		:"=a"(value3times4)
		:"a"(value3)
		:"cc"

	);



	cout << value3 << " multiplied by 4 is " << value3times4 << endl;


	//Signed division
	//Divide by 4 => arith shift right by 2

	int32_t value4 = -180;
	int32_t value4div4 = 0;

	asm volatile
	(
		"sarl $2, %1"
		:"=b"(value4div4)
		:"b"(value4)
		:"cc"
	);


	cout << value4 << " after divide by 4 is " << value4div4 << endl;


	// Multiply a number by 48 using shifts
	// n * 48 = (n * 64) - (n * 16)

	int32_t value5 = 10;
	int32_t value5times48 = 0;

	asm volatile
	(
		"shll $6, %1\n"
		"shll $4, %2\n"
		"subl %%ecx, %%ebx\n"
		:"=b"(value5times48)
		:"b"(value5),"c"(value5)
		:"cc"


	);


	cout << value5 << " after multiplication by 48 is " << value5times48 << endl;



	return 0;
}
