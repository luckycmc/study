<?php
Sgo(function () {
    echo "co1 before sleep" . PHP_EOL;
    SCo::sleep(1000);
    echo "co1 after sleep" . PHP_EOL;
});

Sgo(function () {
    echo "co2 before sleep" . PHP_EOL;
    SCo::sleep(2000);
    echo "co2 after sleep" . PHP_EOL;
});