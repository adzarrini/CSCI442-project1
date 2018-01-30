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
		return -1; // Why do I need this for the tests to pass
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
  // cout << "alias called" << endl; // delete when implemented
  if(argv.size() > 2) {
  	cerr << "Too many arguments" << endl;
  	return -1;
  }
  if(argv.size() == 1) {
  	for(pair<string, string> p : aliases) {
  		cout << "alias " << p.first << "=" << p.second << endl;
  	}
  }
  else {
  	string temp = argv[1];
  	size_t index = 0;
  	if ((index = temp.find("=")) == string::npos) {
  		cerr << "Invalid format" << endl;
  		return -1;
  	}

  	string key = temp.substr(0,index);
  	string val = temp.substr(++index, temp.size());
  	aliases[key] = val;
  }

  return 0;
}


int Shell::com_unalias(vector<string>& argv) {
  // Implemented
  
  if (argv.size() != 2) {
  	cerr << "unalias: usage: unalias [-a] name [name ...]" << endl;
  	return -1;
  }
  if (argv[1] == "-a") {
  	aliases.clear();
  	return 0;
  }
  if (aliases.erase(argv[1]) == 0) {
  	cerr << "bash: unalias: " << argv[1] << ": not found" << endl;
  	return -1;
  }

  return 0;
}


int Shell::com_echo(vector<string>& argv) {
  // Implemented
  
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
