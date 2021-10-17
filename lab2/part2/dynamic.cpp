#include <queue>
#include <thread>
#include <mutex>
#include <stdio.h>

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

void task(int consumerNum, int producerNum, int taskNum) {
    printf("Producer num: %d\n", producerNum);
    printf("Consumer num: %d\n", consumerNum);

    Queue q;    
    thread producers[producerNum];
    thread consumers[consumerNum];
    int consumerCount[consumerNum]{0};

    auto start = chrono::high_resolution_clock::now();

    for (int j = 0; j < producerNum; j++) {
        producers[j] = thread([taskNum, &q](){
            // printf("producer start\n");
            for (int amount = 0; amount < taskNum; amount++) {
                q.push(1);
                // printf("push\n");
            }
        });
    }
    for (int j = 0; j < consumerNum; j++) {
        consumers[j] = thread([j, &consumerCount, &q](){
            // printf("consumer start\n");
            uint8_t val;
            while (q.pop(val)) {
                consumerCount[j] += val;
                // printf("pop\n");
            }
        });
    }
    for (int j = 0; j < producerNum; j++) {
        producers[j].join();
    }
    for (int i = 0; i < consumerNum; i++) {
        consumers[i].join();
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<float> duration = end - start;
    printf("Consumer count: ");
    for (int j = 0; j < consumerNum; j++) {
        printf("%d ", consumerCount[j]);
    }
    printf("\n");
    int sum = 0;
    for (int j = 0; j < consumerNum; j++) {
        sum += consumerCount[j];
    }
    printf("Consumer count sum: %d\n", sum);
    if (sum == taskNum*producerNum) {
        printf("Count sum correct\n");
    }
    else {
        printf("Count sum incorrect\n");
    }
    printf("Elapsed time %f s\n", duration.count());
}

int main() {
    int consumerNum[]{1, 2, 4};
    int producerNum[]{1, 2, 4};
    int taskNum = 1024*1024;      
    
    // for (int i = 0; i < 3; i++) {
    //     for (int j = 0; j < 3; j++) {
    //         task(consumerNum[j], producerNum[i], taskNum);
    //         printf("\n");
    //     }
    // }

    for (int i = 0; i < 3; i++) {
        task(consumerNum[i], producerNum[i], taskNum);
        printf("\n");
    }
}