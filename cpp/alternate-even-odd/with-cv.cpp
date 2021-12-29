#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex m;
std::condition_variable cv;

int number = 1;

void print_odd() {
    while(number < 10) {
        {
            std::unique_lock<std::mutex> l(m);
            cv.wait(l, []{return (number % 2 != 0);});
            std::cout << "t1 prints " << number << std::endl;
            number++;
        }
        // It is possible/allowed to call notify when the lock on the
        // mutex is still held. But that would harm our performance for
        // this use case. As, when we notify, we still have the lock, so
        // the other thread will still have to wait till we leave the lock
        // even when its predicate's condition is true.
        cv.notify_one();
    }
}

void print_even() {
    while(number < 11) {
        {
            std::unique_lock<std::mutex> l(m);
            cv.wait(l, []{return number % 2 == 0;});
            std::cout << "t2 prints " << number << std::endl;
            number++;
        }
        cv.notify_one();
    }
}

int main() {
    std::thread t1(print_odd);
    std::thread t2(print_even);

    cv.notify_one();

    t1.join();
    t2.join();

    std::cout << "Finished Job \n";

    return 0;
}