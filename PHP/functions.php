<?php

 
/*
   Pass: instanciated database object  (in)
         tasks id to get total time from (in)
         variable to fill with decimal total time (out)
         variable to fill with timeblock_total_time * task_rate for each timeblock
         variable to fill with total hours (out)
         variable to fill with total seconds (out)
 */
function GetTaskTimeBlocksTotal($db, $taskid, &$DecimalTimeTotal, &$Tasks_Amount_Total, &$hrTotal,&$minTotal,&$secTotal) {
          
     // used in this function... 
     $total_hours   = 0;
     $total_minutes = 0;
     $total_seconds = 0; 
     $block_decimal_time = 0;
     
     // select... Timeblocks...
     if( $result = $db ->query("SELECT timeblock.starttime, timeblock.endtime, timeblock.state, task.task_rate FROM timeblock JOIN task ON task.taskid = timeblock.taskid WHERE timeblock.taskid = " . $taskid ) )
       {
			 
        // loop... total all the timeblocks for this task...  if any 
        while($row = mysqli_fetch_assoc($result)) 
             {   
                      
                // Get the difference in time for this time block (it's total time worked)
                $hours = $minutes = $seconds = $block_decimal_time = 0;
	        $date1 = new DateTime($row['starttime']);
                                                
                if($row['state'] != 'DONE')
                   $date2 = new DateTime(date("Y-m-d H:i:s"));  // real time "while working" estimates
                  else                                                    
                   $date2 = new DateTime($row['endtime']);
					  
                $interval = $date1 ->diff($date2);
				   
                // get the hours, minutes and seconds worked in this time block
		if ($interval->h) { $hours   = $interval->format("%h"); }
                if ($interval->i) { $minutes = $interval->format("%i"); }
                if ($interval->s) { $seconds = $interval->format("%s"); }                		  
                                        
                // calculate this timeblocks decimal time                
                $block_decimal_time = $hours + 
                                      (float)($minutes/60) + 
                                      (float)(($seconds/60)/60);
                
                $Tasks_Amount_Total += ($block_decimal_time * $row['task_rate']);
                $DecimalTimeTotal += $block_decimal_time;
                 
                $total_seconds += $seconds; 
                AdjustTime($total_seconds, $total_minutes);               
                
                $total_minutes += $minutes;
                AdjustTime($total_minutes, $total_hours);  
                
                $total_hours += $hours;
                
	     } // end while time blocks...
		  
        } // end timeblocks sql result 
   
 $hrTotal  = $total_hours;
 $minTotal = $total_minutes;
 $secTotal = $total_seconds;
        
}
  

function GetPaidHours($db, $clientid)
{
   $total_paid_hours = 0;
   
   if( $result = $db ->query("SELECT * FROM payment WHERE profileid = ".$clientid ) ) 
     { 
      while ($row = mysqli_fetch_assoc($result)) 
      {
         $total_paid_hours += $row['hours_paid_for']; 
      }
      
     }
   
   
   return $total_paid_hours;
}


function GetPaidAmount($db, $clientid)
{
   $total_paid = 0;
   
   if( $result = $db ->query("SELECT * FROM payment WHERE profileid = ".$clientid ) ) 
     { 
      while ($row = mysqli_fetch_assoc($result)) 
      {
         $total_paid += $row['amount']; 
      }
      
     }
   
   
   return $total_paid;
}


function GetAllProjectsTotal($db, $clientid, &$projects_total_time, &$projects_amount_total, &$totalprojectshours, &$totalprojectsminutes, &$totalprojectsseconds ) 
{

                 
 	         // get each project the client owns
	         if( $result = $db ->query("SELECT * FROM project WHERE clientid = ".$clientid ) ) 
		   { 
                         // get a row if there's a match...          
                        while ($row = mysqli_fetch_assoc($result)) // go through each task of this project
                              {  
                               // Go through and tally up all this projects times/amounts...
                               $totalprojecthours = 0;
		               $totalprojectminutes = 0;
	                       $totalprojectseconds = 0;                              
                               $project_amount_total = 0;
                               $project_total_time = 0; // each project will have a total based on all it's tasks times * each tasks rate  
                               GetProjectTasksTotal($db, $row['projectid'], $project_total_time, $project_amount_total, $totalprojecthours, $totalprojectminutes, $totalprojectseconds );
             
      
                               $projects_total_time += $project_total_time; 	       
                               $projects_amount_total += $project_amount_total;
                               
                               $totalprojectsseconds += $totalprojectseconds;
                               AdjustTime($totalprojectsseconds, $totalprojectsminutes);
                                       
		               $totalprojectsminutes += $totalprojectminutes;
                               AdjustTime($totalprojectsminutes, $totalprojectshours);
                                     
		               $totalprojectshours += $totalprojecthours;   
                               
                               } // end result while(tasks)
					
		    } // end SELECT * FROM tasks... 
          
    
}

