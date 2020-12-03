/* 
  encrypt.cpp - This file contains the code to encrypt and decrypt an input string
                and output the modified string.
*/

#include "key.h"
#include <string.h>

__declspec() long s_encrypt (char *src, char* dest, long en_flag);
__declspec() void get_key (long *dir, long *count);
__declspec() void set_key (long dir, long count);

__declspec() void get_key (long *dir, long *count)
{
	*dir = direction;
	*count = shiftcount;
}

__declspec() void set_key (long dir, long count)
{
	direction = dir;
	shiftcount = count;
}


__declspec() long s_encrypt (char* src, char* dest, long en_flag)
{
  
  long characters_changed = 0;
  
  
asm
(
    //Source String in rax
    //Destination String in rbx
    //encryption flag in rcx
    //shiftcount into r11
    //direction into r12
    //String counter in r13
    //characters converted in r14
	  "movq %1, %%rax;" 
    "movq %2, %%rbx;" 
    "movq %3, %%rcx;"
    "movq %4, %%r11;"
    "movq %5, %%r12;"
    "movq $1, %%r13;" 
    "movq $0, %%r14;"

//r15 will have string length
"string_length: "
	"movq $0, %%r15;"
	"dec %%rax;"
"count_word: "
	"inc %%r15;"
	"inc %%rax;"
	"cmpb $0, (%%rax);"
	"jnz count_word;"
"end_word: "
  "movq %1, %%rax;"

  //encryption check
"encrypt_check: "
  "cmpq $1, %%rcx;"			// check to see if it's encryption or decryption
	"je decrypt_if;"			
	"jmp shiftcount;"

//shift count is modular 26, also keep track of direction 1 or 0
"decrypt_if: "
  "cmpq $0, %%r12;"
  "je direction1;"
  "movq $0, %%r12;"
  "jmp shiftcount;"
"direction1: "
  "addq $1, %%r12;"
"shiftcount: "
  "cmpq $26, %%r11;"
  "jge mod;"
  "movq %%r11, %%r8;"
  "jmp loop;"
"mod: "
  "subq $26, %%r11;"
  "jmp shiftcount;"

// moving variables into registers complete
//shift count is in r8, direction in r12, and r15 has string length





//actual loop starts, r13 be our counter
"loop: "
  "cmpq %%r13, %%r15;"
  "je done;"

  //get character
  "movb (%%rax), %%dl;"
  "movb (%%rax), %%r9b;"
  
  //if statement checks A and Z, and lower case checks too, check direction if between
"checkA: "
  "cmpb $65, %%dl;"
  "jl copy;"
  "jmp checkZ;"
"checkZ: "
  "cmpb $90, %%dl;" 
  "jle checkDirection1;"
  "jg smallA;"
"smallA: "
  "cmpb $97, %%dl;"
  "jl copy;"
  "jge smallZ;"
"smallZ: "
  "cmpb $122, %%dl;" 
  "jle checkDirection0;"
  "jg copy;"

//Direction check will send us into forward and backward labels, this will direct to which shift label
"checkDirection1: "
  "cmpq $0, %%r12;"
  "je forward_directionUpperBound;"
  "jg backward_directionUpperBound;"
"checkDirection0: "
  "cmpq $0, %%r12;"
  "je forward_directionLowerBound;"
  "jg backward_directionLowerBound;"

  //shift character by adding shift count (depending on direction will be subtracting or adding)
  //if values goes beyond the ascii number constraints, then subtract or add 26 to wrap back
  //similar to a modular
  //jump to copy at the end
"forward_directionUpperBound: "
  "addq %%r8, %%rdx;"
  "cmpb $90, %%dl;"
  "jg sub26;"
  "jmp copy;"
"backward_directionUpperBound: "
  "subq %%r8, %%rdx;"
  "cmpb $65, %%dl;"
  "jl add26;"
  "jmp copy;"
"forward_directionLowerBound: "
  "addq %%r8, %%rdx;"
  "cmpb $122, %%dl;"
  "jg sub26;"
  "jmp copy;"
"backward_directionLowerBound: "
  "subq %%r8, %%rdx;"
  "cmpb $97, %%dl;"
  "jl add26;"
  "jmp copy;"

//for wrapping, subtracting if it goes beyond, and adding if it goes below
"sub26: "
  "subb $26, %%dl;"
  "jmp copy;"
"add26: "
  "addb $26, %%dl;"
  "jmp copy;"

// assuming we have our character now, we change the case, and then increment the counter
// increment the source string and destination string pointer, and then increment the characters
// converted
"copy: "
  "cmpb $97, %%dl;"
  "jae toUpper;"
  "orb $32, %%dl;"
"copy_into_dest: "
  "movb %%dl, (%%rbx);"
  "inc %%rax;"
  "inc %%r13;"
  "inc %%rbx;"
  "cmpb $0x20, %%dl;"
  "jne incrementConvert;"
"BackToLoop: "
  "jmp loop;"
"incrementConvert: "
  "inc %%r14;"
  "jmp BackToLoop;"
"toUpper: "
  "andb $0xdf, %%dl;"
  "jmp copy_into_dest;"

  //move characters converted into rax and null terminate destination string
"done: "
  "movb $0, (%%rbx);"
  "movq %%r14, %%rax;"


 	 : "=a" (characters_changed)
	 : "r" (src), "m" (dest), "m" (en_flag), "m" (shiftcount), "m" (direction)
	 : 
	 );  

	return characters_changed;

}