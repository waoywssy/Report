<?php
  function set_path(){
	$sid = $_REQUEST['sid'];
	$path = $_REQUEST['p'];
	$op = $_REQUEST['op'];
	if (!empty($op)&&!empty($sid)&& !empty($path)){
		// handling ajax requests
		$path = str_replace("\\", "\\\\", $path);
		if ($op=='add'){
			$sql = "INSERT IGNORE INTO mj_sectors_paths VALUES($sid, '$path')";
		} else if ($op=='del'){
			$sql = "DELETE FROM mj_sectors_paths WHERE sps_sct_id = $sid AND sps_path = '$path'";
			//var_dump($sql);
		}
		db_query($sql);
		exit(0);
	}
	
	/// displaying page content
    drupal_add_css(drupal_get_path('module', 'reports') .'/css/tree.css');
    drupal_add_js(drupal_get_path('module', 'reports') .'/js/setpath.js');
	return '<div><select id="sectors"></select></div><div id="paths"></div><div><button id="add" type="button">Add</button><input id="newpath" value="" size="60"/>';
  }