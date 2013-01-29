<?php
function manage_bots(){
	$op = $_POST['oper'];
	if (!empty($op))
	{
		$id = $_POST['id'];
		$priority = $_POST['priority'];
		$botname = $_POST['botname'];
		$botid = $_POST['bts_id'];
		$jobid = $_POST['jobid'];
		$qaAtBoryi = $_POST['qaAtBoryi'];
		$qaTime = $_POST['qaTime'];
		$qaFreq = $_POST['qaFreq'];
		if (empty($_POST['qaTime']))
		{
			$qaTime = 'NULL';
		}
		if (empty($_POST['jobid']))
		{
			$jobid = 'NULL';
		}
		$sector = $_POST['sector'];
		$version = $_POST['version'];
		$keywords = $_POST['keywords'];
		$keywords = str_replace("\\", "\\\\", $keywords);
		//$keyword = str_replace("\\", "\\\\", $keywords);
		if ($op == 'add') 
		{/*
			global $user;
			var_dump($user);*/
			watchdog('add', serialize($_POST));

			/*  validation here*/
			$sql = "INSERT INTO mj_bots
				(`bts_id`, `botname`, `bts_sct_id`, `qaAtBoryi`, `qaTime`, `qaTimes`, `priority`, `jobid`, `keywords`, `version`)
				VALUES($botid, '$botname', $sector, $qaAtBoryi, $qaTime, $qaFreq, $priority, '$jobid', '$keywords', '$version')";
				
			$records = db_query($sql);
			//echo mysql_insert_id();
		}
		else if ($op == 'edit') 
		{
			watchdog('edit', serialize($_POST));

			/*  validation here*/
			$sql = "UPDATE mj_bots 
					SET bts_id = $botid, botname = '$botname', bts_sct_id = $sector, qaAtBoryi = $qaAtBoryi, 
						qaTime = $qaTime, qaTimes = $qaFreq, priority = $priority, jobid = '$jobid', keywords = '$keywords', version = '$version'
					WHERE id = $id";
			//$records = 
					db_query($sql);
					//var_dump($sql);
		}
	}
	else
	{
		drupal_add_css(drupal_get_path('module', 'jquery_ui') .'/jquery.ui/themes/redmond/jquery.ui.all.css');
		drupal_add_js(drupal_get_path('module', 'reports') .'/js/stripe.js');
		drupal_add_css(drupal_get_path('module', 'reports') .'/css/reports.css');
			
		module_load_include('inc', 'reports', 'inc/reports.view');
		return view_report();
	}
}