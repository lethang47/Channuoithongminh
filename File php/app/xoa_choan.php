<?php
$connect = mysqli_connect("localhost","root","","chan_nuoi_thong_minh");
mysqli_query($connect, "SET NAMES 'utf8'");


$id = $_POST['id'];
$query = "DELETE FROM choan WHERE id = '$id'";
if (mysqli_query($connect, $query)){
	echo "success";
} else {
	echo "error";
}
?>
