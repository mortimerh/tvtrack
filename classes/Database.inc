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

//----- Functions pertaining to transactions --------------------------
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

//----- Functions pertaining to `users` -------------------------------
	function addUser($username, $password, $email){
		require_once($_SERVER["DOCUMENT_ROOT"] . "/../config/config.inc");
		$userSalt = hash("sha512", openssl_random_pseudo_bytes(128) . $username );
		$siteSalt = $config["site"]["salt"];
		$hashedPassword = hash( "sha512", $userSalt . $password . $siteSalt );

		$results = $this->preparedQuery("INSERT INTO `users` (`username`, `email`, `password`, `salt`, `role`) VALUES (?, ?, ?, ?, 1)",
												 "ssss", array($username, $email, $hashedPassword, $userSalt));
		return $results;
	}
	function getUser($userId){
		$query = "SELECT `username`, `email`, `role`, `password`, `salt` FROM `users` WHERE `id` = ?";
		$results = $this->preparedQuery( $query, "i", array($userId) );
		return $results;
	}
	function deleteUser($userId){
		$query = "DELETE FROM `users` WHERE `id` = ?";
		$results = $this->preparedQuery( $query, "i", array($userId) );
		return $results;
	}

//---- Functions pertaining to `users` && `shows` ---------------------
	function addUserFollowedShow($userId, $showId){
		$query = "INSERT INTO `user_has_shows` (`users_id`, `shows_id`) VALUES (?, ?)";
		$results = $this->preparedQuery( $query, "ii", array($userId, $showId,) );
		return $results;
	}
	function getUserFollowedShows($userId){
		$query = "SELECT `shows`.* FROM `user_has_shows` ";
		$query.= "LEFT JOIN `shows` ON `user_has_shows`.`shows_id` = `shows`.`id` ";
		$query.= " WHERE `users_id` = ?";
		$results = $this->preparedQuery( $query, "i", array($userId) );
		return $results;
	}
	/* Deletes row from user_has_shows and rows from user_has_episodes where episodeId belongs to showId */
	function removeUserFolllowedShow($userId, $showId){
		$query = "DELETE `user_has_shows`, `user_has_episodes` FROM `user_has_shows`";
		$query.= "LEFT JOIN `shows` ON `shows`.`id` = `user_has_shows`.`shows_id`";
		$query.= "LEFT JOIN `seasons` ON `seasons`.`shows_id` = `shows`.`id`";
		$query.= "LEFT JOIN `episodes` ON `episodes`.`seasons_id` = `seasons`.`id`";
		$query.= "LEFT JOIN `user_has_episodes` ON `user_has_episodes`.`episodes_id`= `episodes`.`id`";
		$query.= "WHERE `user_has_shows`.`users_id` = ? AND `user_has_shows`.`shows_id` = ?";
		$this->preparedQuery( $query, "ii", array($userId, $showId) );
		return $results;
	}

//---- Functions pertaining to `users` && `episodes` -----------------
	
	/* Takes (int) $userId and (int) episodeId or (int) userId and (array) $episodeId */
	function addUserWatchedEpisodes($userId, $episodeId){
		if ( is_array($episodeId) ) {
			$userId = array_pad( array($userId), count($episodeId), $userId );
		}
		$query = "INSERT INTO `user_has_episodes` (`users_id`, `episodes_id`) VALUES (?, ?)";
		$results = $this->preparedQuery( $query, "ii", array($userId, $episodeId) );
		return $results;
	}
	
	function getUserWatchedEpisodes($userId){
		$query = "SELECT `episodes`.*, `seasons`.* FROM `user_has_episodes` ";
		$query.= "LEFT JOIN `episodes` ON `user_has_episodes`.`episodes_id` = `episodes`.`id` ";
		$query.= "LEFT JOIN `seasons` ON `episodes`.`seasons_id` = `seasons`.`id`	";
		$query.= "WHERE `user_has_episodes`.`users_id` = ?";
		$results = $this->preparedQuery( $query, "i", array($userId) );
		return $results;
	}
	function removeUserWatchedEpisodes($userId, $episodeId){
		$query = "DELETE FROM `user_has_episodes` WHERE `users_id` = ? AND `episodes_id` = ?";
		$results = $this->preparedQuery( $query, "ii", array($userId, $episodeId) );
		return $results;
	}
	
