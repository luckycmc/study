<?php
study_event_init();
// runtime hook
Study\Runtime::enableCoroutine();
zpw_go(function ()
{
    var_dump(Study\Coroutine::getCid());
    sleep(1);
    var_dump(Study\Coroutine::getCid());
});

zpw_go(function ()
{
    var_dump(Study\Coroutine::getCid());
});
study_event_wait();