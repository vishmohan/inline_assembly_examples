/*
Demonstrate FSAVE instruction to save the x87 FPU state
*/

#include <iostream>
#include <cstdint>
#include <math.h>
#include <string>
using namespace std;

#pragma pack(1)
typedef struct
{
	uint64_t fplow = 0;
	uint16_t fphigh = 0;

}fpr;

typedef struct 
{
	uint16_t fpcw;   //fp control word bytes[1:0]
	uint16_t rsvd0;  //bytes[3:2]
	uint16_t fpsw;   //fp status word bytes[5:4]
	uint16_t rsvd1;  //bytes[7:6]
	uint16_t fptw;   //fp tag word bytes [9:8]
	uint16_t rsvd2;  //bytes[11:10]
	uint32_t fpip;   //fpip[31:0] bytes [15:12]
	uint16_t fip_cs; //cs selector bytes[17:16]
	uint16_t fp_op;  //fp opcode bytes[19:18] 
	uint32_t fpdp;   //fpdp[31:0] bytes [23:20]
	uint16_t fip_ds; //ds selector bytes[25:24]
	uint16_t rsvd3;  //bytes[27:26]
	fpr 	 st0 = {0,0};
	fpr 	 st1 = {0,0};
	fpr 	 st2 = {0,0};
	fpr 	 st3 = {0,0};
	fpr 	 st4 = {0,0};
	fpr 	 st5 = {0,0};
	fpr 	 st6 = {0,0};
	fpr 	 st7 = {0,0};

} myfpenv; 


inline void save_fp_state();
inline void display_fp_env(myfpenv* fpenvptr, string info);

int main(void)
{
	save_fp_state();
	return 0;
}

//====================================
//perform a bunch of fp operations
//then save the fpu state with fsave
//call display_fp_env to display the fpu state.
//====================================
inline void save_fp_state()
{
	myfpenv myfpenv1  = {0,0,0,0,0,0,0,0,0,0,0,0};

	double dummy1 = 7.77;
	float  dummy2 = 5.55;
	long double dummy3 = 9.99;

	// 1. st0 = pi, 
	// 2. st1 = 0 
	// 3. st2 = 1.44(log e base 2) 
	// 4. st3 = 3.32(log 10 base 2)
	// 5. st4 = 1.0
	// 6. st5 = 5.55 
	// 7. st6 = 7.77 
	// 8. st7 = 9.99 
	asm volatile(
				 "finit\n"   // initialize fpu
				 "fldt %2\n" // double extended precision
				 "fldl %0\n" // double value
				 "flds %1\n" // float value
				 "fld1\n"    // push 1.0 to the fpu stack
				 "fldl2t\n"  // push log 10 (base 2)  
				 "fldl2e\n"  // push log e (base 2)  
				 "fldz\n"    // push 0.0
				 "fldpi\n"   // push PI
				 :
				 :"m"(dummy1),"m"(dummy2),"m"(dummy3)
				 :
				); 

	// fsave saves fpu state and initializes the fpu after saving
	asm volatile(
				  "fsave %0\n" //save fpu state - mem address at placeholder 0
                 :"=m"(myfpenv1)
				 :
				 :"memory"
				);

	display_fp_env(&myfpenv1,"after fsave");


}

inline void display_fp_env(myfpenv* fpenvptr, string info)
{
	double fpr_register = 0.0;

	/*cout << "fpcw "   << info << " = " << std::hex << fpenvptr->fpcw << endl;
	cout << "fpsw "   << info << " = " << std::hex << fpenvptr->fpsw << endl;
	cout << "fptw "   << info << " = " << std::hex << fpenvptr->fptw << endl;
	cout << "fpip "   << info << " = " << std::hex << fpenvptr->fpip << endl;
	cout << "fip cs " << info << " = " << std::hex << fpenvptr->fip_cs << endl;
	cout << "fp_op "  << info << " = " << std::hex << fpenvptr->fp_op << endl;
	cout << "fpdp "   << info << " = " << std::hex << fpenvptr->fpdp << endl;
	cout << "fip ds " << info << " = " << std::hex << fpenvptr->fip_ds << endl;*/

  	for( int i = 0; i < 8; ++i )
	{
		fpr* addr = &fpenvptr->st0 + i ;
		asm volatile(
					"fldt %1\n"
					"fstpl %0\n"
					:"=m"(fpr_register)
					:"m"(*addr)
					:
				);

		cout << "st" << i  << " " << info << " = " << fpr_register << endl;
	}	


}

