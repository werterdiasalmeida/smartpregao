<?php
require_once( APPRAIZ . 'includes/Snoopy.class.php' );
require_once( APPRAIZ . 'includes/PogProgressBar.php' );
class SnoopyBufferSocket extends Snoopy
{
	protected $boolHasProgressBarInput = false;
	
	protected $objProgressBarInput = null;
	
	protected $boolHasProgressBarOutput = false;
	
	protected $objProgressBarOutput = null;
	
	protected $strOutputType = 'string';
	
	protected $strFileName = null;
	
	protected $objFile = null;
	
	protected $intBufferPostDataLength = 128;
	
	protected $intTotalDataOutput = 0;
	
	protected $intTotalDataInput = 0;
	
	protected $intActualDataOutput = 0;
	
	protected $intActualDataInput = 0;
	
	var	$maxlength	=	50;				// max return data length (body)
	
	public function setHasProgressBarOutput( $boolHasProgressBar )
	{
		$this->boolHasProgressBarOutput = ( $boolHasProgressBar == true );
		if( $this->boolHasProgressBarOutput  )
		{
			$this->objProgressBarOutput = new PogProgressBar( 'objProgressBarOutput' );
		}
	}
	
	public function setHasProgressBarInput( $boolHasProgressBar )
	{
		$this->boolHasProgressBarInput = ( $boolHasProgressBar == true );
		if( $this->boolHasProgressBarInput  )
		{
			$this->objProgressBarInput = new PogProgressBar( 'objProgressBarInput' );
		}
	}
	
	public function changeOutputType( $strType, $strFileName = null, $boolPrepareFileNow = false, $boolCleanFile = false )
	{
		switch ( $strType )
		{
			case 'string':
			{
				$this->strOutputType = $strType;
				$this->checkFileClose();
				$this->objFile = null;
				$this->strFileName = null;
				break;
			}
			case 'file':
			{
				if ( $strFileName == null)
				{
					throw new Exception( 'To be used the output type as file is required a file name' );
				}
				else
				{
					$this->strOutputType = $strType;
					$this->strFileName = $strFileName;
				}
				
				if( $boolPrepareFileNow )
				{
					$this->checkFileWritable();
					$this->checkFileOpen( $boolCleanFile );
				}
				break;
			}
		}
	}

	public function progressBarInputDraw()
	{
		$this->objProgressBarInput->draw();
	}
	
	public function progressBarOuputDraw()
	{
		$this->objProgressBarOutput->draw();
	}
	
	protected function makeProgress()
	{
		if( $this->boolHasProgressBarInput )
		{
			$this->objProgressBarInput = new PogProgressBar( 'SnoopyBufferSocketInput' );
		}
		if( $this->boolHasProgressBarOutput )
		{
			$this->objProgressBarOutput = new PogProgressBar( 'SnoopyBufferSocketOutput' );
		}
	}
	
	protected function bufferDeal( &$strResult, $strData )
	{
		$this->intTotalDataInput += strlen( $strData );
		switch ( $this->strOutputType )
		{
			case 'string':
			{
				$strResult .= $strData;
				break;
			}
			case 'file':
			{
				$this->checkFileWritable();
				$this->checkFileOpen();
				if (!fwrite( $this->objFile , $strData ) )
				{
					throw new Exception( 'Unable to write into the file "'. $this->strFileName .'"' );
				}
				
				break;
			}
		}
		if( $this->boolHasProgressBarInput )
		{
			$intPercent = ceil( ( $this->intActualDataInput  / $this->intTotalDataInput ) * 100 );
			$this->objProgressBarInput->setProgress( $intPercent );			
		}
	}
	
	protected function checkFileOpen( $boolCleanFile = false )
	{
		if( $this->objFile == null )
		{
			if( $this->strFileName == null )
			{
				throw new Exception( 'Unable to open the file, unknow file name ');
			}

			if( $boolCleanFile )
			{
				$strOpenType = 'w';
			}
			else
			{
				$strOpenType = 'a';
			}
			
			if ( !( $this->objFile = fopen( $this->strFileName , $strOpenType ) ) )
			{
				throw new Exception( 'Unable to open the file "' . $this->strFileName . '"' );
			}		
		}
	}
	
	protected function checkFileClose()
	{
		if( $this->objFile !== null )
		{
			fclose( $this->objFile );
		}
	}
	
	protected function checkFileWritable()
	{
		if( file_exists( $this->strFileName ) and !is_writable( $this->strFileName ) )
		{
			throw new Exception( 'The file "' . $this->strFileName . '" is not writable' );
		}
	}
	
