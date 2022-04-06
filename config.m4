PHP_ARG_ENABLE(simhash, whether to enable simhash support,
[  --enable-simhash           Enable simhash support])

if test "$PHP_SIMHASH" != "no"; then
   PHP_NEW_EXTENSION(simhash, php_simhash.c common.c simhash.c simhash_factory.c, $ext_shared)
fi
