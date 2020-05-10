#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "header1.h"
#include "header2.h"

//MSC00-A
int main()
{
    //Clear screen
    system("clear");

    String answer;  //Try to remove this variable if time permits
    nullRemove del;
    
    SIGNIN input;
    input = (SIGNIN)malloc(sizeof(SIGNIN));

    int flag;
    int count = 3;

    green();
    printf("HI THERE!\n\n");

    //FIO37-C
    while (count > 0)
    {
        blue();
        printf("Would you like to login or register?\n[login/register]: ");
        reset();
        if (fgets(answer, sizeof answer, stdin) == NULL)
        {
            red();
            printf("\nError, input invalid.\n");
        }
        else
        {
            del = strchr(answer, '\n');
            if (del)
            {
                *del = '\0';
            }
        }

        flag = BOFcheck(answer, strlen(answer), sizeof answer);

        if (flag == 0)
        {
            --count;
        }
        else if (flag == 1)
        {
            count = 0;
        }

        system("clear");
    }

    if (flag == 1 && count == 0)
    {
        strcpy(input->answer, answer);

        //Welcome message and login/register process
        welcome(input);

        system("clear");

        //Creating the subject plan for the 4 years
        createSubjectOrder();
    }
    else
    {
        printf("\nMax limit reached! Try again later.\n");
        sleep(5);
    }

    free(input);
    input = NULL;

    //End of program
    return 0;
}
