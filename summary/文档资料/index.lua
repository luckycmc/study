local uri_args = ngx.req.get_uri_args()
local gender = uri_args['gender']
if gender =='1' then
                       
elseif gender=='0' then
   ngx.say('index lua file');
else
   ngx.exec('/goods_list_advert_from_data');
end

