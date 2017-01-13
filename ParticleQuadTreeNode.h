#pragma once

#include <memory>>

const unsigned int MAX_PARTICLES_PER_QUAD_TREE_NODE = 25;

/*-----------------------------------------------------------------------------------------------
Description:
    Contains all info necessary for a single node of the quad tree.  It is a dumb container 
    meant for use only by ParticleQuadTree.
Creator:    John Cox (12-17-2016)
-----------------------------------------------------------------------------------------------*/
struct ParticleQuadTreeNode
{
    ParticleQuadTreeNode() :
        _numCurrentParticles(0),
        _startingParticleIndex(0),
        _inUse(0),
        _isSubdivided(0),
        _childNodeIndexTopLeft(-1),
        _childNodeIndexTopRight(-1),
        _childNodeIndexBottomRight(-1),
        _childNodeIndexBottomLeft(-1),
        _leftEdge(0.0f),
        _topEdge(0.0f),
        _rightEdge(0.0f),
        _bottomEdge(0.0f),
        _neighborIndexLeft(-1),
        _neighborIndexTopLeft(-1),
        _neighborIndexTop(-1),
        _neighborIndexTopRight(-1),
        _neighborIndexRight(-1),
        _neighborIndexBottomRight(-1),
        _neighborIndexBottom(-1),
        _neighborIndexBottomLeft(-1)
    {
        memset(_indicesForContainedParticles, 0, sizeof(int) * MAX_PARTICLES_PER_QUAD_TREE_NODE);
    }

    int _indicesForContainedParticles[MAX_PARTICLES_PER_QUAD_TREE_NODE];
    int _numCurrentParticles;
    int _startingParticleIndex;

    int _inUse;
    int _isSubdivided;
    int _childNodeIndexTopLeft;
    int _childNodeIndexTopRight;
    int _childNodeIndexBottomRight;
    int _childNodeIndexBottomLeft;

    // left and right edges implicitly X, top and bottom implicitly Y
    float _leftEdge;
    float _topEdge;
    float _rightEdge;
    float _bottomEdge;

    int _neighborIndexLeft;
    int _neighborIndexTopLeft;
    int _neighborIndexTop;
    int _neighborIndexTopRight;
    int _neighborIndexRight;
    int _neighborIndexBottomRight;
    int _neighborIndexBottom;
    int _neighborIndexBottomLeft;

};
