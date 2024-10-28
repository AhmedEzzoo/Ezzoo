#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TextCoord;
layout(location = 3) in float a_TextSlot;
layout(location = 4) in float a_TilingFactor;
layout(location = 5) in int a_EntityID;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
	
};

struct VertexOutput
{
	vec4 Color;
	vec2 TextCoord;
	float TilingFactor;
};


layout (location = 0) out VertexOutput Output;
layout (location = 3) out flat float v_TextSlot;
layout (location = 4) out flat int v_EntityID;

void main()
{
	Output.Color = a_Color;
	Output.TextCoord = a_TextCoord;
	Output.TilingFactor = a_TilingFactor;

	v_TextSlot = a_TextSlot;
	v_EntityID = a_EntityID;


	gl_Position = u_ViewProjection * vec4(a_Position, 1.0f);
}


#type fragment
#version 450 core

layout(location = 0) out vec4 color;
layout(location = 1) out int EntityID;

struct VertexOutput
{
	vec4 Color;
	vec2 TextCoord;
	float TilingFactor;
};



layout (location= 0) in VertexOutput Input;
layout (location= 3) in flat float v_TextSlot;
layout (location= 4) in flat int v_EntityID;


layout (binding = 0) uniform sampler2D u_Texture[32];

void main()
{

	vec4 texColor = Input.Color;

	
	switch(int(v_TextSlot))
	{
		case  0: texColor *= texture(u_Texture[0], Input.TextCoord * Input.TilingFactor); break;
		case  1: texColor *= texture(u_Texture[1], Input.TextCoord * Input.TilingFactor); break;
		case  2: texColor *= texture(u_Texture[2], Input.TextCoord * Input.TilingFactor); break;
		case  3: texColor *= texture(u_Texture[3], Input.TextCoord * Input.TilingFactor); break;
		case  4: texColor *= texture(u_Texture[4], Input.TextCoord * Input.TilingFactor); break;
		case  5: texColor *= texture(u_Texture[5], Input.TextCoord * Input.TilingFactor); break;
		case  6: texColor *= texture(u_Texture[6], Input.TextCoord * Input.TilingFactor); break;
		case  7: texColor *= texture(u_Texture[7], Input.TextCoord * Input.TilingFactor); break;
		case  8: texColor *= texture(u_Texture[8], Input.TextCoord * Input.TilingFactor); break;
		case  9: texColor *= texture(u_Texture[9], Input.TextCoord * Input.TilingFactor); break;
		case 10: texColor *= texture(u_Texture[10], Input.TextCoord * Input.TilingFactor); break;
		case 11: texColor *= texture(u_Texture[11], Input.TextCoord * Input.TilingFactor); break;
		case 12: texColor *= texture(u_Texture[12], Input.TextCoord * Input.TilingFactor); break;
		case 13: texColor *= texture(u_Texture[13], Input.TextCoord * Input.TilingFactor); break;
		case 14: texColor *= texture(u_Texture[14], Input.TextCoord * Input.TilingFactor); break;
		case 15: texColor *= texture(u_Texture[15], Input.TextCoord * Input.TilingFactor); break;
		case 16: texColor *= texture(u_Texture[16], Input.TextCoord * Input.TilingFactor); break;
		case 17: texColor *= texture(u_Texture[17], Input.TextCoord * Input.TilingFactor); break;
		case 18: texColor *= texture(u_Texture[18], Input.TextCoord * Input.TilingFactor); break;
		case 19: texColor *= texture(u_Texture[19], Input.TextCoord * Input.TilingFactor); break;
		case 20: texColor *= texture(u_Texture[20], Input.TextCoord * Input.TilingFactor); break;
		case 21: texColor *= texture(u_Texture[21], Input.TextCoord * Input.TilingFactor); break;
		case 22: texColor *= texture(u_Texture[22], Input.TextCoord * Input.TilingFactor); break;
		case 23: texColor *= texture(u_Texture[23], Input.TextCoord * Input.TilingFactor); break;
		case 24: texColor *= texture(u_Texture[24], Input.TextCoord * Input.TilingFactor); break;
		case 25: texColor *= texture(u_Texture[25], Input.TextCoord * Input.TilingFactor); break;
		case 26: texColor *= texture(u_Texture[26], Input.TextCoord * Input.TilingFactor); break;
		case 27: texColor *= texture(u_Texture[27], Input.TextCoord * Input.TilingFactor); break;
		case 28: texColor *= texture(u_Texture[28], Input.TextCoord * Input.TilingFactor); break;
		case 29: texColor *= texture(u_Texture[29], Input.TextCoord * Input.TilingFactor); break;
		case 30: texColor *= texture(u_Texture[30], Input.TextCoord * Input.TilingFactor); break;
		case 31: texColor *= texture(u_Texture[31], Input.TextCoord * Input.TilingFactor); break;
	}
	color = texColor;

	if (color.a == 0.0)
		discard;

	EntityID = v_EntityID;
}