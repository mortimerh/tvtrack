<?php
$config = array();

/**
	* Fill in connection info for your database below.
	*/
$config["mysql"]["host"]  = "";
$config["mysql"]["database"] = "";
$config["mysql"]["user"] = "";
$config["mysql"]["password"] = "";
$config["mysql"]["port"] = ;

/**
	*	Enter a salt for passwords here
	* I use base64_encode( openssl_random_pseudo_bytes(64) );
	*/
	$config["site"]["salt"] = "";

/**
  * Enter Client ID and Client secret for Google+ OAuth2 API here
  */
$config["google_oauth2"]["id"] = "";
$config["google_oauth2"]["secret"] = "";
?>