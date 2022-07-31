<?php
/**
 * 测试携程调度器
 */
while (true) {
    Sgo(function () {
        $cid = Sco::getCid();
        var_dump($cid);
    });
}