#include <iostream> 
using namespace std;

const int NUM_ELEMENTS = 4;

/* 
	Take an array of temperatures in
	celsius and update it with the fahrenheit value.
	If C is the temperature in Celsius, the temperature
	in Fahrenheit F is given by:

	F = (9/5) * C + 32.0 
	
Fused Multiply Add: (FMA)

1. Each element in the input arrray is scaled: (This is the multiply part of the FMA)
		scaling = 9/5 = 1.8 
2. The result is then added with 32.(This is the additon part of the FMA)

two options shown here:

1. 256 bit memory for src3 -> m256
vfmadd132pd ymm1, ymm2, m256 

2. broadcast memory for src3 -> m64bcst
vfmadd132pd ymm1, ymm2, m64bcst

*/

int main() {

	double temps[NUM_ELEMENTS] = {30.0, 27.2,45.0, 5.3}; //ymm1
	double inc[NUM_ELEMENTS] = {32.0,32.0,32.0,32.0}; //ymm2
	double scaling[NUM_ELEMENTS] = {1.8,1.8,1.8,1.8}; //ymm3
	//double nscaling{1.8}; //broadcast

	cout << "Temps in Celsius: " << endl;
	for(int i=0; i < NUM_ELEMENTS; ++i)
		cout << temps[i] << " ";
	cout << endl;
	

	// with m256 (256bit memory -> 4 doubles)
	asm volatile (
		"vzeroall\n"
		"vmovupd %%ymm1, %0\n"
		"vmovupd %%ymm2, %2\n"
		"vfmadd132pd %%ymm1, %%ymm2, %1\n"
		"vmovupd %0, %%ymm1\n" 
		:"+m"(temps)
		:"m"(scaling),"m"(inc)
		:
	);


// with broadcast memory - avx512 version
	/*asm volatile (
		"vzeroall\n"
		"vmovupd %%ymm1, %0\n"
		"vmovupd %%ymm2, %2\n"
		"vfmadd132pd %%ymm1, %%ymm2, %1%{1to4%}\n"
		"vmovupd %0, %%ymm1\n" 
		:"+m"(temps)
		:"m"(nscaling),"m"(inc)
		:
	);*/



	cout << "Temps in Fahrenheit: " << endl;
	for(int i=0; i < NUM_ELEMENTS; ++i)
		cout << temps[i] << " ";

	cout << endl;

	return 0;
}
