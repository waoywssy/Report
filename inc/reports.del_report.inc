<?php
	$report_id = $_POST['reportid'];
    
    if (isset($report_id) && !empty($report_id) && strlen($report_id) > 0)
    {
        $date = $_POST['date'];
        $sql = "delete from mj_reports where rpt_id = $report_id";
        db_query($sql);

        showTodayReports($date);
    }
?>