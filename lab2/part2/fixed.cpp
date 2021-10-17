#include <stdio.h>
#include <thread>
#include <mutex>
#include <atomic>
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
        unique_lock<mutex> lock(mtx);
        // if (vals.size() == maxSize) {
        //     cv.wait(lock);
        // }
        cv.wait(lock, [this]{ return vals.size() < maxSize;});
        // printf("push notified\n");
        vals.push(val);
    }

    bool pop(uint8_t& val) {
        unique_lock<mutex> lock(mtx);
        if (vals.empty()) {
            lock.unlock();
            cv.notify_one();
            this_thread::sleep_for(chrono::milliseconds(1));
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

    void setSize(int newSize) {
        if (newSize > 0) maxSize = newSize;
    }

    int getSize() {
        return vals.size();
    }
};

atomic<int> producerCount;
void produce(int taskNum, uint8_t val, Queue* q) {
    // printf("p+ ");
    for (int i = 0; i < taskNum; i++) {
        q->push(val);
    }
    producerCount--;
    // printf("p- ");
}

int consumerCount[4];
void consume(Queue* q, int consumerIndex) {
    // printf("c+ ");
    uint8_t val = 0;
    while(true) {
        if (producerCount > 0) {
            if (q->pop(val)) {
                consumerCount[consumerIndex] += val;
            }
        }
        else if (q->pop(val)) {
            consumerCount[consumerIndex] += val;
        }
        else {
            break;
        }
    }
    // printf("c- ");
}
void nulify() {
    for (int i = 0; i < 4; i++) {
        consumerCount[i] = 0;
    }
}

bool work(int producerNum, int consumerNum, int taskNum, int queueSize) {
    Queue* q = new Queue();
    q->setSize(taskNum*10);

    thread consumers[consumerNum];
    thread producers[producerNum];
    producerCount = producerNum;
    nulify();

    printf("Queue:%d; consumers:%d; producers:%d\n", queueSize, consumerNum, producerNum);

    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < producerNum; i++) {
        producers[i] = thread(produce, taskNum, 1, ref(q));
    }  
    for (int i = 0; i < consumerNum; i++) {
        consumers[i] = thread(consume, ref(q), i);
    }
    for (int i = 0; i < producerNum; i++) {
        producers[i].join();
    }    
    for (int i = 0; i < consumerNum; i++) {
        consumers[i].join();
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<float> duration = end - start;  
    printf("q size: %d\n", q->getSize());

    printf("Consumer count: ");
    for (int i = 0; i < consumerNum; i++) {
        printf("%d ", consumerCount[i]);
    }
    printf("\n");
    int sum = 0;
    for (int i = 0; i < consumerNum; i++) {
        sum += consumerCount[i];
    }
    printf("Consumer sum: %d ", sum);
    printf(sum == consumerNum*taskNum ? "correct\n" : "false(%d)\n", consumerNum*taskNum);
    printf("Elapsed time %f s\n", duration.count());
    delete q;
    return sum == consumerNum*taskNum;
}

int main() {
    int producerNum[]{1, 2, 4};
    int consumerNum[]{1, 2, 4};
    int taskNum = 1024*1024*4;
    printf("Task num: %d\n\n", taskNum);
    int queueSize[]{1, 4, 16};
    
    int testNum = 100;
    for (int testIndex = 0; testIndex < testNum; testIndex++) {
        printf("Test %d\n", testIndex+1);
        for (int i = 0; i < 3; i++) {
            bool res = work(producerNum[i], consumerNum[i], taskNum, queueSize[i]);
            printf("\n");
            if (!res) {
                printf("test failed\n");
                return -1;
            }
        }
    }
}