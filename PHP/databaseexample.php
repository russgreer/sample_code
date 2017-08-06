<?php

  session_start();

  require "../../classes/database.php";

  // Example of using the database class...

  $name       = $_POST['name'];
  $email        = $_POST['email'];
  $password  = $_POST['password'];
  $confirm      = $_POST['confirm'];
  $customerid = $_POST['customerid'];

  if($confirm != $password )
   {
    header("location:profile.php?s=1");
    exit();
   }

 // verify all fields are full
 if(empty($name) || empty($email) || empty($password) )
  {
    header("location:profile.php?s=2");
    exit();
  }  


 // check to see if this login/pw is already in there...
 $db = new database();


 // filter any hack attempts...
 $email=filter_var($email, FILTER_SANITIZE_EMAIL);
 if(! filter_var($email, FILTER_VALIDATE_EMAIL))
   {
    header("location:profile.php?s=4");
    exit();
   }


 // update the database
 $date = date('Y-m-d');

 $keypair = array("name" => $name,
                            "email" => $email,  
                            "password" => $password );

 $db ->update( "customer", $keypair, "customerid", $customerid );

 $_SESSION['name'] = $name;

 // re-direct them to the success and sign in page
 header("location:profile.php?s=5");
 exit();

?>