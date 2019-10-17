#ifndef MAIN_H
#define MAIN_H

#include <vector>

using namespace std;

typedef struct arguments {
  char* in_file;
  bool sw; // Omega = 0, Delta = 1
} Args;

void parse_args(int argc, char* argv[]);

vector<int> parse_input(char* file_name);

#endif
