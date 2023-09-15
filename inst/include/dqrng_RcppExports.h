// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#ifndef RCPP_dqrng_RCPPEXPORTS_H_GEN_
#define RCPP_dqrng_RCPPEXPORTS_H_GEN_

#include <Rcpp.h>

namespace dqrng {

    using namespace Rcpp;

    namespace {
        void validateSignature(const char* sig) {
            Rcpp::Function require = Rcpp::Environment::base_env()["require"];
            require("dqrng", Rcpp::Named("quietly") = true);
            typedef int(*Ptr_validate)(const char*);
            static Ptr_validate p_validate = (Ptr_validate)
                R_GetCCallable("dqrng", "_dqrng_RcppExport_validate");
            if (!p_validate(sig)) {
                throw Rcpp::function_not_exported(
                    "C++ function with signature '" + std::string(sig) + "' not found in dqrng");
            }
        }
    }

    inline void dqset_seed(Rcpp::Nullable<Rcpp::IntegerVector> seed, Rcpp::Nullable<Rcpp::IntegerVector> stream = R_NilValue) {
        typedef SEXP(*Ptr_dqset_seed)(SEXP,SEXP);
        static Ptr_dqset_seed p_dqset_seed = NULL;
        if (p_dqset_seed == NULL) {
            validateSignature("void(*dqset_seed)(Rcpp::Nullable<Rcpp::IntegerVector>,Rcpp::Nullable<Rcpp::IntegerVector>)");
            p_dqset_seed = (Ptr_dqset_seed)R_GetCCallable("dqrng", "_dqrng_dqset_seed");
        }
        RObject rcpp_result_gen;
        {
            rcpp_result_gen = p_dqset_seed(Shield<SEXP>(Rcpp::wrap(seed)), Shield<SEXP>(Rcpp::wrap(stream)));
        }
        if (rcpp_result_gen.inherits("interrupted-error"))
            throw Rcpp::internal::InterruptedException();
        if (Rcpp::internal::isLongjumpSentinel(rcpp_result_gen))
            throw Rcpp::LongjumpException(rcpp_result_gen);
        if (rcpp_result_gen.inherits("try-error"))
            throw Rcpp::exception(Rcpp::as<std::string>(rcpp_result_gen).c_str());
    }

    inline void dqRNGkind(std::string kind, const std::string& normal_kind = "ignored") {
        typedef SEXP(*Ptr_dqRNGkind)(SEXP,SEXP);
        static Ptr_dqRNGkind p_dqRNGkind = NULL;
        if (p_dqRNGkind == NULL) {
            validateSignature("void(*dqRNGkind)(std::string,const std::string&)");
            p_dqRNGkind = (Ptr_dqRNGkind)R_GetCCallable("dqrng", "_dqrng_dqRNGkind");
        }
        RObject rcpp_result_gen;
        {
            rcpp_result_gen = p_dqRNGkind(Shield<SEXP>(Rcpp::wrap(kind)), Shield<SEXP>(Rcpp::wrap(normal_kind)));
        }
        if (rcpp_result_gen.inherits("interrupted-error"))
            throw Rcpp::internal::InterruptedException();
        if (Rcpp::internal::isLongjumpSentinel(rcpp_result_gen))
            throw Rcpp::LongjumpException(rcpp_result_gen);
        if (rcpp_result_gen.inherits("try-error"))
            throw Rcpp::exception(Rcpp::as<std::string>(rcpp_result_gen).c_str());
    }

