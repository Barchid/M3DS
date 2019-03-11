#include "Airplane.h"
#include "GLTool.h"
#include "Tools.h"

using namespace p3d;
using namespace std;

Airplane::Airplane() {
    _increment=1.0;
    _accelerate=0.1;
    _velocity=0.0;
    _orientation.setIdentity();
    _position.set(0,0,0);
    _angleX=_angleY=_angleZ=0.0;
}

Airplane::~Airplane() {

}

void Airplane::initDraw() {
    _obj.initDraw();

}


void Airplane::draw() {

    p3d::diffuseColor=Vector3(1,0,0);
    p3d::shaderLightPhong();
    _obj.draw();
}

void Airplane::drawWorld() {
    p3d::modelviewMatrix.push();
    p3d::modelviewMatrix.translate(_position);

    //  p3d::modelviewMatrix.rotate(_angleY,0,1,0); // yaw
    //  p3d::modelviewMatrix.rotate(_angleX,1,0,0); // pitch
    //  p3d::modelviewMatrix.rotate(_angleZ,0,0,1); // roll

    p3d::modelviewMatrix.rotate(_orientation);
    p3d::modelviewMatrix.scale(2,2,2); // grossir un peu l'avion
    draw();

    p3d::modelviewMatrix.pop();
}

void Airplane::move() {
    // gérer la position pour faire avancer l'avion
    Vector3 uneDirection = *new Vector3(0, 0, 1); // l'avion avance dans l'axe des Z
    uneDirection = _orientation * uneDirection; // appliquer l'orientation de l'avion à la position dans le repère world
    uneDirection = uneDirection * _velocity * 0.3; // gérer l'amplitude de la direction (MOINS VITE !!!)
    _position = _position + uneDirection;
}

void Airplane::pitchDown() {
    //  _angleX+=_increment;
    _orientation.rotate(_increment,Vector3(1,0,0)); // on cumule l'orientation courante avec une rotation (et non plus l'angle autour de X).
}

void Airplane::pitchUp() {
//    _angleX-=_increment;
    _orientation.rotate(_increment,Vector3(-1,0,0)); // on cumule l'orientation courante avec une rotation (et non plus l'angle autour de X).

}

void Airplane::rollRight() {
//    _angleZ+=_increment;
    _orientation.rotate(_increment,Vector3(0,0,1)); // on cumule l'orientation courante avec une rotation (et non plus l'angle autour de Z).

}

void Airplane::rollLeft() {
//    _angleZ-=_increment;
    _orientation.rotate(_increment,Vector3(0,0,-1)); // on cumule l'orientation courante avec une rotation (et non plus l'angle autour de Z).
}

void Airplane::yawLeft() {
//    _angleY+=_increment;
    _orientation.rotate(_increment,Vector3(0,1,0)); // on cumule l'orientation courante avec une rotation (et non plus l'angle autour de Y).

}

void Airplane::yawRight() {
//    _angleY-=_increment;
    _orientation.rotate(_increment,Vector3(0,-1,0)); // on cumule l'orientation courante avec une rotation (et non plus l'angle autour de Y).
}

void Airplane::accelerate() {
    _velocity+=_accelerate;
    if (_velocity>3) _velocity=3;
}

void Airplane::decelerate() {
    _velocity-=_accelerate;
    if (_velocity<0) _velocity=0;
}

// lecture fichier .obj
void Airplane::read(const string &filename) {
    _obj.readInit(filename);
    _obj.rotateY(90);
}



