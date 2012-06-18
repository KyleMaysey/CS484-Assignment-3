//Assignment 3: GLSL
//Phong illumination - Fragment shader
//Author: Kyle Maysey

varying vec3 Normal, LightVec, HalfVec, ReflVec, Vert;

void main()
{
	vec4 color = vec4(0.0, 1.0, 0.0, 1.0);	//Hopefully the vec3 will ignore the alpha with no issues
	vec4 ambient = vec4( 0.0, 0.1, 0.0, 1.0);
	float NdotL, NdotH, EdotR;						//Variables used to store the dot products of variables

	vec3 EyeVec = normalize( -Vert );

	NdotL = max( dot(Normal, LightVec), 0.0);

	if(NdotL > 0.0)
	{
		//NdotH = max( dot( Normal, HalfVec), 0.0);
		EdotR = max( dot( EyeVec, ReflVec), 0.0);

		color = ambient + (color * NdotL) +  (color * pow( EdotR, 0.5));
		//Taken from beginning of equation ""
		//Took out max(ambient, ###)
	}
	else
	{
		color = ambient;
	}
	
	
	
	gl_FragColor = color;
}