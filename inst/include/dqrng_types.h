// Copyright 2018-2019 Ralf Stubner (daqana GmbH)
// Copyright 2023 Ralf Stubner
// Copyright 2023 Henrik Sloot
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

#ifndef DQRNG_TYPES_H
#define DQRNG_TYPES_H 1

#include <mystdint.h>
#include <memory>
#include <stdexcept>

namespace dqrng {

class random_64bit_generator {
protected:
  virtual void output(std::ostream& ost) const = 0;
  virtual void input(std::istream& ist) = 0;
public:
  using result_type = uint64_t;

  virtual ~random_64bit_generator() {};
  virtual result_type operator() () = 0;
  virtual void seed(result_type seed) = 0;
  virtual void seed(result_type seed, result_type stream) = 0;
  static constexpr result_type min() {return 0;};
  static constexpr result_type max() {return UINT64_MAX;};
  virtual uint32_t operator() (uint32_t range) = 0;
#ifdef LONG_VECTOR_SUPPORT
  virtual uint64_t operator() (uint64_t range) = 0;
#endif
  friend std::ostream& operator<<(std::ostream& ost, const random_64bit_generator& e) {
    e.output(ost);
    return ost;
  }
  friend std::istream& operator>>(std::istream& ist, random_64bit_generator& e) {
    e.input(ist);
    return ist;
  }
};

using rng64_t = std::shared_ptr<random_64bit_generator>;

class random_64bit_accessor : public random_64bit_generator {
private:
  dqrng::random_64bit_generator *gen;

protected:
  virtual void output(std::ostream& ost) const override {
    ost << *gen;
  }

  virtual void input(std::istream& ist) override {
    ist >> *gen;
  }

public:
  explicit random_64bit_accessor();

  virtual result_type operator() () override {
    return (*gen)();
  };

  virtual void seed(result_type seed) override {
    throw std::runtime_error("Seed handling not supported for this class!");
  };

  virtual void seed(result_type seed, result_type stream) override {
    throw std::runtime_error("Seed handling not supported for this class!");
  };

  virtual uint32_t operator() (uint32_t range) override {
    return (*gen)(range);
  }
#ifdef LONG_VECTOR_SUPPORT
  virtual uint64_t operator() (uint64_t range) override {
    return (*gen)(range);
  }
#endif
};


} // namespace dqrng
#endif // DQRNG_TYPES_H
