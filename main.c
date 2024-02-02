#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main(void)
{
  Task tasks[MAX_TASKS];
  int taskCount = 0;

  // Load tasks from file at startup
  loadTasksFromFile(tasks, &taskCount, "tasks.txt");

  ui(tasks, &taskCount);

  // Save tasks to file before exiting
  saveTasksToFile(tasks, taskCount, "tasks.txt");

  return 0;
}

void ui(Task* tasks, int* taskCount)
{
  bool loop = true;
  char response[CHARSIZE];
  while (loop) {
    fputs("tskmngr> ", stdout);
    fgets(response, CHARSIZE, stdin);
    // Remove newline character from the response
    response[strcspn(response, "\n")] = '\0';

    if (strcmp(response, "exit") == 0) {
      loop = false;
    } else if (strcmp(response, "list") == 0) {
      listTodos(tasks, *taskCount);
    } else {
      processResponse(tasks, taskCount, response);
    }
  }
}

void processResponse(Task* tasks, int* taskCount, char* response)
{
  // Tokenize the response
  char* command = strtok(response, " ");
  char* taskName = strtok(NULL, "\0");

  // Check the command and perform the corresponding action
  if (command != NULL) {
    if (strcmp(command, "add") == 0) {
      if (taskName != NULL) {
        if (*taskCount < MAX_TASKS) {
          strncpy(tasks[*taskCount].name, taskName, CHARSIZE);
          tasks[*taskCount].status = TODO;
          (*taskCount)++;
          fputs("Task added: ", stdout);
          fputs(taskName, stdout);
          fputs("\n", stdout);
        } else {
          fputs("Task list is full. Cannot add more tasks.\n", stdout);
        }
      } else {
        fputs("Invalid format. Usage: add <Task>\n", stdout);
      }
    } else if (strcmp(command, "done") == 0) {
      if (taskName != NULL) {
        for (int i = 0; i < *taskCount; i++) {
          if (strcmp(tasks[i].name, taskName) == 0) {
            tasks[i].status = DONE;
            fputs("Task marked as done: ", stdout);
            fputs(taskName, stdout);
            fputs("\n", stdout);
            return;
          }
        }
        fputs("Task not found: ", stdout);
        fputs(taskName, stdout);
        fputs("\n", stdout);
      } else {
        fputs("Invalid format. Usage: done <Task>\n", stdout);
      }
    } else if (strcmp(command, "WIP") == 0) {
      if (taskName != NULL) {
        for (int i = 0; i < *taskCount; i++) {
          if (strcmp(tasks[i].name, taskName) == 0) {
            tasks[i].status = WIP;
            fputs("Task marked as work in progress: ", stdout);
            fputs(taskName, stdout);
            fputs("\n", stdout);
            return;
          }
        }
        fputs("Task not found: ", stdout);
        fputs(taskName, stdout);
        fputs("\n", stdout);
      } else {
        fputs("Invalid format. Usage: WIP <Task>\n", stdout);
      }
    } else {
      fputs("Unknown command: ", stdout);
      fputs(command, stdout);
      fputs("\n", stdout);
    }
  }
}

void saveTasksToFile(Task* tasks, int taskCount, const char* filename)
{
  FILE* file = fopen(filename, "w");
  if (file != NULL) {
    for (int i = 0; i < taskCount; i++) {
      fprintf(file, "%s %d\n", tasks[i].name, tasks[i].status);
    }
    fclose(file);
    fputs("Tasks saved to file.\n", stdout);
  } else {
    fputs("Error saving tasks to file.\n", stderr);
  }
}

void loadTasksFromFile(Task* tasks, int* taskCount, const char* filename)
{
  FILE* file = fopen(filename, "r");
  if (file != NULL) {
    *taskCount = 0;
    while (*taskCount < MAX_TASKS && fscanf(file, "%s %d", tasks[*taskCount].name, &(tasks[*taskCount].status)) == 2) {
      (*taskCount)++;
    }
    fclose(file);
    fputs("Tasks loaded from file.\n", stdout);
  } else {
    fputs("File not found. Starting with an empty task list.\n", stdout);
  }
}

void listTodos(Task* tasks, int taskCount)
{
  fputs("TODO tasks:\n", stdout);
  for (int i = 0; i < taskCount; i++) {
    if (tasks[i].status == TODO) {
      fputs(tasks[i].name, stdout);
      fputs("\n", stdout);
    }
  }
}