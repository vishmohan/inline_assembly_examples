#include <iostream>
#include <cstring>
using namespace std;

//
//use aeskeygenassist : aeskeygenassist xmm1, xmm2/m128, imm8
//duplicate xmm1[127:96] into xmm1 - destination of aeskey
//using vpermilps:
//vpermilps xmm1, xmm1, imm8 -> vex.128.66.0F3A.W0 04/r
//Now xmm1[127:96], xmm1[95:64], xmm1[63:32], xmm1[31:0] will
//have the same value as {xmm1[127:96]} after the aeskeygenassist

//Key expansion steps:
//xmm1 = {g(w[3]),g(w[3]),g(w[3]),g(w[3])} after aeskeygenassist and vpermilps
//xmm0 = {w[3],w[2],w[1],w[0]} original key
//xmm1 ^ xmm0 = {g(w[3])^w[3],g(w[3])^w[2],g(w[3]) ^ w[1],g(w[3])^w[0]} -> vpxor xmm1, xmm0, xmm1
//vpslldq xmm0, xmm0, imm8 -> xmm0 = {w[2],w[1],w[0],0} 
//xmm1 ^ xmm0 = {g(w[3])^w[3]^w[2],g(w[3])^w[2]^w[1],g(w[3]) ^ w[1]^w[0],g(w[3])^w[0]} -> vpxor xmm1, xmm0, xmm1
//vpslldq xmm0, xmm0, imm8 -> xmm0 = {w[1],w[0],0,0} 
//xmm1 ^ xmm0 = {g(w[3])^w[3]^w[2]^w[1],g(w[3])^w[2]^w[1]^w[0],g(w[3]) ^ w[1]^w[0],g(w[3])^w[0]} -> vpxor xmm1, xmm0, xmm1
//vpslldq xmm0, xmm0, imm8 -> xmm0 = {w[0],0,0,0} 
//xmm1 ^ xmm0 = {g(w[3])^w[3]^w[2]^w[1]^w[0],g(w[3])^w[2]^w[1]^w[0],g(w[3]) ^ w[1]^w[0],g(w[3])^w[0]} -> vpxor xmm1, xmm0, xmm1


const int MESSAGE_LEN = 16;
const int KEY_LEN = 16;
const int NUM_ROUNDS = 10;

