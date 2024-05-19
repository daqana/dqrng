This release fixes an UBSAN error found by CRAN and a compilation error when the
compiler does not provide a 128bit integer type.

## Test environments

* local:  Ubuntu with R 4.4
* Github Actions:
    * Ubuntu, R-release and R-devel
    * MacOS, R-release
    * Windows, R-release
* winbuilder: Windows, R-devel

## R CMD check results

0 errors | 0 warnings | 0 note

## Reverse dependencies

We checked 12 reverse dependencies from CRAN, comparing R CMD check results
across CRAN and dev versions of this package.

 * We saw 0 new problems
