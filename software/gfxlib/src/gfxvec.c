// Bitfire Graphics Library, 3D Vector Graphics
// mtrojer@arrownordic.com
// $WCREV$

#include <stdlib.h>
#include <math.h>
#include "myfixed.h"
#include "gfxvec.h"
#include "gfxlib.h"

#define MAX_X GL_XSIZE*GL_XBOARD
#define MAX_Y GL_YSIZE*GL_YBOARD

//----- angle and position of 3d object
INT_32 xangle,yangle,zangle;			// angles of rotation
INT_32 xpos,ypos,zpos;					// position of object in 3d

ObjectTYPE Object;						// one object

INT_32 NumOfSortedPolygons;				// number of sorted visible polygons
INT_32 PolygonOrderList[MAXPOLYGONS];	// list of polygon indices for qsorting

// --------------------------------------------------------------------------------------
static void RotatePoints(INT_16 distance)
{
  INT_32 i;
  fixed nx,ny,nz,cosxangle,sinxangle,cosyangle,sinyangle,coszangle,sinzangle;

  sinxangle=FLOAT_TO_FIXED(sin(xangle*360/MAXDEGREES * 3.14159265/180.0));
  cosxangle=FLOAT_TO_FIXED(cos(xangle*360/MAXDEGREES * 3.14159265/180.0));
  sinyangle=FLOAT_TO_FIXED(sin(yangle*360/MAXDEGREES * 3.14159265/180.0));
  cosyangle=FLOAT_TO_FIXED(cos(yangle*360/MAXDEGREES * 3.14159265/180.0));
  sinzangle=FLOAT_TO_FIXED(sin(zangle*360/MAXDEGREES * 3.14159265/180.0));
  coszangle=FLOAT_TO_FIXED(cos(zangle*360/MAXDEGREES * 3.14159265/180.0));

  // loop through all vertices in object
  for(i=0;i<Object.NumOfVertices;i++)
    {
      // make a pointer to the current vertex
      VertexTYPE *vert=&Object.Vertex[i];

      // rotate around the x-axis
      vert->wz=myfixMul(vert->oy, cosxangle) - myfixMul(vert->oz, sinxangle);
      vert->wy=myfixMul(vert->oy, sinxangle) + myfixMul(vert->oz, cosxangle);
      vert->wx=vert->ox;
      
      // rotate around the y-axis
      nx=myfixMul(vert->wx, cosyangle) - myfixMul(vert->wz, sinyangle);
      nz=myfixMul(vert->wx, sinyangle) + myfixMul(vert->wz, cosyangle);
      vert->wx=nx;
      vert->wz=nz;
      
      // rotate around the z-axis
      nx=myfixMul(vert->wx, coszangle) - myfixMul(vert->wy, sinzangle);
      ny=myfixMul(vert->wx, sinzangle) + myfixMul(vert->wy, coszangle);
      vert->wx=nx;
      vert->wy=ny;
      
      // project the 3-D coordinates to screen coordinates
      vert->sx=FIXED_TO_INT(myfixMul(myfixDiv(vert->wx+INT_TO_FIXED(xpos),vert->wz+INT_TO_FIXED(zpos)),INT_TO_FIXED(distance))) + MAX_X/2;
      vert->sy=FIXED_TO_INT(myfixMul(myfixDiv(vert->wy+INT_TO_FIXED(ypos),vert->wz+INT_TO_FIXED(zpos)),INT_TO_FIXED(distance))) + MAX_Y/2;
      vert->sz=FIXED_TO_INT(vert->wz+INT_TO_FIXED(zpos));
    }
}

// --------------------------------------------------------------------------------------
static void DrawObject(gl_col *c)
{
  INT_32 i,j;
  VertexTYPE *v0,*v1,*v2;
  gl_point p1,p2;

  MakePolygonList();
  SortPolygonList();


  for(i=0;i<NumOfSortedPolygons;i++)
    {
      for(j=0; j < Object.Polygon[PolygonOrderList[i]].NumOfVertices-2;j++)
	{
	  v0=&Object.Vertex[Object.Polygon[PolygonOrderList[i]].Vertex[0]];
	  v1=&Object.Vertex[Object.Polygon[PolygonOrderList[i]].Vertex[j+1]];
	  v2=&Object.Vertex[Object.Polygon[PolygonOrderList[i]].Vertex[j+2]];

	  p1.x = ((v0->sx+XOFFSET)>0) ? v0->sx+XOFFSET : 0;
	  p1.y = ((v0->sy+YOFFSET)>0) ? v0->sy+YOFFSET : 0;
	  p2.x = ((v1->sx+XOFFSET)>0)? v1->sx+XOFFSET : 0;
	  p2.y = ((v1->sy+YOFFSET)>0) ? v1->sy+YOFFSET : 0;
	  gl_line(&p1,&p2,c);

	  p1.x = ((v1->sx+XOFFSET)>0) ? v1->sx+XOFFSET : 0;
	  p1.y = ((v1->sy+YOFFSET)>0) ? v1->sy+YOFFSET : 0;
	  p2.x = ((v2->sx+XOFFSET)>0) ? v2->sx+XOFFSET : 0;
	  p2.y = ((v2->sy+YOFFSET)>0) ? v2->sy+YOFFSET : 0;
	  gl_line(&p1,&p2,c);
		
	  p1.x = ((v2->sx+XOFFSET)>0) ? v2->sx+XOFFSET : 0;
	  p1.y = ((v2->sy+YOFFSET)>0) ? v2->sy+YOFFSET : 0;
	  p2.x = ((v0->sx+XOFFSET)>0) ? v0->sx+XOFFSET : 0;
	  p2.y = ((v0->sy+YOFFSET)>0) ? v0->sy+YOFFSET : 0;
	  gl_line(&p1,&p2,c);
	}
    }
}

