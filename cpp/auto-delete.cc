/*
 * Demo to show auto delete of std::unique_ptr
 * Build: g++ -std=c++0x auto-delete.cc -o auto-delete
 * Run: ./auto-delete
 */

#include <iostream>
#include <memory>

class MyClass {
public:
  MyClass(int i) { std::cout << "Calling constructor " << i << std::endl; }
  ~MyClass() { std::cout << "Calling destructor" << std::endl; }
};

int main() {
  std::unique_ptr<MyClass> p(new MyClass(0));
  std::unique_ptr<std::unique_ptr<MyClass>[]> dp(
      new std::unique_ptr<MyClass>[5]);
  for (int i = 0; i < 5; i++)
    dp[i] = std::unique_ptr<MyClass>(new MyClass(i + 1));
  return 0;
}
