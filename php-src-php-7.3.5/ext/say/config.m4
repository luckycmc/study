PHP_ARG_ENABLE(say, whether to enable say support,
Make sure that the comment is aligned:
[  --enable-say          Enable say support], no)

if test "$PHP_SAY" != "no"; then

  PHP_NEW_EXTENSION(say, say.c children.c php_types.c php_callback.c server.c tinyswoole.c,$ext_shared)
fi
