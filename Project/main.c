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
    int previewTime; /* expect execution time of the task */
    int start; /* indicates when the task first started being executed */
    int time; /* indicated the execution time of the task */
} Task;

/* Global variables */
int newTask_id = 1;
int currentTime = 0;
int currentUsers = 0;
int current_activities = 3;
char usrs[MAX_DESC_USERS][MAX_USR_ATV];
char activities[MAX_ATV][MAX_USR_ATV];
Task tasks[MAX_ID];

int addTask(void);
int listTasks(void);
int inputlistTasks(int l[]);
int increaseTime(void);
int getInput(int input[][50], int command);
int addUser(void);
int moveActivity(void);
int inputString(char s[], char command);
int convertToInt(char s[]);
void strcopy(char s[],char c[]);
int teste(void);
int inputIds(int *n);
int taskInActivity(void);


/* Reads commands from stdin and executes the associated action */
int main()
{
    /* Variable that contains the comand */
    char cmd;

    strcopy(TO_DO, activities[0]);
    strcopy(IN_PROGRESS, activities[1]);
    strcopy(DONE, activities[2]);

    printf("%s %s %s", activities[0], activities[1], activities[2]);

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

        case 'l':
            /* List the tasks inputed or all if none are given */
            listTasks();
            break;
        
        case 'n':
            /*Increases the time of the system */
            increaseTime();
            break; 
        
        case 'u':
            /*Add user or list all users*/ 
            addUser();
            break;

        case 'm':
            /* Move one task from one activity to another */
            moveActivity();
            break;

        case 'o':
            teste();
            break;

        case 'd':
            taskInActivity();
            break;
        }
    }
}


/* Creates a new task */
int addTask()
{
    /* Local variables */
    Task newtask;

    int arg = 0, ti = 0, fora = 0, n = 0, i = 0;
    int id = newTask_id;
    char c;

    /* Check if its possible to create the new task */
    if (id - 1 >= MAX_ID) {
        printf("too many tasks\n");
        return 1;
    }


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
/*
    i = 0;
    c = args[1][i];
    while (c != '\0') {
        newtask.description[i] = c;
        i++;
        c = args[1][i];
    }  
    newtask.description[i] = '\0';
*/
    /* Check if there are no tasks with the same description */
    for (i = 0; i < id - 1; i++) {
        if (!(strcmp(tasks[i].description, newtask.description))) {
            printf("duplicate description\n");
            return 1;
        }
    }

    /* Set remaining task values */ 
    newtask.id = newTask_id++;
    newtask.previewTime = n;
    newtask.time = 0;
    n = strlen(TO_DO);
    for (i = 0; i < n; i++) {
        newtask.state[i] = TO_DO[i];
        printf("string %s\n", newtask.state);
    }
    newtask.state[i] = '\0';
    tasks[newTask_id - 2] = newtask;

    /* Print output to stdout */
    printf("task %d\n", newtask.id);
    return 0;
}


int listTasks()
{
    /* Define local variables */
    int i, j, l[MAX_ID], num_id, equal;
    num_id = inputlistTasks(l);

    /* Write requested tasks */
    for(i = 1; i <= num_id; i++) {
        equal = 0;
        for(j = 0; (j < newTask_id - 1) && !equal; j++) {
            if (l[i] == tasks[j].id) {
                printf("%d %s #%d %s\n", tasks[j].id, tasks[j].state, tasks[j].time, tasks[j].description);
                equal = 1;
            }
        }
        if (!equal) {
            printf("%d: no such task", l[i]);
            return 1;
        }
    } 

    if (num_id == 0) {
        for(j = 0; j < newTask_id - 1; j++) {
            printf("%d %s #%d %s\n", tasks[j].id, tasks[j].state, tasks[j].time, tasks[j].description);
        }
    }

    return 0;
}


int inputlistTasks(int l[]) 
{
    /* Local variables */
    int num_id = 0, fora = 0, id = 0, i = 0;
    char c;

    /* Read ids' from stdin */
    c = getchar();
    while (c != '\n') {
        if ('0' <= c && c <= '9') {
            if (fora) {
                fora = 0;
                num_id++;
            }
            id = id * 10 + (c - '0');
        }

        if (!fora && c == ' ') {
            l[i++] = id;
            fora = 1;
            id = 0;
        }

        c = getchar();
    }
    l[i] = id;

    return num_id;
}

int increaseTime()
{
    int t = 0, i, fora = 0;
    char c;

    c = getchar();
    while (c != '\n') {
        if (!fora && c == ' ') {
            fora = 1;
        }

        if (fora && c != ' ') {
            fora = 0;
        }
        if (!fora) {
            if ('0' <= c && c <= '9') {
                t = t * 10 + (c - '0');
            }
            else {
                printf("invalid time\n"); 
                return 1;
            }
        }
        c = getchar();
            
    }

    if (t != 0) {
        currentTime += t;
        for (i = 0; i < newTask_id - 1; i++) {
            if (strcmp(tasks[i].state, TO_DO)) {
                tasks[i].time += t;
            }
        }
    }

    printf("%d\n", currentTime);
    return 0;
}


int addUser()
{
    int i;
    char u[MAX_USR_ATV];

    inputString(u, 'u');

    if (strlen(u) == 0) {
        for (i = 0; i < currentUsers; i++) {
            printf("%s\n", usrs[i]);
        }
        return 0;
    }

    for (i = 0; i < currentUsers; i++) {
        if (!(strcmp(usrs[i], u))) {
            printf("user already exists\n");
            return 1;
        }
    }

    if (currentUsers + 1 > MAX_DESC_USERS) {
        printf("too many users");
        return 1;
    }

    strcopy(u, usrs[currentUsers]);
    currentUsers++;
    return 0;
}


