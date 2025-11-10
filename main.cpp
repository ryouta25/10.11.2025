#include<iostream>

int main()
{
  int next = 0;
  std::cin >> next;
  try {
    IntArray a(next);
    while (std::cin >> next)
    {
      a.add(next);
    }
    if (std::cin.fail()) {
      return 1;
    }

    size_t count = 1;
    for (size_t i = 0; i < a.size() - 1; ++i)
    {
      int d = a.get(i);
      count+=!(d%a.last());
    }
    std::cout << count << std::endl;
  } catch (...) {
    return 2;
  }
  return 0;
}