	/**
	 * Método responsável pela execução de fato da requisição http
	 * 
	 * Este metodo deve ser mantido publico por herdar de uma classe que nao faz
	 * controle de acesso e nativamente os métodos são todos feitos como públicos.
	 * 
	 * @param string $url
	 * @param object $fp
	 * @param string $URI
	 * @param string $http_method
	 * @param string $content_type
	 * @param string $body
	 * @return string
	 */
	public function _httprequest($url,$fp,$URI,$http_method,$content_type="",$body="")
	{
		$cookie_headers = '';
		if($this->passcookies && $this->_redirectaddr)
			$this->setcookies();
			
		$URI_PARTS = parse_url($URI);
		if(empty($url))
			$url = "/";
		$headers = $http_method." ".$url." ".$this->_httpversion."\r\n";		
		if(!empty($this->agent))
			$headers .= "User-Agent: ".$this->agent."\r\n";
		if(!empty($this->host) && !isset($this->rawheaders['Host'])) {
			$headers .= "Host: ".$this->host;
			if(!empty($this->port))
				$headers .= ":".$this->port;
			$headers .= "\r\n";
		}
		if(!empty($this->accept))
			$headers .= "Accept: ".$this->accept."\r\n";
		if(!empty($this->referer))
			$headers .= "Referer: ".$this->referer."\r\n";
		if(!empty($this->cookies))
		{			
			if(!is_array($this->cookies))
				$this->cookies = (array)$this->cookies;
	
			reset($this->cookies);
			if ( count($this->cookies) > 0 ) {
				$cookie_headers .= 'Cookie: ';
				foreach ( $this->cookies as $cookieKey => $cookieVal ) {
				$cookie_headers .= $cookieKey."=".urlencode($cookieVal)."; ";
				}
				$headers .= substr($cookie_headers,0,-2) . "\r\n";
			} 
		}
		if(!empty($this->rawheaders))
		{
			if(!is_array($this->rawheaders))
				$this->rawheaders = (array)$this->rawheaders;
			while(list($headerKey,$headerVal) = each($this->rawheaders))
				$headers .= $headerKey.": ".$headerVal."\r\n";
		}
		if(!empty($content_type)) {
			$headers .= "Content-type: $content_type";
			if ($content_type == "multipart/form-data")
				$headers .= "; boundary=".$this->_mime_boundary;
			$headers .= "\r\n";
		}
		if(!empty($body))	
			$headers .= "Content-length: ".strlen($body)."\r\n";
		if(!empty($this->user) || !empty($this->pass))	
			$headers .= "Authorization: Basic ".base64_encode($this->user.":".$this->pass)."\r\n";
		
		//add proxy auth headers
		if(!empty($this->proxy_user))	
			$headers .= 'Proxy-Authorization: ' . 'Basic ' . base64_encode($this->proxy_user . ':' . $this->proxy_pass)."\r\n";


		$headers .= "\r\n";
		
		// set the read timeout if needed
		if ($this->read_timeout > 0)
			socket_set_timeout($fp, $this->read_timeout);
		$this->timed_out = false;
		
		fwrite($fp,$headers.$body,strlen($headers.$body));
		
		$this->_redirectaddr = false;
		unset($this->headers);
						
		while($currentHeader = fgets($fp,$this->_maxlinelen))
		{
			if ($this->read_timeout > 0 && $this->_check_timeout($fp))
			{
				$this->status=-100;
				return false;
			}
				
			if($currentHeader == "\r\n")
				break;
						
			// if a header begins with Location: or URI:, set the redirect
			if(preg_match("/^(Location:|URI:)/i",$currentHeader))
			{
				// get URL portion of the redirect
				preg_match("/^(Location:|URI:)[ ]+(.*)/i",chop($currentHeader),$matches);
				// look for :// in the Location header to see if hostname is included
				if(!preg_match("|\:\/\/|",$matches[2]))
				{
					// no host in the path, so prepend
					$this->_redirectaddr = $URI_PARTS["scheme"]."://".$this->host.":".$this->port;
					// eliminate double slash
					if(!preg_match("|^/|",$matches[2]))
							$this->_redirectaddr .= "/".$matches[2];
					else
							$this->_redirectaddr .= $matches[2];
				}
				else
					$this->_redirectaddr = $matches[2];
			}
		
			if(preg_match("|^HTTP/|",$currentHeader))
			{
                if(preg_match("|^HTTP/[^\s]*\s(.*?)\s|",$currentHeader, $status))
				{
					$this->status= $status[1];
                }				
				$this->response_code = $currentHeader;
			}
				
			$this->headers[] = $currentHeader;
		}

		
		$intFileSize = strlen($body);
		$this->intTotalDataInput = $intFileSize;
		$results = '';
		do {
    		$_data = fread($fp, $this->maxlength);
    		if (strlen($_data) == 0) {
        		break;
    		}
    		$this->bufferDeal( $results , $_data );
		} while(true);

		if ($this->read_timeout > 0 && $this->_check_timeout($fp))
		{
			$this->status=-100;
			return false;
		}
		
		// check if there is a a redirect meta tag
		
		if(preg_match("'<meta[\s]*http-equiv[^>]*?content[\s]*=[\s]*[\"\']?\d+;[\s]*URL[\s]*=[\s]*([^\"\']*?)[\"\']?>'i",$results,$match))

		{
			$this->_redirectaddr = $this->_expandlinks($match[1],$URI);	
		}

		// have we hit our frame depth and is there frame src to fetch?
		if(($this->_framedepth < $this->maxframes) && preg_match_all("'<frame\s+.*src[\s]*=[\'\"]?([^\'\"\>]+)'i",$results,$match))
		{
			$this->results[] = $results;
			for($x=0; $x<count($match[1]); $x++)
				$this->_frameurls[] = $this->_expandlinks($match[1][$x],$URI_PARTS["scheme"]."://".$this->host);
		}
		// have we already fetched framed content?
		elseif(is_array($this->results))
			$this->results[] = $results;
		// no framed content
		else
			$this->results = $results;
		
		if( $this->boolHasProgressBarInput )
		{
			$this->objProgressBarInput->setProgress( 100 );
		}
		if( $this->boolHasProgressBarOutput )
		{
			$this->objProgressBarOutput->setProgress( 100 );
		}
		return true;
	}
	