//---- Functions pertaining to `users` && `shows` && `episodes` -------
	function getUserShowLastWatchedEpisode($userId, $showId){
		$query = "SELECT `episodes`.* FROM `user_has_episodes` ";
		$query.= "LEFT JOIN `episodes` ON `episodes`.`id` = `user_has_episodes`.`episodes_id` ";
		$query.= "LEFT JOIN `seasons` ON `episodes`.`seasons_id` = `seasons`.`id`	";
		$query.= "WHERE `user_has_episodes`.`users_id` = ? AND `seasons`.`shows_id` = ? ";
		$query.= "ORDER BY `episodes`.`airdate` DESC LIMIT 1";
		$results = $this->preparedQuery( $query, "ii", array($userId, $showId) );
		return $results;
	}
	function getUserShowNextEpisode($userId, $showId){
		$lastWatchedEpisode = $this->getUserShowLastWatchedEpisode($userId, $showId);
		$query = "SELECT `episodes`.*, `seasons`.`number` AS `seasonNo` FROM `episodes` ";
		$query.= "LEFT JOIN `seasons` ON `seasons`.`id` = `episodes`.`seasons_id` ";
		$query.= "WHERE `seasons`.`shows_id` = ? AND `episodes`.`airdate` > ? ";
		$query.= "ORDER BY `episodes`.`airdate` ASC LIMIT 1";
		$results = $this->preparedQuery( $query, "is", array($showId, $lastWatchedEpisode["airdate"]) );
		return $results;
	}
	function getUserShowWatchedEpisodes($userId, $showId){
		$query = "SELECT `shows`.*, `episodes`.* FROM `shows` ";
		$query.= "LEFT JOIN `seasons` ON `seasons`.`shows_id` = `shows`.`id` ";
		$query.= "LEFT JOIN `episodes` ON `episodes`.`seasons_id` = `seasons`.`id` ";
		$query.= "LEFT JOIN `user_has_episodes` ON `user_has_episodes`.`episodes_id` = `episodes`.`id` ";
		$query.= "WHERE `user_has_episodes`.`users_id` = ? AND `shows`.`id` = ?";
		$results = $this->preparedQuery( $query, "ii", array($userId, $showId) );
		return $results;
	}

	
//---- Functions pertaining to `shows`---------------------------------
	function addShow($name, $tvdbId, $tvrageId){
		$query = "INSERT INTO `shows` (`name`, `tvdbId`, `tvrageId`) VALUES (?, ?, ?)";
		$results = $this->preparedQuery( $query, "sii", array($name, $tvdbId, $tvrageId) );
		return $results;
	}
	function getShow($showId){
		$query = "SELECT * FROM `shows` WHERE `id` = ?";
		$results = $this->preparedQuery( $query, "i", array($showId) );
		return $results;
	}
	function updateShow($showId, $name, $tvdbId, $tvrageId){
		if( $this->getShow($showId) ) {
			$query = "UPDATE `shows` SET `name` = ?, `tvdbId` = ?, `tvrageId` = ? WHERE `id` = ?";
			$results = $this->preparedQuery( $query, "siii", array($name, $tvdbId, $tvrageId, $showId) );
			return $results;
		}
		return false;
	}
	function deleteShow(){
		$query = "DELETE FROM `shows` WHERE `id` = ?";
		$results = $this->preparedQuery( $query, "i", array($userId) );
		return $results;
	}
	function getShowSeasons($showId){
		$query = "SELECT `shows`.`name` as `showName`, `seasons`.* FROM `shows` ";
		$query.= "LEFT JOIN `seasons` ON `seasons`.`shows_id` = `shows`.`id` ";
		$query.= "WHERE `shows`.`id` = ?";
		$results = $this->preparedQuery( $query, "i", array($showId) );
		return $results;
	}
	function getShowEpisodes($showId){
		$query = "SELECT `shows`.`name` as `showName`, `episodes`.* FROM `shows` ";
		$query.= "LEFT JOIN `seasons` ON `seasons`.`shows_id` = `shows`.`id` ";
		$query.= "LEFT JOIN `episodes` ON  `episodes`.`seasons_id` = `seasons`.`id` ";
		$query.= "WHERE `shows`.`id` = ? ";
		$results = $this->preparedQuery( $query, "i", array($showId) );
		return $results;
	}
	function getShowFullInfo($showId){
		$query = "SELECT `shows`.*, `shows_meta`.* FROM `shows` ";
		$query.= "LEFT JOIN `shows_meta` ON `shows_meta`.`shows_id` = `shows`.`id` ";
		$query.= "WHERE `shows`.`id` = ? ";
		$results = $this->preparedQuery( $query, "i", array($showId) );
		return $results;
	}