    inline Rcpp::NumericVector dqrunif(size_t n, double min = 0.0, double max = 1.0) {
        typedef SEXP(*Ptr_dqrunif)(SEXP,SEXP,SEXP);
        static Ptr_dqrunif p_dqrunif = NULL;
        if (p_dqrunif == NULL) {
            validateSignature("Rcpp::NumericVector(*dqrunif)(size_t,double,double)");
            p_dqrunif = (Ptr_dqrunif)R_GetCCallable("dqrng", "_dqrng_dqrunif");
        }
        RObject rcpp_result_gen;
        {
            rcpp_result_gen = p_dqrunif(Shield<SEXP>(Rcpp::wrap(n)), Shield<SEXP>(Rcpp::wrap(min)), Shield<SEXP>(Rcpp::wrap(max)));
        }
        if (rcpp_result_gen.inherits("interrupted-error"))
            throw Rcpp::internal::InterruptedException();
        if (Rcpp::internal::isLongjumpSentinel(rcpp_result_gen))
            throw Rcpp::LongjumpException(rcpp_result_gen);
        if (rcpp_result_gen.inherits("try-error"))
            throw Rcpp::exception(Rcpp::as<std::string>(rcpp_result_gen).c_str());
        return Rcpp::as<Rcpp::NumericVector >(rcpp_result_gen);
    }

    inline double runif(double min = 0.0, double max = 1.0) {
        typedef SEXP(*Ptr_runif)(SEXP,SEXP);
        static Ptr_runif p_runif = NULL;
        if (p_runif == NULL) {
            validateSignature("double(*runif)(double,double)");
            p_runif = (Ptr_runif)R_GetCCallable("dqrng", "_dqrng_runif");
        }
        RObject rcpp_result_gen;
        {
            rcpp_result_gen = p_runif(Shield<SEXP>(Rcpp::wrap(min)), Shield<SEXP>(Rcpp::wrap(max)));
        }
        if (rcpp_result_gen.inherits("interrupted-error"))
            throw Rcpp::internal::InterruptedException();
        if (Rcpp::internal::isLongjumpSentinel(rcpp_result_gen))
            throw Rcpp::LongjumpException(rcpp_result_gen);
        if (rcpp_result_gen.inherits("try-error"))
            throw Rcpp::exception(Rcpp::as<std::string>(rcpp_result_gen).c_str());
        return Rcpp::as<double >(rcpp_result_gen);
    }

    inline Rcpp::NumericVector dqrnorm(size_t n, double mean = 0.0, double sd = 1.0) {
        typedef SEXP(*Ptr_dqrnorm)(SEXP,SEXP,SEXP);
        static Ptr_dqrnorm p_dqrnorm = NULL;
        if (p_dqrnorm == NULL) {
            validateSignature("Rcpp::NumericVector(*dqrnorm)(size_t,double,double)");
            p_dqrnorm = (Ptr_dqrnorm)R_GetCCallable("dqrng", "_dqrng_dqrnorm");
        }
        RObject rcpp_result_gen;
        {
            rcpp_result_gen = p_dqrnorm(Shield<SEXP>(Rcpp::wrap(n)), Shield<SEXP>(Rcpp::wrap(mean)), Shield<SEXP>(Rcpp::wrap(sd)));
        }
        if (rcpp_result_gen.inherits("interrupted-error"))
            throw Rcpp::internal::InterruptedException();
        if (Rcpp::internal::isLongjumpSentinel(rcpp_result_gen))
            throw Rcpp::LongjumpException(rcpp_result_gen);
        if (rcpp_result_gen.inherits("try-error"))
            throw Rcpp::exception(Rcpp::as<std::string>(rcpp_result_gen).c_str());
        return Rcpp::as<Rcpp::NumericVector >(rcpp_result_gen);
    }

    inline double rnorm(double mean = 0.0, double sd = 1.0) {
        typedef SEXP(*Ptr_rnorm)(SEXP,SEXP);
        static Ptr_rnorm p_rnorm = NULL;
        if (p_rnorm == NULL) {
            validateSignature("double(*rnorm)(double,double)");
            p_rnorm = (Ptr_rnorm)R_GetCCallable("dqrng", "_dqrng_rnorm");
        }
        RObject rcpp_result_gen;
        {
            rcpp_result_gen = p_rnorm(Shield<SEXP>(Rcpp::wrap(mean)), Shield<SEXP>(Rcpp::wrap(sd)));
        }
        if (rcpp_result_gen.inherits("interrupted-error"))
            throw Rcpp::internal::InterruptedException();
        if (Rcpp::internal::isLongjumpSentinel(rcpp_result_gen))
            throw Rcpp::LongjumpException(rcpp_result_gen);
        if (rcpp_result_gen.inherits("try-error"))
            throw Rcpp::exception(Rcpp::as<std::string>(rcpp_result_gen).c_str());
        return Rcpp::as<double >(rcpp_result_gen);
    }

