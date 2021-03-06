<?php
	$bots_path = drupal_get_path('module', 'reports');
	$jquery_ui_path = drupal_get_path('module', 'jquery_ui');

	drupal_add_css($jquery_ui_path .'/jquery.ui/themes/redmond/jquery.ui.all.css');
	drupal_add_js($bots_path .'/js/stripe.js');
    drupal_add_css($bots_path .'/css/reports.css');

    drupal_add_css($jquery_ui_path . '/jquery.ui/themes/redmond/jquery.ui.all.css');
	drupal_add_css($jquery_ui_path . '/jquery.ui/themes/default/ui.datepicker.css');
	drupal_add_css($bots_path . '/files/jqGrid/themes/ui.jqgrid.css');
	drupal_add_css($bots_path . '/css/bots.css');
	drupal_add_css($bots_path . '/css/tree.css');
	drupal_add_css($bots_path . '/css/uploadify.css');
	jquery_ui_add(array('ui.core','ui.widget','ui.button','ui.mouse','ui.draggable','ui.position','ui.dialog','ui.tabs','ui.datepicker')); // For a single file 
	drupal_add_js($bots_path . '/files/jqGrid/js/i18n/grid.locale-en.js');
	drupal_add_js($bots_path . '/files/jqGrid/js/jquery.jqGrid.min.js');
	drupal_add_js("var imgPath = '$bots_path/images/';", 'inline');
	drupal_add_js($bots_path . '/js/botsid.js');
	drupal_add_js($bots_path . '/js/grid-bots.js');
	drupal_add_js($bots_path . '/js/jquery.contextmenu.js');
	drupal_add_js($bots_path . '/js/jquery.uploadify.min.js');
	
	function view_report()
	{
		//$ztree_path = str_replace("reports", "ztree", $bots_path);
		global $user;
/*
		drupal_add_js($ztree_path . '/js/jquery.ztree.core-3.3.js');
		drupal_add_js($ztree_path . '/js/jquery.ztree.excheck-3.3.js');
		drupal_add_js($bots_path . '/js/tree.js');
		drupal_add_css($ztree_path . '/css/zTreeStyle/zTreeStyle.css');
*/
		//drupal_add_css($bots_path . '/css/tree.css');

		$tpl_dir = dirname(dirname(__FILE__)) . '\\tpl\\';
    $html = file_get_contents($tpl_dir . 'upload_menu.tpl');
		$html = str_replace('$username', $user->name, $html);
	    return $html;
	}
	
	function get_data(){
		// get the requested type
		$type = $_REQUEST['type'];
		// get the requested page
		$page = $_REQUEST['page'];
		// get how many rows we want to have into the grid
		// rowNum parameter in the grid
		$limit = $_REQUEST['rows'];
		// get index row - i.e. user click to sort
		// at first time sortname parameter - after that the index from colModel
		$sidx = $_REQUEST['sidx'];
		// sorting order - at first time sortorder
		$sord = $_REQUEST['sord']; 
		// the criteria
		$search_field = $_REQUEST['searchField'];
		$search_str = $_REQUEST['searchString'];
		$search_oper = $_REQUEST['searchOper'];
		
		
		// if we not pass at first time index use the first column for the index
		if(!$sidx) $sidx = 1;
		
		$sql_count = "SELECT COUNT(*) FROM mj_bots";
		// calculate the number of rows for the query. We need this to paging the result
		$count = db_result(db_query($sql_count));
	
		// calculation of total pages for the query
		if($count > 0) {
			$total_pages = ceil($count/$limit);
		} else {
			$total_pages = 0;
		}
	
		// if for some reasons the requested page is greater than the total
		// set the requested page to total page
		if ($page > $total_pages) $page = $total_pages;
		// calculate the starting position of the rows
		$start = $limit * $page - $limit; // do not put $limit * ($page - 1)
		// if for some reasons start position is negative set it to 0
		// typical case is that the user type 0 for the requested page
		if($start < 0) $start = 0;
		
		$sql = "SELECT id, bts_id, botname, bot_file_name, sector, qaAtBoryi, qaTimes as qaFreq, qaTime, priority, jobid, keywords, version
			FROM
			(
				SELECT id, bts_id, botname, bot_file_name, sector, qaAtBoryi, qaTimes, qaTime, Priority, 0 AS Order1 , jobid, keywords, version
				FROM mj_bots 
				LEFT OUTER JOIN mj_sectors on bts_sct_id = sct_id
				WHERE Priority IS NOT NULL
				UNION
				SELECT id, bts_id, botname, bot_file_name, sector, qaAtBoryi, qaTimes, qaTime, Priority, 1 AS Order1 , jobid, keywords, version
				FROM mj_bots 
				LEFT OUTER JOIN mj_sectors on bts_sct_id = sct_id
				WHERE Priority IS NULL
			) AS M WHERE 1 ";
			
		if ($search_oper=='cn' && !empty($search_str)){
//			$sql .= " AND $search_field LIKE '%" . $search_str . "%' ";
//			there is a bug using 'like', part of the botname can't be matched, 
//			should be resolved later
			$sql .= " AND $search_field REGEXP '$search_str'";
		} else if ($search_oper=='eq'){
			$sql .= " AND $search_field = $search_str";
		}
		
		$sql .= " ORDER BY $sidx $sord LIMIT $start, $limit";
		
		$result = db_query($sql);

		//var_dump($sql);

		// constructing a JSON
		$responce->page = $page;
		$responce->total = $total_pages;
		$responce->records = $count;
		$responce->count = count($result);
		$i = 0;		

		while($row = db_fetch_object($result)) {
			$responce->rows[$i]['id'] = $row->id;

			$priority = '';
			if ($row->priority == 1){
				$priority = 'Most Important';
			}
			else if ($row->priority == 2){
				$priority = 'Very Important';
			}
			else if ($row->priority == 3){
				$priority = 'Important';
			}
			else if ($row->priority == 4){
				$priority = 'Less Important';
			}
			else if ($row->priority == 5){
				$priority = 'Least Important';
			}
			//"20:Daily;4:Weekly;8:Twice a Week;1:Monthly;2:Twice a Month", 

			$frequancy = array(
				'1' => 'Monthly', 
				'2' => 'Twice a Month', 
				'4' => 'Weekly', 
				'8' => 'Twice a Week', 
				'20' => 'Daily', 
				);

			$qaFreq = $frequancy[$row->qaFreq];

			$responce->rows[$i]['cell']=array(
								$row->id,
								$row->bts_id,
								$row->botname,
								$row->bot_file_name,
								$row->sector,
								$row->qaAtBoryi,
								$qaFreq,
								$row->qaTime,
								$priority,
								$row->jobid,
								$row->keywords,
								$row->version,
								);
			$i++;
			}
		return json_encode($responce);
	}
?>