dnl $Id$
dnl config.m4 for extension sff

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(sff, for sff support,
Make sure that the comment is aligned:
[  --with-sff             Include sff support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(sff, whether to enable sff support,
Make sure that the comment is aligned:
[  --enable-sff           Enable sff support])

if test "$PHP_SFF" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-sff -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/sff.h"  # you most likely want to change this
  dnl if test -r $PHP_SFF/$SEARCH_FOR; then # path given as parameter
  dnl   SFF_DIR=$PHP_SFF
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for sff files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       SFF_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$SFF_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the sff distribution])
  dnl fi

  dnl # --with-sff -> add include path
  dnl PHP_ADD_INCLUDE($SFF_DIR/include)

  dnl # --with-sff -> check for lib and symbol presence
  dnl LIBNAME=sff # you may want to change this
  dnl LIBSYMBOL=sff # you most likely want to change this

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $SFF_DIR/$PHP_LIBDIR, SFF_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_SFFLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong sff lib version or lib not found])
  dnl ],[
  dnl   -L$SFF_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(SFF_SHARED_LIBADD)

  PHP_NEW_EXTENSION(sff,\
  sff.c \
  tool/load_file.c \
  sff_interface/load_config.c \
  tool/ssf_string.c \
  tool/ssf_route.c \
  tool/super_container.c \
  tool/sff_process.c \
  tool/sff_signal_handle.c \
  sff_interface/ssf_application.c \
  sff_interface/factory_controller.c \
  sff_interface/sff_loader.c \
  sff_interface/sff_container.c \
  zend_ssf_interface/interface.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi