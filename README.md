## ShaderWatch

Simple CLI that watches over a list of glsl files and auto compiles them when they are edited.

The shaders list is set in a `shaders.json`. It supports two interfaces:
- Simple: Adding the shaders by name. The appropriate suffixes will be added
- Complete: Supports multiple compilation variants with output names and preprocessor defines.

See `shaders_example.json` for a quick example.

#### Usage

Place the json file and the exe (see [releases](https://github.com/oneraul/ShaderWatch/releases/tag/0.1)) in the same directory as the shaders. Execute the exe to start watching.

The names of the compiled shaders will be printed in green if the compilation is successful or in red if there are any errors (followed by the error text in grey).

####Links

[libshaderc](https://github.com/google/shaderc/tree/master/libshaderc) - Used internally to compile glsl to Spir-V