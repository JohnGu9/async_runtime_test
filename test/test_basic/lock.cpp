#include <chrono>
#include <ctime>
#include "async_runtime/object.h"
#include "async_runtime/fundamental/async.h"

static inline void currentTime()
{
    using namespace std;
    auto timenow = chrono::system_clock::to_time_t(chrono::system_clock::now());
    cout << ctime(&timenow) << endl;
}

int main()
{
    ref<ThreadPool> threadPool = Object::create<ThreadPool>(6);
    ref<Lock> lock = Object::create<Lock>();
    threadPool->post([lock] {
        auto sharedLock = lock->sharedLock(); // done at 0s
        std::cout << "Lock::SharedLock#0 for 3 second. Begin at ";
        currentTime();
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout << "Lock::SharedLock#0 released at ";
        currentTime();
        // release at 3s
    });
    threadPool->post([lock] {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        auto sharedLock = lock->sharedLock(); // done at 1s
        std::cout << "Lock::SharedLock#1 for 3 second. Begin at ";
        currentTime();
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout << "Lock::SharedLock#1 released at ";
        currentTime();
        // release at 4s
    });
    threadPool->post([lock] {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "Lock::UniqueLock#0 (try to lock) begin at ";
        currentTime();
        auto uniqueLock = lock->uniqueLock(); // done at 4s
        std::cout << "Lock::UniqueLock#0 for 3 second. Locked at ";
        currentTime();
        std::this_thread::sleep_for(std::chrono::seconds(4));
        std::cout << "Lock::UniqueLock#0 released at ";
        currentTime();
        // release at 8s
    });
    threadPool->post([lock] {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        std::cout << "Lock::SharedLock#2 (try to lock) begin at ";
        currentTime();
        auto sharedLock = lock->sharedLock(); // done at 8s
        std::cout << "Lock::SharedLock#2 for 3 second. Locked at ";
        currentTime();
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout << "Lock::SharedLock#2 released at ";
        currentTime();
        // release at 11s
    });
    threadPool->post([lock] {
        std::this_thread::sleep_for(std::chrono::seconds(6));
        std::cout << "Lock::UniqueLock#1 (try to lock) begin at ";
        currentTime();
        auto uniqueLock = lock->uniqueLock(); // done at 11s
        std::cout << "Lock::UniqueLock#1 for 3 second. Locked at ";
        currentTime();
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout << "Lock::UniqueLock#1 released at ";
        currentTime();
        // release at 14s
    });
    threadPool->post([lock] {
        std::this_thread::sleep_for(std::chrono::seconds(7));
        std::cout << "Lock::SharedLock#3 (try to lock) begin at ";
        currentTime();
        auto sharedLock = lock->sharedLock(); // done at 14s
        std::cout << "Lock::SharedLock#3 for 3 second. Locked at ";
        currentTime();
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout << "Lock::SharedLock#3 released at ";
        currentTime();
        // release at 17s
    });
    threadPool->dispose();
    return EXIT_SUCCESS;
}