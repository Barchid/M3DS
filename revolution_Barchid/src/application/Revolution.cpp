#include "Revolution.h"

#include "glsupport.h"


#include "GLRender.h"

#include "Vector3.h"

#include "GLTool.h"


using namespace std;
using namespace p3d;

Revolution::Revolution() {
    // default profile
    _profile.push_back(Vector3(-0.8,0.6,0.0));
    _profile.push_back(Vector3(-0.3,0.2,0.0));
    _profile.push_back(Vector3(-0.9,-0.8,0.0));

}


void Revolution::initSphere() {
    vector<float> p;
    vector<float> n;
    vector<float> t;
    vector<float> tTemp; // utilisé pour initialiser les coordonnées de textures (quick fix dégueu)

    vector<unsigned int> index;

    float nbSlice=20; // include last slice that closes sphere
    float nbStack=20;

    p.clear();
    index.clear();

    // *******
    //  TODO
    // initialiser les positions des sommets des triangles pour la sphère
    float teta = 0;
    float fi = 0;
    for(int i = 0; i < nbStack; i++)
    {
        for(int j = 0; j < nbSlice; j++)
        {
            float x = cos(teta) * sin(fi);
            float y = cos(fi);
            float z = sin(teta) * sin(fi);

            // coordonnées du point P
            p.push_back(x);
            p.push_back(y);
            p.push_back(z);

            // ajustement de la normale
            n.push_back(x);
            n.push_back(y);
            n.push_back(z);

            // définition de t et s pour les coordonnées de texture (à l'envers parce qu'on va tout remettre à l'endroit après)
            // On doit tout faire à l'envers car j'ai commencé avec nbStack
            float sCoord = teta/(2.0*M_PI);
            float tCoord = fi/(M_PI);
            tTemp.push_back(tCoord);
            tTemp.push_back(sCoord);

            teta += (2.0*M_PI)/(nbSlice-1);
        }
        teta = 0;
        fi += M_PI / (nbStack -1);
    }

    // renverser tTemp pour avoir t
    // C'est obligé parce qu'on est bête et qu'on a attaqué le problème par stack puis slice au lieu de slice puis stack
    for(int i=tTemp.size()-1;i>=0;i--) {
        t.push_back(tTemp[i]);
    }

    // initialiser les index des positions
    for(int i = 0; i < nbStack-1; i ++) {
        for(int j = 0; j < nbSlice-1; j++) {
            int bottomLeft = (i * nbSlice) + j;
            int topLeft = bottomLeft + nbSlice;
            int bottomRight = bottomLeft + 1;
            int topRight = topLeft + 1;

            index.push_back(topLeft);
            index.push_back(bottomLeft);
            index.push_back(bottomRight);
            index.push_back(bottomRight);
            index.push_back(topRight);
            index.push_back(topLeft);
        }
    }

    // *******




    initVAO(index,p,n,t);

    _texture=&_earth;

}


