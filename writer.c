#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define FOO 4096

struct shared_mem{
    char data[FOO];
    int reader1_done;
    int reader2_done;
};

int main() {
    int shmId;
    struct shared_mem *shmPtr;
    key_t my_key;

    // generates key
    my_key = ftok("testfile.txt", 'A');  
    if (my_key == -1) {
        perror("ftok failed");
        exit(1);
    }
    printf("generated key: %d\n", my_key);

    // creates shared memory segment
    shmId = shmget(my_key, sizeof(struct shared_mem), IPC_CREAT | S_IRUSR | S_IWUSR);
    if (shmId < 0) {
        perror("shmget failed");
        exit(1);
    }

    // attaches to shared memory
    shmPtr = shmat(shmId, NULL, 0);
    if (shmPtr == (void *) -1) {
        perror("shmat failed");
        exit(1);
    }

    //initialize shared memory
    shmPtr->reader1_done =1;
    shmPtr->reader2_done = 1;
    
    // writes input into shared memory
    while (1) {
        printf("Enter message (type 'quit' to exit): ");
        fgets(shmPtr->data, FOO, stdin);

        // removes newline
        shmPtr->data[strcspn(shmPtr->data, "\n")] = 0;

        if (strcmp(shmPtr->data, "quit") == 0) {
            break;  // exits loop when "quit" is entered
        }

        // resets reader flags
        shmPtr->reader1_done = 0;
        shmPtr->reader2_done = 0;

        while (shmPtr->reader1_done ==0|| shmPtr->reader2_done == 0);

        shmPtr->data[0] = '\0';
    }

    // detaches and removes shared memory segment
    shmdt(shmPtr);
    shmctl(shmId, IPC_RMID, NULL);

    return 0;
}
