dnl $Id$
dnl config.m4 for extension simhash

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(simhash, for simhash support,
dnl Make sure that the comment is aligned:
dnl [  --with-simhash             Include simhash support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(simhash, whether to enable simhash support,
dnl Make sure that the comment is aligned:
[  --enable-simhash           Enable simhash support])

if test "$PHP_SIMHASH" != "no"; then
  dnl Write more examples of tests here...

  dnl # get library FOO build options from pkg-config output
  dnl AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
  dnl AC_MSG_CHECKING(for libfoo)
  dnl if test -x "$PKG_CONFIG" && $PKG_CONFIG --exists foo; then
  dnl   if $PKG_CONFIG foo --atleast-version 1.2.3; then
  dnl     LIBFOO_CFLAGS=`$PKG_CONFIG foo --cflags`
  dnl     LIBFOO_LIBDIR=`$PKG_CONFIG foo --libs`
  dnl     LIBFOO_VERSON=`$PKG_CONFIG foo --modversion`
  dnl     AC_MSG_RESULT(from pkgconfig: version $LIBFOO_VERSON)
  dnl   else
  dnl     AC_MSG_ERROR(system libfoo is too old: version 1.2.3 required)
  dnl   fi
  dnl else
  dnl   AC_MSG_ERROR(pkg-config not found)
  dnl fi
  dnl PHP_EVAL_LIBLINE($LIBFOO_LIBDIR, SIMHASH_SHARED_LIBADD)
  dnl PHP_EVAL_INCLINE($LIBFOO_CFLAGS)

  dnl # --with-simhash -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/simhash.h"  # you most likely want to change this
  dnl if test -r $PHP_SIMHASH/$SEARCH_FOR; then # path given as parameter
  dnl   SIMHASH_DIR=$PHP_SIMHASH
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for simhash files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       SIMHASH_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$SIMHASH_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the simhash distribution])
  dnl fi

  dnl # --with-simhash -> add include path
  dnl PHP_ADD_INCLUDE($SIMHASH_DIR/include)

  dnl # --with-simhash -> check for lib and symbol presence
  dnl LIBNAME=simhash # you may want to change this
  dnl LIBSYMBOL=simhash # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $SIMHASH_DIR/$PHP_LIBDIR, SIMHASH_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_SIMHASHLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong simhash lib version or lib not found])
  dnl ],[
  dnl   -L$SIMHASH_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(SIMHASH_SHARED_LIBADD)

  PHP_NEW_EXTENSION(simhash, php_simhash.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