//---- Functions pertaining to `seasons`-------------------------------
	function addSeason($showId, $number, $name, $tvrageId, $tvdbId){
		$query = "INSERT INTO `seasons` (`shows_id`, `number`, `name`, `tvrageId`, `tvdbId`) "
		$query.= "VALUES (?, ?, ?, ?, ?)";
		$results = $this->preparedQuery( $query, "iisii", array($showId, $number, $name, $tvrageId, $tvdbId));
		return $results;
	}
	function getSeason($seasonId){
		$query = "SELECT `seasons`.* WHERE `id` = ? ";
		$results = $this->preparedQuery( $query, "i", array($seasonId) );
		return $results;
	}
	function getSeasonByShowAndNumber($showId, $seaonsNo){
		$query = "SELECT `seasons`.* WHERE `seasons`.`shows_id` = ? AND `seasons`.`number` = ?"
		$results = $this->preparedQuery( $query, "ii", array($showId, $seaonsNo) );
		return $results;
	}
	function updateSeason($seasonId, $showId, $number, $name, $tvrageId, $tvdbId){
		if ( $this->getSeason($seasonId) ){
			$query = "UPDATE `seasons` SET `shows_id` = ?, `number` = ?, `name` = ?, `tvrageId` = ?, `tvdbId` = ? ";
			$query.= "WHERE `id` = ? ";
			$results = $this->preparedQuery( $query, "iisiii", array($showId, $number, $name, $tvrageId, $tvdbId, $seasonId))
		}
		return false;
	}
	function deleteSeason($seasonId){
		$query = "DELETE FROM `seasons` WHERE `id` = ?";
		$results = $this->preparedQuery( $query, "i", array($seasonId) );
		return $results;
	}
	function getSeasonEpisodes($seasonId){
		$query = "SELECT `season`.`number` as `seasonNo`, `season`.`name` as `seasonName`, `episodes`.* ";
		$query.= "FROM `seasons` ";
		$query.= "LEFT JOIN `episodes` ON `episodes`.`seasons_id` = `seasons`.`id` ";
		$query.= "WHERE `seasons`.`id` = ?";
		$results = $this->preparedQuery( $query, "i", array($seasonId) );
		return $results;
	}

//---- Functions pertaining to `episodes`------------------------------
	function addEpisode($seasonId, $episodeNo, $name, $airdate, $tvrageId, $tvdbId){
		$query = "INSERT INTO `episodes` (`seasons_id`, `number`, `name`, `airdate`, `tvrageId`, `tvdbId`) ";
		$query.= "VALUES (?, ?, ?, ?, ?, ?)";
		$results = $this->preparedQuery( $query, "iissii", array($seasonId, $episodeNo, $name, $airdate, $tvrageId, $tvdbId) );
		return $results;
	}
	function getEpisode($episodeId){
		$query = "SELECT * FROM `episodes` WHERE `id` = ?"
		$results = $this->preparedQuery( $query, "i", array($episodeId) );
		return $results;
	}
	function updateEpisode($episodeId, $seasonId, $episodeNo, $name, $airdate, $tvrageId, $tvdbId){
		$query = "UPDATE `episodes` ";
		$query.= "SET `seasons_id` = ?, `number` = ?, `name` = ?, `airdate` = ?, `tvrageId` = ?, `tvdbId` = ? ";
		$query.= "WHERE `id` = ?";
		$results = $this->preparedQuery( $query, "iissiii", 
					array($seasonId, $episodeNo, $name, $airdate, $tvrageId, $tvdbId, $episodeId)
		);
		return $results;
	}
	function deleteEpisode($episodeId){
		$query = "DELETE FROM `episodes` WHERE `id` = ?";
		$results = $this->preparedQuery( $query, "i", array($episodeId) );
		return $results;
	}

//----- Helper function for prepared statements -----------------------
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
	    if($multiQuery){ 
	      return $result; 
	    } else { 
	    	if (count($result[0]) == 1) {
	    		return $result[0][0];
	    	}
	      return $result[0]; 
	    } 

	  } else { 
	    return FALSE; 
	  } 
	} 
}
?>