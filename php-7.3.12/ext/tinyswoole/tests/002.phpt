--TEST--
tinyswoole_test1() Basic test
--SKIPIF--
<?php
if (!extension_loaded('tinyswoole')) {
	echo 'skip';
}
?>
--FILE--
<?php
$ret = tinyswoole_test1();

var_dump($ret);
?>
--EXPECT--
The extension tinyswoole is loaded and working!
NULL
