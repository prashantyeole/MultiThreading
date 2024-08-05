#include<iostream>
#include<thread>
#include<mutex>
#include<chrono>
unsigned long long oddSum = 0;
unsigned long long evenSum = 0;
int count = 0;

std::mutex mtx;

void sumOdd_function(unsigned long long start, unsigned long long end) {
    //      std::cout<< "Inside sumOdd Function" << std::endl;
    for (unsigned long long i = start; i < end; i++) {
        if ((i & 1) != 0) {
            oddSum += i;
        }
    }
}
void sumEven_function(unsigned long long start, unsigned long long end) {
    //      std::cout<< "Inside sumEven Function" << std::endl;
    for (unsigned long long i = start; i < end; i++) {
        if ((i & 1) == 0) {
            evenSum += i;
        }
    }
}
void incrementbyALakh() {
    //      std::cout<< "Inside incrementByALakh " << std::endl;
    for (int i = 0; i < 100000; i++) {
        if (mtx.try_lock()) {
            count++;
            mtx.unlock();
        }
    }
}
int main() {
    unsigned long long start{ 0 }, end{ 1999999999 };
    auto startTime = std::chrono::high_resolution_clock::now();
    sumOdd_function(start, end);
    sumEven_function(start, end);
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds> (endTime - startTime);
    std::cout << "Bfore Threading Time taken is: " << duration.count() / 100000 << " usec." << std::endl;

    startTime = std::chrono::high_resolution_clock::now();
    std::thread t1(sumOdd_function, start, end);
    std::thread t2(sumEven_function, start, end);
    t1.join();
    t2.join();
    endTime = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds> (endTime - startTime);
    std::cout << "With Threadming Time taken is: " << duration.count() / 100000 << " usec." << std::endl;
    //      std::cout << "Sum of Even = " << evenSum << std::endl;
    //      std::cout << "Sum of Odd  = " << oddSum << std::endl;

    std::thread t3(incrementbyALakh);
    std::thread t4(incrementbyALakh);
    if (t3.joinable()) {
        t3.join();
    }
    if (t4.joinable()) {
        t4.join();
    }
    std::cout << "Count is: " << count << std::endl;
    return 0;
}