void Revolution::initCube() {
    vector<float> p;
    vector<float> n;
    vector<float> t;

    vector<unsigned int> index;

    // *******
    //  TODO
    // creation des positions du cube
    p = {
        -1, -1, -1, // s0
        -1,  1, -1, // s1
        -1, -1,  1, // s2
        -1,  1,  1, // s3
         1,  1,  1, // s4
         1,  1, -1, // s5
         1, -1,  1, // s6
         1, -1, -1  // s7
    };

    // les index maintenant

    // *******
    index = {
        0, 1, 2, // triangle 1
        2, 1, 3,
        3, 1, 5,
        5, 3, 4,
        4, 2, 3,
        2, 6, 4,
        1, 0, 5,
        0, 7, 5,
        2, 7, 0,
        7, 6, 2,
        7, 4, 6,
        7, 5, 4
    };

    // ##################################################
    // ##################################################
    //             Positions avec normales
    // ##################################################
    // ##################################################
    p = {
        // face 1 : celle de face
        -1, -1, -1,
        1, -1, -1,
        1,  1, -1,

        1,  1, -1,
        -1, -1, -1,
        -1,  1, -1,

       // face 2 : celle de derrière
        -1, -1,  1,
        1, -1,  1,
        1,  1,  1,

        1,  1,  1,
        -1,  1,  1,
        -1, -1,  1,

        // face 3 : celle du bas
        1, -1, -1,
        1, -1,  1,
        -1, -1,  1,
        -1, -1,  1,
        -1, -1, -1,
        1, -1, -1,

        // face 4 : celle du haut
        1,  1,  1,
        1,  1, -1,
        -1,  1,  1,
        -1,  1,  1,
        -1,  1, -1,
        1,  1, -1

        // face 5 : celle de gauche

    };

    p = {
        -1, -1, -1, // s0
        -1, -1, -1, // s0
        -1, -1, -1, // s0
        -1,  1, -1, // s1
        -1,  1, -1, // s1
        -1,  1, -1, // s1
        -1, -1,  1, // s2
        -1, -1,  1, // s2
        -1, -1,  1, // s2
        -1,  1,  1, // s3
        -1,  1,  1, // s3
        -1,  1,  1, // s3
         1,  1,  1, // s4
        1,  1,  1, // s4
        1,  1,  1, // s4
         1,  1, -1, // s5
        1,  1, -1, // s5
        1,  1, -1, // s5
         1, -1,  1, // s6
        1, -1,  1, // s6
        1, -1,  1, // s6
         1, -1, -1,  // s7
        1, -1, -1 , // s7
        1, -1, -1  // s7
    };

    n = {
        0, 0, -1
    };

    initVAO(index,p,n,t);

    _texture=&_dice;
}


void Revolution::initRevolution() {
    if (_profile.size()<2) return; // more than 2 vertices in the profile

    vector<float> p;
    vector<float> n;
    vector<float> t;
    vector<float> tTemp; // utilisé pour initialiser les coordonnées de textures (quick fix dégueu)

    vector<unsigned int> index;

    int nbSlice=40; // include last slice that closes the object
    int nbStack=_profile.size();

    std::vector<Vector3> normalProfile; // to compute normal profile

    Vector3 A; // va nous servir pour effectuer la rotation des points du profil

    // *******
    //  TODO

    // calcul des normales des segments du profile
    for (unsigned int i = 0; i < _profile.size()-1; ++i) {
        Vector3 orthogonal;
        Vector3 dir =_profile[i+1]-_profile[i];
        orthogonal.x(dir.y());
        orthogonal.y(-dir.x());
        orthogonal.z(0);

        normalProfile.push_back(orthogonal); // normal du dernier point du profile est uniquement la direction de ce point
    }

    float fi = 0;
    // initialiser les coordonnées des sommets pour l'objet de révolution
    for (int i = 0; i < nbStack; ++i) {
        Vector3 normaleToAverage1; // vecteur représentant la premiere normale servant à calculer la normale moyenne pour chaque point
        Vector3 normaleToAverage2; // vecteur représentant la deuxième normale servant à calculer la normale moyenne pour chaque point

        if(i==nbStack-1) { // si je suis le dernier, ma normale est le segment d'avant
            normaleToAverage1 = normalProfile[i - 1];
            normaleToAverage2 = normalProfile[i - 1];
        }
        if(i==0) { // si je suis le premier point, ma normale n'a pas de moyenne (une moyenne de deux fois la même chose reste la même chose)
            normaleToAverage1 = normalProfile[0];
            normaleToAverage2 = normalProfile[0];
        }
        else {
            normaleToAverage1 = normalProfile[i];
            normaleToAverage2 = normalProfile[i - 1];
        }

        // calcul de la normale pour le point du profile
        Vector3 normale = (normaleToAverage1 + normaleToAverage2) / 2;
        Vector3 pute;

        float teta = 0;
        for (int j = 0; j < nbSlice; ++j) {
            // effectuer la rotation du point du profile
            A = _profile[i].rotationY(teta);
            pute = normale.rotationY(teta); // faire une rotation de la normale

            // ajouter les coordonnées du point rotaté (??????)
            p.push_back(A.x());
            p.push_back(A.y());
            p.push_back(A.z());

            n.push_back(pute.x());
            n.push_back(pute.y());
            n.push_back(pute.z()); // ce sera toujours 0 de toute façon

            // coordonnées de texture
            float sCoord = teta/(2.0*M_PI);
            float tCoord = ((double)(nbStack - 1) - (double)i) / ((double)(nbStack - 1));
            t.push_back(sCoord);
            t.push_back(tCoord);

            teta += (2.0*M_PI)/(nbSlice-1);
        }
        fi += M_PI / (nbStack -1);
    }

    // initialiser les index des positions
    for(int i = 0; i < nbStack-1; i ++) {
        for(int j = 0; j < nbSlice-1; j++) {
            int bottomLeft = (i * nbSlice) + j;
            int topLeft = bottomLeft + nbSlice;
            int bottomRight = bottomLeft + 1;
            int topRight = topLeft + 1;

            index.push_back(bottomRight);
            index.push_back(bottomLeft);
            index.push_back(topLeft);

            index.push_back(topLeft);
            index.push_back(topRight);
            index.push_back(bottomRight);
        }
    }
    // *******





    initVAO(index,p,n,t);
    _texture=&_ul1;
}


