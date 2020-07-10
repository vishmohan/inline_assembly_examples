#include <iostream>
using namespace std;


int fact(int n);


int main()
{
	int num;
	cout << "Enter num: " << endl;
	cin >> num ;
    cout << fact(num) << endl;
	return 0;
}

int fact(int n)
{
	int result;	
	//rcx has the input number
	asm volatile("movq %0, %%rcx\n" ::"m"(n));
	//rax = 1
	asm volatile("movq $1, %rax\n");
	//handle special case when n = 0
	asm volatile("cmp $0, %rcx\n");
	asm volatile("cmove %rax, %rcx\n"); //if n=0 then make rcx = 1
	//rdx:rax = rcx * rax
	asm volatile("continue: imul %%rcx\n":"=A"(result)::);
	// rcx = rcx - 1 ; if rcx != 0 then continue multiplication
    asm volatile("loop continue");
	asm volatile("done: ");
	return result;
}
