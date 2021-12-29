#include <iostream>
#include <thread>

// This approach will work in any case.
// It is better than having a mutex lock on "print_odd" global
//   -  because, if we have a mutex, it is not guranteed that the lock will
//      be acquired by the two threads alternately. If t1 releases the lock
//      in the if condition(critical-section), t1 can acquire it again before
//      t2 and this can continue. Not a desirable state.

// Drawbacks of this approach
// 1. One thread keeps looping on that if condition check till the other one
//    changes both the vars - numbers and print_odd - waste of CPU.
//    -> Solution would be to use binary semaphor - in that case, the looping thread
//       will sleep until it gets signal from the other thread and thereby not wasting
//       the CPU resources.

int number = 1;

void t1_odd() {
    while(number < 10) {
        if(number % 2 != 0) {
            std::cout << "t1 prints " << number << std::endl;
            number++;
        }
    }
}

void t2_even() {
    while (number < 11) {
        if(number % 2 == 0) {
            std::cout << "t2 prints " << number << std::endl;
            number++;
        }
    }
    
}

int main() {
    std::thread t1(t1_odd);
    std::thread t2(t2_even);

    std::cout << "In Main \n";

    // waiting for threads to finish their execution
    t1.join();
    t2.join();

    std::cout << "Threads processing done \n";

    return 0;
}
