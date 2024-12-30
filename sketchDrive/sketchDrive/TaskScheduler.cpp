#include "TaskScheduler.h"
#include <Arduino.h>

// Not using maybe later date

// Constructor: Initializes taskCount to 0
TaskManager::TaskManager() {
  taskCount = 0;
}

// Add a new task to the tasks array
void TaskManager::addTask(TaskFunction func, unsigned long interval) {
  if (taskCount < 10) {
    tasks[taskCount].function = func;
    tasks[taskCount].interval = interval;
    tasks[taskCount].lastRun = 0;
    taskCount++;
  }
}

// Manage and run all tasks asynchronously
void TaskManager::manageTasks() {
  unsigned long currentMillis = millis();  // Get the current time

  for (int i = 0; i < taskCount; i++) {
    // If enough time has passed for this task, run it
    if (currentMillis - tasks[i].lastRun >= tasks[i].interval) {
      tasks[i].lastRun = currentMillis;  // Update last run time
      tasks[i].function();  // Run the task's function
    }
  }
}
