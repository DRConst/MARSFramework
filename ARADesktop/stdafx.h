// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glfw3dll.lib")
#pragma comment(lib, "SOIL.lib")

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include "glad/glad.h"

#include "GLFW/glfw3.h"

#include "SOIL/SOIL.h"

#include <cstdlib>

#include <vector>
#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"


#include <string>       
#include <sstream>    
#include <fstream>


#include "json.hpp"

using json = nlohmann::json;


// TODO: add component editor
// TODO: add individual buffers for UI strings
// TODO: add class editor to the UI
// TODO: import
