/*
 Strcpy using the movsb instruction.
 Inline assembly example
*/

#include <iostream>
using namespace std;


void computestrlen(char* src, int& len);


int main()
{
	char src[] = "Inline Assembly";
	int num_bytes = 0 ;
    computestrlen(src, num_bytes);
	char dest[num_bytes+1];

	asm volatile( "cld\n"
				  "rep movsb" ::"S"(src),"D"(dest),"c"(num_bytes+1));

	cout << "src string: " << src << endl;
	cout << "dest string: " << dest << endl;
	return 0;
}

void computestrlen(char* src, int& length)
{
	int al = 0;

	asm volatile("mov %0, %%rbx\n" ::"m"(src));
	asm volatile("cld\n"
					"repne scasb\n"::"D"(src),"a"(al)
				 );
    asm volatile("dec %rdi\n");
    asm volatile("sub %%rbx, %%rdi" :"=D"(length));

}
