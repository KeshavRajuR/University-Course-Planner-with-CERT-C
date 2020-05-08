#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <termios.h>

#include "header1.h"
#include "header2.h"

#define MAXPW 20

creds username[10];
creds passwd[MAXPW];

void red()
{
    printf("\033[1;31m");
}

void green()
{
    printf("\033[1;32m");
}

void yellow()
{
    printf("\033[1;33m");
}

void blue()
{
    printf("\033[1;34m");
}

void magenta()
{
    printf("\033[1;35m");
}

void cyan()
{
    printf("\033[1;36m");
}

void reset()
{
    printf("\033[0m");
}

/* read a string from fp into pw masking keypress with mask char.
getpasswd will read upto sz - 1 chars into pw, null-terminating
the resulting string. On success, the number of characters in
pw are returned, -1 otherwise.
*/
ssize_t getpasswd(char **pw, size_t sz, int mask, FILE *fp)
{
    if (!pw || !sz || !fp)
        return -1; /* validate input   */
#ifdef MAXPW
    if (sz > MAXPW)
        sz = MAXPW;
#endif

    if (*pw == NULL)
    { /* reallocate if no address */
        void *tmp = realloc(*pw, sz * sizeof **pw);
        if (!tmp)
            return -1;
        memset(tmp, 0, sz); /* initialize memory to 0   */
        *pw = (char *)tmp;
    }

    size_t idx = 0; /* index, number of chars in read   */
    int c = 0;

    struct termios old_kbd_mode; /* orig keyboard settings   */
    struct termios new_kbd_mode;

    if (tcgetattr(0, &old_kbd_mode))
    { /* save orig settings   */
        fprintf(stderr, "%s() error: tcgetattr failed.\n", __func__);
        return -1;
    } /* copy old to new */
    memcpy(&new_kbd_mode, &old_kbd_mode, sizeof(struct termios));

    new_kbd_mode.c_lflag &= ~(ICANON | ECHO); /* new kbd flags */
    new_kbd_mode.c_cc[VTIME] = 0;
    new_kbd_mode.c_cc[VMIN] = 1;
    if (tcsetattr(0, TCSANOW, &new_kbd_mode))
    {
        fprintf(stderr, "%s() error: tcsetattr failed.\n", __func__);
        return -1;
    }

    /* read chars from fp, mask if valid char specified */
    while (((c = fgetc(fp)) != '\n' && c != EOF && idx < sz - 1) ||
           (idx == sz - 1 && c == 127))
    {
        if (c != 127)
        {
            if (31 < mask && mask < 127) /* valid ascii char */
                fputc(mask, stdout);
            (*pw)[idx++] = c;
        }
        else if (idx > 0)
        { /* handle backspace (del)   */
            if (31 < mask && mask < 127)
            {
                fputc(0x8, stdout);
                fputc(' ', stdout);
                fputc(0x8, stdout);
            }
            (*pw)[--idx] = 0;
        }
    }
    (*pw)[idx] = 0; /* null-terminate   */

    /* reset original keyboard  */
    if (tcsetattr(0, TCSANOW, &old_kbd_mode))
    {
        fprintf(stderr, "%s() error: tcsetattr failed.\n", __func__);
        return -1;
    }

    if (idx == sz - 1 && c != '\n') /* warn if pw truncated */
        fprintf(stderr, " (%s() warning: truncated at %zu chars.)\n",
                __func__, sz - 1);

    return idx; /* number of chars in passwd    */
}

const char *sanitizeInput(char *text)
{
    //STR02-A
    static strings ok_chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890_-.@";
    char *cp = text;
    const char *end = text + strlen(text);
    for (cp += strspn(cp, ok_chars); cp != end; cp += strspn(cp, ok_chars))
    {
        *cp = '_';
    }

    return text;
}

int chkAccount(char *username, char *passwd)
{
    //ARR31-C  //STR05-A
    char login_creds[100];
    int result;

    for (int i = 0; i < strlen(passwd); i++)
    {
        if (passwd[i] == ' ')
        {
            red();
            printf("\nYour password contains a space. This is not allwed!!\nTry again\n");
            return 1;
        }
    }

    strcpy(login_creds, username);
    strcat(login_creds, ":");
    strcat(login_creds, passwd);
    strcat(login_creds, "\n");

    FILE *check;
    int bufferLength = 100;
    char buffer[bufferLength];

    check = fopen("creds.txt", "r");

    //DCL09-A
    if (check == NULL)
    {
        red();
        fprintf(stderr, "\nPROGRAM RAN INTO ERROR \nERROR VALUE : %d \nERROR MESSAGE : %s\n", errno, strerror(errno));
        return -1;
    }
    else
    {
        // FIO37-C  STR31-C
        while (fgets(buffer, bufferLength, check))
        {
            result = strcmp(login_creds, buffer);
            if (result == 0)
            {
                return result;
            }
        }
        return result;
    }
    fclose(check);
}

