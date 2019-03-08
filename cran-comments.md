Bug fix release to address build failures on CRAN:

* On CRAN-MacOS the text given to a C++ exception is not propagated to R. The 
  tests have been adjusted to not expect a particular error message on MacOS.

* On CRAN-Solaris (at least) one test that intentionally triggers a C++ exception
  leads to a segmentation fault. Since I cannot reproduce this behavior
  anywhere else, I have disabled exception throwing tests on Solaris.

* Some template specializations where introduced to inhibit (false-positive)
  compiler warnings during the tests.

## Test environments
* local:  Debian stable with R 3.5
* Travis-CI:
    * Ubuntu Xenial, R 3.5, R 3.4, and R-devel
    * MacOS, R 3.5
* AppVeyor: Windows, R 3.5
* winbuilder: Windows, R-devel

## R CMD check results

0 errors | 0 warnings | 0 notes

The reported UBSAN errors seem to be false positives, c.f.
<https://github.com/RcppCore/Rcpp/issues/832>.

## Reverse dependencies

dqrng currently has no reverse dependencies on CRAN.