	public function sendFilesAsPostAttributes( $strUrl , $arrRegularAttributes , $arrPostDataFiles , $arrFormFiles = array() )
	{
		$strPostData = '';
		switch ($this->_submit_type) {
			
			// formato para quando se envia apenas variaveis e nao arquivos //
			case "application/x-www-form-urlencoded":
			{	
				reset( $arrRegularAttributes );
				foreach( $arrRegularAttributes as $strAttributeName => &$mixAttribute )
				{
					if ( is_array( $mixAttribute ) || is_object( $mixAttribute ) )
					{
						foreach( $mixAttribute as $strPropKey => $strPropValue )
						{
							$strPostData .= urlencode( $strAttributeName ) . "[]=" . urlencode( $strPropValue ) . "&" ;
						}
					}
					else
					{
						$strPostData .= urlencode( $strAttributeName ) . "=" . urlencode( $mixAttribute ) . "&";
					}
				}
				break;
			}
			// formato para quando se envia variaveis e arquivos //
			case "multipart/form-data":
			{
				/**
				 * @todo concluir este metodo
				 */
				$this->_mime_boundary = "Snoopy" . md5( uniqid( microtime() ) );
				
				reset( $arrRegularAttributes );
				foreach( $arrRegularAttributes as $strAttributeName => &$mixAttribute )
				{
					if ( is_array( $mixAttribute ) || is_object( $mixAttribute ) )
					{
						foreach( $mixAttribute as $strPropKey => $strPropValue )
						{
							
							$strPostData .= "--" . $this->_mime_boundary . "\r\n";
							$strPostData .= "Content-Disposition: form-data; name=\"" . $strAttributeName . "\[\]\"\r\n\r\n";
							$strPostData .= $strPropKey . "\r\n";
						}
					}
					else
					{
						$strPostData .= "--" . $this->_mime_boundary."\r\n";
						$strPostData .= "Content-Disposition: form-data; name=\"" . $strAttributeName . "\"\r\n\r\n";
						$strPostData .= $mixAttribute . "\r\n";
						
						$strPostData .= urlencode( $strAttributeName ) . "=" . urlencode( $mixAttribute ) . "&";
					}
				}
				
				foreach( $arrFormFiles as $strAttributeName => &$strFileName )
				{
					
				}
				reset($formfiles);
				while (list($field_name, $file_names) = each( $arrFormFiles ) ) {
					settype($file_names, "array");
					while (list(, $file_name) = each($file_names)) {
						if (!is_readable($file_name)) continue;

						$fp = fopen($file_name, "r");
						$file_content = fread($fp, filesize($file_name));
						fclose($fp);
						$base_name = basename($file_name);

						$postdata .= "--".$this->_mime_boundary."\r\n";
						$postdata .= "Content-Disposition: form-data; name=\"$field_name\"; filename=\"$base_name\"\r\n\r\n";
						$postdata .= "$file_content\r\n";
					}
				}
				$postdata .= "--".$this->_mime_boundary."--\r\n";
				break;
			}
			default:
			{
				/**
				 * @todo Implementar para aceitar tb arquivos nesta submissão
				 */
				throw new Exception( 'Método de submissão não implementado.' );
				break;
			}
		}
		
		$intPostDataLength = strlen( $strPostData );
		
		$arrUrlParts = parse_url( $strUrl );
		if ( !empty( $arrUrlParts[ "user" ] ) )
		{
			$this->user = $arrUrlParts[ "user" ];
		}
		if ( !empty($arrUrlParts[ "pass" ] ) )
		{
			$this->pass = $arrUrlParts[ "pass" ];
		}
		if ( empty( $arrUrlParts[ "query" ] ) )
		{
			$arrUrlParts[ "query" ] = '';
		}
		if ( empty( $arrUrlParts[ "path" ] ) )
		{
			$arrUrlParts[ "path" ] = '';
		}

		switch( strtolower( $arrUrlParts[ "scheme" ] ) )
		{
			case "http":
			{	
				$this->host = $arrUrlParts[ "host" ];
				if( !empty( $arrUrlParts[ "port" ] ) )
				{
					$this->port = $arrUrlParts[ "port" ];
				}
				if($this->_connect($fp))
				{
					if($this->_isproxy)
					{
						// using proxy, send entire URI
						$this->httpRequestFilesAsPostAttributes
						(
							$strUrl					, 
							$fp						, 
							$strUrl					,
							$this->_submit_method	, 
							$this->_submit_type		, 
							$strPostData			, 
							$arrPostDataFiles 
						);
					}
					else
					{
						$strPath = $arrUrlParts[ "path" ] . 
							( $arrUrlParts[ "query" ] ? "?" . $arrUrlParts[ "query" ] : "" );
						// no proxy, send only the path
						$this->httpRequestFilesAsPostAttributes
						(
							$strPath				,
							$fp						,
							$strUrl					,
							$this->_submit_method	,
							$this->_submit_type		, 
							$strPostData			, 
							$arrPostDataFiles 
						);
					}
					
					$this->_disconnect($fp);

					if($this->_redirectaddr)
					{
						/* url was redirected, check if we've hit the max depth */
						if( $this->maxredirs > $this->_redirectdepth )
						{						
							if( !preg_match( "|^" . $arrUrlParts["scheme"] . "://|" , $this->_redirectaddr ) )
							{
								$this->_redirectaddr = $this->_expandlinks
								(
									$this->_redirectaddr ,
									$arrUrlParts[ "scheme" ] . "://" . $arrUrlParts[ "host" ]
								);
							}
							
							// only follow redirect if it's on this site, or offsiteok is true
							if( preg_match( "|^http://" . preg_quote( $this->host ) . "|i" , $this->_redirectaddr ) || $this->offsiteok )
							{
								/* follow the redirect */
								$this->_redirectdepth++;
								$this->lastredirectaddr = $this->_redirectaddr;
								if( strpos( $this->_redirectaddr, "?" ) > 0 )
									$this->fetch( $this->_redirectaddr ); // the redirect has changed the request method from post to get
								else
									$this->submit($this->_redirectaddr, $arrRegularAttributes , $arrFormFiles );
							}
						}
					}

					if($this->_framedepth < $this->maxframes && count($this->_frameurls) > 0)
					{
						$frameurls = $this->_frameurls;
						$this->_frameurls = array();
						
						while(list(,$frameurl) = each($frameurls))
						{														
							if($this->_framedepth < $this->maxframes)
							{
								$this->fetch($frameurl);
								$this->_framedepth++;
							}
							else
								break;
						}
					}					
					
				}
				else
				{
					return false;
				}
				return true;					
				break;
			}
			case "https":
				if( !$this->curl_path )
					return false;
				if( function_exists( "is_executable" ) )
				    if ( !is_executable( $this->curl_path ) )
				        return false;
				$this->host = $arrUrlParts[ "host" ];
				if( !empty( $arrUrlParts[ "port" ] ) )
					$this->port = $arrUrlParts[ "port" ];
				if( $this->_isproxy )
				{
					// using proxy, send entire URI
					$this->httpsRequestFilesAsPostAttributes
					(
						$strUrl					,
						$strUrl					, 
						$this->_submit_method	, 
						$this->_submit_type		, 
						$strPostData			, 
						$arrPostDataFiles 
					);
				}
				else
				{
					$strPath = $arrUrlParts[ "path" ] . ( $arrUrlParts[ "query" ] ? "?" . $arrUrlParts[ "query" ] : "");
					// no proxy, send only the path
					$this->httpsRequestFilesAsPostAttributes
					(
						$strPath				,
						$strUrl					,
						$this->_submit_method	,
						$this->_submit_type		,
						$strPostData			, 
						$arrPostDataFiles 
					);
				}

				if( $this->_redirectaddr )
				{
					/* url was redirected, check if we've hit the max depth */
					if( $this->maxredirs > $this->_redirectdepth )
					{						
						if( !preg_match( "|^" . $URI_PARTS["scheme"] . "://|" , $this->_redirectaddr ) )
							$this->_redirectaddr = $this->_expandlinks
							(
								$this->_redirectaddr , 
								$arrUrlParts[ "scheme" ] . "://" . $arrUrlParts[ "host" ]
							);

						// only follow redirect if it's on this site, or offsiteok is true
						if( preg_match( "|^http://" . preg_quote( $this->host ) . "|i" , $this->_redirectaddr ) || $this->offsiteok )
						{
							/* follow the redirect */
							$this->_redirectdepth++;
							$this->lastredirectaddr=$this->_redirectaddr;
							if( strpos( $this->_redirectaddr, "?" ) > 0 )
							{
								$this->fetch( $this->_redirectaddr ); // the redirect has changed the request method from post to get
							}
							else
							{
								$this->submit( $this->_redirectaddr , $arrRegularAttributes , $arrFormFiles );
							}
						}
					}
				}

				if($this->_framedepth < $this->maxframes && count($this->_frameurls) > 0)
				{
					$frameurls = $this->_frameurls;
					$this->_frameurls = array();

					while(list(,$frameurl) = each($frameurls))
					{														
						if($this->_framedepth < $this->maxframes)
						{
							$this->fetch($frameurl);
							$this->_framedepth++;
						}
						else
							break;
					}
				}					
				return true;					
				break;
				
			default:
				// not a valid protocol
				$this->error = 'Invalid protocol "' . $arrUrlParts[ "scheme" ] . '"\n';
				return false;
				break;
		}		
		return true;
	}
	