void do_aes_decrypt(uint8_t* cipher,uint8_t* expanded_key,uint8_t* decipher) {

    uint8_t* decrypt_key[(NUM_ROUNDS * KEY_LEN)+16]{};


    //Round_Key_Decrypt [0] = Round_Key_Encrypt [Nr]
    // for round = 1, 2, to Nr-1
    // Round_Key_Decrypt [round] = InvMixColumns (Round_Key_Encrypt [Nr- round]) 
    //Round_Key_Decrypt [Nr] = Round_Key_Encrypt [0]
    for(int i=1; i < NUM_ROUNDS;++i)
        asm volatile(
                        "aesimc %1, %%xmm1\n"
                        "vmovupd %%xmm1,%0\n"
                        :"=m"(*(uint8_t(*)[16])(decrypt_key+(i*16)))
                        :"m"(*(expanded_key+((NUM_ROUNDS - i)*16)))
                        :
                    );   

    //Round_Key_Decrypt [0] = Round_Key_Encrypt [Nr]
    asm volatile(
                  "vmovupd %1, %%xmm1\n"
                  "vmovupd %%xmm1,%0\n"
                  :"=m"(*(decrypt_key+0))
                  :"m"(*(expanded_key+160))
                  :
                  );

    //Round_Key_Decrypt [Nr] = Round_Key_Encrypt [0]
    asm volatile(
                  "vmovupd %1, %%xmm1\n"
                  "vmovupd %%xmm1,%0\n"
                  :"=m"(*(decrypt_key+160))
                  :"m"(*(expanded_key+0))
                  :
                  );

    asm volatile( "vmovupd %0, %%xmm1\n"
                  "vpxor %1, %%xmm1, %%xmm1\n"
                  "vmovupd %%xmm1, %0\n"
                  :"+m"(*cipher)
                  :"m"(*decrypt_key)
                  :
                );

    /*for(int i=1; i < NUM_ROUNDS; ++i)
        asm volatile( "vmovupd %0, %%xmm1\n"
                      "aesdec %1, %%xmm1\n" 
                      "vmovupd %%xmm1, %0\n"
                      :"+m"(*cipher)
                      :"m"(*(decrypt_key+(i*16)))
                      :
                      );*/

    //unrolling the above decryption loop
    asm volatile( "vmovupd %0, %%xmm1\n"
                  "aesdec %1, %%xmm1\n" 
                  "vmovupd %%xmm1, %0\n"
                   :"+m"(*cipher)
                   :"m"(*(decrypt_key+(1*16)))
                   :
                  );
    asm volatile( "vmovupd %0, %%xmm1\n"
                  "aesdec %1, %%xmm1\n" 
                  "vmovupd %%xmm1, %0\n"
                   :"+m"(*cipher)
                   :"m"(*(decrypt_key+(2*16)))
                   :
                  );
    asm volatile( "vmovupd %0, %%xmm1\n"
                  "aesdec %1, %%xmm1\n" 
                  "vmovupd %%xmm1, %0\n"
                   :"+m"(*cipher)
                   :"m"(*(decrypt_key+(3*16)))
                   :
                  );
    asm volatile( "vmovupd %0, %%xmm1\n"
                  "aesdec %1, %%xmm1\n" 
                  "vmovupd %%xmm1, %0\n"
                   :"+m"(*cipher)
                   :"m"(*(decrypt_key+(4*16)))
                   :
                  );
    asm volatile( "vmovupd %0, %%xmm1\n"
                  "aesdec %1, %%xmm1\n" 
                  "vmovupd %%xmm1, %0\n"
                   :"+m"(*cipher)
                   :"m"(*(decrypt_key+(5*16)))
                   :
                  );
    asm volatile( "vmovupd %0, %%xmm1\n"
                  "aesdec %1, %%xmm1\n" 
                  "vmovupd %%xmm1, %0\n"
                   :"+m"(*cipher)
                   :"m"(*(decrypt_key+(6*16)))
                   :
                  );
    asm volatile( "vmovupd %0, %%xmm1\n"
                  "aesdec %1, %%xmm1\n" 
                  "vmovupd %%xmm1, %0\n"
                   :"+m"(*cipher)
                   :"m"(*(decrypt_key+(7*16)))
                   :
                  );
    asm volatile( "vmovupd %0, %%xmm1\n"
                  "aesdec %1, %%xmm1\n" 
                  "vmovupd %%xmm1, %0\n"
                   :"+m"(*cipher)
                   :"m"(*(decrypt_key+(8*16)))
                   :
                  );
    asm volatile( "vmovupd %0, %%xmm1\n"
                  "aesdec %1, %%xmm1\n" 
                  "vmovupd %%xmm1, %0\n"
                   :"+m"(*cipher)
                   :"m"(*(decrypt_key+(9*16)))
                   :
                  );

    asm volatile( "vmovupd %1, %%xmm1\n"
                  "aesdeclast %2, %%xmm1\n" 
                  "vmovupd %%xmm1, %0\n"
                   :"=m"(*decipher)
                   :"m"(*cipher),"m"(*(decrypt_key+160))
                   :
                );

}

