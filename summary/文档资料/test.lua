	local redis = require "resty.redis";
	local instance = redis:new();
	local host = "127.0.0.1";
	local port = 6379;
	local ok,err = instance:connect(host,port);
	if not ok then
	   ngx.say("redis connect is error！");
	   ngx.log(ngx.ERR,err);
	   ngx.exit(ngx.HTTP_SERVICE_UNAVAILABLE);
	end
	local suc, err = instance:set('devil', 'hello world')
	if not suc then
		ngx.say("redis set is error")
	else
		ngx.say(instance:get('devil'))
	end
