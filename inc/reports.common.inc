<?php
  
function report_user_access(){
  global $user;
  return in_array('itg_developer',$user->roles);
}

function report_admin_access(){
  global $user;
  return in_array('itg_admin',$user->roles);
}


function displayfiles()
{
    $p_id = $_POST['id'];
	
	if (empty($p_id)){
		$p_id = '0';
	}
	
	$sql = "SELECT `id`, `isparent`, `fullname` FROM `mj_files` WHERE pid = '$p_id'";
    $records = db_query($sql);
	$output = '';
	$output .= '[';
    while ($data = db_fetch_object($records)) {
		$output .= '{';
		$output .= 'id:\'' . $data->id . '\',';
		$output .= 'name:\'' . $data->fullname . '\',';
		$output .= 'isParent:' . ($data->isparent == 0 ? 'false' : 'true');
		$output .= '},';
	}
	if (strlen($output) > 1){
		$output = substr($output, 0, strlen($output) - 1);
	}
	$output .= ']';
	echo $output;
}


function showTodayReports()
{
    $date = $_POST['date'];
    
    if (!isset($date) || empty($date) || strlen($date) <= 0) {
    	$date = date('Y-m-d');
    }
    $sql = "select rpt_id, issue, rdate, delivery, rpt_bts_id, botname, sector, type, time, employee, comment, others from mj_reports ";
    $sql .= "LEFT OUTER JOIN mj_bots ON bts_id = rpt_bts_id LEFT OUTER JOIN mj_sectors ON sct_id = bts_sct_id ";
    $sql .= " where rdate = '$date' order by employee desc, type";
    
    $result = ''; 
    $records = db_query($sql);
    echo '<table>';
    //$space = str_repeat("&nbsp;", 4);
    $space = '</td><td>';
    $totaltime = 0;
    while ($data = db_fetch_object($records)) {
        echo '<tr><td>' . substr($data->rpt_bts_id, 0, 3) . $space . $data->botname .$space . $data->issue . $space . '<img src="' . drupal_get_path('module', 'reports') . '/image/drop-' . ($data->delivery == 1 ? 'yes' : 'no') .'.gif" />';
        echo $space . $data->sector . $space . $data->type . $space . $data->time . $space . $data->employee. $space. $data->comment. $space;
        echo '<img class="close-img" alt="' . $data->rpt_id . '" src="' . drupal_get_path('module', 'reports') . '/image/close.gif" /></td></tr>';
        $totaltime += $data->time;
    }
    echo '</table><br /> ';
    
    if($totaltime > 0){echo 'Total time: ' . $totaltime;}
}



function zip_files($bid, $files){
	if (empty($files)){
		die('false');
	}
	
	$zip = new ZipArchive;
	$appendix = gmDate("Ymd\THis\Z");
	$path = drupal_get_path('module', 'reports') . '/temp/';
	$file =  $path . $bid . '-' . $appendix . '.zip';
	if (!file_exists($file)){
		if($zip->open($file, ZipArchive::CREATE)!==TRUE)
		{
			// Opening zip file to load files
			die("* Sorry ZIP creation failed");
		}
		else {
			foreach($files as $key => $val){
				if (!empty($val)){
					$zip->addFile($val);
				}
			}
		}
		$zip->close();
	}
	echo $file;
/*
	header('Content-Type: application/zip');
	header('Content-Length: ' . filesize($file));
	header('Content-Disposition: attachment; filename="file.zip"');
	readfile($file);
	//unlink($file); 
*/
}

function check_files($bid, $files, $checked){
	foreach($files as $key => $val){
		if (!empty($val)){
			$val = str_replace("\\", "\\\\", $val);
			$sql = "UPDATE mj_bots_files SET checked = $checked WHERE bid = $bid AND file = '$val'";
			db_query($sql);
		}
	}
}

function list_bot_related_files($bid){
	// get the timestamp when files were scaned last time
	$sql = "SELECT IFNULL(MAX(lastfound), 0) FROM mj_bots_files WHERE bid = $bid";
	$unixtime = db_result(db_query($sql));
	
	// remove the files that gone
	$sql = "DELETE FROM mj_bots_files WHERE bid = $bid ";
	if ($unixtime > 0){
		$sql .= " AND lastfound < $unixtime ";	
	}
	db_query($sql);
	
	// list the latest found items only
	$sql = "SELECT file, checked FROM mj_bots_files WHERE bid = $bid ";
	if ($unixtime > 0){
		$sql .= " AND lastfound = $unixtime ";	
	}
	$sql .= "ORDER BY checked DESC, file";
	$result = db_query($sql);
	

	$i = 0;
	while ($row = db_fetch_array($result))
	{
		$responce->f[$i]['checked']=$row[checked];
		$responce->f[$i]['file']=$row[file] . ';' ;
		$i++;
	}
	echo json_encode($responce);
}


function scan_files($bid){
	
	$sql = "SELECT version, keywords FROM `mj_bots` WHERE id = $bid";
	
	$bdata = db_fetch_object(db_query($sql));
	$version = $bdata->version;
	$keywords = $bdata->keywords;
	$scan_path = array();

	if ($version == 'R20')
	{
		$sql = "SELECT sps_path FROM mj_sectors_paths LEFT OUTER JOIN mj_bots ON bts_sct_id = sps_sct_id WHERE id = $bid";
		$result = db_query($sql); 
		while($data = db_fetch_object($result))
		{
			$scan_path[] = $data->sps_path;
		}
	}
	else
	{
		$scan_path[] = "C:\Repository\QAG\Bot\Releases\\$version";
	}
	
	module_load_include('inc', 'reports', 'inc/reports.list');
	
	$scripts = array();

	if (empty($keywords)){
//		die('Keywords can\'t be null');
		die('false');
	}
	
	foreach($scan_path as $key => $val){
		if (strpos($val, '\Config') !== false)
		{
			// attach the config files
			$sql = "SELECT scf_sct_config AS config FROM `mj_bots` LEFT OUTER JOIN mj_sectors_configs ON scf_sct_id = bts_sct_id WHERE id = $bid";
			$result = db_query($sql); 
			while($data = db_fetch_object($result))
			{
				$scripts[] = $val . '\\' . $data->config;
			}
		}
		scan_path($val, $keywords, 0, &$scripts);
	}

	$unixtime = time();
	if (!empty($scripts)){
		foreach($scripts as $key => $val)
		{
			if(!is_array($val)){
				$val = str_replace("\\", "\\\\", $val);
				// update files
				$sql = "INSERT INTO mj_bots_files VALUES ($bid, '$val', NULL, $unixtime) ON DUPLICATE KEY UPDATE lastfound = $unixtime";
				db_query($sql);
			}
		}
	} else {
		// remove the files that gone
		$sql = "DELETE FROM mj_bots_files WHERE bid = $bid ";
		if ($unixtime > 0){
			$sql .= " AND lastfound < $unixtime ";	
		}
		db_query($sql);
	}
	
	list_bot_related_files($bid);
}