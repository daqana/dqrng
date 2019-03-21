## Copyright (C) 2019 Ralf Stubner (daqana GmbH)
##
## This file is part of dqrng.
##
## dqrng is free software: you can redistribute it and/or modify it
## under the terms of the GNU Affero General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## dqrng is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU Affero General Public License for more details.
##
## You should have received a copy of the GNU Affero General Public License
## along with dqrng.  If not, see <http://www.gnu.org/licenses/>.

inlineCxxPlugin <- function(...){
    plugin <- Rcpp::Rcpp.plugin.maker(
        include.before = "#include <dqrng.h>",
        LinkingTo = c("dqrng", "sitmo", "BH", "Rcpp"),
        package = "dqrng"
    )
    settings <- plugin()
    settings$env$USE_CXX11 <- "yes"
    settings
}
