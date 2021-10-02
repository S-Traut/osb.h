#include <iostream>
#include <vector>
#include <chrono>

int main() {

  auto s = std::chrono::high_resolution_clock::now();


  std::vector<int> list;

  for(int i = 0; i < 1000000; i++)
    list.push_back(i);

  auto e = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(e - s);
  std::cout << duration.count() << std::endl;

  return 0;
}
