#include "GLApplication.h"

#include <iostream>

using namespace std;
using namespace p3d;

GLApplication::~GLApplication() {
}

GLApplication::GLApplication() {

    _lightPosition.set(0,0,0);

    _basicMesh.initTetrahedron();

    _angle=0.0;

    // _projection de classe Matrix4 (déclaré dans GLApplication.h)
    _projection.setOrtho(-20,20,-20,20,5,100); // cf calcul de la matrice dans le cours
    _projection.setOrtho(-18,22,-10,30,5,100); // Q 13.1 bouger l'écran
    _projection.setOrtho(-5,2,-10,10,5,100); // Q 13.2 effet de "zoom"
    _projection.setOrtho(-20,20,-20,20,5,1e8); // Q 14
    _projection.setOrtho(-20,20,-20,20,5,3.40282e+038); // Q 14 : far encore + grand
    /* Question 14 : explication de Mr. Aubert
     * J'ai un triangle bleu qui bouffe mon triangle en entier pour la raison que + mon far est grand, plus je perd en précision pendant mon calcul de projection orthogonal
     * De plus, comme il calcule en 1/z, il va encore plus perdre la précision
     * */
    _projection.setOrtho(-20,20,-20,20,-10,100); // Q 14 : revenir à un far raisonnable

    _obj.readInit("bite.obj",Vector3(-10,-10,-30),Vector3(10,10,-10)); // reporte l'objet dans la boite d'extémités (-10,-10,-30) et (10,10,-10)
    _basicMesh.initObj(_obj); // pour remplacer l'initialisation du tétraèdre
}




/** ********************************************************************** **/
void GLApplication::initialize() {
    // appelée 1 seule fois à l'initialisation du contexte
    // => initialisations OpenGL
    glClearColor(1,1,1,1);

    //  glLineWidth(2.0);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

    glEnable(GL_DEPTH_TEST); // activation Depth Buffer (opérations écriture/tests)
    //glDepthFunc(GL_LESS);  // le test passe si depth(src) < depth(dst)
    glDepthFunc(GL_LESS);  // le test passe si depth(src) > depth(dst)
    glClearDepth(1.0); // valeur d'initialisation du depth destination de tous les pixels lors d'un glClear

    _shader.attribute("position",0);
    _shader.attribute("normale",1);
    _shader.read("openGL3D");



    _basicMesh.initDraw();
}

void GLApplication::resize(int width,int height) {
    // appelée à chaque dimensionnement du widget OpenGL
    // (inclus l'ouverture de la fenêtre)
    // => réglages liés à la taille de la fenêtre
    glViewport(0,0,width,height);
    // ...
}

void GLApplication::update() {
    // appelée toutes les 20ms (60Hz)
    // => mettre à jour les données de l'application
    // avant l'affichage de la prochaine image (animation)
    // ...

    _angle+=2;


    _transform.setTranslation(0,0,-15);
    _transform.rotate(_angle,Vector3(1,0.2,0));
    _transform.translate(0,0,15);
}

void GLApplication::draw() {
    // appelée après chaque update
    // => tracer toute l'image

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // affecte tous les pixels du color buffer et du depth buffer avec les valeurs d'initialisation

    glUseProgram(_shader.id());

    _shader.uniform("projection",_projection); // utilisation de la classe shader
    // qui permet d'alléger la syntaxe OpenGL
    _shader.uniform("transform", _transform); // Q 15 : appliquer la rotation

    _shader.uniform("lightPosition", _lightPosition);

    _shader.uniform("diffuseColor",Vector3(0.2,0.5,0.6));

    _basicMesh.draw();
    glUseProgram(0);

    snapshot(); // capture opengl window if requested
}

/** ********************************************************************** **/
/** i = button number, s = button text
 */
void GLApplication::leftPanel(int i,const std::string &s) {
    cout << "GLApplication : button clicked " << i << " " << s << endl;
    /*
  switch (i) {
    case 0:menu0();break;
    case 1:menu1();break;
  }
  */
}

/** *********************************************************** **/




