<?php
function save_excel($date1, $date2)
{
    error_reporting(E_ALL);
    date_default_timezone_set('Asia/Shanghai');
    /** PHPExcel */
    $objPHPExcel = new PHPExcel();

    $objPHPExcel->getProperties()->setCreator("Yang Song")
                                 ->setLastModifiedBy("Yang Song")
                                 ->setTitle("Weekly Report of " . $date1 . " ~ " . $date2 . "")
                                 ->setSubject("Office 2007 XLSX Test Document")
                                 ->setDescription("Weekly report for Majestic Research, reported by www.boryi.com.")
                                 ->setKeywords("office 2007 majestic report")
                                 ->setCategory("Weekly Report");
	
	// Set default font/size							 
	$objPHPExcel->getDefaultStyle()->getFont()->setName('Calibri');
	$objPHPExcel->getDefaultStyle()->getFont()->setSize(10);
	
	// Set report column headers
	$objPHPExcel->setActiveSheetIndex(0)
                    ->setCellValue('A1', 'Bot ID')
                    ->setCellValue('B1', 'Issue ID')
                    ->setCellValue('C1', 'Completion Date')
                    ->setCellValue('D1', 'Delivery Date')
                    ->setCellValue('E1', 'Project')
                    ->setCellValue('F1', 'Sector')
                    ->setCellValue('G1', 'Site Change')
                    ->setCellValue('H1', 'Enhancement')
                    ->setCellValue('I1', 'Defect')
                    ->setCellValue('J1', 'Support')
                    ->setCellValue('K1', 'Development')
                    ->setCellValue('L1', 'QA')
                    ->setCellValue('M1', 'Description')
                    ->setCellValue('N1', 'Hours')
                    ->setCellValue('O1', 'Employee');
	$objPHPExcel->getActiveSheet()->getStyle('A1:O1')->applyFromArray(
	array('fill' 	=> array(
								'type'		=> PHPExcel_Style_Fill::FILL_SOLID,
								'color'		=> array('argb' => 'FFD8D8D8')
							),
		  'borders' => array(
								'allborders'	=> array('style' => PHPExcel_Style_Border::BORDER_THIN),
							)
		 )
	);
	
	$_date1 = strtotime(date($date1));
	$_date2 = strtotime(date($date2));
	
	$itemstart = 1;
	$itemend = 1;
	$current_row = 1;
	
	$qacolor_even = 'FFDBE5F1';
	$qacolor_odd = 'FFF2DDDC';

	$mcolor_even = 'FFB8CCE4';
	$mcolor_odd = 'FFE6B9B8';
	
	$dcolor_even = '0095B3D7';
	$dcolor_odd = '00D99795';
	
	$qacolor = $qacolor_odd;
	$mcolor = $mcolor_odd;
	$dcolor = $dcolor_odd;
	
	$totaldays = 0;
    $rowIndex = 1;
	
	// for each date, generate the report
	for ($d = $_date1; $d <= $_date2; $d += 86400)
	{
		$today_qa_time = 0;
		$totaldays++;
		$_today = date("Y-m-d", $d);
		
		$sql ="SELECT DISTINCT bts_id, botname, qaTime, sector, issue, date_format(rdate,\"%c/%e/%Y\") as date, delivery, type, time, employee, qatimes
		FROM 
		(
			SELECT bts_id, botname, bts_sct_id, qaTime, NULL as issue, '$_today' as rdate, 0 as delivery, 'Routine QA' as type, IFNULL(time, qaTime) as time, IFNULL(employee, 'Chunrong Lei') as employee , qatimes
			FROM (SELECT * FROM mj_bots WHERE qatimes >= %d OR bts_id in (SELECT rpt_bts_id FROM mj_reports WHERE rdate = '$_today' AND TYPE = 'Routine QA')) AS M
			LEFT OUTER JOIN (SELECT * FROM mj_reports WHERE rdate = '$_today' AND TYPE = 'Routine QA') as T
			ON rpt_bts_id = bts_id
		UNION
			SELECT bts_id, botname, bts_sct_id, qaTime, issue, rdate, delivery, type, time, employee, qatimes
			FROM (SELECT * FROM mj_reports WHERE rdate = '$_today' AND TYPE <> 'Routine QA') AS b 
			LEFT OUTER JOIN mj_bots ON bts_id = rpt_bts_id 
		) AS C
		LEFT OUTER JOIN mj_sectors on bts_sct_id = sct_id
		order by type desc, employee, qatimes desc, sector, botname";
	
		// change color for each day
		$qacolor = ($qacolor == $qacolor_odd) ? $qacolor_even : $qacolor_odd;
		$mcolor = ($mcolor == $mcolor_odd) ? $mcolor_even : $mcolor_odd;
		$dcolor = ($dcolor == $dcolor_odd) ? $dcolor_even : $dcolor_odd;
	
		$_qa = 4;	// weekly checked bots, 4 times per month
		if ($totaldays == 1)
		{
			// the first day, should get weekly qa bots
			if (date("j", $d) <= 7)
			{
				// the first week of month, should get monthly qa bots
				$_qa = 1;
			}
		}
		else if($totaldays == 4)
		{
			// the 4th day, should get weekly twice
			$_qa = 8;
		}
		else
		{
			$_qa = 20;
		}
		
		$result = db_query($sql, $_qa);
		$first_qa_row = $current_row + 1;
		$last_qa_row = $first_qa_row;
		
		while ($data = db_fetch_object($result))
		{
			$change = 'N';
			$enhancement = 'N';
			$defect = 'N';
			$support = 'N';
			$development = 'N';
			$qa = 'N';
			
			$todayQAs = array();
			if ($data->type=='Development')
			{
				$development = 'Y';
				$objPHPExcel->getActiveSheet()->getStyle('K' . ($current_row + 1 ))->getFont()->setBold(true);
				$objPHPExcel->getActiveSheet()->getStyle('K' . ($current_row + 1 ))->getFont()->getColor()->setARGB(PHPExcel_Style_Color::COLOR_RED);
			}
			else if ($data->type=='Maintenance')
			{
				$change = 'Y';	
				$support = 'Y';	
				$objPHPExcel->getActiveSheet()->getStyle('G' . ($current_row + 1 ))->getFont()->setBold(true);
				$objPHPExcel->getActiveSheet()->getStyle('J' . ($current_row + 1 ))->getFont()->setBold(true);
				$objPHPExcel->getActiveSheet()->getStyle('G' . ($current_row + 1 ))->getFont()->getColor()->setARGB(PHPExcel_Style_Color::COLOR_RED);
				$objPHPExcel->getActiveSheet()->getStyle('J' . ($current_row + 1 ))->getFont()->getColor()->setARGB(PHPExcel_Style_Color::COLOR_RED);
			}
			else if ($data->type=='Enhancement')
			{
				$enhancement = 'Y';
				$objPHPExcel->getActiveSheet()->getStyle('H' . ($current_row + 1 ))->getFont()->setBold(true);
				$objPHPExcel->getActiveSheet()->getStyle('H' . ($current_row + 1 ))->getFont()->getColor()->setARGB(PHPExcel_Style_Color::COLOR_RED);
			}
			else if ($data->type=='Support')
			{
				$support = 'Y';
				$objPHPExcel->getActiveSheet()->getStyle('J' . ($current_row + 1 ))->getFont()->setBold(true);
				$objPHPExcel->getActiveSheet()->getStyle('J' . ($current_row + 1 ))->getFont()->getColor()->setARGB(PHPExcel_Style_Color::COLOR_RED);
			}
			else if ($data->type=='Routine QA')
			{
				$qa = 'Y';
				$objPHPExcel->getActiveSheet()->getStyle('L' . ($current_row + 1 ))->getFont()->setBold(true);
				$objPHPExcel->getActiveSheet()->getStyle('L' . ($current_row + 1 ))->getFont()->getColor()->setARGB(PHPExcel_Style_Color::COLOR_RED);
				$last_qa_row++;
				
				$today_qa_time += $data->time;
				//continue;
			}
			
			$current_row++;
			$tmp_id = substr($data->bts_id, 0, 3);
			$objPHPExcel->setActiveSheetIndex(0)
						->setCellValue('A' . $current_row, $tmp_id > 0 ? $tmp_id : '') // ID
						->setCellValue('B' . $current_row, $data->issue)          // Issue ID
						->setCellValue('C' . $current_row, $data->date)           // Date
						->setCellValue('D' . $current_row, ($data->delivery == 1) ? $data->date:'')    // Delivery Date
						->setCellValue('E' . $current_row, $data->botname)        // Botname
						->setCellValue('F' . $current_row, $data->sector)         // Sector
						->setCellValue('G' . $current_row, $change)               // Change
						->setCellValue('H' . $current_row, $enhancement)          // Enhancement
						->setCellValue('I' . $current_row, $defect)               // Defect
						->setCellValue('J' . $current_row, $support)              // Support
						->setCellValue('K' . $current_row, $development)          // Development
						->setCellValue('L' . $current_row, $qa)                   // QA
						->setCellValue('M' . $current_row, $data->type)           // ReportType
						->setCellValue('N' . $current_row, $data->time)           // Hours
						->setCellValue('O' . $current_row, $data->employee);      // Employee
						
			$objPHPExcel->getActiveSheet()->getStyle('N' . $current_row)->getNumberFormat()->setFormatCode(PHPExcel_Style_NumberFormat::FORMAT_NUMBER_00);					
			// set color
			if ($data->type=='Development')
			{
				$objPHPExcel->getActiveSheet()->getStyle('A' . $current_row . ':O' . $current_row)->applyFromArray(
				array('fill' 	=> array(
											'type'		=> PHPExcel_Style_Fill::FILL_SOLID,
											'color'		=> array('argb' => $dcolor)
										),
					  'borders' => array(
											'allborders'	=> array('style' => PHPExcel_Style_Border::BORDER_THIN),
										)
					 )
				);
			}
			else if ($data->type=='Routine QA')
			{
				$objPHPExcel->getActiveSheet()->getStyle('A' . $current_row . ':O' . $current_row)->applyFromArray(
				array('fill' 	=> array(
											'type'		=> PHPExcel_Style_Fill::FILL_SOLID,
											'color'		=> array('argb' => $qacolor)
										),
					  'borders' => array(
											'allborders'	=> array('style' => PHPExcel_Style_Border::BORDER_THIN),
										)
					 )
				);
			}
			else
			{
				$objPHPExcel->getActiveSheet()->getStyle('A' . $current_row . ':O' . $current_row)->applyFromArray(
				array('fill' 	=> array(
											'type'		=> PHPExcel_Style_Fill::FILL_SOLID,
											'color'		=> array('argb' => $mcolor)
										),
					  'borders' => array(
											'allborders'	=> array('style' => PHPExcel_Style_Border::BORDER_THIN),
										)
					 )
				);
			}
		}
	
		$objPHPExcel->getActiveSheet()->duplicateStyleArray(
		array(
			'alignment' => array(
				'horizontal' => PHPExcel_Style_Alignment::HORIZONTAL_CENTER,
				),
			),
			'A1:O1'
		);
		$objPHPExcel->getActiveSheet()->duplicateStyleArray(
		array(
			'alignment' => array(
				'horizontal' => PHPExcel_Style_Alignment::HORIZONTAL_CENTER,
				),
			),
			'G2:L'. $current_row
		);
		$objPHPExcel->getActiveSheet()->duplicateStyleArray(
		array(
			'alignment' => array(
				'horizontal' => PHPExcel_Style_Alignment::HORIZONTAL_LEFT,
				),
			),
			'A2:B'. $current_row
		);
		
		// calculate the average qa time up to 8 hours
		$today_qa_time = (8 - $today_qa_time) / ($last_qa_row - $first_qa_row + 1);
		$today_qa_time = round($today_qa_time, 2);
		if ($today_qa_time > 0)
		{
			// for each qa time, add the average additional time to make up close to 8 hours
			for ($qa_row = $first_qa_row; $qa_row < $last_qa_row; $qa_row++)		
			{
				$temp = $objPHPExcel->setActiveSheetIndex(0)->getCellByColumnAndRow(13, $qa_row)->getValue();
				$temp += $today_qa_time;
				$objPHPExcel->setActiveSheetIndex(0)->setCellValue('N' . ($qa_row) , $temp);
			}
		}

		// get the total time for the current date					
		$objPHPExcel->setActiveSheetIndex(0)
						->setCellValue('P' . ($current_row) , '=SUM(N' .$itemstart. ':N' . $current_row . ')');
		// start as new day 
		$itemstart = $current_row + 1;
	}

	$objPHPExcel->getActiveSheet()->getColumnDimension('C')->setWidth(10);
	$objPHPExcel->getActiveSheet()->getColumnDimension('D')->setWidth(10);
	$objPHPExcel->getActiveSheet()->getColumnDimension('E')->setWidth(30);
	$objPHPExcel->getActiveSheet()->getColumnDimension('F')->setWidth(20);
    $objPHPExcel->getActiveSheet()->getColumnDimension('M')->setWidth(15);

	// Set active sheet name and file name, generate the file
    $objPHPExcel->getActiveSheet()->setTitle('Reports');

    // Set active sheet index to the first sheet, so Excel opens this as the first sheet
    $objPHPExcel->setActiveSheetIndex(0);
    
    $filename = 'Boryi Weekly Status Report ' . str_replace("-",  "", $date1) . ' ' . str_replace("-", "", $date2) . '.xlsx';
    
    header('Content-Type: application/excel');
    header('Content-Disposition: attachment;filename="' . $filename . '"');
    header('Cache-Control: max-age=0');

    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel2007');
    $objWriter->save('php://output');
    exit;
}
?>