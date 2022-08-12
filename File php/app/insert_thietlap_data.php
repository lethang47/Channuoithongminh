<?php
$connect = mysqli_connect("localhost","root","","chan_nuoi_thong_minh");
mysqli_query($connect, "SET NAMES 'utf8'");


$tem = $_POST['Tem'];
$hum = $_POST['Hum'];
$id = $_POST['Id'];
$query = "UPDATE thietlap SET tem = '$tem', hum = '$hum' WHERE id = '$id'";
if (mysqli_query($connect, $query)){
	echo "success";
} else {
	echo "error";
}
?>