    inline Rcpp::NumericVector dqrexp(size_t n, double rate = 1.0) {
        typedef SEXP(*Ptr_dqrexp)(SEXP,SEXP);
        static Ptr_dqrexp p_dqrexp = NULL;
        if (p_dqrexp == NULL) {
            validateSignature("Rcpp::NumericVector(*dqrexp)(size_t,double)");
            p_dqrexp = (Ptr_dqrexp)R_GetCCallable("dqrng", "_dqrng_dqrexp");
        }
        RObject rcpp_result_gen;
        {
            rcpp_result_gen = p_dqrexp(Shield<SEXP>(Rcpp::wrap(n)), Shield<SEXP>(Rcpp::wrap(rate)));
        }
        if (rcpp_result_gen.inherits("interrupted-error"))
            throw Rcpp::internal::InterruptedException();
        if (Rcpp::internal::isLongjumpSentinel(rcpp_result_gen))
            throw Rcpp::LongjumpException(rcpp_result_gen);
        if (rcpp_result_gen.inherits("try-error"))
            throw Rcpp::exception(Rcpp::as<std::string>(rcpp_result_gen).c_str());
        return Rcpp::as<Rcpp::NumericVector >(rcpp_result_gen);
    }

    inline double rexp(double rate = 1.0) {
        typedef SEXP(*Ptr_rexp)(SEXP);
        static Ptr_rexp p_rexp = NULL;
        if (p_rexp == NULL) {
            validateSignature("double(*rexp)(double)");
            p_rexp = (Ptr_rexp)R_GetCCallable("dqrng", "_dqrng_rexp");
        }
        RObject rcpp_result_gen;
        {
            rcpp_result_gen = p_rexp(Shield<SEXP>(Rcpp::wrap(rate)));
        }
        if (rcpp_result_gen.inherits("interrupted-error"))
            throw Rcpp::internal::InterruptedException();
        if (Rcpp::internal::isLongjumpSentinel(rcpp_result_gen))
            throw Rcpp::LongjumpException(rcpp_result_gen);
        if (rcpp_result_gen.inherits("try-error"))
            throw Rcpp::exception(Rcpp::as<std::string>(rcpp_result_gen).c_str());
        return Rcpp::as<double >(rcpp_result_gen);
    }

    inline Rcpp::XPtr<dqrng::rng64_t::element_type> get_rng() {
        typedef SEXP(*Ptr_get_rng)();
        static Ptr_get_rng p_get_rng = NULL;
        if (p_get_rng == NULL) {
            validateSignature("Rcpp::XPtr<dqrng::rng64_t::element_type>(*get_rng)()");
            p_get_rng = (Ptr_get_rng)R_GetCCallable("dqrng", "_dqrng_get_rng");
        }
        RObject rcpp_result_gen;
        {
            rcpp_result_gen = p_get_rng();
        }
        if (rcpp_result_gen.inherits("interrupted-error"))
            throw Rcpp::internal::InterruptedException();
        if (Rcpp::internal::isLongjumpSentinel(rcpp_result_gen))
            throw Rcpp::LongjumpException(rcpp_result_gen);
        if (rcpp_result_gen.inherits("try-error"))
            throw Rcpp::exception(Rcpp::as<std::string>(rcpp_result_gen).c_str());
        return Rcpp::as<Rcpp::XPtr<dqrng::rng64_t::element_type> >(rcpp_result_gen);
    }

