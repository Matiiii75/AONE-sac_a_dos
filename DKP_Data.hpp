#pragma once

#include<vector>
#include<string>
#include <cmath>
#include <iostream>
#include <fstream>
#include <limits>

using namespace std;
using uint = unsigned int; 

class C_DKPData {
public :
  string _filename;
  int _nbItems;
  int _capacity;
  int _nbCliques;
  vector<int> _profits;
  vector<int> _weights;
  vector<vector<int>> _cliques;

  C_DKPData(string filename);

  friend ostream & operator<<(ostream & os, C_DKPData & data);
};


ostream & operator<<(ostream & os, C_DKPData & data);
