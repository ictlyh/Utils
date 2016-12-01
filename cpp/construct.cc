/*
 * Demo of constructor and destructor calling sequence in C++ class.
 * Build: g++ construct.cc -o construct
 * Run: ./construct
 */

#include <iostream>
using namespace std;

class A {
public:
  A() { cout << "Default constructor of class A" << endl; }
  ~A() { cout << "Default destructor of class A" << endl; }
};

class B {
private:
  A a;

public:
  B() { cout << "Default constructor of class B" << endl; }
  ~B() { cout << "Default destructor of class B" << endl; }
};

int main() {
  B *b = new B();
  delete b;
  return 0;
}
