#include <iostream>
#include <fstream>
#include <bitset>
#include <cmath>
#include "main.h"

using namespace std;

Args args = { "input.txt", 0};

int N;
int A;
int N_bits;
ofstream file("out.txt");

void parse_args(int argc, char* argv[]){
  // Checking number of arguments
  if(argc < 2) {
	// cout << "Wrong number." << endl;
	// cerr << "Usage: " << argv[0] << " -in <> -sw omega|delta" << endl;
	// exit(1);
  }
  for(int i = 1; i < argc; i += 2){
	string arg_oi = string(argv[i]);
	if(arg_oi == "in"){
	  args.in_file = argv[i+1];
	} else if(arg_oi == "-sw") {
	  string sw_arg = string(argv[i+1]);
	  if(sw_arg == "Omega")
		args.sw = 0;
	  else if(sw_arg == "Delta")
		args.sw = 1;
	  else {
	cerr << "Usage: " << argv[0] << " -in <> -sw Omega|Delta" << endl;
		exit(1);
	  }
	} else {
	  // cout << "Trouble with: " << arg_oi << endl;
	cerr << "Usage: " << argv[0] << " -in <> -sw Omega|Delta" << endl;
	  exit(1);
	}
  }
}

/**
Function to parse the input file.
 */
vector<int> parse_input(char* file_name) {
  vector<int> outputs;
  ifstream infile(file_name);
  
  infile >> N >> A;
  outputs.resize(A);

  for(int i = 0; i < A; i++){
	infile >> outputs[i];
  }
	
  infile.close();
  return outputs;
}

int switch_config(bool Eq, int input) {
  int out;
  if(Eq) {
	// Config is straight: send out on same port
	file << "S ";
	out = input;
  } else {
	// Config is cross: send out on other port
	file << "C ";
	out = (input % 2 == 0) ? (input + 1) : (input - 1);
  }
  return out;
}

int shuffle(int N, int in){
  // If i < N/2 -> map to 2 * i
  if(in < N/2)
	return 2 * in;
  else // 2*i - N + 1
	return 2 * in - N + 1;
}

/** 
	For a N X N switching fabric network, give the src port, dest port and the layer number, gives the switching config
	Omega: Each layer only has a perfect shuffle config.
	Layer 0 is the rightmost layer.
*/
int omega(int input, int layer, bitset<7> dest) {
  bool i_bit = input % 2;
  // Find the bit under consideration
  bool o_bit = dest[layer];

  int out = switch_config(i_bit == o_bit, input);
  // cout << out << "\t";
  // Find the output of this layer => input of next layer
  return shuffle(N, out);
}

/** 
	For a N X N switching fabric network, give the src port, dest port and the layer number, gives the switching config
	Delta: Each switch is in X config with a switch l blocks below, where l is layer
	Layer 0 is the rightmost layer.
*/
int delta(int input, int layer, bitset<7> dest){
  bool i_bit = input % 2;
  // Find bit under consideration
  bool o_bit = dest[layer];

  int out = switch_config(i_bit == o_bit, input);
  // cout << out << "\t";

  // Cycle within a layer
  int size = exp2(layer + 1);
  if(out % size < size/2) {
  // If in 1st half of the set, all multiples of 2 are self linked
	if(out % 2 == 0)
	  return out;
	return size/2 + (out - 1);
  } else {
	if(out % 2 != 0)
	  return out;
	return (out - size/2 + 1);
  }

}

int main(int argc, char* argv[]){
  parse_args(argc, argv);
  vector<int> outputs = parse_input(args.in_file);
  N_bits = log2(N);
  file << ((args.sw == 1) ? "Delta\n" : "Omega\n");
  for(int i = 0; i < outputs.size(); i++){
	bitset<7> dest_bitset(outputs[i]);
	int in = shuffle(N, i);
	for(int j = N_bits - 1; j >= 0; j--){
	  if(args.sw == 0)
		in = omega(in, j, dest_bitset); 
	  else
		in = delta(in, j, dest_bitset);
	}
	// cout << endl;
	file << endl;
  }
}
