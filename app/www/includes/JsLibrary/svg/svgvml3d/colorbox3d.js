function ColorBox3D(aParentScene, aX0,aY0,aZ0, aX1,aY1,aZ1, 
               aX0Color, aY0Color, aZ0Color, aX1Color, aY1Color, aZ1Color, 
               aStrokeColor, aStrokeWeight)
{ this.Parent=aParentScene;
  this.ClassName="Box3D";
  this.Center=new Vector((aX0+aX1)/2,(aY0+aY1)/2,(aZ0+aZ1)/2);
  this.X0Color=aX0Color;
  this.Y0Color=aY0Color;
  this.Z0Color=aZ0Color;
  this.X1Color=aX1Color;
  this.Y1Color=aY1Color;
  this.Z1Color=aZ1Color;
  this.StrokeColor=aStrokeColor;
  this.StrokeWeight=aStrokeWeight;
  this.Zoom=_Object3DZoom;
  this.Shift=_Object3DShift;
  this.SetStrokeColor=_Object3DSetStrokeColor;
  this.SetStrokeWeight=_Object3DSetStrokeWeight;
  this.SetVisibility=_Object3DSetVisibility;
  this.RotateX=_Object3DRotateX;
  this.RotateY=_Object3DRotateY;    
  this.RotateZ=_Object3DRotateZ;
  this.SetPosition=_ColorBox3DSetPosition;
  this.Poly3D=new Array();
  var nn=0;
  if (aZ0Color!="#000000")
  { this.Poly3D[nn]=new Poly3D(aParentScene, aZ0Color, "#000000", aStrokeColor, aStrokeWeight);
    with (this.Poly3D[nn])
    { AddPoint(aX0,aY0,aZ0); AddPoint(aX1,aY0,aZ0); AddPoint(aX1,aY1,aZ0); AddPoint(aX0,aY1,aZ0); Update(); }
    nn++;
  }
  if (aZ1Color!="#000000")
  { this.Poly3D[nn]=new Poly3D(aParentScene, aZ1Color, "#000000", aStrokeColor, aStrokeWeight);
    with (this.Poly3D[nn])
    { AddPoint(aX0,aY1,aZ1); AddPoint(aX1,aY1,aZ1); AddPoint(aX1,aY0,aZ1); AddPoint(aX0,aY0,aZ1); Update(); }
    nn++;
  }
  if (aY0Color!="#000000")
  { this.Poly3D[nn]=new Poly3D(aParentScene, aY0Color, "#000000", aStrokeColor, aStrokeWeight);
    with (this.Poly3D[nn])
    { AddPoint(aX0,aY0,aZ1); AddPoint(aX1,aY0,aZ1); AddPoint(aX1,aY0,aZ0); AddPoint(aX0,aY0,aZ0); Update(); }
    nn++;
  }
  if (aY1Color!="#000000")
  { this.Poly3D[nn]=new Poly3D(aParentScene, aY1Color, "#000000", aStrokeColor, aStrokeWeight);
    with (this.Poly3D[nn])
    { AddPoint(aX0,aY1,aZ0); AddPoint(aX1,aY1,aZ0); AddPoint(aX1,aY1,aZ1); AddPoint(aX0,aY1,aZ1); Update(); }
    nn++;
  }
  if (aX0Color!="#000000")
  { this.Poly3D[nn]=new Poly3D(aParentScene, aX0Color, "#000000", aStrokeColor, aStrokeWeight);
    with (this.Poly3D[nn])
    { AddPoint(aX0,aY1,aZ0); AddPoint(aX0,aY1,aZ1); AddPoint(aX0,aY0,aZ1); AddPoint(aX0,aY0,aZ0); Update(); }
    nn++;
  }
  if (aX1Color!="#000000")
  { this.Poly3D[nn]=new Poly3D(aParentScene, aX1Color, "#000000", aStrokeColor, aStrokeWeight);
    with (this.Poly3D[nn])
    { AddPoint(aX1,aY0,aZ0); AddPoint(aX1,aY0,aZ1); AddPoint(aX1,aY1,aZ1); AddPoint(aX1,aY1,aZ0); Update(); }
    nn++;
  }
}
function _ColorBox3DSetPosition(aX0,aY0,aZ0, aX1,aY1,aZ1)
{ var nn=0;
  if (this.Z0Color!="#000000")
  { with (this.Poly3D[nn])
    { SetPoint(0,aX0,aY0,aZ0); SetPoint(1,aX1,aY0,aZ0); SetPoint(2,aX1,aY1,aZ0); SetPoint(3,aX0,aY1,aZ0); Update(); }
    nn++;
  }
  if (this.Z1Color!="#000000")
  { with (this.Poly3D[nn])
    { SetPoint(0,aX0,aY1,aZ1); SetPoint(1,aX1,aY1,aZ1); SetPoint(2,aX1,aY0,aZ1); SetPoint(3,aX0,aY0,aZ1); Update(); }
    nn++;
  }
  if (this.Y0Color!="#000000")
  { with (this.Poly3D[nn])
    { SetPoint(0,aX0,aY0,aZ1); SetPoint(1,aX1,aY0,aZ1); SetPoint(2,aX1,aY0,aZ0); SetPoint(3,aX0,aY0,aZ0); Update(); }
    nn++;
  }
  if (this.Y1Color!="#000000")
  { with (this.Poly3D[nn])
    { SetPoint(0,aX0,aY1,aZ0); SetPoint(1,aX1,aY1,aZ0); SetPoint(2,aX1,aY1,aZ1); SetPoint(3,aX0,aY1,aZ1); Update(); }
    nn++;
  }
  if (this.X0Color!="#000000")
  { with (this.Poly3D[nn])
    { SetPoint(0,aX0,aY1,aZ0); SetPoint(1,aX0,aY1,aZ1); SetPoint(2,aX0,aY0,aZ1); SetPoint(3,aX0,aY0,aZ0); Update(); }
    nn++;
  }
  if (this.X1Color!="#000000")
  { with (this.Poly3D[nn])
    { SetPoint(0,aX1,aY0,aZ0); SetPoint(1,aX1,aY0,aZ1); SetPoint(2,aX1,aY1,aZ1); SetPoint(3,aX1,aY1,aZ0); Update(); }
    nn++;
  }
}	