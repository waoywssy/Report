<?php
	$bot = $_POST['bot'];

    if (isset($bot) && !empty($bot) && strlen($bot)>0)
    {

        $employee = $_POST['employee'];
        $type = $_POST['type'];
        $time = $_POST['time'];
        $date = $_POST['date'];
        $delivery = $_POST['delivery'];
        
        $issue = $_POST['issue'];

        if($employee==1){$employee='Gang Huang';}
        else if($employee==2){$employee='Yang Song';}
        else if($employee==3){$employee='Yilong Xie';}
        else if($employee==4){$employee='Wei Yin';}
        else if($employee==5){$employee='Zhihui Liu';}
        
        if($type==1){$type='Maintenance';}
        else if($type==2){$type='Enhancement';}
        else if($type==3){$type='Support';}
        else if($type==4){$type='Development';} 
        else if($type==0){$type='Routine QA';} 
        
        $sql = "select * from mj_reports where rdate = '$date' and rpt_bts_id = $bot and type = '$type' and employee = '$employee'";

		$result = db_fetch_object(db_query($sql));
		if (!$result)
		{
			$sql = "insert into mj_reports values(null, $bot, '$issue', '$date', $delivery, '$type', $time, '$employee')";
			db_query($sql);
		}
		else
		{
			echo '<span class="warning">Same bot, same working type, same reporter can be only reported once on the same day</span>';
		}
        showTodayReports($date);
    }
?>