int credValidity(char *username, char *passwd)
{
    int flagInput = 0;

    char *existingUser;

    FILE *checkUser;
    int bufferLength = 100;
    char buffer[bufferLength];

    checkUser = fopen("creds.txt", "r");

    //DCL09-A
    if (checkUser == NULL)
    {
        red();
        fprintf(stderr, "\nPROGRAM RAN INTO ERROR \nERROR VALUE : %d \nERROR MESSAGE : %s\n", errno, strerror(errno));

        return -1;
    }
    else
    {
        // FIO37-C  STR31-C
        while (fgets(buffer, bufferLength, checkUser))
        {
            existingUser = strtok(buffer, ":");
            if (!strcmp(username, buffer))
            {
                red();
                printf("\nThis user already exists. Try a different username.\n");
                sleep(5);
                system("clear");
                return 0;
            }
        }
    }

    fclose(checkUser);

    if (strlen(username) > 10 || strlen(passwd) > 20)
    {
        red();
        printf("\nEither your username or password is longer than required.\nTry again\n");
        return flagInput;
    }
    else
    {
        for (int i = 0; i < strlen(username); i++)
        {
            if (username[i] == ' ')
            {
                red();
                printf("\nYour username contains a space. This is not allwed!!\nTry again\n");
                return flagInput;
            }
        }
        for (int i = 0; i < strlen(passwd); i++)
        {
            if (passwd[i] == ' ')
            {
                red();
                printf("\nYour password contains a space. This is not allwed!!\nTry again\n");
                return flagInput;
            }
        }
        red();
        printf("\nUsername and Password looks good. Proceeding to create the account.\n");
        return !flagInput;
    }
}

int login()
{
    int loginCount = 3;
    int result;

    char *nullRemove;
    int count = 0;

    char pw[MAXPW] = {0};
    char *pass = pw;
    FILE *input = stdin;
    ssize_t passHide = 0;

    while (loginCount > 0)
    {
        count = 0;

        blue();
        printf("Enter your username: ");
        reset();
        if (fgets(username, sizeof username, stdin) == NULL)
        {
            red();
            printf("\nError, input too long.\n");
        }
        else
        {
            nullRemove = strchr(username, '\n');
            if (nullRemove)
            {
                *nullRemove = '\0';
            }
        }

        for (int i = 0; i < strlen(username); i++)
        {
            if (username[i] == '_')
            {
                count = count + 1;
            }
        }

        sanitizeInput(username);

        for (int i = 0; i < strlen(username); i++)
        {
            if (username[i] == '_')
            {
                count = count - 1;
            }
        }

        if (count != 0)
        {
            red();
            printf("\nThere are invalid/special characters in your username.\n");
            --loginCount;
        }
        else
        {
            blue();
            printf("Enter your password: ");
            reset();
            passHide = getpasswd(&pass, MAXPW, '*', input);

            system("clear");

            result = chkAccount(username, pass);
            if (result == 0)
            {
                cyan();
                printf("Login successful!! Welcome %s\n", username);
                return 0;
            }
            else if (result == -1)
            {
                loginCount = 0;
            }
            else
            {
                red();
                printf("\nLogin failed! Try again.\n\n");
            }

            --loginCount;
        }
    }
    if (loginCount == 0)
    {
        system("clear");
        red();
        printf("\nMax limit reached!! Try again later.\n\n");
    }
    exit(0);
}

