--TEST--
imagecreatefromstring() - WEBP format
--SKIPIF--
<?php
if (!extension_loaded('gd')) die('skip ext/gd required');
if (!(imagetypes() & IMG_WEBP)) die('skip WEBP support required');
?>
--FILE--
<?php
// create an image from a WEBP string representation
$im = imagecreatefromstring(file_get_contents(__DIR__ . '/imagecreatefromstring.webp'));
var_dump(imagesx($im));
var_dump(imagesy($im));

?>
===DONE===
--EXPECT--
int(10)
int(10)
===DONE===
