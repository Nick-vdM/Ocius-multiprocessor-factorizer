//
// Created by nicol on 28/09/2020.
// https://www.linkedin.com/in/nick-vd-merwe/
// nick.jvdmerwe@gmail.com

#include <iostream>
#include <string>
#include <sys/shm.h>
#include <array>
#include <unistd.h>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

using namespace std;
#define STDIN 0
#define TERMINAL_WIDTH 120

class myClock {
public:
    void startTimer() {
        isCounting = true;
        clock_gettime(CLOCK_MONOTONIC, &START_TIME_SPEC);
    }

    bool isRunning() const { return isCounting; }

    double getSecondsAndDisable() {
        isCounting = false;
        clock_gettime(CLOCK_MONOTONIC, &END_TIME_SPEC);
        return (double) (END_TIME_SPEC.tv_sec - START_TIME_SPEC.tv_sec) +
               (double) (END_TIME_SPEC.tv_nsec - START_TIME_SPEC.tv_nsec) / 1e9;
    }

private:
    struct timespec START_TIME_SPEC, END_TIME_SPEC;
    bool isCounting;
};

class SharedMemory {
public:
    // client side
    SharedMemory() {
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
        printf("Connected! Number is currently %d. Setting it to 5\n",
               data->number);
        data->number = 5;
    }

    ~SharedMemory() {
        shmdt(data);
    }

    int requestReservation(unsigned long valueToFactorise) {
        // client function
        while (data->clientFlag) {
            usleep(1);
        }
        data->number = valueToFactorise;
        data->clientFlag = true;
        return true;
    }

    int grabReservationIndex() {
        while (data->clientFlag) {
            // reservation was not accepted - wait for it
            usleep(1);
        }
        return data->number; // reservation came through
    }

    long int readSlot(int index) {
        pthread_mutex_lock(&(data->locks[index])); // non-busy wait here
        while (!data->serverFlag[index]) { // double check the flag
            usleep(1);
        }
        // client function
        data->serverFlag[index] = false;
        auto toReturn = data->slots[index];
        pthread_mutex_unlocmmmmdata->locks[index]));
        return toReturn; // if its 0 then the thread is complete
    }

    /**
     * The actual information shared between the server and client
     */
    struct SharedData {
        long int number{0};
        bool clientFlag{false};

        unsigned long slots[10]{};
        int progress[10]{0};
        char serverFlag[10]{false};

        pthread_mutex_t locks[10];
    };

    SharedData *data;
private:
    key_t key;
    int shmid;
};

/**
 * Prints the starting message
 */
void startingMessage() {
    cout << "##########################################################################\n"
            "░░░██╗░██╗░		░█████╗░░█████╗░██╗██╗░░░██╗░██████╗		░░░██╗░██╗░\n"
            "██████████╗		██╔══██╗██╔══██╗██║██║░░░██║██╔════╝		██████████╗\n"
            "╚═██╔═██╔═╝		██║░░██║██║░░╚═╝██║██║░░░██║╚█████╗░		╚═██╔═██╔═╝\n"
            "██████████╗		██║░░██║██║░░██╗██║██║░░░██║░╚═══██╗		██████████╗\n"
            "╚██╔═██╔══╝		╚█████╔╝╚█████╔╝██║╚██████╔╝██████╔╝		╚██╔═██╔══╝\n"
            "░╚═╝░╚═╝░░░		░╚════╝░░╚════╝░╚═╝░╚═════╝░╚═════╝░		░╚═╝░╚═╝░░░\n"
            "###########   ---------- Nicolaas van der Merwe ----------     ###########"
            "\n\n\n\n\n\n"; // make space for all the announcements
    fflush(stdout);
}

SharedMemory GlobalSM;

// Line 1: Last completed query (four up)
// Line 2: Last factor found (three up)
// Line 3: Progress bar (two up)
// Line 4: Whether the user can input (one up)
// Line 5: Inputs (default position)

