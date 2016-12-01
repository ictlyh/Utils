/*
 * Using operator & to calculate modulo.
 * Notice: only pow of 2(such as 2,4,8,...) works
 * Build: g++ test-modulo.cc -o test-modulo
 * Run: ./test-modulo
 */
#include <iostream>

using namespace std;

int main() {
  int modulo = 8;
  for (int i = 0; i < 100; i++) {
    if ((i % modulo) != (i & (modulo - 1))) {
      cout << i << "%" << modulo << "=" << (i % modulo) << ", " << i << "&"
           << modulo << "=" << (i & (modulo - 1)) << endl;
    }
  }
  return 0;
}
