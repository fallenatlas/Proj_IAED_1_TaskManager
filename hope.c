/* 
 * File: main.c
 * Author: Tiago Antunes
 * Description: A program that simulates a project task manager in C.
*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Maximum numbers of values stored */
#define MAX_DESC_USERS 51       /* In descriptions and maximum amount of users */
#define MAX_USR_ATV 21          /* In user names and activity names */
#define MAX_ID 10000            /* Max number of tasks */
#define MAX_ATV 10              /* Max number of activities */

/* Inicial activities */
char TO_DO[] = "TO DO\0";
char IN_PROGRESS[] = "IN PROGRESS\0";
char DONE[] = "DONE\0";

/* Struct with the caracteristics of a task */
typedef struct {
    int id; /* Task id, integer in range [1, 10000] */
    char description[MAX_DESC_USERS]; /* string with the description of the task */
    char user[MAX_USR_ATV]; /* string with the name of the user the task is assigned to */
    char state[MAX_USR_ATV]; /* string with the current ativity of the task */
    int previewTime; /* expected execution time of the task */
    int start; /* indicates when the task first started being executed */
    int time; /* indicated the execution time of the task */
} Task;

/* Struct with the global caracteristics of the program */
typedef struct {
    int newTask_id; /* The id value that a new task will have */
    int currentTime; /* The current time of the system */
    int currentUsers; /* The current number of users registered */
    int current_activities; /* The current number of activities */
    char usrs[MAX_DESC_USERS-1][MAX_USR_ATV]; /* An array with all of the user names */
    char activities[MAX_ATV][MAX_USR_ATV]; /* An array with all of the activity names */ 
} Control;

/* Global variables */
Task tasks[MAX_ID]; /* Array with all of the current tasks and its caracteristics */
Control global; /* Contains the global caracteristics of the program while it executes */

int checkSpace(char *c);
void wasteLine(void);
int addTask();
int errorNewTask(Task newtask);
void addNewTask(Task newtask);
int listTasks();
int inputString(char s[], int size, char c, char command);
void strcopy(char s[],char c[]);
int inputIds(int *n, char c);
void orderByDescription(int l[], char s, int left, int right);
void writeTasks(int l[], int num, int max);
void writeAllTasks(int max);
int increaseTime();
int addUserOrActivity(char cmd);
void writeAllUserOrActivity(char c);
int errorUserActivity(char u[], char c);
int moveActivity();
int errorMoveActivity(int id, char u[], char a[]);
int taskInActivity();



/* Reads commands from stdin and executes the associated action */
int main()
{
    /* Variable that contains the comand */
    char cmd;

    strcpy(global.activities[0], TO_DO);
    strcpy(global.activities[1], IN_PROGRESS);
    strcpy(global.activities[2], DONE);

    global.newTask_id = 1;
    global.currentTime = 0;
    global.currentUsers = 0;
    global.current_activities = 3;

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
            break;

        case 'l':
            /* List the tasks inputed or all if none are given */
            listTasks();
            break;
        
        case 'n':
            /* Increases the time of the system */ 
            increaseTime();
            break; 
       
        case 'u':
            /* Add user or list all users */
            addUserOrActivity('u');
            break;

        case 'm':
            /* Move one task from one activity to another */
            moveActivity();
            break;

        case 'd':
            taskInActivity();
            break;

        case 'a':
            addUserOrActivity('a');
            break;
        }
    }
}

/* Consumes the remaining caracters in the line */
void wasteLine()
{
    char c;
    while(c != '\n') {
        c = getchar();
    }
}

/* Creates a new task */
int addTask()
{
    /* Define local variables */
    Task newtask;
    char c;
    newtask.previewTime = 0;

    checkSpace(&c);
    inputIds(&newtask.previewTime, c);
    checkSpace(&c);
    inputString(newtask.description, MAX_DESC_USERS, c, 's');

/*    printf("%d\n%s\n", n, newtask.description);;*/
    /* Check if there are no error when creating this task */
    if (errorNewTask(newtask))
    {
        return 1;
    }

    /* Finish the task configuration and print output */ 
    addNewTask(newtask);
    return 0;
}

