// Copyright 2019 Ralf Stubner (daqana GmbH)
//
// This file is part of dqrng.
//
// dqrng is free software: you can redistribute it and/or modify it
// under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// dqrng is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with dqrng.  If not, see <http://www.gnu.org/licenses/>.

#ifndef MINIMAL_INT_SET_H
#define MINIMAL_INT_SET_H 1

#include <cstdint>
#include <stdexcept>
#include <boost/dynamic_bitset.hpp>

// Two set implementations meant to store n integers out of m possible values.
// Common interface:
// * ctor (m, n)
// * bool insert(entry, check = true)
//   returns true if insert succesfull and check == true

namespace dqrng {
template<typename T = uint32_t>
class minimal_hash_set {
private:
  // open addressing
  T* entries;
  T empty = -1;
  std::size_t total;
  std::size_t mask;
  std::size_t size{0};

  // identity hash
  std::size_t hash(T entry) {return entry & mask;}
  // quadratic probing
  std::size_t probe(T entry, std::size_t step) {return (step * step + step)/2;}

  minimal_hash_set(std::size_t n) {
    // total will be between 1.5 * n and 3 * n
    total = 1 << static_cast<int>(std::ceil(std::log2(1.5 * n)));
    mask = total - 1;
    entries = new T[total];
    for (std::size_t i = 0; i < total; ++i)
      entries[i] = empty;
  };

public:
  minimal_hash_set(std::size_t m, std::size_t n) : minimal_hash_set(n) {};

  ~minimal_hash_set() {delete[] entries;}

  bool insert(T entry, bool check = true) {
    if (size > 0.8 * total)
      throw std::runtime_error("Hash set is (almost) full!");

    std::size_t home;
    std::size_t pos = home = hash(entry);
    for (int i = 1; empty != entries[pos]; ++i) {
      if (check && entry == entries[pos]) {
        return false;
      }
      pos = (home + probe(entry, i)) & mask;
    }
    entries[pos] = entry;
    ++size;
    return true;
  }
};

class minimal_bit_set {
 private:
    boost::dynamic_bitset<> entries;

    minimal_bit_set(std::size_t m) {
        entries.resize(m);
    };

 public:
    minimal_bit_set(std::size_t m, std::size_t n) : minimal_bit_set(m) {};

    ~minimal_bit_set() {};

    bool insert(std::size_t entry, bool check = true) {
        if (check)
            return !entries.test_set(entry);

        entries.set(entry);
        return true;
    };
};
}
#endif // MINIMAL_INT_SET_H
