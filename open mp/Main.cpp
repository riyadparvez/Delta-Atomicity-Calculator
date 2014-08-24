#include <iostream>
#include <memory>
#include <string>
#include <tbb/task_scheduler_init.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "DefaultScoreComputer.hpp"
#include "InputProcessor.hpp"

void print_trace() 
{
    char pid_buf[30];
    sprintf(pid_buf, "%d", getpid());
    char name_buf[512];
    name_buf[readlink("/proc/self/exe", name_buf, 511)]=0;
    int child_pid = fork();
    if (!child_pid) 
    {           
        dup2(2,1); // redirect output to stderr
        fprintf(stdout,"stack trace for %s pid=%s\n",name_buf,pid_buf);
        execlp("gdb", "gdb", "--batch", "-n", "-ex", "thread", "-ex", "bt", name_buf, pid_buf, NULL);
        abort(); /* If gdb failed to start */
    } 
    else 
    {
        waitpid(child_pid,NULL,0);
    }
}


int main(int argc, char** argv)
{
	// Initialize Google's logging library.
	google::InitGoogleLogging(argv[0]);
	FLAGS_log_dir = ".";

	//it is needed for TBB data structures
	tbb::task_scheduler_init TBBinit;
	
	try
	{
		std::vector<std::string> filePaths;
		for(int i=1; i<argc; i++)
		{
			filePaths.push_back(std::string(argv[i]));
		}
		std::shared_ptr<Zone::ScoreComputer> computer = std::make_shared<DefaultScoreComputer>();
		InputProcessor processor(filePaths, computer);
		processor.Process();
	}
	catch(const std::exception& ex)
	{
		std::cout<<ex.what()<<std::endl<<std::endl;
		print_trace();
	}
	return 0;
}
