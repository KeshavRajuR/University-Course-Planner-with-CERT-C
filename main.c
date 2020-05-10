#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <termios.h>

#include "header1.h"
#include "header2.h"

//I-M-P-O-R-T-A-N-T//
//Need to change exit(0) to return and have a implementation in client.c
//Need to free all the malloc and null the variable and fclose all files

#define MAXPW 20

strings username[10];
strings passwd[MAXPW];

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

int BOFcheck(char *answer, int inputSize, int inputSpace)
{
    //Clear input overflow from fgets if any
    if (inputSize >= inputSpace - 1)
    {
        while ((*answer = getchar()) != '\n' && *answer != EOF)
        {
            //No code here//
        }

        red();
        printf("\nError, input too long.\n");
        sleep(2);
        return 0;
    }
    return 1;
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

CHKCREDS chkAccount(CHKCREDS chk)
{
    //ARR31-C  //STR05-A
    int result;

    FILE *credsFile;
    int bufferLength = 100;
    char buffer[bufferLength];

    credsFile = fopen("creds.txt", "r");

    //DCL09-A
    if (credsFile == NULL)
    {
        red();
        fprintf(stderr, "\nPROGRAM RAN INTO ERROR \nERROR VALUE : %d \nERROR MESSAGE : %s\n", errno, strerror(errno));
        chk->flag = -1;
        return chk;
    }
    else
    {
        // FIO37-C  STR31-C
        while (fgets(buffer, bufferLength, credsFile))
        {
            result = strcmp(chk->login_creds, buffer);
            if (result == 0)
            {
                fclose(credsFile);
                chk->flag = 1;
                return chk;
            }
        }
        fclose(credsFile);
        chk->flag = 0;
        return chk;
    }
}

VALIDATECREDS credValidity(VALIDATECREDS valid)
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
        valid->flag = -1;
        return valid;
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
                valid->flag = 0;
                return valid;
            }
        }
    }

    fclose(checkUser);

    if (strlen(username) > 10 || strlen(passwd) > 20)
    {
        red();
        printf("\nEither your username or password is longer than required.\nTry again\n");
        valid->flag = 0;
        return valid;
    }
    else
    {
        for (int i = 0; i < strlen(username); i++)
        {
            if (username[i] == ' ')
            {
                red();
                printf("\nYour username contains a space. This is not allwed!!\nTry again\n");
                valid->flag = 0;
                return valid;
            }
        }
        for (int i = 0; i < strlen(passwd); i++)
        {
            if (passwd[i] == ' ')
            {
                red();
                printf("\nYour password contains a space. This is not allwed!!\nTry again\n");
                valid->flag = 0;
                return valid;
            }
        }
        red();
        printf("\nUsername and Password looks good. Proceeding to create the account.\n");
        valid->flag = 1;
        return valid;
    }
}

USERCREDS login(USERCREDS creds)
{
    int loginCount = 3;
    int result;

    int count = 0;

    CHKCREDS chk;
    chk = (CHKCREDS)malloc(sizeof(CHKCREDS));

    for (int i = 0; i < strlen(creds->Username); i++)
    {
        if (creds->Username[i] == '_')
        {
            count = count + 1;
        }
    }

    sanitizeInput(creds->Username);

    for (int i = 0; i < strlen(creds->Username); i++)
    {
        if (creds->Username[i] == '_')
        {
            count = count - 1;
        }
    }

    if (count != 0)
    {
        red();
        printf("\nThere are invalid/special characters in your username.\n");
        creds->flag = 0;
        return creds;
    }
    else
    {
        for (int i = 0; i < strlen(creds->Passwd); i++)
        {
            if (creds->Passwd[i] == ' ')
            {
                red();
                printf("\nYour password contains a space. This is not allwed!!\nTry again\n");
                creds->flag = 0;
                return creds;
            }
        }

        strcpy(chk->login_creds, creds->Username);
        strcat(chk->login_creds, ":");
        strcat(chk->login_creds, creds->Passwd);
        strcat(chk->login_creds, "\n");

        chkAccount(chk);
        if (chk->flag == 1)
        {
            system("clear");
            cyan();
            printf("Login successful!! Welcome %s\n", username);
            creds->flag = 1;
            return creds;
        }
        else if (chk->flag == -1)
        {
            creds->flag = -1;
            return creds;
        }
        else
        {
            red();
            printf("\nLogin failed! Try again.\n\n");
            return 0;
        }
    }
}

