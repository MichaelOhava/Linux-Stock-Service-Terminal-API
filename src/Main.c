#include "Operations.h"
void sigint_handler1(int signum);
void sigint_handler2(int signum);
void cleanup();
//globals objects and fd clean function

char Command[] = "ls -l";	
char StdIn[4096] = {0};
char StdOut[50000] = {0};
char StdErr[4096] = {0};
//file discriptors
int infd[2] = {0, 0}; //stdin of fd
int outfd[2] = {0, 0}; //stdout of fd
int errfd[2] = {0, 0}; //stderr of fd

//main function 
int main(int argc, char **argv) {

  int p_id=1,indicator=0,zipunload; 
  int rc = pipe(infd); //IN file descriptor
	if(rc < 0)
	{
		printf("runtime error: %s", strerror(errno));
		return 20;
	}

	rc = pipe(outfd); //OUT file descriptor
	if(rc < 0)
	{
		close(infd[READ_END]);
		close(infd[WRITE_END]);
		printf("runtime error: %s", strerror(errno));
		return 21;
	}

	rc = pipe(errfd); //ERR file descriptor
	if(rc < 0)
	{
		close(infd[READ_END]);
		close(infd[WRITE_END]);

		close(outfd[READ_END]);
		close(outfd[WRITE_END]);
		printf("runtime error: %s", strerror(errno));
		return 22;
	}

    //Unzipping file if exist
    zipunload = CheckAndUnzip();
    if(zipunload==DONE)
    printf("Unloading data from zip successfully!\n");
    else printf("No zipfile was detected, Please creat by using fetch option!\n");
    //process spliting
     p_id = fork(); //splits the process, into the parent and child
     signal(SIGUSR1, FinishedSignal);
      if(p_id == -1)
        {
          printf("problem with using fork() \n");
          cleanup();
          return 2;
        }
  //  }
    if(p_id > 0)
    signal(SIGINT, sigint_handler1); //direct signal to func
    else {signal(SIGINT, sigint_handler2);}


//MAIN LOOP:
    while(1)
    {

    if(p_id > 0) // PARENT PROCESS
    {
       
      while(indicator==0||Check_Validation(StdIn)==false)
      {//VALIDATION CHECK MOVE TO FUNCTION
        indicator=1;
        user_menu();
        scanf("%s", StdIn);
      }
       indicator=0;
       write(outfd[WRITE_END], StdIn , sizeof(StdIn)); //writing command to child in pipe
       kill(p_id, SIGUSR1 );   // signal the child
       pause();                // wait for the child to signal back
       read(infd[READ_END], StdOut, sizeof(StdOut)); //reading response from the pipe of the child to receive answer
       //sleep(3);
       printf("%s \n", StdOut);
       printf("=====================\n");
       memset(StdOut, 0 , sizeof(StdOut));
        if(atoi(StdIn)==5)
       exit(1);
     }

    else if (p_id == 0) //CHILD PROCESS
    {
       pause();                // wait for the signal from the parent
       read(outfd[READ_END], StdIn, sizeof(StdIn));
       int x = atoi(StdIn);
       master_func(x,infd[WRITE_END]);
       fflush(STDIN_FILENO);
      kill(getppid(), SIGUSR1);
      if(x==5){cleanup(); exit(1);}
     
    }

  }
    
}

void cleanup()
{
	close(infd[READ_END]);
	close(infd[WRITE_END]);

	close(outfd[READ_END]);
	close(outfd[WRITE_END]);

	close(errfd[READ_END]);
	close(errfd[WRITE_END]);
}

//Defined signal to ctrl+c => save all data into zip file and kill both chiled and father proccesses
void sigint_handler1(int signum)
{
	printf("\n--got SIGINT--\n");
  prog_four(0,TRUE);
  cleanup();
	exit(1);
}


void sigint_handler2(int signum)
{
  cleanup();
	exit(1);
}