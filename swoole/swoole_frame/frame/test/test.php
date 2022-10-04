<?php
require __DIR__.'/../vendor/autoload.php';

echo (new \SwoStar\Index())->index();

echo app('index')->index();