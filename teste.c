/* 
 * File: main.c
 * Author: Tiago Antunes
 * Description: A program that simulates a project task manager in C.
*/
#include <stdio.h>
#include <string.h>

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
    int previewTime; /* expect execution time of the task */
    int start; /* indicates when the task first started being executed */
    int time; /* indicated the execution time of the task */
} Task;

/* Global variables */
int newTask_id = 1;
int currentTime = 0;
int currentUsers = 0;
int current_activities = 3;
char usrs[MAX_DESC_USERS-1][MAX_USR_ATV];
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
void orderByDescription(int l[],char s, int size);
int addUserOrActivity(char c);
void writeAll(char c);
/*
int errorChecking(char u[], char c);
int errorUser(char u[]);
int errorActivity(char u[]);
*/
int errorUserActivity(char u[], char c);



/* Reads commands from stdin and executes the associated action */
int main()
{
    /* Variable that contains the comand */
    char cmd;

    strcopy(TO_DO, activities[0]);
    strcopy(IN_PROGRESS, activities[1]);
    strcopy(DONE, activities[2]);

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
            /*Add user or list all users
            addUser();*/
            addUserOrActivity('u');
            break;

        case 'm':
            /* Move one task from one activity to another */
            moveActivity();
            break;
/*
        case 'o':
            teste();
            break;
*/
        case 'd':
            taskInActivity();
            break;

        case 'a':
            addUserOrActivity('a');
            break;
        }
    }
}

/* Creates a new task */
int addTask()
{
    /* Define local variables */
    Task newtask;
    int n = 0, i = 0;
    int id = newTask_id;
    char c;

    /* Check if its possible to create the new task */
    if (id - 1 >= MAX_ID) {
        printf("too many tasks\n");
        while(c != '\n') {
            c = getchar();
        }
        return 1;
    }

    /* Get input from stdin and divide it into separate arguments */ 
    if (inputIds(&n) == 2) {
        printf("invalid duration\n");
        while(c != '\n') {
            c = getchar();
        }
        return 1;
    }
    inputString(newtask.description, 's');
    
    /* Check if there are no tasks with the same description */
    for (i = 0; i < id - 1; i++) {
        if (!(strcmp(tasks[i].description, newtask.description))) {
            printf("duplicate description\n");
            return 1;
        }
    }

    /* Set remaining task values */ 
    newtask.start = 0;
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

/* Lists the requested tasks in the same order or
   all by alphabetical order if none are requested */ 
int listTasks()
{
    /* Define local variables */
    int i = 0, j, l[MAX_ID] = {0}, n, num_ids, equal, id = newTask_id;

    /* Get the input from stdin */
    n = 1;
    num_ids = 0;
    while(n && (n != -1) && (n != 2)) {
        n = inputIds(&i);
        l[num_ids++] = i;
        i = 0;
    }
    printf("after read\n");

    /* If no tasks are inserted set set num_ids to a negative value */
    if (n == -1) {
        num_ids = -1;
    }

    if (n == 2) {
        printf("%d: no such task\n", l[i]);
        return 1;
    }

    /* Write requested tasks */
    for(i = 0; i < num_ids; i++) {  /* For each argument */
        /* Position in array tasks of the requested task */
        j = l[i] - 1;              
        if (j < id - 1) {
            printf("%d %s #%d %s\n", tasks[j].id, tasks[j].state, tasks[j].time, tasks[j].description);
        }
        else {
            printf("%d: no such task\n", l[i]);
            return 1;
        }
        /*
        equal = 0;
        for(j = 0; (j < id - 1) && !equal; j++) { 
            if (l[i] == tasks[j].id) {
                printf("%d %s #%d %s\n", tasks[j].id, tasks[j].state, tasks[j].time, tasks[j].description);
                equal = 1;
            }
        }
        if (!equal) {
            printf("%d: no such task\n", l[i]);
            return 1;
        }*/
    } 
    printf("before order\n");
    /* Write all tasks in alphabetical order when none are requested */
    if (num_ids == -1) {
        /* Make an array with all of the current positions with tasks in array tasks*/
        for (i = 0; i < id - 1; i++) {
            l[i] = tasks[i].id - 1;
        }
        printf("before before order\n");
        orderByDescription(l, 'n', id - 1);
        printf("after\n");

        for(i = 0; i < id - 1; i++) {
            printf("%d %s #%d %s\n", tasks[l[i]].id, tasks[l[i]].state, tasks[l[i]].time, tasks[l[i]].description);
        }
    }
    return 0;
}

/* Increaments the time of the system */
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
            if ((strcmp(tasks[i].state, TO_DO)) && (strcmp(tasks[i].state, DONE))) {
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

    /* Get input */
    inputString(u, 'u');

    /* Write all users if none are given */
    if (strlen(u) == 0) {
        for (i = 0; i < currentUsers; i++) {
            printf("%s\n", usrs[i]);
        }
        return 0;
    }

    /* Error checking */
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

int moveActivity()
{
    int i, in = 0, l, n;
    int id = 0;
    char u[MAX_USR_ATV], a[MAX_USR_ATV];

    n = inputIds(&id);
    inputString(u, 'u');
    inputString(a, 's');

    if (n == 2) {
        printf("no such task\n");
        return 1;
    }

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
/*
int inputIds(int *n)
{
    int i = *n, fora = 1, cont = 0;
    char c;

    c = getchar();
    while ((c != ' ' || fora) && c != '\n') {
        if (fora && c != ' ') {
            fora = 0;
        }
        if (!fora) {
            i = i * 10 + (c - '0');
            cont++;
        }
        c = getchar();
    }
    if (cont == 0) {
        return -1;
    }
    *n = i;
    return (c == ' ' ? 1 : 0);
}
*/
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
    int num = 0, i, n, hold, j, t, all[MAX_ID], id = newTask_id;
    char a[MAX_USR_ATV];

    inputString(a, 's');

    n = id - 1;
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

    orderByDescription(all, 's', num);

    printf("after\n");

    for (i = 0; i < num; i++) {
        printf("%d %d %s\n", tasks[all[i]].id, tasks[all[i]].start, tasks[all[i]].description);
    }
    return 0;
}

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
            while((j >= 0) && (strcmp(tasks[l[j]].description, desc) > 0) && (s == 's' ? tasks[l[j]].start == tasks[l[i]].start : 1)) {
                l[j+1] = l[j];
                l[j] = hold;
                j--;
            }
            /*
            if (s == 's' ? tasks[l[j]].start == tasks[l[i]].start : 1)
                l[j + 1] = hold; */
        }
}

