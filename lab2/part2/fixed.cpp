#include <iostream>
#include <queue>
#include <thread>
#include <mutex>

using namespace std;

class Queue 
{
public:
    void push(uint8_t val) {
        mtx.lock();
        vals.push(val);
        mtx.unlock();
    }
    bool pop(uint8_t& val) {
        mtx.lock();
        if (vals.empty()) {
            mtx.unlock();
            this_thread::sleep_for(chrono::milliseconds(1));
            mtx.lock();
        }
        if (!vals.empty()) {
            val = vals.front();
            vals.pop();
            mtx.unlock();
            return true;
        }
        mtx.unlock();
        return false;
    }
private:
    queue<uint8_t> vals;
    mutex mtx;
};

int main() {
    Queue q;  
    int consumerNum[]{1, 2, 4};
    int producerNum[]{1, 2, 4};
    int taskNum = 4*1024*1024;      
    
    for (int i = 0; i < 3; i++) {
        int consumerCount[consumerNum[i]]{};        

        cout << "Producer num:" << producerNum[i] << endl;
        cout << "Consumer num:" << consumerNum[i] << endl;

        thread* producers = new thread[producerNum[i]];
        thread* consumers = new thread[consumerNum[i]];

        auto start = chrono::high_resolution_clock::now();

        for (int j = 0; j < producerNum[i]; j++) {
            producers[j] = thread([taskNum, &q](){
                for (int amount = 0; amount < taskNum; amount++) {
                    q.push(1);
                }
            });
            consumers[j] = thread([j, &consumerCount, &q](){
                uint8_t val;
                while (q.pop(val)) {
                    consumerCount[j] += val;
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
}