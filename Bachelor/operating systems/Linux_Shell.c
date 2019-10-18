#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
//#include <errno.h> another library for errors


int num; //Global variable to count the characters of a command line
int num2; //Global variable to count the number of pointers of argv

//----------------------------------------------------------------------------------------------------------------------

void countline(char *line) //Function declaration and definition 
{	num =0;

	while(*line != '\0') //Enters the loop as long as the content of the array element isn't a NULL terminator
	{
			line++; // Proceeds to the next element of the array
			num++; //Increments the counter value
	}
}

//----------------------------------------------------------------------------------------------------------------------

void parse(char *line, char **argv)
// Parses the line array into a vector of arguments
{	num2=0; //Initialization for the arguments counter
	while(*line != '\0')
	{	
		while(*line == ' ' || *line == '\t') // Replace separators by terminators
		{	
			*line = '\0'; 
			line++;
		}

		
		if (*line == '"') // This if case is used to remove the " " and keep what's inside unchanged
		{
			*line = '\0';// Removes the first "
			line++; // Increments the line 
		    *argv = line; // Saves the first character even if it is space or tab
		    argv++;
		    num2++;
			while (*line != '"') // Ignore the next characters till the other "
			{
				line++;
			}
			*line='\0'; // Removes the second "
			line++; // Increment the line
		
		}
		
			
		*argv = line; // Save the pointer to the argument
		argv++; //Increments for the up comming command/argument
		num2++; //counts the number of pointers
		
		while(*line != ' ' && *line != '\t' && *line != '\0') //This while loop is used to ignore the other characters of a command/word
		{													  //except for the first character which is saved
			if (*line == '\n')
				{*line = '\0';}
			
			line++;
			

		}
	}
	*argv = '\0'; //Makes the last argv as null for the execvp command 
}


//----------------------------------------------------------------------------------------------------------------------


//Forground execution
void executef(char **argv)
// Forks a child process to run execvp
{  	
	if (strcmp(argv[num2-1], "\0")==0 || strcmp(argv[num2-1], " ")==0) //To prevent an empty command line making an error
	argv[num2-1]= '\0';
	
	
	pid_t pid;
	int status;
	pid = fork();
	if(pid < 0)
	// Fork failed
	{
		printf("Error, no fork\n");
	}
	else if (pid == 0)
	// In child process
	{   if (execvp(argv[0], argv) < 0)
		{perror("Error");
        /*printf("Error: %s\n",strerror(errno) );*/ //another way for errors
        exit(1);
			
		}
		else
		execvp(argv[0], argv);
	}
	else 
	// In parent process
	{	waitpid(pid, &status, 0);

	}
	
}

//----------------------------------------------------------------------------------------------------------------------


//Background execution
void executeb(char **argv)
// Forks a child process to run execvp
{	
	if (strcmp(argv[num2-1], "\0")==0 || strcmp(argv[num2-1], " ")==0) //To prevent an empty command line making an error
		    argv[num2-1]= '\0';
	pid_t pid; 
	pid = fork();
	if(pid < 0)
	// Fork failed
	{
		printf("Error, no fork\n");
	}
	else if (pid == 0)
	// In child process
	{
	    
		if (execvp(argv[0], argv) < 0)
		{perror("Error");
          /*printf("Error: %s\n",strerror(errno) );*/ //another way for errors
			exit(1);
		}
          else
		execvp(argv[0], argv);
      
	}
}
	

//----------------------------------------------------------------------------------------------------------------------

int main()
{	
	char line[512]; // An array used to get the input considering that a command length should not exceed 512 characters
	char *argv[128]; // An array of pointers used to separate the command's words
	// Assuming that the average number of characters per word is 4 characters
	
	while(1)
// Main loop to ensure that the shell program continuously executes
	{
		printf("Shell> "); //New line indicator
		fgets(line, 512, stdin); // Getting the input from keyboard "Standard input"
		countline (line); //Counts the characters entered by the user

//This for loop is made to check if the user enters a line of spaces to prevent core dumping
//and it has another use; 
//the while loop inside it is used to remove any space after the entered command
//to help finding the & character in the last character position

	for(int i = 0 ; i<num-2 ; i++) //Accessing all elements of the array
	{
		if(line[i]!=' ' && line[i]!='\t') //to check the first entered -non space or tab- character
										 //indication that the line is not a line of spaces
		
		{
			while (line[num-2]== ' ' ||line[num-2]=='\t') //This while loop is used to remove any additional spaces after the command
														  //It stops at the last -non space of tab- character
			{
			line[num-2]='\0';
			num--;
			}
		}
	} 
		
		if (line[num-2]=='&')
		{
		line[num-2]= '\0';
		parse(line, argv); //Converting the one-dimensional array into a two-dimensional array (i.e. Separating the command's words)
		if(strcmp(argv[0], "cd") == 0)
		// An example of a shell built-in
			chdir(argv[1]);
		else if (strcmp(argv[0], "exit") == 0) 
                     //exit(0);  
                     num = num;
	         else   
		              executeb(argv);
		      		
		}
		else
		{
		parse(line, argv); //Converting the one-dimensional array into a two-dimensional array (i.e. Separating the command's words)
		if(strcmp(argv[0], "cd") == 0)
		// An example of a shell built-in
			chdir(argv[1]);
		else if (strcmp(argv[0], "exit") == 0) 
                     exit(0);                  
		 else   
		              executef(argv);
		      		}
	
	
		}
	
return 0;
}