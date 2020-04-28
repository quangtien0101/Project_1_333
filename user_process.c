#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>

#include<fcntl.h>

#define BYFFER 99              
static char receive[BYFFER];    

int main()
{
   int device;

   device = open("/dev/random_number", O_RDWR); 
   // Open the device with read/write rights
   if (device < 0)
	{
      perror("Can't open the device!");
      return errno;
   	}

   printf("Getting the magic number\n");

   int recv = read(device, receive, BYFFER);       
   if (recv < 0)
   {
      perror("Something went wrong with the magic number");
      return errno;
   }
   printf("The magic number is '%s' \n", receive);
   return 0;
}
