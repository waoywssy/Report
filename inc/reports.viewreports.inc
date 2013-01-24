<?php
    drupal_add_css(drupal_get_path('module', 'jquery_ui') .'/jquery.ui/themes/redmond/jquery.ui.all.css');
    drupal_add_css(drupal_get_path('module', 'jquery_ui') .'/jquery.ui/themes/default/ui.datepicker.css');
	drupal_add_js(drupal_get_path('module', 'reports') .'/js/jquery.validate.js');
    drupal_add_css(drupal_get_path('module', 'reports') .'/css/reports.css');
    
    jquery_ui_add(array('ui.datepicker', 'ui.widget', 'ui.button','ui.mouse', 'ui.slider'));
    jquery_ui_add('ui.datepicker');  // For a single file
    drupal_add_js(drupal_get_path('module', 'reports') .'/js/date.js');
    
    function view_reports(){
    	$date = $_POST['date'];
	    if (isset($date) && !empty($date) && strlen($date) > 0){
	        showTodayReports($date);
	        return;
	    }
	    
	    $result = '';
	    $result .= t('Report Date: <input id="datepicker" type="text" />');
	    $result .= t('&nbsp;&nbsp;<button id="view">View</button>&nbsp;&nbsp;<br /><br />');
	    $result .= t('</p>');
	    $result .= t('<p><div id="reports-grid"></div></p>');
	    return $result;
    }
    