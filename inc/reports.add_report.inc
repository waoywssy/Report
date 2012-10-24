<?php
	function add_report()
	{
		$bots = '';	
		$sql = 'select bts_id, botname from mj_bots order by botname';
		$result = db_query($sql);
		while ($data = db_fetch_object($result))
		{
			$bots .= '<option value=' . $data->bts_id . '>' . $data->botname . '</option>';
		}
		
		$result = '';
		$result .= t('<p>Issue Id: <input id="issueid" type="text" />&nbsp;&nbsp;&nbsp;&nbsp;');
		$result .= t('Bot: <select id="bot">' . $bots . '</select>&nbsp;&nbsp;&nbsp;&nbsp;');
		$result .= t('Delivery: <input id="delivery" type="checkbox" />&nbsp;&nbsp;&nbsp;&nbsp;');
		$result .= t('Report Date: <input id="datepicker" type="text" /><br /><br /><br />');
		
		$result .= t('Time cost: <input id="time" type="text" style="border:1; color:#f6931f; font-weight:bold;" />&nbsp;hour(s)&nbsp;&nbsp;&nbsp;<div id="slider-range-min" style="width:260px"></div>');
		$result .= t('<br />Report Type: <select id="type"><option value="1">Maintenance</option><option value="2">Enhancement</option><option value="3">Support</option><option value="4">Development</option><option value="0">Routine QA</option></select>&nbsp;&nbsp;&nbsp;&nbsp;');
		//<option value="1">Gang Huang</option>
		$result .= t('By: <select id="employee"><option value="3">Yilong Xie</option><option value="4">Wei Yin</option><option value="5">Zhihui Liu</option><option value="2">Yang Song</option></select><br /><br /><br />');
		$result .= t('&nbsp;&nbsp;<button id="add">Add</button>&nbsp;&nbsp;<br /><br />');
		$result .= t('</p>');
		$result .= t('<p><div id="reports-grid"></div></p>');
		return $result;
	}
?>