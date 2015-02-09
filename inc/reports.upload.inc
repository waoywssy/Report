<?php 
		$appendix = gmDate("Ymd");
		$targetFolder = drupal_get_path('module', 'reports') . '/uploads/' . $appendix ;// // Relative to the root
		$targetPath = $_SERVER['DOCUMENT_ROOT'] .'/drupal/'. $targetFolder;
	//	$verifyToken = md5('unique_salt' . $_POST['timestamp']);
		$verifyToken = 'c376e347a876096d1fee91a9e64b77ac';
	//	$appendix = gmDate("Ymd\THis\Z");


  if ($_REQUEST['new']==1){
		if(!(file_exists($targetPath) && is_dir($targetPath))){
			mkdir($targetPath);
		}

		$targetPath .= '/new';
		if(!(file_exists($targetPath) && is_dir($targetPath))){
			mkdir($targetPath);
		}

  	if (!empty($_FILES) //&& $_POST['token'] == $verifyToken
  		) {
		//		header("Content-Type:text/html;charset=utf8");
				$tempFile = $_FILES['Filedata']['tmp_name'];
				$filename = iconv('utf-8','gb2312',$_FILES['Filedata']['name']);
				
				$targetFile = rtrim($targetPath,'/') . '/' . $filename;

				// Validate the file type *.cs;*.regex;.sql;*.nlog;*.xml;*.bat;*.bat;
				$fileTypes = array('regex','sql'); // File extensions
				$fileParts = pathinfo($_FILES['Filedata']['name']);
				
				if (in_array(strtolower($fileParts['extension']), $fileTypes)) {
					move_uploaded_file($tempFile, $targetFile);
					
					if ($_REQUEST['type']=='sql'){
							
		    			$convertor_path = getcwd() . '\\sites\\all\\modules\\reports\\';
							// chdir('D:\\Program Files (x86)\\CodeSmith\\v6.5\\'); 

						$cmd = '"' . $convertor_path . 'files\\UTF82ISO.exe"' . ' "' . $targetFile . '" "' . $targetFile . '"';
						$cmd = str_replace('/', '\\', $cmd);
						//echo $cmd;
		    			echo shell_exec($cmd); 

							$target_str = file_get_contents($targetFile);
							preg_match_all('/create \s*table[^(\.]+\.?\s*\[?([^\(\]]+)/iu', $target_str, $out, PREG_SET_ORDER);

								//var_dump($out);

							foreach ($out as $key => $value) {
								if (strpos($out[$key][1], 'Event_')===false){

									$index = strrpos($out[$key][1], '_');
									$tmp = substr($out[$key][1],  $index + 1, strlen($out[$key][1]) - $index - 1);
									if ($tmp == 'Checklist' || $tmp == 'Static' || 
										$tmp == 'Run' || $tmp == 'Paths' || $tmp == 'Path'){
											$tmp = '';
									}
									// handling 's' ending and 'ies' ending
									if (substr($tmp, -3) == 'ies'){
											$tmp = substr($tmp, 0, -3) . 'y';
									}
									if (substr($tmp, -1) == 's'){
											$tmp = substr($tmp, 0, -1);
									}

									$tables_html .= '<tr><td><label>' . $out[$key][1] . '</label></td><td><input type="text"  value="' . $tmp . '"/>&nbsp;</td><td><input type="checkbox" ></td></tr>';
								}
							}
							echo $tables_html;
					} else {
							//echo var_dump($_FILES);
					}
					// move the file to other place that can be uploaded
				} else {
					echo 'Invalid file type.';
				}
			}
  } else {

		global $user;

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

			$targetPath .= '/' . str_replace('/', '', $botname);
			if(!(file_exists($targetPath) && is_dir($targetPath))){
				mkdir($targetPath);
			}

			if (!empty($_FILES) && $_POST['token'] == $verifyToken) {
		//		header("Content-Type:text/html;charset=utf8");
				$tempFile = $_FILES['Filedata']['tmp_name'];
				$filename = iconv('utf-8','gb2312',$_FILES['Filedata']['name']);
				
				$targetFile = rtrim($targetPath,'/') . '/' . $filename;
				
				// Validate the file type *.cs;*.regex;.sql;*.nlog;*.xml;*.bat;*.bat;
				$fileTypes = array('cs','regex','sql','nlog','xml','bat','txt', 'config'); // File extensions
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
	}