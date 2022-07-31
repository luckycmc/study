<?php
print_r(token_get_all('<?php echo "hello";$a=1;echo $a;?>'));