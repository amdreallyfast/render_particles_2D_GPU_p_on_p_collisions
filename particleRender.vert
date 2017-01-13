#version 440

// Note: The vec2's are in window space (both X and Y on the range [-1,+1])
// Also Note: The vec2s are provided as vec4s on the CPU side and specified as such in the 
// vertex array attributes, but it is ok to only take them as a vec2, as I am doing for this 
// 2D demo.
layout (location = 0) in vec2 pos;  
layout (location = 1) in vec2 vel;  
layout (location = 2) in vec2 netForce;
layout (location = 3) in int collisionCountThisFrame;
layout (location = 4) in float mass;
layout (location = 5) in float radiusOfInfluence;
layout (location = 6) in int isActive;

// must have the same name as its corresponding "in" item in the frag shader
smooth out vec4 particleColor;

void main()
{
    if (isActive == 0)
    {
        // invisible (alpha = 0), but "fully transparent" does not mean "no color", it merely 
        // means that the color of this thing will be added to the thing behind it (see Z 
        // adjustment later)
        particleColor = vec4(0.0f, 1.0f, 0.0f, 0.0f);
        gl_Position = vec4(pos, -0.6f, 1.0f);
    }
    else
    {
        // active => opaque (alpha = 1) white
        particleColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
        gl_Position = vec4(pos, -0.7f, 1.0f);
    }
}

