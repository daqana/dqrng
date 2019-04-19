This release brings new features and one bug fix w.r.t. handling of the
RNG state. 

The reported UBSAN errors seem to be false positives, c.f.
<https://github.com/RcppCore/Rcpp/issues/832>.

## Test environments
* local:  Debian stable with R 3.5
* Travis-CI:
    * Ubuntu Xenial, R 3.5, R 3.4, and R-devel
    * MacOS, R 3.5
* AppVeyor: Windows, R 3.5
* winbuilder: Windows, R-devel

## R CMD check results

0 errors | 0 warnings | 0 notes

## Reverse dependencies

We checked 1 reverse dependency on CRAN, comparing `R CMD check` results across
CRAN and dev versions of this package.

 * We saw 0 new problems
 * We failed to check 0 packages
