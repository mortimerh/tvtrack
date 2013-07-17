#!/usr/bin/php
<?
	require('../config/config.inc');
	$filename = $argv[1];
	if(! file_exists($filename)){
		exit("Failed to open file: ".$filename . '\n');
	}

	$showlistXML = simplexml_load_file($filename);
	$db = new mysqli($config["mysql"]["host"], $config["mysql"]["tvtrack"], $config["mysql"]["upb8sK6sDrv6pdss"],
											$config["mysql"]["tvtrack"], $config["mysql"]["port"]);
	
	if($db->connect_errno >0 ){
		exit('Unable to connect to database [' . $db->connect_error . "] \n");
	}

	if( $stmt = $db->prepare('INSERT INTO shows (name, tvrageId) VALUES (?, ?) ON DUPLICATE KEY UPDATE name=?;') ) {
		$stmt->bind_param('sis', $name, $tvrageId, $name);
		$rows_inserted=0;
		foreach($showlistXML as $show) {
			$name = utf8_decode($show->name);
			$tvrageId = $show->id;
			$success = $stmt->execute();
			$rows_inserted += $stmt->affected_rows;
		}

		if($stmt->affected_rows < 0) {
			echo "Error:\n";
    	print_r($stmt->error_list);
  	}
		printf("%d row(s) inserted.\n", $rows_inserted);
		$stmt->close();
	}

	$db->close();
?>