#include "Mission.h"
#include <thread>
#include <mutex>
#include <random>

using namespace std;
using namespace acg;

mutex mtx;

int main() {


    for (int i = 0; i < 20; ++i) {
        mtx.lock();
        cout << "main thread: " << i << endl;
        mtx.unlock();
    }

    return 0;
}


//* mutex mtx
//* lock_guard <mutex> guard <mtx>  (или mtx.lock  -->  mtx.unlock)
//*

