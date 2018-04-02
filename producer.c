#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

#define BUFFERSIZE 10

typedef struct {
	bool read;
	int buffer[BUFFERSIZE];
} shm_struct;

int main(int argc, char* argv[])
{
    int shmid;
    key_t key;
    shm_struct * shm;
    int rand_num; 
    FILE *fp; //file object

    //create a producer txt file
    fp = fopen("producer.txt", "w+");

    srand(time(NULL));   // should only be called once

    /*
     * We'll name our shared memory segment
     * "5678".
     */
    key = 5671;

    /*
     * Create the segment.
     */
    if ((shmid = shmget(key, sizeof(shm_struct), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    /*
     * Now we attach the segment to our data space.
     */
    if ((shm = shmat(shmid, NULL, 0)) == (shm_struct *) -1) {
        perror("shmat");
        exit(1);
    }

    /*
     * Now put some things into the memory for the
     * other process to read.
     */
	shm->read = true;

	//pushing values
	for (int j=0; j<100; j++) //100x 10  = 1000 elements
	{
		while(shm->read); //wait for consumer to read
		if (!shm->read) //if consumer has read, produce.
		{
			for (int i=0; i<10; i++)
			{
				rand_num = rand() % 100; //generate random numbers
				shm->buffer[i] = rand_num;//store to array
				fprintf(fp, "%d \n",rand_num);//store to file
			}
			shm->read = true;// allow consumer to read
		}
	}

    //destroy shared memory
    shmdt (shm);
    shmctl (shmid, IPC_RMID, NULL);

    //close file
    fclose(fp);

    exit(0);
}