// --------------------------------------------------------------------------------------
void InitGVectors(const INT_16 *data, BOOL_8 setuptables)
{
  LoadObject(data);

  if (setuptables) {

    // initialize angle and position of object
    xangle=0;
    yangle=0;
    zangle=0;
    xpos=0;
    ypos=0;
    zpos=-1024;
  };
}

// --------------------------------------------------------------------------------------
void UpdateGVectors(INT_16 distance, gl_col *c)
{
  xangle+=4; if(xangle >= MAXDEGREES) xangle=0;
  yangle+=6; if(yangle >= MAXDEGREES) yangle=0;
  zangle+=8; if(zangle >= MAXDEGREES) zangle=0;

  RotatePoints(distance);
  DrawObject(c);
}

// --------------------------------------------------------------------------------------
static void MakePolygonList(void)
{
  INT_32 i,j;

  VertexTYPE *v0,*v1,*v2;

  j=0;
  for(i=0;i<Object.NumOfPolygons;i++)
    {
      v0=&Object.Vertex[Object.Polygon[i].Vertex[0]];
      v1=&Object.Vertex[Object.Polygon[i].Vertex[1]];
      v2=&Object.Vertex[Object.Polygon[i].Vertex[2]];

      // if expression results in a negative then polygon is visible
      if( ((v1->sx - v0->sx) * (v2->sy - v0->sy) - (v1->sy - v0->sy) * (v2->sx - v0->sx)) < 0 )
	{
	  PolygonOrderList[j++]=i;
	}
    }
  NumOfSortedPolygons=j;
}

// --------------------------------------------------------------------------------------
static void SortPolygonList(void)
{
  INT_32 i,j;
  INT_32 maxz,minz;
  PolygonTYPE *poly;

  // first find the distance of each polygon (from midpoint of max & min z's)
  for(i=0;i<Object.NumOfPolygons;i++)
    {

      poly=&Object.Polygon[i];
      minz=65535;
      maxz=-65536;
      for(j=0;j<poly->NumOfVertices;j++)
	{
	  if(Object.Vertex[poly->Vertex[j]].sz < minz)
	    {
	      minz=Object.Vertex[poly->Vertex[j]].sz;
	    }
	  if(Object.Vertex[poly->Vertex[j]].sz > maxz)
	    {
	      maxz=Object.Vertex[poly->Vertex[j]].sz;
	    }
	}
      // now calculate the distance
      poly->zcenter=(maxz+minz)<<1;
    }

  // qsort the polygons
  qsort(PolygonOrderList,NumOfSortedPolygons,sizeof(int),ComparePolygons);

  // all done the sorting process
}

// --------------------------------------------------------------------------------------
static int ComparePolygons(const void *a, const void *b)
{
  if( Object.Polygon[*(int *)a].zcenter < Object.Polygon[*(int *)b].zcenter )
    {
      return -1;
    }
  else if( Object.Polygon[*(int *)a].zcenter > Object.Polygon[*(int *)b].zcenter )
    {
      return +1;
    }
  else
    {
      return 0;
    }
}

// --------------------------------------------------------------------------------------
static void LoadObject(const INT_16 *data)
{
  INT_32 i,j;
  // short temp;

  Object.NumOfVertices=*data++;
  for(i=0;i < Object.NumOfVertices; i++)
    {
      Object.Vertex[i].ox=INT_TO_FIXED(*data++);
      Object.Vertex[i].oy=INT_TO_FIXED(*data++);
      Object.Vertex[i].oz=INT_TO_FIXED(*data++);
    }

  Object.NumOfPolygons=*data++;
  for(i=0; i < Object.NumOfPolygons; i++)
    {
      Object.Polygon[i].NumOfVertices=*data++;
      for(j=0; j< Object.Polygon[i].NumOfVertices; j++)
	{
	  Object.Polygon[i].Vertex[j]=*data++;
	}
      data++; // skip the color
    }
}
