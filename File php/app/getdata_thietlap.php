<?php
$connect = mysqli_connect("localhost","root","","chan_nuoi_thong_minh");
mysqli_query($connect, "SET NAMES 'utf8'");

$query = "SELECT * FROM thietlap";
$data = mysqli_query($connect, $query);
class User{
	function User($id, $tem, $hum){
		$this->Id = $id;
		$this->Tem = $tem;
		$this->Hum = $hum;
	}
}

$mangUser = array();
while($row = mysqli_fetch_assoc($data)){
	array_push($mangUser, new User($row['id'], $row['tem'], $row['hum']));	
}
echo json_encode($mangUser);
?>

