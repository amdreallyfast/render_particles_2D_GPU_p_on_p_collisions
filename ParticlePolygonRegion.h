#pragma once

#include "glm/vec2.hpp"
#include "glm/mat4x4.hpp"
#include "PolygonFace.h"
#include <vector>


/*-----------------------------------------------------------------------------------------------
Description:
    This object defines a polygonal region within which particles are considered active.  It is 
    a stripped down variation of the one in "render particles 2D advanced CPU" because it only 
    needs to keep track of the polygon boundaries and however they are transformed and does NOT
    need to have "out of bounds" checks.
Creator:    John Cox (10-10-2016)
-----------------------------------------------------------------------------------------------*/
class ParticleRegionPolygon
{
public:
    ParticleRegionPolygon(const std::vector<PolygonFace> faces);
    virtual void SetTransform(const glm::mat4 &regionTransform);

    const std::vector<PolygonFace> &GetFaces() const;
private:
    // this stuff is only kept around as a reference for when transforms need to update it, 
    // which is merely once per frame, so unlike "render particles 2D advanced CPU", cache 
    // coherency isn't much of a concern here
    std::vector<PolygonFace> _originalFaces;
    std::vector<PolygonFace> _transformedFaces;
};

