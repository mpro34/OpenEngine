#pragma once

#include <vector>
#include <string>

#include <GL\glew.h>

#include "../../libs/glm/glm.hpp"
#include "../../libs/glm/gtc/matrix_transform.hpp"
#include "../../libs/glm/gtc/type_ptr.hpp"
#include "Shader.h"
#include "Mesh.h"

#include "CommonValues.h"

class Skybox {
  public:
    Skybox();

    Skybox(std::vector<std::string> face_locations);

    void DrawSkybox(glm::mat4 view_matrix, glm::mat4 projection_matrix);

    ~Skybox();

  private:
    Mesh *sky_mesh;
    Shader *sky_shader;

    GLuint texture_id;
    GLuint uniform_projection, uniform_view;

};  // SKYBOX_H_
