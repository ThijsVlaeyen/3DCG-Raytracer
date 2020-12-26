#pragma once

#include "tasks/task-scheduler.h"


namespace tasks
{
	namespace schedulers
	{
		/// <summary>
		/// Creates a scheduler that performs each task in parallel.
		/// </summary>
		TaskScheduler parallel_Thijs();
	}
}