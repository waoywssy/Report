<?php
	//$sectors = array();
	$sql = "select sct_id as id, sector from mj_sectors";
	$data = db_query($sql);
	$i = 0;
	while ($rec = db_fetch_object($data)){
		$sectors->f[$i]['id']=$rec->id;
		$sectors->f[$i]['sector']=$rec->sector;
		$i++;
	}
	echo str_replace(
			array('<', '>', '&'), 
			array('\\u003c', '\\u003e', '\\u0026'), 
			json_encode($sectors));