#include <math.h>
#include "Car.h"
#include "GLTool.h"
#include "GLRender.h"

using namespace p3d;
using namespace std;


// ************************************************************
Car::~Car() {
}

Car::Car() {
    _orientation.setIdentity();
    _position.set(0,0,0);
    _steering=0;
    _rotateWheel=0;
    _acceleration=0;
    _velocity=0;
}

// ******************************************************************
// méthodes à compléter lors du TP
void Car::drawRim() {
    p3d::modelviewMatrix.push();
    p3d::diffuseColor = Vector3(1, 0.3, 0.5);

    // ############ Cylindre horizontal ################
    p3d::modelviewMatrix.push();
    p3d::modelviewMatrix.translate(0, 0, -2.5);
    p3d::modelviewMatrix.scale(0.3,0.3,5);
    drawCylinder();
    p3d::modelviewMatrix.pop();
    // #################################################

    // ##################### Cylindre vertical #########
    p3d::modelviewMatrix.push();
    p3d::modelviewMatrix.rotate(90, 1, 0, 0); // rotation AVANT scale sinon ça fait une déformation elliptique
    p3d::modelviewMatrix.translate(0, 0, -2.5);
    p3d::modelviewMatrix.scale(0.3,0.3,5);
    drawCylinder();
    p3d::modelviewMatrix.pop();
    // #################################################

    // ############### Cylindre diagonal 45° ###########
    p3d::modelviewMatrix.push();
    p3d::modelviewMatrix.rotate(45, 1, 0, 0);
    p3d::modelviewMatrix.translate(0, 0, -2.5);
    p3d::modelviewMatrix.scale(0.3,0.3,5);
    drawCylinder();
    p3d::modelviewMatrix.pop();
    // #################################################

    // ############### Cylindre diagonal 135° ###########
    p3d::modelviewMatrix.push();
    p3d::modelviewMatrix.rotate(135, 1, 0, 0);
    p3d::modelviewMatrix.translate(0, 0, -2.5);
    p3d::modelviewMatrix.scale(0.3,0.3,5);
    drawCylinder();
    p3d::modelviewMatrix.pop();
    // #################################################


    p3d::modelviewMatrix.pop();
}

void Car::drawWheel() {
    p3d::modelviewMatrix.push();

    // ############### pneu ############################
    p3d::modelviewMatrix.push();
    p3d::diffuseColor = Vector3(0.5, 0, 1);
    p3d::modelviewMatrix.rotate(90, 0, 1, 0);
    p3d::modelviewMatrix.scale(1.5,1.5,1);
    drawTorus();
    p3d::modelviewMatrix.pop();
    // #################################################

    // ############### jante ############################
    p3d::modelviewMatrix.push();
    p3d::modelviewMatrix.scale(0.5, 0.5, 0.5);
    drawRim();
    p3d::modelviewMatrix.pop();
    // #################################################
    p3d::modelviewMatrix.pop();
}

void Car::drawAxle() {
    p3d::modelviewMatrix.push();
    p3d::diffuseColor = Vector3(0, 1, 0);

    // ############### essieu ##########################
    p3d::modelviewMatrix.push();
    p3d::modelviewMatrix.translate(0,0,0);
    p3d::modelviewMatrix.rotate(90, 0, 1, 0);
    p3d::modelviewMatrix.scale(0.3,0.3,5);
    drawCylinder();
    p3d::modelviewMatrix.pop();
    // #################################################

    // ############### roue 1 ##########################
    p3d::modelviewMatrix.push();
    p3d::modelviewMatrix.rotate(_rotateWheel, -1, 0, 0);
    p3d::modelviewMatrix.scale(1.5,1.5,1.5);
    drawWheel();
    p3d::modelviewMatrix.pop();
    // #################################################

    // ############### roue 2 ##########################
    p3d::modelviewMatrix.push();
    p3d::modelviewMatrix.translate(5, 0, 0);
    p3d::modelviewMatrix.rotate(_rotateWheel, -1, 0, 0);
    p3d::modelviewMatrix.scale(1.5,1.5,1.5);
    drawWheel();
    p3d::modelviewMatrix.pop();
    // #################################################

    p3d::modelviewMatrix.pop();
}

void Car::drawBody() {
    // REGLE : push en début de draw() et pop en fin de draw()
    // REGLE 2 : PUSH -> APPLY -> DRAW -> POP pour chaque chose à draw
    p3d::modelviewMatrix.push();

    // ############# Cube horizontal ############
    p3d::modelviewMatrix.push();
    p3d::diffuseColor = Vector3(0.5, 0, 1);
    p3d::modelviewMatrix.scale(1,1,3);
    drawCube();
    p3d::modelviewMatrix.pop();
    // ##########################################

    // ############# Cube vertical ##############
    p3d::modelviewMatrix.push();
    p3d::modelviewMatrix.translate(0,2,2);
    p3d::modelviewMatrix.scale(1,1,1);
    drawCube();
    p3d::modelviewMatrix.pop();
    // ##########################################

    p3d::modelviewMatrix.pop();
}

void Car::draw() {
    p3d::modelviewMatrix.push();


    // ############### carrosserie #####################
    p3d::modelviewMatrix.push();
//    p3d::modelviewMatrix.rotate(90, 0, 1, 0);
    p3d::modelviewMatrix.scale(1.5,1.5,1.5);
    p3d::modelviewMatrix.translate(0,0.75,0);
    drawBody();
    p3d::modelviewMatrix.pop();
    // #################################################

    // ############### essieu 1 ########################
    p3d::modelviewMatrix.push();
    p3d::modelviewMatrix.translate(-2.5, -0.75, 2.5);
//    p3d::modelviewMatrix.scale(1.5,1.5,1.5);
    drawAxle();
    p3d::modelviewMatrix.pop();
    // #################################################

    // ############### essieu 2 ########################
    p3d::modelviewMatrix.push();
    p3d::modelviewMatrix.translate(-2.5, -0.75, -3);
    p3d::modelviewMatrix.rotate(_steering, 0, 1, 0);
    drawAxle();
    p3d::modelviewMatrix.pop();
    // #################################################


    p3d::modelviewMatrix.pop();
}


void Car::drawWorld() {

    p3d::modelviewMatrix.push();

    // Application de la position et de l'orientation
    p3d::modelviewMatrix.translate(_position);
    p3d::modelviewMatrix.rotate(_orientation);
    draw(); // tracé de la voiture dans son repère local
    p3d::modelviewMatrix.pop();
}

void Car::move() {
    _acceleration+=-_velocity/50;
    _velocity+=_acceleration;
    _rotateWheel+=_velocity*20;
    _steering-=_steering/10*fabs(_velocity);

    _orientation.rotate(_steering*_velocity/(1.0+fabs(_velocity)),Vector3(0,1,0)); // le /(1.0+fabs(_velocity)) a été déterminé empiriquement

    // gérer la position pour faire avancer la voiture
    Vector3 uneDirection = *new Vector3(0, 0, -1); // la voiture avance quand elle remonte l'axe des Z
    uneDirection = _orientation * uneDirection; // appliquer l'orientation de la voiture à la position dans le repère world
    uneDirection = uneDirection * _velocity * 0.3; // gérer l'amplitude de la direction
    _position = _position + uneDirection;
}


void Car::accelerate() {
    _acceleration=0.05;

}

void Car::decelerate() {
    _acceleration=0;
}

void Car::brake() {
    _acceleration=-0.02;

}

void Car::turnLeft() {
    _steering+=0.5;
    if (_steering>35) _steering=35;
}

void Car::turnRight() {
    _steering-=0.5;
    if (_steering<-35) _steering=-35;
}





