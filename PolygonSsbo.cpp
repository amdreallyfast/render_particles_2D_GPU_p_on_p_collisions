#include "PolygonSsbo.h"

#include "glload/include/glload/gl_4_4.h"

/*-----------------------------------------------------------------------------------------------
Description:
    Calls the base class to give members initial values (zeros).

    Allocates space for the SSBO and dumps the given collection of polygon faces into it.
Parameters: 
    faceCollection  Self-explanatory
Returns:    None
Creator: John Cox, 9-8-2016
-----------------------------------------------------------------------------------------------*/
PolygonSsbo::PolygonSsbo(const std::vector<PolygonFace> &faceCollection) :
    SsboBase(), // generate buffers
    _bufferSizeBytes(0)
{
    // two vertices per face (used with glDrawArrays(...))
    _numVertices = faceCollection.size() * 2;

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, _bufferId);
    _bufferSizeBytes = sizeof(PolygonFace) * faceCollection.size();
    glBufferData(GL_SHADER_STORAGE_BUFFER, _bufferSizeBytes, faceCollection.data(), GL_STATIC_DRAW);

    // cleanup
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

}

/*-----------------------------------------------------------------------------------------------
Description:
    Does nothing.  Exists to be declared virtual so that the base class' destructor is called 
    upon object death.
Parameters: None
Returns:    None
Creator: John Cox, 9-8-2016
-----------------------------------------------------------------------------------------------*/
PolygonSsbo::~PolygonSsbo()
{
}
//
///*-----------------------------------------------------------------------------------------------
//Description:
//    Generates the SSBO and the VAO, but does not allocate space for them.  
//    The SSBO is bound in ConfigureCompute(...) and sized/resized in UpdateValues(...).
//    The VAO is initialized in ConfigureRender(...).
//
//    Note: MUST be called before calling ConfigureCompute(...) and ConfigureRender(...).
//    Also Note: The buffers cannot be generated in the constructor unless the constructor is 
//    initialized AFTER OpenGL's context is initialized.  If PolygonSsbo is used as a global 
//    somewhere, then it will attempt initialization at program start, which is prior to OpenGL's
//    context and the program will blow up.
//Parameters: None
//Returns:    None
//Creator: John Cox, 11-24-2016
//-----------------------------------------------------------------------------------------------*/
//void PolygonSsbo::Init()
//{
//    if (_bufferId == 0)
//    {
//        glGenBuffers(1, &_bufferId);
//        // do not allocate space; that happens at runtime with UpdateValues(...)
//    }
//    if (_vaoId == 0)
//    {
//        glGenVertexArrays(1, &_vaoId);
//    }
//
//    _hasBeenInitialized = true;
//}

/*-----------------------------------------------------------------------------------------------
Description:
    Binds the SSBO object (a CPU-side thing) to its corresponding buffer in the shader (GPU).

    Note: It is ok to call this function for multiple compute shaders so that the same SSBO can 
    be used in each shader.  No member variables are altered in this function.
Parameters: 
    computeProgramId    Self-explanatory
Returns:    None
Creator: John Cox, 11-24-2016
-----------------------------------------------------------------------------------------------*/
void PolygonSsbo::ConfigureCompute(unsigned int computeProgramId)
{
    //if (!_hasBeenInitialized)
    //{
    //    fprintf(stderr, "PolygonSsbo::ConfigureCompute(...) error: SSBO has not been initialized\n");
    //    return;
    //}

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, _bufferId);

    // see the corresponding area in ParticleSsbo::Init(...) for explanation
    // Note: MUST use the same binding point 
    GLuint ssboBindingPointIndex = 13;   // or 1, or 5, or 17, or wherever IS UNUSED
    GLuint storageBlockIndex = glGetProgramResourceIndex(computeProgramId, GL_SHADER_STORAGE_BLOCK, "FaceBuffer");
    glShaderStorageBlockBinding(computeProgramId, storageBlockIndex, ssboBindingPointIndex);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssboBindingPointIndex, _bufferId);

    // cleanup
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

}

