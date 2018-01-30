/**
 * This file contains the implementations of the builtin functions provided by
 * the shell (or will, once you've finished implementing them all).
 */

#include "shell.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>


using namespace std;


int Shell::com_ls(vector<string>& argv) {
	// Implemented
  
  if(argv.size() > 2) {
  	cerr << "Too many arguments" << endl;
  	return -1;
  }

	DIR *dir;
	struct dirent *direct;

	if (argv.size() == 1) {
		dir = opendir("./");
	}
	else {
		dir = opendir(argv[1].c_str());
	}

	if (dir != NULL) {
		while((direct = readdir(dir)) != NULL) {
			printf("%s\n", direct->d_name);
		}
		closedir(dir);
	}
	else {
		string message = argv[0] + ": cannot access '" + argv[1] + "'";
		perror(message.c_str());
		exit(-1);
	}

  return 0;
}


int Shell::com_cd(vector<string>& argv) {
  // Implemented
	int returnVal = chdir(getenv("HOME"));;

  if(argv.size() > 2) {
  	cerr << "Too many arguments" << endl;
  	return -1;
  }
  if (argv.size() == 2) {
		returnVal = chdir(argv[1].c_str());
	}
	if (returnVal != 0) {
		string message = "bash: " + argv[0] + " " + argv[1];
		perror(message.c_str());
		exit(-1);
	}

  
  return returnVal;
}


int Shell::com_pwd(vector<string>& argv) {
  // Implemented
  
  if (argv.size() > 1) {
  	cerr << "Too many arguments" << endl;
  	return -1;
  }
  system("echo $PWD");
  return 0;
}


int Shell::com_alias(vector<string>& argv) {
  // TODO: YOUR CODE GOES HERE
  cout << "alias called" << endl; // delete when implemented
  return 0;
}


int Shell::com_unalias(vector<string>& argv) {
  // TODO: YOUR CODE GOES HERE
  cout << "unalias called" << endl; // delete when implemented
  return 0;
}


int Shell::com_echo(vector<string>& argv) {
  // TODO: YOUR CODE GOES HERE
  
  for (unsigned int i = 1; i < argv.size(); i++) {
	cout << argv[i] + " ";
  }
  cout << endl;

  return 0;
}


int Shell::com_history(vector<string>& argv) {
  // TODO: YOUR CODE GOES HERE
  cout << "history called" << endl; // delete when implemented
  return 0;
}


int Shell::com_exit(vector<string>& argv) {
  // Implemented
  exit(0);
  return 0;
}
