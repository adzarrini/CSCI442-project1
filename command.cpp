/**
 * Contains the implementation of functions declared in command.h.
 */

#include "command.h"
#include <algorithm>
#include <iostream>
#include <iterator>

using namespace std;


bool partition_tokens(vector<string> tokens, vector<command_t>& commands) {
  // TODO: implement me

  // This is optional, but hopefully it helps. Read the Piazza post. =)
	command_t cmd;
	bool cmd_out = false;
	bool cmd_in = false;
	//bool check = true;
	for (unsigned int i = 0; i < tokens.size(); i++) {
		if (tokens[i] == "|") {
			if(cmd_out) {
				cerr << "command has multiple outputs" << endl;
				return false;
			}
			if (check_cmd(tokens, i)) {
				cmd.output_type = WRITE_TO_PIPE;
				commands.push_back(cmd);
				cmd = command_t();
				cmd.input_type = READ_FROM_PIPE;
				cmd.argv.push_back(tokens[i+1]);
				++i;
				cmd_in = true;
			}
			else return false;
		}
		else if (tokens[i] == ">") {
			if(cmd_out) {
				cerr << "command has multiple outputs" << endl;
				return false;
			}
			if (check_cmd(tokens, i)) {
				cmd.output_type = WRITE_TO_FILE;
				cmd.outfile = tokens[i+1];
				++i;
				cmd_out = true;
			}
			else return false;
		}
		else if (tokens[i] == ">>") {
			if(cmd_out) {
				cerr << "command has multiple outputs" << endl;
				return false;
			}
			if (check_cmd(tokens, i)) {
				cmd.output_type = APPEND_TO_FILE;
				cmd.outfile = tokens[i+1];
				++i;
				cmd_out = true;
			}
			else return false;
		}
		else if (tokens[i] == "<") {
			if(cmd_in) {
				cerr << "command has multiple inputs" << endl;
				return false;
			}
			if (check_cmd(tokens, i)) {
				cmd.input_type = READ_FROM_FILE;
				cmd.infile = tokens[i+1];
				++i;
				cmd_in = true;
			}
			else return false;
		}
		else {
			cmd.argv.push_back(tokens[i]);
		}
	}
	commands.push_back(cmd);
	
  return true;
}

bool check_cmd(vector<string>& tokens, unsigned int index) {
	if(index == 0 || index+1 >= tokens.size() || invalid_next(tokens[index+1])) {
		cerr << "Error with input" << endl;
		return false;
	}
	return true;
}
bool invalid_next(string cmd) {
	if(cmd == "|" || cmd == ">" || cmd == ">>" || cmd == "<") return true;
	return false;
}



// Feel free to ignore everything below here. It's just code to allow you to
// cout a command in a nice, pretty format. =)


const char* input_types[] = {
  "READ_FROM_STDIN",
  "READ_FROM_FILE",
  "READ_FROM_PIPE"
};


const char* output_types[] = {
  "WRITE_TO_STDOUT",
  "WRITE_TO_PIPE",
  "WRITE_TO_FILE",
  "APPEND_TO_FILE"
};


ostream& operator <<(ostream& out, const command_t& cmd) {
  copy(cmd.argv.begin(), cmd.argv.end(), ostream_iterator<string>(out, " "));

  out << "\n    input:   " << input_types[cmd.input_type]
      << "\n    output:  " << output_types[cmd.output_type]
      << "\n    infile:  " << cmd.infile
      << "\n    outfile: " << cmd.outfile;

  return out;
}
