#pragma once

#include "IParticleEmitter.h"
#include "ParticleEmitterPoint.h"
#include "ParticleEmitterBar.h"
#include <string>
#include <vector>

/*-----------------------------------------------------------------------------------------------
Description:
    Encapsulates particle updating via compute shader.  Particle updating includes particle 
    movement and checking if it is out of the polygon boundaries.  There is one compute shader 
    that does this, and this class is built to communicate with and summon that particular 
    shader.

    Note: This class is not concerned with the particle SSBO.  It is concerned with uniforms and
    summoning the shader.  SSBO setup is performed in the appropriate SSBO object.

Creator:    John Cox (11-24-2016)
-----------------------------------------------------------------------------------------------*/
class ComputeParticleUpdate
{
public:
    ComputeParticleUpdate(unsigned int numParticles, unsigned int numFaces, const std::string &computeShaderKey);
    ~ComputeParticleUpdate();

    unsigned int Update(const float deltaTimeSec) const;

private:
    unsigned int _totalParticleCount;
    unsigned int _computeProgramId;

    // the atomic counter is used to count the total number of active particles after this 
    // update
    // Also Note: The copy buffer is necessary to avoid trashing OpenGL's beautifully 
    // synchronized pipeline.  Experiments showed that, after particle updating, mapping a 
    // pointer to the atomic counter dropped frame rates from ~60fps -> ~3fps.  Ouch.  But now 
    // I've learned about buffer copying, so now the buffer mapping happens on a buffer that is 
    // not part of the compute shader's pipeline, and frame rates are back up to ~60fps.  
    // Lovely :)
    unsigned int _acParticleCounterBufferId;
    unsigned int _acParticleCounterCopyBufferId;

    // unlike most OpeGL IDs, uniform locations are GLint
    int _unifLocParticleCount;
    int _unifLocPolygonFaceCount;
    int _unifLocDeltaTimeSec;
};
