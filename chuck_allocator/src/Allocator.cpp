//
// Created by berlioz on 26.10.2020.
//
#include <iostream>
#include <vector>

#define MULT 1000

template <typename T> class Chunk {
private:
  uint8_t *p;
  std::size_t sizeLeft;
  Chunk<T> *prev;
  inline static size_t chunkRefCnt = 0;

public:
  Chunk<T>() : prev(nullptr) {
    p = new u_int8_t[MULT * sizeof(T)];
    sizeLeft = MULT;
    ++chunkRefCnt;
  }

  ~Chunk<T>() {
    if (chunkRefCnt > 1) {
      --chunkRefCnt;
    } else {
      delete[] p;
    }
    if (prev) {
      prev->~Chunk<T>();
    }
  }

  void allocate(const size_t n) { sizeLeft -= n; }

  template <typename... Args> void construct(Args &&... args) {
    new (p) T(args...);
  }

  std::size_t getSizeLeft() { return sizeLeft; }

  uint8_t *getPointer() const { return p; }

  Chunk<T> *getPrev() { return prev; }

  void setPrev(Chunk<T> *prevChunk) { prev = prevChunk; }
};

template <typename T> class Allocator {
private:
  Chunk<T> *chunk;

public:
  using value_type = T;
  using pointer = T *;
  using const_pointer = const T *;
  using reference = T &;
  using const_reference = const T &;
  using size_type = std::size_t;
  using rebind = struct rebind { typedef Allocator<T> other; };

  Allocator<T>() : chunk(nullptr){};
  ~Allocator() {
    if (chunk) {
      chunk->~Chunk<T>();
    }
  }

  T *allocate(const size_t &n) {
    if (n > max_size()) {
      throw std::runtime_error("Trying to allocate more than allowed!");
    }

    if (!chunk) {
      chunk = new Chunk<T>();
    }
    if (n <= chunk->getSizeLeft()) {
      chunk->allocate(n);
      return reinterpret_cast<T *>(chunk->getPointer());
    } else {
      // check all previous chunks for spare memory
      auto prev = chunk->getPrev();
      while (prev != nullptr) {
        if (n <= prev->getSizeLeft()) {
          prev->allocate(n);
          return reinterpret_cast<T *>(prev->getPointer());
        }
        prev = prev->getPrev();
      }
      // no luck, gotta create a new chunk
      auto newChunk = new Chunk<T>();
      newChunk->setPrev(chunk);
      chunk = newChunk;
      chunk->allocate(n);
      return reinterpret_cast<T *>(chunk->getPointer());
    }
  }

  void deallocate(T *p, const size_t n) {
    // do nothing
  }

  template <typename... Args> void construct(T *p, Args &&... args) {
    chunk->construct(args...);
  }

  void destroy(T *p) { p->~T(); }

  std::size_t max_size() { return MULT * sizeof(T); }
};

// int main() {
// std::vector<int, Allocator<int>> vec;
// for(int i = 0; i < MULT; ++i) {
//   vec.push_back(1);
// }
//  for(int i = 0; i < MULT; ++i) {
//    vec.push_back(2);
//  }
// std::cout << vec[0] << std::endl;
//}