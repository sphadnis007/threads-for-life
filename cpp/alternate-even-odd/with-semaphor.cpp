#include <iostream>
#include <semaphore>
#include <thread>

// we can use binary semaphore too like this -
// std::binary_semaphore sem1(0), sem2(0);
std::counting_semaphore<1> sem1(0), sem2(0);

int number = 1;

void print_odd() {
    while(number < 10) {
        sem1.acquire();
        std::cout << "t1 " << number << std::endl;
        number++;
        sem2.release();
    }
}

void print_even() {
    while(number < 11) {
        sem2.acquire();
        std::cout << "t2 " << number << std::endl;
        number++;
        sem1.release();
    }
}

int main() {
    std::thread t1(print_odd);
    std::thread t2(print_even);
    
    // This will signal the print_odd func, to start printing the
    // odd numbers.
    sem1.release();

    t1.join();
    t2.join();

    return 0;
}