    inline Rcpp::IntegerVector dqrrademacher(size_t n) {
        typedef SEXP(*Ptr_dqrrademacher)(SEXP);
        static Ptr_dqrrademacher p_dqrrademacher = NULL;
        if (p_dqrrademacher == NULL) {
            validateSignature("Rcpp::IntegerVector(*dqrrademacher)(size_t)");
            p_dqrrademacher = (Ptr_dqrrademacher)R_GetCCallable("dqrng", "_dqrng_dqrrademacher");
        }
        RObject rcpp_result_gen;
        {
            rcpp_result_gen = p_dqrrademacher(Shield<SEXP>(Rcpp::wrap(n)));
        }
        if (rcpp_result_gen.inherits("interrupted-error"))
            throw Rcpp::internal::InterruptedException();
        if (Rcpp::internal::isLongjumpSentinel(rcpp_result_gen))
            throw Rcpp::LongjumpException(rcpp_result_gen);
        if (rcpp_result_gen.inherits("try-error"))
            throw Rcpp::exception(Rcpp::as<std::string>(rcpp_result_gen).c_str());
        return Rcpp::as<Rcpp::IntegerVector >(rcpp_result_gen);
    }

    inline Rcpp::IntegerVector dqsample_int(int m, int n, bool replace = false, Rcpp::Nullable<Rcpp::NumericVector> probs = R_NilValue, int offset = 0) {
        typedef SEXP(*Ptr_dqsample_int)(SEXP,SEXP,SEXP,SEXP,SEXP);
        static Ptr_dqsample_int p_dqsample_int = NULL;
        if (p_dqsample_int == NULL) {
            validateSignature("Rcpp::IntegerVector(*dqsample_int)(int,int,bool,Rcpp::Nullable<Rcpp::NumericVector>,int)");
            p_dqsample_int = (Ptr_dqsample_int)R_GetCCallable("dqrng", "_dqrng_dqsample_int");
        }
        RObject rcpp_result_gen;
        {
            rcpp_result_gen = p_dqsample_int(Shield<SEXP>(Rcpp::wrap(m)), Shield<SEXP>(Rcpp::wrap(n)), Shield<SEXP>(Rcpp::wrap(replace)), Shield<SEXP>(Rcpp::wrap(probs)), Shield<SEXP>(Rcpp::wrap(offset)));
        }
        if (rcpp_result_gen.inherits("interrupted-error"))
            throw Rcpp::internal::InterruptedException();
        if (Rcpp::internal::isLongjumpSentinel(rcpp_result_gen))
            throw Rcpp::LongjumpException(rcpp_result_gen);
        if (rcpp_result_gen.inherits("try-error"))
            throw Rcpp::exception(Rcpp::as<std::string>(rcpp_result_gen).c_str());
        return Rcpp::as<Rcpp::IntegerVector >(rcpp_result_gen);
    }

    inline Rcpp::NumericVector dqsample_num(double m, double n, bool replace = false, Rcpp::Nullable<Rcpp::NumericVector> probs = R_NilValue, int offset = 0) {
        typedef SEXP(*Ptr_dqsample_num)(SEXP,SEXP,SEXP,SEXP,SEXP);
        static Ptr_dqsample_num p_dqsample_num = NULL;
        if (p_dqsample_num == NULL) {
            validateSignature("Rcpp::NumericVector(*dqsample_num)(double,double,bool,Rcpp::Nullable<Rcpp::NumericVector>,int)");
            p_dqsample_num = (Ptr_dqsample_num)R_GetCCallable("dqrng", "_dqrng_dqsample_num");
        }
        RObject rcpp_result_gen;
        {
            rcpp_result_gen = p_dqsample_num(Shield<SEXP>(Rcpp::wrap(m)), Shield<SEXP>(Rcpp::wrap(n)), Shield<SEXP>(Rcpp::wrap(replace)), Shield<SEXP>(Rcpp::wrap(probs)), Shield<SEXP>(Rcpp::wrap(offset)));
        }
        if (rcpp_result_gen.inherits("interrupted-error"))
            throw Rcpp::internal::InterruptedException();
        if (Rcpp::internal::isLongjumpSentinel(rcpp_result_gen))
            throw Rcpp::LongjumpException(rcpp_result_gen);
        if (rcpp_result_gen.inherits("try-error"))
            throw Rcpp::exception(Rcpp::as<std::string>(rcpp_result_gen).c_str());
        return Rcpp::as<Rcpp::NumericVector >(rcpp_result_gen);
    }

}

#endif // RCPP_dqrng_RCPPEXPORTS_H_GEN_
