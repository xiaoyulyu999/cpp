#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <condition_variable>
#include<queue>

std::queue<int> q;
std::condition_variable cv;
std::mutex mutex;

void producer() {
    for (int i = 0; i < 10; ++i) {
        {
            std::unique_lock<std::mutex> ul(mutex);
            q.push(i);
            std::cout << "we pushing "<< i << std::endl;
        }
        cv.notify_one();
    }
}

void consumer() {
    for (int i = 0; i < 10; ++i) {
        std::unique_lock<std::mutex> ul(mutex);
        cv.wait(ul, [] { return !q.empty(); });//wait until queue is not empty(condition is true)
        int item = q.front();
        q.pop();
        ul.unlock();
        std::cout << "Consumed: " << item << std::endl;
    }
}


int main () {
    producer();
    consumer();
    return 0;
}