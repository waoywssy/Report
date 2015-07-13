<?php

define('SECONDS_PER_DAY', 24 * 3600);
define('CHECKED_MONTHLY', 1);
define('CHECKED_TWICE_PER_MONTH', 2);
define('CHECKED_WEEKLY', 4);
define('CHECKED_TWICE_PER_WEEK', 8);
define('CHECKED_DAILY', 20);

define('COLOR_QA_EVEN', 'FFDBE5F1');
define('COLOR_QA_ODD', 'FFF2DDDC');

define('COLOR_MAINTENANCE_EVEN', 'FFB8CCE4');
define('COLOR_MAINTENANCE_ODD', 'FFE6B9B8');

define('COLOR_DEVELOPMENT_EVEN', '0095B3D7');
define('COLOR_DEVELOPMENT_ODD', '00D99795');

define('QA_TYPE_DEVELOPMENT', 'Development');
define('QA_TYPE_MAINTENANCE', 'Maintenance');
define('QA_TYPE_QASCRIPT', 'QA Auto Script');
define('QA_TYPE_ENHANCEMENT', 'Enhancement');
define('QA_TYPE_ROUTINEQA', 'Routine QA');

define('QAER_JIAN_ZHOU', 'Jian Zhou');
define('QAER_YANG_SONG', 'Yang Song');



