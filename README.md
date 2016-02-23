A Drupal field formatter module for diplaying the http live streaming services provide by Wowza media server (v 7.x-1.xx) - 201400910

* OVERVIEW *****************************************************

Wowza media server live streaming URL to display video, it will switch player objects depend on browser type, mainly between Safari and other browsers.

The URL must be provided.

An example of the URL looks like:

http://myWowzaMediaServer.com:1234/live/myLiveStreamingTitle/playlist.m3u8?DVR

Omit the three letters "DVR" at the end of line for live streaming, "DVR" is only used for replay the streamed object.

There is no admin/config page for this module yet, so please feel free to modify html string on bcm_media.module file to customize as you like.

NOTE: All Apple devices(iOS and Desktop) and Other desktop browsers with Flash player should be working fine.
On some Android devices has playback issue with the "Video" object tag (<video></video>) that plays audio only in the beginning,
but it start show video as well once you move the timeline scroll back and forth.


* INSTALLATION & SET UP ****************************************

Same as other contributed modules


* USAGE ********************************************************

1. Create a text field for any content type (bundle) you wish to attach the player.

2. Select "http live stream player" for the text field format on Manage Display page.

3. Go to "node/add/[the content type]" and paste full URL for the streaming link.

4. Click "Save" and see if the player is loaded on the node view page.


* CONTACT ******************************************************

JAESUNG SONG (jsong@berklee.edu) : Maintainer