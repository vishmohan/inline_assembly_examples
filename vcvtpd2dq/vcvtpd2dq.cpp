#include <iostream>
using namespace std;

int main()
{
	double d_array[] = {17.2789, 34.2653, 41.21, 3.149};
	int i_array[4] = {0};
	

	//Step 1: Convert the double elements from the d_array into integer. Put the 
    //          result in xmm1
	//Step 2: Move the result from xmm1 into the int array.

	asm volatile
    (
		"vcvtpd2dqy %1, %%xmm1\n"
		"vmovdqu %%xmm1, %0\n"
		:"=m"(i_array[0])
		:"m"(d_array[0])
	);



	for ( int i = 0; i < 4 ; i++)
	{
		cout << i_array[i] << endl;
	}

	return 0;
}