	public function sendFileToRequest( $strPostAttributeName , $strFileName , $objFileRequest )
	{
		$objFile = fopen( $strFileName , 'r' );
		if( !$objFile )
		{
			throw new Exception( 'Não foi possível abrir o arquivo ' . $strFileName);
		}
		
		$intBufferSize = $this->intBufferPostDataLength;
		
		switch ($this->_submit_type) {
			
			// formato para quando se envia apenas variaveis e nao arquivos //
			case "application/x-www-form-urlencoded":
			{	
				$strHeader = urlencode( $strPostAttributeName ) . "=" ;
				fwrite( $objFileRequest , $strHeader, strlen( $strHeader ) );
				while( ! feof( $objFile ) )
				{
					$strBuffer = urlencode( fread( $objFile , $intBufferSize ) );
					fwrite( $objFileRequest , $strBuffer , strlen( $strBuffer ) );
					$this->intActualDataOutput += strlen( $strBuffer );
					if( $this->boolHasProgressBarOutput)
					{
						$intPercent = ceil( ( $this->intActualDataOutput / $this->intTotalDataOutput ) * 100 );
						$this->objProgressBarOutput->setProgress( $intPercent );			
					}
				}
				
				fwrite( $objFileRequest , "&" , 1 );
				fclose( $objFile );
				break;
			}
			// formato para quando se envia variaveis e arquivos //
			case "multipart/form-data":
			default:
			{
				/**
				 * @todo Implementar para aceitar tb arquivos nesta submissão
				 */
				throw new Exception( 'Método de submissão não implementado para multipart/form-data.' );
				break;
			}
		}
	}
			