/* Auxiliar function for addTask(), checks for any errors */
int errorNewTask(Task newtask)
{
    int i;
    int ids = global.newTask_id - 1;

    /* Check if the number of tasks is below the max */
    if (ids >= MAX_ID) {
        printf("too many tasks\n");
        return 1;
    }

    /* Check if there are already tasks with the inputed description */
    for (i = 0; i < ids; i++) {
        if (!(strcmp(tasks[i].description, newtask.description))) {
            printf("duplicate description\n");
            return 1;
        }
    }

    if (newtask.previewTime <= 0) {
        printf("invalid duration\n");
        return 1;
    }

    return 0;
}

/* Auxiliar function for addTask(), 
   completes the adding process and prints output */
void addNewTask(Task newtask)
{
    /* Setup the remaining task values */
    newtask.start = 0;
    newtask.id = global.newTask_id++;
    newtask.time = 0;
    strcpy(newtask.state, TO_DO);

    /* Save task in the global pool */
    tasks[global.newTask_id - 2] = newtask;

    /* Print output to stdout */
    printf("task %d\n", newtask.id);
}

/* Lists the requested tasks in the same order or
   all by alphabetical order if none are requested */ 
int listTasks()
{
    /* Define local variables */
    int i = 0, j = 0, n = 0, max_id = global.newTask_id - 1;
    int l[MAX_ID] = {0};
    char c;

    n = checkSpace(&c);
    if (n == -1) {
        writeAllTasks(max_id);
        return 0;
    }

    n = 1;
    while (n != -1) {
        n = inputIds(&i, c);
        l[j++] = i;
        i = 0;
        if (n == 0) {
            break;
        }
        n = checkSpace(&c);
    }

    /* Write requested tasks */
    writeTasks(l, j, max_id);

    return 0;
}
void writeTasks(int l[], int num, int max)
{
    int i, n = 0;
    int id, time;
    char state[MAX_USR_ATV], desc[MAX_DESC_USERS];

    for(i = 0; i < num; i++) {  /* For each argument */
        /* Position in array tasks of the requested task */
        n = l[i] - 1;              
        if ((n < max) && n >= 0) {
            id = tasks[n].id;
            strcpy(state, tasks[n].state);
            time = tasks[n].previewTime;
            strcpy(desc, tasks[n].description);
            printf("%d %s #%d %s\n", id, state, time, desc);
        }
        else {
            printf("%d: no such task\n", l[i]);
        }
       
    }
}

void writeAllTasks(int max)
{
    int i, l[MAX_ID];
    int id, time, j;
    char state[MAX_USR_ATV], desc[MAX_DESC_USERS];

    for (i = 0; i < max; i++) {
        l[i] = i;
    }
        
    orderByDescription(l, 'n', 0, max-1);

    for(i = 0; i < max; i++) {
        j = l[i];
        id = tasks[j].id;
        strcpy(state, tasks[j].state);
        time = tasks[j].previewTime;
        strcpy(desc, tasks[j].description);
        printf("%d %s #%d %s\n", id, state, time, desc);
    }
}

int inputString(char s[], int size, char c, char command)
{
    int i = 0;
/*
    while(c != '\n' && (command == 'u' ? (c != ' ' || fora) : 1)) {
        if (fora && c != ' ') {
            fora = 0;
        }
        if (!fora) {
            s[i++] = c;
        }
        c = getchar();
    }
*/
    while((i < size-1) && (c != '\n') && 
        (command == 'u' ? (!isspace(c)) : 1)) {
        s[i++] = c;
        c = getchar();
    }

    s[i] = '\0';
    return (c == ' ' ? 1 : 0);
}

void strcopy(char s[],char c[])
{
    int i;

    i = 0;
    while(s[i] != '\0') {
        c[i] = s[i];
        i++;
    }

    c[i] = '\0';
}

