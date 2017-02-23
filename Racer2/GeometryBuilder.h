#ifndef GEOMETRYBUILDER_H
#define GEOMETRYBUILDER_H

class MeshManager;
class Model;
class Mesh;


Mesh& BuildQuad(MeshManager& mgr);

Mesh& BuildPyramid(MeshManager& mgr);

Mesh& BuildQuadPyramid(MeshManager& mgr);

Mesh& BuildCube(MeshManager& mgr);

Mesh& BuildSphere(MeshManager& mgr, int LatLines, int LongLines);

#endif
