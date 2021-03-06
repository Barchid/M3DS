#include "GLApplication.h"

#include <iostream>
#include <cmath>

using namespace std;

GLApplication::~GLApplication() {
}

GLApplication::GLApplication() {
    // initialiser les données du programme
    //  _trianglePosition = {
    ////    // triangle 1
    ////    -0.8f,-0.5f,0.0f, // 0 sommet en bas à gauche
    ////    -0.2f,-0.5f,0.0f, // 1 sommet en bas à droite
    ////    -0.5f,0.5f,0.0f,  // 2 sommet en haut

    ////    // triangle 2
    ////    0.2f,0.5f,0.0f,  // 3 sommet en haut à gauche
    ////    0.8f,0.5f,0.0f,  // 4 sommet en haut à droite
    ////    0.5f,-0.5f,0.0f  // 5 sommet en bas

    //      // pas beau !
    ////      -0.8,-0.5,0.0, // vertex 0 anciennement vertex 0
    ////      0.8,0.5,0.0,  // 1 anciennement 4
    ////      -0.5,0.5,0.0,  // 2 anciennement 2
    ////      -0.2,-0.5,0.0, // 3 anciennement 1
    ////      0.5,-0.5,0.0,  // 4 anciennement 5
    ////      0.2,0.5,0.0  // 5 anciennement 3

    //      // pour avoir en beau :
    //      // triangle 1 = S0, S3, S2
    //      // triangle 2 = S4, S5, S1

    //      // sommet commun (question 16)
    //      -0.8,-0.8,0.0,
    //      0.8,0.8,0.0,
    //      0.0,0.2,0.0,
    //      -0.8,0.8,0.0,
    //      0.8,-0.8,0.0,
    //  };

    //  _triangleColor = { //rgba
    //    0.3f,0,0.6f,1,
    //    0.3f,0,0.6f,1,
    //    0.0f,0.9f,0.0f,1,

    //    0.0f,0.5f,0.6f,1,
    //    0.0f,0.5f,0.6f,1,
    //    0.9f,0.0f,0.0f,1
    //  };

    // indexed face set
    // les couleurs changent (on n'a plus la moyenne comme avant)
    // ceci s'explique par le fait que l'interpolation se fait dans l'ordre des positions et non pas par triangle indexé

    // les couleurs, quand on a un sommet commun, changent car on a jarté le point inexploité qui donnait du rouge


    // QUESTION 17
    _trianglePosition = {
        -0.8,-0.8,0.0,
        -0.8,0.8,0.0,

        -0.4,-0.8,0.0,
        -0.4,0.8,0.0,

        0.0,-0.8,0.0,
        0.0,0.8,0.0,

        0.4,-0.8,0.0,
        0.4,0.8,0.0,
    };
    // tous les sommets à rouge :
    _triangleColor.clear();
    for(unsigned int i=0;i<9;++i) {
        _triangleColor.push_back(1);_triangleColor.push_back(0);_triangleColor.push_back(0);_triangleColor.push_back(1);
    }

    _elementData = {
        // triangle 1
        0,3,2,

        // triangle 2
        2,1,4
    };

    this->initStrip(18,-0.8,0.4,-0.8,0.8);
    this->initRing(50, 0.2, 0.6);

    // initialiser le coefficient
    _coeff = 1.0; // il vaut 1 --> la forme est entière$
    _isGrowing = false;

    // EXERCICES SUR LA TEXTURE

//    _trianglePosition = { // rectangle tracé avec TRIANGLE_STRIP
//                          -0.6,-0.8,0,
//                          -0.6,0.8,0,
//                          0.6,-0.8,0,
//                          0.6,0.8,0
//                        };
//    _triangleColor = { // tous les sommets en rouge
//                       1,0,0,1,
//                       1,0,0,1,
//                       1,0,0,1,
//                       1,0,0,1,
//                     };
//    _triangleTexCoord = { // coordonnées de texture en chaque sommet
//                          0.5,0,
//                          0.5,1,
//                          1.0,0,
//                          ,1
//                        };
}

/**
 * Exercice 21 : initialiser trianglePosition pour faire nbSlice bandes verticales
 * @brief GLApplication::initStrip
 * @param nbSlice
 * @param xmin
 * @param xmax
 * @param ymin
 * @param ymax
 */
