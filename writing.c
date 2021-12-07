#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/shm.h>

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>

#define SH_KEY 24602
#define SEM_KEY 24603


int main() {
   struct sembuf sb;
   sb.sem_num = 0; //index of sempahore to work on
   sb.sem_op = -1; //set operation to down

   //attempt to access file using sempahore
   int semd = semget(SEM_KEY, 1, 0);
   semop(semd, &sb, 1);
   //open file
   int file = open("message.txt", O_RDWR | O_APPEND);

   //access shared memory
   int shmd = shmget(SH_KEY, 0, 0);
   int *sizeOfLast = shmat(shmd, 0, 0);
   //goes to the end of the file
   lseek(file, -1 * (*sizeOfLast), SEEK_END);

   //read and display last line added to file
   printf("Writing...\n");
   char line[*sizeOfLast];
   read(file, line, (*sizeOfLast)-1);
   printf("Last line: %s\n", line);

   //prompt the use for the next line
   printf("Enter a new line: ");
   char input[1000];
   fgets(input, 1000, stdin);
   //sets size of last to new line's size
   *sizeOfLast = strlen(input);
   //writes new line
   write(file, input, strlen(input));

   shmdt(sizeOfLast); //detach

   sb.sem_op = 1; //set the operation to up
   semop(semd, &sb, 1); //perform the operation

   close(file);
   return 0;
}
