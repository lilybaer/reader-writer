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

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <reader_id>\n", argv[0]);
        return 1;
    }

    int reader_id = atoi(argv[1]);
    int shmId;
    struct shared_mem *shmPtr;
    key_t my_key;

    // generates key
    my_key = ftok("testfile.txt", 'A');  // ensure this file exists
    if (my_key == -1) {
        perror("ftok failed");
        exit(1);
    }

    // accesses shared memory
    shmId = shmget(my_key, sizeof(struct shared_mem), 0);
    if (shmId < 0) {
        perror("shmget failed");
        exit(1);
    }

    // attaches to the shared memory
    shmPtr = shmat(shmId, NULL, 0);
    if (shmPtr == (void *) -1) {
        perror("shmat failed");
        exit(1);
    }

    // reads from shared memory
    while (1) {
        // waits for new messages
        while (shmPtr->data[0] == '\0'); // first byte used as flag for new message

        printf("Reader %d: %s\n", reader_id, shmPtr->data);

        if (strcmp(shmPtr->data, "quit") == 0) {
            break;  // Exit loop on "quit" command
        }

        // set flags to done for readers
        if (reader_id == 1) {
            shmPtr->reader1_done = 1;
        } 
        else {
            shmPtr->reader2_done = 1;
        }

        // busy wait until the message is reset by writer
        while (shmPtr->data[0] != '\0');
    }

    // detaches from shared memory
    shmdt(shmPtr);

    return 0;
}
