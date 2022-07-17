#!/bin/btudy_coroutine_util.cc \sh

#/usr/local/php7.3/bin/phpize --clean

#/usr/local/php7.3/bin/phpize

#./configure --with-php-config=/usr/local/php7.3/bin/php-config

/usr/local/php/bin/phpize --clean

/usr/local/php/bin/phpize

./configure --with-php-config=/usr/local/php/bin/php-config

make clean

make && make install
