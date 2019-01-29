#include "BasicMesh.h"
#include "ObjLoader.h"
#include <vector>
#include <iostream>

using namespace std;
using namespace p3d;

BasicMesh::~BasicMesh() {
    //TODO : can be deleted only when GL context is active
    //  glDeleteBuffers(1,&_attributeBuffer);
    //  glDeleteBuffers(1,&_elementBuffer);
}


BasicMesh::BasicMesh() {
    _attributeBuffer=0;
    _elementBuffer=0;
}







void BasicMesh::initTetrahedron() {

    // truc de base
    std::vector<float> position={
        -1,0,-1, // V0
        1,0,-1,  // V1
        0,1,1,  // V2
        0,-1,1  // V3
    };

    std::vector<float> color={
        1,0,0, // rouge
        0,1,0, // vert
        0,0,1, // bleu
        0,1,1  // cyan
    };

    // index for 4 triangles
    _element={
        0,1,2,0,2,3,0,1,3,1,2,3
    };

    // Question 6
    position={
        -1,0,-1, // V0
        1,0,-1,  // V1
        0,1,1,  // V2

        -1,0,-1, // V0
        0,1,1,  // V2
        0,-1,1,  // V3

        -1,0,-1, // V0
        1,0,-1,  // V1
        0,-1,1,  // V3

        1,0,-1,  // V1
        0,1,1,  // V2
        0,-1,1  // V3
    };


    color={
        1,0,0,
        1,0,0,
        1,0,0,
        0,1,0,
        0,1,0,
        0,1,0,
        0,0,1,
        0,0,1,
        0,0,1,
        0,1,1,
        0,1,1,
        0,1,1
    };

    _element={
        0,1,2,3,4,5,6,7,8,9,10,11
    };


    // Question 10
    position={
        -20,0,-10, // V0
        10,0,-10,  // V1
        0,10,-30,  // V2
        0,-20,-30  // V3
    };

    color={
        1,0,0, // rouge
        0,1,0, // vert
        0,0,1, // bleu
        0,1,1  // cyan
    };

    // index for 4 triangles
    _element={
        0,1,2,0,2,3,0,1,3,1,2,3
    };

    _attribute.clear();
    // on itère sur les triangles
    for(unsigned int i=0;i<position.size()/3;++i) {

        // Pour le buffer entrelacé, on met d'abord les trois positions
        _attribute.push_back(position[i*3+0]);
        _attribute.push_back(position[i*3+1]);
        _attribute.push_back(position[i*3+2]);

        // ensuite on met les 3 couleurs
        _attribute.push_back(color[i*3+0]);
        _attribute.push_back(color[i*3+1]);
        _attribute.push_back(color[i*3+2]);

    }
}


void BasicMesh::initObj(const ObjLoader &obj) {
    _attribute.clear();
    _element.clear();

    // initialisation de _attribute :
    // - obj.nbFace() = nombre de triangles
    // - obj.positionVertex(i,j) = position du j-ème sommet dans le i-ème triangle (de type Vector3)
    // - obj.normalVertex(i,j) = normale du j-ème sommet dans le i-ième triangle (de type Vector3)
    for(unsigned int i=0;i<obj.nbFace();++i) {
        for(unsigned int j=0;j<3;++j) {
            Vector3 v = obj.positionVertex(i,j);
            Vector3 c = obj.normalVertex(i, j);

            _attribute.push_back(v.x());
            _attribute.push_back(v.y());
            _attribute.push_back(v.z());
// Q 22 : désactiver le report dans 0,1
//            float x = (c.x() - -1)/(1 - -1);
//            float y = (c.y() - -1)/(1 - -1);
//            float z = (c.z() - -1)/(1 - -1);
//            _attribute.push_back(x);
//            _attribute.push_back(y);
//            _attribute.push_back(z);
            // Q 22 : faire normal
            _attribute.push_back(c.x());
            _attribute.push_back(c.y());
            _attribute.push_back(c.z());
        }
    }


}

// INIT LES VBO
void BasicMesh::initBuffer() {
    // init le buffer entralacé (position et couleur en même temps)
    glGenBuffers(1,&_attributeBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,_attributeBuffer);
    glBufferData(GL_ARRAY_BUFFER,_attribute.size()*sizeof(float),_attribute.data(),GL_STATIC_DRAW);


    // indexed face set
    // Q 16 : devient inutile, on bosse avec un cube
    //    glGenBuffers(1,&_elementBuffer);
    //    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_elementBuffer);
    //    glBufferData(GL_ELEMENT_ARRAY_BUFFER,_element.size()*sizeof(unsigned int),_element.data(),GL_STATIC_DRAW);

}

void BasicMesh::initVAO() {

    glGenVertexArrays(1,&_vao);
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER,_attributeBuffer);


    // TODO : gérer le buffer entrelacé (sauter 6 par 6 et, pour les couleurs, commencer à indice 3)

    // Gérer liaison avec la position (indice 0), je saute de 6 floats (X,Y,Z + 4 couelrus
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);

    // Gérer liaison avec la couleur (indice 1), je saute de 6 floats (X,Y,Z + 4 couelrus
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *) (3 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    // END TODO

    // Q 16 : devient inutile
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_elementBuffer);

    glBindVertexArray(0);
}

void BasicMesh::initDraw() {
    initBuffer();
    initVAO();
}


void BasicMesh::draw() {
    glBindVertexArray(_vao);

    // Q16 : on remplace draw elements par draw arrays
    glDrawArrays(GL_TRIANGLES,0,_attribute.size());
    //    glDrawElements(GL_TRIANGLES,_element.size(),GL_UNSIGNED_INT,(const GLvoid *)(0*sizeof(GLuint)));

    glBindVertexArray(0);
}


void BasicMesh::updateBuffer() {

}

