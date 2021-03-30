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

/* Inicial activities */
const char TO_DO[] = "TO DO";
const char IN_PROGRESS[] = {'I','N',' ','P','R','O','G','R','E','S','S','\0'};
const char DONE[] = {'D','O','N','E','\0'};

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
int getInput(char input[][50], int command);


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
            printf("id:%d state:%s prev:%d desc:%s\n", tasks[newTask_id - 2].id, tasks[newTask_id - 2].state, tasks[newTask_id - 2].previewTime, tasks[newTask_id - 2].description);
            break;

        
        }
    }
}


/* Creates a new task */
int addTask()
{
    /* Local variables */
    Task newtask;

    char args[2][MAX_DESC_USERS] = {0}; 
    int id = newTask_id, i = 0, n = 0;
    char c;

    getInput(args, 't');
    printf("%d %s", args[0][0], args[1]);

    /* Check if its possible to create the new task */
    if (id - 1 >= MAX_ID) {
        printf("too many tasks\n");
        return 1;
    }

    i = 0;
    c = args[1][i];
    while (c != '\0') {
        newtask.description[i] = c;
        i++;
        c = args[1][i];
    }  
    newtask.description[i] = '\0';

    /* Check if there are no tasks with the same description */
    for (i = 0; i < id - 1; i++) {
        if (!(strcmp(tasks[i].description, newtask.description))) {
            printf("duplicate description\n");
            return 1;
        }
    }

    i = 0;
    n = 0;
    while (args[0][i] != '\0') {
        n = args[0][i] * 10 + (c - '0');
    }

    /* Set remaining task values */ 
    newtask.id = newTask_id++;
    printf("%d", args[0][0]);
    newtask.previewTime = n;
    newtask.time = 0;
    tasks[newTask_id - 2] = newtask;
    n = strlen(TO_DO);
    for (i = 0; i < n; i++) {
        newtask.state[i] = TO_DO[i];
        printf("string %s\n", newtask.state);
    }
    newtask.state[i] = '\0';

    /* Print output to stdout */
    printf("task %d\n", newtask.id);
    return 0;
}

int getInput(char input[][50], int command)
{
    int args, fora, i;
    char c;

    args = 0;
    fora = 1;
    i = 0;

    c = getchar();
    c = getchar();
    while (c != '\n') {
        if (c == ' ' && !fora && (args != 1 && command == 't')) {
            fora = 1;
            input[args][i] = '\0';
            args++;
            i = 0;
        }

        if (fora && c != ' ') {
            fora = 0;
        }

        if (!fora) {
            if (command == 'l' || command == 'n' || (command == 't' && args == 0) || (command == 'm' && args == 0)) {
                input[args][i++] = c;
                printf("args:%d c:%c input:%d\n", args, c, input[args][i - 1]);
            }
            else if (command == 't' && args == 1 && i < MAX_DESC_USERS) {
                input[args][i++] = c;
            }
            else if (i < MAX_USR_ATV) {
                input[args][i++] = c;
            }
        }
        c = getchar();
    }
    input[args][i] = '\0';
    printf("here: %s %s", input[0], input[1]);

    return args;
}

/*
int getInput(int arg[],char command,int arg_num)
{
    int c, i;
    i = arg_num;

    c = getchar();
    if (command == 't') {
        scanf("%s", arg[i])
    }
*/