REGISTERUSERS registerUser(REGISTERUSERS reg)
{
    int count = 0;
/*
    FILE *newUser;
    newUser = fopen("creds.txt", "a");
    if (newUser == NULL)
    {
        red();
        fprintf(stderr, "\nPROGRAM RAN INTO ERROR \nERROR VALUE : %d \nERROR MESSAGE : %s\n", errno, strerror(errno));
        reg->flag = -1;
        return reg;
    }
*/

    int registerCount = 5;

    for (int i = 0; i < strlen(reg->Username); i++)
    {
        if (reg->Username[i] == '_')
        {
            count = count + 1;
        }
    }

    sanitizeInput(reg->Username);

    for (int i = 0; i < strlen(reg->Username); i++)
    {
        if (reg->Username[i] == '_')
        {
            count = count - 1;
        }
    }

    printf("\nUsername = %s\nPasswd1 = %s\nPasswd2 = %s\nDifference = %d\n", reg->Username, reg->Passwd1, reg->Passwd2, strcmp(reg->Passwd1, reg->Passwd2));

    if (count != 0)
    {
        red();
        printf("\nThere are invalid/special characters in your username.\n");
        reg->flag = 0;
        return reg;
    }
    else
    {
        if (strcmp(reg->Passwd1, reg->Passwd2) == 0)
        {
            VALIDATECREDS validate;
            validate = (VALIDATECREDS)malloc(sizeof(VALIDATECREDS));

            strcpy(validate->Username, reg->Username);
            strcpy(validate->Passwd, reg->Passwd2);

            //ENV04-A
            system("clear");
            //STR00-A   STR01-A

            printf("fhdskjlhflshflahflsdhalf");

            sleep(5);

            credValidity(validate);

            printf("fdhskfhdsjkfhsdkjf");

            sleep(5);

            if (validate->flag == -1)
            {
                reg->flag = -1;
                return reg;
            }
            else if (validate->flag == 0)
            {
                reg->flag = 0;
                return reg;
            }
            else if (validate->flag == 1)
            {
                strcat(reg->login_creds, validate->Username);
                strcat(reg->login_creds, ":");
                strcat(reg->login_creds, validate->Passwd);
                strcat(reg->login_creds, "");
                //fprintf(newUser, "%s\n", reg->login_creds);
                //fclose(newUser);

                cyan();
                printf("Your account has been created. Please go ahead and login.\n");
                sleep(5);

                system("clear");
            }
            printf("The password is the same");
            reg->flag = 1;
            return reg;
        }
        else
        {
            red();
            printf("\nYour password doesn't match. Enter carefully.\n");
            reg->flag = 0;
            return reg;
        }
    }
}

bool prefix(const char *pre, const char *str)
{
    return strncmp(pre, str, strlen(pre)) == 0;
}

