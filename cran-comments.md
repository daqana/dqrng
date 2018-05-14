## Resubmission
This is a resubmission. In this version I have:

* Patched the included PCG RNG to compile on Solaris 

## Test environments
* local:  Debian stable with R 3.5.0
* Travis-CI:
    * Ubuntu Trusty, R 3.5.0, R 3.3.3, R-devel
    * MacOS, R 3.5.0
* AppVeyor: Windows, R 3.5.0
* winbuilder: Windows, R-devel
* r-hub.io:
    * Windows, R-devel
    * Ubuntu, gcc, R-release
    * Fedora, clang, R-devel
    * Solaris, R-patched

## R CMD check results

0 errors | 0 warnings | 1 note

* Days since last update: 3

=> Submission fixes compilation ERROR on Solaris
