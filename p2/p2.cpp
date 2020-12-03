//* CS47 - Project #2 template */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Example - add function */
long add (long op1, long op2)
{
	long output=0;
asm
(
	"movq %1, %%rax;" 		
	"addq %2, %%rax;" 
	 : "=a" (output)
	 : "r" (op1), "r" (op2) 
	 : 
	 ); /* add the second operand to eax, eax has result */
	 return output;
}

void sorter (long* list, long count, long opcode)
{
/* Move the array pointer to rax, opcode to rbx, count to rcx */
/* The following sample code swap the array elements in reverse order */
/* You would need to replace it with the logic from the bubble sort algorithm */
asm
(
	"movq %0, %%rax;"           
	"movq %1, %%rbx;" 

	"movq %2, %%rcx;" //rcx is count
	"movq $0, %%rdx;" //rdx is x
	"movq $0, %%rsi;" //rsi is y

"loop_start:"
	"cmpq %%rdx, %%rcx;" //check if x < count - 1
	"je done;" // end loop if
	"jmp loop2;" // else go to inner loop
"loop2: "
	"cmpq %%rsi, %%rcx;" //check if y is equal to count
	"je end_loop2;" //jump to end loop2 if

	//get element of x and y
	"movq (%%rax, %%rdx, 8), %%r10;"
	"movq (%%rax, %%rsi, 8), %%r11;"
	"cmpq $1, %%rbx;" //asc or dsc
	"jne dsc;" //if dsc jump to dsc
	"cmpq %%r10, %%r11;" //compare elements from x and y
	"jg swap;" //if greater than swap
	"inc %%rsi;" //increment y
	"jmp loop2;" 
"dsc: " //if less than swap for a dsc
	"cmpq %%r10, %%r11;"
	"jl swap;"
	"inc %%rsi;"
	"jmp loop2;"
"swap: "
	"xchgq %%r10, (%%rax, %%rsi, 8);"		
	"xchgq %%r11, (%%rax, %%rdx, 8);"
	"inc %%rsi;"
	"jmp loop2;"
"end_loop2: "
	"movq $0, %%rsi;"
	"inc %%rdx;"
	"jmp loop_start;"
"done:"
 	 : 
	 : "r" (list), "r" (opcode), "r" (count)
	 : 
	 ); 

}

int main(int argc, char** argv)
{
	long numlist[1000], asc;
	FILE *fptr;

	long i = 0;
	
	if (argc != 3)
	{
		printf("Usage: %s filename asc_des\n", argv[0]);
		return 1;
	}

	asc = atol (argv[2]);
	asc = (asc == 1) ? 1 : 2;

	printf("\n");

	fptr = fopen((argv[1]), "rtc");
	if (fptr == NULL)
      printf( "File %s was not opened\n",argv[1] );
	else
   {
      /* Set pointer to beginning of file: */
      fseek( fptr, 0L, SEEK_SET );

      /* Read data from file: */
	  while ( fscanf(fptr, "%ld", &numlist[i]) != EOF )
	  {
          printf( "%ld ", numlist[i] );
		  i++;
	  }

      printf( "\n\nNumber of integer = %ld\n", i );
      printf( "Ascend_or_Descend = %ld\n\n", asc );
      fclose( fptr );
   }

   sorter( numlist, i, asc);

   for (int j = 0; j < i; j++)
          printf( "%ld ", numlist[j] );

   printf("\n");

   return 0;
}



	


