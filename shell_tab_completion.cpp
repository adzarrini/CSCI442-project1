/**
 * This file contains implementations of the functions that provide
 * tab-completion for the shell.
 *
 * You will need to finish the implementations of the completion functions,
 * though you're spared from implementing the high-level readline callbacks with
 * their weird static variables...
 */

#include "shell.h"
#include <cstdlib>
#include <iostream>
#include <readline/readline.h>
#include <readline/history.h>

#include <dirent.h>
#include <unistd.h>

using namespace std;


void Shell::get_env_completions(const char* text, vector<string>& matches) {
  // TODO: implement

  // ENV tab completions
  get_ENV_completions(text, matches);
  // Iterate through local vars but append $ to string before comparing to text
  get_local_completions(text, matches);

}


void Shell::get_command_completions(const char* text, vector<string>& matches) {
  // TODO: implement

  // Iterate through builtins and see if text contains any of the keys
  get_builtin_completions(text, matches);
  // Iterate through alias and see if text contains any of the keys
  get_alias_completions(text, matches);
  // External commands
  get_external_completions(text,matches);

}


char** Shell::word_completion(const char* text, int start, int end) {
  char** matches = NULL;

  if (text[0] == '$') {
    matches = rl_completion_matches(text, env_completion_generator);
  } else if (start == 0) {
    matches = rl_completion_matches(text, command_completion_generator);
  } else {
    // We get directory matches for free (thanks, readline!).
  }

  return matches;
}


char* Shell::env_completion_generator(const char* text, int state) {
  // A list of all the matches.
  // Must be static because this function is called repeatedly.
  static vector<string> matches;

  // If this is the first time called, construct the matches list with
  // all possible matches.
  if (state == 0) {
    getInstance().get_env_completions(text, matches);
  }

  // Return a single match (one for each time the function is called).
  return pop_match(matches);
}


char* Shell::command_completion_generator(const char* text, int state) {
  // A list of all the matches.
  // Must be static because this function is called repeatedly.
  static vector<string> matches;

  // If this is the first time called, construct the matches list with
  // all possible matches.
  if (state == 0) {
    getInstance().get_command_completions(text, matches);
  }

  // Return a single match (one for each time the function is called).
  return pop_match(matches);
}


char* Shell::pop_match(vector<string>& matches) {
  if (matches.size() > 0) {
    const char* match = matches.back().c_str();

    // Delete the last element.
    matches.pop_back();

    // We need to return a copy, because readline deallocates when done.
    char* copy = (char*) malloc(strlen(match) + 1);
    strcpy(copy, match);

    return copy;
  }

  // No more matches.
  return NULL;
}

// Helper functions
void Shell::get_ENV_completions(const char* text, vector<string>& matches) {
  char** env = environ;
  // Iterate through env make sure it is not null
  while(*env != NULL) {
    string temp = (string) *env;
    size_t pos = temp.find("=");
    string look = "$" + temp.substr(0, pos);
    // find where = and append $ to substring. Check it it equals text 
    if(look.find((string) text) == 0) {
      matches.push_back(look);
    }
    ++env;
  }
}

void Shell::get_local_completions(const char* text, vector<string>& matches) {
  map<string, string>::iterator lvars;
  for (lvars = localvars.begin(); lvars != localvars.end(); ++lvars) {
    string look = "$" + (lvars->first);
    if (look.find((string) text) == 0) {
      matches.push_back(look);
    }
  }
}

void Shell::get_builtin_completions(const char* text, vector<string>& matches){
  map<string, builtin_t>::iterator builtin;
  for (builtin = builtins.begin(); builtin != builtins.end(); ++builtin) {
    if ((builtin->first).find((string) text) == 0) {
      matches.push_back(builtin->first);
    }
  }
}

void Shell::get_alias_completions(const char* text, vector<string>& matches) {
  map<string, string>::iterator alias;
  for (alias = aliases.begin(); alias != aliases.end(); ++alias) {
    if ((alias->first).find((string) text) == 0) {
      matches.push_back(alias->first);
    }
  }
}

void Shell::get_external_completions(const char* text, vector<string>& matches) {
  DIR *dir;
  struct dirent *direct;
  string path = getenv("PATH");
  size_t pos;
  while((pos = path.find(":")) != string::npos) {
    if ((dir = opendir(path.substr(0, pos).c_str())) != NULL) {
      while((direct = readdir(dir)) != NULL) {
        string look = direct->d_name;
        if(look == "." || look == "..") continue;
        if(look.find((string) text) == 0) {
          matches.push_back(look);
        }
      }
    }
    path.erase(0,pos+1);
  }
}