#include <iostream>
#include <thread>
#include <semaphore>
#include <chrono>

// Couldn't find any way to define array of semaphore. Even if we declare,
// there was no way to initialize it to 0/1 using loop as it is not
// CopyConstructible like we can do with threads -
// std::thread t[x];
// for(int i = 0; i < x; i++) { t[i] = std::thread(func, args...); }
// So manually declared chop1/2/3/4/5 and t1/2/3/4/5.

std::binary_semaphore chop1(1), chop2(1), chop3(1), chop4(1), chop5(1);
bool done = false;

// pid = philosopher's id
void process(int pid, std::binary_semaphore &c1, std::binary_semaphore &c2) {
    while(!done) {
        c1.acquire();
        c2.acquire();
        
        // standard output is shared between multiple threads
        // so some prints may get mixed. To avoid this, we can
        // add a mutex to protect std::cout as a enhancement.
        std::cout << "Philosopher id " << pid << " is eating \n";
        
        c1.release();
        c2.release();
    }
}

int main() {
    // As semaphore is not CopyConstructible - sending it as a reference
    // using the std::ref library.
    std::thread t1(process, 1, std::ref(chop1), std::ref(chop2));
    std::thread t2(process, 2, std::ref(chop2), std::ref(chop3));
    std::thread t3(process, 3, std::ref(chop3), std::ref(chop4));
    std::thread t4(process, 4, std::ref(chop4), std::ref(chop5));
    // Last philosopher picks up chops in reverse direction than
    // others to avoid the deadlock situation.
    std::thread t5(process, 5, std::ref(chop1), std::ref(chop5));

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    done = true;

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    
    std::cout << "Done \n";
    
    return 0;
}
