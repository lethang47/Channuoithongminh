<?php

$dbname = 'chan_nuoi_thong_minh';
$dbuser = 'root';  
$dbpass = ''; 
$dbhost = 'localhost'; 

$connect = @mysqli_connect($dbhost,$dbuser,$dbpass,$dbname);

if(!$connect){
	exit();
}
$query = "SELECT * FROM choan order by id DESC";
$result = mysqli_query($connect,$query);
$row = mysqli_fetch_assoc($result);
echo $row['id'];

?>
