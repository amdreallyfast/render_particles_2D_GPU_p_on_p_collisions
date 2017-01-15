#include "ComputeQuadTreePopulate.h"

#include "glload/include/glload/gl_4_4.h"
#include "ShaderStorage.h"


// TODO: header
ComputeQuadTreePopulate::ComputeQuadTreePopulate(
    unsigned int maxParticlesPerNode, 
    unsigned int maxNodes, 
    unsigned int maxParticles, 
    unsigned int maxPolygonFaces, 
    float particleRegionRedius, 
    const glm::vec4 &particleRegionCenter, 
    unsigned int numColumnsInTreeInitial, 
    unsigned int numRowsInTreeInitial,
    const std::string computeShaderKey)
{
    _totalParticleCount = maxParticles;
    _totalNodeCount = maxNodes;

    ShaderStorage &shaderStorageRef = ShaderStorage::GetInstance();

    _unifLocMaxParticlesPerNode = shaderStorageRef.GetUniformLocation(computeShaderKey, "uMaxParticlesPerNode");
    _unifLocMaxNodes = shaderStorageRef.GetUniformLocation(computeShaderKey, "uMaxNodes");
    _unifLocMaxParticles = shaderStorageRef.GetUniformLocation(computeShaderKey, "uMaxParticles");
    _unifLocMaxPolygonFaces = shaderStorageRef.GetUniformLocation(computeShaderKey, "uMaxPolygonFaces");
    _unifLocParticleRegionRadius = shaderStorageRef.GetUniformLocation(computeShaderKey, "uParticleRegionRadius");
    _unifLocParticleRegionCenter = shaderStorageRef.GetUniformLocation(computeShaderKey, "uParticleRegionCenter");
    _unifLocNumColumnsInTreeInitial = shaderStorageRef.GetUniformLocation(computeShaderKey, "uNumColumnsInTreeInitial");
    _unifLocInverseXIncrementPerColumn = shaderStorageRef.GetUniformLocation(computeShaderKey, "uInverseXIncrementPerColumn");
    _unifLocInverseYIncrementPerRow = shaderStorageRef.GetUniformLocation(computeShaderKey, "uInverseYIncrementPerRow");

    _computeProgramId = shaderStorageRef.GetShaderProgram(computeShaderKey);

    // now set up the atomic counter
    _acNodesInUseOffset = 0;
    _acPolygonFacesInUseOffset = 4;
    _acNodeSubdivisionCrudeMutexOffset = 8;
    _acPolygonFacesCrudeMutexOffset = 12;
    _acNodeAccessCrudeMutexsOffset = 16;

    glUseProgram(_computeProgramId);

    glGenBuffers(1, &_atomicCounterBufferId);
    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, _atomicCounterBufferId);
    
    // atomic counters:
    // - nodes in use
    // - polygon faces in use
    // - subdivision mutex
    // - polygon faces access mutex
    // - node accesss (1 per node)
    glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint) * (4 + maxNodes), 0, GL_DYNAMIC_READ);

    // now the copy buffer
    glGenBuffers(1, &_acNodesInUseCopyBufferId);
    glBindBuffer(GL_COPY_WRITE_BUFFER, _acNodesInUseCopyBufferId);
    glBufferData(GL_COPY_WRITE_BUFFER, sizeof(GLuint), 0, GL_DYNAMIC_COPY);
    glBindBuffer(GL_COPY_WRITE_BUFFER, 0);

    // cleanup
    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);
    glUseProgram(0);

    // don't need to have a program or bound buffer to set the buffer base
    // Note: It seems that atomic counters must be bound where they are declared and cannot be 
    // bound dynamically like the ParticleSsbo and PolygonSsbo.  So remember to use the SAME buffer 
    // binding base as specified in the shader.
    glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 3, _atomicCounterBufferId);
}

// TODO: header
ComputeQuadTreePopulate::~ComputeQuadTreePopulate()
{
    glDeleteBuffers(1, &_atomicCounterBufferId);
}

// TODO: header
void ComputeQuadTreePopulate::PopulateTree()
{
    // reset atomic counters
    // calculate number of work groups
    // dispatch compute 
    

    /*
    glBindBuffer(GL_COPY_READ_BUFFER, _acParticleCounterBufferId);
    glBindBuffer(GL_COPY_WRITE_BUFFER, _acParticleCounterCopyBufferId);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, sizeof(GLuint));
    unsigned int *ptr = (GLuint*)glMapBufferRange(GL_COPY_WRITE_BUFFER, 0, sizeof(GLuint), GL_MAP_READ_BIT);
    _activeParticleCount = *ptr;
    glUnmapBuffer(GL_COPY_WRITE_BUFFER);
*/
}




