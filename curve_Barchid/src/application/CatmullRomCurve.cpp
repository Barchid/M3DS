#include "GLTool.h"

#include "CatmullRomCurve.h"
#include "Vector3.h"

/**
@file
@author F. Aubert
*/


using namespace p3d;
using namespace std;


/**
* Tracé de la courbe d'interpolation
*/

void CatmullRomCurve::draw() {


  // A COMPLETER :
  //
  // doit tracer une courbe en reliant 2 points saisis successifs par une bézier cubique
  // nbPoint() vous donne le nombre de points saisis
  // on doit donc tracer nbPoint()-1 courbes de bézier cubiques : la ième courbe a pour points de controle point(i),intermediate(i,0),intermediate(i,1) et point(i+1)
  // il suffit donc d'affecter la variable "cubic" avec ces points (affectez les points avec le setter cubic.point(j,un_point), puis la tracer (en appelant simplement cubic.drawBezier() ).


  Curve cubic;
  cubic.create(4); // pour chaque courbe (boucle) il faudra donner les 4 points de la bézier cubique

  if (nbPoint()<2) return; // tracé uniquement si plus de 2 points saisis.



}


/**
*  Calcul des points intermédiaires pour obtenir une continuité C1 par Catmull-Rom
*/
void CatmullRomCurve::setup() {
  Vector3 parallele;

  // A COMPLETER : il s'agit d'initialiser les points intermédiaires (les points bleus) en appelant les setters intermediate(i,0,<un Vector3>) et intermediate(i,1,<un Vector3>) )
  // les points intermédiaires doivent être fixés par la méthode vue en cours (tangentes parallèles aux segments [point(i-1),point(i+1)]).

}

Matrix4 CatmullRomCurve::tbn(double tValue) {

  double tCubic;
  Curve c=cubic(tValue,&tCubic);

  Vector3 p=c.evalCubicBezier(tCubic);
  Vector3 v=c.evalCubicVelocity(tCubic);
  Vector3 a=c.evalCubicAcceleration(tCubic);

  Vector3 t=v;
  Vector3 b=v.cross(a);
  t.normalize();
  b.normalize();
  Vector3 n=b.cross(t);

  Matrix4 res;
  res.setFrame(p,t,b,n); // matrice de changement de repère => colonne 0 = t; colonne 1 = b; colonne 2 =n; colonne 3 = p


  return res;
}


/** *********************************************************************************************************** */
/** *********************************************************************************************************** */
/** *********************************************************************************************************** */
/** *********************************************************************************************************** */

CatmullRomCurve::CatmullRomCurve() {
  _intermediate.clear();
}

CatmullRomCurve::~CatmullRomCurve() {
}

void CatmullRomCurve::create(int nb) {
  Curve::create(nb);
  _intermediate.resize(2*(nb-1));
}

void CatmullRomCurve::interactInsert(unsigned int i, const Vector3 &insertP) {
    Curve::interactInsert(i/3+i%3,insertP);
    addIntermediate();
}


const Vector3 &CatmullRomCurve::intermediate(unsigned int i,unsigned int j) const {
  return _intermediate[2*i+j];
}

void CatmullRomCurve::intermediate(unsigned int i,unsigned int j,const Vector3 &p) {
  _intermediate[2*i+j]=p;
}

void CatmullRomCurve::addIntermediate() {
  int i=nbPoint()-2;
  if (i>=0) {
    if (_intermediate.size()!=(nbPoint()-2)*2) throw Error("incoherence between pts and intermediate",__LINE__,__FILE__);
    Vector3 p1=point(i)+0.33*(point(i+1)-point(i));
    Vector3 p2=point(i)+0.66*(point(i+1)-point(i));
    _intermediate.push_back(p1);
    _intermediate.push_back(p2);
  }
}



void CatmullRomCurve::drawTBN(double t) {
 if (nbPoint()<2) return;
 p3d::drawFrame(this->tbn(t),"",0.5);
}

void CatmullRomCurve::drawFrame(double t) {
  p3d::drawFrame(frame(t));
}


void CatmullRomCurve::drawPoint() {
  vector<Vector3> allPts;

  for(unsigned int i=0; i<nbPoint(); i++) {
    allPts.push_back(point(i));
    p3d::draw(i,point(i)+Vector3(0.02,0.02,0.0));
  }
  p3d::shaderVertexAmbient();
  p3d::draw(GL_POINTS,allPts);
}

vector<Vector3> *CatmullRomCurve::intermediatev() {
  return &_intermediate;
}

void CatmullRomCurve::drawControl(int mode) {
  if (nbPoint()>0) {
    if (mode>0) {
      p3d::uniformAmbient(1,0,0);
      if (mode==1) {
        drawPoint();
      }
      if (mode>1) {
        Curve::drawControl();
        if (mode>2) {
          p3d::uniformAmbient(0,0,1);
          for(unsigned int i=0; i<nbPoint()-1; i++) {
            p3d::drawLines(vector<Vector3>{point(i),intermediate(i,0),
                            intermediate(i,0),intermediate(i,1),
                            intermediate(i,1),point(i+1)});
            p3d::drawPoints(vector<Vector3>{intermediate(i,0),intermediate(i,1)});
          }
        }
      }
    }
  }
}

