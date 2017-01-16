#pragma once

#include <string>

class ComputeQuadTreeGenerateGeometry
{
public:
    ComputeQuadTreeGenerateGeometry(unsigned int maxNodes, unsigned int maxPolygonFaces, const std::string &computeShaderKey);
    ~ComputeQuadTreeGenerateGeometry();

    void GenerateGeometry();


private:
    unsigned int _computeProgramId;
    unsigned int _totalNodes;
    unsigned int _totalPolygonFaces;

    unsigned int _atomicCounterBufferId;
    unsigned int _acPolygonFacesInUseOffset;
    unsigned int _acPolygonFacesCrudeMutexOffset;

    int _unifLocMaxNodes;
    int _unifLocMaxPolygonFaces;
};
