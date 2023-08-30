This release brings two new functions, dqrmvnorm and dqrrademacher.
It also fixes two NOTEs
* C++11 is no longer requested explicitly
* "sitmo" is added to Suggests in addition to LinkingTo to satisfy xref requirements

## Test environments

* local:  Ubuntu with R 4.3
* Github Actions:
    * Ubuntu, R-release and R-devel
    * MacOS, R-release
    * Windows, R-release
* winbuilder: Windows, R-devel

## R CMD check results

0 errors | 0 warnings | 0 note

## Reverse dependencies

We checked 13 reverse dependencies (9 from CRAN + 3 from Bioconductor), comparing R CMD check results across CRAN and dev versions of this package.

 * We saw 0 new problems
 * We failed to check 2 packages
