// assembly inline code will only handle short strings
// x86 family if searching for machine constraints

#include <stdio.h>
#include <stdlib.h>

static inline char * sub_str(char *dst, char *src, int s_idx, int e_idx){
  printf("in sub_str\n");
  // length of resultant substring + 1 for null-terminating character
  int len = e_idx - s_idx + 1;
  int i;
  // loop through substring characters, adding them to char array located at *dst
  for(i = 0; i < len; i++){
    *(dst + i) = *(src + s_idx + i);
  }
  // null-terminating character
  *(dst + len) = '\0';
  return dst;
}

static inline char * asm_sub_str(char *dst, char *src, int s_idx, int e_idx){
  printf("in asm_sub_str\n");
  // inline assembly statement
  // volatile to tell compiler not to optimize since it could cause problems
  __asm__ __volatile__("cld\n" // sets string direction flag to 0
                       "rep\n" // repeats while "c" (rcx) is not 0
                       "movsb\n" // moves character (byte) from "S" ESI to "D" EDI
                       : //output
//x86 family of machine constraints below
                       : "S" (src + s_idx), "D" (dst), "c" (e_idx - s_idx + 1)//input
                       : //clobbered register(s)
  );
                         // "S" is for RSI register, "D" is for RDI register, "c" is for RCX
}

int main(int argc, char **argv){
  // get a string and starting and ending indices from the command line
 // char str2sub = *(argv[1]);
  char *str2sub = argv[1];
  int start_idx = atoi(argv[2]);
  int end_idx = atoi(argv[3]);
  char res_sub_str[end_idx - start_idx + 1];
  char res_asm_sub_str[end_idx - start_idx + 1];
  // call a C function, d1=sub_str(d, s, s_idx, e_idx)
  char *d1 = sub_str(res_sub_str, str2sub, start_idx, end_idx);
  // call a C function, d2=asm_sub_str(d, s, s_idx, e_idx) using inlining
  char *d2 = asm_sub_str(res_asm_sub_str, str2sub, start_idx, end_idx);
  // compare the two strings d1 and d2 and return the result
  printf("res_sub_str: %s  |  res_asm_sub_str: %s\n", res_sub_str, res_asm_sub_str);
  return 0;
}
