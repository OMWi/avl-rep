#include <stdlib.h>
#include <thread>
#include <mutex>
#include <vector>

using namespace std;

mutex mtx;
int thrIndex;
bool ready = true;

void foo(vector<int8_t>& arr, int size) {
    while (true) {
        mtx.lock();
        if (thrIndex >= size) {
            mtx.unlock();
            break;
        }
        int curIndex = thrIndex++;
        mtx.unlock();
        arr.at(curIndex)++;    
        // this_thread::sleep_for(chrono::nanoseconds(1));    
    }
    // printf("thread %d stopped\n", this_thread::get_id());
}

void task(int numTasks, int numThreads) {
    printf("Number of threads %d\n", numThreads);
    thread threads[numThreads];

    vector<int8_t> arr(numTasks, 0);
    thrIndex = 0;
    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < numThreads; i++) {
        threads[i] = thread([&arr, numTasks]() {
            foo(arr, numTasks);
        });
    }
    for (int i = 0; i < numThreads; i++) {
        threads[i].join();
    }

    auto end = chrono::high_resolution_clock::now();    
    chrono::duration<float> duration = end - start;
    printf("Elapsed time %f s\n", duration.count());

    bool arrayError = false;
    for (int i = 0; i < numTasks; i++) {
        if (arr[i] != 1) {
            arrayError = true;
            break;
        }
    }
    if (arrayError) {
        printf("Array error\n");            
    }
    else {
        printf("Array correct\n");
    }
    printf("\n");
}

int main() {
    int numTasks = 1024*1024;
    int numThreads[4]{4, 8, 16, 32};

    for (int i = 0; i < 4; i++) {
        task(numTasks, numThreads[i]);
    }
}