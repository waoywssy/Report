<?php
	drupal_add_css(drupal_get_path('module', 'jquery_ui') .'/jquery.ui/themes/redmond/jquery.ui.all.css');
    drupal_add_css(drupal_get_path('module', 'jquery_ui') .'/jquery.ui/themes/default/ui.datepicker.css');
    drupal_add_css(drupal_get_path('module', 'reports') .'/css/reports.css');
    
    jquery_ui_add(array('ui.datepicker', 'ui.widget', 'ui.button', 'ui.mouse', 'ui.slider'));
    jquery_ui_add('ui.datepicker');  // For a single file
    drupal_add_js(drupal_get_path('module', 'reports') .'/js/date.js');
    
	function add_report()
	{
		$bots = '<option value=-999>-</option>';	
		$sql = 'select bts_id, botname from mj_bots order by botname';
		$result = db_query($sql);
		while ($data = db_fetch_object($result))
		{
			$bots .= '<option value=' . $data->bts_id . '>' . $data->botname . '</option>';
		}

		$comments = '';	
		$sql = 'SELECT `cmt_id`, `cmt_comment` FROM `mj_comments` order by cmt_id';
		$result = db_query($sql);
		while ($data = db_fetch_object($result))
		{
			$comments .= '<option value=' . $data->cmt_id . '>' . $data->cmt_comment . '</option>';
		}
		
		$employee = '';	
		$sql = 'SELECT `emp_id`,`emp_name`,`emp_uid` FROM `mj_employee` ORDER BY emp_name';
		$result = db_query($sql);
		global $user;
		//var_dump($user);
		while ($data = db_fetch_object($result))
		{
			$employee .= '<option value=' . $data->emp_id;
			if ($user->uid == $data->emp_uid){
			 	$employee .= ' selected="selected"';
			}
			$employee .= ' >' . $data->emp_name . '</option>';
		}

		$result = '';
		$result .= t('<p>Issue: <input id="issueid" type="text" />&nbsp;&nbsp;&nbsp;&nbsp;');
		$result .= t('Bot: <select id="bot">' . $bots . '</select>&nbsp;&nbsp;&nbsp;&nbsp;');
		$result .= t('Delivery: <input id="delivery" type="checkbox" />&nbsp;&nbsp;&nbsp;&nbsp;');
		$result .= t('Date: <input id="datepicker" type="text" value="' . date("Y-m-d") . '" /><br /><br />');
		
		$result .= t('<div id="slider-range-min" style="width:260px;float:left;"></div><br/>Time cost: <input id="time" type="text" style="border:1; color:#f6931f; font-weight:bold;" />&nbsp;hour(s)&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;');
		$result .= t('Type: <select id="type"><option value="1">Maintenance</option><option value="2">Enhancement</option><option value="3">Support</option><option value="4">Development</option><option value="0">Routine QA</option></select>&nbsp;&nbsp;&nbsp;&nbsp;');
		$result .= t('By: <select id="employee">' . $employee . '</select><br /><br /><br />');
		$result .= t('<span id="sp_comment">Comment: <select id="comment">' . $comments . '</select><span id="sp_others"><br/><br/><textarea  id="others" name="others" cols="40" rows="5"></textarea></span></span><br /><br /><br />');
		$result .= t('&nbsp;&nbsp;<button id="add">Add</button>&nbsp;&nbsp;<br /><br />');
		$result .= t('</p>');
		$result .= t('<p><div id="reports-grid"></div></p>');
		return $result;
	}
?>