#include "OpenGlErrorHandling.h"

#include <string>
#include <stdio.h>

/*-----------------------------------------------------------------------------------------------
Description:
    Rather than calling glGetError(...) every time I make an OpenGL call, I register this
    function as the debug callback.  If an error or any OpenGL message in general pops up, this
    prints it to stderr.  I can turn it on and off by enabling and disabling the "#define DEBUG" 
    statement in main(...).
Parameters:
    Unknown.  The function pointer is provided to glDebugMessageCallbackARB(...), and that
    function is responsible for calling this one as it sees fit.
Returns:    None
Creator:    John Cox (2014)
-----------------------------------------------------------------------------------------------*/
void APIENTRY DebugFunc(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
    const GLchar* message, const GLvoid* userParam)
{
    std::string srcName;
    switch (source)
    {
    case GL_DEBUG_SOURCE_API_ARB: srcName = "API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB: srcName = "Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB: srcName = "Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY_ARB: srcName = "Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION_ARB: srcName = "Application"; break;
    case GL_DEBUG_SOURCE_OTHER_ARB: srcName = "Other"; break;
    default:
        srcName = "UNKNOWN SOURCE";
        break;
    }

    std::string errorType;
    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR_ARB: errorType = "Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB: errorType = "Deprecated Functionality"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB: errorType = "Undefined Behavior"; break;
    case GL_DEBUG_TYPE_PORTABILITY_ARB: errorType = "Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE_ARB: errorType = "Performance"; break;
    case GL_DEBUG_TYPE_OTHER_ARB: errorType = "Other"; break;
    default:
        errorType = "UNKNOWN ERROR TYPE";
        break;
    }

    std::string typeSeverity;
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH_ARB: typeSeverity = "High"; break;
    case GL_DEBUG_SEVERITY_MEDIUM_ARB: typeSeverity = "Medium"; break;
    case GL_DEBUG_SEVERITY_LOW_ARB: typeSeverity = "Low"; break;
    default:
        typeSeverity = "UNKNOWN SEVERITY";
        break;
    }
    
    fprintf(stderr, "DebugFunc: length = '%d', id = '%u', userParam = '%x'\n", length, id, (unsigned int)userParam);
    fprintf(stderr, "%s from %s,\t%s priority\nMessage: %s\n",
        errorType.c_str(), srcName.c_str(), typeSeverity.c_str(), message);
    fprintf(stderr, "\n");  // separate this error from the next thing that prints
}

//
//GLenum err = glGetError();
//if (GL_NO_ERROR != err)
//{
//    switch (err)
//    {
//    case GL_INVALID_ENUM:
//        printf("GL error: GL_INVALID_ENUM\n");
//        break;
//    case GL_INVALID_VALUE:
//        printf("GL error: GL_INVALID_VALUE\n");
//        break;
//    case GL_INVALID_OPERATION:
//        printf("GL error: GL_INVALID_OPERATION\n");
//        break;
//    case GL_STACK_OVERFLOW:
//        printf("GL error: GL_STACK_OVERFLOW\n");
//        break;
//    case GL_STACK_UNDERFLOW:
//        printf("GL error: GL_STACK_UNDERFLOW\n");
//        break;
//    case GL_OUT_OF_MEMORY:
//        printf("GL error: GL_OUT_OF_MEMORY\n");
//        break;
//    case GL_INVALID_FRAMEBUFFER_OPERATION:
//        printf("GL error: GL_INVALID_FRAMEBUFFER_OPERATION\n");
//        break;
//        //case GL_CONTEXT_LOST: // OpenGL 4.5 or higher
//        //    break;
//    case GL_TABLE_TOO_LARGE:
//        printf("GL error: GL_TABLE_TOO_LARGE\n");
//        break;
//    default:
//        printf("GL error: UNKNOWN\n");
//        break;
//    }
//}
