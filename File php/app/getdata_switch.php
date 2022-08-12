<?php
$connect = mysqli_connect("localhost","root","","chan_nuoi_thong_minh");
mysqli_query($connect, "SET NAMES 'utf8'");

$query = "SELECT * FROM switch";
$data = mysqli_query($connect, $query);
class User{
	function User($id, $status, $name){
		$this->Id = $id;
		$this->Status = $status;
		$this->Name = $name;
	}
}

$mangUser = array();
while($row = mysqli_fetch_assoc($data)){
	array_push($mangUser, new User($row['id'], $row['status'], $row['name']));	
}
echo json_encode($mangUser);
?>
