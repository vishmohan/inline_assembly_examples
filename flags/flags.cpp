#include <iostream>
#include <map>
#include <cstdint>
using namespace std;

map<int,string> flags_map = {{0, "CF"},{2, "PF"},{4, "AF"},{6, "ZF"},{7,"SF"}, 
							{11,"OF"}};

void display(uint64_t flags_val, string info);

int main()
{
	uint64_t flags_val;
	uint32_t ebx_val = 5;

	uint8_t  result1;
	uint32_t  result2;
	uint32_t  result3 = 7;

	//======================================================
	// Example 1 -  add 1 to 127
	// 0x7F   -> 0111 1111
	// 0x01	  -> 0000 0001 
	// ----	     ---------
	// 0x80	  -> 1000 0000 
	// ----	     ---------
	// 1. The final result is not 0. So ZF = 0.
	// 2. CF = 0 => there is no overflow or underflow for unsigned data operation.
	// 3. OF = 1 => the result 80h is outside the range of signed byte data (-128 to +127).
	// 4. PF = 0 => the result has odd number of ones in the lower byte.
	// 5. AF = 1 => During the add there was a carry of 1 from the lower nibble to the higher nibble.
	// 6. SF = 1 => For a signed operation the result is negative (the msb is a 1).
	//======================================================

	asm volatile
	(
	"movb $127, %%al\n"
	"addb $1, %%al\n"
	"movb %%al, %0\n"
	"pushfq\n"
	"popq %1\n"
	:"=m"(result1),"=m"(flags_val)
	);

	display(flags_val, "Example1(127+1)");
	cout << "Example1(127+1) result = " << std::hex << (int) result1 << endl;

	cout << "===================================" << endl;

	//======================================================
	// Example 2 -  subtract 1 from 0
	// 0x00000000  -> 0000 0000 0000 0000 0000 0000 0000 0000 
	// 0xFFFFFFFF  -> 1111 1111 1111 1111 1111 1111 1111 1111 --> -1 in 2's complement
	// ----------	  ---------------------------------------
	// 0xFFFFFFFF  -> 1111 1111 1111 1111 1111 1111 1111 1111 -->  2's complement result => result = -1
	// ----------	  ---------------------------------------
	// 1. The final result is not 0. So ZF = 0.
	// 2. CF = 1 => The result of the subtraction (-1) is outside the range of unsigned int(0 to 2^32).
	// 3. OF = 0 => the result -1 is within the range of signed int data (-2^31 to 2^31-1).
	// 4. PF = 1 => the result has even number of ones in the lower byte.
	// 5. AF = 1 => During the sub there was a borrow of 1 from bit3 to bit4 (the lower nibble to the higher nibble).
	// 6. SF = 1 => The result is negative (msb is 1)
	//======================================================

	
	asm volatile
	(
	"movl $0, %%eax\n"
	"subl $1, %%eax\n"
	"movl %%eax, %0\n"
	"pushfq\n"
	"popq %1\n"
	:"=m"(result2),"=m"(flags_val)
	);

	display(flags_val, "Example2(0-1)");
	cout << "Example2(0-1) result = " << std::hex << result2 << endl;
	cout << "===================================" << endl;
	

	//======================================================
	// Example 3 -  5 xor  5
	// 0x05   -> 0000 0101
	// 0x05	  -> 0000 0101 
	// ----	     ---------
	// 0x00	  -> 0000 0000 
	// ----	     ---------
	// 1. ZF = 1 => The result of xor operation is 0.
	// 2. CF = 0 => there is no overflow or underflow for unsigned data operation.
	// 3. OF = 0 => the result 00h is outside the range of signed byte data (-128 to +127).
	// 4. PF = 1 => the result has even number of ones in the lower byte.
	// 5. AF = 0 => No carry from the lower nibble to the higher nibble.
	// 6. SF = 0 => the result is not negative.
	//======================================================

	
	asm volatile
	(
	"movl %2, %%ebx\n"
	"xorl %%ebx, %%ebx\n"
	"pushfq\n"
	"popq %1\n"
	:"=b"(result3),"=m"(flags_val)
	:"m"(ebx_val)
	);

	display(flags_val, "Example3 xor");
	cout << "Example3 xor result = " << std::hex << result3 << endl;

	cout << "===================================" << endl;

	


	return 0;
}

void display(uint64_t flags_val, string info)
{
	string set_flags = "Status Flags: ";

	for ( auto [k, v]: flags_map )
	{
		if ( ( flags_val >> k ) & 1 )
			set_flags += "  " + v;
	}

	cout << info << " : "<< "Flags = 0x" << std::hex << flags_val << endl;
	cout << set_flags << endl;
}
