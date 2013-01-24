<?php
  $op = $_REQUEST['op'];
	$bid = $_REQUEST['bid'];
	// validation here
	
	if ($op == 'scan')
	{
		scan_files($bid);
	}/*
	else if ($op == 'upload')
	{
		// get the upload package
		// unzip
		// overwrite
	}*/
	else if ($op == 'download')
	{
		$checked = split(';', $_REQUEST['checked']);
		
		zip_files($bid, $checked);
	}
	else if ($op == 'confirm')
	{
		$checked = $_REQUEST['checked'];
		$unchecked = $_REQUEST['unchecked'];
		
		$checked = split(';', $checked);
		$unchecked = split(';', $unchecked);
		
		check_files($bid, $checked, 1);
		check_files($bid, $unchecked, 0);
		
		// get script list
		// update to database
		//list_bot_related_files($bid);
		echo 'true';
	}