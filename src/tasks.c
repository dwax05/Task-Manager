#include "tasks.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void setConsoleColor(TaskStatus status) {
    switch (status) {
    case TODO:
        printf("\033[1;31m"); // Red
        break;
    case WIP:
        printf("\033[1;33m"); // Yellow
        break;
    case DONE:
        printf("\033[1;32m"); // Green
        break;
    default:
        printf("\033[0m"); // Reset color
        break;
    }
}

void ui(Task* tasks, int* taskCount) {
    bool loop = true;
    char response[CHARSIZE];
    while (loop) {
        fputs("tskmngr> ", stdout);
        fgets(response, CHARSIZE, stdin);

        // Remove newline character from the response
        response[strcspn(response, "\n")] = '\0';
        // Convert the response to lowercase
        for (int i = 0; response[i]; i++) {
            response[i] = tolower(response[i]);
        }

        if (strcmp(response, "exit") == 0) {
            loop = false;
        } else if (strncmp(response, "list", 4) == 0) {
            char* argument = strtok(response + 4, " ");
            TaskStatus filter;
            if (argument != NULL) {
                if (strcmp(argument, "done") == 0) {
                    filter = DONE;
                } else if (strcmp(argument, "wip") == 0) {
                    filter = WIP;
                } else if (strcmp(argument, "todo") == 0) {
                    filter = TODO;
                } else {
                    fputs("Invalid list argument. Usage: list [done/wip/todo]\n", stdout);
                    continue;
                }
            } else {
                filter = -1;  // Default to listing all tasks
            }
            listTasks(tasks, *taskCount, filter);
        } else {
            processResponse(tasks, taskCount, response);
        }
    }
}

void processResponse(Task* tasks, int* taskCount, char* response) {
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
        } else if (strcmp(command, "wip") == 0) {
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

void saveTasksToFile(Task* tasks, int taskCount, const char* filename) {
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

void loadTasksFromFile(Task* tasks, int* taskCount, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file != NULL) {
        *taskCount = 0;
        while (*taskCount < MAX_TASKS &&
               fscanf(file, "%s %d", tasks[*taskCount].name, &(tasks[*taskCount].status)) == 2) {
            (*taskCount)++;
        }
        fclose(file);
        fputs("Tasks loaded from file.\n", stdout);
    } else {
        fputs("File not found. Starting with an empty task list.\n", stdout);
    }
}


void listTasks(Task* tasks, int taskCount, TaskStatus filter) {
    fputs("Task list:\n", stdout);
    for (int i = 0; i < taskCount; i++) {
        if (filter == -1 || tasks[i].status == filter) {
            setConsoleColor(tasks[i].status);
            const char* statusStr;
            switch (tasks[i].status) {
            case TODO:
                statusStr = "TODO";
                break;
            case WIP:
                statusStr = "WIP ";
                break;
            case DONE:
                statusStr = "DONE";
                break;
            default:
                statusStr = "Unknown";
                break;
            }
            fputs(statusStr, stdout);
            fputs(" ", stdout);
            fputs(tasks[i].name, stdout);
            fputs("\033[0m\n", stdout); // Reset color
        }
    }
}
