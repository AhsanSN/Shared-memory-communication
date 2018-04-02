#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define BUFFERSIZE 10

typedef struct {
	bool read;//if the consumer should read or not
	int buffer[BUFFERSIZE];
} shm_struct;


int main(int argc, char* argv[])
{
    int shmid;
    key_t key;
    shm_struct *shm;
    FILE *fp; //file object

    //open file
    fp = fopen("consumer.txt", "w+");
 
    /*
     * We need to get the segment named
     * "5678", created by the server.
     */
    key = 5671;

    /*
     * Locate the segment.
     */
    if ((shmid = shmget(key, sizeof(shm_struct), 0666)) < 0) {
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
     * Now read what the server put in the memory.
     */
 
	shm->read=false;// have read now
	for (int j=0; j<100; j++)//100 x 10 = 1000 elements
	{
		while (!shm->read); //wait for it to producer to write
		if (shm->read) //read now
		{
	 		for (int i=0; i<10; i++){
				fprintf(fp, "%d \n",shm->buffer[i]);//save to file
	 		}
		shm->read = false;// have read now
		}
	}

    /*
     * cleanup: detach  shm
     */
    shmdt (shm);
    
    //close file
    fclose(fp);

    exit(0);
}
