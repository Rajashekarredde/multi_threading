#include<bits/stdc++.h>
using namespace std;
struct request
{
    int id;
    string data;
    bool isCompleted;
};

std::mutex m;
std::queue<request> q;
std::condition_variable cv;

void producer(int numofReqs)
{
    for( int i =1; i<=numofReqs; i++ )
    {
        struct request req;
        req.id = i;
        req.data = "data " + to_string(i);
        req.isCompleted = false;
        
        {
            std::lock_guard<mutex> l(m);
            q.push(req);
        }
        cv.notify_one();
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
}

void consumer()
{
    while( true )
    {    
        struct request req;
        {
            std::unique_lock<mutex> l(m);
            cv.wait(l, [] { return !q.empty();});
            req = q.front();
            q.pop();
            req.isCompleted = false;
            req.data = "data for "+to_string(req.id)+" is consumed";
        }
        cout<<req.data<<endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
}


int main()
{
    const int num = 5;
    std::thread t1(producer, num);
    std::thread t2(consumer);
    t1.join();
    t2.join();
    return 0;
}