void GLApplication::initStrip(int nbSlice,float xmin,float xmax,float ymin,float ymax) {
    _trianglePosition.clear();
    _triangleColor.clear();

    float greenColor = 0.0; // évolution de la couleur verte suivant les points créés
    float blueColor = 1.0; // évolution de la couleur bleue suivant les points créés

    float step = xmin;
    for (int i = 0; i < nbSlice; i++) {
        // point du bas de la ligne
        _trianglePosition.push_back(step); // x
        _trianglePosition.push_back(ymin); // y
        _trianglePosition.push_back(0.0); // z

        // point du haut de la ligne
        _trianglePosition.push_back(step); // x
        _trianglePosition.push_back(ymax); // y
        _trianglePosition.push_back(0.0); //z

        // couleurs

        // en vert (pour les points du haut)
        _triangleColor.push_back(0);_triangleColor.push_back(greenColor);_triangleColor.push_back(0);_triangleColor.push_back(1);

        // en bleu (pour les points du haut)
        _triangleColor.push_back(0);_triangleColor.push_back(0);_triangleColor.push_back(blueColor);_triangleColor.push_back(1);

        // mise à jour des variables
        step +=(xmax - xmin) / (nbSlice - 1);
        greenColor += (1.0)/nbSlice;
        blueColor -= (1.0)/nbSlice;
    }
}

void GLApplication::initRing(int nbSlice, float r0, float r1) {
    _trianglePosition.clear();
    _triangleColor.clear();
    _triangleTexCoord.clear();

    float greenColor = 0.0; // évolution de la couleur verte suivant les points créés
    float blueColor = 1.0; // évolution de la couleur bleue suivant les points créés
    float coordTex = 0.0;

    float theta = 0.0;
    for (int i = 0; i < nbSlice; i++) {
        // point du petit cercle
        _trianglePosition.push_back(r0*cos(theta)); // x est donnée par la formule donnée
        _trianglePosition.push_back(r0*sin(theta)); // y est donnée par la formule donnée
        _trianglePosition.push_back(0.0); // z
        // coordonnée de texture pour le petit cercle
        _triangleTexCoord.push_back((1 + r0*cos(theta))/2); // x de coordonnée de texture
        _triangleTexCoord.push_back((1 + r0*sin(theta))/2); // y de coordonnée de texture (le ciel est sur le petit cercle ici)

        // point du haut de la ligne
        _trianglePosition.push_back(r1*cos(theta)); // x est donnée par la formule donnée
        _trianglePosition.push_back(r1*sin(theta)); // y est donnée par la formule donnée
        _trianglePosition.push_back(0.0); //z
        // coordonnée de texture pour le grand cercle
        _triangleTexCoord.push_back((1 + r1*cos(theta))/2); // x de coordonnée de texture
        _triangleTexCoord.push_back((1 + r1*sin(theta))/2); // y de coordonnée de texture (la mer est sur le grand cercle ici)

        // couleurs
        // en bleu (pour les points du cercle du bas)
        _triangleColor.push_back(0);_triangleColor.push_back(0);_triangleColor.push_back(blueColor);_triangleColor.push_back(1);
        // en vert (pour les points du grand cercle )
        _triangleColor.push_back(0);_triangleColor.push_back(greenColor);_triangleColor.push_back(0);_triangleColor.push_back(1);

        // mise à jour des variables
        theta +=(2.0*3.14159)/nbSlice;
        greenColor += (1.0)/nbSlice;
        blueColor -= (1.0)/nbSlice;
        coordTex += (1.0)/nbSlice;
    }


    // ajout du dernier point pour faire le lien
    _trianglePosition.push_back(r0); // x est donnée par la formule donnée
    _trianglePosition.push_back(0.0); // y est donnée par la formule donnée
    _trianglePosition.push_back(0.0); // z
    // ajout de la coordonnée de texture finale pour le dernier point
    _triangleTexCoord.push_back((1.0+r0)/2);
    _triangleTexCoord.push_back(0.5);

    _trianglePosition.push_back(r1); // x est donnée par la formule donnée
    _trianglePosition.push_back(0.0); // y est donnée par la formule donnée
    _trianglePosition.push_back(0.0); // z
    // ajout de la coordonnée de texture finale pour le dernier point du haut
    _triangleTexCoord.push_back((1.0+r1)/2);
    _triangleTexCoord.push_back(0.5);

    // en bleu
    _triangleColor.push_back(0);_triangleColor.push_back(0);_triangleColor.push_back(0);_triangleColor.push_back(1);
    // en vert (pour les points du cercle du haut)
    _triangleColor.push_back(0);_triangleColor.push_back(1);_triangleColor.push_back(0);_triangleColor.push_back(1);
}

