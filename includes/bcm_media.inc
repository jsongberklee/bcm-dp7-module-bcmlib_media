<?php

function _bcm_contest_media_render_player($entity, $items){

	$formatted = '<div class="bcm-contest-media-player">';

	foreach ($items as $delta => $item) {
		//dpm($item['value']);
		$formatted .= $item['value'].', ';
  }

	$formatted .= '</div>';

	return $formatted;

}

function _bcm_media_add_js($title, $liveStreamingProvided, $access){

	drupal_add_js(array('bcm_media' => array('title' => $title, 'liveStreamingProvided' => $liveStreamingProvided)), array('type' => 'setting'));
	if($access === 'autoplay'){
		drupal_add_js(drupal_get_path('module', 'bcm_media') .'/js/bcm_media_autoplay.js', 'file');
	}else{
		drupal_add_js(drupal_get_path('module', 'bcm_media') .'/js/bcm_media.js', 'file');
	}

}

function _bcm_event_video_render_player($entity, $items){

	// to validate the user access
	global $user;
	$userAccess = $user->uid;
	$publicAccess = $entity->field_video_access['und'][0]['value'];
	$access = _bcm_media_check_access($userAccess, $publicAccess);

	// if this is true, we'll pass true to javascript and it'll will deal with it.
	$liveStreamingProvided = false;

	// prepare GA event lable : title + date
	$display = array('label'=>'hidden','settings' => array('format_type' => 'ymd'));
	$eventDate = trim(strip_tags(render(field_view_field('node', $entity, 'field_lceventdate', $display))));
	$eventTitle = $entity->title.' ['.$eventDate.']';

	// check if the video link url is provided
	if(!isset($entity->field_video_link) || empty($entity->field_video_link['und'][0]['value'])){

		// check if the live streaming field is provided
		if(isset($entity->field_live_streaming)
				&& !empty($entity->field_live_streaming['und'][0]['value'])
				&& empty($items)
			){

			$liveStreamingProvided = true;
			_bcm_media_add_js($eventTitle, $liveStreamingProvided, $access);
		}

		$elements[0]['#markup'] = '';
		return $elements;

	}


	$defaultHeight = 485;

	$bannerHeight = $entity->field_image['und'][0]['height'];
	$bannerWidth = $entity->field_image['und'][0]['width'];

	$elements = array();

	//dpm($browser);
	$modulePath =  file_create_url(drupal_get_path('module', 'bcm_media'));

	$formatted = '<div class="bcm-media-player '. ($bannerHeight ? 'bcm-media-banner' : '') .'">';
	//$formatted = '<div class="bcm-media-player">';

	if(!$access){
		// the query "video=autoplay" is not going to work due to implementing SSO
		// need to find alternate solution for this
		$loginLinks = l(t('Sorry, this video is only available to the currently enrolled Berklee student, click to login.'), 'onelogin_saml/sso', array('query' => array('destination' => '/'.drupal_get_path_alias(), 'video' => 'autoplay'), 'attributes' => array('title' => t('Click here to login'))));
		$formatted .= '<h3>'.$loginLinks.'</h3>';
	}
	else{

		foreach ($items as $delta => $item) {
			$url = 'http://lrmedia.berklee.edu:1935/learningcenter/_definst_/lcmedia/'.($publicAccess ? 'public' : 'limitedaccess').'/video/mp4:'.$item['value'];
			if(_bcm_media_check_client() == 'video'){
				$url .= '/playlist.m3u8';
				$formatted .= '<video id="bcm-media" x-webkit-airplay="allow" alt="" width="100%" height="'.($bannerHeight ? $bannerHeight : $defaultHeight).'px" src="'.$url.'" controls >'; // autoplay '.($access == 'autoplay' ? 'autoplay' : '').'
			}
			else{
				$url .= '/manifest.f4m';
				$formatted .= '
				  <object type="application/x-shockwave-flash" name="bcm-media" data="'.$modulePath.'/StrobeMediaPlayback.swf" id="bcm-media" style="visibility: visible;" width="100%" height="'.($bannerHeight ? $bannerHeight : $defaultHeight).'">
				  	<param name="allowFullScreen" value="true">
				  	<param name="wmode" value="direct">
				  	<param name="flashvars" value="src='.$url.'&amp;autoPlay=true&amp;verbose=true&amp;controlBarAutoHide=false&amp;controlBarPosition=bottom&amp;playButton=Connect">
				  </object>';
			}
	  }
	}

	$formatted .= '</div>';

	_bcm_media_add_js($eventTitle, $liveStreamingProvided, $access);

	return $formatted;
}
function _bcm_media_check_access($userAccess, $publicAccess){

	if($publicAccess || $userAccess){

		//dpm($_GET);
		// this get value is set for login user who logged in by click the play button on the video player
		if(isset($_GET['video']) && $_GET['video'] == 'autoplay'){
			 return 'autoplay';
		}

		return true;
	}

	// check if the visitor is from outside of Berklee network, then disable all media..
	// apperently '192.136.22' only works for berklee staff, berklee public
	//echo ip_address();
	// temporary adding '172.17.1'
	if(strstr(ip_address(), '192.136') != false || strstr(ip_address(), '172.17') != false){
		return true;
	}else{
		return false;
	}

}

