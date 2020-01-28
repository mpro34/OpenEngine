#version 330                                                        
                                                                    
layout (location = 0) in vec3 pos;  
layout (location = 1) in vec2 tex;      
layout (location = 2) in vec3 norm;                          
                                                                    
out vec4 vert_color;              
out vec2 tex_coord;  
out vec3 normal;   
out vec3 frag_pos;     
out vec4 directional_light_space_pos;                        
                                                                    
uniform mat4 model;                                                 
uniform mat4 projection;                                            
uniform mat4 view;
uniform mat4 directional_light_transform;

void main() {                                                       
  gl_Position = projection * view * model * vec4(pos, 1.0f);    
  directional_light_space_pos = directional_light_transform * model * vec4(pos, 1.0);

  vert_color = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);       
  tex_coord = tex;      
  normal = mat3(transpose(inverse(model))) * norm;    
  frag_pos = (model * vec4(pos, 1.0f)).xyz;
}