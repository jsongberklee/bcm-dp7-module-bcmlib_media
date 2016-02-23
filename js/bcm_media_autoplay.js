(function( $ ) {

		// create small plugin for ensuring function to be called after the target image is loaded or refreshed.
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

			if(Drupal.settings.bcm_media.liveStreamingProvided){
				$(".wow_media.wowza-player").css("display", "none");
			}


			banner.ensureLoad(function(){
				var player = $("#bcm-media");
				player.width(banner.width()); player.height(banner.height());
				wrapper.append(function() {

					if(Drupal.settings.bcm_media.liveStreamingProvided){

						// set autoPlay to true for the wowza player
						var wowza = $(".wow_media.wowza-player");
						wowza.html(function(index,text){
								return text.replace('autoPlay=false','autoPlay=true');
						});

						return wowza
            					.css("width", banner.width()+"px")
            					.css("height", banner.height()+"px")
            					.css("display", "block");

					}else{

						return $(".bcm-media-player.bcm-media-banner")
            					.css("width", banner.width()+"px")
            					.css("height", banner.height()+"px")
            					.css("display", "block");
					}

        });
				player.trigger("play");
				banner.remove();

				ga('send', 'event', 'Media', 'Play', 'LC Event Video: '+Drupal.settings.bcm_media.title, 1);

			});
		});

}( jQuery ));