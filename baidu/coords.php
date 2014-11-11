<?php

class Coordinate{
	public $lat;
	public $lng;
}

class CoordinateCalculator{
	private $ak;
	private $bMapApiBase;
	private $coords; 	// the original GPS coordinates got by address

	function __construct(){
		$this->ak = 'B80e31da09924630b63f8aeb4d07218f';
		$this->bMapApiBase = 'http://api.map.baidu.com/';
		$this->coords = new Coordinate();
	}

	/*
	* The returned value should be an array of two value, $results('lng', 'lat');
	*/
	private function GetCoordinateByAddress($address, $city){
		$url = $this->bMapApiBase . 'geocoder/v2/?callback=renderOption&output=json&address='
			 		. $address . '&city=' . $city . '&ak=' . $this->ak;
		$page = $this->HttpGet($url);
		preg_match("/\"lng\"\:(\d+\.\d+)+,\"lat\"\:(\d+\.\d+)+/", $page, $results);
		$results = array_slice($results, 1);
		$this->coords->lng = $results[0];
		$this->coords->lat = $results[1];

		if (DEBUG){
			$this->coords->lng = '112.86920043186';
			$this->coords->lat = '27.886390132078';

			echo $url;	
			var_dump($this->coords);
		}
	}

	/* 
	* return an array of converted coordinates array('lng', 'lat');
	*/
	private function GetGeoConvCoordinate($lat, $lng){
		$url = $this->bMapApiBase . 'geoconv/v1/?coords='
	 		. $lng . ',' . $lat . '&from=1&to=5&ak=' . $this->ak;
 		$page = $this->HttpGet($url);
		preg_match("/\"x\"\:(\d+\.\d+)+,\"y\"\:(\d+\.\d+)+/", $page, $results);
		$results = array_slice($results, 1);
		$coords = new Coordinate();
		$coords->lng = $results[0];
		$coords->lat = $results[1];
		if (DEBUG){
			echo '转换得到的坐标:';
			var_dump($coords);
		}
		return $coords;
	}

	private function getDistance($coords1, $coords2){
		return sqrt(pow($coords1->lat - $coords2->lat, 2) + pow($coords1->lng - $coords2->lng, 2));
	}

	private function HttpGet($url){
	    $curl = curl_init();
	    curl_setopt($curl, CURLOPT_URL, $url);
	    curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
	    $content = curl_exec($curl);
	    curl_close($curl);
	    return $content;
	}

	private function Test($times){
		$coords = new Coordinate();
		$coords->lat = $this->coords->lat; // x0'
		$coords->lng = $this->coords->lng; // x0'

		$coords1 = $this->GetGeoConvCoordinate($this->coords->lat, $this->coords->lng); // x->x1

		for ($i=1; $i <= $times; $i++) { 

			$coords->lat = $coords->lat + $this->coords->lat - $coords1->lat; // x0'
			$coords->lng = $coords->lng + $this->coords->lng - $coords1->lng; // x0'
			
			$coords1 = $this->GetGeoConvCoordinate($coords->lat, $coords->lng); 	
			if (DEBUG){
				echo '第' .$i. '次距离:' . $this->getDistance($this->coords, $coords1) . '<br />';
			}
		}
		var_dump($coords);
		return $coords;
	}

	public function GetCoords($city, $addr){
		$this->GetCoordinateByAddress($addr, $city);
		$this->Test(2);
	}


	public function Run(){
		$address = array(
			/*
			array('中央电视台', '北京市'),
			array('浦东机场', '上海市'),
			array('罗湖工业园', '深圳市'),
			array('于洪区人民政府', '辽宁省'),
			array('岳麓区麓谷小学', '湖南省长沙市'),
			array('长沙火车站', '湖南省长沙市'), */
			array('雨湖区文中路体训馆', '湖南省湘潭市'),
		);

		foreach ($address as $key => $value) {
			$this->GetCoordinateByAddress($value[0], $value[1]);
			$this->Test(2);
		}
	}

	public function decodePOI($poi) {
		 $settings = array(
			 'digi'=> 16,
			 'add'=> 10,
			 'plus'=> 7,
			 'cha'=> 36,
			 'center'=> array(
				 'lat'=> 34.957995,
				 'lng'=> 107.050781,
				 'isDef'=> true
				)
		 );
		 $i = -1;
		 $h = 0;
		 $b = "";
		 $j = strlen($poi);
		 $g = ord($poi{$j-1});
		 $c = substr($poi, 0, $j-1);
		 $j--;
		 
		 for($e=0; $e<$j; $e++) {
		 $d = base_convert($c{$e}, $settings['cha'], 10)-$settings['add'];
		 if ($d>=$settings['add']) 
		 {
		 $d = $d- $settings['plus'];
		 }
		 $b .= base_convert($d, 10, $settings['cha']);
		 if ($d>$h) {
		 $i = $e;
		 $h = $d;
		 }
		 }
		 $a = intval(substr($b, 0, $i), $settings['digi']);
		 $f = intval(substr($b, $i+1), $settings['digi']);
		 $l = ($a+$f-intval($g))/2;
		 $k = ($f-$l)/100000;
		 $l /= 100000;
		 $lat = $k;
		 $lng = $l;
		 return array($lng, $lat);
	}
}
?>