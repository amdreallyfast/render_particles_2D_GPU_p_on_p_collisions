#include "ParticlePolygonRegion.h"


/*-----------------------------------------------------------------------------------------------
Description:
    Ensures that the object starts object with initialized values.  
Parameters:
    faces   A collection of PolygonFace structures.  This object will keep a copy of the 
            originals and any transformed version.
Returns:    None
Exception:  Safe
Creator:    John Cox (10-10-2016)
-----------------------------------------------------------------------------------------------*/
ParticleRegionPolygon::ParticleRegionPolygon(const std::vector<PolygonFace> faces)
{
    _originalFaces = faces;

    // the transformed variants begin equal to the original points, then diverge after 
    // SetTransform(...) is called
    _transformedFaces = faces;
}

/*-----------------------------------------------------------------------------------------------
Description:
    Why transform this for every emission of every particle when I can do it once before
    particle updating and be done with it for the rest of the frame?
Parameters:
    regionTransform     Transform the faces' points and normals with this.
Returns:    None
Exception:  Safe
Creator:    John Cox (10-10-2016)
-----------------------------------------------------------------------------------------------*/
void ParticleRegionPolygon::SetTransform(const glm::mat4 &regionTransform)
{
    for (size_t faceIndex = 0; faceIndex < _originalFaces.size(); faceIndex++)
    {
        PolygonFace faceCopy = _originalFaces[faceIndex];
        faceCopy._start._position = regionTransform * faceCopy._start._position;
        faceCopy._start._normal = regionTransform * faceCopy._start._normal;
        faceCopy._end._position = regionTransform * faceCopy._end._position;
        faceCopy._end._normal = regionTransform * faceCopy._end._normal;

        _transformedFaces[faceIndex] = faceCopy;
    }
}

/*-----------------------------------------------------------------------------------------------
Description:
    A simple getter for the transformed faces of this polygon.  If no transform has been set,
    then these values will be the same as the originals.

    This is part of the pre-computing-transforms approach that provides ready-to-go polygon 
    faces for the compute shader, and as a bonus the geometry shader is off the hook too.

    Note: Used to update the values of a polygon SSBO.
Parameters: None
Returns:    
    A const reference to the internal collection of polygon faces.  A reference doesn't summon 
    the copy constructor, which slows things down unless cache coherency was desired out of a 
    local array of data, but cache coherency will always be a miss with std containers because 
    their contents are located elsewhere in memory, so I just return a reference.
Exception:  Safe
Creator:    John Cox (10-10-2016)
-----------------------------------------------------------------------------------------------*/
const std::vector<PolygonFace> &ParticleRegionPolygon::GetFaces() const
{
    return _transformedFaces;
}