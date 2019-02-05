#ifndef REVOLUTION_H
#define REVOLUTION_H

#include <vector>

#include "glsupport.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Texture.h"
#include "InteractPosition.h"

class Revolution : public p3d::InteractPosition
{


public:
  typedef enum {Shader_Ambient,Shader_Light,Shader_Texture} EShader;


  Revolution();

  void init();

  void initCube();
  void initRevolution();
  void initSphere();

  void draw(EShader mode);

  void drawProfile();
  void clearProfile() {_profile.clear();}


  virtual p3d::Vector3 *interactPoint(unsigned int i) {return &(_profile[i]);}
  virtual void interactUpdate(unsigned int) {}
  virtual unsigned int interactSize() {return _profile.size();}
  virtual void interactInsert(unsigned int i,const p3d::Vector3 &p) {  _profile.insert(_profile.begin()+i,p);}

private:
  void initVAO(const std::vector<unsigned int> &index, const std::vector<float> &position, const std::vector<float> &normal={}, const std::vector<float> &texCoord={});



  GLuint _vao=0,_elementBuffer=0,_vbo=0;
  GLuint _nbElement=0;

  p3d::Texture _dice,_earth,_ul1,_ground;
  p3d::Texture *_texture;

  std::vector<p3d::Vector3> _profile;
};

#endif // REVOLUTION_H

