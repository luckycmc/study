--TEST--
Check if tinyswoole is loaded
--SKIPIF--
<?php
if (!extension_loaded('tinyswoole')) {
	echo 'skip';
}
?>
--FILE--
<?php
echo 'The extension "tinyswoole" is available';
?>
--EXPECT--
The extension "tinyswoole" is available
