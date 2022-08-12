<?php
$connect = mysqli_connect("localhost","root","","chan_nuoi_thong_minh");
mysqli_query($connect, "SET NAMES 'utf8'");


$time = $_POST['time'];
$query = "DELETE FROM `hengio` WHERE time = '$time'";
if (mysqli_query($connect, $query)){
	echo "success";
} else {
	echo "error";
}
?>
