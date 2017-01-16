#include "ComputeQuadTreeGenerateGeometry.h"

#include "glload/include/glload/gl_4_4.h"
#include "ShaderStorage.h"

// TODO: header
ComputeQuadTreeGenerateGeometry::ComputeQuadTreeGenerateGeometry(unsigned int maxNodes, unsigned int maxPolygonFaces, const std::string &computeShaderKey) :
    _computeProgramId(0),
    _totalNodes(0),
    _atomicCounterBufferId(0),
    _acPolygonFacesInUseOffset(0),
    _acPolygonFacesCrudeMutexOffset(0),
    _unifLocMaxNodes(0),
    _unifLocMaxPolygonFaces(0)
{
    _totalNodes = maxNodes;

    ShaderStorage &shaderStorageRef = ShaderStorage::GetInstance();

    _unifLocMaxNodes = shaderStorageRef.GetUniformLocation(computeShaderKey, "uMaxNodes");
    _unifLocMaxPolygonFaces = shaderStorageRef.GetUniformLocation(computeShaderKey, "uMaxPolygonFaces");

    _computeProgramId = shaderStorageRef.GetShaderProgram(computeShaderKey);

    glUseProgram(_computeProgramId);

    // set uniform data
    glUniform1ui(_unifLocMaxNodes, maxNodes);
    glUniform1ui(_unifLocMaxPolygonFaces, maxPolygonFaces);

    // atomic counters
    glGenBuffers(1, &_atomicCounterBufferId);
    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, _atomicCounterBufferId);
    glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint) * 2, 0, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);
    glUseProgram(0);

    // offsets and binding MUST match the offsets specified in the "quad tree generate geometry" shader
    _acPolygonFacesInUseOffset = 0;
    _acPolygonFacesCrudeMutexOffset = 4;
    glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 4, _atomicCounterBufferId);
}

// TODO: header
ComputeQuadTreeGenerateGeometry::~ComputeQuadTreeGenerateGeometry()
{
    glDeleteBuffers(1, &_atomicCounterBufferId);
}

// TODO: header
// resets the atomic counters and dispatches the compute shader
void ComputeQuadTreeGenerateGeometry::GenerateGeometry()
{
    glUseProgram(_computeProgramId);

    // both atomic counters are 0
    // Note: This shader will run through all the nodes and generate new faces for every single one, so the initial faces in use is 0.  The crude mutex should be available by default, so it is also 0.

    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, _atomicCounterBufferId);

    GLuint zeroArr[2] = { 0, 0 };
    glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(zeroArr), &zeroArr);

    // calculate the number of work groups and start the magic
    GLuint numWorkGroupsX = (_totalNodes / 256) + 1;
    GLuint numWorkGroupsY = 1;
    GLuint numWorkGroupsZ = 1;

    glDispatchCompute(numWorkGroupsX, numWorkGroupsY, numWorkGroupsZ);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);


    // cleanup
    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);
    glUseProgram(0);
}


