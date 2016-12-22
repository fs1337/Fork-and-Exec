/*head files*/
#include "stdio.h"
#include "unistd.h"
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

//function declaration
void break_down_line(char *input, char **data, int &num);

/*main function*/
int main()
{
    char *input0 = NULL;
    size_t len = 0;
    size_t flag = 0;
    char *argv[6];
    int i = 0;
    int status;
    int num = 0;

    //init arguments, the maximum number of arguments is 5
    //argv[5] is always NULL
    for (i=0; i<6; i++)
    {
        argv[i]=NULL;
    }

	while(1)
	{
		//get the command_line from the user
		//if errors exist, exit
	    printf("Please input your command:\n");
	    if((flag = getline(&input0, &len, stdin)) == -1)
	    {
	        return 0;
	    }
	    else//break down the input from user
	    {
	        char *input1 = (char *)malloc(len);
	        strcpy(input1, input0);
	        input1[strlen(input1) - 1] = ' '; //change "\n" into " "
	        break_down_line(input1, argv, num);
            free(input1);
	    }

        //if user types "exit", exit
        if(strcmp(argv[0],"exit")==0)
        {
            exit(0);
        }
	    
	    //After read commands from user, create a child process using fork()
	    int pid = fork();
	    
	    //executes the command in the child process
	    //displays the results if any
	    if(pid == 0)
	    {
	        if( execvp(argv[0],argv) < 0)
            {
                exit(1);
            }
            else
            {
                for(i = 0; i < num; i ++)
                {
                    free(argv[i]);
                }
            }
	    }
        else
        {
             //wait for the child process to execute
             while (wait(&status) != pid);
        }
	}

    return 0;
}

//use " " to seperate each parameter for execvp.
void break_down_line(char *input, char **data, int &num)
{
    char *token;
    int count = 0;

    token=strtok(input, " ");
    while (token!=NULL && count < 6)
    {
        data[count] = (char *)malloc(strlen(token) + 1);//malloc( _MAX_PATH )
        strcpy(data[count++],token);
        token=strtok(NULL, " ");
    }
    num = count - 1;
}
