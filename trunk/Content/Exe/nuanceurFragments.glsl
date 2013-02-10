
// PROVIENT DE http://www.ozone3d.net/tutorials/glsl_lighting_phong_p3.php

varying vec3 normal, eyeVec;
#define MAX_LIGHTS 8
#define NB_LIGHTS 8
varying vec3 lightDir[MAX_LIGHTS];
varying vec3 positionFragment;
uniform sampler2D colorMap;

void main (void)
{
  
	
	vec4 final_color = (gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient);
	

	int i;
	for (i=0; i<NB_LIGHTS; ++i)
	{
		
		
		float distance = sqrt( dot(lightDir[i], lightDir[i]) );
		float aCon = gl_LightSource[i].constantAttenuation;
		float aLin = gl_LightSource[i].linearAttenuation;
		float aQua = gl_LightSource[i].quadraticAttenuation;
		float attenuation = 1.0/((aCon)+(aLin*distance)+(aQua*distance*distance));
		attenuation = min(attenuation, 1.0);
		
		
		
		final_color += (gl_LightSource[i].ambient * gl_FrontMaterial.ambient)*attenuation;
	
		vec3 L = normalize(lightDir[i]);
		vec3 D = normalize(gl_LightSource[i].spotDirection);
		
		float cos_cur_angle = dot(-L, D);
		
		float cos_inner_cone_angle = gl_LightSource[i].spotCosCutoff;
		float cos_outer_cone_angle = cos_inner_cone_angle-0.05;
		float cos_inner_minus_outer_angle = cos_inner_cone_angle - cos_outer_cone_angle;
		
		// Pour eviter que les spotlights eclairent le plancher sous la table.
		float attenuationSpot = attenuation*gl_LightSource[i].spotExponent;
		
		if(positionFragment.z < -135.0 && gl_LightSource[i].spotCutoff!=180.0)
			attenuationSpot = 1.0;
		
		if (cos_cur_angle > cos_inner_cone_angle) 
		{
			vec3 N = normalize(normal);
			float lambertTerm = max( dot(N,L), 0.0);
			if(lambertTerm > 0.0)
			{
				final_color += gl_LightSource[i].diffuse * gl_FrontMaterial.diffuse * lambertTerm * attenuationSpot;	
			
				vec3 E = normalize(eyeVec);
				vec3 R = reflect(-L, N);
				
				float specular = pow( max(dot(R, E), 0.0), 
				max(1.0, gl_FrontMaterial.shininess) );
				
				final_color += gl_LightSource[i].specular * gl_FrontMaterial.specular * specular * attenuationSpot;	
			}
		}
		else if( cos_cur_angle > cos_outer_cone_angle )
		{
			float falloff = (cos_cur_angle - cos_outer_cone_angle) / cos_inner_minus_outer_angle;
			vec3 N = normalize(normal);
		
			float lambertTerm = max( dot(N,L), 0.0);
			if(lambertTerm > 0.0)
			{
				final_color += gl_LightSource[i].diffuse * gl_FrontMaterial.diffuse * lambertTerm * falloff * attenuationSpot;	
			
				vec3 E = normalize(eyeVec);
				vec3 R = reflect(-L, N);
				
				float specular = pow( max(dot(R, E), 0.0), 
				max(1.0, gl_FrontMaterial.shininess) );
				
				final_color += gl_LightSource[i].specular * gl_FrontMaterial.specular * specular * falloff * attenuationSpot;	
			}
		}
	}
	
	gl_FragColor = final_color*texture2D(colorMap, gl_TexCoord[0].xy);
	
}













