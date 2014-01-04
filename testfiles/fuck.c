#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>



int main (int argc, char **argv) {
	
	
	
	char str1[30] = "hwe";
	char str2[30] = "hwer";
	const char jpg[15] = "jpg"; //1
	
	const char png[15] = "png"; //2
	
	
	char *answer1; 
	char *answer2; 
	
	
	
	 strcpy(str1, argv[argc-1]);
	 strcpy(str2, str1);
	 answer1 = strstr(str1, jpg);
	 answer2 = strstr(str2, png);
	 
	 		if (answer1 && *answer1  || answer2 && *answer2)
					printf("??%s, %d\n", answer1, answer1);
			else
				abort();
				
				
			printf("still runngin\n");
	 
	 
	 
	// printf( "answer - %s ,  answer2 %s str1, %s str2 %s\n", answer1, answer2, str1, str2);
	 
	 
	 
	 
	 
	// (strcmp (argv[i], "-dia") == 0)
	/*
		if (answer1 != NULL && answer1[0] != '\0' ||  answer2 != NULL && answer2[0] != '\0')
				printf("%s --1-- %s\n", answer1, answer2);
		          
			***/
			
			/*
			 // a1 is jpg a2 is png
			if (answer1 != NULL && answer1[0] != '\0' )
			
				if (answer2 != NULL && answer2[0] != '\0')
				printf("%s --2-- %s\n", answer1, answer2);
				
				
				**/
				
		
				
				
				
				
				
				
				
				
				
				
				
	//if (strcmp(answer2, "png") == 0 )
	//	printf("%s\n", answer2);
	
	 //    char *answer1; 
	     
	//	if ( strcmp(answer1, "jpg") == 0)
		//	printf("JPG\n");
	
	
	/*
		if (strcmp(answer1,"jpg") == 0  && strcmp(answer2, "png") == 0)
		{
			printf("abort 1\n");
			//abort();
		}
			if ((strstr(str1, jpg)) || ((strstr(str2, png))))
		{
			printf("abort\n");
			//abort();
		}
		
		printf("still running\n");
		* ***/
		
		
		
		
		
		
		
		
		
		/*
		
	 if (strcmp(answer1,"jpg") == 0 || strcmp(answer2, "png") == 0 )
	 {
		 printf("|| yep %s or yep %s, \n", answer1, answer2);
			
		}
		else
		{
			printf("ABORT\n");
		}
		**/
		
return 0;
}
