#ifndef TASKMANAGER_H
#define TASKMANAGER_H

// not using maybe later date

// Function pointer type for task functions
typedef void (*TaskFunction)();

// Task structure to store the task function, interval, and last execution time
struct Task {
  TaskFunction function;    // Task function pointer
  unsigned long lastRun;    // Last time the task was executed
  unsigned long interval;   // Interval for the task
};

// TaskManager class to manage multiple tasks
class TaskManager {
  public:
    // Constructor to initialize the TaskManager with a list of tasks
    TaskManager();

    // Add a new task to the task list
    void addTask(TaskFunction func, unsigned long interval);

    // Manage and run all tasks asynchronously
    void manageTasks();

  private:
    Task tasks[10];  // Array of tasks (can store up to 10 tasks)
    int taskCount;   // Number of tasks added
};

#endif
