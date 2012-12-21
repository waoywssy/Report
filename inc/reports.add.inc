<?php
	$bot = $_POST['bot'];

    if (isset($bot) && !empty($bot) && strlen($bot)>0)
    {

        $employee = $_POST['employee'];
        $type = $_POST['type'];
        $time = $_POST['time'];
        $date = $_POST['date'];
        $delivery = $_POST['delivery'];
        $comment = $_POST['comment'];
		$others = $_POST['others'];
        
        $issue = $_POST['issue'];
        
		$employeeArray = array('', 'Gang Huang', 'Yang Song', 'Yilong Xie', 'Wei Yin', 'Chunrong Lei', 'Weilong Chen');
		$employee = $employeeArray[$employee];
        
		$typesArray = array('Routine QA', 'Maintenance', 'Enhancement', 'Support', 'Development');
		$type = $typesArray[$type];
		

		$sql = "SELECT `cmt_comment` FROM `mj_comments` WHERE `cmt_id` = $comment";
		//var_dump($sql);
		$comment = db_result(db_query($sql));

/*
		$commentsArray = array('Site Changed', 'Data is fine now', 'Matches the site', 'Bug fixed', 'Issue Checked, no maintenance', 'Others');
		$comment = $commentsArray[$comment];
*/


        $sql = "select * from mj_reports where rdate = '$date' and rpt_bts_id = $bot and type = '$type' and employee = '$employee'";

		$result = db_fetch_object(db_query($sql));
		if (!$result)
		{
			$sql = "insert into mj_reports values(null, $bot, '$issue', '$date', $delivery, '$type', $time, '$employee', '$comment', '$others')";
			//echo $sql;
			db_query($sql);
		}
		else
		{
			echo '<span class="warning">Same bot, same working type, same reporter can be only reported once on the same day</span>';
		}
        showTodayReports($date);
    }
?>