function _bcm_media_check_client(){
	$browser = _bcm_media_get_browser();
	if(strpos($browser['userAgent'], 'Mobile Safari') !== FALSE
			|| strpos($browser['name'], 'Apple Safari') !== FALSE
			|| strpos($browser['userAgent'], 'Android') !== FALSE
		){
		return 'video';
	}
	else{
		return 'object';
	}

}
/*
 * this function is originally part of jsongutil module,
 * original name: _get_browser
 * @description: returns an array of browser information
 * @parameters: none
 */
function _bcm_media_get_browser()
{
    $u_agent = $_SERVER['HTTP_USER_AGENT'];
    $bname = 'Unknown';
    $platform = 'Unknown';
    $version= "";

    if (preg_match('/linux/i', $u_agent)) {
        $platform = 'linux';
    }
    elseif (preg_match('/macintosh|mac os x/i', $u_agent)) {
        $platform = 'mac';
    }
    elseif (preg_match('/windows|win32/i', $u_agent)) {
        $platform = 'windows';
    }

    if(preg_match('/MSIE/i',$u_agent) && !preg_match('/Opera/i',$u_agent))
    {
        $bname = 'Internet Explorer';
        $ub = "MSIE";
    }
    elseif(preg_match('/Firefox/i',$u_agent))
    {
        $bname = 'Mozilla Firefox';
        $ub = "Firefox";
    }
    elseif(preg_match('/Chrome/i',$u_agent))
    {
        $bname = 'Google Chrome';
        $ub = "Chrome";
    }
    elseif(preg_match('/Safari/i',$u_agent))
    {
        $bname = 'Apple Safari';
        $ub = "Safari";
    }
    elseif(preg_match('/Opera/i',$u_agent))
    {
        $bname = 'Opera';
        $ub = "Opera";
    }
    elseif(preg_match('/Netscape/i',$u_agent))
    {
        $bname = 'Netscape';
        $ub = "Netscape";
    }

    $known = array('Version', $ub, 'other');
    $pattern = '#(?<browser>' . join('|', $known) .
    ')[/ ]+(?<version>[0-9.|a-zA-Z.]*)#';
    if (!preg_match_all($pattern, $u_agent, $matches)) {
        // we have no matching number just continue
    }

    $i = count($matches['browser']);
    if ($i != 1) {
        if (strripos($u_agent,"Version") < strripos($u_agent,$ub)){
            $version= $matches['version'][0];
        }
        else {
            $version= $matches['version'][1];
        }
    }
    else {
        $version= $matches['version'][0];
    }

    if ($version==null || $version=="") {$version="?";}

    return array(
        'userAgent' => $u_agent,
        'name'      => $bname,
        'version'   => $version,
        'platform'  => $platform,
        'pattern'    => $pattern
    );
}

function _get_iOSplayer($entity, $items)
{

	// set html media tag
	$mediaTag = substr($vars["items"][0]["#markup"], -3) == 'mp3' ? "audio" : "video";
/*

	// url to added infront of the filename
	$preUrl = 'http://lrmedia.berklee.edu:1935/learningcenter/_definst_/lcmedia/'.$vars['video_preurl'].'/'.$mediaTag.'/'.($mediaTag == 'audio' ? 'mp3:' : '');

	// url to added after the filename
	$postUrl = '/playlist.m3u8';

	// js array of the file names
	$jsArray = '<script type="text/javascript">alist=[';

	// will be used if there are no title provided
	$noTitle = "Title is not set!";

	// total length of media
	$iLength = (count($vars['items'])-1);

	// <select> tag content holder
	$htmlTitleSet = '';

	if($iLength == 0) // check if there is only one media provided
	{
		$jsArray .= '"'. $preUrl . $vars["items"][0]["#markup"] . $postUrl . '"';
		$htmlTitleSet = '<div id="mediasingleitem" >'.($aVideoTitle[0]["value"] ? $aVideoTitle[0]["value"] : $noTitle).'</div>';
	}
	else // if there is more than one media is provided
	{
		$htmlTitleSet = '<select id="mediaselect" onchange="playselected(this.value)"><option id="mediatitlelist">Select Media</option>';
		for($i=0; $i <= $iLength; $i++)
		{
			$title = (empty($aVideoTitle[$i]["value"]) ? $noTitle : $aVideoTitle[$i]["value"]);
	    $jsArray .= '"'. $preUrl . $vars["items"][$i]["#markup"] . $postUrl . ($i == $iLength ? '"' : '",');
			$ii = ($i+1); $htmlTitleSet .= '<option id="mediaoption'. $ii .'"value="'. $ii . '" >' . $ii . ' : '.$title.'</option>';
		}
		$htmlTitleSet .= '</select>';

	}

	$jsArray .= '];loadList(alist); ' .$videoPoster. '</script>';

    $iOSoutput = '
	    <a name="mediaplayer"></a><div id=lcmedialist>'.$htmlTitleSet.'</div>
			<'.$mediaTag.' id="lcmedia" width="100%" height="268" style="width:100%; background-color:#000000;" controls="controls" preload="none" >
	      HTML5 media not supported
	    </'.$mediaTag.'>
			<div id="tokentest"></div>' . $jsArray;

		return $iOSoutput;
*/
}
