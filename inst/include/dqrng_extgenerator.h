#ifndef DQRNG_EXTERNAL_GENERATOR_H
#define DQRNG_EXTERNAL_GENERATOR_H 1

#include <Rcpp.h>
#include <dqrng.h>
#include <dqrng_generator.h>
#include <dqrng_distribution.h>

namespace dqrng {
class random_64bit_accessor : public random_64bit_generator {
private:
    dqrng::random_64bit_generator *gen;

public:
    explicit random_64bit_accessor() : gen(dqrng::get_rng().get()) {}
    
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

}; // dqrng

namespace boost {
namespace random {
namespace detail {

template<>
inline std::pair<double, int> generate_int_float_pair<double, 8, dqrng::random_64bit_accessor>(dqrng::random_64bit_accessor& eng)
{
  return generate_int_float_pair<double, 8, dqrng::random_64bit_generator>(eng);
}

template<>
inline double generate_uniform_real<dqrng::random_64bit_accessor, double>(dqrng::random_64bit_accessor& eng, double min, double max)
{
  return generate_uniform_real<dqrng::random_64bit_generator, double>(eng, min, max);
}


}; // detail
}; // random
}; // boost

#endif
