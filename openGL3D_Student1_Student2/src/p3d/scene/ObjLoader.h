#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <vector>
#include <string>
#include "Object3D.h"
#include "Vector2.h"
#include "Vector3.h"


/*!
*
* @file Simple OBJ Loader :
* all faces are in the array vertexIndexFace
* each face (type VertexFace) is an array of VertexAttrib (one VertexAttrib per vertex of the face).
* VertexAttrib contains 3 indices (position, normal, texCoord) that refers to the following arrays :
* - positionObj : array of all positions directly read from the file .obj (i.e. the v)
* - normalObj : array of all normals read from the file .obj (i.e. the vn)
* - texCoordObj : array of all texture coordinates from the file .obj (i.e. the t).
*
* @brief
* @author F. Aubert
*
*/

namespace p3d {
class Vector2;


class VertexAttrib {
public:
  int _positionIndex,_normalIndex,_texCoordIndex;
  VertexAttrib() : _positionIndex(-1),_normalIndex(-1),_texCoordIndex(-1) {}
};

typedef std::vector<VertexAttrib> VertexAttribFace;

static inline int cycle(int i,int nb) {
    return (i%nb+nb)%nb;
}


class ObjLoader {
public:
    ObjLoader();
    virtual ~ObjLoader();

    void read(const std::string &filename);
    void readInit(const std::string &resourceName);
    void readInit(const std::string &resourceName, const p3d::Vector3 &mini, const p3d::Vector3 &maxi);

    void scaleInBox(double left, double right, double top, double bottom,double znear,double zfar);
    void scaleInBoxMin(double left, double right, double bottom, double top, double znear, double zfar);

    void check();
    void computeNormal();
    void computeTexCoord();
    void computeNormalFace();
    void computeNormalFace(unsigned int i);
    void triangulate();

    inline const Vector3 &positionOBJ(unsigned int i) const {return _positionOBJ[i];}
    inline const Vector3 &normalOBJ(unsigned int i) const {return _normalOBJ[i];}
    inline const Vector2 &texCoordOBJ(unsigned int i) const {return _texCoordOBJ[i];}

    inline const Vector3 &normalFace(unsigned int i) const {return _normalFace[i];}

    inline const VertexAttrib &vertexIndex(unsigned int i,unsigned int j) const {return _vertexIndexFace[i][cycle(j,nbVertex(i))];}
    inline int positionIndex(unsigned int i,unsigned int j) const {return vertexIndex(i,j)._positionIndex;}

    inline const Vector3 &positionVertex(unsigned int i,unsigned int j) const {return positionOBJ(positionIndex(i,j));}
    inline const Vector3 &normalVertex(unsigned int i,unsigned int j) const {return normalOBJ(vertexIndex(i,j)._normalIndex);}
    inline const Vector2 &texCoordVertex(unsigned int i,unsigned int j) const {return texCoordOBJ(vertexIndex(i,j)._texCoordIndex);}

    inline unsigned int nbPosition() const {return _positionOBJ.size();}
    inline unsigned int nbNormal() const {return _normalOBJ.size();}
    inline unsigned int nbTexCoord() const {return _texCoordOBJ.size();}

    inline unsigned int nbVertex(unsigned int i) const {return _vertexIndexFace[i].size();}
    inline unsigned int nbFace() const {return _vertexIndexFace.size();}


    void rotateY(double angle);
    void rotateX(double angle);
    void translate(double x, double y, double z);

protected: // protected only for easier coding of the answers (visibility of the internal structure)
    std::vector<p3d::Vector3> _positionOBJ; //! x,y,z of a vertex (read from OBJ)
    std::vector<p3d::Vector3> _normalOBJ; //! x,y,z of a normal  (read from OBJ)
    std::vector<p3d::Vector2> _texCoordOBJ; //! s,t of a texCoord (read from OBJ)
    std::vector<p3d::Vector3> _normalFace; //! x,y,z of a normal (one normal per face)
    std::vector<VertexAttribFace> _vertexIndexFace; //! _vertexIndexFace[i][j] returns the VertexAttrib of the j-ieme vertex of the i-ieme face


};
}

#endif // OBJLOADER_H

