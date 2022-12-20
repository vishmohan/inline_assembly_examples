#include <iostream>
using namespace std;

/* copy elements from m_src array to m_dest array
using the vmovupd instruction.
First vmovupd is a load from the src array.
Second vmovupd is a store to the dest array.
*/

const int NUM_ELEMENTS = 8;

int main() {

	double m_src[NUM_ELEMENTS]  = {0.0,1.1,2.2,3.3,4.4,5.5,6.6,7.7};
	double m_dest[NUM_ELEMENTS] = {0.0};

	asm (
			"vmovupd %1, %%zmm31\n"
			"vmovupd %%zmm31, %0\n"
			:"=m"(m_dest)
			:"m"(m_src)
			:
	);
	
	for(int i=0; i < NUM_ELEMENTS; ++i)
		cout << m_dest[i] << " ";
	cout << endl;

	return 0;
}
