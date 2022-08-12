<?php
$connect = mysqli_connect("localhost","root","","chan_nuoi_thong_minh");
mysqli_query($connect, "SET NAMES 'utf8'");


$status = $_POST['status'];
$id = $_POST['id'];
$query = "UPDATE switch SET status = '$status' WHERE id = '$id'";
if (mysqli_query($connect, $query)){
	echo "success";
} else {
	echo "error";
}
?>
