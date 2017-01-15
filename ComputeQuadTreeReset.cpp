#include "ComputeQuadTreeReset.h"

#include "glload/include/glload/gl_4_4.h"
#include "ShaderStorage.h"


// TODO: header
ComputeQuadTreeReset::ComputeQuadTreeReset(unsigned int numStartingNodes, unsigned int maxNodes, const std::string computeShaderKey)
{
    _totalNodeCount = maxNodes;
    ShaderStorage &shaderStorageRef = ShaderStorage::GetInstance();

    _unifLocNumStartingNodes = shaderStorageRef.GetUniformLocation(computeShaderKey, "uNumStartingNodes");
    _unifLocMaxNodes = shaderStorageRef.GetUniformLocation(computeShaderKey, "uMaxNodes");

    _computeProgramId = shaderStorageRef.GetShaderProgram(computeShaderKey);

    // upload uniform values
    glUseProgram(_computeProgramId);
    glUniform1ui(_unifLocNumStartingNodes, numStartingNodes);
    glUniform1ui(_unifLocMaxNodes, maxNodes);
    glUseProgram(0);
}

// TODO: header
// doesn't do a whole lot
void ComputeQuadTreeReset::ResetQuadTree()
{
    GLuint numWorkGroupsX = (_totalNodeCount / 256) + 1;
    GLuint numWorkGroupsY = 1;
    GLuint numWorkGroupsZ = 1;

    glUseProgram(_computeProgramId);

    // compute ALL the resets!
    glDispatchCompute(numWorkGroupsX, numWorkGroupsY, numWorkGroupsZ);

    //??is the vertex attrib array barrier bit necessary after a reset, before there are any new values??
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

    glUseProgram(0);
}
