#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

// Not an efficient code. Producer and Consumer are picked up
// randomely and processing(produce/consume) is done. But this
// code works in a similar fashion for semaphore implementation
// too. There too, the producer will keep producing until it
// reaches buffer limit OR it gets swaped with consumer in cpu.
// Consumer will keep consuming until buffer is empty or it gets
// swapped with producer thread in cpu.

// Ideally, the producer-consumer problem statement has a
// buffer which they share to use the resources. Here, i have kept
// size limit of 10 instead of using an array as buffer.
#define BUFFER_SIZE 10

std::mutex m;
std::condition_variable cv;

int products = 0;
bool done = false;

void producer() {
    while(!done) {
        {
            std::unique_lock<std::mutex> l(m);
            cv.wait(l, []{ return products >= 0 && products < BUFFER_SIZE; });
            products++;
            std::cout << "produced new product. count = " << products << std::endl;
        }
        cv.notify_one();
    }
}

void consumer() {
    while(!done) {
        {
            std::unique_lock<std::mutex> l(m);
            cv.wait(l, []{ return products > 0; });
            products--;
            std::cout << "consumed. count = " << products << std::endl;
        }
        cv.notify_one();
    }
}

int main() {
    std::thread t1(producer);
    std::thread t2(consumer);

    // signal to stop after some sleep
    std::this_thread::sleep_for(std::chrono::nanoseconds(1));
    done = true;

    t1.join();
    t2.join();

    std::cout << "final count = " << products << std::endl;

    return 0;
}