/*    int fora = 1, i = 0, cont = 0;
    char c, n[MAX_USR_ATV + 1];
    c = getchar();
    if (c == '\n') {
        for (i = 0; i < currentUsers; i++) {
            printf("%s\n", usrs[i]);
        }
    }

    c = getchar();
    while(c != '\n' && (cont < 2) && i < MAX_USR_ATV) {
        if (fora && c != ' ') {
            cont++;
            fora = 0;
        }

        if (!fora && c == ' ') {
            fora = 1;
        }

        if (!fora) {
            n[i++] = c;
        }

        c = getchar();
    }

    n[i] = '\0';
    if (cont == 0) {
        for (i = 0; i < currentUsers; i++) {
            printf("%s\n", usrs[i]);
        }
    }

    for (i = 0; i < currentUsers; i++) {
        if (!(strcmp(usrs[i], n))) {
            printf("user already exists\n");
            return 1;
        }
    }

     Check if its possible to create the new task */
/*    if (currentUsers + 1 > MAX_DESC_USERS) {
        printf("too many users\n");
        return 1;
    }

    i = 0;
    while(n[i] != '\0') {
        usrs[currentUsers][i] = n[i];
        i++;
    }

    usrs[currentUsers++][i] = '\0';

    return 0;
}
*/

int moveActivity()
{
    int i, in = 0, l;
    int id = 0;
    char u[MAX_USR_ATV], a[MAX_USR_ATV];

    inputIds(&id);
    inputString(u, 'u');
    inputString(a, 's');

    if (id >= newTask_id) {
        printf("no such task\n");
        return 1;
    }

    if ((strcmp(TO_DO, tasks[id-1].state)) && !(strcmp(TO_DO, a))) {
        printf("task already started\n");
        return 1;
    }

    for (i = 0; i < currentUsers && !in; i++) {
        if (!(strcmp(usrs[i], u))) {
            in = 1;
        }
    }

    if (!in) {
        printf("no such user\n");
        return 1;
    }
    
    in = 0;
    l = current_activities;
    for (i = 0; i < l && !in; i++) {
        printf("%d %d %s %s\n", i, l, activities[i], u);
        if (!(strcmp(activities[i], a))) {
            in = 1;
        }
    }

    if (!in) {
        printf("no such activity\n");
        return 1;
    }

    if (!strcmp(tasks[id-1].state, TO_DO)) {
        tasks[id-1].start = currentTime;
    }
    strcopy(a, tasks[id-1].state);
    

    if (!(strcmp(a, DONE))) {
        printf("duration=%d slack=%d\n", tasks[id-1].time, tasks[id-1].time - tasks[id-1].previewTime);
    }

    return 0;
}

int teste()
{

    char n[MAX_USR_ATV], u[MAX_USR_ATV];

    inputString(u, 'u');
    inputString(n, 's');

    printf("n:%s u:%s\n", n, u);

    return 0;
}

int inputString(char s[], char command)
{
    int i = 0, fora = 1;
    char c;

    c = getchar();
    while(c != '\n' && (command == 'u' ? (c != ' ' || fora) : 1)) {
        if (fora && c != ' ') {
            fora = 0;
        }
        if (!fora) {
            s[i++] = c;
        }
        c = getchar();
    }

    s[i] = '\0';
    return (c == ' ' ? 1 : 0);
}

int inputIds(int *n)
{
    int i = *n, fora = 1;
    char c;

    c = getchar();
    while ((c != ' ' || fora) && c != '\n') {
        if (fora && c != ' ') {
            fora = 0;
        }
        if (!fora) {
            i = i * 10 + (c - '0');
        }
        c = getchar();
    }
    *n = i;
    return (c == ' ' ? 1 : 0);
}


int convertToInt(char s[])
{
    int i, n;

    n = 0;
    i = 0;
    while(s[i] != '\0') {
        n = n * 10 + (s[i] - '0');
    }

    return n;
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

int taskInActivity()
{
    int num = 0, i, n, hold, j, t, all[MAX_ID];
    char a[MAX_USR_ATV], desc[MAX_DESC_USERS];

    inputString(a, 's');

    n = newTask_id - 1;
    for (i = 0; i < n; i++) {
        if (!strcmp(tasks[i].state, a)) {
            all[num++] = i; 
        }
    }

    for (i = 1; i < num; i++) {
        hold = all[i];
        t = tasks[all[i]].start;
        j = i - 1;
        while(t < tasks[all[j]].start && j >= 0) {
            printf("order num:%d %d\n", t, tasks[all[j]].start);
            all[j+1] = all[j];
            j--;
        }
        all[j + 1] = hold;
    }

    for (i = 0; i < num; i++) {
        printf("%d\n", all[i]);
    }

    for (i = 1; i < num; i++) {
        j = i-1;
        hold = all[i];
        strcopy(tasks[all[i]].description, desc);
        while((strcmp(tasks[all[j]].description, desc) > 0) && (tasks[all[j]].start == tasks[all[i]].start) && j >= 0) {
            all[j+1] = all[j];
            j--;
        }
        all[j + 1] = hold;
    }

    for (i = 0; i < num; i++) {
        printf("%d %d %s\n", tasks[all[i]].id, tasks[all[i]].start, tasks[all[i]].description);
    }

    return 0;
}






























/*
int getInput(int input[][50], int command)
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
                input[args][0] = input[args][0] * 10 + (c - '0');
                printf("args:%d c:%c input:%d\n", args, c, input[args][0]);
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

    return args;
}
*/