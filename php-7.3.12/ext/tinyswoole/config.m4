dnl config.m4 for extension tinyswoole

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(tinyswoole, for tinyswoole support,
dnl Make sure that the comment is aligned:
dnl [  --with-tinyswoole             Include tinyswoole support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(tinyswoole, whether to enable tinyswoole support,
dnl Make sure that the comment is aligned:
[  --enable-tinyswoole          Enable tinyswoole support], no)

if test "$PHP_TINYSWOOLE" != "no"; then
  dnl Write more examples of tests here...

  dnl # get library FOO build options from pkg-config output
  dnl AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
  dnl AC_MSG_CHECKING(for libfoo)
  dnl if test -x "$PKG_CONFIG" && $PKG_CONFIG --exists foo; then
  dnl   if $PKG_CONFIG foo --atleast-version 1.2.3; then
  dnl     LIBFOO_CFLAGS=\`$PKG_CONFIG foo --cflags\`
  dnl     LIBFOO_LIBDIR=\`$PKG_CONFIG foo --libs\`
  dnl     LIBFOO_VERSON=\`$PKG_CONFIG foo --modversion\`
  dnl     AC_MSG_RESULT(from pkgconfig: version $LIBFOO_VERSON)
  dnl   else
  dnl     AC_MSG_ERROR(system libfoo is too old: version 1.2.3 required)
  dnl   fi
  dnl else
  dnl   AC_MSG_ERROR(pkg-config not found)
  dnl fi
  dnl PHP_EVAL_LIBLINE($LIBFOO_LIBDIR, TINYSWOOLE_SHARED_LIBADD)
  dnl PHP_EVAL_INCLINE($LIBFOO_CFLAGS)

  dnl # --with-tinyswoole -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/tinyswoole.h"  # you most likely want to change this
  dnl if test -r $PHP_TINYSWOOLE/$SEARCH_FOR; then # path given as parameter
  dnl   TINYSWOOLE_DIR=$PHP_TINYSWOOLE
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for tinyswoole files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       TINYSWOOLE_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$TINYSWOOLE_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the tinyswoole distribution])
  dnl fi

  dnl # --with-tinyswoole -> add include path
  dnl PHP_ADD_INCLUDE($TINYSWOOLE_DIR/include)

  dnl # --with-tinyswoole -> check for lib and symbol presence
  dnl LIBNAME=TINYSWOOLE # you may want to change this
  dnl LIBSYMBOL=TINYSWOOLE # you most likely want to change this

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $TINYSWOOLE_DIR/$PHP_LIBDIR, TINYSWOOLE_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_TINYSWOOLELIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong tinyswoole lib version or lib not found])
  dnl ],[
  dnl   -L$TINYSWOOLE_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(TINYSWOOLE_SHARED_LIBADD)

  dnl # In case of no dependencies
  AC_DEFINE(HAVE_TINYSWOOLE, 1, [ Have tinyswoole support ])

  PHP_NEW_EXTENSION(tinyswoole, tinyswoole.c, $ext_shared)
fi