	public function getFileSize( $strPostAttributeName , $strFileName )
	{
		$objFile = fopen( $strFileName , 'r' );
		if( !$objFile )
		{
			throw new Exception( 'Não foi possível abrir o arquivo ' . $strFileName);
		}
		
		$intFileSize = 0;
		$intBufferSize = 128;
		
		switch ($this->_submit_type) {
			
			// formato para quando se envia apenas variaveis e nao arquivos //
			case "application/x-www-form-urlencoded":
			{	
				$strHeader = urlencode( $strPostAttributeName ) . "=" ;
				$intFileSize += strlen( $strHeader );
				
				while( ! feof( $objFile ) )
				{
					$strBuffer = fread( $objFile , $intBufferSize );
					$intFileSize += strlen( urlencode( $strBuffer ) );
				}
				
				fclose( $objFile );
				break;
			}
			// formato para quando se envia variaveis e arquivos //
			case "multipart/form-data":
			default:
			{
				/**
				 * @todo Implementar para aceitar tb arquivos nesta submissão
				 */
				throw new Exception( 'Método de submissão não implementado para multipart/form-data.' );
				break;
			}
		}
		
		return $intFileSize;
	}
	
	protected function httpRequestFilesAsPostAttributes( $strUrl , $fp , $strURI , $strHttpMethod , 
		$strContentType = "" , $strBody = ""  , $arrFilesPostAttributes )
	{
		# preparando
		$cookie_headers = '';
		if($this->passcookies && $this->_redirectaddr)
			$this->setcookies();
			
		$arrUriParts = parse_url($strURI);
		
		if( empty( $strUrl ) )
		{
			$strUrl = "/";
		}
		
		$strHeaders = $strHttpMethod . " " . $strUrl . " " . $this->_httpversion . "\r\n";
		
		if( !empty( $this->agent ) )
		{
			$strHeaders .= "User-Agent: " . $this->agent."\r\n";
		}
		
		if( !empty( $this->host ) && !isset( $this->rawheaders[ 'Host'] ) )
		{
			$strHeaders .= "Host: " . $this->host;
			if( !empty( $this->port ) )
			{
				$strHeaders .= ":" . $this->port;
			}
			$strHeaders .= "\r\n";
		}
		
		if( !empty( $this->accept ) )
		{
			$strHeaders .= "Accept: " . $this->accept . "\r\n";
		}
		
		if( !empty( $this->referer ) )
		{
			$strHeaders .= "Referer: " . $this->referer . "\r\n";
		}
		
		if( !empty( $this->cookies ) )
		{			
			if( !is_array( $this->cookies ) )
			{
				$this->cookies = (array) $this->cookies;
			}
	
			reset($this->cookies);
			if ( count($this->cookies) > 0 )
			{
				$cookie_headers .= 'Cookie: ';
				foreach ( $this->cookies as $cookieKey => $cookieVal )
				{
					$cookie_headers .= $cookieKey . "=" . urlencode( $cookieVal ) . "; ";
				}
				
				$strHeaders .= substr( $cookie_headers , 0 , -2 ) . "\r\n";
			} 
		}
		
		if( !empty( $this->rawheaders ) )
		{
			if( !is_array( $this->rawheaders ) )
			{
				$this->rawheaders = (array) $this->rawheaders;
			}
			
			while( list( $headerKey , $headerVal ) = each( $this->rawheaders ) )
			{
				$strHeaders .= $headerKey . ": " . $headerVal . "\r\n";
			}
		}
		
		if( !empty( $strContentType ) )
		{
			$strHeaders .= "Content-type: " . $strContentType;
			if ( $strContentType == "multipart/form-data")
			{
				$strHeaders .= "; boundary=" . $this->_mime_boundary;
			}
			$strHeaders .= "\r\n";
		}
		
		# calculando o tamanho do body 
		
		$intPostVarIntoFilesLength =  strlen( $strBody );
		
		foreach( $arrFilesPostAttributes as $strPostAttributeName => $strFileName )
		{
			$intPostVarIntoFilesLength += $this->getFileSize( $strPostAttributeName , $strFileName );
		}
		
		if( $intPostVarIntoFilesLength > 0 )
		{
			$strHeaders .= "Content-length: " . $intPostVarIntoFilesLength ."\r\n";
		}
		if( !empty( $this->user ) || !empty( $this->pass ) )
		{
			$strHeaders .= "Authorization: Basic " . base64_encode( $this->user . ":" . $this->pass ) . "\r\n";
		}
		
		//add proxy auth headers
		if( !empty( $this->proxy_user ) )
		{
			$strHeaders .= 'Proxy-Authorization: ' . 'Basic ' . base64_encode($this->proxy_user . ':' . $this->proxy_pass)."\r\n";
		}


		$strHeaders .= "\r\n";
		
		// set the read timeout if needed
		if( $this->read_timeout > 0)
		{
			socket_set_timeout( $fp, $this->read_timeout );
		}
		
		$this->timed_out = false;
		
		# enviando
		
		$this->intTotalDataOutput = $intPostVarIntoFilesLength;
//		fwrite( $fp , $strHeaders . $body , strlen( $strHeaders . $body ) );
		fwrite( $fp , $strHeaders . $strBody . "&" , strlen( $strHeaders . $strBody ) );
		foreach( $arrFilesPostAttributes as $strPostAttributeName => $strFileName )
		{
			$this->sendFileToRequest( $strPostAttributeName , $strFileName , $fp );
		}
		
		
		# recebendo
		$this->_redirectaddr = false;
		unset( $this->headers );
						
		while( $currentHeader = fgets( $fp , $this->_maxlinelen ) )
		{
			if( $this->read_timeout > 0 && $this->_check_timeout( $fp ) )
			{
				$this->status=-100;
				return false;
			}
				
			if($currentHeader == "\r\n")
				break;
						
			// if a header begins with Location: or URI:, set the redirect
			if( preg_match( "/^(Location:|URI:)/i" , $currentHeader ) )
			{
				// get URL portion of the redirect
				preg_match( "/^(Location:|URI:)[ ]+(.*)/i" , chop( $currentHeader ) , $matches );
				// look for :// in the Location header to see if hostname is included
				if( !preg_match( "|\:\/\/|" , $matches[ 2 ] ) )
				{
					// no host in the path, so prepend
					$this->_redirectaddr = $arrUriParts[ "scheme" ] . "://" . $this->host . ":" . $this->port;
					// eliminate double slash
					if( !preg_match( "|^/|" , $matches[ 2 ] ) )
					{
						$this->_redirectaddr .= "/" . $matches[ 2 ];
					}
					else
					{
						$this->_redirectaddr .= $matches[ 2 ];
					}
				}
				else
				{
					$this->_redirectaddr = $matches[2];
				}
			}
		
			if( preg_match( "|^HTTP/|" , $currentHeader ) )
			{
                if( preg_match( "|^HTTP/[^\s]*\s(.*?)\s|" , $currentHeader , $status ) )
				{
					$this->status= $status[ 1 ];
                }				
				$this->response_code = $currentHeader;
			}
				
			$this->headers[] = $currentHeader;
		}

		$results = '';
		$this->intTotalDataInput = $intPostVarIntoFilesLength;
		do {
    		$_data = fread($fp, $this->maxlength);
    		if (strlen($_data) == 0) {
        		break;
    		}
    		$this->bufferDeal( $results , $_data );
		} while(true);

		if( $this->read_timeout > 0 && $this->_check_timeout( $fp ) )
		{
			$this->status=-100;
			return false;
		}
		
		// check if there is a a redirect meta tag
		
		if(preg_match("'<meta[\s]*http-equiv[^>]*?content[\s]*=[\s]*[\"\']?\d+;[\s]*URL[\s]*=[\s]*([^\"\']*?)[\"\']?>'i",$results,$match))

		{
			$this->_redirectaddr = $this->_expandlinks($match[1],$strURI);	
		}

		// have we hit our frame depth and is there frame src to fetch?
		if(($this->_framedepth < $this->maxframes) && preg_match_all("'<frame\s+.*src[\s]*=[\'\"]?([^\'\"\>]+)'i",$results,$match))
		{
			$this->results[] = $results;
			for($x=0; $x<count($match[1]); $x++)
				$this->_frameurls[] = $this->_expandlinks($match[1][$x],$arrUriParts["scheme"]."://".$this->host);
		}
		// have we already fetched framed content?
		elseif(is_array($this->results))
			$this->results[] = $results;
		// no framed content
		else
			$this->results = $results;
		if( $this->boolHasProgressBarInput )
		{
			$this->objProgressBarInput->setProgress( 100 );
		}
		if( $this->boolHasProgressBarOutput )
		{
			$this->objProgressBarOutput->setProgress( 100 );
		}
		return true;
	}
	
	
	function httpsRequestFilesAsPostAttributes( $strUrl , $strURI , $strHttpMethod,$content_type="",$body="")
	{
		if($this->passcookies && $this->_redirectaddr)
			$this->setcookies();

		$headers = array();		
					
		$arrUriParts = parse_url($strURI);
		if(empty($strUrl))
			$strUrl = "/";
		// GET ... header not needed for curl
		//$headers[] = $strHttpMethod." ".$strUrl." ".$this->_httpversion;		
		if(!empty($this->agent))
			$headers[] = "User-Agent: ".$this->agent;
		if(!empty($this->host))
			if(!empty($this->port))
				$headers[] = "Host: ".$this->host.":".$this->port;
			else
				$headers[] = "Host: ".$this->host;
		if(!empty($this->accept))
			$headers[] = "Accept: ".$this->accept;
		if(!empty($this->referer))
			$headers[] = "Referer: ".$this->referer;
		if(!empty($this->cookies))
		{			
			if(!is_array($this->cookies))
				$this->cookies = (array)$this->cookies;
	
			reset($this->cookies);
			if ( count($this->cookies) > 0 ) {
				$cookie_str = 'Cookie: ';
				foreach ( $this->cookies as $cookieKey => $cookieVal ) {
				$cookie_str .= $cookieKey."=".urlencode($cookieVal)."; ";
				}
				$headers[] = substr($cookie_str,0,-2);
			}
		}
		if(!empty($this->rawheaders))
		{
			if(!is_array($this->rawheaders))
				$this->rawheaders = (array)$this->rawheaders;
			while(list($headerKey,$headerVal) = each($this->rawheaders))
				$headers[] = $headerKey.": ".$headerVal;
		}
		if(!empty($content_type)) {
			if ($content_type == "multipart/form-data")
				$headers[] = "Content-type: $content_type; boundary=".$this->_mime_boundary;
			else
				$headers[] = "Content-type: $content_type";
		}
		if(!empty($body))	
			$headers[] = "Content-length: ".strlen($body);
		if(!empty($this->user) || !empty($this->pass))	
			$headers[] = "Authorization: BASIC ".base64_encode($this->user.":".$this->pass);
			
		for($curr_header = 0; $curr_header < count($headers); $curr_header++) {
			$safer_header = strtr( $headers[$curr_header], "\"", " " );
			$cmdline_params .= " -H \"".$safer_header."\"";
		}
		
		if(!empty($body))
			$cmdline_params .= " -d \"$body\"";
		
		if($this->read_timeout > 0)
			$cmdline_params .= " -m ".$this->read_timeout;
		
		$headerfile = tempnam($temp_dir, "sno");

		$safer_URI = strtr( $strURI, "\"", " " ); // strip quotes from the URI to avoid shell access
		if($this->_isproxy) {
			exec($this->curl_path." -x \"" . $this->proxy_host . ":" . $this->proxy_port . "\" -U " . $this->proxy_user . ":" . $this->proxy_pass . " -k -D \"$headerfile\"".$cmdline_params." \"".$safer_URI."\"",$results,$return);
		}
		else {
			exec($this->curl_path." -k -D \"$headerfile\"".$cmdline_params." \"".$safer_URI."\"",$results,$return);
		}
		
		if($return)
		{
			$this->error = "Error: cURL could not retrieve the document, error $return.";
			return false;
		}
			
			
		$results = implode("\r\n",$results);
		
		$result_headers = file("$headerfile");
						
		$this->_redirectaddr = false;
		unset($this->headers);
						
		for($currentHeader = 0; $currentHeader < count($result_headers); $currentHeader++)
		{
			
			// if a header begins with Location: or URI:, set the redirect
			if(preg_match("/^(Location: |URI: )/i",$result_headers[$currentHeader]))
			{
				// get URL portion of the redirect
				preg_match("/^(Location: |URI:)\s+(.*)/",chop($result_headers[$currentHeader]),$matches);
				// look for :// in the Location header to see if hostname is included
				if(!preg_match("|\:\/\/|",$matches[2]))
				{
					// no host in the path, so prepend
					$this->_redirectaddr = $arrUriParts["scheme"]."://".$this->host.":".$this->port;
					// eliminate double slash
					if(!preg_match("|^/|",$matches[2]))
							$this->_redirectaddr .= "/".$matches[2];
					else
							$this->_redirectaddr .= $matches[2];
				}
				else
					$this->_redirectaddr = $matches[2];
			}
		
			if(preg_match("|^HTTP/|",$result_headers[$currentHeader]))
				$this->response_code = $result_headers[$currentHeader];

			$this->headers[] = $result_headers[$currentHeader];
		}

		// check if there is a a redirect meta tag
		
		if(preg_match("'<meta[\s]*http-equiv[^>]*?content[\s]*=[\s]*[\"\']?\d+;[\s]*URL[\s]*=[\s]*([^\"\']*?)[\"\']?>'i",$results,$match))
		{
			$this->_redirectaddr = $this->_expandlinks($match[1],$URI);	
		}

		// have we hit our frame depth and is there frame src to fetch?
		if(($this->_framedepth < $this->maxframes) && preg_match_all("'<frame\s+.*src[\s]*=[\'\"]?([^\'\"\>]+)'i",$results,$match))
		{
			$this->results[] = $results;
			for($x=0; $x<count($match[1]); $x++)
				$this->_frameurls[] = $this->_expandlinks($match[1][$x],$URI_PARTS["scheme"]."://".$this->host);
		}
		// have we already fetched framed content?
		elseif(is_array($this->results))
			$this->results[] = $results;
		// no framed content
		else
			$this->results = $results;

		if( $this->boolHasProgressBarInput )
		{
			$this->objProgressBarInput->setProgress( 100 );
		}
		if( $this->boolHasProgressBarOutput )
		{
			$this->objProgressBarOutput->setProgress( 100 );
		}
		unlink("$headerfile");
		
		return true;
	}
	
}
?>