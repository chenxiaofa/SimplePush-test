(function(){
	var _WebPush,_Jsonp_Polling;
	window.WebPush = _WebPush = 
		function(params){
			var host      = '';
			var port      = 80;
			var timeout   = 30000;
			var APP_ID    = '';
			var on_data_received = null;
			var connector = null;
			if(params){
				host    = params.host || host ;
				port    = params.port || port  ;
				timeout = params.timeout || timeout;
				on_data_received = params.on_data_received || on_data_received;
				APP_ID  = params.APP_ID || APP_ID;
			}

			var _on_data_receiced =
				function(data){
					if(on_data_received)
						on_data_received(data);
				}

			connector = new this.Jsonp_Polling(
														{
														'host':host,
														'port':port,
														'on_data_received':_on_data_receiced,
														'APP_ID':APP_ID,
														}
												);
			this.start = 
				function(){
					connector.regiest();
				}
		};
	_WebPush.ACTION_REGIEST = 0;
	_WebPush.ACTION_POLLING = 1;

	_WebPush.prototype.Jsonp_Polling = _Jsonp_Polling = 
		function(params){
			//init
			var ajax = $.ajax;
			var polling = false;
			//end of init
			
			//server path
			var url = 'http://'+params.host+':'+params.port;
			
			//events
			var on_data_received = params.on_data_received;
			
			var do_require = function(){
				ajax(jsonp_option);
			};

			var on_complete = 
				function(){
					console.log("jsonp_polling on_complete");
					if(polling)
						setTimeout(do_require(),0);
				}
			var on_success = 
				function(data){
					console.log("jsonp_polling on_success");
					if (app_settings.action == WebPush.ACTION_POLLING)
					{

						on_data_received(data);
					}
					else if (app_settings.action == WebPush.ACTION_REGIEST)
					{
						app_settings.action = WebPush.ACTION_POLLING;
						if(data.data.sid)
							app_settings.sid = data.data.sid;
						on_data_received({data:"session id "+data.data.sid});
						jsonp_option.data = app_settings;
						polling = true;
					}
				}
			var on_error  = 
				function(data){
					console.log("jsonp_polling on_error");
				}
			var app_settings = {
								app_id:params.APP_ID,
								action:WebPush.ACTION_REGIEST,
							}
			var jsonp_option = 
			{
				url:url,
				accepts:'text',
				type : "post",
				dataType : "jsonp",
				data:app_settings,
				jsonp: "callback",//服务端用于接收callback调用的function名的参数
				jsonpCallback:"jCallback",//callback的function名称
				success:on_success,
				complete:on_complete,
				error:on_error,
			};
			this.regiest = function()
			{
				jsonp_option.data = app_settings;
				do_require();
			}
			start_polling = function()
			{
				polling = true;
				do_require();
			}
			this.stop_polling = function(){
				polling = false;
			}
		}


})();