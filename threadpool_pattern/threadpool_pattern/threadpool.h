#include <functional>
#include <thread>
#include <queue>
#include <mutex>
#include <memory>
#include <condition_variable>

using namespace std;

template<class T>
struct data_p
{
	data_p() :ready(false){}
	bool ready;
	T data;
};

class thread_pool
{
public:

	typedef function<void()> func_type;

	class worker
	{
	public:

		worker():enabled(true), func_queue(), thread(&worker::thread_func, this){}

		~worker()
		{
			enabled = false;
			con_var.notify_one();
			thread.join();
		}

		void append_func(func_type func)
		{
			unique_lock<std::mutex> locker(mutex);
			func_queue.push(func);
			con_var.notify_one();
		}

		size_t get_task_count()
		{
			unique_lock<std::mutex> locker(mutex);
			return func_queue.size();
		}

		bool   is_empty()
		{
			unique_lock<std::mutex> locker(mutex);
			return func_queue.empty();
		}

	private:

		bool					enabled;
		std::condition_variable con_var;
		std::queue<func_type>		func_queue;
		std::mutex				mutex;
		std::thread				thread;

		void thread_func()
		{
			while (enabled)
			{
				unique_lock<std::mutex> locker(mutex);
				con_var.wait(locker, [&](){ return !func_queue.empty() || !enabled; });
				while (!func_queue.empty())
				{
					func_type fn = func_queue.front();
					locker.unlock();
					fn();
					locker.lock();
					func_queue.pop();
				}
			}
		}
	};

	typedef shared_ptr<worker> worker_ptr;

	thread_pool(size_t threads = 1)
	{
		if (threads == 0)
			threads = 1;
		for (size_t i = 0; i<threads; i++)
		{
			worker_ptr Worker_(new worker);
			_workers.push_back(Worker_);
		}
	}

	~thread_pool() {}

	template<class R, class FUNC, class... ARGS>
	shared_ptr<data_p<R>> run_async(FUNC _func, ARGS... _args)
	{
		function<R()> rfn = std::bind(_func, _args...);
		shared_ptr<data_p<R>> Data(new data_p<R>());
		func_type func = [=]()
		{
			Data->data = rfn();
			Data->ready = true;
		};
		auto pWorker = get_free_worker();
		pWorker->append_func(func);
		return Data;
	}

	template<class FUNC, class... ARGS>
	void run_async(FUNC _func, ARGS... _args)
	{
		auto pWorker = get_free_worker();
		pWorker->append_func(std::bind(_func, _args...));
	}

private:

	worker_ptr get_free_worker()
	{
		worker_ptr p_worker;
		size_t min_tasks = UINT32_MAX;
		for (auto &it : _workers)
		{
			if (it->is_empty())
			{
				return it;
			}
			else if (min_tasks > it->get_task_count())
			{
				min_tasks = it->get_task_count();
				p_worker = it;
			}
		}
		return p_worker;
	}

	vector<worker_ptr> _workers;


};