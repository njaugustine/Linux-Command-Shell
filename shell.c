/* Nick Augustine*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define MAXLINE 40 /* 40 chars per line, per command, should be enough. */

/** The setup() routine reads in the next command line string storing it in the input buffer.
The line is separated into distinct tokens using whitespace as delimiters.  Setup also 
modifies the args parameter so that it holds points to the null-terminated strings  which 
are the tokens in the most recent user command line as well as a NULL pointer, indicating the
end of the argument list, which comes after the string pointers that have been assigned to
args. ***/

void setup(char inputBuff[], char *args[],int *background)
{
    int length,  /* Num characters in the command line */
        i,       /* Index for inputBuff arrray          */
        j,       /* Where to place the next parameter into args[] */
        start;   /* Beginning of next command parameter */

    /* Read what the user enters */


    length = read(STDIN_FILENO, inputBuff, MAXLINE);


    j = 0;
    start = -1;

    if (length == 0)
        exit(0);            /* Cntrl-d was entered, end of user command stream */

    if (length < 0){
        perror("error reading command");
	exit(-1);           /* Terminate with error code of -1 */
    }

    /* Examine every character in the input buffer */
    for (i = 0; i < length; i++) {
        switch (inputBuff[i]){
        case ' ':
        case '\t' :          /* Argument separators */

            if(start != -1){
                args[j] = &inputBuff[start];    /* Set up pointer */
                j++;
            }

            inputBuff[i] = '\0'; /* Add a null char; make a C string */
            start = -1;
            break;

        case '\n':             /* Final char examined */
            if (start != -1){
                args[j] = &inputBuff[start];     
                j++;
            }

            inputBuff[i] = '\0';
            args[j] = NULL; /* No more arguments to this command */
            break;

        case '&':
            *background = 1;
            inputBuff[i] = '\0';
            break;
            
        default :             /* Some other character */
            if (start == -1)
                start = i;
	}
 
    }    
    args[j] = NULL; /* Just in case the input line was > 40 */
    
} 

int main(void)
{
    char inputBuff[MAXLINE]; /* Input buffer  to hold the command entered */
    char *args[MAXLINE/2+1];/* Command line arguments */
    char lastEntry[40];
    char buffer[200];	    /*holds 5 history commands*/
    int background;         /* Equals 1 if a command is followed by '&', else 0 */
    int i;
	int rnum = 0;

    while (1){            /* Program terminates normally inside setup */
	pid_t pid;
	char **command_args_ptr = &args[1];
	background = 0;
	

	printf("CSE2431Sh$ ");  /* Shell prompt */
        fflush(0);
	setup(inputBuff, args, &background);       /* Get next command */
	

	if(!strcmp(inputBuff,"history") || !strcmp(inputBuff, "h"))
	{
		for(i = 0; i < 5; i++){
			printf("%d%", i+1);
			printf("%*s\n", 40, buffer + (i*40));
		}

	}

	else if(!strcmp(inputBuff, "r1"))
	{
		strncpy(lastEntry, buffer, 39);
		lastEntry[39] = '\0';
		printf("CSE2431Sh$ %s\n", lastEntry);
		rnum = 1;
	}
	else if(!strcmp(inputBuff, "r2"))
	{
		strncpy(lastEntry, buffer + 40, 39);
		lastEntry[39] = '\0';
		printf("CSE2431Sh$ %s\n", lastEntry);
		rnum = 1;
	}
			
	else if(!strcmp(inputBuff, "r3"))
	{
		strncpy(lastEntry, buffer + 80, 39);
		lastEntry[39] = '\0';
		printf("CSE2431Sh$ %s\n", lastEntry);
		rnum = 1;		
	}
	else if(!strcmp(inputBuff, "r4"))
	{
		strncpy(lastEntry, buffer + 120, 39);
		lastEntry[39] = '\0';
		printf("CSE2431Sh$ %s\n", lastEntry);
		rnum = 1;
	}
	else if(!strcmp(inputBuff, "r5") || !strcmp(inputBuff, "rr"))
	{
		strncpy(lastEntry, buffer + 160, 39);
		lastEntry[39] = '\0';
		printf("CSE2431Sh$ %s\n", lastEntry);
		rnum = 1;
	}
	else
	{
		//moves everythings up 40 characters
		for(i = 0; i < 160; i++)
		{	
			buffer[i] = buffer[i+40];
		}
		//inserts next command 		
		for(i = 0; i < 40; i++)
		{
			buffer[160 + i] = inputBuff[i];
		}

		buffer[199] = '\0'; // inserts null character

	}



	/*  
	 (1) Fork a child process using fork(),
	 (2) The child process will invoke execvp(),
	 (3) If bkgnd == 0, the parent will wait; 
		otherwise returns to the setup() function. */
	pid = fork();
	if (pid == 0) {
		if(rnum) /*runs if rnum was entered*/
			execvp(lastEntry, args);
		else
			execvp(args[0], args);

		//kills child process on history command
		if(strcmp(inputBuff,"history") || !strcmp(inputBuff, "h"))
			exit(0);
	}
	else {
		if (background == 0) {
			waitpid(pid);
		}
		else sleep(1);
	}
    }
}

