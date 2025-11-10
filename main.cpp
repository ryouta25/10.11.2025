#include<iostream>

struct IntArray
{
  void add(int i);
  int at(size_t id) const;
  int get(size_t id) const noexcept;
  size_t size() const noexcept;
  int last() const noexcept;
  IntArray(int i);
  ~IntArray();
  int * a;
  size_t k;
};

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

IntArray::~IntArray() {
  delete[] a;
}
IntArray::IntArray(int i):
  k(1),
  a(new int[1])
{
    * a = i;
}