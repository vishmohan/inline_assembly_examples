#include <iostream>
#include <cstdint>
#include <cpuid.h>
using namespace std;


/*

VENDORID is a 12 byte string 

int __get_cpuid(unsigned int __level,  --> specifies which cpuid leaf
																			 --> leaf = 0 for vendor-id
									unsigned int* __eax, --> eax value after execution of cpuid=0 (max cpuid leaf supported by the processor)
									unsigned int* __ebx, --> ebx = first 4 bytes of Vendor id [bytes 0 through 3]
									unsigned int* __ecx, --> ecx = bytes 8 through 11 of vendor id
									unsigned int* __edx);--> edx = bytes 4 through 7 of vendor id

The return value of the function is 1 , if a valid cpuid information is returned in the variables eax through edx.
If an illegal leaf (level) is specified as input, the functions returns a 0 and the values in eax through edx are
invalid.

Vendorid string = GenuineIntel
0 1 2 3 4 5 6 7 8 9 10 11
G e n u i n e I n t e  l  

			31:24| 23:16 | 15:8 | 7:0 |   
EBX			75 | 6E    | 65   | 47  |  contains ascii bytes for "Genu"  -> 4 bytes
EDX			49 | 65    | 6E   | 69	|  contains ascii bytes for "ineI"  -> 4 bytes
ECX			6C | 65    | 74   | 6E  |  contains ascii bytes for "ntel"  -> 4 bytes

*/

const int VENDORID_SIZE = 12;

void process_ascii_bytes(unsigned eax, unsigned ebx, unsigned ecx, unsigned edx);

int main() {

	unsigned int level{0}; //leaf0 for vendorid information
	unsigned eax{0}, ebx{0}, ecx{0}, edx{0};

	//call cpuid for leaf0
	if( __get_cpuid(level,&eax,&ebx,&ecx,&edx) ) {
		cout << "valid cpuid information returned by the processor" << endl;
		process_ascii_bytes(eax, ebx, ecx, edx);
	} else {
		cout << "invalid cpuid information. did you specify a valid leaf?" << endl;
	}

	return 0;

}

void process_ascii_bytes(unsigned eax, unsigned ebx, unsigned ecx, unsigned edx) {

	char vendorid[VENDORID_SIZE+1]{'\0'};

	for (unsigned i = 0; i < 4; ++i) {
		vendorid[i] = ((ebx>>(i*8))&(0xff)) ;
		vendorid[i+4] = ((edx>>(i*8))&(0xff)) ;
		vendorid[i+8] = ((ecx>>(i*8))&(0xff)) ;
	}
	vendorid[12] = '\0';

	cout << "Vendor id = " << vendorid << endl;
	cout << "Max cpuid leaf supported by the processor = " << eax << endl;

}




















