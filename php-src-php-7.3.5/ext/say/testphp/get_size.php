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
     
    var_dump(getSize("abc"));
    var_dump(getSize(array(1,2)));
    var_dump(getSize(666666));