int registerUser()
{
    int count = 0;
    char *nullRemove;

    char pw[MAXPW] = {0};
    FILE *input = stdin;
    char *pass1 = pw;
    char *pass2 = pw;
    ssize_t passHide = 0;

    FILE *newUser; //File has been opened by newuser later in the code...
    int registerCount = 5;

    cyan();
    printf("Let's create your login credentials!!! \nRules for username and password:\n");
    printf("1) Username cannot be longer than 10 characters\n2) Password cannot be longer than 20 characetersr\n3) No spaces in the username or password\n\n");

    while (registerCount > 0)
    {
        blue();
        printf("Enter your username: ");
        reset();
        if (fgets(username, sizeof username, stdin) == NULL)
        {
            red();
            printf("\nError, input too long.\n");
        }
        else
        {
            nullRemove = strchr(username, '\n');
            if (nullRemove)
            {
                *nullRemove = '\0';
            }
        }

        for (int i = 0; i < strlen(username); i++)
        {
            if (username[i] == '_')
            {
                count = count + 1;
            }
        }

        sanitizeInput(username);

        for (int i = 0; i < strlen(username); i++)
        {
            if (username[i] == '_')
            {
                count = count - 1;
            }
        }

        if (count != 0)
        {
            red();
            printf("\nThere are invalid/special characters in your username.\n");
            --registerCount;
        }
        else
        {
            blue();
            printf("Enter your password: ");
            reset();
            passHide = getpasswd(&pass1, MAXPW, '*', input);

            printf("\n");

            blue();
            printf("Enter your password again: ");
            reset();
            passHide = getpasswd(&pass2, MAXPW, '*', input);

            if (!strcmp(pass1, pass2))
            {
                strcpy(passwd, pass2);

                //ENV04-A
                system("clear");
                //STR00-A   STR01-A
                int checkUser = credValidity(username, passwd);

                if (checkUser == -1)
                {
                    return 0;
                }
                else if (checkUser == 0)
                {
                    --registerCount;
                }
                else if (checkUser == 1)
                {
                    char user[30];
                    strcat(user, username);
                    strcat(user, ":");
                    strcat(user, passwd);
                    strcat(user, "");
                    newUser = fopen("creds.txt", "a");
                    fprintf(newUser, "%s\n", user);
                    fclose(newUser);

                    cyan();
                    printf("Your account has been created. Please go ahead and login.\n");
                    sleep(5);

                    system("clear");

                    login();

                    return 0;
                    --registerCount;
                }
            }
            else
            {
                red();
                printf("\nYour password doesn't match. Enter carefully.\n");
                --registerCount;
            }
        }
    }
    red();
    printf("\nLooks like you have too many wrong attempts. Try again later.\n");
    exit(0);
}

bool prefix(const char *pre, const char *str)
{
    return strncmp(pre, str, strlen(pre)) == 0;
}

int studentInfo()
{
    int flag = 0;
    strings ans[3];

    strings *name = (char *)malloc(20 * sizeof(char));
    strings *SRN = (char *)malloc(13 * sizeof(char));

    //Getting some information about the user.
    //The number before the s is to make sure that scanf will only read that many characters.
    //This helps us avoid stack smashing
    //MEM05-A
    blue();
    printf("Enter your Full Name: ");
    reset();
    scanf(" %[^\n]s ", name);

    blue();
    printf("Enter your SRN: ");
    reset();
    scanf("%14s", SRN);

    sanitizeInput(name);
    sanitizeInput(SRN);

    for (int i = 0; i < strlen(name); i++)
    {
        if (name[i] == '_')
        {
            red();
            printf("\nIssue in Name: Please do not enter special characters in your name.\n");
            i = strlen(name);
            flag = 1;
            sleep(5);
        }
        if (isdigit(name[i]))
        {
            red();
            printf("\nIssue in Name: Do not enter numbers in your name. We know your name doesn't have any numbers.\n");
            i = strlen(name);
            flag = 1;
            sleep(5);
        }
    }

    if (strlen(SRN) == 13)
    {
        int srnCheck = prefix("PES", SRN);
        if (srnCheck == 1)
        {
            for (int i = 0; i < strlen(SRN); i++)
            {
                if (SRN[i] == '_')
                {
                    red();
                    printf("\nIssue in SRN: Special characters are not part of your ID. Do not enter them.\n");
                    i = strlen(SRN);
                    flag = 1;
                    sleep(5);
                }
                if (i >= 3 && !isdigit(SRN[i]))
                {
                    red();
                    printf("\nIssue in SRN: You have added more letters than present in the format. Stick to the SRN format.\n");
                    i = strlen(SRN);
                    flag = 1;
                    sleep(5);
                }
            }
        }
        else
        {
            red();
            printf("\nIssue in SRN: Your SRN doesn't have this format. Enter it exactly as given to you.\n");
            flag = 1;
            sleep(5);
        }
    }
    else
    {
        red();
        printf("\nIssue in SRN: The entered SRN is not of required length. Enter carefully.\n");
        flag = 1;
        sleep(5);
    }

    if (flag == 0)
    {
        cyan();
        printf("\nYour information will be printed here, make sure it is correct.\n");
        printf("Name: %s\tSRN: %s\n", name, SRN);
        
        blue();
        printf("\nIs everything correct?\n[yes/no]: ");
        reset();
        scanf("%3s", ans);

        if (!strcmp(ans, "yes"))
        {
            flag = 0;
        }
        else
        {
            flag = 1;
        }
    }

    return flag;
}