void readAnyNewFactors() {
    // Move up three, we'll move up again in the loop
    printf("\033[s" // save pos
           "\033[1A\033\033[1A"); // up up - we'll move another up again
    for (int i = 0; i < 10; i++) {
        if (!GlobalSM.data->serverFlag[i] || GlobalSM.data->slots[i] == 0) {
            continue; // there's nothing to read
        }
        printf("\033[1A\033[2K\r"); // reset the line to write
        unsigned long factorFound = GlobalSM.readSlot(i);
        printf("Factor found on slot %i : %lu\n", i, factorFound); // purposefully no newline
    }
    printf("\033[u"); // return to the original position
    fflush(stdout);
}

void clearWaitForQuery() {
    printf("\033[s\033[1A" // its one up
           "\033[2K\r"); // up a line and delete
    printf("\033[u");
    fflush(stdout);
}

void sayQueryFinished(int queryID, double secondToFinish) {
    // Save position, move up three lines and wipe the line
    printf("\033[s"
           "\033[1A\033[1A\033[1A\033[1A"
           "\033[2K\r");
    printf("Query %d finished : Took %lf seconds\n", queryID, secondToFinish);
    printf("\033[u");
    fflush(stdout);
    clearWaitForQuery();
}

void showProgressBar() {
    // Move up two lines
    printf("\033[s"
           "\033[1A\033[1A"
           "\033[2K\r");
    printf(">Progress:\t");
    for (int i = 0; i < 10; i++) {
        if (GlobalSM.data->slots[i] == 0) {
            continue; // there's nothing useful there - its done
        }
        float progress = static_cast<float>(GlobalSM.data->progress[i]) / 32 * 100;
        printf("Q%d: %f%% ", i, progress);
        printf("|");
        int tags = GlobalSM.data->progress[i] / 8; // 32 max divided by four...
        // One for every 25%
        for (int i = 0; i < tags; i++) {
            printf("#");
        }
        for (int i = 0; i < 4 - tags; i++) {
            printf("-");
        }
        printf("|\t");
    }
    printf("\033[u");
    fflush(stdout);
}

void printWaitForQuery() {
    printf("\033[s\033[1A\033[2K\r"); // up a line and delete
    printf("No, the input is currently full\n");
    printf("\033[u");
}


void handleAllPrints(array<myClock, 10> &timers) {
    // First, check if any queries finished. We can tell if a timer
    // is running but the slot is set to 0
    for (int i = 0; i < 10; i++) {
        if (timers[i].isRunning() && GlobalSM.data->slots[i] == 0) {
            // This means that there is a timer running but the slot actually
            // finished
            sayQueryFinished(i, timers[i].getSecondsAndDisable());
        }
    }
    // Second, read any factors that need reading and report them
    readAnyNewFactors();
    // Thirdly, update the progress bars
    showProgressBar();
}


int main(int argc, char *argv[]) {
    // For now the first counts how many factors it found
    array<myClock, 10> timers{};
    startingMessage();
    unsigned long number;
    struct timeval timeout{0, 0}; // no wait
    fd_set fileDescriptors;
    while (true) {
        // Then lets print how the progress is going
        handleAllPrints(timers);

        FD_ZERO(&fileDescriptors);
        FD_SET(STDIN, &fileDescriptors);
        select(STDIN + 1, &fileDescriptors, NULL, NULL, &timeout);
        if (FD_ISSET(STDIN, &fileDescriptors)) {
            // The user is asking something
            scanf("%lu", &number);
            printf("\033[1A\033[2K\r"); // reset the position
            // empty out everything else
            GlobalSM.requestReservation(number);
            int index = GlobalSM.grabReservationIndex();
            if (index == -1) {
                printWaitForQuery();
            } else {
                timers[index].startTimer();
            }
        }
    }
}

#pragma clang diagnostic pop