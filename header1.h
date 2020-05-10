#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

//DCL12-A   //DCL05-A
typedef int counter;
typedef int StackTop;
typedef int size;
typedef char strings;

/* =========================================== */

typedef struct node //DCL05-A
{
    int vertex;
    struct node *next;
} node;

typedef struct node* NODE;

/* =========================================== */

typedef struct Graph //DCL05-A
{
    //DCL32-C   //DCL02-A
    int numVertices;
    bool *visited;
    int *indegree;
    node **adjLists;
} Graph;

typedef struct Graph* GRAPH;

/* =========================================== */

typedef char option[10];
typedef char String[10];
typedef char* nullRemove;

typedef struct signIn
{
    option answer;
} signIn;

typedef struct signIn* SIGNIN;

/* =========================================== */

#define MAXPW 20
typedef char ID[10];
typedef char pass[MAXPW];
typedef int loginFlag;

typedef struct userCreds
{
    ID Username;
    pass Passwd;
    loginFlag flag;
}userCreds;

typedef struct userCreds* USERCREDS;

/* =========================================== */

typedef char loginCreds[30];
typedef int chkFlag;

typedef struct chkCreds
{
    loginCreds login_creds;
    chkFlag flag;
}chkCreds;

typedef struct chkCreds* CHKCREDS;

/* =========================================== */

//typedef for ID, pass and loginCreds already declared
typedef int registerFlag;

typedef struct registerUsers
{
    ID Username;
    pass Passwd1;
    pass Passwd2;
    loginCreds login_creds;
    registerFlag flag;
}registerUsers;

typedef struct registerUsers* REGISTERUSERS;

/* =========================================== */

typedef int registerLoginFlag;

typedef struct registerLogin
{
    ID Username;
    pass Passwd;
    registerLoginFlag flag;
}registerLogin;

typedef struct registerLogin* REGISTERLOGIN;

/* =========================================== */

//typedef for ID and pass already declared
typedef int validFlag;

typedef struct validateCreds
{
    ID Username;
    pass Passwd;
    validFlag flag;
}validateCreds;

typedef struct validateCreds* VALIDATECREDS;

/* =========================================== */
