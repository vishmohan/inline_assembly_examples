#include <iostream>
using namespace std;

int main()
{
	char mystr[20] = "This is a string";
	int al = 0;
	int length = 0;

	//step 1 - load the starting address of the first string
	asm volatile("lea %0, %%rbx\n" ::"m"(mystr));

	//step 2 - Use the scan string instruction to scan for the null byte
	asm volatile("cld\n"
					"repne scasb\n"::"D"(mystr),"a"(al)
				 );
	//step 3 - rdi is now pointing to the null+1 byte 
	// decrement to get the address of the null byte
    asm volatile("dec %rdi\n");

	//step 4 - subtract the firstbyte address from the lastbyte addr  
    asm volatile("sub %%rbx, %%rdi" :"=D"(length));

	cout << "The length of the string is: " << length << endl;
	return 0;
}
