<?php

$dbname = 'chan_nuoi_thong_minh';
$dbuser = 'root';  
$dbpass = ''; 
$dbhost = 'localhost'; 

$connect = @mysqli_connect($dbhost,$dbuser,$dbpass,$dbname);

if(!$connect){
	exit();
}
$id = $_POST['id'];
$query = "SELECT * FROM switch WHERE id = '$id'";
$result = mysqli_query($connect,$query);

$row =  mysqli_fetch_assoc($result);
echo $row['status'];

?>
