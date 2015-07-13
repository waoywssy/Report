<?php
	drupal_add_css(drupal_get_path('module', 'jquery_ui') .'/jquery.ui/themes/redmond/jquery.ui.all.css');
    drupal_add_css(drupal_get_path('module', 'jquery_ui') .'/jquery.ui/themes/default/ui.datepicker.css');
    drupal_add_css(drupal_get_path('module', 'reports') .'/css/reports.css');
    
    jquery_ui_add(array('ui.datepicker', 'ui.widget', 'ui.button'));
    jquery_ui_add('ui.datepicker');  // For a single file
    drupal_add_js(drupal_get_path('module', 'reports') .'/js/jquery.validate.js');
    drupal_add_js(drupal_get_path('module', 'reports') .'/js/generate.js');

    function get_reports(){
		$output='';
		$date = '';
		$date1 = '';
		$date2 = '';

		$employee = array('Yang Song'=>'Yang', 'Wei Yin' => 'Wei', 
			'Weilong Chen' => 'Weilong', 'Jian Zhou'=> 'JianZhou');

	    $type = $_POST['type'];

		if ($type == 1){
			// weekly report 
			$date1 = $_POST['fromdate'];
	    $date2 = $_POST['todate'];

	    if ((isset($date1) && !empty($date1) && strlen($date1) > 0)
				&& isset($date2) && !empty($date2) && strlen($date2) > 0){
				module_load_include('inc', 'reports', 'inc/reports.generate');
			    require_once 'files/Classes/PHPExcel.php';
		        return save_excel($date1, $date2);
	    	}
		}
		else{
			$new = $_POST['new'];	// if asks for new tasks
			$svn = $_POST['svn'];	// if SVN failed to commit
		    $date = $_POST['date'];
		    if (!isset($date) || empty($date) || strlen($date) <= 0){
		    	$date = date('Y-m-d');
		    }

	    	$sql = "SELECT issue, delivery, type, employee, comment, botname, others FROM `mj_reports` left outer join `mj_bots` on bts_id = rpt_bts_id WHERE `rdate` = '$date' and type<> 'Routine QA'
				order by employee, delivery desc";

			$output = '<hr />';
			$output .= '<b>Boryi - a summary of today\'s work</b><br /><br />';
	    	//$output .= 'Daily repot of ' . $date . ':<br /><br />';

	    	$output .= 'Hi Henry,<br/><br/>';
	    	$output .= 'Here is a summary of today\'s work:<br/><br/>';

			$result = db_query($sql); 
			$index = 1;
			while($data = db_fetch_object($result))
			{
				$output .= $index++ . ') ';
				$output .= $employee["$data->employee"] . ' ';
				$output .= ' has ';
				
				$skip_type = false;
				if($data->delivery == 1 ){
					$output .='delivered the ';
				}
				else {
					// no delivery
					if ($data->comment=='Still working on it'){
						$output .='been working on the ';		
					}
					else if ($data->comment=='Site Changed'){
						$output .=' been working on the ';
					}
					else if ($data->type=='Development' 
							|| $data->type=='Enhancement'){
						$output .=' been working on the ';
					}
					else {
						$output .='checked the ';
						$skip_type = true;
					}
				}

				if ($data->comment=='Others'){
					$data->comment = $data->others;
				}
				if (!$skip_type){
					$output .= strtolower($data->type) . ' of ';
				}
				$output .= '<b>' . $data->botname . '</b> bot';

				if (!empty($data->issue)){
					$output .= '(issue ' . $data->issue . ')';
				}
				$output .= '.';

				if (!empty($data->comment) && $data->type != 'QA Auto Script'){
					$output .= ' ' . $data->comment . '.';
				}
				$output .= $data->delivery == 1 ? ' Please deploy.':'';
				$output .= '<br />';
			}

			$output .= '<br/>Please deploy: <br/>';
			$output .= '<b>QA SCRIPTS HERE!</b><br/><br/>';

			if (isset($svn)){
				$output .= '<br/>The SVN server just runs out of disk space again and we failed to commit todays scripts. Please check.<br/>';
			}
			if (isset($new)){
				$output .= '<br/>We have finished most of the development/maintenance. Please assign new tasks to us. Thanks. <br/>';
			}

			$output .= '<br/>Best Regards,';
			$output .= '<br/>Yang<br /><br />';
		}
		$output .= '<hr />';
		$output .= '<b>Boryi - weekly report</b><br /><br />';
		$output .= 'Hi Henry, <br /><br />';
		$output .= 'This is a weekly report of last week. Please check the attachment. <br /><br />';
		$output .= 'Best Regards,<br />';
		$output .= 'Yang<br />';

		$output = str_replace('..', '.', $output);
	    
	    
	    $result = '';
	    // weekly report from
	    $result .= t('Weekly Report:');
	    $result .= t('<form id="datesform" name="datesform" action="#" method="post" />');
	    $result .= t('From Date: <input id="fromdate" name="fromdate" type="text" />');
	    $result .= t('To Date: <input id="todate" name="todate" type="text" />');
	    $result .= t('&nbsp;&nbsp;<button id="generate" type="submit">Weekly Report</button>&nbsp;&nbsp;<br /><br />');
	    $result .= t('<input id="type" name="type" type="hidden" value="1" />');
	    $result .= t('</form>');

		// daily report from
		$result .= t('Daily Report:');
		$result .= t('<form id="dateform" name="dateform" action="#" method="post" />');
		$result .= t('Date: <input id="date" name="date" type="text" value="' . $date . '" />');
		$result .= t('&nbsp;&nbsp;<button id="getdailyreport" type="submit">Daily Report</button>&nbsp;&nbsp;<br /><br />');
		$result .= t('<input id="type" name="type" type="hidden" value="0" />');
		$result .= t('Ask for New Tasks? <input type="checkbox" id="new" name="new"/>');
		$result .= t('SVN failed? <input type="checkbox" id="svn" name="svn" />');
	    $result .= t('</form>');
		$result .= t('<br /><br />');
	    $result .= t($output);
	    return $result;
	}