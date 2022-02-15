<?php
    /* function getSize($value) {
        if (is_string($value)) {
            return "string size is ". strlen($value);
        } else if (is_array($value)) {
            return "array size is ". sizeof($value);
        } else {
              return "can not support";
        }
    } */
     
    var_dump(get_size("abc"));
    var_dump(get_size(array(1,2)));
    var_dump(get_size(666666));