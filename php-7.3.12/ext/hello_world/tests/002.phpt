--TEST--
hello_world_test1() Basic test
--SKIPIF--
<?php
if (!extension_loaded('hello_world')) {
	echo 'skip';
}
?>
--FILE--
<?php
$ret = hello_world_test1();

var_dump($ret);
?>
--EXPECT--
The extension hello_world is loaded and working!
NULL