/** ********************************************************************** **/
void GLApplication::initialize() {
    // appelée 1 seule fois à l'initialisation du contexte
    // => initialisations OpenGL
    glClearColor(1,1,1,1); // couleur du clear

    glLineWidth(2.0); // epaisseur des lignes tracées
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);


    _shader0=initProgram("simple");

    // init le VBO
    initTriangleBuffer();

    // init le VAO
    initTriangleVAO();

    // init le program shader
    initTexture();


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

    // SI mon cercle doit grandir pour l'animation
    if(_isGrowing) {
        _coeff += 0.02;
        if(_coeff >= 1.0) {
            _isGrowing = false;
        }
    }
    else {
        _coeff -= 0.02;
        if(_coeff <= 0.0) {
            _isGrowing = true;
        }
    }

}

void GLApplication::draw() {
    // appelée après chaque update
    // => tracer toute l'image
    glClear(GL_COLOR_BUFFER_BIT);

    // on utilise le program shader "_shader0"
    glUseProgram(_shader0);

    // affecter l'uniform "coeff" pour gérer l'animation
    GLint coeffLocation = glGetUniformLocation(_shader0, "coeff");
    glUniform1f(coeffLocation, _coeff);

    // on bind le VAO à utiliser
    glBindVertexArray(_triangleVAO);

    // on dessine le triangle (3 premiers vec3 dans le VBO --> les 3 positions du sommet)
    //  glDrawArrays(GL_TRIANGLES,0,9);

    glActiveTexture(GL_TEXTURE0); // on travaille avec l'unité de texture 0
      // dans l'instruction suivante, _textureId correspond à l'image "lagoon.jpg"; cf GLApplication::initTexture pour l'initialisation de _textureId
    glBindTexture(GL_TEXTURE_2D,_textureId); // l'unité de texture 0 correspond à la texture _textureId
      // (le fragment shader manipule des unités de textures et non les identifiants de texture directement)
    glUniform1f(glGetUniformLocation(_shader0,"textureUnit"),0); // on affecte la valeur du sampler2D du fragment shader à l'unité de texture 0.

    glDrawArrays(GL_TRIANGLE_STRIP,0, _trianglePosition.size()/3);
    //    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // eviter les effets de bords
    glBindVertexArray(0);
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




/**
 * Compile les sources du program shader et go
 * @brief GLApplication::initProgram
 * @param filename nom du program shader pour .frag et .vert
 * @return l'id du program shader chargé dans OpenGL à partir des fichiers .vert et .frag
 */
GLuint GLApplication::initProgram(const std::string &filename) {
    GLuint program;
    int infoLength; // for message error

    // lire les sources des vertex/fragment shaders
    string vertexSource=p3d::readTextFile(filename+".vert");
    string fragmentSource=p3d::readTextFile(filename+".frag");

    // créer un program shader
    program=glCreateProgram();


    // creer shaders
    GLuint vertexShader=glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader=glCreateShader(GL_FRAGMENT_SHADER);

    // attacher au program shader qu'on a créé
    glAttachShader(program,vertexShader);
    glAttachShader(program,fragmentShader);

    // pécho les sources
    const char *source;
    source=vertexSource.c_str();
    glShaderSource(vertexShader,1,&source,NULL);
    source=fragmentSource.c_str();
    glShaderSource(fragmentShader,1,&source,NULL);

    GLint ok; // code ok quand on fait un truc

    // compiler le vertex shader
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&ok);
    if (!ok) {
        glGetShaderiv(vertexShader,GL_INFO_LOG_LENGTH,&infoLength);
        cout << "=============================" << endl;
        cout << "GLSL Error : " << endl;
        char *info=new char[infoLength];
        glGetShaderInfoLog(vertexShader,infoLength,NULL,info);
        cout << info;
        cout << endl;
        delete info;
        throw ErrorD("Vertex Shader compilation error");
    }

    // compiler le fragment shader
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&ok);
    if (!ok) {
        glGetShaderiv(fragmentShader,GL_INFO_LOG_LENGTH,&infoLength);
        cout << "=============================" << endl;
        cout << "GLSL Error : " << endl;
        char *info=new char[infoLength];
        glGetShaderInfoLog(fragmentShader,infoLength,NULL,info);
        cout << info;
        cout << endl;
        delete info;
        throw ErrorD("Vertex Shader compilation error");
    }

    // binder le paramètre 0 de .vert (le paramètre in vec3 position)
    glBindAttribLocation(program,0,"position");
    glBindAttribLocation(program, 1, "color");
    glBindAttribLocation(program, 2, "texCoord");

    // lirer .vert et .frag
    glLinkProgram(program);
    glGetProgramiv(program,GL_LINK_STATUS,&ok);
    if (!ok) {
        char *info=new char[infoLength];
        glGetProgramInfoLog(program,infoLength,NULL,info);
        cout << "Info Log :" << endl;
        cout << info;
        cout << endl;
        delete info;
        throw ErrorD("Program Shader : link error (varyings ok ?)");
    }

    // retourne l'id du program shader qu'on a compilé blabla à partir des sources
    return program;
}


