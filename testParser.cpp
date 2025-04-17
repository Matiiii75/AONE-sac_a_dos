#include<iostream>
#include<string>
#include"DKP_Data.hpp"

using namespace std;

int main(int argc, char** argv){

  string filename;
  if (argc > 1){
    filename = argv[1];
  } else {
    cout << "Enter instance file name :";
    cin >> filename;
  }

  C_DKPData data(filename);

  cout << data;
  
  return 0;
}
