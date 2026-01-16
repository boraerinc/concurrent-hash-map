#include "Stack.h"

// test moves helper
Stack<int> generateNumbers(size_t size) {
  // Test with simple type
  Stack<int> intstack;
  // should print constructor

  std::cout << "pushing numbers... \n";
  for(int i = 0; i <= size; ++i) {
    std::cout << i*i << " ";
    intstack.push_back(i*i);
  }
  std::cout << "\n";

  return intstack;
}

int main() {

  Stack<int> first = generateNumbers(10);
  // should print move copy constructor

  first = generateNumbers(20);
  // should print move assignment op.

  Stack<int> second(first);
  // should print copy constr.

  Stack<int> third = std::move(first);
  // should print move copy constr


  std::cout << " \n stack contains:  \n";
  while ( !third.isEmpty() ) {
    std::cout << third.pop() << " "; 
  }

  // testing exception:
  try{
    std::cout << "will it print?\n";
    std::cout << first.top();

  } catch (std::out_of_range err){
    std::cerr << "We caught an error!\n";
    std::cerr << err.what();
  }

  std::cout << std::endl;
  return 0;

}