int studentInfo()
{
    int flag = 0;
    char *nullRemove;

    strings ans[3];

    strings fname[10];
    strings lname[10];

    strings *name = (char *)malloc(20 * sizeof(char));
    strings *SRN = (char *)malloc(13 * sizeof(char));

    blue();
    printf("Enter your first Name: ");
    reset();
    if (fgets(fname, sizeof fname, stdin) == NULL)
    {
        red();
        printf("\nError, input invalid.\n");
    }
    else
    {
        nullRemove = strchr(fname, '\n');
        if (nullRemove)
        {
            *nullRemove = '\0';
        }
    }

    blue();
    printf("Enter your last Name: ");
    reset();
    if (fgets(lname, sizeof lname, stdin) == NULL)
    {
        red();
        printf("\nError, input invalid.\n");
    }
    else
    {
        nullRemove = strchr(lname, '\n');
        if (nullRemove)
        {
            *nullRemove = '\0';
        }
    }

    //Getting some information about the user.
    //The number before the s is to make sure that scanf will only read that many characters.
    //This helps us avoid stack smashing
    //MEM05-A
    blue();
    printf("Enter your SRN: ");
    reset();
    scanf("%14s", SRN);

    sanitizeInput(fname);
    sanitizeInput(lname);

    strcpy(name, fname);
    strcat(name, " "); //we are intentionally adding this space to store it properly. This is not an issue as any other unwanted spaces will be removed by the sanitizeInput function
    strcat(name, lname);

    //sanitizeInput(name);
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

SIGNIN welcome(SIGNIN input)
{
    int flag = 1;
    nullRemove del;

    FILE *passInput = stdin;
    ssize_t passHide = 0;

    USERCREDS creds;
    creds = (USERCREDS)malloc(sizeof(USERCREDS));
    char *loginPassword = creds->Passwd;

    system("clear");

    if (!strcmp(input->answer, "login"))
    {

        int loginRetries = 3;
        while (loginRetries > 0)
        {
            blue();
            printf("Enter your username: ");
            reset();
            if (fgets(creds->Username, sizeof creds->Username, stdin) == NULL)
            {
                red();
                printf("\nError, input invalid.\n");
                exit(0);
            }
            else
            {
                del = strchr(creds->Username, '\n');
                if (del)
                {
                    *del = '\0';
                }
            }

            flag = BOFcheck(creds->Username, strlen(creds->Username), sizeof creds->Username);

            if (flag == 1)
            {
                blue();
                printf("Enter your password: ");
                reset();
                passHide = getpasswd(&loginPassword, MAXPW, '*', passInput);

                flag = BOFcheck(creds->Passwd, strlen(creds->Passwd), sizeof creds->Passwd);

                if (flag == 1)
                {
                    login(creds);
                    if (creds->flag == 0)
                    {
                        --loginRetries;
                        sleep(5);
                    }
                    else if (creds->flag == 1)
                    {
                        loginRetries = 0;
                    }
                    else
                    {
                        exit(0);
                    }
                }
            }
            system("clear");
        }
        if (loginRetries == 0 && (creds->flag == -1 || creds->flag == 0))
        {
            system("clear");
            red();
            printf("\nMax limit reached!! Try again later.\n\n");
            exit(0);
        }
    }
    else if (!strcmp(input->answer, "register"))
    {
        REGISTERUSERS reg;
        reg = (REGISTERUSERS)malloc(sizeof(REGISTERUSERS));

        char *registerPassword1 = reg->Passwd1;
        char *registerPassword2 = reg->Passwd2;

        int registerRetries = 5;
        while (registerRetries > 0)
        {
            cyan();
            printf("Let's create your login credentials!!! \nRules for username and password:\n");
            printf("1) Username cannot be longer than 10 characters\n2) Password cannot be longer than 20 characeters\n3) No spaces in the username or password\n\n");

            blue();
            printf("Enter your username: ");
            reset();
            if (fgets(reg->Username, sizeof reg->Username, stdin) == NULL)
            {
                red();
                printf("\nError, input invalid.\n");
            }
            else
            {
                del = strchr(reg->Username, '\n');
                if (del)
                {
                    *del = '\0';
                }
            }

            flag = BOFcheck(reg->Username, strlen(reg->Username), sizeof reg->Username);

            if (flag == 1)
            {
                blue();
                printf("Enter your password: ");
                reset();
                passHide = getpasswd(&registerPassword1, MAXPW, '*', passInput);

                flag = BOFcheck(reg->Passwd1, strlen(reg->Passwd1), sizeof reg->Passwd1);

                if (flag == 0)
                {
                    red();
                    printf("\nError, input too long\n");
                    return 0;
                }

                printf("\n");

                blue();
                printf("Enter your password: ");
                reset();
                passHide = getpasswd(&registerPassword2, MAXPW, '*', passInput);

                flag = BOFcheck(reg->Passwd2, strlen(reg->Passwd2), sizeof reg->Passwd2);

                printf("\nPasswd1 = %s\nPasswd2 = %s\nDifference = %d\n", reg->Passwd1, reg->Passwd2, strcmp(reg->Passwd1, reg->Passwd2));

                //sleep(2);

                registerUser(reg);

                printf("\nEnd of register\n");

                if (reg->flag == 0)
                {
                    --registerRetries;
                }
                else if (reg->flag == 1)
                {
                    registerRetries = 0;
                }
                else
                {
                    exit(0);
                }

                printf("\nEnd of register if statement\n");
            }
            //system("clear");
        }
        printf("\nRegister retry max limit if statement\n");
        if (registerRetries == 0 && (reg->flag == -1 || reg->flag == 0))
        {
            system("clear");
            red();
            printf("\nMax limit reached!! Try again later.\n\n");
            exit(0);
        }
        printf("\nStart of login in register\n");
        if (registerRetries == 0 && reg->flag == 1)
        {
            printf("\nStart of login retry\n");
            int loginRetries = 3;
            while (loginRetries > 0)
            {
                printf("\nStart of login\n");

                strcpy(creds->Username, reg->Username);
                strcpy(creds->Passwd, reg->Passwd1);

                sleep(5);

                login(creds);

                printf("\nEnd of login\n");

                if (creds->flag == 0)
                {
                    --loginRetries;
                }
                else if (creds->flag == 1)
                {
                    loginRetries = 0;
                }
                else
                {
                    exit(0);
                }
            }
            printf("\nLogin retry max limit if statement\n");
            if (loginRetries == 0 && (creds->flag == -1 || creds->flag == 0))
            {
                system("clear");
                red();
                printf("\nMax limit reached!! Try again later.\n\n");
                exit(0);
            }
        }
    }
    else
    {
        red();
        printf("\nInvalid input.\n");
        exit(0);
    }

    if (1 == 1)
    {
        cyan();
        printf("WELCOME TO PES UNIVERSITY\n\n");

        while (flag)
        {
            flag = studentInfo();

            system("clear");
        }
    }
    else
    {
        exit(0);
    }

    printf("\nEnd of function\n");

    //free(creds);
    //creds = NULL;
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

    for (int i = 0; i < numberofsubjects; i++)
    {
    }

    Graph *graph = createGraph(numberofsubjects);
    cyan();
    printf("\nEnter the prerequisite in case there is any.");
    printf("Enter -1 -1 to end.\n");
    printf("Format: \nYou will have to enter the value of the subject i.e the serial number from the above list.\n");
    printf("PRE-REQUISITE<SPACE>SUBJECT\n");

    flag = 1;
    while (flag)
    {
        int prereq, subject;

        blue();
        printf("\nEnter subject code and its pre-requisite: "); //Not able to choose random subjects. This has to be fixed.
        reset();
        scanf("%d %d", &prereq, &subject);

        green();
        printf("%d --> %d\n", prereq, subject);

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
