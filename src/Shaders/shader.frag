#version 330                                          
                                                        
in vec4 vert_color;  
in vec2 tex_coord;    
in vec3 normal;      
in vec3 frag_pos;                       
                                                    
out vec4 color;   

struct DirectionalLight {
  vec3 color;
  float ambient_intensity;
  vec3 direction;
  float diffuse_intensity;
};

struct Material {
  float specular_intensity;
  float shininess;
};

uniform sampler2D the_texture;
uniform DirectionalLight directional_light;
uniform Material material;

uniform vec3 eye_position;
                                                      
void main() {            
  vec4 ambient_color = vec4(directional_light.color, 1.0f) * directional_light.ambient_intensity;          

  float diffuse_factor = max(dot(normalize(normal), normalize(directional_light.direction)), 0.0f);
  vec4 diffuse_color = vec4(directional_light.color, 1.0f) * directional_light.diffuse_intensity * diffuse_factor;

  vec4 specular_color = vec4(0, 0, 0, 0);
  if (diffuse_factor > 0.0f) {
    vec3 frag_to_eye = normalize(eye_position - frag_pos);
    vec3 reflected_vertex = normalize(reflect(directional_light.direction, normalize(normal)));

    float specular_factor = dot(frag_to_eye, reflected_vertex);
    if (specular_factor > 0.0f) {
      specular_factor = pow(specular_factor, material.shininess);
      specular_color = vec4(directional_light.color * material.specular_intensity * specular_factor, 1.0f);
    }
  }

  color = texture(the_texture, tex_coord) * (ambient_color + diffuse_color + specular_color);  // Phong lighting model implementation                                   
}
