/* 
 * File: main.c
 * Author: Tiago Antunes
 * Description: A program that simulates a project task manager in C.
*/
#include <stdio.h>
#include <string.h>

/* Maximum numbers of values stored */
#define MAX_DESC_USERS 50       /* In descriptions and maximum amount of users */
#define MAX_USR_ATV 20          /* In user names and activity names */
#define MAX_ID 2            /* Max number of tasks */
#define MAX_ATV 10              /* Max number of activities */

/* Inicial activities */ /*
#define TO_DO[] = {'T','O',' ','D','O','\0'}
#define IN_PROGRESS[] = {'I','N',' ','P','R','O','G','R','E','S','S','\0'}
#define DONE[] = {'D','O','N','E','\0'}
*/
/* Struct with the caracteristics of a task */
typedef struct {
    int id; /* Task id, integer in range [1, 10000] */
    char description[MAX_DESC_USERS]; /* string with the description of the task */
    char user[MAX_USR_ATV]; /* string with the name of the user the task is assigned to */
    char state[MAX_USR_ATV]; /* string with the current ativity of the task */
    int previewTime; /* expect execution time of the task */
    int start; /* indicates when the task first started being executed */
    int time; /* indicated the execution time of the task */
} Task;

/* Global variables */
int newTask_id = 1;
int currentTime = 0;
int currentUsers = 0;
char usrs[MAX_DESC_USERS];
char activities[MAX_ATV];
Task tasks[MAX_ID];

int addTask(void);

/* Reads commands from stdin and executes the associated action */
int main()
{
    /* Variable that contains the comand */
    char cmd;

    /* Reads caracters from stdin until it finds the terminal case or a command to execute */
    while (1) {
        cmd = getchar();
        switch (cmd) {
        case 'q':
            /* Ends the program */
            return 0;

        case 't':
            /* Adds a new task */
            addTask();
            /* printf("%d %d %s\n", tasks[newTask_id - 2].id, tasks[newTask_id - 2].previewTime, tasks[newTask_id - 2].description); */
            break;

        case 'l':
            listTasks();
            break;
        }
    }
}


/* Creates a new task */
int addTask()
{
    /* Local variables */
    Task newtask;

    int arg = 0, ti = 0, fora = 0, n = 0, i = 0, id = newTask_id;
    char c;

    /* Get input from stdin and divide it into separate arguments */ 
    c = getchar();
    while (c != '\n') {
        if (c == ' ' && !fora && (arg != 2)) {
            fora = 1;
            arg++;
        }

        if (fora && c != ' ') {
            fora = 0;
        }

        if (!fora) {
            if (arg == 1 && ti < 10)
                n = n * 10 + (c - '0');
            if (arg == 2 && i < MAX_DESC_USERS)
                newtask.description[i++] = c;
        }
        c = getchar();
    }


    newtask.description[i] = '\0';

    /* Check if its possible to create the new task */
    if (id - 1 >= MAX_ID) {
        printf("too many tasks\n");
        return 1;
    }

    for (i = 0; i < id - 1; i++) {
        if (!(strcmp(tasks[i].description, newtask.description))) {
            printf("duplicate description\n");
            return 1;
        }
    }

    /* Set remaining task values */ 
    newtask.id = newTask_id++;
    newtask.previewTime = n;
    newtask.start = currentTime;
    newtask.time = 0;
    tasks[newTask_id - 2] = newtask;

    /* Print output to stdout */
    printf("task %d\n", newtask.id);
    return 0;
}












