#include <fstream>
#include <iostream>

using namespace std;

int main() {
  ofstream outfile;
  outfile.open("out.txt", ios::binary | ios::out);
  ifstream infile;
  infile.open("in.txt", ios::binary | ios::in);
  char buffer[2];
  while (infile.read(buffer, 1)) {
    outfile.write(buffer, 1);
  }
  infile.close();
  outfile.close();
  infile.open("out.txt", ios::binary | ios::in);
  while (infile.read(buffer, 1)) {
    cout << buffer;
  }
  infile.close();
  return 0;
}