Curve CatmullRomCurve::cubic(unsigned int i) {
  Curve cubic;
  cubic.create(4);

  cubic.point(0,point(i));
  cubic.point(1,intermediate(i,0));
  cubic.point(2,intermediate(i,1));
  cubic.point(3,point(i+1));

  return cubic;
}


Curve CatmullRomCurve::cubic(double t,double *tCubic) {
  double intpart,fracpart;
  fracpart=modf(t*(nbPoint()-1.0),&intpart);
  double realT=fracpart;
  int whichCurve=int(intpart);


  Curve cubic;
  cubic.create(4);

  cubic.point(0,point(whichCurve));
  cubic.point(1,intermediate(whichCurve,0));
  cubic.point(2,intermediate(whichCurve,1));
  cubic.point(3,point(whichCurve+1));

  *tCubic=realT;

  return cubic;
}

Vector3 CatmullRomCurve::interpolateAcceleration(double t) {
  int cubicNumber;
  double tCubic;
  whichCurve(t,&cubicNumber,&tCubic);
  Vector3 acc1=_acceleration[cubicNumber];
  Vector3 acc2=_acceleration[cubicNumber==nbPoint()-1?0:cubicNumber+1];
  Vector3 acc=(1-tCubic)*acc1+tCubic*acc2;
  return acc;
}


double CatmullRomCurve::interpolateAngle(double t) {
  int cubicNumber;
  double tCubic;
  whichCurve(t,&cubicNumber,&tCubic);
  double acc1=_angle[cubicNumber];
  double acc2=_angle[(cubicNumber==nbPoint()-1)?0:cubicNumber+1];
  double acc=(1-tCubic)*acc1+tCubic*acc2;
  return acc;
}


void CatmullRomCurve::whichCurve(double t,int *cubicNumber,double *tCubic) {
  double intpart,fracpart;
  fracpart=modf(t*(nbPoint()-1.0),&intpart);
  *tCubic=fracpart;
  *cubicNumber=int(intpart);
}



Vector3 CatmullRomCurve::eval(double t) {
  double tCubic;
  Curve c=cubic(t,&tCubic);
  return c.evalCubicBezier(tCubic);
}

Vector3 CatmullRomCurve::rN(double t) {
  double tCubic;
  Curve c=cubic(t,&tCubic);
  return c.rN(tCubic);
}


Matrix4 CatmullRomCurve::frame(double t) {
  if (nbPoint()<2) return Matrix4::identity();
  double intpart,fracpart;
  fracpart=modf(t*(nbPoint()-1.0),&intpart);
  double realT=fracpart;
  int whichCurve=int(intpart);


  Curve cubic;
  cubic.create(4);
  Vector3 p1,p2;

  cubic.point(0,point(whichCurve));
  cubic.point(1,intermediate(whichCurve,0));
  cubic.point(2,intermediate(whichCurve,1));
  cubic.point(3,point(whichCurve+1));
  return cubic.frame(realT);//,_acceleration[whichCurve],_acceleration[whichCurve+1],&_oldB,&_flip);

}


Matrix4 CatmullRomCurve::tbnManual(double t) {
  double tCubic;
  int iCubic;
  whichCurve(t,&iCubic,&tCubic);
  Curve c=cubic(iCubic);

  Vector3 p=c.evalCubicBezier(tCubic);
  Vector3 v=c.evalCubicVelocity(tCubic);
  double a=interpolateAngle(t);
  v.normalize();
  Vector3 b=Vector3(0,1,0).cross(v);
  b.rotate(toDegree(a),v);
  b.normalize();
  Vector3 n=v.cross(b);
  Matrix4 res;
  res.setFrame(p,v,b,n);
  return res;

}






void CatmullRomCurve::initAcceleration() {
  if (nbPoint()>2) {
    Curve cubic;
    cubic.create(4);


    _acceleration.clear();
    _acceleration.push_back(Vector3(0,1,0)); //cubic.evalCubicAcceleration(1));
    for(unsigned int i=0;i<nbPoint()-2;++i) {
      cubic.point(0,point(i));
      cubic.point(1,intermediate(i,0));
      cubic.point(2,intermediate(i,1));
      cubic.point(3,point(i+1));
      _acceleration.push_back(cubic.evalCubicAcceleration(1));
    }
    _acceleration.push_back(Vector3(0,1,0));
  }
}

void CatmullRomCurve::drawTBNManual(double t) {
  p3d::drawFrame(tbnManual(t));
}


void CatmullRomCurve::drawTBNManual() {
  Curve c;
  for(unsigned int i=0;i<_pts.size()-1;++i) {
    c=cubic(i);
    double a=_angle[i];
    Vector3 t=c.evalCubicVelocity(0);
    t.normalize();
    Vector3 b=Vector3(0,1,0).cross(t);
//    cout << "a [" << i << "] = " << a << endl;
    b.rotate(toDegree(a),t);
    b.normalize();
    Vector3 n=t.cross(b);
    Matrix4 frame;
    frame.setFrame(_pts[i],t,b,n);
    p3d::drawFrame(frame,"",5.0);
  }
}



