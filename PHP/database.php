<?php

/* This class is designed so any database changes in PHP
 * (like changing from mysql to PDO or mysqli in future
 *  releases) can be delt with here. No database code changes need
 *  to be done anywhere else in the site.
 *  
 *  code by Russ Greer
 * */
class database 
{
  var $host = "localhost";
  var $databasename  = "telejobs";
  var $user = "root";
  var $password = "*******";
  var $link;

  function __construct()
  {
    $this ->connect();  // auto connect on class instance
  }


  function connect()
  {
     if( ! $this ->link = mysql_connect( $this ->host, $this ->user, $this ->password) ) 
         die('<br />Could not connect: '.$this ->error());

     $this ->selectdb( $this ->databasename );
  }


  function selectdb($dbname)
  {
    if( ! $sel = mysql_select_db($dbname, $this ->link))
        die('Can\'t use '.$dbname.' : '.$this ->error());
  }


  function query( $sql )
  {
     if( ! $result = mysql_query($sql))
         die('<br />Invalid query: ' .$sql.'    '.$this ->error());

     return $result;
  }

  
  // use:
  // $keypair = array("employerid" => 5, "jobdesc" => "another damn test..." );
  // $db ->insert( "jobs", $keypair );
  function insert( $tblname, $keypair )
  {
  	$fields = implode(array_keys($keypair), ',');
    $values = "'".implode(array_values($keypair), "','")."'";

    $sql = 'INSERT INTO `'.$tblname.'` ('.$fields.') VALUES ('.$values.')';

    $this ->query($sql) or die($this ->error());
  }

  
  // use:
  // $keypair = array("jobdesc" => "blah...");
  // $db ->update( "jobs", $keypair, "jobid", 11 );
  function update( $tblname, $keypair, $fieldname, $id )
  {  
    $fields = array_keys($keypair);
    $values = array_values($keypair);
    
    $sql = "UPDATE ".$tblname." SET ";
    
    $i=0;    
    while(@$fields[$i])
         {
           if($i<0){ $sql.=", "; }
           $sql.=$fields[$i]." = '".$values[$i]."'";
           $i++;
         }
          
    $sql.=" WHERE ".$fieldname." = ".$id." LIMIT 1;";
    
    $this ->query($sql) or die($this ->error()); 	
  }

  
  function deleterow( $tblname, $filedname, $id )
  { 
  	$this ->query("delete from $tblname where $fieldname = $id ");  	
  }

  function droptable( $tblname )
  { 
    $this ->query("drop $tblname ");  	
  }

  function dropdatabase( $dbname )
  {
  	$this ->query("drop $dbname ");  	
  }

  function error()
  {
  	mysql_error();
  }
  
  function disconnect()
  {
    mysql_close($this ->link);
  }


}


?>