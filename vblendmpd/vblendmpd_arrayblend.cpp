#include <iostream>
using namespace std;

/* blend elements from m_src1 array and m_src2 array
	 to m_dest array
*/

const int NUM_ELEMENTS = 8;

int main() {

	double m_src1[NUM_ELEMENTS]  = {-27,1.1,-27,3.3,-27,5.5,-27,7.7};
	double m_src2[NUM_ELEMENTS]  = {0.0,-27,2.2,-27, 4.4,-27,6.6,-27};
	double m_dest[NUM_ELEMENTS] = {0.0};
	uint64_t mask{0x55};

	//"vblendmpd %2, %%zmm31, %%zmm31%{ %3 %}\n"
	asm volatile (
			"kmovq %3, %%k1\n"
			"vmovupd %1, %%zmm31\n"
			"vblendmpd %2, %%zmm31, %%zmm31 %{%%k1%}\n"
			"vmovupd %%zmm31, %0\n"
			:"=m"(m_dest)
			:"m"(m_src1),
			 "m"(m_src2),
			 "m"(mask)
			:
	);

	
	
	for(int i=0; i < NUM_ELEMENTS; ++i)
		cout << m_dest[i] << " ";
	cout << endl;
	return 0;
}
