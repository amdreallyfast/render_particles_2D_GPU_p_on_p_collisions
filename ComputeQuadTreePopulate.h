#pragma once

#include <string>
#include "glm/vec4.hpp"


/*-----------------------------------------------------------------------------------------------
Description:
    Controls the compute shader that populates the quad tree with particles.  There is one 
    shader dispatched for every particle.

    That's it.  It was a bit difficult to figure out how to populate a quad tree when 
    (1) There is no malloc/new in a shader.
    (2) It must be thread safe.
    (3) There are no mutexes in a shader.
    (4) Avoid running through the particle array more than once.  One of my earlier ideas was to dispatch a compute shader for each node in the tree, and then have each node run through all particles.  That would allow me to not worry about multiple particles trying to add themselves to the same node at the same time, but it also makes each node have to run through the entire particle collection.  That's a lot of duplicate work.  I am hoping that my approach of running a compute shader for each particle and then using using atomic counters as crude mutexes to restrict node access (especially during subdivision) will be faster.

Note: This class is not concerned with the particle SSBO.  It is concerned with uniforms and
summoning the shader.  SSBO setup is performed in the appropriate SSBO object.
Creator:    John Cox (1-16-2017)
-----------------------------------------------------------------------------------------------*/
class ComputeQuadTreePopulate
{
public:
    ComputeQuadTreePopulate(unsigned int maxParticlesPerNode, unsigned int maxNodes, unsigned int maxParticles, float particleRegionRedius, const glm::vec4 &particleRegionCenter, unsigned int numColumnsInTreeInitial, unsigned int numRowsInTreeInitial, unsigned int numNodesInTreeInitial, const std::string computeShaderKey);
    ~ComputeQuadTreePopulate();

    void PopulateTree();
    unsigned int NumActiveNodes() const;

private:
    unsigned int _computeProgramId;
    unsigned int _totalParticles;
    unsigned int _totalNodes;
    unsigned int _activeNodes;
    unsigned int _initialNodes;

    // all atomic counts are lumped into one buffer
    unsigned int _atomicCounterBufferId;
    unsigned int _acOffsetNodesInUse;
    unsigned int _acOffsetParticleCounterPerNode;

    // similar to the copy atomic counter in ComputeParticleUpdate for "active particle count" , this is an atomic counter copy buffer for "active node count"
    unsigned int _acNodesInUseCopyBufferId;

    int _unifLocMaxParticlesPerNode;
    int _unifLocMaxNodes;
    int _unifLocMaxParticles;
    int _unifLocParticleRegionRadius;
    int _unifLocParticleRegionCenter;

    // both the initial number of columns and the initial number of rows are needed for the "inverse X/Y increment" calculation, but only the initial number of rows needs to stick around for the calculation
    int _unifLocNumColumnsInTreeInitial;
    int _unifLocInverseXIncrementPerColumn;
    int _unifLocInverseYIncrementPerRow;


};