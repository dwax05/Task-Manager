#ifndef TASKS_H
#define TASKS_H

#include <stdbool.h>

#define CHARSIZE 200
#define MAX_TASKS 100

typedef enum {
    TODO,
    DONE,
    WIP
} TaskStatus;

typedef struct {
    char name[CHARSIZE];
    TaskStatus status;
} Task;

void ui(Task* tasks, int* taskCount);
void processResponse(Task* tasks, int* taskCount, char*);
void saveTasksToFile(Task* tasks, int taskCount, const char* filename);
void loadTasksFromFile(Task* tasks, int* taskCount, const char* filename);
void listTodos(Task* tasks, int taskCount);

#endif // TASKS_H
