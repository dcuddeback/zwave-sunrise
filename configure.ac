AC_PREREQ([2.68])
AC_INIT([sunrise], [0.1.0], [david.cuddeback@gmail.com])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])

AC_CONFIG_SRCDIR([src/sunrise.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_INSTALL

# Checks for libraries.
AC_LANG([C++])
AC_LANG_CPLUSPLUS
AX_CXX_COMPILE_STDCXX_11([noext], [mandatory])

AC_CHECK_LIB([udev], [udev_ref],
             [LIBS="$LIBS -ludev"],
             [AC_MSG_ERROR(libudev is not installed.)])
AC_CHECK_LIB([openzwave], [ozw_vers],
             [CXXFLAGS="$CXXFLAGS -I/usr/include/openzwave -I/usr/include/openzwave/value_classes -I/usr/include/openzwave/platform"]
             [LIBS="$LIBS -lopenzwave -ludev"],
             [AC_MSG_ERROR([libopenzwave is not installed.])])
AC_CHECK_LIB([pthread], [pthread_create],
             [LIBS="$LIBS -lpthread"],
             [AC_MSG_ERROR(libpthread is not installed.)])

# Checks for header files.
AC_CHECK_HEADERS([stdint.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