void GLApplication::initTexture() {
    QImage img;
    img.load("../media/lagoon.jpg");
    img=img.convertToFormat(QImage::Format_ARGB32).mirrored();

    glGenTextures(1,&_textureId);
    glBindTexture(GL_TEXTURE_2D,_textureId);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img.width(),img.height(),0,GL_BGRA,GL_UNSIGNED_BYTE,img.bits());



    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
}

/**
 * initialiser le VBO
 * @brief GLApplication::initTriangleBuffer
 */
void GLApplication::initTriangleBuffer() {
    // créer le vbo et le donner à attribut trianglePositionBuffer (qui contiendra l'id du vbo)
    glGenBuffers(1,&_trianglePositionBuffer);

    // Lier notre buffer à GL_ARRAY_BUFFER (c'est un "handle")
    glBindBuffer(GL_ARRAY_BUFFER,_trianglePositionBuffer);

    // Copier notre triangle position en attribut à GL_ARRAY_BUFFER
    glBufferData(GL_ARRAY_BUFFER,_trianglePosition.size()*sizeof(float),_trianglePosition.data(),GL_STATIC_DRAW);


    // POUR LES COULEURS
    glGenBuffers(1,&_triangleColorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,_triangleColorBuffer);
    glBufferData(GL_ARRAY_BUFFER,_triangleColor.size()*sizeof(float),_triangleColor.data(),GL_STATIC_DRAW);

    // POUR LES COORDONNES DE TEXTURES
    glGenBuffers(1,&_triangleTexCoordBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,_triangleTexCoordBuffer);
    glBufferData(GL_ARRAY_BUFFER,_triangleTexCoord.size()*sizeof(float),_triangleTexCoord.data(),GL_STATIC_DRAW);

    // POUR LES COORDONNEES
    //  glGenBuffers(1,&_elementDataBuffer);
    //  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_elementDataBuffer);
    //  glBufferData(GL_ELEMENT_ARRAY_BUFFER,_elementData.size()*sizeof(unsigned int),_elementData.data(),GL_STATIC_DRAW);
}

/**
 * Initialiser le VAO pour lier le VBO au program shader
 * @brief GLApplication::initTriangleVAO
 */
void GLApplication::initTriangleVAO() {
    // créer le vao et le mettre dans l'attribut _triangleVAO
    glGenVertexArrays(1,&_triangleVAO);

    // lier le VBO et le VAO
    // ATTENTION : VBO pas référencer parce qu'on sait qu'on la placer à GL_ARRAY_BUFFER donc pas de référence direct mais c'est là
    glBindVertexArray(_triangleVAO);
    glBindBuffer(GL_ARRAY_BUFFER,_trianglePositionBuffer);

    // L'attribut "in" de vertex shader de valeur 0 (ça représente position)
    //va contenir 3 floats (qui représentent les positions XYZ) qui sont dans le VBO (donc on saute de 3 par 3)
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,nullptr);



    // pour couleur
    glBindBuffer(GL_ARRAY_BUFFER, _triangleColorBuffer);
    glVertexAttribPointer(1,4,GL_FLOAT, GL_FALSE, 0,0);

    // pour coordonnée de textures
    glBindBuffer(GL_ARRAY_BUFFER, _triangleTexCoordBuffer);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // fin (enable les attributs in des shaders)
    glEnableVertexAttribArray(0); // position
    glEnableVertexAttribArray(1); // couleur
    glEnableVertexAttribArray(2); // coordonnées de texture

    // pour coordonnées
    //    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementDataBuffer);
    // éviter effet de bord
    glBindVertexArray(0);
}



