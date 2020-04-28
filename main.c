#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "header1.h"
#include "header2.h"

creds username[10];
creds passwd[20];

int chkAccount(char *username, char *passwd)
{
    //ARR31-C  //STR05-A
    char login_creds[100];
    int result;

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
        fprintf(stderr, "PROGRAM RAN INTO ERROR \nERROR VALUE : %d \nERROR MESSAGE : %s\n", errno, strerror(errno));

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
}

int credValidity(char *username, char *passwd)
{
    int flagInput = 0;

    if (strlen(username) > 10 || strlen(passwd) > 20)
    {
        printf("Either your username or password is longer than required.\nTry again\n");
        return flagInput;
    }
    else
    {
        for (int i = 0; i < strlen(username); i++)
        {
            if (username[i] == ' ')
            {
                printf("Your username contains a space. This is not allwed!!\nTry again\n");
                return flagInput;
            }
        }
        for (int i = 0; i < strlen(passwd); i++)
        {
            if (passwd[i] == ' ')
            {
                printf("Your password contains a space. This is not allwed!!\nTry again\n");
                return flagInput;
            }
        }

        printf("Username and Password looks good. Proceeding to create the account.\n");
        return !flagInput;
    }
}

int registerUser()
{
    FILE *newUser;
    int registerCount = 5;
    printf("Let's create your login credentials. Rules for username and password:\n");
    printf("1) Username cannot be longer than 10 characters\n2) Password cannot be longer than 20 characetersr\n3) No spaces in the username or password\n");

    while (registerCount != 0)
    {
        printf("Enter your username: ");
        scanf(" %[^\n]s ", username);
        printf("Enter your password: ");
        scanf(" %[^\n]s ", passwd);
        //ENV04-A
        system("clear");
        //STR00-A   STR01-A
        int check = credValidity(username, passwd);
        if (check == 1)
        {
            char user[30];
            strcat(user, username);
            strcat(user, ":");
            strcat(user, passwd);
            strcat(user, "");
            newUser = fopen("creds.txt", "a");
            fprintf(newUser, "%s\n", user);
            fclose(newUser);

            system("clear");

            printf("Your account has been created. Please go ahead and login.\n");

            login();

            return 0;
        }
        --registerCount;
    }
    printf("Looks like you have too many wrong attempts. Try again later.\n");
    exit(0);
}

int login()
{
    int loginCount = 3;
    int result;

    while (loginCount >= 0)
    {
        printf("Enter your username: ");
        scanf(" %10[^\n]s ", username);
        printf("Enter your password: ");
        scanf(" %20[^\n]s ", passwd);

        system("clear");

        result = chkAccount(username, passwd);
        if (result == 0)
        {
            printf("Login successful!! Welcome %s\n", username);
            return 0;
        }
        else if (result == -1)
        {
            loginCount = 0;
        }
        else
        {
            printf("Login failed! Try again.\n");
        }

        --loginCount;
    }
    if (loginCount == 0)
    {
        printf("Max limit reached!! Try again later.\n");
    }
    exit(0);
}

int studentInfo()
{
    //STR02-A
    static strings ok_chars_name[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890_-.@";
    static strings ok_chars_srn[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890_-.@";

    int flag = 0;
    strings SRN[14], name[20];
    strings ans[3];

    //Getting some information about the user.
    //The number before the s is to make sure that scanf will only read that many characters.
    //This helps us avoid stack smashing
    //MEM05-A
    printf("Enter your Full Name: ");
    scanf(" %[^\n]s ", name);

    printf("Enter your SRN: ");
    scanf("%14s", SRN);

    strings *namePointer = name;
    strings *srnPointer = SRN;

    const strings *end_name = name + strlen(name);
    const strings *end_srn = SRN + strlen(SRN);

    for (namePointer += strspn(namePointer, ok_chars_name); namePointer != end_name; namePointer += strspn(namePointer, ok_chars_name))
    {
        *namePointer = '_';
    }

    for (srnPointer += strspn(srnPointer, ok_chars_srn); srnPointer != end_srn; srnPointer += strspn(srnPointer, ok_chars_srn))
    {
        *srnPointer = '_';
    }

    for (int i = 0; i < strlen(name); i++)
    {
        if (name[i] == '_')
        {
            printf("Looks like you have entered invalid characters. You can have only letters in your name.\n");
            i = strlen(name);
            flag = 1;
            sleep(5);
        }
    }

    for (int i = 0; i < strlen(SRN); i++)
    {
        if (SRN[i] == '_')
        {
            printf("Looks like you have entered invalid characters. You have to follow the format of the college.\n");
            i = strlen(SRN);
            flag = 1;
            sleep(5);
        }
    }

    if(flag == 0)
    {
        printf("Your information will be printed here, make sure it is correct.\n");
        printf("Name: %s\tSRN: %s\n", name, SRN);
        printf("\nIs everything correct?\n[yes/no]: ");
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