int addUserOrActivity(char c)
{
    int i;
    char u[MAX_USR_ATV];

    /* Get input */
    inputString(u, c);

    /* Write all users if none are given */
    if (strlen(u) == 0) {
        writeAll(c);
        return 0;
    }

    /* Error checking */
    if(errorUserActivity(u, c)) {
        return 1;
    }
    
    /* Add new user or activity to the global pool */
    if (c == 'u') {
        strcopy(u, usrs[currentUsers]);
        currentUsers++;
    }
    else {
        strcopy(u, activities[current_activities]);
        current_activities++;
    }
    printf("users:%d\n", currentUsers);
    return 0;
}

void writeAll(char c)
{
    int i;

    if (c == 'u') {
        for (i = 0; i < currentUsers; i++) {
             printf("%s\n", usrs[i]);
        }
    }
    else {
        for (i = 0; i < current_activities; i++) {
            printf("%s\n", activities[i]);
        }
    }
}

int errorUserActivity(char u[], char c)
{
    int i, n, fn, b;
    fn = (c == 'u' ? currentUsers : current_activities);
    b = (c == 'u' ? 1 : 0);

    printf("fn:%d\n", fn);

    for (i = 0; i < fn; i++) {
        if (!(strcmp((b ? usrs[i] : activities[i]), u))) {
            (b ? printf("user already exists\n") : 
                 printf("duplicate activity\n"));
            return 1;
        }
    }

    if (!b) {
        n = strlen(u);
        for (i = 0; i < n; i++) {
            if ('a' <= u[i] && 'z' >= u[i]){
                printf("invalid description\n");
             return 1;
            }
        }
    }

    if ((fn + 1) > (b ? MAX_DESC_USERS : MAX_ATV)) {
        (b ? printf("too many users\n") : printf("too many activities\n"));
        return 1;
    }
    return 0;
}

int inputIds(int *n)
{
    int i = *n, fora = 1, cont = 0;
    char c;

    c = getchar();
    while ((c != ' ' || fora) && c != '\n') {
        if (fora && c != ' ') {
            fora = 0;
        }

        if (!fora) {
            if (!('0' <= c && '9' >= c)) {
                return 2;
            }
            i = i * 10 + (c - '0');
            cont++;
        }
        c = getchar();
    }
    if (cont == 0) {
        return -1;
    }
    *n = i;
    return (c == ' ' ? 1 : 0);
}









/*
int errorChecking(char u[], char c)
{
    int i, n;

    if (c == 'u' && errorUser(u)) {
        return 1;
    }

    if (c == 'a' && errorActivity(u)) {
        return 1;
    }

    return 0;
}

int errorUser(char u[])
{
    int i;

    for (i = 0; i < currentUsers; i++) {
        if (!(strcmp(usrs[i], u))) {
            printf("user already exists\n");
            return 1;
        }
    }
    if (currentUsers + 1 > MAX_DESC_USERS) {
        printf("too many users\n");
        return 1;
    }
    return 0;
}

int errorActivity(char u[])
{
    int i, n, fn, b; */
/*    fn = (c == 'u' ? currentUsers : current_activities);
    b = (c == 'u' ? 1 : 0);

    for (i = 0; i < fn; i++) {
        if (!(strcmp((b ? usrs[i] : activities[i]), u))) {
            (b ? printf("user already exists\n") : printf("duplicate activity\n"));
            return 1;
        }
    }

    if (!b) {
        n = strlen(u);
        for (i = 0; i < n; i++) {
            if ('a' <= u[i] && 'z' >= u[i]){
                printf("invalid description\n");
             return 1;
            }
        }
    }

    if (fn + 1 > MAX_ATV) {
        (b ? printf("user already exists\n") : printf("duplicate activity\n"));
        return 1;
    } */
/*    
    for (i = 0; i < current_activities; i++) {
        if (!(strcmp(activities[i], u))) {
            printf("duplicate activity\n");
            return 1;
        }
    }

    n = strlen(u);
    for (i = 0; i < n; i++) {
        if ('a' <= u[i] && 'z' >= u[i]){
            printf("invalid description\n");
            return 1;
        }
    }

    if (current_activities + 1 > MAX_ATV) {
        printf("too many activities\n");
        return 1;
    }
    
    return 0;
}

*/