find_package(PkgConfig)

PKG_CHECK_MODULES(PC_GR_BER gnuradio-ber)

FIND_PATH(
    GR_BER_INCLUDE_DIRS
    NAMES gnuradio/ber/api.h
    HINTS $ENV{BER_DIR}/include
        ${PC_BER_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    GR_BER_LIBRARIES
    NAMES gnuradio-ber
    HINTS $ENV{BER_DIR}/lib
        ${PC_BER_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/gnuradio-berTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GR_BER DEFAULT_MSG GR_BER_LIBRARIES GR_BER_INCLUDE_DIRS)
MARK_AS_ADVANCED(GR_BER_LIBRARIES GR_BER_INCLUDE_DIRS)