void do_aes_1to9(uint8_t* cipher,uint8_t* expanded_key) {
    //vaesenc xmm1, xmm2, m128
    //        dest  text   key
    //vex.128.66.0F38.WIG DC /r
    /*for(int i=1; i < NUM_ROUNDS ; ++i) {
        asm volatile
                (
                    "vmovupd %0, %%xmm2\n"
                    "vaesenc %1, %%xmm2, %%xmm1\n"
                    "vmovupd %%xmm1, %0\n"
                    :"+m"(*cipher)
                    :"m"(*(expanded_key+(i*16)))
                    :
                );
    }*/

   //rounds 1 thru 9 - unrolling the above loop
   asm volatile ( "vmovupd %0, %%xmm2\n"
                  "vaesenc %1, %%xmm2, %%xmm1\n"
                  "vmovupd %%xmm1, %0\n"
                  :"+m"(*cipher)
                  :"m"(*(expanded_key+(1*16)))
                  :
                );
   asm volatile ( "vmovupd %0, %%xmm2\n"
                  "vaesenc %1, %%xmm2, %%xmm1\n"
                  "vmovupd %%xmm1, %0\n"
                  :"+m"(*cipher)
                  :"m"(*(expanded_key+(2*16)))
                  :
                );
   asm volatile ( "vmovupd %0, %%xmm2\n"
                  "vaesenc %1, %%xmm2, %%xmm1\n"
                  "vmovupd %%xmm1, %0\n"
                  :"+m"(*cipher)
                  :"m"(*(expanded_key+(3*16)))
                  :
                );
   asm volatile ( "vmovupd %0, %%xmm2\n"
                  "vaesenc %1, %%xmm2, %%xmm1\n"
                  "vmovupd %%xmm1, %0\n"
                  :"+m"(*cipher)
                  :"m"(*(expanded_key+(4*16)))
                  :
                );
   asm volatile ( "vmovupd %0, %%xmm2\n"
                  "vaesenc %1, %%xmm2, %%xmm1\n"
                  "vmovupd %%xmm1, %0\n"
                  :"+m"(*cipher)
                  :"m"(*(expanded_key+(5*16)))
                  :
                );
   asm volatile ( "vmovupd %0, %%xmm2\n"
                  "vaesenc %1, %%xmm2, %%xmm1\n"
                  "vmovupd %%xmm1, %0\n"
                  :"+m"(*cipher)
                  :"m"(*(expanded_key+(6*16)))
                  :
                );
   asm volatile ( "vmovupd %0, %%xmm2\n"
                  "vaesenc %1, %%xmm2, %%xmm1\n"
                  "vmovupd %%xmm1, %0\n"
                  :"+m"(*cipher)
                  :"m"(*(expanded_key+(7*16)))
                  :
                );
   asm volatile ( "vmovupd %0, %%xmm2\n"
                  "vaesenc %1, %%xmm2, %%xmm1\n"
                  "vmovupd %%xmm1, %0\n"
                  :"+m"(*cipher)
                  :"m"(*(expanded_key+(8*16)))
                  :
                );
   asm volatile ( "vmovupd %0, %%xmm2\n"
                  "vaesenc %1, %%xmm2, %%xmm1\n"
                  "vmovupd %%xmm1, %0\n"
                  :"+m"(*cipher)
                  :"m"(*(expanded_key+(9*16)))
                  :
                );
}

//round 10 here
void do_aes_10(uint8_t* cipher,uint8_t* expanded_key) {
    asm volatile
            (       
                    "vmovupd %0, %%xmm0\n"
                    "aesenclast %1, %%xmm0\n"
                    "vmovupd %%xmm0, %0\n"
                    :"+m"(*cipher)
                    :"m"(*(expanded_key+(10*16)))
                    :
            );

}

void key_assist(int round, uint8_t* rkey, uint8_t* temp) {

    switch(round) {
        case 1:
                asm volatile("vmovupd %0, %%xmm0\n" 
                             "aeskeygenassist $0x1, %%xmm0,%%xmm1\n"
                              : :"m"(*temp) : );
                break;

        case 2:
                asm volatile("vmovupd %0, %%xmm0\n" 
                             "aeskeygenassist $0x2, %%xmm0,%%xmm1\n"
                              : :"m"(*temp) : );
                break;

        case 3:
                asm volatile("vmovupd %0, %%xmm0\n" 
                             "aeskeygenassist $0x4, %%xmm0,%%xmm1\n"
                              : :"m"(*temp) : );
                break;

        case 4:
                asm volatile("vmovupd %0, %%xmm0\n" 
                             "aeskeygenassist $0x8, %%xmm0,%%xmm1\n"
                              : :"m"(*temp) : );
                break;

        case 5:
                asm volatile("vmovupd %0, %%xmm0\n" 
                             "aeskeygenassist $0x10, %%xmm0,%%xmm1\n"
                              : :"m"(*temp) : );
                break;

        case 6:
                asm volatile("vmovupd %0, %%xmm0\n" 
                             "aeskeygenassist $0x20, %%xmm0,%%xmm1\n"
                              : :"m"(*temp) : );
                break;

        case 7:
                asm volatile("vmovupd %0, %%xmm0\n" 
                             "aeskeygenassist $0x40, %%xmm0,%%xmm1\n"
                              : :"m"(*temp) : );
                break;

        case 8:
                asm volatile("vmovupd %0, %%xmm0\n" 
                             "aeskeygenassist $0x80, %%xmm0,%%xmm1\n"
                              : :"m"(*temp) : );
                break;

        case 9:
                asm volatile("vmovupd %0, %%xmm0\n" 
                             "aeskeygenassist $0x1b, %%xmm0,%%xmm1\n"
                              : :"m"(*temp) : );
                break;

        case 10:
                asm volatile("vmovupd %0, %%xmm0\n" 
                             "aeskeygenassist $0x36, %%xmm0,%%xmm1\n"
                              : :"m"(*temp) : );
                break;

        default:
                cout << "Error in key assist" << endl;
    };


    asm volatile("vmovupd %%xmm1, %0\n" :"=m"(*rkey) : :);
}


