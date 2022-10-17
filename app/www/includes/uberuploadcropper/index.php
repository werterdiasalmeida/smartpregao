<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
		
		<title>Uber Upload Cropper</title>
		
		<link href="css/default.css" rel="stylesheet" type="text/css" />
		<link href="css/uploadify.css" rel="stylesheet" type="text/css" />
		<link href="scripts/Jcrop/css/jquery.Jcrop.css" rel="stylesheet" type="text/css" />
		
		<script type="text/javascript" src="scripts/jquery-1.4.2.min.js"></script>
		<script type="text/javascript" src="scripts/swfobject.js"></script>
		<script type="text/javascript" src="scripts/jquery.uploadify.v2.1.0.min.js"></script>
		<script type="text/javascript" src="scripts/jquery-impromptu.3.1.min.js"></script>
		<script type="text/javascript" src="scripts/Jcrop/js/jquery.Jcrop.min.js"></script>
		<script type="text/javascript" src="scripts/jquery-uberuploadcropper.js"></script>
		
		<script type="text/javascript">
			$(function() {
				
				$('#UploadPhoto').uberuploadcropper({
					//---------------------------------------------------
					// uploadify options..
					//---------------------------------------------------
					'uploader'  : 'scripts/uploadify.swf',
					'script'    : 'upload.php',
					'cancelImg' : 'cancel.png',
					'multi'     : false,
					'auto'      : true,
					'folder'    : 'uploads',
					'fileDesc'  : 'JPG',
					'fileExt'   : '*.jpg;*.jpeg',
					//---------------------------------------------------
					//now the cropper options..
					//---------------------------------------------------
					//'aspectRatio': 1, 
					'allowSelect': false,			//can reselect
					'allowResize' : true,			//can resize selection
					'setSelect': [ 0, 0, 100, 100 ],	//these are the dimensions of the crop box x1,y1,x2,y2
					//'minSize': [ 100, 100 ],		//if you want to be able to resize, use these
					//'maxSize': [ 100, 100 ],
					/*
					onError: function (a, b, c, d) {
						if (d.status == 404)
							alert('Could not find upload script. Use a path relative to: '+'<?= getcwd() ?>');
						else if (d.type === "HTTP")
							alert('error '+d.type+": "+d.status);
						else if (d.type ==="File Size")
							alert(c.name+' '+d.type+' Limit: '+Math.round(d.sizeLimit/1024)+'KB');
						else
							alert('error '+d.type+": "+d.text);
					},*/
					//---------------------------------------------------
					//now the uber options..
					//---------------------------------------------------
					'cropScript': 'crop.php',
					'onComplete': function(imgs,data){ 
						$('#PhotoPrev').attr('src','uploads/'+imgs[0].name +'?d='+ (new Date()).getTime()); 
					}
				});
				
			});
		</script>
	</head>

	<body>
		<input type="file" name="UploadPhoto" id="UploadPhoto" />
		
		<img src="photo_not_available.png" id="PhotoPrev" alt="Photo Preview" />
		
	</body>
</html>
