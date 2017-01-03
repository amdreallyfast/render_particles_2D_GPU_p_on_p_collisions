#pragma once

#include "MyVertex.h"

/*-----------------------------------------------------------------------------------------------
Description:
    This is a simple structure that describes the face of a 2D polygon.  The face begins at P1 
    and ends at P2.  In this demo, the normal will be used to calculate collisions.
Creator:    John Cox (9-8-2016)
-----------------------------------------------------------------------------------------------*/
struct PolygonFace
{
    /*-------------------------------------------------------------------------------------------
    Description:
        Gives members initial values to describe the face of a 2D polygon face.  Without the 
        normals, this would just be a 2D line, but with surface normals the line is technically 
        considered a face.
    Parameters: 
        start   Self-eplanatory.
    Returns:    None
    Creator: John Cox, 9-25-2016
    -------------------------------------------------------------------------------------------*/
    PolygonFace(const MyVertex &start, const MyVertex &end) :
        _start(start),
        _end(end)
    {
    }

    MyVertex _start;
    MyVertex _end;
};
