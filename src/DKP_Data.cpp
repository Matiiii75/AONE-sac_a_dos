#include"DKP_Data.hpp"


C_DKPData::C_DKPData(string filename):_filename(filename){
  ifstream inf(filename);
  if (!inf){
    cerr << "Error opening file " << filename << " -- abort " << endl;
    abort();
  }

  inf >> _nbItems >> ws >> _capacity >> ws;

  _profits.resize(_nbItems);
  _weights.resize(_nbItems);

  int tmp;
  for (int i = 0; i < _nbItems; ++i){
    inf >> tmp >> _profits[i] >> _weights[i] >> ws;
  }

  inf >> _nbCliques >> ws;

  _cliques.resize(_nbCliques);
  
  int nb;
  for (int i = 0; i < _nbCliques; ++i){
    inf >> nb;
    _cliques[i].resize(nb);
    for (int j = 0; j < nb; ++j){
      inf >> _cliques[i][j];
    }
    inf >> ws;
  }

  inf.close();
  
}


ostream & operator<<(ostream & os, C_DKPData & data){
  os << "Instance : " << data._filename << endl;
  os << "\t" << data._nbItems << " Items -- Knapsack capacity : " << data._capacity << endl;
  os << "Item \tProfit \tWeight" << endl;
  for (int i = 0; i < data._nbItems; ++i){
    os << i << "\t" << data._profits[i] << "\t" << data._weights[i] << endl;
  }
  os << "Cliques :" << endl;
  for (int i = 0; i < data._nbCliques; ++i){
    os << "\t";
    for (int j = 0; j < (int)data._cliques[i].size();++j){
      os << data._cliques[i][j] << " ";
    }
    os << endl;
  }
  return os;
}