function save_excel($date1, $date2){
    /** PHPExcel */
    $objPHPExcel = new PHPExcel();

	set_headers($objPHPExcel, $date1, $date2);
	set_fonts($objPHPExcel);
	
	$_date1 = strtotime(date($date1));
	$_date2 = strtotime(date($date2));
	
	$itemstart = 1;
	$itemend = 1;
	$current_row = 1;
	
	$totaldays = 0;
    $rowIndex = 1;
	
	// for each date, generate the report
	for ($d = $_date1; $d <= $_date2; $d += SECONDS_PER_DAY) {
		$today_qa_time = 0;
		$totaldays++;
		$_today = date("Y-m-d", $d);
		
		$sql ="SELECT DISTINCT bts_id, botname, qaTime, sector, issue, date_format(rdate,\"%c/%e/%Y\") as date, delivery, type, time, employee, qatimes
		FROM 
		(
			SELECT bts_id, botname, bts_sct_id, qaTime, NULL as issue, '$_today' as rdate, 0 as delivery, 'Routine QA' as type, IFNULL(time, qaTime) as time, IFNULL(employee, 'Jian Zhou') as employee , qatimes
			FROM (SELECT * FROM mj_bots WHERE (qaAtBoryi = 1 and qatimes >= %d) OR bts_id in (SELECT rpt_bts_id FROM mj_reports WHERE rdate = '$_today' AND TYPE = 'Routine QA')) AS M 
			LEFT OUTER JOIN (SELECT * FROM mj_reports WHERE rdate = '$_today' AND TYPE = 'Routine QA') as T
			ON rpt_bts_id = bts_id 
			LIMIT 0, 1
		UNION
			SELECT bts_id, botname, bts_sct_id, qaTime, issue, rdate, delivery, type, time, employee, qatimes
			FROM (SELECT * FROM mj_reports WHERE rdate = '$_today' AND TYPE <> 'Routine QA') AS b 
			LEFT OUTER JOIN mj_bots ON bts_id = rpt_bts_id 
		) AS C
		LEFT OUTER JOIN mj_sectors on bts_sct_id = sct_id
		order by type desc, employee, qatimes desc, sector, botname";
	
		// change color for each day
		$qacolor = ($qacolor == COLOR_QA_ODD) ? COLOR_QA_EVEN : COLOR_QA_ODD;
		$mcolor = ($mcolor == COLOR_MAINTENANCE_ODD) ? COLOR_MAINTENANCE_EVEN : COLOR_MAINTENANCE_ODD;
		$dcolor = ($dcolor == COLOR_DEVELOPMENT_ODD) ? COLOR_DEVELOPMENT_EVEN : COLOR_DEVELOPMENT_ODD;
	
		$_qa = CHECKED_WEEKLY;	// weekly checked bots, 4 times per month 
		if ($totaldays == 1) { // qa monthly tasks on the 2nd day
			// the first day, should get weekly qa bots
			if (date("j", $d) <= 7) {
				// the first week of month, should get monthly qa bots
				$_qa = CHECKED_MONTHLY;
			} else if (date("j", $d) > 14 && date("j", $d) <= 21){
				// the 3rd week of month, should get monthly twice qa bots
				$_qa = CHECKED_TWICE_PER_MONTH;
			}
		} else if($totaldays == 4) {
			// the 4th day, should get weekly twice
			$_qa = CHECKED_TWICE_PER_WEEK;
		} else {
			$_qa = CHECKED_DAILY;
		}
		
		$result = db_query($sql, $_qa);
		$first_qa_row = $current_row + 1;
		$last_qa_row = $first_qa_row;
	
		while ($data = db_fetch_object($result)) {
			$change = 0.00;
			$enhancement = 0.00;
			$defect = 0.00;
			$support = 0.00;
			$development = 0.00;
			$qa = 0.00;
			
			$todayQAs = array();
			
			if ($data->type== QA_TYPE_DEVELOPMENT) {
				$development = $data->time;
			} else if ($data->type == QA_TYPE_MAINTENANCE || $data->type == QA_TYPE_QASCRIPT) {
				$defect = $data->time;
			} else if ($data->type== QA_TYPE_ENHANCEMENT) {
				$enhancement = $data->time;
			} else if ($data->type== QA_TYPE_ROUTINEQA) {
				$qa = $data->time;
				$last_qa_row++;
				$today_qa_time += $data->time;
			}
			
			$current_row++;
			if ($data->bts_id > 1000 && $data->bts_id < 2000){
				$tmp_id = substr($data->bts_id, 0, 4); 	
			} else {
				$tmp_id = substr($data->bts_id, 0, 3); 	
			}
			
			$QAer = $data->employee;

			// roughly estimate the QA time for each QAer
			if ($today_qa_time > 8.10 && $QAer == QAER_JIAN_ZHOU) {
				$QAer = QAER_YANG_SONG;
			}

			if($data->qatimes >= CHECKED_DAILY || $data->type != QA_TYPE_ROUTINEQA){
				$data->qatimes = ''; // not display daily QAed bots
			}

			$objPHPExcel->setActiveSheetIndex(0) 
						->setCellValue('A' . $current_row, $tmp_id > 0 ? $tmp_id : '') 					// ID
						->setCellValue('B' . $current_row, $data->issue)          						// Issue ID
						->setCellValue('C' . $current_row, $data->date)           						// Date
						->setCellValue('D' . $current_row, ($data->delivery == 1) ? $data->date:'')     // Delivery Date
						->setCellValue('E' . $current_row, $data->botname)        						// Botname
						->setCellValue('F' . $current_row, $data->sector)         						// Sector
						->setCellValue('G' . $current_row, $development)          						// Development
						->setCellValue('H' . $current_row, $enhancement)          						// Enhancement
						->setCellValue('I' . $current_row, $defect)               						// Defect
						->setCellValue('J' . $current_row, $qa)                   						// QA
						->setCellValue('K' . $current_row, $QAer)	 			 						// Employee
						->setCellValue('L' . $current_row, $data->qatimes);      						// QATimes

			$objPHPExcel->getActiveSheet()->getStyle('G' . $current_row . ':J' . $current_row)->getNumberFormat()->setFormatCode(PHPExcel_Style_NumberFormat::FORMAT_NUMBER_00);				
			// set color
			if ($data->type==QA_TYPE_DEVELOPMENT) {
				set_color(&$objPHPExcel, $current_row, $dcolor);
			} else if ($data->type==QA_TYPE_ROUTINEQA) {
				set_color(&$objPHPExcel, $current_row, $qacolor);
			} else {
				set_color(&$objPHPExcel, $current_row, $mcolor);
			}
		}
	
		set_alignment(&$objPHPExcel, $current_row);
		
		// calculate the average qa time up to 8 hours
		$today_qa_time = (8 - $today_qa_time) / ($last_qa_row - $first_qa_row + 1);
		$today_qa_time = round($today_qa_time, 2);
		if ($today_qa_time > 0) {
			// for each qa time, add the average additional time to make up close to 8 hours
			for ($qa_row = $first_qa_row; $qa_row < $last_qa_row; $qa_row++) {
				$temp = $objPHPExcel->setActiveSheetIndex(0)->getCellByColumnAndRow(13, $qa_row)->getValue();
				$temp += $today_qa_time;
				// $objPHPExcel->setActiveSheetIndex(0)->setCellValue('N' . ($qa_row) , $temp);
			}
		}

		// get the total time for the current date 
		$objPHPExcel->setActiveSheetIndex(0)
						->setCellValue('L' . ($current_row) , '=SUM(J' .$itemstart. ':J' . $current_row . ')');
		// start as new day 
		$itemstart = $current_row + 1;
	}

	set_total($objPHPExcel, $itemstart);
	set_width($objPHPExcel);

    $filename = 'Boryi Weekly Status Report ' . str_replace("-",  "", $date1) . ' ' . str_replace("-", "", $date2) . '.xlsx';
    
    header('Content-Type: application/excel');
    header('Content-Disposition: attachment;filename="' . $filename . '"');
    header('Cache-Control: max-age=0');

    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel2007');
    $objWriter->save('php://output');
    exit;
}













function set_fonts(&$objPHPExcel){
	// Set default font/size							 
	$objPHPExcel->getDefaultStyle()->getFont()->setName('Calibri');
	$objPHPExcel->getDefaultStyle()->getFont()->setSize(10);
}

