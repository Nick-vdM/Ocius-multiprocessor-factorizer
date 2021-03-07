//
// Created by nicol on 28/09/2020.
// https://www.linkedin.com/in/nick-vd-merwe/
// nick.jvdmerwe@gmail.com

#include <iostream>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <array>
#include <vector>
#include <unistd.h>
#include <cmath>

using namespace std;


class SharedMemory {
public:
    // server side
    SharedMemory() {
        printf("Generating key\n");
        key = ftok("shmfile", 65);
        shmid = shmget(key, sizeof(SharedData), 0666 | IPC_CREAT);
        if (shmid == -1) {
            perror("Failed to generated shmid");
            exit(EXIT_FAILURE);
        }

        data = (SharedData *) shmat(shmid, nullptr, 0);
        if (data == nullptr) {
            perror("Failed to allocate data/connect : ");
            exit(EXIT_FAILURE);
        }
        printf("Connected! Number is currently %d\n", data->number);
        for (int i = 0; i < 10; i++) {
            pthread_mutex_init(&(data->locks[i]), NULL);
        }
    }

    ~SharedMemory() {
        // server side - this is going to be clear on the other side
        for (int i = 0; i < 10; ++i) {
            pthread_mutex_destroy(&(data->locks[i]));
        }
        shmdt(data);
        // This is the server side so we destroy it
        shmctl(shmid, IPC_RMID, nullptr);
    }

    void setToDefault(){
        // Resets all the values. Sometimes the shared memory gets reused between
        // uses, and that can cause major problems.
        data->number = 0;
        data->clientFlag = false;

        for(int i = 0; i < 10; i++){
            data->slots[i] = 0;
            data->progress[i] = 0;
            data->serverFlag[i] = false;
        }
        // and we just pray that the conditionals and locks aren't locked
    }

    /**
     * Checks if there's a new number
     * @return The number's slot
     */
    int waitForReservation() {
        while (!data->clientFlag) {
            usleep(1); // wait for a reservation
        }
        int index = -1;
        for (int i = 0; i < 10; i++) {
            if (data->slots[i] == 0) {
                index = i;
            }
        }
        data->slots[index] = data->number; // save the value to factorise in the slot
        data->number = index;
        data->clientFlag = false;
        return index;
    }

    /**
     * Built in waiting time for writing slot - can get stuck if its impossible!
     * TODO: Check if there's a way to stop that
     * @param index
     * @param value
     */
    void writeSlot(int index, unsigned long value) {
        pthread_mutex_lock(&(data->locks[index])); // non busy wait
        while (data->serverFlag[index]) {
            usleep(1);
        }
        // server function
        data->serverFlag[index] = true;
        data->slots[index] = value;
        usleep(100); // its too fast so sleep a bit
        pthread_mutex_unlock(&(data->locks[index]));
    }

    /**
     * The actual information shared between the server and client
     */
    struct SharedData {
        long int number{0};
        bool clientFlag{false};

        unsigned long slots[10]{};
        int progress[10]{0};
        bool serverFlag[10]{false};

        pthread_mutex_t locks[10];
    };

    SharedData *data;
private:

    key_t key;
    int shmid;
};

SharedMemory GlobalSM;

class Factorizor {
public:
    Factorizor(unsigned long number, int memoryIndex) {
        // Launch up 32 threads
        GlobalSM.data->progress[memoryIndex] = 0;
        for (int i = 0; i < 32; i++) {
            // Right rotated -> 0000001 goes to 100000000 and 0000010 goes to 000000001
            // (don't count the zeros... just see the idea)
            auto numberToFactoriseInThread = (number << i) |
                                             (number >> ((sizeof(number) * 8) - i));
            ThreadData *td = new ThreadData{numberToFactoriseInThread, memoryIndex};
            pthread_t pthread;
            pthread_create(&pthread, nullptr,
                           &Factorizor::factoriseThread,
                           (void *) td);
            // let the thread know it can just break off and die
            usleep(100); // put a little time gap in between
            if (pthread_detach(pthread) != 0) {
                perror("An error occurred : ");
            }
        }
    }

private:
    struct ThreadData {
        unsigned long numToFactorise;
        int memoryIndex; // Used for mutex and shared memory
    };

    static void *factoriseThread(void *ptr) {
        auto &tD = *(ThreadData *) ptr;
        printf("My goal is to factorise %lu\n", tD.numToFactorise);
        for (auto i = 1; i <= tD.numToFactorise; i++) {
            if (tD.numToFactorise % i == 0) {
                // Our mutexes are handled internally in this write function
                printf("%lu: Writing %lu\n", tD.numToFactorise, i);
                GlobalSM.writeSlot(tD.memoryIndex, i);
            }
        }

        GlobalSM.data->progress[tD.memoryIndex]++;
        printf("\t\tFinished %lu on thread %d - progress == %d\n",
               tD.numToFactorise,
               tD.memoryIndex,
               GlobalSM.data->progress[tD.memoryIndex]);

        if (GlobalSM.data->progress[tD.memoryIndex] == 32) { // Check if everything is done
            printf("Done!\n");
            GlobalSM.writeSlot(tD.memoryIndex, 0); // let it know we're done
        }

        delete &tD; // This was allocated so we need to free it
        return nullptr; // They detached so we just have to do this
    }
};

void manageQuery(int index) {
    // The number to factorise is currently loaded into the slot
    printf("Making factorisor\n");
    unsigned long numberToFactorise = GlobalSM.data->slots[index];
    auto f = Factorizor(numberToFactorise, index);
}

/**
 * Prints the starting message
 */
void startingMessage() {
    cout << "##########################################################################\n"
            "░░░██╗░██╗░	░█████╗░░█████╗░██╗██╗░░░██╗░██████╗	░░░██╗░██╗░\n"
            "██████████╗	██╔══██╗██╔══██╗██║██║░░░██║██╔════╝	██████████╗\n"
            "╚═██╔═██╔═╝	██║░░██║██║░░╚═╝██║██║░░░██║╚█████╗░	╚═██╔═██╔═╝\n"
            "██████████╗	██║░░██║██║░░██╗██║██║░░░██║░╚═══██╗	██████████╗\n"
            "╚██╔═██╔══╝	╚█████╔╝╚█████╔╝██║╚██████╔╝██████╔╝	╚██╔═██╔══╝\n"
            "░╚═╝░╚═╝░░░	░╚════╝░░╚════╝░╚═╝░╚═════╝░╚═════╝░	░╚═╝░╚═╝░░░\n"
            "###########   ---------- Nicolaas van der Merwe ----------     ###########\n";
}


int main(int argc, char *argv[]) {
    GlobalSM.setToDefault(); // Make sure everything is default at the start. It
    // could be reusing previous values
    startingMessage();
    while (1) {
        printf("Waiting for reservations...\n");
        int index = GlobalSM.waitForReservation();
        if(index == -1) continue; // it was rejected
        printf("The index is currently %d\n", index);
        printf("Received a reservation. Now we need to make "
               "a factorizor and split away\n");
        manageQuery(index);
    }
}
