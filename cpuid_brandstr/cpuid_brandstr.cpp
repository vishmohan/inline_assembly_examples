#include <iostream>
#include <cstdint>
#include <cpuid.h>
using namespace std;


/*

BRAND STRING is a 48 byte string 

int __get_cpuid(unsigned int __level,  --> specifies which cpuid leaf
																			 --> extended leaf = 0x8000_000{2,3,4} for brand string
																			 --> each leaf returns 16 bytes of processor brand string

									unsigned int* __eax, --> eax = bytes 0 thru     3   of processor brand string
									unsigned int* __ebx, --> ebx = bytes 4 thru     7   of processor brand string
									unsigned int* __ecx, --> ecx = bytes 8 through  11  of processor brand string
									unsigned int* __edx);--> edx = bytes 12 through 15  of processor brand string

The return value of the function is 1 , if a valid cpuid information is returned in the variables eax through edx.
If an illegal leaf (level) is specified as input, the functions returns a 0 and the values in eax through edx are
invalid.

Processor brand string = { 16 bytes returned by cpuid(0x8000_0002)
													 16 bytes returned by cpuid(0x8000_0003) 
													 16 bytes returned by cpuid(0x8000_0004)
												 }

*/

const int BRANDSTRING_SIZE = 48;
const int BRANDSTRING_SUB_SIZE = 16; //for every call we get 16 bytes of brand string
const int NUM_ITERATIONS = 3; //BRANDSTRING_SIZE = BRANDSTRING_SUB_SIZE * NUM_ITERATIONS

string process_ascii_bytes(unsigned eax, unsigned ebx, unsigned ecx, unsigned edx);

int main() {

	unsigned int level{0x80000002}; //Extended Leaf2 starting value
	unsigned eax{0}, ebx{0}, ecx{0}, edx{0};
	string brand_string{};

	//call cpuid for extended leaf2 through extended leaf4
	for(unsigned i=0 ; i < NUM_ITERATIONS ; ++i) {
		if( __get_cpuid(level+i,&eax,&ebx,&ecx,&edx) ) {
			brand_string += process_ascii_bytes(eax, ebx, ecx, edx);
		} else {
			cout << "invalid cpuid information. did you specify a valid leaf?" << endl;
		}
	}

	cout << "Processor Brand string is : " << endl;

	cout << brand_string << endl;

	return 0;

}

string process_ascii_bytes(unsigned eax, unsigned ebx, unsigned ecx, unsigned edx) {

	char brand_str[BRANDSTRING_SUB_SIZE+1]{'\0'};

	for (unsigned i = 0; i < 4; ++i) {
		brand_str[i]    =  ((eax>>(i*8))&(0xff)) ;
		brand_str[i+4]  =  ((ebx>>(i*8))&(0xff)) ;
		brand_str[i+8]  =  ((ecx>>(i*8))&(0xff)) ;
		brand_str[i+12] =  ((edx>>(i*8))&(0xff)) ;
	}

	return string(brand_str);
}




















