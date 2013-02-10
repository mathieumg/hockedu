varying vec3 normal, eyeVec;
#define MAX_LIGHTS 8
#define NB_LIGHTS 8
varying vec3 lightDir[MAX_LIGHTS];
varying vec3 positionFragment;
void main()
{
	
  
  // calculer la normale qui sera interpolée pour le nuanceur de fragment
   normal = gl_NormalMatrix * gl_Normal;
   //normal = normalize( gl_NormalMatrix * gl_Normal );

   // calculer la position du sommet dans l'espace de la caméra ("eye-coordinate position")
   vec3 ecPosition = vec3( gl_ModelViewMatrix * gl_Vertex );

   // vecteur de la direction de la lumière
   int i;
   for (i=0; i<NB_LIGHTS; ++i)
   {
		vec4 positionLumiere = gl_LightSource[i].position;
		
		lightDir[i] = vec3( positionLumiere.xyz - ecPosition*positionLumiere.w );
	}
		
   eyeVec = -ecPosition; // vecteur qui pointe vers l'oeil
	
  
   gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
   
   // transformation standard du sommet (ModelView et Projection)
   gl_Position = ftransform();
   positionFragment = gl_Vertex.xyz;
}
