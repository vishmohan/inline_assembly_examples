#include <iostream>
using namespace std;

int main()
{
	int i_array[2] = {13, 12};
	double d_array[2] = {0.0};
	

	//Step 1: Convert the integer elements from the i_array into double. Put the 
    //          result in xmm1
	//Step 2: Move the result from xmm1 into the double array.
	asm volatile(
				"cvtpi2pd %1 ,%%xmm1\n" //conversion is done and result in xmm1
				"movdqu %%xmm1, %0\n"
				:"=m"(d_array[0])
				:"m"(i_array[0])
				);

	for ( int i = 0; i < 2 ; i++)
	{
		cout << std::fixed << d_array[i] << endl;
	}

	return 0;
}



