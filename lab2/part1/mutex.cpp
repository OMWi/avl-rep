#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

mutex mtx;
int thrIndex;

void foo(int8_t* arr, int size) {
    while (thrIndex < size) {
        mtx.lock();
        int curIndex = thrIndex++;
        mtx.unlock();
        arr[curIndex]++;
        this_thread::sleep_for(chrono::nanoseconds(10));
    }
}

int main() {
    int numTasks = 1024*1024;
    int numThreads[4]{4, 8, 16, 32};

    for (int j = 0; j < 4; j++) {
        cout << "Number of threads - " << numThreads[j] << endl;
        thread* threads = new thread[numThreads[j]];

        int8_t arr[numTasks]{0};
        thrIndex = 0;
        auto start = chrono::high_resolution_clock::now();

        for (int i = 0; i < numThreads[j]; i++) {
            threads[i] = thread([&arr, numTasks](){
                foo(arr, numTasks);
            });
        }
        for (int i = 0; i < numThreads[j]; i++) {
            threads[i].join();
        }

        auto end = chrono::high_resolution_clock::now();    
        chrono::duration<float> duration = end - start;
        cout << "Elapsed time " << duration.count() << " s" << endl;

        bool arrayError = false;
        for (int i = 0; i < numTasks; i++) {
            if (arr[i] != 1) {
                arrayError = true;
                break;
            }
        }
        if (arrayError) {
            cout << "Array error" << endl;
        }
        else {
            cout << "Array correct" << endl;
        }
        cout << endl;
        delete[] threads;
    }
}