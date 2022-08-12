<?php
$connect = mysqli_connect("localhost","root","","chan_nuoi_thong_minh");
mysqli_query($connect, "SET NAMES 'utf8'");

$soluong = $_POST['soluong'];
$check = $_POST['check'];

$query = "INSERT INTO choan (soluong, check_) VALUES('$soluong','$check')";
if (mysqli_query($connect, $query)){
	echo "success";
} else {
	echo "error";
}
?>
