#include <iostream>
using namespace std;

int SIZE = 5;
int find_max(int* a);

int main()
{
	int a[SIZE] = {21,17,11,62,43};
    cout << find_max(a) << endl;	
	return 0;
}


int find_max(int* a)
{
	int max = 0;
    //1. Load the base address of the array in rbx
    //2. Load the size of the array in rcx
	//3. Load the first element of the array in eax
    //4. compare element with max
    //5. if element > max then max = element
    //6. Advance pointer to the next element of the array
    //7. decrement rcx; if rcx!=0 jump to step 3
	asm volatile(

				"movq %1, %%rbx\n"
				"movl SIZE, %%ecx\n"
				"continue: movl (%%rbx), %%eax\n"
				"cmp %%rdi, %%rax\n"
				"cmovg %%rax, %%rdi\n"
				"add $4, %%rbx\n" //points to the next element
				"loop continue\n"
				:"+D"(max)
				:"m"(a)
				);

	return max;
}
