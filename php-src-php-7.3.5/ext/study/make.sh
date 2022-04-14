#!/bin/bash

/usr/local/php7.3/bin/phpize --clean

/usr/local/php7.3/bin/phpize

./configure --with-php-config=/usr/local/php7.3/bin/php-config

make clean

make && make install