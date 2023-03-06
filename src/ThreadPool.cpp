#include "ThreadPool.h"

ThreadPoll::ThreadPoll(unsigned int threadcnt){
	threadcnt = min(threadcnt, thread::hardware_concurrency());	// 不能超过最大线程
	run = true;
	for (unsigned int i = 0; i < threadcnt; i++)
		work.emplace_back(
			thread( std::bind(&ThreadPoll::runinthread, this) )
		);
}

ThreadPoll::~ThreadPoll() {
	{
		lock_guard<mutex> lock(mtx);	// ?
		run = 1;
	}
	cv.notify_all();	// 唤醒所有的线程
	for (thread& w : work)
		if (w.joinable())
			w.join();
}

void ThreadPoll::runinthread() {
	while (run) {
		unique_lock<mutex> lock(mtx);		// ?
		while (task.size() == 0 && run) 
			cv.wait(lock);	// 当没有任务且线程池正在运行，不要轮询CPU，会造成资源浪费
		
		if (task.size()) {
			function<void()> t = move(task.front());
			task.pop_front();
			lock.unlock();	// 解锁
			t();			// 执行任务
		}
	}
}

void ThreadPoll::AddTask(function<void()> f, bool pry) {
	{
		unique_lock<mutex> lock(mtx);
		if (!run) {
			// 等着写！！
			return;
		}
		if (pry) task.emplace_front(f);
		else task.emplace_back(f);
	}
	cv.notify_one();	// 唤醒一个函数？
}
