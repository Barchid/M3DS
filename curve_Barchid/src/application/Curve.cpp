#include "Curve.h"
#include <cstdlib>
#include <iostream>
#include <sstream>

#include "GLTool.h"

/**
@file
@author F. Aubert
*/

using namespace p3d;
using namespace std;

/**
* Evaluation de la Curve de Bézier en $t$
**/
Vector3 Curve::evalBezier(double t) {
    Vector3 result;
    if (nbPoint()>1) {
        vector<Vector3> castel=_pts;      // tableau de points sur lesquels on applique deCasteljau
                                          // on recopie les points de controles dans le tableau castel (castel est donc initialisé avec la première ligne de l'algo triangulaire).

        // De CastelJau est en fait une suite d'interpolations linéaires avec un t fixé
        for (int n = nbPoint(); n > 1; n--) { // on débute par la taille initiale des points
            for (int k = 0; k < n-1; ++k) {
                // Application de la formule de récurrence de casteljau
                castel[i] = (1 - t) * castel[i] + t * castel[i+1];
            }
        }


        // le point de la courbe en t doit se trouver dans castel[0] à la fin de l'algo
        result=castel[0];
    }
    return result;
}


/**
* evalCubicBezier est une alternative à evalBezier mais uniquement pour les cubiques.
**/
Vector3 Curve::evalCubicBezier(double t) {
    if (_pts.size()!=4) throw Error("Curve should have 4 control points",__LINE__,__FILE__);

    Vector3 res;
    float mat[16]={-1,3,-3,1,3,-6,3,0,-3,3,0,0,1,0,0,0}; // bezier matrix

    vector<Vector3> q;
    q.resize(4);
    for(int i=0;i<4;i++) {
        q[i].set(0,0,0);
        for(int j=0;j<4;j++) {
            q[i]+=mat[i*4+j]*point(j);
        }
    }
    res=t*t*t*(-point(0)+3*point(1)-3*point(2)+point(3))+
        t*t*(3*point(0)-6*point(1)+3*point(2))+
        t*(-3*point(0)+3*point(1))+
        point(0);

    return res;
}

Vector3 Curve::evalCubicVelocity(double t) {
  if (_pts.size()!=4) throw Error("Curve should have 4 control points",__LINE__,__FILE__);
  Vector3 res;


  return res;

}

Vector3 Curve::evalCubicAcceleration(double t) {
  if (_pts.size()!=4) throw Error("Curve should have 4 control points",__LINE__,__FILE__);

  Vector3 res;


  return res;

}



/** *********************************************************************************************************************** **/


Curve::Curve() {
    _pts.clear();
}

Curve::~Curve() {
}


void Curve::create(int nb) {
  _pts.resize(nb);
  _angle.resize(nb);
}

void Curve::point(int i,const Vector3 &p) {
    _pts[i]=p;
}



Matrix4 Curve::frame(double t) {
  Vector3 p=evalCubicBezier(t);
  Vector3 tt=evalCubicVelocity(t);
  Vector3 acc=evalCubicAcceleration(t);

  Vector3 B=tt.cross(acc);


  double r=1.0/(B.length()/(tt.length()*tt.length()*tt.length()));
  tt.normalize();
  Vector3 bb=normalize(B);
  Vector3 N=bb.cross(tt);

  Vector3 rN=r*N;

  Vector3 down=normalize(Vector3(0,1,0)+rN);

  Vector3 other=tt.cross(down);

  Matrix4 res;
  res.setFrame(p,tt,down,other);
      return res;
}

Matrix4 Curve::tbn(double t,p3d::Vector3 *previousB) {
  Vector3 p=evalCubicBezier(t);
  Vector3 tt=evalCubicVelocity(t);

  tt.normalize();
  Vector3 n=previousB->cross(tt);
  n.normalize();
  *previousB=normalize(tt.cross(n));

  Matrix4 res;
  res.setFrame(p,tt,*previousB,n);
  return res;
}


Vector3 Curve::rN(double t) {
  Vector3 tt=evalCubicVelocity(t);
  Vector3 acc=evalCubicAcceleration(t);

  Vector3 B=tt.cross(acc);


  double r=1.0/(B.length()/(tt.length()*tt.length()*tt.length()));
  tt.normalize();
  Vector3 bb=normalize(B);
  Vector3 N=bb.cross(tt);
  cout << r << endl;
  return r*N;
}

Vector3 Curve::evalCubicTierce(double t) {
  if (_pts.size()!=4) throw Error("Curve should have 4 control points",__LINE__,__FILE__);

  Vector3 res;


  return res;

}


void Curve::drawBezier() {
  if (nbPoint()<2) return;


    float pas=1.0/(100.0-1);
    float t=0;

    vector<Vector3> drawPts;

    for(int i=0;i<100;i++) {
        drawPts.push_back(evalBezier(t));
        t+=pas;
    }
    p3d::drawThickLineStrip(drawPts);
}


void Curve::drawControl() {
  vector<Vector3> linePts;
  glPointSize(10);
  p3d::shaderVertexAmbient();
  if (nbPoint()>0) {
    unsigned int i;
    for(i=0;i<nbPoint();++i) {
      linePts.push_back(point(i));
      p3d::draw(i,point(i)+Vector3(0.01,0.01,0));
    }
    p3d::drawPoints(linePts);
    p3d::drawLineStrip(linePts);
  }
}



void Curve::interactInsert(unsigned int i, const Vector3 &insertPoint) {
  _pts.insert(_pts.begin()+i,insertPoint);
  _angle.insert(_angle.begin()+i,0.0);
}












