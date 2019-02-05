#ifndef GLAPPLICATION_H
#define GLAPPLICATION_H

#include "GLWidget.h"

#include "Texture.h"
#include "Camera.h"

#include "Revolution.h"


/*!
*
* @file
*
* @brief
* @author F. Aubert - Univ Lille 1
*
*/

class GLApplication : public GLWidget
{

  typedef enum {Init_Cube,Init_Sphere,Init_Revolution,Init_None} ERequestInit;
  typedef enum {View_Input,View_3D} EView;

public:
  virtual ~GLApplication();
  GLApplication();

  /** ***** **/
  /** GLApplication must implement these methods : */
  virtual void initialize();
  virtual void update();
  virtual void draw();
  virtual void resize(int width,int height);
  /** ***** **/
  virtual void leftPanel(int i, const std::string &s);
  QSize sizeHint() const {return QSize(500,500);}

  void drawGround();
  void updateCamera();
  void apply(const p3d::Camera &c);

  // ****************************
  // TP :



  void drawScene();

private:
  void drawGroundTool(const p3d::Texture &texture);



private:
  p3d::Camera _camera,_cameraInput;
  p3d::Texture _groundTexture;

  Revolution _revolution;

  ERequestInit _requestInit=Init_Sphere; // Init_Cube, Init_Revolution
  Revolution::EShader _shaderMode=Revolution::Shader_Ambient; // Shader_Light, Shader_Texture
  EView _viewMode=View_3D; // View_2D for profile input

};

#endif // GLAPPLICATION_H

