<?php

function save_excel($date1, $date2)
{
	// set 
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

    $sql = 'select bts_id, botname, sector, issue, date_format(rdate, "%c/%e/%Y") as date, delivery, type, time, employee from mj_reports';
    $sql .= ' left outer join mj_bots on bts_id = rpt_bts_id left outer join mj_sectors on sct_id = bts_sct_id';
	$sql .= ' where rdate between \'' . $date1 . '\' and \'' . $date2 . '\'';
    $sql .= ' order by rdate, type desc, employee desc';
	
	
	$sql_qa = 'select bts_id, botname, sector, qaTime as time from mj_bots';
    $sql_qa .= ' left outer join mj_sectors on sct_id = bts_sct_id';
	$sql_qa .= ' where qaAtBoryi = 1 and qaTimes between %d and %d';
    $sql_qa .= ' order by qatimes desc, sct_id, botname';
	
	$qa_date = '';
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
	
    $i = 1;
    $result = db_query($sql);
    while ($data = db_fetch_object($result))
    {
		$change = 'N';
		$enhancement = 'N';
		$defect = 'N';
		$support = 'N';
		$development = 'N';
		$qa = 'N';
		
		// Get SUM values of each day, except the last one
		if ($i > 1 && $data->date != $qa_date)
		{
			$objPHPExcel->setActiveSheetIndex(0)
                    ->setCellValue('P' . ($i) , '=SUM(N' .$itemstart. ':N' . $i . ')');
		}
		$todayQAs = array();
		if ($data->date != $qa_date)
		{
			$qacolor = ($qacolor == $qacolor_odd) ? $qacolor_even : $qacolor_odd;
			$mcolor = ($mcolor == $mcolor_odd) ? $mcolor_even : $mcolor_odd;
			$dcolor = ($dcolor == $dcolor_odd) ? $dcolor_even : $dcolor_odd;
			
			$itemstart = $i + 1;
			
			$totaldays++;
			
			if($totaldays == 1)
			{
				$qa_result = db_query($sql_qa, 1, 5);
			}
			else if($totaldays == 4)
			{
				$qa_result = db_query($sql_qa, 2, 5);
			}
			else
			{
				$qa_result = db_query($sql_qa, 5, 5);
			}
			
			while ($qa_data = db_fetch_object($qa_result))
			{
		        $i++;
				$objPHPExcel->setActiveSheetIndex(0)
                    ->setCellValue('A' . $i, substr($qa_data->bts_id, 0, 3))         // ID
                    ->setCellValue('C' . $i, $data->date)             // Date
                    ->setCellValue('E' . $i, $qa_data->botname)        // Botname
                    ->setCellValue('F' . $i, $qa_data->sector)         // Sector
                    ->setCellValue('G' . $i, $change)                  // Change
                    ->setCellValue('H' . $i, $enhancement)             // Enhancement
                    ->setCellValue('I' . $i, $defect)                  // Defect
                    ->setCellValue('J' . $i, $support)                 // Support
                    ->setCellValue('K' . $i, $development)             // Development
                    ->setCellValue('L' . $i, 'Y')                      // QA
                    ->setCellValue('M' . $i, 'Routine QA')             // ReportType
                    ->setCellValue('N' . $i, $qa_data->time)           // Hours
                    ->setCellValue('O' . $i, 'Wei Yin');      		   // Employee
				$todayQAs[] = array($qa_data->botname => $i);
			}
			
			$itemend = $i;

			$objPHPExcel->getActiveSheet()->getStyle('A' . $itemstart . ':O' . $itemend)->applyFromArray(
				array('fill' 	=> array(
											'type'		=> PHPExcel_Style_Fill::FILL_SOLID,
											'color'		=> array('argb' => $qacolor)
										),
					  'borders' => array(
											'allborders'	=> array('style' => PHPExcel_Style_Border::BORDER_THIN),
										)
					 )
				);
			$objPHPExcel->getActiveSheet()->getStyle('L' . $itemstart . ':L' . $itemend)->getFont()->setBold(true);
			$objPHPExcel->getActiveSheet()->getStyle('L' . $itemstart . ':L' . $itemend)->getFont()->getColor()->setARGB(PHPExcel_Style_Color::COLOR_RED);
			
			$objPHPExcel->getActiveSheet()->getStyle('N' . $itemstart . ':N' . $itemend)->getNumberFormat()->setFormatCode(PHPExcel_Style_NumberFormat::FORMAT_NUMBER_00);					
				
			$qa_date = $data->date;
		}
	
		if ($data->type=='Development')
        {
            $development = 'Y';
			$objPHPExcel->getActiveSheet()->getStyle('K' . ($i + 1 ))->getFont()->setBold(true);
			$objPHPExcel->getActiveSheet()->getStyle('K' . ($i + 1 ))->getFont()->getColor()->setARGB(PHPExcel_Style_Color::COLOR_RED);
        }
		else if ($data->type=='Maintenance')
		{
			$change = 'Y';	
			$support = 'Y';	
			$objPHPExcel->getActiveSheet()->getStyle('G' . ($i + 1 ))->getFont()->setBold(true);
			$objPHPExcel->getActiveSheet()->getStyle('J' . ($i + 1 ))->getFont()->setBold(true);
			$objPHPExcel->getActiveSheet()->getStyle('G' . ($i + 1 ))->getFont()->getColor()->setARGB(PHPExcel_Style_Color::COLOR_RED);
			$objPHPExcel->getActiveSheet()->getStyle('J' . ($i + 1 ))->getFont()->getColor()->setARGB(PHPExcel_Style_Color::COLOR_RED);
		}
		else if ($data->type=='Enhancement')
		{
			$enhancement = 'Y';
			$objPHPExcel->getActiveSheet()->getStyle('H' . ($i + 1 ))->getFont()->setBold(true);
			$objPHPExcel->getActiveSheet()->getStyle('H' . ($i + 1 ))->getFont()->getColor()->setARGB(PHPExcel_Style_Color::COLOR_RED);
		}
		else if ($data->type=='Support')
		{
			$support = 'Y';
			$objPHPExcel->getActiveSheet()->getStyle('J' . ($i + 1 ))->getFont()->setBold(true);
			$objPHPExcel->getActiveSheet()->getStyle('J' . ($i + 1 ))->getFont()->getColor()->setARGB(PHPExcel_Style_Color::COLOR_RED);
		}
		else if ($data->type=='Routine QA')
		{
/*			foreach ($todayQAs as $key => $value)
			{
				if (key($value) == $qa_data->botname)
				{
					$objPHPExcel->setActiveSheetIndex(0)
			                    ->setCellValue('N' . $value[$qa_data->botname], $data->time);	
				}
			}*/
			
			$qa = 'Y';
			$objPHPExcel->getActiveSheet()->getStyle('L' . ($i + 1 ))->getFont()->setBold(true);
			$objPHPExcel->getActiveSheet()->getStyle('L' . ($i + 1 ))->getFont()->getColor()->setARGB(PHPExcel_Style_Color::COLOR_RED);
			//continue;
		}
		
        $i++;
        $objPHPExcel->setActiveSheetIndex(0)
                    ->setCellValue('A' . $i, substr($data->bts_id, 0, 3))         	// ID
                    ->setCellValue('B' . $i, $data->issue)          				// Issue ID
                    ->setCellValue('C' . $i, $data->date)          				// Date
                    ->setCellValue('D' . $i, ($data->delivery == 1) ? $data->date:'')    // Delivery Date
                    ->setCellValue('E' . $i, $data->botname)        				// Botname
                    ->setCellValue('F' . $i, $data->sector)         				// Sector
                    ->setCellValue('G' . $i, $change)               				// Change
                    ->setCellValue('H' . $i, $enhancement)          				// Enhancement
                    ->setCellValue('I' . $i, $defect)               				// Defect
                    ->setCellValue('J' . $i, $support)              				// Support
                    ->setCellValue('K' . $i, $development)          				// Development
                    ->setCellValue('L' . $i, $qa)                   				// QA
                    ->setCellValue('M' . $i, $data->type)           				// ReportType
                    ->setCellValue('N' . $i, $data->time)           				// Hours
                    ->setCellValue('O' . $i, $data->employee);      				// Employee

					
		$objPHPExcel->getActiveSheet()->getStyle('N' . $i)->getNumberFormat()->setFormatCode(PHPExcel_Style_NumberFormat::FORMAT_NUMBER_00);					
		// set color
		if ($data->type=='Development')
		{
			$objPHPExcel->getActiveSheet()->getStyle('A' . $i . ':O' . $i)->applyFromArray(
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
			$objPHPExcel->getActiveSheet()->getStyle('A' . $i . ':O' . $i)->applyFromArray(
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
			$objPHPExcel->getActiveSheet()->getStyle('A' . $i . ':O' . $i)->applyFromArray(
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
	
			
	// Get the last SUM values of each day
	if ($i > 1)
	{
		$objPHPExcel->setActiveSheetIndex(0)
				->setCellValue('P' . ($i) , '=SUM(N' .$itemstart. ':N' . $i . ')');
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
    	'G2:L'. $i
	);
	$objPHPExcel->getActiveSheet()->duplicateStyleArray(
    array(
        'alignment' => array(
            'horizontal' => PHPExcel_Style_Alignment::HORIZONTAL_LEFT,
    	    ),
	    ),
    	'A2:B'. $i
	);
	
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