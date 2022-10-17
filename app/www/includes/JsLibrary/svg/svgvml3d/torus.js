function TorusZ(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight, R, N, r, n)
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
  this.Poly3D=new Array();
  var s, x, y, z, dR, p, Theta, Phi, d=-1, M=(2*n-1)*(2*N-1)-d;
  for (s=0; s<M/2; s++)
  { this.Poly3D[s]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);
    
    p=2*s+1;
    Theta=2*Math.PI*p/M;
    Phi=(2*N-1)*2*Math.PI*p/M;
    dR=r*Math.sin(Phi);
    z=r*Math.cos(Phi);
    x=(R+dR)*Math.sin(Theta);
    y=(R+dR)*Math.cos(Theta);
    this.Poly3D[s].AddPoint(x,y,z);
      
    p=2*s+(2*n-1)+2;
    Theta=2*Math.PI*p/M;
    Phi=(2*N-1)*2*Math.PI*p/M;
    dR=r*Math.sin(Phi);
    z=r*Math.cos(Phi);
    x=(R+dR)*Math.sin(Theta);
    y=(R+dR)*Math.cos(Theta);
    this.Poly3D[s].AddPoint(x,y,z);
      
    p=2*s+2*(2*n-1)+1;
    Theta=2*Math.PI*p/M;
    Phi=(2*N-1)*2*Math.PI*p/M;
    dR=r*Math.sin(Phi);
    z=r*Math.cos(Phi);
    x=(R+dR)*Math.sin(Theta);
    y=(R+dR)*Math.cos(Theta);
    this.Poly3D[s].AddPoint(x,y,z);
      
    p=2*s+(2*n-1);
    Theta=2*Math.PI*p/M;
    Phi=(2*N-1)*2*Math.PI*p/M;
    dR=r*Math.sin(Phi);
    z=r*Math.cos(Phi);
    x=(R+dR)*Math.sin(Theta);
    y=(R+dR)*Math.cos(Theta);
    this.Poly3D[s].AddPoint(x,y,z);
          
    this.Poly3D[s].Update();
    
  }    
}
function TorusX(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight, R, N, r, n)
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
  this.Poly3D=new Array();
  var s, x, y, z, dR, p, Theta, Phi, d=-1;
  for (s=0; s<n*N-d; s++)
  { this.Poly3D[s]=new Poly3D(aParentScene, aFrontColor, aBackColor, aStrokeColor, aStrokeWeight);

    p=s;
    Theta=2*Math.PI*p/(n*N-d);
    Phi=2*Math.PI*p*N/(n*N-d);
    dR=r*Math.sin(Phi);
    z=r*Math.cos(Phi);
    x=(R+dR)*Math.sin(Theta);
    y=(R+dR)*Math.cos(Theta);
    this.Poly3D[s].AddPoint(z,x,y);
      
    p=s+1;
    Theta=2*Math.PI*p/(n*N-d);
    Phi=2*Math.PI*p*N/(n*N-d);
    dR=r*Math.sin(Phi);
    z=r*Math.cos(Phi);
    x=(R+dR)*Math.sin(Theta);
    y=(R+dR)*Math.cos(Theta);
    this.Poly3D[s].AddPoint(z,x,y); 
      
    p=s+n+1;
    Theta=2*Math.PI*p/(n*N-d);
    Phi=2*Math.PI*p*N/(n*N-d);
    dR=r*Math.sin(Phi);
    z=r*Math.cos(Phi);
    x=(R+dR)*Math.sin(Theta);
    y=(R+dR)*Math.cos(Theta);
    this.Poly3D[s].AddPoint(z,x,y);
      
    p=s+n;
    Theta=2*Math.PI*p/(n*N-d);
    Phi=2*Math.PI*p*N/(n*N-d);
    dR=r*Math.sin(Phi);
    z=r*Math.cos(Phi);
    x=(R+dR)*Math.sin(Theta);
    y=(R+dR)*Math.cos(Theta);
    this.Poly3D[s].AddPoint(z,x,y);   
      
    this.Poly3D[s].Update();
    
  }    
}