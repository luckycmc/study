--TEST--
Check if hello_world is loaded
--SKIPIF--
<?php
if (!extension_loaded('hello_world')) {
	echo 'skip';
}
?>
--FILE--
<?php
echo 'The extension "hello_world" is available';
?>
--EXPECT--
The extension "hello_world" is available
