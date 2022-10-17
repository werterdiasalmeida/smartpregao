function BoundingBox(aParentScene, aFillColor, aStrokeColor)
{ var ii, jj;
  this.Parent=aParentScene;
  this.ClassName="BoundingBox";
  this.Min=new Vector(0.0, 0.0, 0.0);
  this.Max=new Vector(0.0, 0.0, 0.0);
  this.Scale=new Vector(1, 1, 1);
  this.Label=new Vector("X", "Y", "Z");
  this.GridDelta=new Vector(0, 0, 0);
  this.FillColor=aFillColor;
  this.StrokeColor=aStrokeColor;
  this.StrokeWeight=1;
  this.SetBorder=_BoundingBoxSetBorder;
  this.Draw=_BoundingBoxDraw;
  this.Plane=new Array(3);
  this.Line=new Array(6);
  this.Text=new Array(6);
  for (ii=0; ii<6; ii++)
  { this.Line[ii]=new Array(11);
    this.Text[ii]=new Array(11);
  }  
  if (useSVG)
  { for (ii=0; ii<3; ii++)
    { this.Plane[ii] = this.Parent.Parent.GetSVGDoc().createElementNS("http://www.w3.org/2000/svg","path");
      this.Plane[ii].setAttribute("z-index",this.Parent.zIndex);
      this.Parent.Parent.GetScene().appendChild(this.Plane[ii]);
    }
    for (ii=0; ii<6; ii++)
    { for (jj=0; jj<11; jj++)
      { this.Line[ii][jj] = this.Parent.Parent.GetSVGDoc().createElementNS("http://www.w3.org/2000/svg","line");
        this.Line[ii][jj].setAttribute("z-index",this.Parent.zIndex+1);
        this.Line[ii][jj].setAttribute("stroke-width",1);
        this.Parent.Parent.GetScene().appendChild(this.Line[ii][jj]);
        this.Text[ii][jj] = this.Parent.Parent.GetSVGDoc().createElementNS("http://www.w3.org/2000/svg","text");
        with (this.Text[ii][jj])
        { setAttribute("width",1); 
          setAttribute("height",20);
          setAttribute("font-family","Verdana");
          setAttribute("font-size","12px");
          setAttribute("style","text-anchor:middle");
          setAttribute("z-index",this.Parent.zIndex+2);
        }
        this.Parent.Parent.GetScene().appendChild(this.Text[ii][jj]);
        this.Text[ii][jj].appendChild(this.Parent.Parent.GetSVGDoc().createTextNode(''));        
      }
    }    
  }
  else  	
  { for (ii=0; ii<3; ii++)
    { this.Plane[ii] = this.Parent.Document.createElement("v:shape");
      with (this.Plane[ii].style)
      { position="absolute"; left=0; top=0; 
        width=this.Parent.BoxGroup.style.width; 
        height=this.Parent.BoxGroup.style.height;
        zIndex=this.Parent.zIndex;
      }
      this.Parent.BoxGroup.insertBefore(this.Plane[ii], null);
    }
    for (ii=0; ii<6; ii++)
    { for (jj=0; jj<11; jj++)
      { this.Line[ii][jj] = this.Parent.Document.createElement("v:line");
        with (this.Line[ii][jj].style)
        { position="absolute"; left=0; top=0; 
          width=this.Parent.BoxGroup.style.width; 
          height=this.Parent.BoxGroup.style.height;
          zIndex=this.Parent.zIndex+1;
        }
        this.Line[ii][jj].strokeweight=1;
        this.Parent.BoxGroup.insertBefore(this.Line[ii][jj], null);
        this.Text[ii][jj] = this.Parent.Document.createElement("div");
        with (this.Text[ii][jj].style)
        { position="absolute"; left=0; top=0; 
          width=100; 
          height=20;
          fontFamily="Verdana";
          fontSize="12px";
          textAlign="center";
          zIndex=this.Parent.zIndex+2;
        }
        this.Parent.BoxGroup.insertBefore(this.Text[ii][jj], null);
      }
    }
  }  
  this.Parent.BoundingBox=this;
}
function _BoundingBoxSetBorder(xxmin,yymin,zzmin, xxmax,yymax,zzmax)
{ vv=this.Parent.Zoom;
  this.Min.x=xxmin*vv.x;
  this.Min.y=yymin*vv.y;
  this.Min.z=zzmin*vv.z;
  this.Max.x=xxmax*vv.x;
  this.Max.y=yymax*vv.y;
  this.Max.z=zzmax*vv.z;
}
function _OAV(oo,aa,vv) //Object, Attribute, Value
{ if (useSVG)
  { if (aa=="from")
    { with (oo) 
      { setAttribute("x1",vv.split(",")[0]);
        setAttribute("y1",vv.split(",")[1]);
      }
      return;
    }
    if (aa=="to")
    { with (oo) 
      { setAttribute("x2",vv.split(",")[0]);
        setAttribute("y2",vv.split(",")[1]);
      }
      return;      
    }
    if (aa=="color")
    { with (oo) setAttribute("fill",vv);
      return;
    }
    if (aa=="innerText") 
    { oo.firstChild.replaceData(0,108,vv);
      return;
    }	
    with (oo) setAttribute(aa,vv);
  }
  else
  { if (aa=="fill") oo.fillcolor=vv;
    if (aa=="stroke") oo.strokecolor=vv;
    if (aa=="stroke-width") oo.strokeweight=vv;
    if (aa=="visibility") oo.style.visibility=vv;
    if (aa=="from") oo.from=vv;
    if (aa=="to") oo.to=vv;
    if (aa=="x") oo.style.left=vv;
    if (aa=="y") oo.style.top=vv;  
    if (aa=="d") oo.path=vv;
    if (aa=="color") oo.style.color=vv;   
    if (aa=="innerText") oo.innerText=vv;    
  }  	
}	
function _BoundingBoxDraw()
{ var ii, jj, xx, yy, vv, gg, ff, pp=new Vector(0,0,0);
  var uu = useSVG ? 0 : 1;
//z plane
  if ((this.Min.x<this.Max.x)&&(this.Min.y<this.Max.y))
  { if (this.Parent.Th<0) pp.z=this.Min.z;
    else pp.z=this.Max.z;
    pp.x=this.Min.x; pp.y=this.Min.y;
    vv=this.Parent.ScreenPos(pp);
    if (useSVG) ss="M "+parseInt(vv.x)+" "+parseInt(vv.y)+" ";
    else ss="m "+parseInt(vv.x)+","+parseInt(vv.y)+" l";
    pp.x=this.Max.x;
    vv=this.Parent.ScreenPos(pp);
    if (useSVG)  ss+="L "+parseInt(vv.x)+" "+parseInt(vv.y)+" ";
    else ss+=" "+parseInt(vv.x)+","+parseInt(vv.y)+",";
    pp.y=this.Max.y;
    vv=this.Parent.ScreenPos(pp);
    if (useSVG) ss+="L "+parseInt(vv.x)+" "+parseInt(vv.y)+" ";
    else ss+=" "+parseInt(vv.x)+","+parseInt(vv.y)+",";
    pp.x=this.Min.x;
    vv=this.Parent.ScreenPos(pp);
    if (useSVG) ss+="L "+parseInt(vv.x)+" "+parseInt(vv.y)+" z";
    else ss+=" "+parseInt(vv.x)+","+parseInt(vv.y)+" x e";
    _OAV(this.Plane[0],"d",ss);
    pp.x=0; pp.y=0; pp.z=1;
    if (this.Parent.Th<0)
      _OAV(this.Plane[0],"fill",this.Parent.GetColor(this.FillColor, this.FillColor, pp, this.Min));
    else
      _OAV(this.Plane[0],"fill",this.Parent.GetColor(this.FillColor, this.FillColor, pp, this.Max));
    _OAV(this.Plane[0],"stroke",this.StrokeColor);
    _OAV(this.Plane[0],"stroke-width",parseInt(this.StrokeWeight));
    _OAV(this.Plane[0],"visibility","visible");
    if (this.Parent.Th<0) pp.z=this.Min.z;
    else pp.z=this.Max.z;

    gg=_GetGrid(this.Min.x/this.Parent.Zoom.x, this.Max.x/this.Parent.Zoom.x, this.Scale.x);
    if (this.GridDelta.x!=0) gg[1]=this.GridDelta.x;
    ii=0;
    for (jj=gg[2]; jj>=gg[0]; jj-=gg[1])
    { pp.x=jj*this.Parent.Zoom.x;
      pp.y=this.Min.y; vv=this.Parent.ScreenPos(pp);
      _OAV(this.Line[0][ii],"from",parseInt(vv.x)+","+parseInt(vv.y));
      xx=vv.x; yy=vv.y;
      pp.y=this.Max.y; vv=this.Parent.ScreenPos(pp);
      _OAV(this.Line[0][ii],"to",parseInt(vv.x)+","+parseInt(vv.y));
      _OAV(this.Line[0][ii],"stroke",this.StrokeColor);
      _OAV(this.Line[0][ii],"visibility","visible");
      if (this.Parent.Fi>=180)
      { _OAV(this.Text[0][ii],"x",Math.floor(xx+(vv.x-xx)*1.06)-50*uu);
        _OAV(this.Text[0][ii],"y",Math.floor(yy+(vv.y-yy)*1.06)-7*uu);
      }
      else
      { _OAV(this.Text[0][ii],"x",Math.floor(vv.x+(xx-vv.x)*1.06)-50*uu);
        _OAV(this.Text[0][ii],"y",Math.floor(vv.y+(yy-vv.y)*1.06)-7*uu);
      }
      _OAV(this.Text[0][ii],"color",this.StrokeColor);
      _OAV(this.Text[0][ii],"visibility","visible");
      if ((ii==1)&&(this.Label.x)) _OAV(this.Text[0][ii],"innerText",this.Label.x);
      else 
      { if (isNaN(this.Scale.x)) 
        { if (this.Scale.x.substr(0,9)=="function ")
          { ff=eval("window."+this.Scale.x.substr(9));
            if (ff) _OAV(this.Text[0][ii],"innerText",ff(_ScaleString(jj,gg[1])));
          }
          else _OAV(this.Text[0][ii],"innerText",_ScaleString(jj,gg[1])+this.Scale.x);
        }
        else
        { if (this.Scale.x<1) _OAV(this.Text[0][ii],"innerText","");
          if (this.Scale.x==1) _OAV(this.Text[0][ii],"innerText",_ScaleString(jj,gg[1]));
          if (this.Scale.x>1) _OAV(this.Text[0][ii],"innerText",_DateFormat(jj, gg[1], this.Scale.x));
        }
      }
      ii++;
    }
    if (this.Min.z<this.Max.z)
    { if ((this.Parent.Fi<90)||(this.Parent.Fi>=270)) 
      { if (this.Min.x/this.Parent.Zoom.x>gg[0]-gg[1]/3) _OAV(this.Text[0][ii-1],"innerText",""); }
      else
      { if (this.Max.x/this.Parent.Zoom.x<gg[2]+gg[1]/3) _OAV(this.Text[0][0],"innerText",""); }
    }
    while(ii<11)
    { _OAV(this.Line[0][ii],"visibility","hidden");
      _OAV(this.Text[0][ii],"visibility","hidden");  
      ii++;
    }

    gg=_GetGrid(this.Min.y/this.Parent.Zoom.y, this.Max.y/this.Parent.Zoom.y, this.Scale.y);
    if (this.GridDelta.y!=0) gg[1]=this.GridDelta.y;
    ii=0;
    for (jj=gg[2]; jj>=gg[0]; jj-=gg[1])
    { pp.y=jj*this.Parent.Zoom.y;
      pp.x=this.Min.x; vv=this.Parent.ScreenPos(pp);
      _OAV(this.Line[1][ii],"from",parseInt(vv.x)+","+parseInt(vv.y));
      xx=vv.x; yy=vv.y;
      pp.x=this.Max.x; vv=this.Parent.ScreenPos(pp);
      _OAV(this.Line[1][ii],"to",parseInt(vv.x)+","+parseInt(vv.y));
      _OAV(this.Line[1][ii],"stroke",this.StrokeColor);
      _OAV(this.Line[1][ii],"visibility","visible");
      if ((this.Parent.Fi<90)||(this.Parent.Fi>=270))
      { _OAV(this.Text[1][ii],"x",Math.floor(xx+(vv.x-xx)*1.06)-50*uu);
        _OAV(this.Text[1][ii],"y",Math.floor(yy+(vv.y-yy)*1.06)-7*uu);
      }
      else
      { _OAV(this.Text[1][ii],"x",Math.floor(vv.x+(xx-vv.x)*1.06)-50*uu);
        _OAV(this.Text[1][ii],"y",Math.floor(vv.y+(yy-vv.y)*1.06)-7*uu);
      }
      _OAV(this.Text[1][ii],"color",this.StrokeColor);
      _OAV(this.Text[1][ii],"visibility","visible");
      if ((ii==1)&&(this.Label.y)) _OAV(this.Text[1][ii],"innerText",this.Label.y);
      else 
      { if (isNaN(this.Scale.y))
        { if (this.Scale.y.substr(0,9)=="function ")
          { ff=eval("window."+this.Scale.y.substr(9));
            if (ff) _OAV(this.Text[1][ii],"innerText",ff(_ScaleString(jj,gg[1])));
          }
          else _OAV(this.Text[1][ii],"innerText",_ScaleString(jj,gg[1])+this.Scale.y);
        }      
        else
        { if (this.Scale.y<1) _OAV(this.Text[1][ii],"innerText","");
          if (this.Scale.y==1) _OAV(this.Text[1][ii],"innerText",_ScaleString(jj,gg[1]));
          if (this.Scale.y>1) _OAV(this.Text[1][ii],"innerText",_DateFormat(jj, gg[1], this.Scale.y));
        }
      }
      ii++;
    }
    if (this.Min.z<this.Max.z)
    { if (this.Parent.Fi>=180)
      { if (this.Min.y/this.Parent.Zoom.y>gg[0]-gg[1]/3) _OAV(this.Text[1][ii-1],"innerText",""); }
      else
      { if (this.Max.y/this.Parent.Zoom.y<gg[2]+gg[1]/3) _OAV(this.Text[1][0],"innerText",""); }
    }
    while(ii<11)
    { _OAV(this.Line[1][ii],"visibility","hidden");
      _OAV(this.Text[1][ii],"visibility","hidden");  
      ii++;
    }
  }
  else
  { _OAV(this.Plane[0],"visibility","hidden");
    for (ii=0; ii<11; ii++) _OAV(this.Line[0][ii],"visibility","hidden");
    for (ii=0; ii<11; ii++) _OAV(this.Line[1][ii],"visibility","hidden");
  }
//y plane
  if ((this.Min.x<this.Max.x)&&(this.Min.z<this.Max.z))
  { if (this.Parent.Fi>=180) pp.y=this.Min.y;
    else pp.y=this.Max.y;
    pp.x=this.Min.x; pp.z=this.Min.z;
    vv=this.Parent.ScreenPos(pp);
    if (useSVG) ss="M "+parseInt(vv.x)+" "+parseInt(vv.y)+" ";
    else ss="m "+parseInt(vv.x)+","+parseInt(vv.y)+" l";
    pp.x=this.Max.x;
    vv=this.Parent.ScreenPos(pp);
    if (useSVG) ss+="L "+parseInt(vv.x)+" "+parseInt(vv.y)+" ";
    else ss+=" "+parseInt(vv.x)+","+parseInt(vv.y)+",";
    pp.z=this.Max.z;
    vv=this.Parent.ScreenPos(pp);
    if (useSVG) ss+="L "+parseInt(vv.x)+" "+parseInt(vv.y)+" ";
    else ss+=" "+parseInt(vv.x)+","+parseInt(vv.y)+",";
    pp.x=this.Min.x;
    vv=this.Parent.ScreenPos(pp);
    if (useSVG) ss+="L "+parseInt(vv.x)+" "+parseInt(vv.y)+" z";
    else ss+=" "+parseInt(vv.x)+","+parseInt(vv.y)+" x e";
    _OAV(this.Plane[1],"d",ss);
    pp.x=0; pp.y=1; pp.z=0;
    if (this.Parent.Fi>=180)
      _OAV(this.Plane[1],"fill",this.Parent.GetColor(this.FillColor, this.FillColor, pp, this.Min));
    else
      _OAV(this.Plane[1],"fill",this.Parent.GetColor(this.FillColor, this.FillColor, pp, this.Max));
    _OAV(this.Plane[1],"stroke",this.StrokeColor);
    _OAV(this.Plane[1],"stroke-width",parseInt(this.StrokeWeight));
    _OAV(this.Plane[1],"visibility","visible");
    if (this.Parent.Fi>=180) pp.y=this.Min.y;
    else pp.y=this.Max.y;

    gg=_GetGrid(this.Min.x/this.Parent.Zoom.x, this.Max.x/this.Parent.Zoom.x, this.Scale.x);
    if (this.GridDelta.x!=0) gg[1]=this.GridDelta.x;
    ii=0;
    for (jj=gg[2]; jj>=gg[0]; jj-=gg[1])
    { pp.x=jj*this.Parent.Zoom.x;
      pp.z=this.Min.z; vv=this.Parent.ScreenPos(pp);
      _OAV(this.Line[2][ii],"from",parseInt(vv.x)+","+parseInt(vv.y));
      xx=vv.x; yy=vv.y;
      pp.z=this.Max.z; vv=this.Parent.ScreenPos(pp);
      _OAV(this.Line[2][ii],"to",parseInt(vv.x)+","+parseInt(vv.y));
      _OAV(this.Line[2][ii],"stroke",this.StrokeColor);
      _OAV(this.Line[2][ii],"visibility","visible");
      if (this.Parent.Th<0)
      { _OAV(this.Text[2][ii],"x",Math.floor(xx+(vv.x-xx)*1.06)-50*uu);
        _OAV(this.Text[2][ii],"y",Math.floor(yy+(vv.y-yy)*1.06)-7*uu);
      }
      else
      { _OAV(this.Text[2][ii],"x",Math.floor(vv.x+(xx-vv.x)*1.06)-50*uu);
        _OAV(this.Text[2][ii],"y",Math.floor(vv.y+(yy-vv.y)*1.06)-7*uu);
      }
      _OAV(this.Text[2][ii],"color",this.StrokeColor);
      _OAV(this.Text[2][ii],"visibility","visible");
      if ((ii==1)&&(this.Label.x)) _OAV(this.Text[2][ii],"innerText",this.Label.x);
      else 
      { if (isNaN(this.Scale.x))
        { if (this.Scale.x.substr(0,9)=="function ")
          { ff=eval("window."+this.Scale.x.substr(9));
            if (ff) _OAV(this.Text[2][ii],"innerText",ff(_ScaleString(jj,gg[1])));
          }
          else _OAV(this.Text[2][ii],"innerText",_ScaleString(jj,gg[1])+this.Scale.x);
        }      
        else
        { if (this.Scale.x<1) _OAV(this.Text[2][ii],"innerText","");
          if (this.Scale.x==1) _OAV(this.Text[2][ii],"innerText",_ScaleString(jj,gg[1]));
          if (this.Scale.x>1) _OAV(this.Text[2][ii],"innerText",_DateFormat(jj, gg[1], this.Scale.x));
        }
      }
      ii++;
    }
    if (this.Min.y<this.Max.y)
    { if ((this.Parent.Fi<90)||(this.Parent.Fi>=270))
      { if (this.Min.x/this.Parent.Zoom.x>gg[0]-gg[1]/3) _OAV(this.Text[2][ii-1],"innerText",""); }
      else
      { if (this.Max.x/this.Parent.Zoom.x<gg[2]+gg[1]/3) _OAV(this.Text[2][0],"innerText",""); }
    }
    while(ii<11)
    { _OAV(this.Line[2][ii],"visibility","hidden");
      _OAV(this.Text[2][ii],"visibility","hidden");  
      ii++;
    }

    gg=_GetGrid(this.Min.z/this.Parent.Zoom.z, this.Max.z/this.Parent.Zoom.z, this.Scale.z);
    if (this.GridDelta.z!=0) gg[1]=this.GridDelta.z;
    ii=0;
    for (jj=gg[2]; jj>=gg[0]; jj-=gg[1])
    { pp.z=jj*this.Parent.Zoom.z;
      pp.x=this.Min.x; vv=this.Parent.ScreenPos(pp);
      _OAV(this.Line[3][ii],"from",parseInt(vv.x)+","+parseInt(vv.y));
      xx=vv.x; yy=vv.y;
      pp.x=this.Max.x; vv=this.Parent.ScreenPos(pp);
      _OAV(this.Line[3][ii],"to",parseInt(vv.x)+","+parseInt(vv.y));
      _OAV(this.Line[3][ii],"stroke",this.StrokeColor);
      _OAV(this.Line[3][ii],"visibility","visible");
      if ((this.Parent.Fi<90)||(this.Parent.Fi>=270))
      { _OAV(this.Text[3][ii],"x",Math.floor(xx+(vv.x-xx)*1.06)-50*uu);
        _OAV(this.Text[3][ii],"y",Math.floor(yy+(vv.y-yy)*1.06)-7*uu);
      }
      else
      { _OAV(this.Text[3][ii],"x",Math.floor(vv.x+(xx-vv.x)*1.06)-50*uu);
        _OAV(this.Text[3][ii],"y",Math.floor(vv.y+(yy-vv.y)*1.06)-7*uu);
      }
      _OAV(this.Text[3][ii],"color",this.StrokeColor);
      _OAV(this.Text[3][ii],"visibility","visible");
      if ((ii==1)&&(this.Label.z)) _OAV(this.Text[3][ii],"innerText",this.Label.z);
      else 
      { if (isNaN(this.Scale.z)) 
        { if (this.Scale.z.substr(0,9)=="function ")
          { ff=eval("window."+this.Scale.z.substr(9));
            if (ff) _OAV(this.Text[3][ii],"innerText",ff(_ScaleString(jj,gg[1])));
          }
          else _OAV(this.Text[3][ii],"innerText",_ScaleString(jj,gg[1])+this.Scale.z);
        }      
        else
        { if (this.Scale.z<1) _OAV(this.Text[3][ii],"innerText","");
          if (this.Scale.z==1) _OAV(this.Text[3][ii],"innerText",_ScaleString(jj,gg[1]));
          if (this.Scale.z>1) _OAV(this.Text[3][ii],"innerText",_DateFormat(jj, gg[1], this.Scale.z));
        }
      }
      ii++;
    }
    if (this.Min.y<this.Max.y)
    { if (this.Parent.Th<0)
      { if (this.Min.z/this.Parent.Zoom.z>gg[0]-gg[1]/3) _OAV(this.Text[3][ii-1],"innerText",""); }
      else
      { if (this.Max.z/this.Parent.Zoom.z<gg[2]+gg[1]/3) _OAV(this.Text[3][0],"innerText",""); }
    }
    while(ii<11)
    { _OAV(this.Line[3][ii],"visibility","hidden");
      _OAV(this.Text[3][ii],"visibility","hidden");  
      ii++;
    }
  }
  else
  { _OAV(this.Plane[1],"visibility","hidden");
    for (ii=0; ii<11; ii++) _OAV(this.Line[2][ii],"visibility","hidden");
    for (ii=0; ii<11; ii++) _OAV(this.Line[3][ii],"visibility","hidden");
  }
//x plane
  if ((this.Min.y<this.Max.y)&&(this.Min.z<this.Max.z))
  { if ((this.Parent.Fi<90)||(this.Parent.Fi>=270)) pp.x=this.Min.x;
    else pp.x=this.Max.x;
    pp.y=this.Min.y; pp.z=this.Min.z;
    vv=this.Parent.ScreenPos(pp);
    if (useSVG) ss="M "+parseInt(vv.x)+" "+parseInt(vv.y)+" ";
    else ss="m "+parseInt(vv.x)+","+parseInt(vv.y)+" l";
    pp.y=this.Max.y;
    vv=this.Parent.ScreenPos(pp);
    if (useSVG) ss+="L "+parseInt(vv.x)+" "+parseInt(vv.y)+" ";
    else ss+=" "+parseInt(vv.x)+","+parseInt(vv.y)+",";
    pp.z=this.Max.z;
    vv=this.Parent.ScreenPos(pp);
    if (useSVG) ss+="L "+parseInt(vv.x)+" "+parseInt(vv.y)+" ";
    else ss+=" "+parseInt(vv.x)+","+parseInt(vv.y)+",";
    pp.y=this.Min.y;
    vv=this.Parent.ScreenPos(pp);
    if (useSVG) ss+="L "+parseInt(vv.x)+" "+parseInt(vv.y)+" z";
    else ss+=" "+parseInt(vv.x)+","+parseInt(vv.y)+" x e";
    _OAV(this.Plane[2],"d",ss);
    pp.x=1; pp.y=0; pp.z=0;
    if ((this.Parent.Fi<90)||(this.Parent.Fi>=270))
      _OAV(this.Plane[2],"fill",this.Parent.GetColor(this.FillColor, this.FillColor, pp, this.Min));
    else
      _OAV(this.Plane[2],"fill",this.Parent.GetColor(this.FillColor, this.FillColor, pp, this.Max));
    _OAV(this.Plane[2],"stroke",this.StrokeColor);
    _OAV(this.Plane[2],"stroke-width",parseInt(this.StrokeWeight));
    _OAV(this.Plane[2],"visibility","visible");
    if ((this.Parent.Fi<90)||(this.Parent.Fi>=270)) pp.x=this.Min.x;
    else pp.x=this.Max.x;

    gg=_GetGrid(this.Min.y/this.Parent.Zoom.y, this.Max.y/this.Parent.Zoom.y, this.Scale.y);
    if (this.GridDelta.y!=0) gg[1]=this.GridDelta.y;
    ii=0;
    for (jj=gg[2]; jj>=gg[0]; jj-=gg[1])
    { pp.y=jj*this.Parent.Zoom.y;
      pp.z=this.Min.z; vv=this.Parent.ScreenPos(pp);
      _OAV(this.Line[4][ii],"from",parseInt(vv.x)+","+parseInt(vv.y));
      xx=vv.x; yy=vv.y;
      pp.z=this.Max.z; vv=this.Parent.ScreenPos(pp);
      _OAV(this.Line[4][ii],"to",parseInt(vv.x)+","+parseInt(vv.y));
      _OAV(this.Line[4][ii],"stroke",this.StrokeColor);
      _OAV(this.Line[4][ii],"visibility","visible");
      if (this.Parent.Th<0)
      { _OAV(this.Text[4][ii],"x",Math.floor(xx+(vv.x-xx)*1.06)-50*uu);
        _OAV(this.Text[4][ii],"y",Math.floor(yy+(vv.y-yy)*1.06)-7*uu);
      }
      else
      { _OAV(this.Text[4][ii],"x",Math.floor(vv.x+(xx-vv.x)*1.06)-50*uu);
        _OAV(this.Text[4][ii],"y",Math.floor(vv.y+(yy-vv.y)*1.06)-7*uu);
      }
      _OAV(this.Text[4][ii],"color",this.StrokeColor);
      _OAV(this.Text[4][ii],"visibility","visible");
      if ((ii==1)&&(this.Label.y)) _OAV(this.Text[4][ii],"innerText",this.Label.y);
      else 
      { if (isNaN(this.Scale.y)) 
        { if (this.Scale.y.substr(0,9)=="function ")
          { ff=eval("window."+this.Scale.y.substr(9));
            if (ff) _OAV(this.Text[4][ii],"innerText",ff(_ScaleString(jj,gg[1])));
          }
          else _OAV(this.Text[4][ii],"innerText",_ScaleString(jj,gg[1])+this.Scale.y);
        }      
        else
        { if (this.Scale.y<1) _OAV(this.Text[4][ii],"innerText","");
          if (this.Scale.y==1) _OAV(this.Text[4][ii],"innerText",_ScaleString(jj,gg[1]));
          if (this.Scale.y>1) _OAV(this.Text[4][ii],"innerText",_DateFormat(jj, gg[1], this.Scale.y));
        }
      }
      ii++;
    }
    if (this.Min.y<this.Max.y)
    { if (this.Parent.Fi>=180)
      { if (this.Min.y/this.Parent.Zoom.y>gg[0]-gg[1]/3) _OAV(this.Text[4][ii-1],"innerText",""); }
      else
      { if (this.Max.y/this.Parent.Zoom.y<gg[2]+gg[1]/3) _OAV(this.Text[4][0],"innerText",""); }
    }
    while(ii<11)
    { _OAV(this.Line[4][ii],"visibility","hidden");
      _OAV(this.Text[4][ii],"visibility","hidden");  
      ii++;
    }

    gg=_GetGrid(this.Min.z/this.Parent.Zoom.z, this.Max.z/this.Parent.Zoom.z, this.Scale.z);
    if (this.GridDelta.z!=0) gg[1]=this.GridDelta.z;
    ii=0;
    for (jj=gg[2]; jj>=gg[0]; jj-=gg[1])
    { pp.z=jj*this.Parent.Zoom.z;
      pp.y=this.Min.y; vv=this.Parent.ScreenPos(pp);
      _OAV(this.Line[5][ii],"from",parseInt(vv.x)+","+parseInt(vv.y));
      xx=vv.x; yy=vv.y;
      pp.y=this.Max.y; vv=this.Parent.ScreenPos(pp);
      _OAV(this.Line[5][ii],"to",parseInt(vv.x)+","+parseInt(vv.y));
      _OAV(this.Line[5][ii],"stroke",this.StrokeColor);
      _OAV(this.Line[5][ii],"visibility","visible");
      if (this.Parent.Fi>=180)
      { _OAV(this.Text[5][ii],"x",Math.floor(xx+(vv.x-xx)*1.06)-50*uu);
        _OAV(this.Text[5][ii],"y",Math.floor(yy+(vv.y-yy)*1.06)-7*uu);
      }
      else
      { _OAV(this.Text[5][ii],"x",Math.floor(vv.x+(xx-vv.x)*1.06)-50*uu);
        _OAV(this.Text[5][ii],"y",Math.floor(vv.y+(yy-vv.y)*1.06)-7*uu);
      }
      _OAV(this.Text[5][ii],"color",this.StrokeColor);
      _OAV(this.Text[5][ii],"visibility","visible");
      if ((ii==1)&&(this.Label.z)) _OAV(this.Text[5][ii],"innerText",this.Label.z);
      else 
      { if (isNaN(this.Scale.z)) 
        { if (this.Scale.z.substr(0,9)=="function ")
          { ff=eval("window."+this.Scale.z.substr(9));
            if (ff) _OAV(this.Text[5][ii],"innerText",ff(_ScaleString(jj,gg[1])));
          }
          else _OAV(this.Text[5][ii],"innerText",_ScaleString(jj,gg[1])+this.Scale.z);
        }   
        else
        { if (this.Scale.z<1) _OAV(this.Text[5][ii],"innerText","");
          if (this.Scale.z==1) _OAV(this.Text[5][ii],"innerText",_ScaleString(jj,gg[1]));
          if (this.Scale.z>1) _OAV(this.Text[5][ii],"innerText",_DateFormat(jj, gg[1], this.Scale.z));
        }
      }
      ii++;
    }
    if (this.Min.x<this.Max.x)
    { if (this.Parent.Th<0)
      { if (this.Min.z/this.Parent.Zoom.z>gg[0]-gg[1]/3) _OAV(this.Text[5][ii-1],"innerText",""); }
      else
      { if (this.Max.z/this.Parent.Zoom.z<gg[2]+gg[1]/3) _OAV(this.Text[5][0],"innerText",""); }
    }
    while(ii<11)
    { _OAV(this.Line[5][ii],"visibility","hidden");
      _OAV(this.Text[5][ii],"visibility","hidden");  
      ii++;
    }
  }
  else
  { _OAV(this.Plane[2],"visibility","hidden");
    for (ii=0; ii<11; ii++) _OAV(this.Line[4][ii],"visibility","hidden");
    for (ii=0; ii<11; ii++) _OAV(this.Line[5][ii],"visibility","hidden");
  }
}
function _DateInterval(vv)
{ var bb=140*24*60*60*1000; //140 days
  if (vv>=bb) //140 days < 5 months
  { bb=8766*60*60*1000;//1 year
    if (vv<bb) //1 year 
      return(bb/12); //1 month
    if (vv<bb*2) //2 years 
      return(bb/6); //2 month
    if (vv<bb*5/2) //2.5 years
      return(bb/4); //3 month
    if (vv<bb*5) //5 years
      return(bb/2); //6 month
    if (vv<bb*10) //10 years
      return(bb); //1 year
    if (vv<bb*20) //20 years
      return(bb*2); //2 years
    if (vv<bb*50) //50 years
      return(bb*5); //5 years
    if (vv<bb*100) //100 years
      return(bb*10); //10 years
    if (vv<bb*200) //200 years
      return(bb*20); //20 years
    if (vv<bb*500) //500 years
      return(bb*50); //50 years
    return(bb*100); //100 years
  }
  bb/=2; //70 days
  if (vv>=bb) return(bb/5); //14 days
  bb/=2; //35 days
  if (vv>=bb) return(bb/5); //7 days
  bb/=7; bb*=4; //20 days
  if (vv>=bb) return(bb/5); //4 days
  bb/=2; //10 days
  if (vv>=bb) return(bb/5); //2 days
  bb/=2; //5 days
  if (vv>=bb) return(bb/5); //1 day
  bb/=2; //2.5 days
  if (vv>=bb) return(bb/5); //12 hours
  bb*=3; bb/=5; //1.5 day
  if (vv>=bb) return(bb/6); //6 hours
  bb/=2; //18 hours
  if (vv>=bb) return(bb/6); //3 hours
  bb*=2; bb/=3; //12 hours
  if (vv>=bb) return(bb/6); //2 hours
  bb/=2; //6 hours
  if (vv>=bb) return(bb/6); //1 hour
  bb/=2; //3 hours
  if (vv>=bb) return(bb/6); //30 mins
  bb/=2; //1.5 hours
  if (vv>=bb) return(bb/6); //15 mins
  bb*=2; bb/=3; //1 hour
  if (vv>=bb) return(bb/6); //10 mins
  bb/=3; //20 mins
  if (vv>=bb) return(bb/4); //5 mins
  bb/=2; //10 mins
  if (vv>=bb) return(bb/5); //2 mins
  bb/=2; //5 mins
  if (vv>=bb) return(bb/5); //1 min
  bb*=3; bb/=2; //3 mins
  if (vv>=bb) return(bb/6); //30 secs
  bb/=2; //1.5 mins
  if (vv>=bb) return(bb/6); //15 secs
  bb*=2; bb/=3; //1 min
  if (vv>=bb) return(bb/6); //10 secs
  bb/=3; //20 secs
  if (vv>=bb) return(bb/4); //5 secs
  bb/=2; //10 secs
  if (vv>=bb) return(bb/5); //2 secs
  return(bb/10); //1 sec
}
function _DateFormat(vv, ii, ttype)
{ var yy, mm, dd, hh, nn, ss, vv_date=new Date(vv);
  Month=new Array("Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec");
  Weekday=new Array("Sun","Mon","Tue","Wed","Thu","Fri","Sat");
  if (ii>15*24*60*60*1000)
  { if (ii<365*24*60*60*1000)
    { vv_date.setTime(vv+15*24*60*60*1000);
      yy=vv_date.getYear()%100;
      if (yy<10) yy="0"+yy;
      mm=vv_date.getUTCMonth()+1;
      if (ttype==5) ;//You can add your own date format here
      if (ttype==4) return(Month[mm-1]);
      if (ttype==3) return(Month[mm-1]+" "+yy);
      return(mm+"/"+yy);
    }
    vv_date.setTime(vv+183*24*60*60*1000);
    yy=vv_date.getYear();
    return(yy);
  }
  vv_date.setTime(vv);
  mm=vv_date.getUTCMonth()+1;
  dd=vv_date.getUTCDate();
  ww=vv_date.getUTCDay();
  hh=vv_date.getUTCHours();
  nn=vv_date.getUTCMinutes(); 
  ss=vv_date.getUTCSeconds();
  if (ii>=86400000)//1 day
  { if (ttype==5) ;//You can add your own date format here
    if (ttype==4) return(Weekday[ww]);
    if (ttype==3) return(mm+"/"+dd);
    return(dd+"."+mm+".");
  }
  if (ii>=21600000)//6 hours 
  { if (hh==0) 
    { if (ttype==5) ;//You can add your own date format here
      if (ttype==4) return(Weekday[ww]);
      if (ttype==3) return(mm+"/"+dd);
      return(dd+"."+mm+".");
    }
    else
    { if (ttype==5) ;//You can add your own date format here
      if (ttype==4) return((hh<=12) ? hh+"am" : hh%12+"pm");
      if (ttype==3) return((hh<=12) ? hh+"am" : hh%12+"pm");
      return(hh+":00");
    }
  }
  if (ii>=60000)//1 min
  { if (nn<10) nn="0"+nn;
    if (ttype==5) ;//You can add your own date format here
    if (ttype==4) return((hh<=12) ? hh+"."+nn+"am" : hh%12+"."+nn+"pm");
    if (nn=="00") nn="";
    else nn=":"+nn;
    if (ttype==3) return((hh<=12) ? hh+nn+"am" : hh%12+nn+"pm");
    if (nn=="") nn=":00";
    return(hh+nn);
  }
  if (ss<10) ss="0"+ss;
  return(nn+":"+ss);
}
function _GetGrid(mmin, mmax, sscale)
{ var ii,jj,xx,rr,dd,xxr;
  gg=new Array(3);
  if ((sscale<=1)||(isNaN(sscale)))
  { dd=(mmax-mmin);
    rr=1;
    while (Math.abs(dd)>=100) { dd/=10; rr*=10; }
    while (Math.abs(dd)<10) { dd*=10; rr/=10; }
    if (Math.abs(dd)>=50) dd=10*rr;
    else
    { if (Math.abs(dd)>=20) dd=5*rr;
      else dd=2*rr;
    }
  }
  else dd=_DateInterval(mmax-mmin);
  xx=Math.floor(mmin/dd)*dd;
  ii=0;
  gg[1]=dd;
  for (jj=12; jj>=-1; jj--)
  { xxr=xx+jj*dd;
    if ((xxr>=mmin)&&(xxr<=mmax))
    { if (ii==0) gg[2]=xxr;
      gg[0]=xxr;
      ii++;
    }
  }
  return(gg);
}
function _ScaleString(jj,gg)
{ if (Math.abs(gg)>=1) return(jj);
  return(Math.round(10*jj/gg)/Math.round(10/gg))
}