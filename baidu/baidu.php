<?php

define(DEBUG, true);
define(CACHE_SIZE_COMPANY, 1800); // other static cache can also set limits like this one

require_once "db.php";
require_once "coords.php";
require_once "worker.php";

$cc = new CoordinateCalculator; 
//$cc->Run();

$today = Date('Y-m-d');
$date = Date('Y-m-d', strtotime("+15 days")); 

$db = new BoryiDB("localhost", "root", "", "boryi_01_jobs"); 
$db->Init();

$db1 = new BoryiDB("localhost", "root", "", "boryi_nearby_jobs");
$db1->Init();

$query = "SELECT `jb_cmp_id`, `jb_id`, `jb_title`, `jb_salary_low`, `jb_salary_high`, `jb_social_insurance`, 
				`jb_house_funding`, `jb_annual_vacation`, `jb_housing`, `jb_meals`, `jb_travel`, `jb_overtime`, 
				`jb_night_shift`, `jb_edu_id`, `jb_experience`, `jb_gender`, `jb_male_age_low`, `jb_male_age_high`, 
				`jb_male_height_low`, `jb_male_height_high`, `jb_female_age_low`, `jb_female_age_high`, 
				`jb_female_height_low`, `jb_female_height_high`, `jb_benefits`, `jb_requirements`, `jb_description`,
				 `jb_contacts`, `jb_email`, `jb_phone`, `jb_mobile`, `jb_qq`, `jb_first_found`, `jb_last_found` 
			FROM `jobs_jb`
			WHERE 1
			LIMIT 0, 10";//LIMIT 0, 3

$result = $db->Query($query);

$query = "INSERT INTO `boryi_nearby_jobs`.`nearby_job_info_nj` 
		(`nj_id`, 
		`nj_openid`, 
		`nj_start`, 
		`nj_end`, 
		`nj_title`, 
		`nj_type`, 
		`nj_sex`, 
		`nj_age_l`, 
		`nj_age_h`, 
		`nj_height_l`, 
		`nj_height_h`, 
		`nj_edu`, 
		`nj_exp`, 
		`nj_salary_l`, 
		`nj_salary_h`, 
		`nj_social_security`, 
		`nj_housing_fund`, 
		`nj_annual_vacations`, 
		`nj_housing`, 
		`nj_meals`, 
		`nj_travel`, 
		`nj_overtime`, 
		`nj_nightshift`, 
		`nj_requirement`, 
		`nj_description`, 
		`nj_benefit`, 
		`nj_company`, 
		`nj_wx`, 
		`nj_qq`, 
		`nj_phone`, 
		`nj_email`, 
		`nj_address`, 
		`nj_views`) VALUES";
$value_str = '';
$i = 0;
$batch = 800; // 1.55s ~ 1.70s/13k data

