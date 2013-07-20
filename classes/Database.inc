<?php

class Database{
	protected $conn;

	function __construct(){
	  require_once($_SERVER["DOCUMENT_ROOT"] . "/../config/config.inc");
		$host = $config["mysql"]["host"];
		$database = $config["mysql"]["database"];
		$user = $config["mysql"]["user"];
		$password = $config["mysql"]["password"];
		$this->conn = new mysqli($host, $user, $password, $database);
		$this->conn->set_charset("utf8");
	}

	//----- Functions pertaining to transactions ---------------
	function startTransaction(){
		$this->conn->autocommit(FALSE);
	}
	function commit(){
		$this->conn->commit();
		$this->conn->autocommit(TRUE);
	}
	function rollback(){
		$this->conn->rollback();
		$this->conn->autocommit(TRUE);
	}

	//----- Functions pertaining to `users` --------------------
	function createUser($username, $password, $email){
		require_once($_SERVER["DOCUMENT_ROOT"] . "/../config/config.inc");
		$userSalt = hash("sha512", openssl_random_pseudo_bytes(128) . $username );
		$siteSalt = $config["site"]["salt"];
		$hashedPassword = hash( "sha512", $userSalt . $password . $siteSalt );

		$results = $this->preparedQuery("INSERT INTO `users` (username, email, password, salt, role) VALUES (?, ?, ?, ?, 1)",
												 "ssss", array($username, $email, $hashedPassword, $userSalt));
		return $results[0];
	}
	function getUser($userId){
		$query = "SELECT `username`, `email`, `role`, `password`, `salt FROM `users` WHERE `id` = ?";
		$results = $this->preparedQuery( $query, "i", array($userId) );
		return $results[0];
	}
	function deleteUser($userId){
		$query = "DELETE FROM `users` WHERE `id` = ?";
		$results = $this->preparedQuery( $query, "i", array($userId) );
		return $results[0];
	}



	//----- Helper function for prepared statements -------------
	private function preparedQuery($sql, $typeDef = FALSE, $params = FALSE){ 
	  if( $stmt = $this->conn->prepare($sql) ){ 
	    if(count($params) == count($params,1)){ 
	      $params = array($params); 
	      $multiQuery = FALSE; 
	    } else { 
	      $multiQuery = TRUE; 
	    }  
	    
	    if($typeDef){ 
	      $bindParams = array();    
	      $bindParamsReferences = array(); 
	      $bindParams = array_pad($bindParams,(count($params,1)-count($params))/count($params),"");         
	      foreach($bindParams as $key => $value){ 
	        $bindParamsReferences[$key] = &$bindParams[$key];  
	      } 
	      array_unshift($bindParamsReferences,$typeDef); 
	      $bindParamsMethod = new ReflectionMethod('mysqli_stmt', 'bind_param'); 
	      $bindParamsMethod->invokeArgs($stmt,$bindParamsReferences); 
	    } 
	    
	    $result = array(); 
	    foreach($params as $queryKey => $query){ 
	      foreach($bindParams as $paramKey => $value){ 
	        $bindParams[$paramKey] = $query[$paramKey]; 
	      } 
	      $queryResult = array(); 
	      if(mysqli_stmt_execute($stmt)){ 
	        $resultMetaData = mysqli_stmt_result_metadata($stmt); 
	        if($resultMetaData){                                                                               
	          $stmtRow = array();   
	          $rowReferences = array(); 
	          while ($field = mysqli_fetch_field($resultMetaData)) { 
	            $rowReferences[] = &$stmtRow[$field->name]; 
	          }                                
	          mysqli_free_result($resultMetaData); 
	          $bindResultMethod = new ReflectionMethod('mysqli_stmt', 'bind_result'); 
	          $bindResultMethod->invokeArgs($stmt, $rowReferences); 
	          while(mysqli_stmt_fetch($stmt)){ 
	            $row = array(); 
	            foreach($stmtRow as $key => $value){ 
	              $row[$key] = $value;           
	            } 
	            $queryResult[] = $row; 
	          } 
	          mysqli_stmt_free_result($stmt); 
	        } else { 
	          $queryResult[] = mysqli_stmt_affected_rows($stmt); 
	        } 
	      } else { 
	        $queryResult[] = FALSE; 
	      } 
	      $result[$queryKey] = $queryResult; 
	    } 
	    mysqli_stmt_close($stmt);   
	  } else { 
	    $result = FALSE; 
	  } 
	  
	  if($multiQuery){ 
	    return $result; 
	  } else { 
	    return $result[0]; 
	  } 
	} 
}
?>