<?php

function list_files_under_path($path, &$scripts){
	if (file_exists($path)){
		$files = scandir($path);
		foreach ($files as $key => $value) {
			if ($value != '.' && $value != '..'){
				$fullpath = $path . '/' . $value;
				if (is_dir($fullpath)){
					list_files_under_path($fullpath, &$scripts);
				}
				else {
					$scripts[] = $fullpath;
				}
			}
		}
	}
}

function deploy(){
	$result = '';
	$result .= t('Deploy by One Click:');
    $result .= t('<form id="deploy" name="deploy" action="#" method="post" />');
    $result .= t('&nbsp;&nbsp;<button id="btn_deploy" type="submit">Deploy</button>&nbsp;&nbsp;<br /><br />');
    $result .= t('</form>');

	$path = drupal_get_path('module', 'reports') . '/uploads/' . date('Ymd');// // Relative to the root
	$path = $_SERVER['DOCUMENT_ROOT'] .'/drupal/'. $path;

	if (file_exists($path)){
		$files = scandir($path);
		$scripts = array();
		
		foreach ($files as $key => $value) {
			if ($value != '.' && $value != '..'){
	//			$result .= t('<span>' . $value . '</span><br/>');			
				list_files_under_path($path . '/' . $value, &$scripts);
			}
		}

		asort($scripts);
		$i = 1;
		foreach ($scripts as $key => $value) {
			if (stripos($value, "path.txt") <= 0){
				// doesn't display path.txt
				$temp = substr($value, 80);
				$result .= t($i++ . ') ' . $temp . '<br />');		
			}
		}
	}
	else {
		$result .= t('No delivery yet today!!');
	}
    //var_dump($scripts);
    return $result;
}