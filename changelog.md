# Change Log

## [0.0.7] - 2023-04-02
### Added
- counting and displaying deltaTime and FramesPerSecond


## [0.0.6] - 2023-03-31

### Added
- rapidjson lib
- effolkronium/random lib
- wqking/eventpp lib
- Neargye/magic_enum lib
- fmt lib

### Fixed
- removed openAL lib from cmake and Game.cpp because its fucked

## [0.0.5] - 2023-03-30
### Added
- point light
- camera 

### Changed
- frag shader
- lights are working with obj

### Fixed
- spot light

## [0.0.4] - 2023-03-29
### Added
- class Transform with Position, Rotation and Scale
- class Object3D containing Transform and Model
- rendering 3d object affected by light

- directional light
- spot light (but i cant locate it on the cube, so i'm not sure if it works tbh)
### Changed
- both shaders

## [0.0.3] - 2023-03-28
### Tip
you can install GLSL extension to Clion easily, and it helps a lot


- lights hierarchy
- loading and rendering 2 obj Models

### Todo
- fix models' textures
- add transform to each individual one

### Changed
- in Sierpinski.draw() changed 12 to 36
- drawing cube instead of triangle

## [0.0.2] - 2023-03-27

### Added
- displaying Sierpinski's Tetrahedron instead of Hello World Triangle
- can change color, rotation and recursion depth
- class Mesh
- class Model loading Meshes and Textures via Assimp
### Changed
- moved project to CLion

## [0.0.1] - 2023-03-21

### Added
- displaying "Hello world"

