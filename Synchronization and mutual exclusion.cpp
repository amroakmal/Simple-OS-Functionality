#include <bits/stdc++.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <ext/pb_ds/assoc_container.hpp> // Common file
#include <ext/pb_ds/tree_policy.hpp> // Including tree_order_statistics_node_update

using namespace std;
using namespace __gnu_pbds;

template<typename key> using ordered_set = tree <key, null_type, less<key>, rb_tree_tag, tree_order_statistics_node_update>;

vector< pthread_t > threads;
pthread_t monitorThread, collectorThread;
int threadCount, counter, buffSize;
deque< int > buffer;
sem_t counterSem, s, e, n;

void* mutualExeclusionForGlobalVariable(void* args) {
    while(1) {
        sleep(rand() % 13);
        int* currentThreadNumber = (int*) args;
        printf("Counter thread: %d, received a message\n", *currentThreadNumber);
        int* currentSValue = (int*) malloc(sizeof (int));
        sem_getvalue(&counterSem, currentSValue);
        if(*currentSValue <= 0) {
            printf("Counter thread: %d, waiting to write\n", *currentSValue);
        }
        sem_wait(&counterSem);
        counter++;
        printf("Counter thread: %d,now adding to counter, counter value = %d\n", *currentThreadNumber, counter);
        sem_post(&counterSem);
    }
}

void* monitorFunction(void* args) {
    while(1) {
        sleep(rand() % 13);
        sem_wait(&counterSem);
        int currentGloablValue = counter;
        counter = 0;
        sem_post(&counterSem);
        printf("Monitor thread: reading a count of value = %d\n", currentGloablValue);
        int* currentSValue = (int*) malloc(sizeof (int));
        sem_getvalue(&e, currentSValue);
        if(*currentSValue <= 0) {
            printf("No empty places available in the buffer\n");
        }
        sem_wait(&e);
        sem_wait(&s);
        buffer.push_back(currentGloablValue);
        printf("Moniter thread is writing to the buffer at position: %d\n", buffer.size());
        sem_post(&s);
        sem_post(&n);
    }
}

void* collectorFunction(void* args) {
    while(1) {
        sleep(rand() % 13);
        int* currentSValue = (int*) malloc(sizeof (int));
        sem_getvalue(&n, currentSValue);
        if(*currentSValue <= 0) {
            printf("Collector thread: nothing is in the buffer\n");
        }
        sem_wait(&n);
        sem_wait(&s);
        buffer.pop_front();
        printf("Reading from buffer at position: %d\n", buffer.size());
        sem_post(&s);
        sem_post(&e);
    }
}

int main () {
   printf("Please enter wanted number of threads:\n");
   scanf("%d", &threadCount);
   printf("Please enter buffer size:\n");
   scanf("%d", &buffSize);
   threads.resize(threadCount + 1);
   sem_init(&counterSem, 0, 1);
   for(int i = 1; i <= threadCount; i++) {
        int* ptr = (int*) malloc(sizeof (int));
        *ptr = i;
        pthread_create(&threads[i], NULL, mutualExeclusionForGlobalVariable, ptr);
   }
   sem_init(&s, 0, 1);
   sem_init(&n, 0, 0); // current buffer capacity
   sem_init(&e, 0, buffSize); // number of empty cells on the buffer
   pthread_create(&monitorThread, NULL, monitorFunction, NULL);
   pthread_create(&collectorThread, NULL, collectorFunction, NULL);
   for(int i = 1; i <= threadCount; i++) {
        pthread_join(threads[i], NULL);
   }
   pthread_join(monitorThread, NULL);
   pthread_join(collectorThread, NULL);
   return 0;
}