void Revolution::init() {
    glGenBuffers(1,&_vbo);
    glGenBuffers(1,&_elementBuffer);
    glGenVertexArrays(1,&_vao);

    _earth.readMipmap("EarthDay512.jpg");
    _dice.readMipmap("dice.png");
    _ul1.readMipmap("UL1-IEEA.png");

}


void Revolution::draw(EShader mode) {
    //  return;
    glBindVertexArray(_vao);


    p3d::material(Vector4(0,0,0,1),Vector3(0,0.8,0.2),Vector3(0,0.2,0.8),100);
    p3d::diffuseBackColor=Vector3(0.8,0,0);
    if (mode==Shader_Ambient) {
        p3d::ambientColor=Vector4(0.0,0.8,0,1);
        p3d::ambientBackColor=Vector4(0.8,0.0,0,1);
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    }

    _texture->bind(0);

    switch(mode) {
    case Shader_Ambient:p3d::shaderVertexAmbient();break;
    case Shader_Light:p3d::shaderLightPhong();break;
    case Shader_Texture:p3d::shaderTextureLight();break;
    }


    glDrawElements(GL_TRIANGLES,_nbElement,GL_UNSIGNED_INT,0);



    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glBindVertexArray(0);

}


// Caution : position should be a (x,y,z) array, normal (x,y,z) array, texCoord (x,y) array. lengths should be coherent
void Revolution::initVAO(const std::vector<unsigned int> &index,const std::vector<float> &position,const std::vector<float> &normal,const std::vector<float> &texCoord) {

    vector<float> attrib;
    attrib.resize(position.size()/3*8);
    for(unsigned int i=0;i<position.size()/3;++i) {
        attrib[i*8+0]=position[i*3+0];
        attrib[i*8+1]=position[i*3+1];
        attrib[i*8+2]=position[i*3+2];

        if (!normal.empty()) {
            attrib[i*8+3]=normal[i*3+0];
            attrib[i*8+4]=normal[i*3+1];
            attrib[i*8+5]=normal[i*3+2];
        }

        if (!texCoord.empty()) {
            attrib[i*8+6]=texCoord[i*2+0];
            attrib[i*8+7]=texCoord[i*2+1];
        }
    }



    glBindBuffer(GL_ARRAY_BUFFER,_vbo);
    glBufferData(GL_ARRAY_BUFFER,attrib.size()*sizeof(float),attrib.data(),GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,index.size()*sizeof(unsigned int),index.data(),GL_STATIC_DRAW);
    _nbElement=index.size();

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER,_vbo);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),0);                                   // position
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(const GLvoid *)(3*sizeof(float)));   // normal
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(const GLvoid *)(6*sizeof(float)));   // texCoord

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_elementBuffer);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_elementBuffer);

    glBindVertexArray(0);

}

void Revolution::drawProfile() {
    p3d::ambientColor=Vector4(1,0,0,1);
    p3d::shaderVertexAmbient();
    p3d::drawLineStrip(_profile);
    p3d::drawPoints(_profile);

    vector<Vector3> mirror;
    for(auto &p:_profile) {
        mirror.push_back(Vector3(-p.x(),p.y(),p.z()));
    }

    p3d::uniformAmbient(Vector4(0,0,0,1));
    p3d::drawLineStrip(mirror);
    p3d::drawPoints(mirror);

    p3d::uniformAmbient(Vector4(0,1,0,1));
    p3d::drawLines({Vector3(0,-1,0),Vector3(0,1,0)});



}



