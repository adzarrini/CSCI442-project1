/**
 * This file contains the implementations of the builtin functions provided by
 * the shell (or will, once you've finished implementing them all).
 */

#include "shell.h"
#include <iostream>

using namespace std;


int Shell::com_ls(vector<string>& argv) {
  // TODO: YOUR CODE GOES HERE
  cout << "ls called" << endl; // delete when implemented
  return 0;
}


int Shell::com_cd(vector<string>& argv) {
  // TODO: YOUR CODE GOES HERE
  cout << "cd called" << endl; // delete when implemented
  return 0;
}


int Shell::com_pwd(vector<string>& argv) {
  // Implemented
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
