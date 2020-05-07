#include <stdio.h>
#include <stdlib.h>
#include "header1.h"
#include "header2.h"

//MSC00-A
int main()
{
    //Clear screen
    system("clear");

    //Welcome message and login/register process
    welcomeMessage();

    system("clear");

    //Creating the subject plan for the 4 years
    createSubjectOrder();

    //End of program
    return 0;
}
