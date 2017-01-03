#version 440

// position in window space (both X and Y on the range [-1,+1])
layout (location = 0) in vec2 pos;  

// velocity also in window space (ex: an X speed of 1.0 would cross the window horizontally in 2 
// seconds)
layout (location = 1) in vec2 vel;  

layout (location = 2) in int isActive;

// must have the same name as its corresponding "in" item in the frag shader
smooth out vec4 particleColor;

void main()
{
    if (isActive == 0)
    {
        // invisible (alpha = 0), but "fully transparent" does not mean "no color", it merely 
        // means that the color of this thing will be added to the thing behind it (see Z 
        // adjustment later)
        particleColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
        gl_Position = vec4(pos, -0.6f, 1.0f);
    }
    else
    {
        // active => opaque (alpha = 1) white
        particleColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
        gl_Position = vec4(pos, -0.7f, 1.0f);
    }
}

