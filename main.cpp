#include <iostream>
#include <fstream>
#include <cstdlib>
#include <limits>
#include <stdexcept>

struct IntArray
{
  void add(int i);
  int at(size_t id) const;
  int get(size_t id) const noexcept;
  size_t size() const noexcept;
  int last() const noexcept;
  IntArray(int i);

  IntArray();
  IntArray(size_t n);
  IntArray(const IntArray& other);
  IntArray& operator=(const IntArray& other);

  ~IntArray();
  int * a;
  size_t k;
};

struct IntMatrix {
  size_t rows_;
  size_t cols_;
  IntArray data_;

public:
  IntMatrix() : rows_(0), cols_(0), data_() {}

  void readFromFile(const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
      throw std::runtime_error("Cannot open file");
    }

    size_t r, c;
    if (!(file >> r >> c)) {
      throw std::runtime_error("Invalid header");
    }

    rows_ = r;
    cols_ = c;
        
    data_ = IntArray(0);

    size_t total = rows_ * cols_;
    for (size_t i = 0; i < total; ++i) {
      int val;
      if (!(file >> val)) {
        throw std::runtime_error("Not enough data");
      }
      data_.add(val);
    }
  }

  void print() const {
    for (size_t i = 0; i < rows_; ++i) {
      for (size_t j = 0; j < cols_; ++j) {
        std::cout << data_.get(i * cols_ + j) << (j == cols_ - 1 ? "" : " ");
      }
      std::cout << std::endl;
    }
  }

  void insertRow(int after_row, int value) {
    if (after_row < 0 || static_cast<size_t>(after_row) > rows_) {
      throw std::logic_error("Invalid row index");
    }

    size_t new_rows = rows_ + 1;
    size_t new_total = new_rows * cols_;
        
    IntArray new_data(new_total);
        
    size_t src_idx = 0;
    size_t dest_idx = 0;
    size_t insert_pos_elements = static_cast<size_t>(after_row) * cols_;

    for (; dest_idx < insert_pos_elements; ++dest_idx) {
      new_data.a[dest_idx] = data_.a[src_idx++];
    }

    for (size_t c = 0; c < cols_; ++c) {
      new_data.a[dest_idx++] = value;
    }

    while (src_idx < data_.size()) {
      new_data.a[dest_idx++] = data_.a[src_idx++];
    }

    data_ = new_data;
    rows_ = new_rows;
    }
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

int IntArray::get(size_t id) const noexcept{
  return a[id];
}
int IntArray::at(size_t id) const {
  if (! id < k){
    throw std::logic_error("no");
  }
  return a[id];
}
size_t IntArray::size() const noexcept {
  return k;
}
int IntArray::last() const noexcept{
  return get(size() - 1);
}
void IntArray::add(int i) {
  int * tmp = new int[size() + 1];
  for (size_t j = 0; j< size(); ++j) {
    tmp[j] = a[j];
  }
  tmp[size()] = i;
  delete[] a;
  a = tmp;
  ++k;
}