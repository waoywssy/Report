<?php

class BoryiDB{
	private $db_host = "localhost";
	private $db_user = "root";
	private $db_pwd = "";
	//private $db_name = "boryi_nearby_jobs";
	private $db_name = "boryi_01_jobs";
	private $con;

	function __constructor($host, $user, $pwd, $db){
		$this->db_host = $host;
		$this->db_user = $user;
		$this->db_pwd = $pwd;
		$this->db_name = $name;
	}

	public function Init(){
		$this->con = mysqli_connect(
			$this->db_host, 
			$this->db_user, 
			$this->db_pwd, 
			$this->db_name);	
			// Check connection
		if (mysqli_connect_errno())
		{
			echo 'failed';
			return false;
		}
		//mysqli_autocommit($con, false); // InnoDB, not for MyISAM
		mysqli_set_charset($this->con, "UTF8");	
	}

	public function Query($sql){
		$result = mysqli_query($this->con, $sql);
		if (mysqli_errno ($this->con)){
			printf("Connect failed: %s\n", mysqli_error($this->con));	
			exit();
		}
    	
		return $result;
	}

	public function NextRow($result){
		return mysqli_fetch_array($result);
	}

	public function GetItem($sql){
		$result = $this->Query($sql);
		if (!$result){
			return NULL;
		}
		return mysqli_fetch_array($result);
	}

	public function GetItems($sql){
		$result = $this->Query($sql);
		$items = array();
		while ($row = mysqli_fetch_array($result)){
			$items[] = $row;
		}
		return $items;
	}

	public function GetValue($sql){
		$item = $this->GetItem($sql);
		return $item[0];
	}
/*
	public function Close(){
		return mysqli_close($this->con);
	}
*/
}
?>