<?php

	/**
	 * getDir() get files and directories under current dir
	 */
	// Get files/directories list
	function getDir($dir) {
		$dirArray[] = NULL;
		if (false != ($handle = opendir($dir))) {
			$i=0;
			while (false !== ($file = readdir($handle))) {
				if ($file == '' || $file == '.' || $file == '..' 
					|| $file == 'obj' || $file == 'bin' || $file == '.svn' 
					|| strpos($file, 'pdsyn') > 0 || strpos($file, '.bk') > 0){
					continue;
				}
				$dirArray[$i++]=$file;
			}
			closedir($handle);
		}
		return $dirArray;
	}

	function scan_path($path, $keywords, $matchdir, &$scripts)
	{
		//echo 'Scanning ' . $path;
		//$keywords_array = split(';', $keywords);
		
		$dirs = getDir($path);
		$i = 0;

		foreach($dirs as $key => $val)
		{
			$i++;
			if (!empty($val)){
				// get the full path
				$dir = $path . '\\' . $val;
				$tmp = str_replace("\\", "\\\\", $dir);				
				if (is_dir($dir) ) {
					//foreach($keywords_array as $keyword_key => $keyword_val){
						//if (!empty($keyword_val)){
							if (!empty($keywords)){
							if (preg_match('/' . $keywords. '/i', $tmp)){
								// watch out, here we must pass the original $keywords 
								scan_path($dir, $keywords, 1, &$scripts);
							}
						}
					//}
				}
				else {
					//foreach($keywords_array as $keyword_key => $keyword_val){
						//if (!empty($keyword_val)){
							if (!empty($keywords)){
//							$tmp = str_replace("\\", "\\\\", $dir);
							if ($matchdir || preg_match('/' . $keywords . '/i', $tmp)){
								// if folder matches, all files under it will match
								$scripts[] = $dir;
							}
						}
					//}
				}
			}
		}
	}
	
?>