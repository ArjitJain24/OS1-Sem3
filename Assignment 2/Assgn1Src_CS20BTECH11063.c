#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    //If number of arguments is not 2 then throws an error and terminates the program
    if(argc != 2)
    {
        printf("Incorrect number of inputs\n");
        printf("Input: final_exec n (where n is the number)\n");
        return EXIT_FAILURE;
    }
    //Our number n on which Colatz conjecture will be tested
    int n = atoi(argv[1]);
    //If n is less than zero then ask for input again
    while(n <= 0)
    {
        printf("Please enter number greater than 0: ");
        scanf("%d", &n);
    }

    //forking a child process
    pid_t pid = fork();
    if (pid < 0)
    {
        //If error occurs
        fprintf(stderr, "Fork Failed\n");
        return EXIT_FAILURE;
    }
    else if (pid == 0)
    {
        //Child Process
        printf("Child Process is working: ");
        printf("%d ", n);
        while (n != 1)
        {
            //If n is even it is divided by 2
            if (n % 2 == 0)
            {
                n = n / 2;
            }
            //else n is 3n+1
            else
            {
                n = 3 * n + 1;
            }
            printf("%d ", n);
        }
        printf("\nChild Process is completed\n");
    }
    else
    {
        //Parent Process
        //Parent waits for child process to be completed
        printf("Parent Process is waiting for Child Process\n");
        wait(NULL);
        printf("Parent Process is completed\n");
    }
    return EXIT_SUCCESS;
}


    // //Getting input from user for a positive integer
    // while (n <= 0)
    // {
    //     printf("Enter an integer greater than 0: ");
    //     scanf("%d", &n);
    // }
