#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "header1.h"
#include "header2.h"

//MSC00-A
int main()
{
    system("clear");

    int flag = 1;
    int numberofsubjects = 2;
    char answer[10];
    char srn[14], name[20];
    char ans[3];

    printf("HI THERE!\n\n");

    printf("Would you like to login or register?\n[login/register]: ");
    scanf("%s", answer);

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
        printf("Invalid input.");
        exit(0);
    }

    printf("WELCOME TO PES UNIVERSITY\n");

    //Getting some information about the user.
    //The number before the s is to make sure that scanf will only read that many characters.
    //This helps us avoid stack smashing
    //MEM05-A
    while (flag)
    {
        printf("Enter your Full Name: ");
        scanf(" %[^\n]s ", name);

        printf("Enter your SRN: ");
        scanf("%14s", srn);

        printf("Your information will be printed here, make sure it is correct.\n");
        printf("Name: %s\tSRN: %s\n", name, srn);
        printf("\nIs everything correct?\n[yes/no]: ");
        scanf("%3s", ans);

        if (!strcmp(ans, "yes"))
        {
            flag = 0;
        }
    }
    
    system("clear");

    //Finding his choice of subjects and their order.
    printf("BELOW ARE THE COURSES WE OFFER FOR CSE DEPARTMENT\n\n\n");
    printf("\n1) Physics\n2) Mathematics 1\n3) Python\n4) Electrical Engineering\n5) Mechanical Engineering\n");
    printf("6) Chemistry\n7) Mathematics 2\n8) C\n9) Electronic Engineering\n10) Mechanics\n11) Biotechnology\n");
    printf("12) Digital Design and Computer Organization\n13) Data Structures\n14) Introduction to Data Science\n15) Web Techonologies 1\n16) Discrete Mathematics and Logic\n17) Special Topic 1\n");
    printf("18) Linear Algebra and It's Application\n19) Design and Analysis of Algorithms\n20) Data Base Management Systems\n21) Microprocessor and Computer Architecture\n22) Theory of Computation\n23) Special Topic 2\n");
    printf("23) Computer Networks\n24) Introduction to Operating Systems\n25) Principles of Programming Languages\n");
    printf("26) Advanced Algorithms\n27) Advanded Data Base Management Systems\n28) Big Data\n29) Multimedia Computing\n");
    printf("30) Information Security\n31) Web Services\n32) Algorithms for Intelligent Web\n");
    printf("Only 4 subjects can be taken during a semester. The next set of subject no matter how many there may be will be conducted in the next semester.\n\n");
    
    flag = 1;
    while(flag)
    {
        printf("\n\nEnter the number of subjects :\n");
        scanf("%d", &numberofsubjects);
        
        if(numberofsubjects < 3)
        {
            printf("You need to select at least 3 subjects!");
        }
    }

    Graph *graph = createGraph(numberofsubjects);
    printf("Enter the prerequisite in case there is any.");
    printf("Enter -1 -1 to end.\n");

    flag = 1;
    while (flag)
    {
        int a, b;
        printf("Enter subject code and its pre-requisite :\n");
        scanf("%d %d", &a, &b);

        if (a == -1)
        {
            flag = 0;
        }
        addEdge(graph, b - 1, a - 1);
    }
    //Final sort
    printf("All Topological Sort: \n");
    printAllTopologicalOrders(graph);

    return 0;
}