/*-----------------------------------------------------------------------------------------------
Description:
    Sets up the vertex attribute pointers for this SSBO's VAO.
Parameters: 
    RenderProgramId     Self-explanatory
    drawStyle           Expected to be GL_LINES (2D program) or GL_TRIANGLES (3D program).
Returns:    None
Creator: John Cox, 11-24-2016
-----------------------------------------------------------------------------------------------*/
void PolygonSsbo::ConfigureRender(unsigned int renderProgramId, unsigned int drawStyle)
{
    //if (!_hasBeenInitialized)
    //{
    //    fprintf(stderr, "PolygonSsbo::ConfigureRender(...) error: SSBO has not been initialized\n");
    //    return;
    //}

    _drawStyle = drawStyle;

    // the render program is required for vertex attribute initialization or else the program WILL crash at runtime
    glUseProgram(renderProgramId);
    glGenVertexArrays(1, &_vaoId);
    glBindVertexArray(_vaoId);

    // the vertex array attributes only work on whatever is bound to the array buffer, so bind 
    // shader storage buffer to the array buffer, set up the vertex array attributes, and the 
    // VAO will then use the buffer ID of whatever is bound to it
    glBindBuffer(GL_ARRAY_BUFFER, _bufferId);
    // do NOT call glBufferData(...) because it was called earlier for the shader storage buffer

    // each face is made up of two vertices, so set the attribtues for the vertices
    unsigned int vertexArrayIndex = 0;
    unsigned int bufferStartOffset = 0;
    unsigned int bytesPerStep = sizeof(MyVertex);

    // position
    GLenum itemType = GL_FLOAT;
    unsigned int numItems = sizeof(MyVertex::_position) / sizeof(float);
    glEnableVertexAttribArray(vertexArrayIndex);
    glVertexAttribPointer(vertexArrayIndex, numItems, itemType, GL_FALSE, bytesPerStep, (void *)bufferStartOffset);

    // normal
    itemType = GL_FLOAT;
    numItems = sizeof(MyVertex::_normal) / sizeof(float);
    bufferStartOffset += sizeof(MyVertex::_position);
    vertexArrayIndex++;
    glEnableVertexAttribArray(vertexArrayIndex);
    glVertexAttribPointer(vertexArrayIndex, numItems, itemType, GL_FALSE, bytesPerStep, (void *)bufferStartOffset);

    // cleanup
    glBindVertexArray(0);   // unbind this BEFORE the array
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);    // render program
}
//
///*-----------------------------------------------------------------------------------------------
//Description:
//    Dumps the given collection of particle faces into the SSBO that is managed by this object.
//    
//    This is a convenience method for giving the compute shader an already-transformed array of 
//    items to work with.  A simple transform could be passed into the compute shader, but then 
//    every invocation of the sahder would have to transform the same faces with the same matrix.  
//    Rather than duplicate load, I'll go out of my way to give the compute shader pre-transformed 
//    values.
//    
//    Note: Do not pass in arrays of different sizes at runtime.  Just use a single array. This 
//    SSBO object has no concept of the compute shader's contents, so it does not update the 
//    compute shader's "num faces" uniform.  
//Parameters:
//    faceCollection  Self-explanatory
//Returns:    None
//Creator: John Cox, 10-10-2016
//-----------------------------------------------------------------------------------------------*/
//void PolygonSsbo::UpdateValues(const std::vector<PolygonFace> &faceCollection)
//{
//    // two vertices per face (used with glDrawArrays)
//    _numVertices = faceCollection.size() * 2;
//
//    glBindBuffer(GL_SHADER_STORAGE_BUFFER, _bufferId);
//    unsigned int byteCounter = sizeof(PolygonFace) * faceCollection.size();
//    if (byteCounter > _bufferSizeBytes)
//    {
//        // re-allocate more space (yes, this is a crude resize, but its a demo)
//        _bufferSizeBytes = byteCounter;
//        glBufferData(GL_SHADER_STORAGE_BUFFER, _bufferSizeBytes, faceCollection.data(), GL_STATIC_DRAW);
//    }
//    else
//    {
//        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, _bufferSizeBytes, faceCollection.data());
//    }
//
//    // cleanup
//    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
//}