/*
void orderByDescription(int l[], char s, int size) 
{
    int i = 0, j, hold;
    char desc[MAX_DESC_USERS];

    printf("in here\n");

    for (i = 1; i < size; i++) {
            j = i-1;
            hold = l[i];
            printf("after hold\n");
            strcopy(tasks[l[i]].description, desc);
            printf("after strcopy\n");
            while((j >= 0) && (strcmp(tasks[l[j]].description, desc) > 0) && 
                 (s == 's' ? tasks[l[j]].start == tasks[l[i]].start : 1)) {
                l[j+1] = l[j];
                l[j] = hold;
                j--;
            }
        }
}
*/
int checkSpace(char *c)
{
    char i = *c;
    i = getchar();
    while (isspace(i) && i != '\n') {
        i = getchar();
    }
    if (i == '\n')
        return -1;
    *c = i;
    return 0;
}

int inputIds(int *n, char c)
{
    int i = *n;
    int neg = 0;
    if (c == '-') {
        neg = 1;
        c = getchar();
    }

    while ((!isspace(c)) && c != '\n') {
        i = i * 10 + (c - '0');
        c = getchar();
    }

    if (neg)
        i *= -1;

    *n = i;
    return (c == '\n' ? 0 : 1);
}

/*-------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------
*/

/* Increaments the time of the system */
int increaseTime()
{
    int t = 0, i, max = global.newTask_id - 1;
    char c;

    checkSpace(&c);
    inputIds(&t, c);

    if (t < 0) {
        printf("invalid time\n");
        return 1;
    }

    if (t > 0) {
        global.currentTime += t;
        for (i = 0; i < max; i++) {
            if ((strcmp(tasks[i].state, TO_DO)) && 
               (strcmp(tasks[i].state, DONE))) {
                tasks[i].time += t;
            }
        }
    }

    printf("%d\n", global.currentTime);
    return 0;
}

/* ---------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------*/

int addUserOrActivity(char cmd)
{
    int n;
    int size = MAX_USR_ATV;
    char c, u[MAX_USR_ATV];

    /* Get input */
    n = checkSpace(&c);
    if (n == -1) {
        /* Write all users if none are given */
        writeAllUserOrActivity(cmd);
        return 0;
    }
    inputString(u, size, c, cmd);

    /* Error checking */
    if(errorUserActivity(u, cmd)) {
        return 1;
    }
    
    /* Add new user or activity to the global pool */
    if (cmd == 'u') {
        strcopy(u, global.usrs[global.currentUsers]);
        global.currentUsers++;
    }
    else {
        strcopy(u, global.activities[global.current_activities]);
        global.current_activities++;
    }
    return 0;
}

void writeAllUserOrActivity(char c)
{
    int i;

    if (c == 'u') {
        for (i = 0; i < global.currentUsers; i++) {
             printf("%s\n", global.usrs[i]);
        }
    }
    else {
        for (i = 0; i < global.current_activities; i++) {
            printf("%s\n", global.activities[i]);
        }
    }
}

int errorUserActivity(char u[], char c)
{
    int i, n, total, cmd;
    total = (c == 'u' ? global.currentUsers : global.current_activities);
    cmd = (c == 'u' ? 1 : 0);

    for (i = 0; i < total; i++) {
        if (!(strcmp((cmd ? global.usrs[i] : global.activities[i]), u))) {
            (cmd ? printf("user already exists\n") : 
                 printf("duplicate activity\n"));
            return 1;
        }
    }

    if (!cmd) {
        n = strlen(u);
        for (i = 0; i < n; i++) {
            if ('a' <= u[i] && 'z' >= u[i]){
                printf("invalid description\n");
            return 1;
            }
        }
    }

    if ((total) >= (cmd ? MAX_DESC_USERS-1 : MAX_ATV)) {
        (cmd ? printf("too many users\n") : 
        printf("too many activities\n"));
        return 1;
    }
    return 0;
}

/* --------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------- */

