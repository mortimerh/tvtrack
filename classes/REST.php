<?php
class RestUtils {
	public static function processRequest(){
		$requestMethod = strtolower($_SERVER["REQUEST_METHOD"]);
		$returnObject = new RestRequest();
		$data = array();

		// Load $data depending on request method
		switch($requestMethod){
			case 'get':
				$data = $_GET;
				break;
			case 'post':
				$data = $_POST;
				break;
			case 'delete':
				// no data accepted for delete requests
				break;
			case 'put':
				// Parse string for php special input url into $data
				parse_str( file_get_contents("php://input"), $data );
				break;
		}

		$returnObject->setMethod($requestMethod);
		$returnObject->setRequestVars($data);
		if( isset($data["data"]) ) {
			$returnObject->setData( json_decode($data["data"]) );
		}
		return $returnObject;
	}

	public static function sendResponse($status = 200, $body = "", $contentType = "text/html"){
		$statusHeader = 'HTTP/1.1 ' . $status . ' ' . RestUtils::getStatusCodeMessage($status);  
	  // set the status  
	  header($statusHeader);  
	  // set the content type  
	  header('Content-type: ' . $contentType);  

	  // pages with body are easy  
	  if($body != ''){  
      // send the body  
      echo $body;  
      exit;  
	  } else {  // we need to create the body if none is passed
      // create some body messages  
      $message = '';  

      // this is purely optional, but makes the pages a little nicer to read  
      // for your users.  Since you won't likely send a lot of different status codes,  
      // this also shouldn't be too ponderous to maintain  
      switch($status) {  
        case 401:  
          $message = 'You must be authorized to view this page.';  
          break;  
        case 404:  
          $message = 'The requested URL ' . $_SERVER['REQUEST_URI'] . ' was not found.';  
          break;  
        case 500:  
          $message = 'The server encountered an error processing your request.';  
          break;  
        case 501:  
          $message = 'The requested method is not implemented.';  
          break;  
      }  

      // servers don't always have a signature turned on (this is an apache directive "ServerSignature On")  
      $signature = ($_SERVER['SERVER_SIGNATURE'] == '') ? $_SERVER['SERVER_SOFTWARE'] . ' Server at ' . $_SERVER['SERVER_NAME'] . ' Port ' . $_SERVER['SERVER_PORT'] : $_SERVER['SERVER_SIGNATURE'];  

      // this should be templatized in a real-world solution  
      $body = '<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">  
                  <html>  
                      <head>  
                          <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">  
                          <title>' . $status . ' ' . RestUtils::getStatusCodeMessage($status) . '</title>  
                      </head>  
                      <body>  
                          <h1>' . RestUtils::getStatusCodeMessage($status) . '</h1>  
                          <p>' . $message . '</p>  
                          <hr />  
                          <address>' . $signature . '</address>  
                      </body>  
                  </html>';  

      echo $body;  
      exit;  
	  } 
	}

	public static function getStatusCodeMessage($status){
		$codes = Array(  
        100 => 'Continue',  
        101 => 'Switching Protocols',  
        200 => 'OK',  
        201 => 'Created',  
        202 => 'Accepted',  
        203 => 'Non-Authoritative Information',  
        204 => 'No Content',  
        205 => 'Reset Content',  
        206 => 'Partial Content',  
        300 => 'Multiple Choices',  
        301 => 'Moved Permanently',  
        302 => 'Found',  
        303 => 'See Other',  
        304 => 'Not Modified',  
        305 => 'Use Proxy',  
        306 => '(Unused)',  
        307 => 'Temporary Redirect',  
        400 => 'Bad Request',  
        401 => 'Unauthorized',  
        402 => 'Payment Required',  
        403 => 'Forbidden',  
        404 => 'Not Found',  
        405 => 'Method Not Allowed',  
        406 => 'Not Acceptable',  
        407 => 'Proxy Authentication Required',  
        408 => 'Request Timeout',  
        409 => 'Conflict',  
        410 => 'Gone',  
        411 => 'Length Required',  
        412 => 'Precondition Failed',  
        413 => 'Request Entity Too Large',  
        414 => 'Request-URI Too Long',  
        415 => 'Unsupported Media Type',  
        416 => 'Requested Range Not Satisfiable',  
        417 => 'Expectation Failed',  
        500 => 'Internal Server Error',  
        501 => 'Not Implemented',  
        502 => 'Bad Gateway',  
        503 => 'Service Unavailable',  
        504 => 'Gateway Timeout',  
        505 => 'HTTP Version Not Supported'  
    );  
    return (isset($codes[$status])) ? $codes[$status] : '';
	}
}

class RestRequest {
	private $requestVars;  
  private $data;  
  private $httpAccept;  
  private $method;  

  public function __construct(){  
    $this->requestVars      = array();  
    $this->data              = '';  
    $this->httpAccept       = (strpos($_SERVER['HTTP_ACCEPT'], 'json')) ? 'json' : 'xml';  
    $this->method            = 'get';  
  }  

  public function setData($data){  
    $this->data = $data;  
  }  

  public function setMethod($method){  
    $this->method = $method;  
  }  

  public function setRequestVars($requestVars){  
    $this->requestVars = $requestVars;  
  }  

  public function getData(){  
		return $this->data;  
  }  

  public function getMethod(){  
		return $this->method;  
  }  

  public function getHttpAccept(){  
		return $this->httpAccept;  
  }  

  public function getRequestVars(){  
		return $this->requestVars;  
  }  
}
?>