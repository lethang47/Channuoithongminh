<?php
$connect = mysqli_connect("localhost","root","","chan_nuoi_thong_minh");
mysqli_query($connect, "SET NAMES 'utf8'");

$soluong = $_POST['soluong'];
$time = $_POST['time'];

$query = "INSERT INTO hengio (soluong, time) VALUES('$soluong','$time')";
if (mysqli_query($connect, $query)){
	echo "success";
} else {
	echo "error";
}
?>
