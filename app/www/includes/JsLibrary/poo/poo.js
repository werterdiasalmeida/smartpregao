window.Class =	function Class( strClassName , arrMethodsAndAttributes )
{
	this.className = strClassName;			
	
	window[strClassName] = function ClassElement()
	{
		this.__construct = function __construct()
		{
			strCommand = strClassName + '.prototype.' + '__construct( ' + strClassName + '.arguments )';
			eval( strCommand );
		}
		
		if( IE )
		{
			if( isUndefined( window.Class.arrInstances ) )
			{
				window.Class.arrInstances = new Array();
			}
		}
		window.Class.arrInstances.push( this );
		this.__construct();
	}
	
	for( strElement in arrMethodsAndAttributes )
	{
		funcMethod = arrMethodsAndAttributes[ strElement ];
		strCommand = strClassName + '.prototype.' + strElement + ' =  ' + funcMethod;
		window.eval( strCommand );
	}
}
window.Class.arrInstances = new Array();

window.ClassExtends = function ClassExtends( strClassName, strParentClass, arrMethodsAndAttributes )
{
	this.parentClassName = strParentClass;
	this.className = strClassName;
	 
	window[strClassName] = function ClassElement()
	{
		that = new this.parentClassName();
		window.Class.arrInstances.push( that );
		that.__construct();
	}
	
	for( strElement in arrMethodsAndAttributes )
	{
		funcMethod = arrMethodsAndAttributes[ strElement ]; 
		strCommand = strClassName + '.prototype.' + strElement + ' =  ' + funcMethod;
		window.eval( strCommand );
	}
}

window.onunload = function DestructClassElements()
{
	for( var i = 0 ; i < window.Class.arrInstances.length ; ++i )
	{
		var objClassElement = window.Class.arrInstances[ i ];
		if( objClassElement.__destruct !== undefined )
		{
			objClassElement.__destruct();
		}
	}
	window.Class.arrInstances = new Array();
}