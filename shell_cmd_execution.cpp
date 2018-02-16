/**
 * This file contains implementations of the functions that are responsible for
 * executing commands that are not builtins.
 *
 * Much of the code you write will probably be in this file. Add helper methods
 * as needed to keep your code clean. You'll lose points if you write a single
 * monolithic function!
 */

#include "shell.h"
#include <iostream>

#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <typeinfo>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>

using namespace std;

int Shell::wait_child_process(command_t& cmd, int in_pipe[2], int out_pipe[2]) {
	// Make sure pipe is in READ_FROM_PIPE before closing
	if (cmd.input_type == READ_FROM_PIPE && (close(in_pipe[0]) == -1)) {
		perror("closing error");
		exit(EXIT_FAILURE);
	}	

	if (cmd.output_type == WRITE_TO_PIPE) {
		in_pipe[0] = out_pipe[0];

		if (close(out_pipe[1]) == -1) {
			perror("closing error");
			exit(EXIT_FAILURE);
		}
	}	

	int return_val = 0;
	if (wait(&return_val) == 1) {
		perror("waiting error");
		exit(EXIT_FAILURE);
	}

	if(WIFEXITED(return_val) == 1) {
		return_val = WEXITSTATUS(return_val);
	}	
	
	return return_val;
}

void Shell::execute_child_process(command_t& cmd, int in_pipe[2], int out_pipe[2]) {

	int fd;
	vector<char *> argv = cmd.get_cstring_argv();

	switch(cmd.input_type) {

		case READ_FROM_FILE:
		// command reading to a file
			fd = open(cmd.infile.c_str(), O_RDONLY);
			if (fd == -1) exit(EXIT_FAILURE);
			dup2(fd, STDIN_FILENO);
			break;

		case READ_FROM_PIPE:
		// comman reading from pipe
			dup2(in_pipe[0], STDIN_FILENO);
			break;

		default:
		// READ_FROM_STDIN is default
			break;
	}

	switch(cmd.output_type) {

		case WRITE_TO_PIPE:
		// If command is writing to pipe
			dup2(out_pipe[1], STDOUT_FILENO);
			break;

		case WRITE_TO_FILE:
		// command writing to a file
			fd = open(cmd.outfile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1) exit(EXIT_FAILURE);
			dup2(fd, STDOUT_FILENO);
			break;
		
		case APPEND_TO_FILE:
		// command appending to a file
			fd = open(cmd.outfile.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1) exit(EXIT_FAILURE);
			dup2(fd, STDOUT_FILENO);
			break;

		default:
		// WRITE_TO_STDIN is defualt
			break;
	}
	// Execute command and check for error
	if (execvp(argv[0], &argv[0]) == -1) {
		cout << "exec error" << endl;
		perror(argv[0]);
		exit(EXIT_FAILURE);
	}
}


int Shell::execute_external_command(vector<string>& tokens) {
  // Implemented with help from Josh Thomas in class

	vector<command_t> commands;
	int cpid, in_pipe[2], out_pipe[2];
	int return_val = 0;

	if (!partition_tokens(tokens, commands)) return 1;

	for (size_t i = 0; i < commands.size(); i++) {
		command_t& cmd = commands[i];

		// Check for piping failures
		// Only pipe of output type is write to  pipe
		// must be before child process is forked so that parent 
		// process can share the read end with the next command
		if (cmd.output_type == WRITE_TO_PIPE && pipe(out_pipe) == -1) {
			perror("Piping process failed!");
			exit(EXIT_FAILURE);
		}
		// Check for forking failures
		if ((cpid = fork()) == -1) {
			perror("Forking process failed!");
			exit(EXIT_FAILURE);
		}

		// Check if pipe successful, determine if need to wait or execute
		if (cpid == 0) {
			execute_child_process(cmd, in_pipe, out_pipe);
		}
		else {
			return_val = wait_child_process(cmd, in_pipe, out_pipe);
		}
	}
	return return_val;
}


