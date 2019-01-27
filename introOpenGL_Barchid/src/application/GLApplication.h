#ifndef GLAPPLICATION_H
#define GLAPPLICATION_H


#include "GLWidget.h"
#include "Tools.h"

#include <string>

class GLApplication : public GLWidget {
  Q_OBJECT  
public:
  GLApplication();
  virtual ~GLApplication();

  // exo 21 : init strip
  void initStrip(int nbSlice,float xmin,float xmax,float ymin,float ymax);

  // exo 23 : initRing
  void initRing(int nbSlice,float r0,float r1);

  /** ***** **/
  /** GLApplication must implement these methods : */
  virtual void initialize(); // initialiser application
  virtual void update(); // mise à jour des données en 60fps
  virtual void draw(); // mise à jour du tracé (juste après update())
  virtual void resize(int width,int height); // callback quand on resize
  /** ***** **/
  virtual void leftPanel(int i, const std::string &s);
  virtual QSize sizeHint() const {return QSize(1000,1000);}
  /** ***** **/
  GLuint initProgram(const std::string &filename);
  void initTriangleBuffer();
  void initTriangleVAO();
  void initTexture();

private:
  std::vector<float> _trianglePosition; // positions du triangle (pour VBO)
  std::vector<float> _triangleColor; // couleuyrs du triangle
  std::vector<float> _triangleTexCoord;
  std::vector<unsigned int> _elementData;

  float _coeff; // coefficient nécessaire à l'animation
  bool _isGrowing;

  GLuint _trianglePositionBuffer; // id du vbo pour position
  GLuint _triangleColorBuffer; // id du vbo pour couleur
  GLuint _triangleTexCoordBuffer;
  GLuint _elementDataBuffer;

  GLuint _triangleVAO;
  GLuint _shader0;
  GLuint _textureId;



};

#endif // GLAPPLICATION_H

