<?php 
	global $user;
	$appendix = gmDate("Ymd");
	$targetFolder = drupal_get_path('module', 'reports') . '/uploads/' . $appendix ;// // Relative to the root
	$targetPath = $_SERVER['DOCUMENT_ROOT'] .'/drupal/'. $targetFolder;
//	$verifyToken = md5('unique_salt' . $_POST['timestamp']);
	$verifyToken = 'c376e347a876096d1fee91a9e64b77ac';
//	$appendix = gmDate("Ymd\THis\Z");

	$bid = $_REQUEST['botid']; // get the bot id
	$uname = $_REQUEST['uname']; // get the username
	if (is_numeric($bid) && $bid > 0){

		if(!(file_exists($targetPath) && is_dir($targetPath))){
			mkdir($targetPath);
		}

		$targetPath .= '/' . $uname;
		if(!(file_exists($targetPath) && is_dir($targetPath))){
			mkdir($targetPath);
		}

		$sql = "SELECT botname FROM `mj_bots` WHERE id = $bid";
		$botname = db_result(db_query($sql));

		$targetPath .= '/' . $botname;
		if(!(file_exists($targetPath) && is_dir($targetPath))){
			mkdir($targetPath);
		}

		if (!empty($_FILES) && $_POST['token'] == $verifyToken) {
	//		header("Content-Type:text/html;charset=utf8");
			$tempFile = $_FILES['Filedata']['tmp_name'];
			$filename = iconv('utf-8','gb2312',$_FILES['Filedata']['name']);
			
			$targetFile = rtrim($targetPath,'/') . '/' . $filename;
			
			// Validate the file type *.cs;*.regex;.sql;*.nlog;*.xml;*.bat;*.bat;
			$fileTypes = array('cs','regex','sql','nlog','xml','bat','txt'); // File extensions
			$fileParts = pathinfo($_FILES['Filedata']['name']);
			
			if (in_array(strtolower($fileParts['extension']), $fileTypes)) {
				move_uploaded_file($tempFile, $targetFile);
				echo 'success';
				// move the file to other place that can be uploaded
			} else {
				echo 'Invalid file type.';
			}
		}
	}