// apd_set_pprof_trace();
$start = microtime(TRUE);
while ($row = $db->NextRow($result))
{
	$jb_cmp_id = $row['jb_cmp_id'];
	$jb_id = $row['jb_id'];
	
	$company = getCompany($db1, $jb_cmp_id);
	$workType = getWorkType($db1, $jb_id);
	$language = getJobLanguages($db1, $jb_id);
	if(!empty($language)){
		$language = "'" . $language . "'";
	}

	$cmp_lct_id = $company['cmp_lct_id']; 

	$cmp_location = getCompanyLocation($db1, $cmp_lct_id); 

	$jb_title = $row['jb_title'];
	$jb_salary_low = $row['jb_salary_low'] == NULL ? 'NULL': $row['jb_salary_low'];
	$jb_salary_high = $row['jb_salary_high'] == NULL ? 'NULL': $row['jb_salary_high'];
	$jb_social_insurance = $row['jb_social_insurance'] == NULL ? 'NULL': $row['jb_social_insurance'];
	$jb_house_funding = $row['jb_house_funding'] == NULL ? 'NULL': $row['jb_house_funding'];
	$jb_annual_vacation = $row['jb_annual_vacation'] == NULL ? 'NULL': $row['jb_annual_vacation'];
	$jb_housing = $row['jb_housing'] == NULL ? 'NULL': $row['jb_housing'];
	$jb_meals = $row['jb_meals'] == NULL ? 'NULL': $row['jb_meals'];
	$jb_travel = $row['jb_travel'] == NULL ? 'NULL': $row['jb_travel'];
	$jb_overtime = $row['jb_overtime'] == NULL ? 'NULL': $row['jb_overtime'];
	$jb_night_shift = $row['jb_night_shift'] == NULL ? 'NULL': $row['jb_night_shift'];
	$jb_edu_id = $row['jb_edu_id'] == NULL ? 'NULL': $row['jb_edu_id'];
	$jb_experience = $row['jb_experience'] == NULL ? 'NULL': $row['jb_experience'];
	$jb_gender = $row['jb_gender'] == NULL ? 'NULL': $row['jb_gender'];
	
	$age_low = getValue($row['jb_male_age_low'], $row['jb_female_age_low']);
	$age_high = getValue($row['jb_male_age_high'], $row['jb_female_age_high'], true);
	$height_low = getValue($row['jb_male_height_low'], $row['jb_female_height_low']);
	$height_high = getValue($row['jb_male_height_high'], $row['jb_female_height_high'], true);

	$jb_benefits = $row['jb_benefits'] == NULL ? 'NULL': "'" . $row['jb_benefits'] . "'";
	$jb_requirements = $row['jb_requirements'] == NULL ? $language: "'" . $row['jb_requirements'] . "'";
	$jb_description = "'" . $row['jb_description'] . "'";
	$jb_contacts = $row['jb_contacts'] == NULL ? 'NULL': "'" . $row['jb_contacts'] . "'";
	$jb_email = $row['jb_email'] == NULL ? 'NULL': "'" . $row['jb_email'] . "'";
	$jb_phone = $row['jb_phone'] == NULL ? 'NULL': "'" . $row['jb_phone'] . "'";
	$jb_mobile = $row['jb_mobile'] == NULL ? 'NULL': "'" . $row['jb_mobile'] . "'";
	$jb_qq = $row['jb_qq'] == NULL ? 'NULL': "'" . $row['jb_qq'] . "'";
	$jb_company = $company['cmp_name'] == NULL ? 'NULL': "'" . $company['cmp_name'] . "'";
	$jb_address = $company['cmp_address'] == NULL ? 'NULL': "'" . $company['cmp_address'] . "'";

	$value_str .= "(NULL, 
						'oFIixtyZRRpCQEjyrB4ax8eM8i2A',
						'$today',
						'$date', 
						'$jb_title', 
						$workType, 
						$jb_gender,
						$age_low, 
						$age_high, 
						$height_low,
						$height_high, 
						$jb_edu_id, 
						$jb_experience, 
						$jb_salary_low, 
						$jb_salary_high, 
						$jb_social_insurance, 
						$jb_house_funding, 
						$jb_annual_vacation, 
						$jb_housing, 
						$jb_meals, 
						$jb_travel, 
						$jb_overtime, 
						$jb_night_shift, 
						$jb_requirements, 
						$jb_description, 
						$jb_benefits, 
						$jb_company, 
						NULL, 
						$jb_qq, 
						$jb_mobile, 
						$jb_email, 
						$jb_address, 
						0)";


		
/*
	$inserted_jobid = mysql_insert_id();
	$cmp_location;
	$jb_address;
	echo $cmp_location[0] . '%20' . $cmp_location[1] . '%20' . $cmp_location[2] . '<br />';
*/

	if (++$i % $batch == 0){
		$value_str .= ';';
		$db1->Query($query . $value_str);
		$value_str = '';
	} else {
		$value_str .= ',';
	}
}
if (strlen($value_str) > 1){
	$value_str = substr($value_str, 0, -1) . ';';
	$db1->Query($query . $value_str);
}

if (!DEBUG){
	echo $query . $value_str;	
}

mysqli_free_result($result);

?>