function set_color(&$objPHPExcel, $row, $color){
	$objPHPExcel->getActiveSheet()->getStyle('A' . $row . ':K' . $row)->applyFromArray(
		array('fill' 	=> array(
								'type'		=> PHPExcel_Style_Fill::FILL_SOLID,
								'color'		=> array('argb' => $color)),
		  'borders' => array(
								'allborders'	=> array('style' => PHPExcel_Style_Border::BORDER_THIN),)
		 )
	);
}


function set_alignment(&$objPHPExcel, $current_row){
	$objPHPExcel->getActiveSheet()->duplicateStyleArray(
	array(
		'alignment' => array(
			'horizontal' => PHPExcel_Style_Alignment::HORIZONTAL_CENTER,
			),
		),
		'A1:K1'
	);
	$objPHPExcel->getActiveSheet()->duplicateStyleArray(
	array(
		'alignment' => array(
			'horizontal' => PHPExcel_Style_Alignment::HORIZONTAL_LEFT,
			),
		),
		'A2:B'. $current_row
	);
}

function set_width(&$objPHPExcel){
	$objPHPExcel->getActiveSheet()->getColumnDimension('C')->setWidth(10);
	$objPHPExcel->getActiveSheet()->getColumnDimension('D')->setWidth(10);
	$objPHPExcel->getActiveSheet()->getColumnDimension('E')->setWidth(30);
	$objPHPExcel->getActiveSheet()->getColumnDimension('F')->setWidth(20);
    $objPHPExcel->getActiveSheet()->getColumnDimension('K')->setWidth(12);

	// Set active sheet name and file name, generate the file
    $objPHPExcel->getActiveSheet()->setTitle('Reports');

    // Set active sheet index to the first sheet, so Excel opens this as the first sheet
    $objPHPExcel->setActiveSheetIndex(0);
}


function set_total(&$objPHPExcel, $itemstart){
	//// the total sum part~
	$objPHPExcel->getActiveSheet()->getStyle('G' . $itemstart . ':J' . $itemstart)->applyFromArray(
		array(
			  'font' => array('bold' => true,),
			  'borders'  => array('top' => array('style' => PHPExcel_Style_Border::BORDER_THIN)),)
	);

	$objPHPExcel->setActiveSheetIndex(0)
		->setCellValue('G' . $itemstart , '=SUM(G2:G' . ($itemstart - 1) . ')');
	$objPHPExcel->setActiveSheetIndex(0)
		->setCellValue('H' . $itemstart , '=SUM(H2:H' . ($itemstart - 1) . ')');
	$objPHPExcel->setActiveSheetIndex(0)
		->setCellValue('I' . $itemstart , '=SUM(I2:I' . ($itemstart - 1) . ')');
	$objPHPExcel->setActiveSheetIndex(0)
		->setCellValue('J' . $itemstart , '=SUM(J2:J' . ($itemstart - 1) . ')');

	$objPHPExcel->getActiveSheet()->getStyle('G' . $itemstart . ':J' . $itemstart)
		->getNumberFormat()->setFormatCode(PHPExcel_Style_NumberFormat::FORMAT_NUMBER_00);
}

function set_headers(&$objPHPExcel, $date1, $date2){
	error_reporting(E_ALL);
    date_default_timezone_set('Asia/Shanghai');

	$objPHPExcel->getProperties()->setCreator("Yang Song")
                                 ->setLastModifiedBy("Yang Song")
                                 ->setTitle("Weekly Report of " . $date1 . " ~ " . $date2 . "")
                                 ->setSubject("Office 2007 XLSX Test Document")
                                 ->setDescription("Weekly report for Majestic Research, reported by www.boryi.com.")
                                 ->setKeywords("office 2007 majestic report")
                                 ->setCategory("Weekly Report");

	// Set report column headers
	$objPHPExcel->setActiveSheetIndex(0)
                    ->setCellValue('A1', 'Bot ID')
                    ->setCellValue('B1', 'OnTime ID')
                    ->setCellValue('C1', 'Date')
                    ->setCellValue('D1', 'Delivery Date')
                    ->setCellValue('E1', 'Project')
                    ->setCellValue('F1', 'Sector')
                    ->setCellValue('G1', 'Development')
                    ->setCellValue('H1', 'Enhancement')
                    ->setCellValue('I1', 'Defect')
                    ->setCellValue('J1', 'QA')
                    ->setCellValue('K1', 'Employee');

	$objPHPExcel->getActiveSheet()->setAutoFilter("A1:K1");
	$objPHPExcel->getActiveSheet()->getStyle('A1:K1')->applyFromArray(
		array('fill' 	=> array(
									'type'		=> PHPExcel_Style_Fill::FILL_SOLID,
									'color'		=> array('argb' => 'FFD8D8D8')),
			  'borders' => array(
									'allborders'	=> array('style' => PHPExcel_Style_Border::BORDER_THIN),
			 )
		));

}
?>