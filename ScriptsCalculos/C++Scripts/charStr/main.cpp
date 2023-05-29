#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

int main()
{
  double example{1.85};
  
  std::string s{std::to_string(example)};
  
  for (const auto& c : s)
  {
    std::cout << c << ' ';
  }
  std::cout << '\n';
  
  return EXIT_SUCCESS;
}
