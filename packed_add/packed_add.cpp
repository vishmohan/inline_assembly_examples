#include <iostream>
using namespace std;

int SIZE = 8;

int main()
{
	int arr1[SIZE] = {88, 77, 66, 55, 44, 33, 22, 11};
	int arr2[SIZE] = {1,  2,  3,  4,   5,  6,  7,  8};
	int result[SIZE];

    //Step 1 - Use vmovdqu to load the lower 16 bytes (128 bits) of arr1 into xmm1 -> Indices 0 through 3.
    //Step 2 - Do packed add -> lower 16 bytes (128 bits) of arr2 with the value in xmm1. (vpaddd)
    //Step 3 - Store the result of step 2 in the result array (vmovdqu)
	//Step 4 - Repeat Step1 through Step3 for the upper 16 bytes -> Indices 4 through 7.
	asm volatile(

				"vmovdqu %2, %%xmm1\n"
				"vpaddd %3, %%xmm1, %%xmm0\n"
				"vmovdqu %%xmm0, %0\n"
				"vmovdqu %4, %%xmm1\n"
				"vpaddd %5, %%xmm1, %%xmm0\n"
				"vmovdqu %%xmm0, %1\n"
				:"=m"(result[0]), "=m"(result[4])
				:"m"(arr1[0]), "m"(arr2[0]), "m"(arr1[4]), "m"(arr2[4])
				);


	for( int i = 0; i < SIZE; ++i )
	{
		cout << result[i] << endl;
	}

	return 0;
}
