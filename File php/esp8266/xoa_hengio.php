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

$id = $_GET['id'];
 
$query = "DELETE FROM hengio WHERE id = '$id'";
$result = mysqli_query($connect,$query);

echo "Insertion Success!<br>";

?>
