





#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>




int main(int argc, char **argv) {


			char str1[20];
			char *answer1;
			
			 strcpy(str1, argv[argc-1]);

	     answer1 = strstr(str1, "jpg");

	
		if (answer1 != NULL && answer1[0] != '\0')
		
		          printf("%s\n", answer1);
		          
			//if (answer1 == NULL && answer1[0] == '\0')
			//	printf("is NULL\n");
				
return 0;
}
