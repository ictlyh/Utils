#include <iostream> // std::cout
#include <memory>

class MyClass {
public:
  MyClass() { std::cout << "MyClass constructed\n"; }
  ~MyClass() { std::cout << "MyClass destroyed\n"; }
};

int main() {
  MyClass *pt;
  pt = new MyClass[3];
  delete[] pt;
  // double free cause to core dump
  // delete[] pt;
  MyClass *npt = NULL;
  delete npt;
  std::unique_ptr<MyClass> p(new MyClass());
  return 0;
}
