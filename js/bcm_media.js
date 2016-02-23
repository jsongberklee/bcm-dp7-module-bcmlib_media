(function( $ ) {

		jQuery.fn.extend({
	    ensureLoad: function(handler) {
        return this.each(function() {
          if(this.complete) {
              handler.call(this);
          } else {
              $(this).load(handler);
          }
        });
			}
		});

		$(document).ready(function () {
			var wrapper = $("body.page-node .field-name-field-image .field-item");
			var banner = $("body.page-node .field-name-field-image .field-item img");
			var player = $("#bcm-media");

			if(Drupal.settings.bcm_media.liveStreamingProvided){
				$(".wow_media.wowza-player").css("display", "none");
			}
			
			banner.ensureLoad(function(){
			
				if($(".bcm-media-btn").length == 0) {				
					wrapper.append('<div class="bcm-media-btn"><img src="/sites/all/modules/bcm_media/images/play-fff.png" /></div>');
					var btn = $(".bcm-media-btn");
					btn.css('width', wrapper.width()+'px')
							.css('height', wrapper.height()+'px')
							.css('margin-top', (wrapper.height()*-1)+'px');
				}
								
				var imgbtn = $(".bcm-media-btn > img");
				imgbtn.load(function(){
					imgbtn.css('margin-top', (wrapper.height()-imgbtn.height())/2+'px');
					//imgbtn.hide().css('margin-top', (wrapper.height()-imgbtn.height())/2+'px');
					/*
imgbtn.fadeIn('slow', function(){
						setInterval(function(){
							imgbtn.fadeToggle(1000, "easeInQuad")
						}, 2000);
					});
*/
					
					$(window).resize(function(){
						$(".bcm-media-btn").css('width', wrapper.width()+'px')
															.css('height', wrapper.height()+'px')
															.css('margin-top', (wrapper.height()*-1)+'px');
						imgbtn.css('margin-top', (wrapper.height()-imgbtn.height())/2+'px')
					});
				});
			});

			wrapper.click(function(){
				player.width(banner.width()); player.height(banner.height());
				wrapper.append(function() {
					if(Drupal.settings.bcm_media.liveStreamingProvided){
						
						// set autoPlay to true for the wowza player
						var wowza = $(".wow_media.wowza-player");
						wowza.html(function(index,text){
								//text.replace('controls','autoplay controls');
								return text.replace('autoPlay=false','autoPlay=true');
						});
						
						return wowza
            					.css("width", banner.width()+"px")
            					.css("height", banner.height()+"px")
            					.css("display", "table-cell");
						
					}else{
						
						return $(".bcm-media-player.bcm-media-banner")
            					.css("width", banner.width()+"px")
            					.css("height", banner.height()+"px")
            					.css("display", "table-cell");
					}

        }).find("img").remove();
				//player.trigger("play"); // for video tag control
				wrapper.unbind( "click" );
				// For google analytics uncomment below after the module activated.
				ga('send', 'event', 'Media', 'Play', 'LC Event Video: '+Drupal.settings.bcm_media.title, 1);

			});
		});

}( jQuery ));