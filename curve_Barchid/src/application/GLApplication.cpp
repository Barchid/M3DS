/**

  @author F. Aubert
  **/


#include "GLApplication.h"
#include "GLTool.h"
#include <iostream>

#include "Hermite.h"
#include "Curve.h"
#include "CatmullRomCurve.h"


using namespace std;
using namespace p3d;


/// ctor
GLApplication::GLApplication() {
  _leftPanelMenu << "Hermite" << "Bezier" << "Bezier Interpolation" << "Catmull-Rom";
  _leftPanelMenu << "" << "Switch Control Points" << "Switch TBN";

  _camera.ortho(-1,1,-1,1,1,2);
  _camera.position(0,0,0);

  // ************************
  _modeDraw=Draw_Hermite;
  _viewControlPoint=0;

  _bezier.cameraInteract(&_camera);
  _catmullRom.cameraInteract(&_camera);
  _hermite.cameraInteract(&_camera);

  _whichInteraction=&_hermite;

  p3d::lightPosition[0]=Vector4(0,0,10);

  _tAnimation=0;
  _animateTBN=false;


}

/// dtor
///
GLApplication::~GLApplication() {

}


void GLApplication::initialize() {
  // appelée 1 seule fois à l'initialisation du contexte
  // => initialisations OpenGL

  glClearColor(1,1,1,1);

  glLineWidth(4);
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glClearDepth(1);

  initGLTool();
  glPointSize(4.0);

}



/** ***************************************************************************
resize
  **/


void GLApplication::resize(int width,int height) {
  // appelée à chaque dimensionnement du widget OpenGL
  // (inclus l'ouverture de la fenêtre)
  // => réglages liés à la taille de la fenêtre
  _camera.viewport(0,0,width,height);
}



void GLApplication::update() {
  if (_whichInteraction) {
    Vector2 mouse(mouseX(),mouseY());
    if (mouseLeftPressed()) {
      _whichInteraction->selectNearest(mouse,10);
    }
    if (mouseLeft()) {
      _whichInteraction->moveSelected(mouse);
    }
    if (mouseRightPressed()) {
      _whichInteraction->addEnd(mouse);
    }
  }
  if (_animateTBN) {
    _tAnimation+=0.001;
    if (_tAnimation>1) {
      _tAnimation=0;
    }
  }

}

void GLApplication::leftPanel(int i,const std::string &s) {
  cout << "GLApplication : button clicked " << i << " " << s << endl;
  switch(i) {
    case 0:_modeDraw=Draw_Hermite;_whichInteraction=&_hermite;break;
    case 1:_modeDraw=Draw_Bezier;_whichInteraction=&_bezier;break;
    case 2:_modeDraw=Draw_Catmull;_whichInteraction=&_catmullRom;break;
    case 3:_modeDraw=Draw_Catmull;_whichInteraction=&_catmullRom;_catmullRom.setup();break;
    case 5:_viewControlPoint=(_viewControlPoint+1)%2;break;
    case 6:_animateTBN=!_animateTBN;break;
  }
}

void GLApplication::updateCamera() {
}



void GLApplication::draw() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  p3d::apply(_camera);

  printMode();


  switch (_modeDraw) {
    case Draw_Bezier:
      drawBezier();
      break;
    case Draw_Catmull:
      drawCatmullRom();
      break;
    case Draw_Hermite:
      drawHermite();
      break;
    default:
      break;
  }

  if (_animateTBN) {

    if (_modeDraw==Draw_Catmull) {
      _catmullRom.drawTBN(_tAnimation);
    }
  }


  glUseProgram(0);

  snapshot(); // capture opengl window if requested

}






/** ******************************************************************* **/
/**
  Drawings
**/

void GLApplication::drawBezier() {
  p3d::shaderVertexAmbient();
  glLineWidth(3);
  if (_viewControlPoint==0) {
    p3d::uniformAmbient(Vector4(1,0,0));
    _bezier.drawControl();
  }
  p3d::uniformAmbient(Vector4(0,1,0.3));
  _bezier.drawBezier();
  glLineWidth(1);
}


void GLApplication::drawCatmullRom() {
  p3d::shaderVertexAmbient();
  if (_viewControlPoint==0) {
    p3d::uniformAmbient(1,0,0);
    _catmullRom.drawControl(3-_viewControlPoint);
  }
  p3d::uniformAmbient(0,1,0.3);
  _catmullRom.draw();
}

void GLApplication::drawHermite() {
  _hermite.drawControl();
  if (_hermite.nbInput()==4) {
    p3d::uniformAmbient(1,0,0);
    glLineWidth(3);
    _hermite.draw();
    glLineWidth(1);
  }
}


void GLApplication::printMode() {
  switch (_modeDraw) {
  case Draw_Bezier:
    p3d::draw("Bezier",Vector2(-1,-1));
    break;
  case Draw_Catmull:
    p3d::draw("Catmull Rom",Vector2(-1,-1));
    break;
  case Draw_Hermite:
    p3d::draw("Hermite",Vector2(-1,-1));
    break;
  }
}







