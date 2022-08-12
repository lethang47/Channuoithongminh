<?php

$dbname = 'chan_nuoi_thong_minh';
$dbuser = 'root';  
$dbpass = ''; 
$dbhost = 'localhost'; 

$connect = @mysqli_connect($dbhost,$dbuser,$dbpass,$dbname);

if(!$connect){
	echo "Error: " . mysqli_connect_error();
	exit();
}

echo "Connection Success!<br><br>";

$status = $_GET['status'];
$id = $_GET['id'];
 
$query = "UPDATE switch SET status = '$status' WHERE id = '$id'";
$result = mysqli_query($connect,$query);

echo "Insertion Success!<br>";

?>
