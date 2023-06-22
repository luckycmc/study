
--获取get或post参数--------------------

local request_method = ngx.var.request_method
local args = nil
local param = nil
--获取参数的值
if "GET" == request_method then
    args = ngx.req.get_uri_args()
elseif "POST" == request_method then
    ngx.req.read_body()
    args = ngx.req.get_post_args()
end
sku_id = args["sku_id"]


--关闭redis的函数--------------------

local function close_redis(redis_instance)
    if not redis_instance then
        return
    end
    local ok,err = redis_instance:close();
    if not ok then
        ngx.say("close redis error : ",err);
    end
end


--连接redis--------------------

local redis = require("resty.redis");
--local redis = require "redis"
-- 创建一个redis对象实例。在失败，返回nil和描述错误的字符串的情况下
local redis_instance = redis:new();
--设置后续操作的超时（以毫秒为单位）保护，包括connect方法
redis_instance:set_timeout(1000)
--建立连接
local ip = '127.0.0.1'
local port = 6379
--尝试连接到redis服务器正在侦听的远程主机和端口
local ok,err = redis_instance:connect(ip,port)
if not ok then
    ngx.say("connect redis error : ",err)
    return close_redis(redis_instance);
end


--从redis里面读取开关--------------------

local key = "level_goods_list_advert"
local switch, err = redis_instance:get(key)
if not switch then
    ngx.say("get msg error : ", err)
    return close_redis(redis_instance)
end


--得到的开关为空处理--------------------

if switch == ngx.null then
    switch = "FROM_DATA"  --比如默认值
end


--当开关是要从服务中获取数据时--------------------
if "FROM_DATA" == switch then
    ngx.exec('/goods_list_advert_from_data');

--当开关是要从缓存中获取数据时--------------------
elseif "FROM_CACHE" == switch then

    local resp, err = redis_instance:get("nihao")
    ngx.say(resp)

--当开关是要从静态资源中获取数据时--------------------
elseif "FROM_STATIC" == switch then

    ngx.header.content_type="application/x-javascript;charset=utf-8"
    local file = "/etc/nginx/html/goods_list_advert.json"
    local f = io.open(file, "rb")
    local content = f:read("*all")
    f:close()
    ngx.print(content)

--当开关是要停掉数据获取时--------------------
elseif "SHUT_DOWN" == switch then

    ngx.say('no data')
end

--close_redis(redis_instance)
--ngx.exit(ngx.OK)


--判断错误的响应，并进行计数， 后续便可以参考这个数值进行降级
if tonumber(ngx.var.status) == 200 then
    ngx.say(ngx.var.status)
    ngx.log(ngx.ERR,"upstream reponse status is " .. ngx.var.status .. ",please notice it")

    local error_count, err = redis_instance:get("error_count_goods_list_advert")
    --得到的数据为空处理
    if error_count == ngx.null then
        error_count = 0
    end
    error_count = error_count + 1

    --统计错误次数到error_count_goods_list_advert
    local resp,err = redis_instance:set("error_count_goods_list_advert",error_count)
    if not resp then
        ngx.say("set msg error : ",err)
        return close_redis(redis_instance)
    end
end

close_redis(redis_instance)