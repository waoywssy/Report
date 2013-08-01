<?php
function getpath(){
	$sector_id = $_REQUEST['sid'];
	$sql = "SELECT * FROM mj_sectors_paths WHERE sps_sct_id = $sector_id";
	$data = db_query($sql);
	$output = array();
	while ($row = db_fetch_object($data)){
		$val = $row->sps_path . ';';
		//$val = str_replace("\\\\", "\\", $val);
		$output[] = $val;
	}
	drupal_json($output);
}
?>