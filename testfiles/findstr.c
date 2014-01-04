
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main (int argc, char **argv) 
{


char str1[40];

int a = 0;

for ( a = 0; a < 30; a++)
{



if (strstr(str1, "jpg")  || strstr(str1, "png") == NULL)
  {
   printf("found jpg -- %s\n", str1);
				//break;
  }
else
  {
   	int e;
				for ( e  = 0; e < 30; e++)
					printf("%d\n",e);
  }

}

return 0;


}// end main