void welcomeMessage()
{
    int flag = 1;
    char *nullRemove;

    strings answer[10];

    green();
    printf("HI THERE!\n\n");
    blue();
    printf("Would you like to login or register?\n[login/register]: ");
    reset();

    //FIO37-C
    if (fgets(answer, sizeof answer, stdin) == NULL)
    {
        red();
        printf("\nError, input too long");
    }
    else
    {
        nullRemove = strchr(answer, '\n');
        if (nullRemove)
        {
            *nullRemove = '\0';
        }
    }

    system("clear");

    if (!strcmp(answer, "login"))
    {
        login();
    }
    else if (!strcmp(answer, "register"))
    {
        registerUser();
    }
    else
    {
        red();
        printf("\nInvalid input.\n");
        exit(0);
    }

    cyan();
    printf("WELCOME TO PES UNIVERSITY\n\n");

    while (flag)
    {
        flag = studentInfo();

        system("clear");
    }

    reset();
}

void createSubjectOrder()
{
    cyan();
    //Finding his choice of subjects and their order.
    printf("BELOW ARE THE COURSES WE OFFER FOR CSE DEPARTMENT\n\n");

    reset(); // or use some other color
    printf("\n1) Physics\n2) Mathematics 1\n3) Python\n4) Electrical Engineering\n5) Mechanical Engineering\n");
    printf("6) Chemistry\n7) Mathematics 2\n8) C\n9) Electronic Engineering\n10) Mechanics\n11) Biotechnology\n");
    printf("12) Digital Design and Computer Organization\n13) Data Structures\n14) Introduction to Data Science\n15) Web Techonologies 1\n16) Discrete Mathematics and Logic\n17) Special Topic 1\n");
    printf("18) Linear Algebra and It's Application\n19) Design and Analysis of Algorithms\n20) Data Base Management Systems\n21) Microprocessor and Computer Architecture\n22) Theory of Computation\n23) Special Topic 2\n");
    printf("23) Computer Networks\n24) Introduction to Operating Systems\n25) Principles of Programming Languages\n");
    printf("26) Advanced Algorithms\n27) Advanded Data Base Management Systems\n28) Big Data\n29) Multimedia Computing\n");
    printf("30) Information Security\n31) Web Services\n32) Algorithms for Intelligent Web\n");
    
    red();
    printf("\nNOTE:\n");
    cyan();
    printf("Only 4 subjects can be taken during a semester. The next set of subject no matter how many there may be will be conducted in the next semester.\n\n");

    int flag;
    size numberofsubjects = 2;

    flag = 1;
    while (flag)
    {
        blue();
        printf("\n\nEnter the number of subjects: ");
        reset();
        scanf("%d", &numberofsubjects);

        if (numberofsubjects < 3)
        {
            red();
            printf("\nYou need to select at least 3 subjects!");
        }
        else
        {
            flag = 0;
        }
    }

    Graph *graph = createGraph(numberofsubjects);
    cyan();
    printf("\nEnter the prerequisite in case there is any.");
    printf("Enter -1 -1 to end.\n");

    flag = 1;
    while (flag)
    {
        int prereq, subject;

        blue();
        printf("\nEnter subject code and its pre-requisite :");
        reset();
        scanf("%d %d", &prereq, &subject);

        green();
        printf("%d --> %d", prereq, subject);

        if (prereq == -1)
        {
            flag = 0;
        }
        addEdge(graph, subject - 1, prereq - 1);
    }

    cyan();
    //Final sort
    printf("\n\nAll Topological Sort: \n");
    printAllTopologicalOrders(graph);
    //MEM00-A   MEM30-C     MEM31-C
    free(graph);
}
