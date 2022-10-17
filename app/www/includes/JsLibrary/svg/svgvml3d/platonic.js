//Thanks to Paul Bourke (http://astronomy.swin.edu.au/~pbourke/) who provided the coordinates

function Tetrahedron(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight)
{ this.Parent=aParentScene;
  this.ClassName="Tetrahedron";
  this.Center=new Vector(0,0,0);
  this.FrontColor=aFrontColor;
  this.BackColor=aBackColor;
  this.StrokeColor=aStrokeColor;
  this.StrokeWeight=aStrokeWeight;
  this.Zoom=_Object3DZoom;
  this.Shift=_Object3DShift;
  this.SetFrontColor=_Object3DSetFrontColor;
  this.SetBackColor=_Object3DSetBackColor;
  this.SetStrokeColor=_Object3DSetStrokeColor;
  this.SetStrokeWeight=_Object3DSetStrokeWeight;
  this.SetVisibility=_Object3DSetVisibility;
  this.RotateX=_Object3DRotateX;
  this.RotateY=_Object3DRotateY;
  this.RotateZ=_Object3DRotateZ;
  this.SetId=_Object3DSetId;
  this.SetEventAction=_Object3DSetEventAction;
  this.Poly3D=new Array();
  var a=0.5;
  this.Poly3D[0]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[0])
  { AddPoint(a,a,a); AddPoint(-a,a,-a); AddPoint(a,-a,-a); Update(); }
  this.Poly3D[1]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[1])
  { AddPoint(-a,a,-a); AddPoint(-a,-a,a); AddPoint(a,-a,-a); Update(); }
  this.Poly3D[2]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[2])
  { AddPoint(a,a,a); AddPoint(a,-a,-a); AddPoint(-a,-a,a); Update(); }
  this.Poly3D[3]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[3])
  { AddPoint(a,a,a); AddPoint(-a,-a,a); AddPoint(-a,a,-a); Update(); }
}
function Octahedron(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight)
{ this.Parent=aParentScene;
  this.ClassName="Octahedron";
  this.Center=new Vector(0,0,0);
  this.FrontColor=aFrontColor;
  this.BackColor=aBackColor;
  this.StrokeColor=aStrokeColor;
  this.StrokeWeight=aStrokeWeight;
  this.Zoom=_Object3DZoom;
  this.Shift=_Object3DShift;
  this.SetFrontColor=_Object3DSetFrontColor;
  this.SetBackColor=_Object3DSetBackColor;
  this.SetStrokeColor=_Object3DSetStrokeColor;
  this.SetStrokeWeight=_Object3DSetStrokeWeight;
  this.SetVisibility=_Object3DSetVisibility;
  this.RotateX=_Object3DRotateX;
  this.RotateY=_Object3DRotateY;  
  this.RotateZ=_Object3DRotateZ;
  this.SetId=_Object3DSetId;
  this.SetEventAction=_Object3DSetEventAction;
  this.Poly3D=new Array();
  var a=Math.sqrt(0.125), b=0.5;
  this.Poly3D[0]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[0])
  { AddPoint(-a,0,a); AddPoint(-a,0,-a); AddPoint(0,b,0); Update(); }
  this.Poly3D[1]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[1])
  { AddPoint(-a,0,-a); AddPoint(a,0,-a); AddPoint(0,b,0); Update(); }
  this.Poly3D[2]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[2])
  { AddPoint(a,0,-a); AddPoint(a,0,a); AddPoint(0,b,0); Update(); }
  this.Poly3D[3]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[3])
  { AddPoint(a,0,a); AddPoint(-a,0,a); AddPoint(0,b,0); Update(); }
  this.Poly3D[4]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[4])
  { AddPoint(a,0,-a); AddPoint(-a,0,-a); AddPoint(0,-b,0); Update(); }
  this.Poly3D[5]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[5])
  { AddPoint(-a,0,-a); AddPoint(-a,0,a); AddPoint(0,-b,0); Update(); }
  this.Poly3D[6]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[6])
  { AddPoint(a,0,a); AddPoint(a,0,-a); AddPoint(0,-b,0); Update(); }
  this.Poly3D[7]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[7])
  { AddPoint(-a,0,a); AddPoint(a,0,a); AddPoint(0,-b,0); Update(); }
}
function Cube(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight)
{ this.Parent=aParentScene;
  this.ClassName="Cube";
  this.Center=new Vector(0,0,0);
  this.FrontColor=aFrontColor;
  this.BackColor=aBackColor;
  this.StrokeColor=aStrokeColor;
  this.StrokeWeight=aStrokeWeight;
  this.Zoom=_Object3DZoom;
  this.Shift=_Object3DShift;
  this.SetFrontColor=_Object3DSetFrontColor;
  this.SetBackColor=_Object3DSetBackColor;
  this.SetStrokeColor=_Object3DSetStrokeColor;
  this.SetStrokeWeight=_Object3DSetStrokeWeight;
  this.SetVisibility=_Object3DSetVisibility;
  this.RotateX=_Object3DRotateX;
  this.RotateY=_Object3DRotateY;
  this.RotateZ=_Object3DRotateZ;
  this.SetId=_Object3DSetId;
  this.SetEventAction=_Object3DSetEventAction;
  this.Poly3D=new Array();
  var a=0.5;
  this.Poly3D[0]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[0])
  { AddPoint(-a,-a,a); AddPoint(a,-a,a); AddPoint(a,-a,-a); AddPoint(-a,-a,-a); Update(); }
  this.Poly3D[1]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[1])
  { AddPoint(-a,a,-a); AddPoint(-a,a,a); AddPoint(-a,-a,a); AddPoint(-a,-a,-a); Update(); }
  this.Poly3D[2]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[2])
  { AddPoint(-a,a,a); AddPoint(a,a,a); AddPoint(a,-a,a); AddPoint(-a,-a,a); Update(); }
  this.Poly3D[3]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[3])
  { AddPoint(a,a,-a); AddPoint(a,a,a); AddPoint(-a,a,a); AddPoint(-a,a,-a); Update(); }
  this.Poly3D[4]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[4])
  { AddPoint(a,-a,a); AddPoint(a,a,a); AddPoint(a,a,-a); AddPoint(a,-a,-a); Update(); }
  this.Poly3D[5]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[5])
  { AddPoint(a,-a,-a); AddPoint(a,a,-a); AddPoint(-a,a,-a); AddPoint(-a,-a,-a); Update(); }
}
function Icosahedron(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight)
{ this.Parent=aParentScene;
  this.ClassName="Icosahedron";
  this.Center=new Vector(0,0,0);
  this.FrontColor=aFrontColor;
  this.BackColor=aBackColor;
  this.StrokeColor=aStrokeColor;
  this.StrokeWeight=aStrokeWeight;
  this.Zoom=_Object3DZoom;
  this.Shift=_Object3DShift;
  this.SetFrontColor=_Object3DSetFrontColor;
  this.SetBackColor=_Object3DSetBackColor;
  this.SetStrokeColor=_Object3DSetStrokeColor;
  this.SetStrokeWeight=_Object3DSetStrokeWeight;
  this.SetVisibility=_Object3DSetVisibility;
  this.RotateX=_Object3DRotateX;
  this.RotateY=_Object3DRotateY;  
  this.RotateZ=_Object3DRotateZ;
  this.SetId=_Object3DSetId;
  this.SetEventAction=_Object3DSetEventAction;
  this.Poly3D=new Array();
  var a=0.5, b=1.0/(1.0+Math.sqrt(5.0));
  this.Poly3D[0]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[0])
  { AddPoint(0,b,-a); AddPoint(b,a,0); AddPoint(-b,a,0); Update(); }
  this.Poly3D[1]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[1])
  { AddPoint(0,b,a); AddPoint(-b,a,0); AddPoint(b,a,0); Update(); }
  this.Poly3D[2]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[2])
  { AddPoint(0,b,a); AddPoint(0,-b,a); AddPoint(-a,0,b); Update(); }
  this.Poly3D[3]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[3])
  { AddPoint(0,b,a); AddPoint(a,0,b); AddPoint(0,-b,a); Update(); }
  this.Poly3D[4]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[4])
  { AddPoint(0,b,-a); AddPoint(0,-b,-a); AddPoint(a,0,-b); Update(); }
  this.Poly3D[5]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[5])
  { AddPoint(0,b,-a); AddPoint(-a,0,-b); AddPoint(0,-b,-a); Update(); }
  this.Poly3D[6]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[6])
  { AddPoint(0,-b,a); AddPoint(b,-a,0); AddPoint(-b,-a,0); Update(); }
  this.Poly3D[7]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[7])
  { AddPoint(0,-b,-a); AddPoint(-b,-a,0); AddPoint(b,-a,0); Update(); }
  this.Poly3D[8]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[8])
  { AddPoint(-b,a,0); AddPoint(-a,0,b); AddPoint(-a,0,-b); Update(); }
  this.Poly3D[9]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[9])
  { AddPoint(-b,-a,0); AddPoint(-a,0,-b); AddPoint(-a,0,b); Update(); }
  this.Poly3D[10]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[10])
  { AddPoint(b,a,0); AddPoint(a,0,-b); AddPoint(a,0,b); Update(); }
  this.Poly3D[11]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[11])
  { AddPoint(b,-a,0); AddPoint(a,0,b); AddPoint(a,0,-b); Update(); }
  this.Poly3D[12]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[12])
  { AddPoint(0,b,a); AddPoint(-a,0,b); AddPoint(-b,a,0); Update(); }
  this.Poly3D[13]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[13])
  { AddPoint(0,b,a); AddPoint(b,a,0); AddPoint(a,0,b); Update(); }
  this.Poly3D[14]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[14])
  { AddPoint(0,b,-a); AddPoint(-b,a,0); AddPoint(-a,0,-b); Update(); }
  this.Poly3D[15]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[15])
  { AddPoint(0,b,-a); AddPoint(a,0,-b); AddPoint(b,a,0); Update(); }
  this.Poly3D[16]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[16])
  { AddPoint(0,-b,-a); AddPoint(-a,0,-b); AddPoint(-b,-a,0); Update(); }
  this.Poly3D[17]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[17])
  { AddPoint(0,-b,-a); AddPoint(b,-a,0); AddPoint(a,0,-b); Update(); }
  this.Poly3D[18]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[18])
  { AddPoint(0,-b,a); AddPoint(-b,-a,0); AddPoint(-a,0,b); Update(); }
  this.Poly3D[19]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[19])
  { AddPoint(0,-b,a); AddPoint(a,0,b); AddPoint(b,-a,0); Update(); }
}
function Dodecahedron(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight)
{ this.Parent=aParentScene;
  this.ClassName="Dodecahedron";
  this.Center=new Vector(0,0,0);
  this.FrontColor=aFrontColor;
  this.BackColor=aBackColor;
  this.StrokeColor=aStrokeColor;
  this.StrokeWeight=aStrokeWeight;
  this.Zoom=_Object3DZoom;
  this.Shift=_Object3DShift;
  this.SetFrontColor=_Object3DSetFrontColor;
  this.SetBackColor=_Object3DSetBackColor;
  this.SetStrokeColor=_Object3DSetStrokeColor;
  this.SetStrokeWeight=_Object3DSetStrokeWeight;
  this.SetVisibility=_Object3DSetVisibility;
  this.RotateX=_Object3DRotateX;
  this.RotateY=_Object3DRotateY;  
  this.RotateZ=_Object3DRotateZ;
  this.SetId=_Object3DSetId;
  this.SetEventAction=_Object3DSetEventAction;
  this.Poly3D=new Array();
  var a=1.0/(1.0+Math.sqrt(5.0)), b=(1.0+Math.sqrt(5.0))/4.0, c=0.5;
  this.Poly3D[0]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[0])
  { AddPoint(-c,-c,c); AddPoint(-b,0,a); AddPoint(-c,c,c); AddPoint(0,a,b); AddPoint(0,-a,b); Update(); }
  this.Poly3D[1]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[1])
  { AddPoint(0,-a,b); AddPoint(0,a,b); AddPoint(c,c,c); AddPoint(b,0,a); AddPoint(c,-c,c); Update(); }
  this.Poly3D[2]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[2])
  { AddPoint(c,-c,-c); AddPoint(b,0,-a); AddPoint(c,c,-c); AddPoint(0,a,-b); AddPoint(0,-a,-b); Update(); }
  this.Poly3D[3]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[3])
  { AddPoint(-c,c,-c); AddPoint(-b,0,-a); AddPoint(-c,-c,-c); AddPoint(0,-a,-b); AddPoint(0,a,-b); Update(); }
  this.Poly3D[4]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[4])
  { AddPoint(c,-c,-c); AddPoint(a,-b,0); AddPoint(c,-c,c); AddPoint(b,0,a); AddPoint(b,0,-a); Update(); }
  this.Poly3D[5]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[5])
  { AddPoint(b,0,-a); AddPoint(b,0,a); AddPoint(c,c,c); AddPoint(a,b,0); AddPoint(c,c,-c); Update(); }
  this.Poly3D[6]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[6])
  { AddPoint(-b,0,-a); AddPoint(-b,0,a); AddPoint(-c,-c,c); AddPoint(-a,-b,0); AddPoint(-c,-c,-c); Update(); }
  this.Poly3D[7]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[7])
  { AddPoint(-c,c,-c); AddPoint(-a,b,0); AddPoint(-c,c,c); AddPoint(-b,0,a); AddPoint(-b,0,-a); Update(); }
  this.Poly3D[8]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[8])
  { AddPoint(c,c,c); AddPoint(0,a,b); AddPoint(-c,c,c); AddPoint(-a,b,0); AddPoint(a,b,0); Update(); }
  this.Poly3D[9]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[9])
  { AddPoint(a,b,0); AddPoint(-a,b,0); AddPoint(-c,c,-c); AddPoint(0,a,-b); AddPoint(c,c,-c); Update(); }
  this.Poly3D[10]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[10])
  { AddPoint(a,-b,0); AddPoint(-a,-b,0); AddPoint(-c,-c,c); AddPoint(0,-a,b); AddPoint(c,-c,c); Update(); }
  this.Poly3D[11]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
  with (this.Poly3D[11])
  { AddPoint(c,-c,-c); AddPoint(0,-a,-b); AddPoint(-c,-c,-c); AddPoint(-a,-b,0); AddPoint(a,-b,0); Update(); }
}