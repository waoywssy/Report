<?php

/*
 * 从给定的两个数值中获取最值，无则返回空
 *
 */
function getValue($x, $y, $islower = TRUE){
	if ($x > 0 && $y > 0){
		return $islower ? min($x, $y) : max($x, $y);
	} else if ($x > 0){
		return $x;
	} else if ($y > 0){
		return $y;
	} else {
		return 'NULL';
	} 
}

/*
 * 获取company对象，包含三个属性`cmp_lct_id`, `cmp_name`, `cmp_address`
 *
 */
function getCompany($db, $id){
	static $companies = array();

	// First check if dealing with a previously defined static variable.
	if (isset($companies[$id]) || array_key_exists($id, $companies)) {
		// Non-NULL $id and $companies[$id] statics exist.
		if (DEBUG){
			//echo '<span style="color:red">company cached</span><br />';
		}
		return $companies[$id];
	}
	if (DEBUG){
		//echo '<span style="color:red">company found</span><br />';
	}

	$query = "SELECT `cmp_lct_id`, `cmp_name`, `cmp_address` FROM `companies_cmp` WHERE cmp_id = $id";
	if (count($companies) >= CACHE_SIZE_COMPANY){
		// remove the first cached company
		array_shift($companies);
	}
	$companies[$id] = $db->GetItem($query);
	return $companies[$id];
}

/*
 * 获取公司地址：
 * 		只取中国大陆地址
 *				
 *
 */
function getCompanyLocation($db, $cmp_lct_id){
	$len = strlen($cmp_lct_id);
	if ($len != 16){
		// not in China 
		return NULL;
	}
	if (substr($cmp_lct_id, 0, 1) != "1"){
		// not in China 
		return NULL;
	}
	$id = substr($cmp_lct_id, 1, 6); 

	static $adds = array();
	if (isset($adds[$id]) || array_key_exists($id, $adds)) {
		// Non-NULL $id and $adds[$id] statics exist.
		if (DEBUG){
			echo '<span style="color:red">address cached</span><br />';
		}
		return $adds[$id];
	}

	static $cities = array(); 
	$arrs = str_split($id, 2);

	if (!isset($cities[$arrs[0]])){
		$name = $db->GetValue("SELECT `lct_name` FROM `location_names_lct` WHERE `lct_id` = 1{$arrs[0]}0000000000000");
		echo 'province found!' . $name."1{$arrs[0]}{$arrs[1]}00000000000" . '<br />';
		$cities[$arrs[0]] = array('name' => $name, 'cities' => array());
	}

	if (!isset($cities[$arrs[0]]['cities'][$arrs[1]]) && $arrs[1] != '00'){
		$name = $db->GetValue("SELECT `lct_name` FROM `location_names_lct` WHERE `lct_id` = 1{$arrs[0]}{$arrs[1]}00000000000");
		echo 'city found!' . $name ."1{$arrs[0]}{$arrs[1]}00000000000". '<br />';
		$cities[$arrs[0]]['cities'][$arrs[1]] = array('name' => $name, 'districts' => array());
	}

	if (!isset($cities[$arrs[0]]['cities'][$arrs[1]]['districts'][$arrs[2]]) && $arrs[2] != '00'){
		$name = $db->GetValue("SELECT `lct_name` FROM `location_names_lct` WHERE `lct_id` = 1{$arrs[0]}{$arrs[1]}{$arrs[2]}000000000");
		echo 'district found!' . $name . '<br />';
		$cities[$arrs[0]]['cities'][$arrs[1]]['districts'][$arrs[2]] = $name;
	}

	$adds[$id] = array($cities[$arrs[0]]['name'], 
						$cities[$arrs[0]]['cities'][$arrs[1]]['name'],
						$cities[$arrs[0]]['cities'][$arrs[1]]['districts'][$arrs[2]]);
	return $adds[$id];
}


function getWorkType($db, $id){
	$query = "SELECT `jbwrktyp_wrktyp_id` FROM `job_work_types_jbwrktyp` WHERE `jbwrktyp_jb_id` = $id";
	return $db->GetValue($query);
}

function getJobLanguages($db, $jid){
	$query = "SELECT `jblng_lng_id` FROM `job_languages_jblng` WHERE `jblng_jb_id` = $jid";
	$lids = $db->GetItems($query);

	$str = '';
	foreach ($lids as $key => $value) {
		$str .= getLanguage($db, $value[0]) . '&middot;';
	}
	if (empty($str)){
		return 'NULL';
	}
	return substr($str, 0, -8);
}

function getLanguage($db, $lid){
	static $language = array();
	// First check if dealing with a previously defined static variable.
	if (isset($language[$lid]) || array_key_exists($lid, $language)) {
		// Non-NULL $id and $language[$lid] statics exist.
		return $language[$lid];
	}

	$query = "SELECT `lng_name` FROM `languages_lng` WHERE `lng_id` = $lid"; 
	$language[$lid] = $db->GetValue($query);
	return $language[$lid];
}



?>