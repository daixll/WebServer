#include <iostream>
#include <queue>
#include <vector>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
using namespace std;

class ThreadPoll
{
private:
	vector<thread> work;			// 线程池
	deque<function<void()>> task;	// 任务池

	mutex mtx;				// 锁
	condition_variable cv;	// 条件变量

	bool run;				// 线程池是否能运行
	void runinthread();		// 执行函数

public:
	ThreadPoll(unsigned int threadcnt);
	~ThreadPoll();

	void AddTask(function<void()>, bool pry = false);	// 添加任务
	void GetInfo();			// 获取线程池的信息
};