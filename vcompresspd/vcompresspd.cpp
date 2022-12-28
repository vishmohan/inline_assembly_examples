#include <iostream>
using namespace std;

/*
	Compress the elements from the src array (src_array)
	into the destination array(dest_array).
	This example will ignore the unwanted element(255.0) from 
	the src_array and store the final result in the
	dest_array. The other elements of the destination array
	are unchanged (EVEX.z = 0)
*/

const int NUM_ELEMENTS = 8;

int main() {

	double src_array[NUM_ELEMENTS] = {1.0,255.0,255.0, 2.0, 255.0, 255.0,3.0,4.0};
	double dest_array[NUM_ELEMENTS] = {9.0,10.0,11.0, 12.0, 5.0,6.0,7.0,8.0};
	uint8_t mask{0xc9};

	asm (
				"kmovb %1, %%k3\n"
				"vmovupd %2 , %%zmm5\n"
				"vcompresspd %%zmm5, %0 %{%%k3%}\n"
				:"=m"(dest_array)
				:"m"(mask),
				 "m"(src_array)
				:
	);

	for(int i=0; i < NUM_ELEMENTS; ++i)
		cout << dest_array[i] << " ";

	cout << endl;
	
	return 0;
}
