#include "tasks/parallel-task-scheduler.h"
#include <thread>
#include <mutex>
#include <algorithm>

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
			size_t n = std::thread::hardware_concurrency();
			//if (n > 1) n--;
			std::vector<std::thread> threads(n);
			auto divided_tasks = divide_tasks(tasks, n);
			for (int i = 0; i < n; i++) {
				threads[i] = std::thread([this, t = divided_tasks[i]]{ this->perform_tasks(t); });
			}

			//for joining
			for (int i = 0; i < n; i++) {
				threads[i].join();
			}
		}

		void perform_tasks(std::vector<std::shared_ptr<Task>> tasks) const {
			for (auto task : tasks) {
				task->perform();
			}
		}

	private:
		std::vector<std::vector<std::shared_ptr<Task>>> divide_tasks(std::vector<std::shared_ptr<Task>> tasks, size_t n) const {
			std::vector<std::vector<std::shared_ptr<Task>>> outVec;

			size_t length = tasks.size() / n;
			size_t remain = tasks.size() % n;

			size_t begin = 0;
			size_t end = 0;

			for (size_t i = 0; i < std::min(n, tasks.size()); ++i)
			{
				end += (remain > 0) ? (length + !!(remain--)) : length;

				outVec.push_back(std::vector<std::shared_ptr<Task>>(tasks.begin() + begin, tasks.begin() + end));

				begin = end;
			}

			return outVec;
		}
	};
}

TaskScheduler tasks::schedulers::parallel()
{
	return TaskScheduler(std::make_shared<ParallelTaskScheduler>());
}