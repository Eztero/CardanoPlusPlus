AC_PREREQ([2.69])
AC_INIT([libcardanoplusplus], [0.0.0], [tricahuepool@protonmail.com])
LT_INIT([dlopen win32-dll])
AC_CONFIG_MACRO_DIRS([m4])
#m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
CARDANOPLUPLUS_LIBRARY_VERSION=0:1:0
AC_SUBST(CARDANOPLUPLUS_LIBRARY_VERSION)
EXTRA_FLAGS="-version-info $CARDANOPLUPLUS_LIBRARY_VERSION"
AC_SUBST(EXTRA_FLAGS)
AM_MAINTAINER_MODE([enable])
AC_PREFIX_DEFAULT(/usr)
AC_CONFIG_HEADERS([config.h:config.hin])
AM_INIT_AUTOMAKE([dist-bzip2 tar-ustar foreign subdir-objects])
AC_CONFIG_FILES([Makefile src/Makefile include/Makefile])
CXXFLAGS="-std=c++11"
#LT_LANG(C++)
AC_LANG(C++)
AC_PROG_CXX
AC_CHECK_HEADERS([vector cstdint string cstring new cstdlib],,[AC_MSG_ERROR([not all required headers found.])])
AC_SEARCH_LIBS(sodium_init, sodium , ,[AC_MSG_ERROR([ search cannot link with -lsodium, install libsodium.])] )
#PKG_CHECK_MODULES([libsodium], [libsodium >= 1.0.16])
AC_OUTPUT

echo \
"----------------------------------------------------------------

${PACKAGE_NAME} Version ${CARDANOPLUPLUS_LIBRARY_VERSION}

Current compiler options:

   CXX:                                 ${CXX}
   CXXFLAGS:                            ${CXXFLAGS}
   LIB:                                 ${LIBS}
   PREFIX:                              ${prefix}
   
----------------------------------------------------------------"
