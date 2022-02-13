<?php

function default_value ($type, $value = null) {
    if ($type == "int") {
        return $value ?? 0;
    } else if ($type == "bool") {
        return $value ?? false;
    } else if ($type == "str") {
        return is_null($value) ? "" : $value;
    }
    return null;
}

var_dump(default_value("int"));
var_dump(default_value("int", 1));
var_dump(default_value("bool"));
var_dump(default_value("bool", true));
var_dump(default_value("str"));
var_dump(default_value("str", "a"));
var_dump(default_value("array"));