//temp is a pointer to the appropriate slot in the expanded key
void expand_key128(uint8_t* rkey_ptr, uint8_t* temp, uint8_t* ekey) {

        asm volatile(
                     "vmovupd %2, %%xmm0\n"
                     "vmovupd %3, %%xmm1\n" //rkey
                     "vpermilps $0xff, %%xmm1, %%xmm1\n"
                     "vpxor %%xmm1, %%xmm0, %%xmm1\n"
                     "vpslldq $4, %%xmm0, %%xmm0\n"
                     "vpxor %%xmm1, %%xmm0, %%xmm1\n"
                     "vpslldq $4, %%xmm0, %%xmm0\n"
                     "vpxor %%xmm1, %%xmm0, %%xmm1\n"
                     "vpslldq $4, %%xmm0, %%xmm0\n"
                     "vpxor %%xmm1, %%xmm0, %%xmm1\n"
                     "vmovupd %%xmm1, %0\n"
                     "vmovupd %%xmm1, %1\n"
                     :"=m"(*(uint8_t(*)[16])ekey),//(*ekey) this works too
                      "=m"(*(uint8_t (*) [16] )temp)
                     :"m"(*(ekey-16)),"m"(*rkey_ptr)
                     :
                    );

}



int main() {

    //char plaintext[MESSAGE_LEN+1]{"abcdefghijklmnop"};
    //char temp[KEY_LEN]{"encryptionisfun!"};
    //encryptionisfun!
    uint8_t temp[KEY_LEN]{ 
                           0x65, 0x6e, 0x63, 0x72,
                           0x79, 0x70, 0x74, 0x69,
                           0x6f, 0x6e, 0x69, 0x73,
                           0x66, 0x75, 0x6e, 0x21
                        };

    uint8_t plaintext[MESSAGE_LEN]{ 
                           0x61, 0x62, 0x63, 0x64, 
                           0x65, 0x66, 0x67, 0x68, 
                           0x69, 0x6a, 0x6b, 0x6c, 
                           0x6d, 0x6e, 0x6f, 0x70 
                        };
    //will contain w43..w0 after the key expansion
    uint8_t expanded_key[KEY_LEN * (NUM_ROUNDS+1)]{0};
    uint8_t round_key[KEY_LEN]{}; //output of the transformation g(w3), g(w7) etc

    uint8_t cipher[MESSAGE_LEN]{0}; //output/input to each round
    uint8_t decipher[MESSAGE_LEN]{0}; //output of decryption

    //first 4 bytes of the expanded key are from the original key
    //this is w3..w0
    memcpy(expanded_key,temp, sizeof(temp)/sizeof(uint8_t));

    //aes key expansion generates w4-w7 ...w40-w43
    for(int i=1; i <= NUM_ROUNDS; ++i) {
        key_assist(i,round_key, temp); 
        expand_key128(round_key, temp,expanded_key+(16*i)); //populate expanded_key
    }

    //prepare before round1 
    asm volatile
            (
                 "vmovupd %1, %%xmm1\n"
                 "vmovupd %2, %%xmm2\n"
                 "vpxor %%xmm1, %%xmm2, %%xmm1\n"
                 "vmovupd %%xmm1, %0\n"
                 :"=m"(*cipher)
                 :"m"(*plaintext),"m"(*expanded_key)
                 :
            );

    do_aes_1to9(cipher,expanded_key);
    do_aes_10(cipher,expanded_key);


    /*for(int i=0; i < KEY_LEN*(NUM_ROUNDS+1); ++i){
        cout << std::hex << static_cast<int>(expanded_key[i]&0xff) << " "; 
        if((i+1)%KEY_LEN==0) cout << endl;
    }
    cout << endl;*/

    for(int i=0; i < KEY_LEN; ++i) {
        cout << std::hex << static_cast<int> (cipher[i]&0xff) << " ";
    }

    cout << endl;

    do_aes_decrypt(cipher,expanded_key,decipher);

    for(int i=0; i < KEY_LEN; ++i) {
        cout << std::hex << static_cast<int> (decipher[i]&0xff) << " ";
    }

    cout << endl;

    return 0;
}

