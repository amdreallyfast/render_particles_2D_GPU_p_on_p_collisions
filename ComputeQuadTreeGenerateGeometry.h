#pragma once

#include <string>

class ComputeQuadTreeGenerateGeometry
{
public:
    ComputeQuadTreeGenerateGeometry(unsigned int maxNodes, unsigned int maxPolygonFaces, const std::string &computeShaderKey);
    ~ComputeQuadTreeGenerateGeometry();

    void GenerateGeometry();
    unsigned int NumActiveFaces() const;

private:
    unsigned int _computeProgramId;
    unsigned int _totalNodes;
    unsigned int _facesInUse;

    unsigned int _atomicCounterBufferId;
    unsigned int _acOffsetPolygonFacesInUse;
    unsigned int _acOffsetPolygonFacesCrudeMutex;

    unsigned int _atomicCounterCopyBufferId;

    int _unifLocMaxNodes;
    int _unifLocMaxPolygonFaces;
};
