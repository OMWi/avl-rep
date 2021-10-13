#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

using namespace std;

class Queue
{
public:
    void push(uint8_t val) {
        unique_lock<mutex> lck(mtx);
        if(curSize == maxSize) cv.wait(lck);
        vals.push(val);
        curSize++;
    }

    bool pop(uint8_t& val) {
        unique_lock<mutex> lck(mtx);
        if (vals.empty()) {
            this_thread::sleep_for(chrono::milliseconds(1));
        }
        if (!vals.empty()) {
            val = vals.front();
            vals.pop();
            curSize--;
            cv.notify_one();
            return true;
        }
        return false;
    }

    void setSize(int size) {
        if (size > 0) {
            maxSize = size;
        }
    }
private:
    queue<uint8_t> vals;
    mutex mtx;
    condition_variable cv;
    int curSize = 0, maxSize = 1;
};

int main() {
    Queue q;  
    int consumerNum[]{1, 2, 4};
    int producerNum[]{1, 2, 4};
    int taskNum = 4*1024*1024;  
    int qSizes[]{1, 4, 16};

    q.setSize(16);
    int pCount = 0, cCount = 0;

       
    // for (int k = 0; k < 3; k++) {
    //     q.setSize(qSizes[k]);
        for (int i = 0; i < 3; i++) {
            int consumerCount[consumerNum[i]]{};        

            cout << "Producer num:" << producerNum[i] << endl;
            cout << "Consumer num:" << consumerNum[i] << endl;

            thread* producers = new thread[producerNum[i]];
            thread* consumers = new thread[consumerNum[i]];

            auto start = chrono::high_resolution_clock::now();

            for (int j = 0; j < producerNum[i]; j++) {
                producers[j] = thread([taskNum, &q, &pCount](){
                    for (int amount = 0; amount < taskNum; amount++) {
                        q.push(1);
                        cout << "prod " << ++pCount << endl;
                    }
                });
                consumers[j] = thread([j, &consumerCount, &q, &cCount](){
                    uint8_t val;
                    while (q.pop(val)) {
                        consumerCount[j] += val;
                        cout << "cons " << ++cCount << endl;
                    }
                });
            }
            for (int j = 0; j < producerNum[i]; j++) {
                producers[j].join();
                consumers[j].join();
            }

            auto end = chrono::high_resolution_clock::now();
            chrono::duration<float> duration = end - start;
            delete[] producers;
            delete[] consumers;

            cout << "Consumer count: ";
            for (int j = 0; j < consumerNum[i]; j++) {
                cout << consumerCount[j] << ' ';
            }
            cout << endl;
            int sum = 0;
            for (int j = 0; j < consumerNum[i]; j++) {
                sum += consumerCount[j];
            }
            cout << "Consumer count sum: " << sum << endl;
            if (sum == taskNum*producerNum[i]) {
                cout << "Count sum correct" << endl;
            }
            else {
                cout << "Count sum incorrect" << endl;
            }
            cout << "Elapsed time " << duration.count() << "s" << endl;
            cout << endl;
        }        
    // }  
}