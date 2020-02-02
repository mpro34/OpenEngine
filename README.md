# OpenEngine

This engine will be created using GLFW for window and opengl context creation as well as input management. Over a past engine I created using SDL for these purposes.
I am also use GLEW to use the newer OpenGL functions, as well as GLM for vector math libraries.

I am using GLSL in the `Shaders` directory to program the vertex and fragment shaders. 


## Getting Started

Note: This is running on Windows 10, therefore the following commands are using the mingw gnu for windows.
This project was built with Visual Studio Code, therefore I am using a Makefile and have installed GLEW, GLFW and am using the already installed OpenGL libraries on my windows machine. Reading the `Makefile` will help illustrate what needs to be included to successfully build the project.

The `Makefile` has three commands to `build`, `run` and `clean`.

#### Build the project
To `build` the project, run `mingw32-make` or `mingw32-make build`. Note: the variant of Make that you have installed on your machine. (Again, i'm using mingw).
This will create a `game.exe` executable. 

#### Run the project
To `run` the project, run `mingw32-make run` to execute the `game.exe` created during the build step.

#### Clean the project
Run `mingw32-make clean`, which will clean any executables created during the `build` step.


## Phong Lighting model implementation
##### Ambient and Diffuse Lighting
This image shows diffuse lighting in action on two triangles with different textures.

![Alt text.](https://github.com/mpro34/OpenEngine/blob/master/images/diffuse_lighting1.png)

This image shows another angle of the diffuse lighting on the triangles.

![Alt text.](https://github.com/mpro34/OpenEngine/blob/master/images/diffuse_lighting2.png)


#### Specular Lighting

This image show specular lighting that uses the camera's position when calculating its normals.

![Alt text.](https://github.com/mpro34/OpenEngine/blob/master/images/specular_lighting.png)

Another image of specular lighting up-close.

![Alt text.](https://github.com/mpro34/OpenEngine/blob/master/images/specular_lighting2.png)


#### Items in-Progress
- Get assimp to compile so asset models can be imported.
- Restrict player movement to x,z planes; but allow camera to move in x,y,z planes.
- Create maze of walls, implement horror-like backgrounds.
- Implement collision with walls.
- Implement raycasting and projectiles from player.
- Implement enemy models.
