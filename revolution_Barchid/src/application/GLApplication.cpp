#include "GLApplication.h"

#include "GLTool.h"

#include <iostream>

/*!
*
* @file
*
* @brief
* @author F. Aubert
*
*/


using namespace p3d;
using namespace std;

GLApplication::~GLApplication() {

}

GLApplication::GLApplication() {
  _leftPanelMenu << "Sphere" << "Cube" << "Revolution" << " " << "Wire" << "Light" << "Texture" << "" << "Input Profile";

  _camera.position(0,2,5);
  _camera.lookAt(Vector3(0,0,0));

  _revolution.cameraInteract(&_cameraInput);


}

void GLApplication::initialize() {
  glClearColor(1,1,1,1);

  glLineWidth(2);
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glClearDepth(1);

  p3d::initGLTool();
  glPointSize(5.0);


  _groundTexture.readMipmap("ground01.jpg");
  glEnable(GL_STENCIL_TEST);

  glFrontFace(GL_CCW);

  _revolution.init();



}



void GLApplication::drawScene() {

  p3d::modelviewMatrix.push();

  drawGround();

   _revolution.draw(_shaderMode);

  p3d::modelviewMatrix.pop();

}


void GLApplication::drawGround() {
  p3d::modelviewMatrix.push();
  p3d::textureMatrix.push();
  p3d::textureMatrix.scale(135,135,135);
  p3d::modelviewMatrix.translate(0,-3,0);
  p3d::modelviewMatrix.scale(10,10,10);
  p3d::ambientColor=Vector4(0,0,0,1);
  p3d::shaderTextureLight();
  p3d::drawGround(_groundTexture);
  p3d::textureMatrix.pop();
  p3d::modelviewMatrix.pop();
}



/** ***************************************************************** **/
void GLApplication::resize(int width,int height) {
  _camera.viewport(0,0,width,height);
  double ratio=double(width)/height;
  _camera.frustum(-0.01*ratio,0.01*ratio,-0.01,0.01,0.03,100.0);

  _cameraInput.viewport(0,0,width,height);
  _cameraInput.ortho(-1*ratio,1*ratio,-1,1,0,1);
}


void GLApplication::update() {
  if (_viewMode==View_Input) {
    Vector2 mouse(mouseX(),mouseY());
    if (mouseLeftPressed()) {
      _revolution.selectNearest(mouse,10);
    }
    if (mouseLeft()) {
      _revolution.moveSelected(mouse);
    }
    if (mouseRightPressed()) {
      _revolution.addEnd(mouse);
    }
    if (keyPressed(Qt::Key_C)) {
      _revolution.clearProfile();
    }
  }
  else {
    updateCamera();
  }
}


void GLApplication::updateCamera() {
  if (left()) {
    _camera.translate(-0.1,0,0,Coordinate_Local);
  }
  if (right()) {
    _camera.translate(0.1,0,0,Coordinate_Local);
  }
  if (forward()) {
    _camera.translate(0,0,-0.1,Coordinate_Local);
  }
  if (backward()) {
    _camera.translate(0,0,0.1,Coordinate_Local);
  }
  if (mouseLeft()) {
    // rotate camera : deltaMouseX(), deltaMouseY() give the mouse motion
    Vector3 vertical=_camera.directionTo(Coordinate_Local,Vector3(0,1,0));
    auto t=_camera.pointTo(Coordinate_Local,Vector3(0,0,0));
    _camera.translate(t,Coordinate_Local);
    _camera.rotate(-deltaMouseX()/5.0,vertical,Coordinate_Local);
    _camera.rotate(deltaMouseY()/5.0,1,0,0,Coordinate_Local);
    _camera.translate(-t,Coordinate_Local);
  }

  p3d::lightPosition[0]=Vector4(0,0,0,1);
}


void GLApplication::draw() {
  switch (_requestInit) {
  case Init_Sphere:_revolution.initSphere();break;
  case Init_Cube:_revolution.initCube();break;
  case Init_Revolution:_revolution.initRevolution();break;
  default:break;
  }
  _requestInit=Init_None;


  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


  if (_viewMode==View_3D) {
    p3d::apply(_camera);
    drawScene();
  }
  else {
    p3d::apply(_cameraInput);
    _revolution.drawProfile();
  }

}


void GLApplication::leftPanel(int i,const std::string &s) {
  cout << "GLApplication : button clicked " << i << " " << s << endl;
  if (i!=8) _viewMode=View_3D;
  switch(i) {
  case 1:_requestInit=Init_Cube;break;
  case 0:_requestInit=Init_Sphere;break;
  case 2:_requestInit=Init_Revolution;break;

  case 4:_shaderMode=Revolution::Shader_Ambient;break;
  case 5:_shaderMode=Revolution::Shader_Light;break;
  case 6:_shaderMode=Revolution::Shader_Texture;break;

  case 8:_viewMode=View_Input;break;
  }
}