function GetProjectTasksTotal($db, $projectid, &$project_total_Time, &$project_amount_total, &$totalprojecthours, &$totalprojectminutes, &$totalprojectseconds ) {
    
    		 $totalprojecthours = 0;
		 $totalprojectminutes = 0;
	         $totalprojectseconds = 0;                                                  
                 
                 $project_amount_total = 0;
                 $project_total_Time = 0; // each project will have a total based on all it's tasks times * each tasks rate
                 
 	         // sub select...
	         if( $result = $db ->query("SELECT * FROM task WHERE projectid = ".$projectid ) ) 
		   { 
                         // get a row if there's a match...          
                        while ($row = mysqli_fetch_assoc($result)) // go through each task of this project
                              {  
                               $total_task_hours = 0;
                               $total_task_minutes = 0;
                               $total_task_seconds = 0;
                               
	                       // total all the timeblocks for this task...  if any         
                               $hrTotal = $minTotal = $secTotal = 0;           
                               $DecimalTimeTotal = 0; 
                               $Tasks_Amount_Total = 0;
                               GetTaskTimeBlocksTotal($db, $row['taskid'], $DecimalTimeTotal, $Tasks_Amount_Total, $hrTotal, $minTotal, $secTotal);                               
                               $project_total_Time += $DecimalTimeTotal; 	       
                               $project_amount_total += $Tasks_Amount_Total;
                               
                               $total_task_seconds += $secTotal;  
                               AdjustTime($total_task_seconds, $total_task_minutes);
                                                            
                      	       $total_task_minutes += $minTotal; 
                               AdjustTime($total_task_minutes, $total_task_hours);
                                                            
                               $total_task_hours += $hrTotal;
   
                               
                               $totalprojectseconds += $total_task_seconds;
                               AdjustTime($totalprojectseconds, $totalprojectminutes);
                                       
		               $totalprojectminutes += $total_task_minutes;
                               AdjustTime($totalprojectminutes, $totalprojecthours);
                                     
		               $totalprojecthours += $total_task_hours;   
                               
                               } // end result while(tasks)
					
		    } // end SELECT * FROM tasks... 
        
}



/*
   Pass:
         First time to check for overflows > 60 (like $seconds or $minutes),
         Second time to add to when First time overflows >60  (like $minutes or $hours),
 */
function AdjustTime(&$time1, &$time2) {                    

    if($time1 >= 60) {
       $time2 += 1; 
       $time1 = ($time1 % 60);
    }
}


//  2016-06-27 22:38:42
function showAmPM($timestr)
{
  // grab the  date...
  $parts = explode(" ", $timestr );
	 
  $date = explode("-", $parts[0]);
  $time = explode(":", $parts[1]) ;
	
  list( $year, $month, $day ) = $date;
  list( $hour, $minute, $second )  = $time;
	 
  $ampm = "am";
	 
  if($hour >= 13 ) $ampm = "pm"; 
	 
  if($hour >= 13 ) 
    {		 
      $hour -= 12;
		
      if($hour == 0) $hour = 12;
    } 
   else 
    {  
      if($hour == 0) $hour = 12;
    }
	 
 if(!$hour)   $hour = "00";
 if(!$minute ) $minute  = "00";
 if(!$second ) $second  = "00";    
    
 return $hour . ":" . $minute . ":" . $second .  " ". $ampm . "&nbsp;<b>Date</b>:&nbsp;". $month . "-" . $day . "-" . $year;
}
 

// date = 12/25/2017
// time = 1:30pm
function ToTimestamp($date, $time ) {
    $date_parts = explode("/", $date);
    
    $time_parts = explode(":", $time);
    
    if(strstr($time_parts[1], "pm")) { // make millitary time
       if($time_parts[0] != "12") // don't change it if it's already 12pm 
          $time_parts[0] += 12; 
    }
   else {
    // it's am...
    if($time_parts[0] == "12") // if it's 12am make it ZERO... 
       $time_parts[0] = "00"; 
   }
    $hour = $time_parts[0];
    $minute = substr( $time_parts[1], 0, strlen($time_parts[1]) - 2);
    
    return $date_parts[2]."-".$date_parts[0]."-".$date_parts[1]." ".$hour.":".$minute.":00";
}
 

function ShowTime($starttime, $endtime, &$total_decimal_time ) {
    
 $date1 = new DateTime($starttime);
 $date2 = new DateTime($endtime);
					  
 $interval = $date1 ->diff($date2);
                                 
 $totalhours = 0;
 $totalminutes = 0;
 $totalseconds = 0;
                                 
 if ($interval->h) { $totalhours = $interval->format("%h"); }
 if ($interval->i) { $totalminutes = $interval->format("%i"); }
 if ($interval->s) { $totalseconds = $interval->format("%s"); }

 $decimal_time = $totalhours + (float)($totalminutes/60) + (float)(($totalseconds/60)/60); 
 
 $total_decimal_time = $decimal_time;
         
 AdjustTime($totalseconds, $totalminutes);		
 AdjustTime($totalminutes, $totalhours); 	

 $str = "";
 
 if(!$totalhours)   $totalhours = "00";
 if(!$totalminutes) $totalminutes = "00";
 if(!$totalseconds) $totalseconds = "00";
 
 $str = $totalhours."hrs&nbsp;".$totalminutes."mins&nbsp;".$totalseconds."secs";
 $str .= sprintf("<br/><b>Decimal time:</b>&nbsp;%.2f",$decimal_time);                     
        
 return $str;
}


?>