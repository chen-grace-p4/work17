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


union semun {
   int              val;    /* Value for SETVAL *\/ */
   struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET *\/ */
   unsigned short  *array;  /* Array for GETALL, SETALL *\/ */
   struct seminfo  *__buf;  /* Buffer for IPC_INFO */
};


int main(int argc, char ** argv) {
   //if there is a flag or not
   if (argc > 1) {
      //create mode
      if (strcmp(argv[1], "-c") == 0 || strcmp(argv[1], "-create") == 0) {
         //create and access shared memory
         int shmd = shmget(SH_KEY, sizeof(int), IPC_CREAT | 0640);
         /*
         //shared memory should contain size of the last line
         //attach shared memory
         int *sizeOfLast;
         sizeOfLast = shmat(shmd, 0, 0);
         *sizeOfLast = 0;
         //detach shared memory
         shmdt(sizeOfLast);
         */

         //create and access semaphore and initialize value to 1
         int semd = semget(SEM_KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
         //set value to 1
         union semun us;
         us.val = 1;
         int r = semctl(semd, 0, SETVAL, us);

         //create and open file
         char * message = argv[2];
         int file = open("message.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
         close(file);

         return 0;
      }
      //remove mode
      else if (strcmp(argv[1], "-r") == 0 || strcmp(argv[1], "-remove") == 0) {
         //remove shared memory
         int shmid = shmget(SH_KEY, 0, 0);
         shmctl(shmid, IPC_RMID, 0);

         //remove semaphore
         int semid = semget(SEM_KEY, 1 ,0);
         semctl(semid, IPC_RMID, 0);

         //display full contents of the story
         char contents[1000];
         int file = open("message.txt", O_RDONLY);
         read(file, contents, 1000);
         printf("Contents of story: \n%s", contents);

         close(file);
         return 0;
      } else {
         printf("Please use flag -create or -remove\n");
      }
   } else {
      printf("Please use flag -create or -remove\n");
   }

   return 0;
}
