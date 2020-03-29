#include <iostream>
using namespace std;

int val = 0;

int main(void)
{
	//step 0: save old value of rax
	 asm volatile("pushq %rax\n");
	//step 1: move 5 to an architectural register
	asm volatile("movq $5, %rax\n");

	//step 2: move the value from the register to the variable (val)
	asm volatile("movq %rax, val\n");

	//step 3: restore the old value of rax
	 asm volatile("popq %rax\n");

	cout << "The value is " << val << endl;
	return 0;
}
