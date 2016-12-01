/*
 * Demo of reinterpret_cast and static_cast
 * Build: g++ test-reinterpret-cast.cc -o test-reinterpret-cast
 * Run: ./test-reinterpret-cast
 */

#include <iostream>
#include <limits>

using namespace std;

int main() {
  int a = 65535;
  int *pa = &a;
  cout << "a = " << a << endl;
  cout << "pa = " << pa << endl;
  cout << "reinterpret_cast<short*>(pa) = " << reinterpret_cast<short *>(pa)
       << endl;
  cout << "*reinterpret_cast<short*>(pa) = " << *reinterpret_cast<short *>(pa)
       << endl;
  cout << "static_cast<unsigned>(numeric_limits<int>::max()) = "
       << static_cast<unsigned>(numeric_limits<int>::max()) << endl;
  cout << "numeric_limits<int>::max() = " << numeric_limits<int>::max() << endl;
  cout << "static_cast<unsigned>(numeric_limits<int>::min()) = "
       << static_cast<unsigned>(numeric_limits<int>::min()) << endl;
  cout << "numeric_limits<int>::min() = " << numeric_limits<int>::min() << endl;
  cout << "numeric_limits<unsigned int>::max() = "
       << numeric_limits<unsigned int>::max() << endl;
  return 0;
}
