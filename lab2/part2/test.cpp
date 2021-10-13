#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

using namespace std;

class Queue {
private:
    queue<uint8_t> vals;
    int maxSize = 1;
    mutex mtx;
    condition_variable cv;
public:
    void push(uint8_t val) {
        auto start = chrono::high_resolution_clock::now();

        unique_lock<mutex> lock(mtx);
        if (vals.size() == maxSize) {
            cv.wait(lock);
        }
        vals.push(val);

        auto end = chrono::high_resolution_clock::now();
        chrono::duration<float> duration = end - start;
        cout << "size(after push) " << vals.size() << "   elapsed time " << duration.count()*1000*1000 << " mcs" << endl;
    }
    bool pop(uint8_t& val) {
        unique_lock<mutex> lock(mtx);
        if (vals.empty()) {
            lock.unlock();
            cv.notify_one();
            this_thread::sleep_for(chrono::milliseconds(3));
            lock.lock();
        }
        if (!vals.empty()) {
            val = vals.front();
            vals.pop();
            lock.unlock();
            cv.notify_one();
            return true;
        }
        return false;
    }
    void setSize(int size) {
        if (size > 0) maxSize = size;
    }
};

void produce(int taskNum, uint8_t val, Queue* q) {
    for (int i = 0; i < taskNum; i++) {
        q->push(val);
    }
    cout << "producer thread " << this_thread::get_id() << " stopped" << endl;
}

int consumerCount[4]{0};
void consume(Queue* q, int consumerIndex) {
    uint8_t val;
    while(q->pop(val)){
        consumerCount[consumerIndex]++;
    }
    cout << "consumer thread " << this_thread::get_id() << " stopped" << endl;
}
void nulify() {
    for (int i = 0; i < 4; i++) {
        consumerCount[i] = 0;
    }
}

void work(int producerNum, int consumerNum, int taskNum, Queue* q) {
    thread consumers[consumerNum];
    thread producers[producerNum];
    cout << "Consumers number " << consumerNum << endl;
    cout << "Producers number " << producerNum << endl;

    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < producerNum; i++) {
        producers[i] = thread(produce, taskNum, 1, q);
    }
    for (int i = 0; i < consumerNum; i++) {
        consumers[i] = thread(consume, q, i);
    }

    for (int i = 0; i < producerNum; i++) {
        producers[i].join();
    }
    for (int i = 0; i < consumerNum; i++) {
        consumers[i].join();
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<float> duration = end - start;
    cout << "Elapsed time " << duration.count() << "s" << endl;

    cout << "Consumer count: ";
    for (int i = 0; i < consumerNum; i++) {
        cout << consumerCount[i] << ' ';
    }
    cout << endl;
    nulify();
}

int main() {
    int producerNum[]{1, 2, 4};
    int consumberNum[]{1, 2, 4};
    int taskNum = 32;
    int queueSize[]{1, 4, 16};
    Queue* q = new Queue();
    q->setSize(queueSize[1]);

    work(producerNum[0], consumberNum[0], taskNum, q);
}