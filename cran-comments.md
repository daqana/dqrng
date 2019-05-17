This release updates the maintainer's email address and brings two bug fixes.

The reported UBSAN errors seem to be false positives, c.f.
<https://github.com/RcppCore/Rcpp/issues/832>.

## Test environments
* local:  Debian stable with R 3.6
* Travis-CI:
    * Ubuntu Xenial, R 3.6, R 3.5, and R-devel
    * MacOS, R 3.6
* AppVeyor: Windows, R 3.6
* winbuilder: Windows, R-devel

## R CMD check results

0 errors | 0 warnings | 1 note

* Maintainer's email address has changed.

## Reverse dependencies

We checked 1 reverse dependency on CRAN, comparing `R CMD check` results across
CRAN and dev versions of this package.

 * We saw 0 new problems
 * We failed to check 0 packages
