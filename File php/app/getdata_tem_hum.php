<?php
$connect = mysqli_connect("localhost","root","","chan_nuoi_thong_minh");
mysqli_query($connect, "SET NAMES 'utf8'");

$query = "SELECT * FROM dht11";
$data = mysqli_query($connect, $query);
class User{
	function User($id, $tem, $hum, $time){
		$this->Id = $id;
		$this->Tem = $tem;
		$this->Hum = $hum;
		$this->Time = $time;
	}
}

$mangUser = array();
while($row = mysqli_fetch_assoc($data)){
	array_push($mangUser, new User($row['id'], $row['tem'], $row['hum'], $row['time']));	
}
echo json_encode($mangUser);
?>

