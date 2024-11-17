#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool thread1Done = false;

void countUp() {
    std::unique_lock<std::mutex> lock(mtx);
    for (int i = 1; i <= 20; ++i) {
        std::cout << "Thread 1 counting up: " << i << std::endl;
    }
    thread1Done = true;
    cv.notify_one(); // this is to give notice to the second thread. This critical for arranging 
    // execution of two threads so the sequence works correctly.
}

void countDown() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [] { return thread1Done; }); // waiting for thread1 to finish, this part of the code
    // provides definition of countDown and exhibits how the second thread should function
    for (int i = 20; i >= 0; --i) {
        std::cout << "Thread 2 counting down: " << i << std::endl;
    }
}

int main() {
    std::thread t1(countUp);
    std::thread t2(countDown);

    t1.join();
    t2.join();

    return 0;
}