int moveActivity()
{
    int id = 0;
    char c, u[MAX_USR_ATV], a[MAX_USR_ATV];

    /* Get inputs */
    checkSpace(&c);
    inputIds(&id, c);
    checkSpace(&c);
    inputString(u, MAX_USR_ATV, c, 'u');
    checkSpace(&c);
    inputString(a, MAX_USR_ATV, c, 's');

    /* Check for errors */
    if (errorMoveActivity(id, u, a))
        return 1;

    /* Iniciate the time is task is leaving TO DO */
    if (!strcmp(tasks[id-1].state, TO_DO)) {
        tasks[id-1].start = global.currentTime;
    }
    strcpy(tasks[id-1].state, a);
    
    
    if (!(strcmp(a, DONE))) {
        printf("duration=%d slack=%d\n", tasks[id-1].time, 
        tasks[id-1].time - tasks[id-1].previewTime);
    }

    return 0;
}

int errorMoveActivity(int id, char u[], char a[])
{
    int max_id = global.newTask_id - 1;
    int i, t, j, in = 0;

    /* If moving to same activity do nothing */
    if (!strcmp(a, tasks[id-1].state)) {
        return 1;
    }

    /* Check for valid id */
    if (id <= 0 || id > max_id) {
        printf("no such task\n");
        return 1;
    }

    /* Check if task is already started and the user requested to 
    move the task to TO DO */
    if ((strcmp(TO_DO, tasks[id-1].state)) && !(strcmp(TO_DO, a))) {
        printf("task already started\n");
        return 1;
    }
/*
     Check is user exists 
    t = global.currentUsers;
    for (i = 0; i < t && !in; i++) {
        if (!(strcmp(global.usrs[i], u))) {
            in = 1;
        }
    }
    if (!in) {
        printf("no such user\n");
        return 1;
    }

     Check if activity exists 
    in = 0;
    t = global.current_activities;
    for (i = 0; i < t && !in; i++) {
        if (!(strcmp(global.activities[i], a))) {
            in = 1;
        }
    }
    if (!in) {
        printf("no such activity\n");
        return 1;
    }
*/

    for (i = 0; i < 2; i++) {
        in = 0;
        t = (i ? global.current_activities : global.currentUsers);
        for (j = 0; j < t && !in; j++) {
            if (!(strcmp((i ? global.activities[j] : global.usrs[j]),
            (i ? a : u)))) {
                in = 1;
            }
        }
        if (!in) {
            (i ? printf("no such activity\n") :
            printf("no such user\n"));
            return 1;
        }
    }
    return 0;
}

/* ----------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------- */

int taskInActivity()
{
    int num = 0, i, hold, j, t, all[MAX_ID], max_id = global.newTask_id - 1, in = 0;
    char c, a[MAX_USR_ATV];

    /* Get input */
    checkSpace(&c);
    inputString(a, MAX_USR_ATV, c, 's');

    /* Error cheking */
    for (i = 0; i < global.current_activities && !in; i++) {
        if (!(strcmp(global.activities[i], a))) {
            in = 1;
        }
    }
    if (!in)
        printf("no such activity\n");

    /* Build and array that contains all the positions of tasks with 
    the same activity */
    for (i = 0; i < max_id; i++) {
        if (!strcmp(tasks[i].state, a)) {
            all[num++] = i; 
        }
    }

    for (i = 1; i < num; i++) {
        hold = all[i];
        t = tasks[all[i]].start;
        j = i - 1;
        while(t < tasks[all[j]].start && j >= 0) {
            all[j+1] = all[j];
            j--;
        }
        all[j + 1] = hold;
    }

    orderByDescription(all, 's', 0, num-1);

    for (i = 0; i < num; i++) {
        printf("%d %d %s\n", tasks[all[i]].id, tasks[all[i]].start, tasks[all[i]].description);
    }
    return 0;
}

/* -------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------- */

void orderByDescription(int l[], char s, int left, int right)
{
    int i, j;

    for (i = left+1; (i <= right); i++) {
        int key = l[i];
        j = i-1;
        while (j >= left && 
        (s == 's' ? tasks[l[j]].start == tasks[l[i]].start : 1) &&
        (strcmp(tasks[key].description, tasks[l[j]].description) < 0)) {
            l[j+1] = l[j];
            j--;
        }
        l[j+1] = key;
    }
}


/*ant(A, B) (strcmp(task(A), task(B))>0)*/
