#include <deque>
#include <thread>
#include <functional>
#include <utility>
#include <mutex>
#include <condition_variable>

template<class... Params>
class Pool;

template<class... Args>
class Worker {
public:
    void AddTask(std::function<void(Args...)>& fun, Args... arg) { 
        std::lock_guard<std::mutex> lock(queueMutex);
        queue.push_back(std::pair<std::function<void(Args...)>&, Args...>(fun, arg...)); 
        this->Notify();
    }
    
    void Stop() {
        std::lock_guard<std::mutex> lock(queueMutex); //td
        doStopThread = true; 
        this->Notify();
    }
    
    Worker() : isNotified(false), doStopThread(false) { thr = std::thread(&Worker::Loop, this); }
    
    ~Worker() { thr.join(); }

private:
    std::deque<std::pair<std::function<void(Args...)>&, Args...>> queue;
    std::thread thr;
    std::mutex queueMutex;
    std::condition_variable cv;
    bool isNotified;
    bool doStopThread;

    void Notify() {
        isNotified = true;
        cv.notify_one();
    }

    void Loop() { 
        while(!doStopThread) {
            std::unique_lock<std::mutex> lk(queueMutex);
            if(queue.empty()) {
                while(!isNotified) {
                    cv.wait(lk);
                }
                if(doStopThread) {
                    return;
                }
            }
            auto task = queue.front();
            queue.pop_front();
            lk.unlock();
            isNotified = false;
            task.first(task.second);
        }
    }
    
    template<class... Params>
    friend class Pool;
  
}; 
