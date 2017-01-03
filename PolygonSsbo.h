#pragma once

#include "SsboBase.h"
#include "PolygonFace.h"
#include <vector>

/*-----------------------------------------------------------------------------------------------
Description:
    Sets up the Shader Storage Block Object for a 2D polygon.  The polygon will be used in both 
    the compute shader and in the geometry render shader.
Creator: John Cox, 9-8-2016
-----------------------------------------------------------------------------------------------*/
class PolygonSsbo : public SsboBase
{
public:
    PolygonSsbo();
    virtual ~PolygonSsbo();

    void Init();
    void ConfigureCompute(unsigned int computeProgramId) override;
    void ConfigureRender(unsigned int renderProgramId) override;

    void UpdateValues(const std::vector<PolygonFace> &faceCollection);

private:
    unsigned int _bufferSizeBytes;
};

