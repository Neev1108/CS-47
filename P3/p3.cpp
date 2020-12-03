//* CS47 - Project #2 template */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct student
{
  long	ID;          /* 8 bytes in 64-bit, and 4 bypes in 32-bit */
  char	name[24];
};

/* This function returns the char* to the name string if the input sid has a match on the student ID
   from the array of student structures */

long search_by_id (char* list, long count, long sid)
{
	long index=0;
asm
(
	"movq %1, %%rax;"           
	"movq %2, %%rbx;" 	
	"movq %3, %%rcx;"
	"xorq %%rdx, %%rdx;"
"loop_start:"
	"cmpq %%rdx, %%rcx;"
	"je not_found;"
	"movq (%%rax), %%rsi;"
	"cmpq %%rsi, %%rbx;"
	"je found;"
	"addq $32, %%rax;"   /* 32 = size of each record for 64-bit; In 32-bit, it is 28 */ 
	"inc %%rdx;"	
    "jmp loop_start;"
"not_found:"
	"xorq %%rax, %%rax;"
	"jmp done;"
"found:"
	"addq $8, %%rax;"
"done:"
 	 : "=a" (index)
	 : "r" (list), "r" (sid), "r" (count)
	 : 
	 );
	 return index;
}



long search_by_name (char* list, long count, char* token)
{
	long index = 0;
	
	asm
(
	

/* Moving variables into registers 
rax: holds list
rbx: holds count
rcx: holds token
rdx: will be out index for our for loop
r12: will be our index for comparing strings*/

	"movq %1, %%rax;"          
	"movq %2, %%rbx;" 	
	"movq %3, %%rcx;"	
	"movq $0, %%rdx;"   
	"movq $0, %%r12;" 
	
	
//we need token length to compare lengths of strings (to stop partial match)
// pretty much increments through characters in string until we find the null termination
"token_length: "
	"movq $0, %%r13;"
	"dec %%rcx;"
"count: "
	"inc %%r13;"
	"inc %%rcx;"
	"cmpb $0, (%%rcx);" //null termination comparision
	"jnz count;"
"end: "
	"dec %%r13;"
	"movq %3, %%rcx;"

//for loop will start, compares index to count, add 8 to get to position for name array
"for_loop:"
	"cmpq %%rdx, %%rbx;" 
	"je not_f;" 
	"addq $8, %%rax;"

//inner loop will compare the token to the name
"inner_loop: " 

//length for the name so we can stop partial match, at the end just compare the 2 lengths
//same algorithm as done for the token
"name_length: "
	"movq $0, %%r15;"
	"dec %%rax;"
"count_word: "
	"inc %%r15;"
	"inc %%rax;"
	"cmpb $0, (%%rax);"
	"jnz count_word;"
"end_word: "
	"dec %%r15;"
	"subq %%r15, %%rax;"
	"cmpq %%r13, %%r15;" //comparision, if not the same length, then we know its not the same word
	"jne not_same_word;"


//we never reached the not found label, so all characters are the same, and word is found
	"cmpq %%r13, %%r12;" 
	"je found_word;" 


	//move characters from name and token into r10 and r11 dereferenced
	//r12 incrementing in the inner loop allows to load the element at r12
	//from rax and rcx respectively
	"leaq (%%rcx, %%r12, 1), %%r10;" 
	"leaq (%%rax, %%r12, 1), %%r11;" 
	"movq (%%r11), %%r11;"
	"movq (%%r10), %%r10;"
	
	//make both characters lower case
	//inc r12 to get to next character
	"orq $32, %%r11;" 
	"orq $32, %%r10;"
	"inc %%r12;"

	//compare both characters, 
	//if not the same, then we know right away that that the names are not equal
	//so we reset indexes, add to the rax pointer to get to next name, and jump back into
	//the outer for loop
	"cmpb %%r10b, %%r11b;"
	"jne not_same_word;" 
	"jmp inner_loop;"

	//check next name
	//it will increment the index for the for loop
	//reset r12 for the next name
	//add 24 to rax to move the pointer to the next name
	//and hop back into for next iteration of the for loop
"not_same_word: "
	"inc %%rdx;"
	"movq $0, %%r12;"
	"addq $24, %%rax;"
    "jmp for_loop;"

//rax just becomes zero if name is not found
"not_f:"
	"xorq %%rax, %%rax;"
	"jmp donezo;"

//we make the pointer go back to the position where the id was located and return
"found_word:"
	"subq $8, %%rax;"
	"movq (%%rax), %%rax;"

"donezo:"

 	 : "=a" (index)
	 : "r" (list), "r" (count), "r" (token)
	 : 
	 );
	 return index;
	

}



int main(int argc, char** argv)
{
	char *token;
	char *sname=NULL;
	FILE *fptr;
	long id = 0;
	long sid = 0;
	long i = 0;
	struct student list[100];	/* array of structure */
	
	if (argc != 4)
	{
		printf("Usage: %s filename token student_id\n", argv[0]);
		return 1;
	}

	token = argv[2];

	sid = atol(argv[3]);

	printf("\n");

	fptr = fopen((argv[1]), "rtc");
	if (fptr == NULL)
      printf( "File %s was not opened\n",argv[1] );
	else
   {
      /* Set pointer to beginning of file: */
      fseek( fptr, 0L, SEEK_SET );

      /* Read data from file: */
	  while ( fscanf(fptr, "%s", (char*) &list[i].name) != EOF )
	  {
	  list[i].ID = i+1001;
          printf( "%s ", list[i].name );
		  i++;
	  }

      printf( "\n\nNumber of names = %ld\n", i );
      printf( "Search Token = %s\n", token );
      fclose( fptr );
   }

	printf( "\nStudent Id = %ld; Name = %s.\n\n", sid, (char*)search_by_id((char*)list, i, sid));

   id = search_by_name ((char*)list, i, token);

   if (id)
	   printf( "Student Id = %ld; Name = %s\n", id, list[id-(1001)].name );
   else
     printf( "Student is not found.\n");

   return 0;
}



	


