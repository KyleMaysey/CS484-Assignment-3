//Assignment 3: GLSL
//Phong illumination - vertex shader
//Author: Kyle Maysey

varying vec3 Normal, LightVec, HalfVec, ReflVec, Vert;

void main()
{

	Normal = normalize(gl_NormalMatrix * gl_Normal);	//transform the normal into eye space coordinates
	Vert = gl_ModelViewMatrix * gl_Vertex;

	LightVec = normalize(vec3(gl_LightSource[0].position) - (Vert) );	//Find the light vector for surface
	// "- (gl_ModelViewMatrix * gl_Vertex)" Removed from end of function above

	//Using halfvector from the Blinn-Phong shader as OpenGL already calculates it.
	HalfVec = normalize( gl_LightSource[0].halfVector.xyz);

	ReflVec = reflect(LightVec, Normal);
	// Above should calculate this: -LightVec + 2 * dot(LightVec, Normal) * Normal;

	gl_Position = ftransform();

}