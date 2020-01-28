#version 330                                          
                                                        
in vec4 vert_color;  
in vec2 tex_coord;    
in vec3 normal;      
in vec3 frag_pos;  
in vec4 directional_light_space_pos;                     
                                                    
out vec4 color;   

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

struct Light {
  vec3 color;
  float ambient_intensity;
  float diffuse_intensity;
};

struct DirectionalLight {
  Light base;
  vec3 direction;
};

struct PointLight {
  Light base;
  vec3 position;
  float constant;
  float linear;
  float exponent;
};

struct SpotLight {
  PointLight base;
  vec3 direction;
  float edge;
};

struct Material {
  float specular_intensity;
  float shininess;
};

uniform int point_light_count;
uniform int spot_light_count;

uniform DirectionalLight directional_light;
uniform PointLight point_lights[MAX_POINT_LIGHTS];
uniform SpotLight spot_lights[MAX_SPOT_LIGHTS];

uniform sampler2D the_texture;
uniform sampler2D directional_shadow_map;

uniform Material material;

uniform vec3 eye_position;

float CalcDirectionalShadowFactor(DirectionalLight light) {
  vec3 proj_coords = directional_light_space_pos.xyz / directional_light_space_pos.w;
  proj_coords = (proj_coords * 0.5) + 0.5;

  float current = proj_coords.z;

  vec3 normal = normalize(normal);
  vec3 light_dir = normalize(light.direction);

  float bias = max(0.05 * (1 - dot(normal, light_dir)), 0.005);

  float shadow = 0.0;

  vec2 texel_size = 1.0 / textureSize(directional_shadow_map, 0);
  for (int x = -1; x <= 1; ++x) {
    for (int y = -1; y <= 1; ++y) {
      float pcf_depth = texture(directional_shadow_map, proj_coords.xy + vec2(x, y) * texel_size).r;
      shadow += current - bias > pcf_depth ? 1.0 : 0.0;
    }
  }

  shadow /= 9.0;

  if (proj_coords.z > 1.0) {
    shadow = 0.0;
  }

  return shadow;
}

vec4 CalcLightByDirection(Light light, vec3 direction, float shadow_factor) {
  // Calculate light by a direction input
  vec4 ambient_color = vec4(light.color, 1.0f) * light.ambient_intensity;          

  float diffuse_factor = max(dot(normalize(normal), normalize(direction)), 0.0f);
  vec4 diffuse_color = vec4(light.color, 1.0f) * light.diffuse_intensity * diffuse_factor;

  vec4 specular_color = vec4(0, 0, 0, 0);
  if (diffuse_factor > 0.0f) {
    vec3 frag_to_eye = normalize(eye_position - frag_pos);
    vec3 reflected_vertex = normalize(reflect(direction, normalize(normal)));

    float specular_factor = dot(frag_to_eye, reflected_vertex);
    if (specular_factor > 0.0f) {
      specular_factor = pow(specular_factor, material.shininess);
      specular_color = vec4(light.color * material.specular_intensity * specular_factor, 1.0f);
    }
  }

  // Phong lighting model implementation
  return (ambient_color + (1.0 - shadow_factor) * (diffuse_color + specular_color));   
}

vec4 CalcDirectionalLight() {
  float shadow_factor = CalcDirectionalShadowFactor(directional_light);
  return CalcLightByDirection(directional_light.base, directional_light.direction, shadow_factor);
}

vec4 CalcPointLight(PointLight p_light) {
  // Calculate the vec4 for a single point light, p_light.
  vec3 direction = frag_pos - p_light.position;
  float distance = length(direction);
  direction = normalize(direction);

  vec4 color = CalcLightByDirection(p_light.base, direction, 0.0f);
  float attenuation = p_light.exponent * distance * distance +
                      p_light.linear * distance +
                      p_light.constant;
  return (color / attenuation);
}

vec4 CalcSpotLight(SpotLight s_light) {
  vec3 ray_direction = normalize(frag_pos - s_light.base.position);
  float sl_factor = dot(ray_direction, s_light.direction);

  if (sl_factor > s_light.edge) {
    vec4 color = CalcPointLight(s_light.base);

    return color * (1.0f - (1.0f - sl_factor)*(1.0f/(1.0f - s_light.edge)));

  } else {
    return vec4(0, 0, 0, 0);
  }
}

vec4 CalcSpotLights() {
  vec4 total_color = vec4(0, 0, 0, 0);
  for (int i=0; i < spot_light_count; i++) {
    
    total_color += CalcSpotLight(spot_lights[i]);
  }

  return total_color;
}

vec4 CalcPointLights() {
  // Calculate total color over all point lights, by finding the distance between each fragment and point light position
  // Then using ax^2 + bx + c from the point_lights struct to calculate an attenuation factor.
  vec4 total_color = vec4(0, 0, 0, 0);
  for (int i=0; i < point_light_count; i++) {
    
    total_color += CalcPointLight(point_lights[i]);
  }

  return total_color;
}
                                                      
void main() {            
  vec4 final_color = CalcDirectionalLight();
  final_color += CalcPointLights();
  final_color += CalcSpotLights();

  color = texture(the_texture, tex_coord) * final_color;  // Phong lighting model implementation                                   
}
