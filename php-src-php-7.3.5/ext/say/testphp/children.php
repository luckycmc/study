<?php
    $children = new children();
    var_dump($children->memory);
    $children->learn("love");
    var_dump($children->memory);
    var_dump($children->toString());

    echo "*************************************************************\n";
    $loaded_extensions = get_loaded_extensions();//获取已加载的扩展
    var_dump($loaded_extensions);