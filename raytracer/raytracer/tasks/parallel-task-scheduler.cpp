#include "tasks/parallel-task-scheduler.h"
#include <thread>
#include <mutex>

using namespace tasks;


namespace
{
	/// <summary>
	/// Performs tasks in parallel.
	/// </summary>
	class ParallelTaskScheduler : public tasks::schedulers::_private_::TaskSchedulerImplementation
	{

	public:
		void perform(std::vector<std::shared_ptr<Task>> tasks) const
		{
			static std::mutex mtx;
			std::vector<std::thread> th;

			// Check how many threads can run concurrently
			unsigned int num_threads = std::thread::hardware_concurrency();

			// Create threads and call execute with a pointer to tasks.
			for (unsigned i = 0; i < num_threads; i++)
			{
				th.push_back(std::thread(&ParallelTaskScheduler::execute, &tasks, &mtx));
			}

			// Join the threads.
			for (auto& t : th)
			{
				t.join();
			}
		}

		static void* execute(std::vector<std::shared_ptr<Task>>* tasks, std::mutex* mtx)
		{
			// Check if there are tasks left to perform
			if (tasks->size() > 0)
			{
				// Acquire lock to get access to list of tasks.
				mtx->lock();

				// Create a task from the back of the tasks list.
				auto task = tasks->back();

				// Remove task from list.
				tasks->pop_back();

				// Release lock.
				mtx->unlock();

				// Perform the task.
				task->perform();

				// Call recursively until all tasks have been performed.
				execute(tasks, mtx);
			}
			return 0;
		}
	};
}

TaskScheduler tasks::schedulers::parallel()
{
	return TaskScheduler(std::make_shared<ParallelTaskScheduler>());
}