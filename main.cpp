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

    void insertCol(int after_col, int value) {
      if (after_col < 0 || static_cast<size_t>(after_col) > cols_) {
        throw std::logic_error("Invalid col index");
      }

      size_t new_cols = cols_ + 1;
      size_t new_total = rows_ * new_cols;
        
      IntArray new_data(new_total);
        
      size_t dest_idx = 0;
      size_t insertion_index = static_cast<size_t>(after_col);

      for (size_t r = 0; r < rows_; ++r) {
        size_t row_start = r * cols_;
            
        for (size_t c = 0; c < insertion_index; ++c) {
          new_data.a[dest_idx++] = data_.a[row_start + c];
        }
            
        new_data.a[dest_idx++] = value;
            
        for (size_t c = insertion_index; c < cols_; ++c) {
          new_data.a[dest_idx++] = data_.a[row_start + c];
        }
      }

      data_ = new_data;
      cols_ = new_cols;
    }
};

int main(int argc, char** argv)
{
  if (argc < 2) {
    return 1;
  }

  try {
    IntMatrix matrix;
        
    try {
      matrix.readFromFile(argv[1]);
    } catch (const std::runtime_error&) {
      return 1;
    } catch (const std::exception&) {
      return 1;
    }

    int cmd, p1, p2;
    while (std::cin >> cmd) {
      if (!(std::cin >> p1 >> p2)) {
        return 1; 
      }

      try {
        if (cmd == 1) {
          matrix.insertRow(p1, p2);
        } else if (cmd == 2) {
          matrix.insertCol(p1, p2);
        } else {
          throw std::logic_error("Unknown command");
        }
                
        matrix.print();

      } catch (const std::logic_error&) {
        return 3;
      }
    }

    if (!std::cin.eof() && std::cin.fail()) {
      return 1;
    }

  } catch (const std::bad_alloc&) {
    return 2;
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

IntArray::IntArray() : k(0), a(nullptr) {}

IntArray::IntArray(size_t n) : k(n), a(nullptr) {
  if (n > 0) a = new int[n];
}

IntArray::IntArray(const IntArray& other) : k(other.k), a(nullptr) {
  if (k > 0) {
    a = new int[k];
    for (size_t i = 0; i < k; ++i) {
      a[i] = other.a[i];
    }
  }
}

IntArray& IntArray::operator=(const IntArray& other) {
  if (this != &other) {
    delete[] a;
    k = other.k;
    a = nullptr;
    if (k > 0) {
      a = new int[k];
      for (size_t i = 0; i < k; ++i) {
        a[i] = other.a[i];
      }
    }
  }
  return *this;
}

int IntArray::get(size_t id) const noexcept{
  return a[id];
}

int IntArray::at(size_t id) const {
  if (! (id < k)){
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