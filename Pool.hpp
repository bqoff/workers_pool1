#include <functional>
#include <vector>
#include <string>
#include <map>
#include "Worker.hpp"

namespace pmt {
    typedef int ManagerType;
    const ManagerType DEFAULT_MANAGER = 0;
    const ManagerType CUSTOM_MANAGER  = 1;
}

template<class... Args>
class Pool {
public:
    
    void Do(const int managerType, const std::string & name, Args... arg) {
        switch(managerType) {
            case pmt::DEFAULT_MANAGER:
                 DefaultManager(name, arg...);
                 break;
                 
            case pmt::CUSTOM_MANAGER:
                 CustomManager(name, arg...);
                 break;
        }
    }
    
    void AddHandler(std::string & name, std::function<void(Args...)> & fun) {
        handlers.insert(std::pair<std::string, std::function<void(Args...)> &>(name, fun));
    }
    
    void Stop() {
        for(int i = 0 ; i < workers.size(); ++i) {
            workers[i].Stop();
        }
    }
    
    Pool(int poolSize) : iWorker(0) { 
        workers = std::vector<Worker<Args...>>(poolSize);
    }
    //~Pool() { Stop(); }
private:
    void CustomManager(const std::string & name, Args... arg);
    
    std::vector<Worker<Args...>> workers;
    std::map<std::string, std::function<void(Args...)> &> handlers;
    
    int iWorker;
    
    void DefaultManager(const std::string & name, Args... arg) {
        if(iWorker >= workers.size()) {
            iWorker = 0;
        }
        try {
            workers[iWorker].AddTask(handlers.at(name), arg ...);
        } catch (...) {
            //std::cerr << "smth wrong" << std::endl;
        }
        iWorker++;
    }
    
};


#include "Custom.hpp"
