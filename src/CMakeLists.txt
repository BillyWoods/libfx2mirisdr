# Copyright 2012 OSMOCOM Project
#
# This file is part of MiriSDR
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

########################################################################
# Setup library
########################################################################
add_library(fx2mirisdr_shared SHARED
    libfx2mirisdr.c
    tuner_msi001.c
)

target_link_libraries(fx2mirisdr_shared
    ${LIBUSB_LIBRARIES}
)

set_target_properties(fx2mirisdr_shared PROPERTIES DEFINE_SYMBOL "fx2mirisdr_EXPORTS")
set_target_properties(fx2mirisdr_shared PROPERTIES OUTPUT_NAME fx2mirisdr)
set_target_properties(fx2mirisdr_shared PROPERTIES SOVERSION ${MAJOR_VERSION})
set_target_properties(fx2mirisdr_shared PROPERTIES VERSION ${LIBVER})

add_library(fx2mirisdr_static STATIC
    libfx2mirisdr.c
    tuner_msi001.c
)

target_link_libraries(fx2mirisdr_static
    ${LIBUSB_LIBRARIES}
)

set_property(TARGET fx2mirisdr_static APPEND PROPERTY COMPILE_DEFINITIONS "fx2mirisdr_STATIC" )

if(NOT WIN32)
# Force same library filename for static and shared variants of the library
set_target_properties(fx2mirisdr_static PROPERTIES OUTPUT_NAME fx2mirisdr)
endif()

########################################################################
# Build utility
########################################################################
add_executable(fx2miri_sdr fx2miri_sdr.c)
target_link_libraries(fx2miri_sdr fx2mirisdr_static
    ${LIBUSB_LIBRARIES}
)

if(WIN32)
set_property(TARGET fx2miri_sdr APPEND PROPERTY COMPILE_DEFINITIONS "fx2mirisdr_STATIC" )
endif()

########################################################################
# Install built library files & utilities
########################################################################
install(TARGETS fx2mirisdr_shared fx2mirisdr_static fx2miri_sdr
    LIBRARY DESTINATION lib${LIB_SUFFIX} # .so/.dylib file
    ARCHIVE DESTINATION lib${LIB_SUFFIX} # .lib file
    RUNTIME DESTINATION bin              # .dll file
)
