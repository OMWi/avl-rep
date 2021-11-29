#include <stdio.h>
#include <thread>
#include <atomic>
#include <vector>

using namespace std;

atomic<int> thrIndex{0};

void foo(vector<int>& arr, int size) {
    int curIndex = thrIndex.fetch_add(1);
    while(curIndex < size) {        
        arr.at(curIndex)++;
        curIndex = thrIndex.fetch_add(1);
        // this_thread::sleep_for(chrono::nanoseconds(1));
    }
}

void task(int numTasks, int numThreads) {
    printf("Number of threads %d\n", numThreads);
    thread threads[numThreads];

    vector<int> arr(numTasks, 0);

    thrIndex = 0;
    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < numThreads; i++) {
        threads[i] = thread([&arr, numTasks](){
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

    for (int j = 0; j < 4; j++) {
        task(numTasks, numThreads[j]);
    }
}