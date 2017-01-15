#pragma once

#include <string>
#include "glm/vec4.hpp"


// TODO: header
class ComputeQuadTreePopulate
{
public:
    ComputeQuadTreePopulate(unsigned int maxParticlesPerNode, unsigned int maxNodes, unsigned int maxParticles, unsigned int maxPolygonFaces, float particleRegionRedius, const glm::vec4 &particleRegionCenter, unsigned int numColumnsInTreeInitial, unsigned int numRowsInTreeInitial, const std::string computeShaderKey);
    ~ComputeQuadTreePopulate();

    void PopulateTree();

private:
    unsigned int _computeProgramId;
    unsigned int _totalParticleCount;
    unsigned int _totalNodeCount;

    // all atomic counts are lumped into one buffer
    unsigned int _atomicCounterBufferId;
    unsigned int _acNodesInUseOffset;
    unsigned int _acPolygonFacesInUseOffset;
    unsigned int _acNodeSubdivisionCrudeMutexOffset;
    unsigned int _acPolygonFacesCrudeMutexOffset;
    unsigned int _acNodeAccessCrudeMutexsOffset;

    // similar to the copy atomic counter in ComputeParticleUpdate for "active particle count" , this is an atomic counter copy buffer for "active node count"
    unsigned int _acNodesInUseCopyBufferId;

    unsigned int _unifLocMaxParticlesPerNode;
    unsigned int _unifLocMaxNodes;
    unsigned int _unifLocMaxParticles;
    unsigned int _unifLocMaxPolygonFaces;
    unsigned int _unifLocParticleRegionRadius;
    unsigned int _unifLocParticleRegionCenter;

    // both the initial number of columns and the initial number of rows are needed for the "inverse X/Y increment" calculation, but only the initial number of rows needs to stick around for the calculation
    unsigned int _unifLocNumColumnsInTreeInitial;
    unsigned int _unifLocInverseXIncrementPerColumn;
    unsigned int _unifLocInverseYIncrementPerRow;


};