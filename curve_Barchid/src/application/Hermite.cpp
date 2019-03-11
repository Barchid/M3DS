#include "Hermite.h"

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
* Evaluation de la courbe de hermite P(t) :
* - _a,_b,_ta et _tb sont les données géométriques (points extrémités et tangentes).
* - on peut utiliser les opérateurs *, + entre les points et des doubles. Exemple : p=matrix[i]*_a+_b, ...
**/
Vector3 Hermite::eval(double t) {
    // remplacer la matrice 4x4 identité suivante avec les valeurs correctes pour une courbe de Hermite (i.e. matrice de hermite; cf cours)

    double matrix[16]={
        2.0, -2.0,  1.0,  1.0,
        -3.0,  3.0, -2.0, -1.0,
        0.0,  0.0,  1.0,  0.0,
        1.0,  0.0,  0.0,  0.0
    };

    // Matrice de Hermite M
    double M[4][4] = {
        {2.0, -2.0,  1.0,  1.0},
        {-3.0,  3.0, -2.0, -1.0},
        {0.0,  0.0,  1.0,  0.0},
        {1.0,  0.0,  0.0,  0.0}
    };

    Vector3 res(0.0,0.0,0.0);

    double t3 = pow(t, 3.0);
    double t2 = pow(t, 2.0);

    //res = (2.0*t3 - 3.0*t2 + 1.0) * _a + (-2.0*t3 + 3.0*t2) * _b + (t3 - 2.0*t2 + 1.0) * _ta + (t3 - t2) * _tb;

    double c1 = matrix[0]*t3 + matrix[4]*t2 + matrix[8]*t + matrix[12]*1;
    double c2 = matrix[1]*t3 + matrix[5]*t2 + matrix[9]*t + matrix[13]*1;
    double c3 = matrix[2]*t3 + matrix[6]*t2 + matrix[10]*t + matrix[14]*1;
    double c4 = matrix[3]*t3 + matrix[7]*t2 + matrix[11]*t + matrix[15]*1;

    res = c1*_a + c2*_b + c3*_ta + c4*_tb;
    return res;
}

/**
* Trace la courbe de hermite (100 points)
**/
void Hermite::draw() {
    vector<Vector3> lPoints;

    // A COMPLETER : calculer 100 (par exemple) points successifs de la courbe pour décrire la courbe de hermite
    // on insère les points calculés dans le tableau lPoints par des lPoints.push_back(unVector3) (lPoints sera alors tracé à la fin de la méthode par l'appel, déjà présent, à p3d::drawThickLineStrip ).

    for (double i = 1; i < 100; ++i) {
        lPoints.push_back(this->eval(i/100.0));
    }

    p3d::drawThickLineStrip(lPoints);

}


/** **************************************************************************************** */
Hermite::Hermite(const Vector3 &a,const Vector3 &na,const Vector3 &b,const Vector3 &nb) {
    _a=a;
    _b=b;
    _ta=na;
    _tb=nb;

    _nbInput=0;
}

p3d::Vector3 *Hermite::interactPoint(unsigned int i) {
    switch(i) {
    case 0:return &_a;
    case 2:return &_b;
    case 1:return &_interactTa;
    case 3:return &_interactTb;
    default:return NULL;
    }
}

void Hermite::interactUpdate(unsigned int i) {
    if (i==1) _ta=(_interactTa-_a)*5;
    if (i==3) _tb=(_interactTb-_b)*5;
    if (i==0) _interactTa=(_a+_ta/5);
    if (i==2) _interactTb=(_b+_tb/5);
}

void Hermite::interactInsert(unsigned int i, const Vector3 &p) {

    switch(i) {
    case 0:_a=p;break;
    case 2:_b=p;break;
    case 1:_interactTa=p;break;
    case 3:_interactTb=p;break;
    case 4:_nbInput=0;_a=p;break;
    default:break;
    }
    _nbInput++;
}

void Hermite::set(const Vector3 &a,const Vector3 &ta,const Vector3 &b,const Vector3 &tb) {
    _a=a;
    _b=b;
    _ta=ta;
    _tb=tb;
}

void Hermite::drawControl() {
    p3d::diffuseColor=Vector3(0,0,1);
    p3d::ambientColor=Vector4(0,0,1,1);
    glPointSize(5);
    switch(nbInput()) {
    case 4:
        p3d::shaderLightPhong();
        p3d::drawArrow(b(),tb()/5,0.01,"","T_B");
    case 3:
        p3d::draw("B",b()+Vector3(0.02,0.02,0.0));
        p3d::shaderVertexAmbient();
        p3d::drawPoints(vector<Vector3>{b()});
    case 2:
        p3d::shaderLightPhong();
        p3d::drawArrow(a(),ta()/5,0.01,"","T_A");
    case 1:
        p3d::draw("A",a()+Vector3(0.01,0.01,0.0));
        p3d::shaderVertexAmbient();
        p3d::drawPoints(vector<Vector3>{a()});
    }
}


Hermite::Hermite() {
    //ctor
    _nbInput=0;

}

Hermite::~Hermite() {
    //dtor
}

