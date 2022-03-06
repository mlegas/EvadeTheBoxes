uniform sampler2D in_Texture;
uniform vec3 in_Emissive;
uniform vec3 in_Ambient;
uniform vec3 in_LightPos;
 
uniform mat4 in_View;

varying vec2 ex_TexCoord;
varying vec3 ex_FragPos;
varying vec3 ex_Normal;

void main()
{
  vec4 tex = texture2D(in_Texture, ex_TexCoord);
  vec3 lighting = in_Emissive + in_Ambient;
  vec3 norm = normalize(ex_Normal);
  vec3 lightDir = normalize(in_LightPos - ex_FragPos);
  
  float diff = max(dot(norm, lightDir), 0.0);
  
  vec3 diffuse = diff * vec3(1,1,1);
  
  vec4 viewPos = inverse(in_View) * vec4(0,0,0,1);
  vec3 viewDir = normalize(vec3(viewPos) - ex_FragPos);
  vec3 halfwayDir = normalize(lightDir + viewDir);
  float spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0);
  
  vec3 specular = spec * vec3(1,1,1);
  
  lighting = lighting + diffuse + specular;
  gl_FragColor = vec4(lighting, 1) * tex;
}