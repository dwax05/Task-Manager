#include "tasks.h"

int main(void) {
    Task tasks[MAX_TASKS];
    int taskCount = 0;

    // Load tasks from file at startup
    loadTasksFromFile(tasks, &taskCount, "tasks.txt");

    ui(tasks, &taskCount);

    // Save tasks to file before exiting
    saveTasksToFile(tasks, taskCount, "tasks.txt");

    return 0;
}
