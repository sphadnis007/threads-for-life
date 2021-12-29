// Problem statement - N number of readers can read same file concurrently
// Only one writer can write at a time and no reader should be reading at
// that point of time.

#include <iostream>
#include <thread>
#include <semaphore>
#include <chrono>
#include <mutex>

#define NUM_OF_READERS 2
#define NUM_OF_WRITERS 1

std::binary_semaphore my_write(1);

bool done = false;

int num_of_reads = 0;
std::mutex protectRead;

void reader_func() {
    while(!done) {

        {
            // a binary semaphore can be used instead of this mutex too
            std::lock_guard<std::mutex> l(protectRead);
            num_of_reads++;
            if(num_of_reads == 1) {
                my_write.acquire();
            }
        }

        std::cout << std::this_thread::get_id() << " is reading the file \n";

        {
            std::lock_guard<std::mutex> l(protectRead);
            num_of_reads--;
            if(num_of_reads == 0) {
                my_write.release();
            }
        }

    }
}

void writer_func() {
    while(!done) {
        my_write.acquire();
        std::cout << std::this_thread::get_id() << " is writing to the file " << num_of_reads << std::endl;
        my_write.release();
    }
}

int main() {
    std::thread readers[NUM_OF_READERS];
    std::thread writers[NUM_OF_WRITERS];

    for(int i = 0; i < NUM_OF_READERS; i++) {
        readers[i] = std::thread(reader_func);
    }

    for(int i = 0; i < NUM_OF_WRITERS; i++) {
        writers[i] = std::thread(writer_func);
    }

    // sleep - then send done
    std::this_thread::sleep_for(std::chrono::nanoseconds(40));
    done = true;


    for(int i = 0; i < NUM_OF_READERS; i++) {
        readers[i].join();
    }

    for(int i = 0; i < NUM_OF_WRITERS; i++) {
        writers[i].join();
    }

    std::cout << "Done processing \n";

    return 0;

}