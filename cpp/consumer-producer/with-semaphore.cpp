#include <iostream>
#include <thread>
#include <semaphore>
#include <chrono>

// semaphrores to track number of empty and full/filled
// slots of our resource/buffer array.
std::counting_semaphore<10> empty{10}, full{0};
// semaphore to lock the buffer - only one thread will
// be using the buffer at a time.
std::counting_semaphore<1> buffer{1};
// stop the 2 threads processing
bool done = false;
// count of number of resources
int count = 0;


// The first call will go to producer as empty is 10 and full is 0
// Set initial value for buffer as 1 - we don't have to trigger a
// signal from the main thread as we are sure that the control is
// going to which thread initially.
void producer() {
    while(!done) {
        empty.acquire();
        buffer.acquire();

        count++;
        std::cout << "produced one resource " << count << std::endl;

        buffer.release();
        full.release();
    }
}

void consumer() {
    while(!done) {
        full.acquire();
        buffer.acquire();

        count--;
        std::cout << "consumed one " << count << std::endl;

        buffer.release();
        empty.release();
    }
}


int main() {
    std::thread t1(producer);
    std::thread t2(consumer);

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    done = true;

    t1.join();
    t2.join();

    std::cout << "Job done \n";
    return 0;
}
