#ifndef IMGUI_HELPER_H_
#define IMGUI_HELPER_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include "../contrib/imgui/include/imgui.h"
#include "../contrib/imgui/include/imgui_impl_glfw.h"
#include "../contrib/imgui/include/imgui_impl_opengl3.h"

/*Uniform helpers*/
void imguiGetShaderAttribute(GLuint program, GLuint index, GLsizei &length, GLint &size, GLenum &type, GLchar* name){
    const GLsizei bufSize = 16;
    glGetActiveAttrib(program, index, bufSize, &length, &size, &type, name);
}

void imguiGetShaderUniform(GLuint program, GLuint index, GLsizei &length, GLint &size, GLenum &type, GLchar* name){
    const GLsizei bufSize = 16;
    glGetActiveUniform(program, index, bufSize, &length, &size, &type, name);
}

std::string getStringFromGlType(const GLenum &type){
    //All relevant types for https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetActiveAttrib.xhtml
    std::string s;
    switch(type){
        /*Required for attributes*/
        case GL_FLOAT: s = "GL_FLOAT"; break;
        case GL_FLOAT_VEC2: s = "GL_FLOAT_VEC2"; break; 
        case GL_FLOAT_VEC3: s = "GL_FLOAT_VEC3"; break; 
        case GL_FLOAT_VEC4: s = "GL_FLOAT_VEC4"; break; 
        case GL_FLOAT_MAT2: s = "GL_FLOAT_MAT2"; break; 
        case GL_FLOAT_MAT3: s = "GL_FLOAT_MAT3"; break; 
        case GL_FLOAT_MAT4: s = "GL_FLOAT_MAT4"; break; 
        case GL_FLOAT_MAT2x3: s = "GL_FLOAT_MAT2x3"; break;
        case GL_FLOAT_MAT2x4: s = "GL_FLOAT_MAT2x4"; break;
        case GL_FLOAT_MAT3x2: s = "GL_FLOAT_MAT3x2"; break;
        case GL_FLOAT_MAT3x4: s = "GL_FLOAT_MAT3x4"; break;
        case GL_FLOAT_MAT4x2: s = "GL_FLOAT_MAT4x2"; break;
        case GL_FLOAT_MAT4x3: s = "GL_FLOAT_MAT4x3"; break;
        case GL_INT: s = "GL_INT"; break; 
        case GL_INT_VEC2: s = "GL_INT_VEC2"; break; 
        case GL_INT_VEC3: s = "GL_INT_VEC3"; break; 
        case GL_INT_VEC4: s = "GL_INT_VEC4"; break; 
        case GL_UNSIGNED_INT: s = "GL_UNSIGNED_INT"; break;
        case GL_UNSIGNED_INT_VEC2: s = "GL_UNSIGNED_INT_VEC2"; break;
        case GL_UNSIGNED_INT_VEC3: s = "GL_UNSIGNED_INT_VEC3"; break;
        case GL_UNSIGNED_INT_VEC4: s = "GL_UNSIGNED_INT_VEC4"; break;
        case GL_DOUBLE: s = "GL_DOUBLE"; break;
        case GL_DOUBLE_VEC2: s = "GL_DOUBLE_VEC2"; break;
        case GL_DOUBLE_VEC3: s = "GL_DOUBLE_VEC3"; break;
        case GL_DOUBLE_VEC4: s = "GL_DOUBLE_VEC4"; break;
        case GL_DOUBLE_MAT2: s = "GL_DOUBLE_MAT2"; break;
        case GL_DOUBLE_MAT3: s = "GL_DOUBLE_MAT3"; break;
        case GL_DOUBLE_MAT4: s = "GL_DOUBLE_MAT4"; break;
        case GL_DOUBLE_MAT2x3: s = "GL_DOUBLE_MAT2x3"; break; 
        case GL_DOUBLE_MAT2x4: s = "GL_DOUBLE_MAT2x4"; break; 
        case GL_DOUBLE_MAT3x2: s = "GL_DOUBLE_MAT3x2"; break; 
        case GL_DOUBLE_MAT3x4: s = "GL_DOUBLE_MAT3x4"; break; 
        case GL_DOUBLE_MAT4x2: s = "GL_DOUBLE_MAT4x2"; break; 
        case GL_DOUBLE_MAT4x3: s = "GL_DOUBLE_MAT4x3"; break;

        /*Required for uniforms*/
        case GL_BOOL: s = "GL_BOOL"; break;
        case GL_BOOL_VEC2: s = "GL_BOOL_VEC2"; break;
        case GL_BOOL_VEC3: s = "GL_BOOL_VEC3"; break;
        case GL_BOOL_VEC4: s = "GL_BOOL_VEC4"; break;
        case GL_SAMPLER_1D: s = "GL_SAMPLER_1D"; break;
        case GL_SAMPLER_2D: s = "GL_SAMPLER_2D"; break;
        case GL_SAMPLER_3D: s = "GL_SAMPLER_3D"; break;
        case GL_SAMPLER_CUBE: s = "GL_SAMPLER_CUBE"; break;
        case GL_SAMPLER_1D_SHADOW: s = "GL_SAMPLER_1D_SHADOW"; break;
        case GL_SAMPLER_2D_SHADOW: s = "GL_SAMPLER_2D_SHADOW"; break;
        case GL_SAMPLER_1D_ARRAY: s = "GL_SAMPLER_1D_ARRAY"; break;
        case GL_SAMPLER_2D_ARRAY: s = "GL_SAMPLER_2D_ARRAY"; break;
        case GL_SAMPLER_1D_ARRAY_SHADOW: s = "GL_SAMPLER_1D_ARRAY_SHADOW"; break;
        case GL_SAMPLER_2D_ARRAY_SHADOW: s = "GL_SAMPLER_2D_ARRAY_SHADOW"; break;
        case GL_SAMPLER_2D_MULTISAMPLE: s = "GL_SAMPLER_2D_MULTISAMPLE"; break;
        case GL_SAMPLER_2D_MULTISAMPLE_ARRAY: s = "GL_SAMPLER_2D_MULTISAMPLE_ARRAY"; break;
        case GL_SAMPLER_CUBE_SHADOW: s = "GL_SAMPLER_CUBE_SHADOW"; break;
        case GL_SAMPLER_BUFFER: s = "GL_SAMPLER_BUFFER"; break;
        case GL_SAMPLER_2D_RECT: s = "GL_SAMPLER_2D_RECT"; break;
        case GL_SAMPLER_2D_RECT_SHADOW: s = "GL_SAMPLER_2D_RECT_SHADOW"; break;
        case GL_INT_SAMPLER_1D: s = "GL_INT_SAMPLER_1D"; break;
        case GL_INT_SAMPLER_2D: s = "GL_INT_SAMPLER_2D"; break;
        case GL_INT_SAMPLER_3D: s = "GL_INT_SAMPLER_3D"; break;
        case GL_INT_SAMPLER_CUBE: s = "GL_INT_SAMPLER_CUBE"; break;
        case GL_INT_SAMPLER_1D_ARRAY: s = "GL_INT_SAMPLER_1D_ARRAY"; break;
        case GL_INT_SAMPLER_2D_ARRAY: s = "GL_INT_SAMPLER_2D_ARRAY"; break;
        case GL_INT_SAMPLER_2D_MULTISAMPLE: s = "GL_INT_SAMPLER_2D_MULTISAMPLE"; break;
        case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: s = "GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY"; break;
        case GL_INT_SAMPLER_BUFFER: s = "GL_INT_SAMPLER_BUFFER"; break;
        case GL_INT_SAMPLER_2D_RECT: s = "GL_INT_SAMPLER_2D_RECT"; break;
        case GL_UNSIGNED_INT_SAMPLER_1D: s = "GL_UNSIGNED_INT_SAMPLER_1D"; break;
        case GL_UNSIGNED_INT_SAMPLER_2D: s = "GL_UNSIGNED_INT_SAMPLER_2D"; break;
        case GL_UNSIGNED_INT_SAMPLER_3D: s = "GL_UNSIGNED_INT_SAMPLER_3D"; break;
        case GL_UNSIGNED_INT_SAMPLER_CUBE: s = "GL_UNSIGNED_INT_SAMPLER_CUBE"; break;
        case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY: s = "GL_UNSIGNED_INT_SAMPLER_1D_ARRAY"; break;
        case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY: s = "GL_UNSIGNED_INT_SAMPLER_2D_ARRAY"; break;
        case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE: s = "GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE"; break;
        case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: s = "GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY"; break;
        case GL_UNSIGNED_INT_SAMPLER_BUFFER: s = "GL_UNSIGNED_INT_SAMPLER_BUFFER"; break;
        case GL_UNSIGNED_INT_SAMPLER_2D_RECT: s = "GL_UNSIGNED_INT_SAMPLER_2D_RECT"; break;
        case GL_IMAGE_1D: s = "GL_IMAGE_1D"; break;
        case GL_IMAGE_2D: s = "GL_IMAGE_2D"; break;
        case GL_IMAGE_3D: s = "GL_IMAGE_3D"; break;
        case GL_IMAGE_2D_RECT: s = "GL_IMAGE_2D_RECT"; break;
        case GL_IMAGE_CUBE: s = "GL_IMAGE_CUBE"; break;
        case GL_IMAGE_BUFFER: s = "GL_IMAGE_BUFFER"; break;
        case GL_IMAGE_1D_ARRAY: s = "GL_IMAGE_1D_ARRAY"; break;
        case GL_IMAGE_2D_ARRAY: s = "GL_IMAGE_2D_ARRAY"; break;
        case GL_IMAGE_2D_MULTISAMPLE: s = "GL_IMAGE_2D_MULTISAMPLE"; break;
        case GL_IMAGE_2D_MULTISAMPLE_ARRAY: s = "GL_IMAGE_2D_MULTISAMPLE_ARRAY"; break;
        case GL_INT_IMAGE_1D: s = "GL_INT_IMAGE_1D"; break;
        case GL_INT_IMAGE_2D: s = "GL_INT_IMAGE_2D"; break;
        case GL_INT_IMAGE_3D: s = "GL_INT_IMAGE_3D"; break;
        case GL_INT_IMAGE_2D_RECT: s = "GL_INT_IMAGE_2D_RECT"; break;
        case GL_INT_IMAGE_CUBE: s = "GL_INT_IMAGE_CUBE"; break;
        case GL_INT_IMAGE_BUFFER: s = "GL_INT_IMAGE_BUFFER"; break;
        case GL_INT_IMAGE_1D_ARRAY: s = "GL_INT_IMAGE_1D_ARRAY"; break;
        case GL_INT_IMAGE_2D_ARRAY: s = "GL_INT_IMAGE_2D_ARRAY"; break;
        case GL_INT_IMAGE_2D_MULTISAMPLE: s = "GL_INT_IMAGE_2D_MULTISAMPLE"; break;
        case GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY: s = "GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY"; break;
        case GL_UNSIGNED_INT_IMAGE_1D: s = "GL_UNSIGNED_INT_IMAGE_1D"; break;
        case GL_UNSIGNED_INT_IMAGE_2D: s = "GL_UNSIGNED_INT_IMAGE_2D"; break;
        case GL_UNSIGNED_INT_IMAGE_3D: s = "GL_UNSIGNED_INT_IMAGE_3D"; break;
        case GL_UNSIGNED_INT_IMAGE_2D_RECT: s = "GL_UNSIGNED_INT_IMAGE_2D_RECT"; break;
        case GL_UNSIGNED_INT_IMAGE_CUBE: s = "GL_UNSIGNED_INT_IMAGE_CUBE"; break;
        case GL_UNSIGNED_INT_IMAGE_BUFFER: s = "GL_UNSIGNED_INT_IMAGE_BUFFER"; break;
        case GL_UNSIGNED_INT_IMAGE_1D_ARRAY: s = "GL_UNSIGNED_INT_IMAGE_1D_ARRAY"; break;
        case GL_UNSIGNED_INT_IMAGE_2D_ARRAY: s = "GL_UNSIGNED_INT_IMAGE_2D_ARRAY"; break;
        case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE: s = "GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE"; break;
        case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY: s = "GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY"; break;
        case GL_UNSIGNED_INT_ATOMIC_COUNTER: s = "GL_UNSIGNED_INT_ATOMIC_COUNTER"; break;
        default: s = "ERROR_NO_TYPE_FOUND"; break;
    }

    return s;
}

void imguiShaderInspector(GLuint program){
    if (ImGui::CollapsingHeader("Shader Inspector")){   
        /*******************
         *Current Attributes
        *******************/
        ImGui::Text("Attributes:");
        ImGui::Columns(4, "AttributesColumns"); // 4-ways, with border
        ImGui::Separator();
        ImGui::Text("ID"); ImGui::NextColumn();
        ImGui::Text("Name"); ImGui::NextColumn();
        ImGui::Text("Type"); ImGui::NextColumn();
        ImGui::Text("Size"); ImGui::NextColumn();
        ImGui::Separator();

        /*Fetch opengl data*/
        GLint count;
        GLint size;
        GLenum type;
        const GLsizei bufSize = 16; // maximum name length
        GLchar name[bufSize]; // variable name in GLSL
        GLsizei length; // name length
        glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &count);

        for (GLint i = 0; i < count; i++){
            imguiGetShaderAttribute(program, (GLuint)i, length, size, type, name);
            ImGui::Text("%i", i); ImGui::NextColumn();
            ImGui::Text("%s", name); ImGui::NextColumn();
            ImGui::Text("%s", getStringFromGlType(type).c_str()); ImGui::NextColumn(); 
            ImGui::Text("%i", size); ImGui::NextColumn();
        }

        ImGui::Columns(1);
        ImGui::Separator();

        /*******************
         *Current Uniforms
        *******************/
        ImGui::Text("Uniforms:");
        ImGui::Columns(5, "UniformsColumns");
        ImGui::Separator();
        ImGui::Text("ID"); ImGui::NextColumn();
        ImGui::Text("Name"); ImGui::NextColumn();
        ImGui::Text("Type"); ImGui::NextColumn();
        ImGui::Text("Size"); ImGui::NextColumn();
        ImGui::Text("Values"); ImGui::NextColumn();
        ImGui::Separator();

        glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);

        for (GLint i = 0; i < count; i++){
            imguiGetShaderUniform(program, (GLuint)i, length, size, type, name);
            ImGui::Text("%i", i); ImGui::NextColumn();
            ImGui::Text("%s", name); ImGui::NextColumn();
            ImGui::Text("%s", getStringFromGlType(type).c_str()); ImGui::NextColumn(); 
            ImGui::Text("%i", size); ImGui::NextColumn();

            /*Uniform values*/
            GLfloat output[16];
            glGetnUniformfv(program, i, 16 * sizeof(GLfloat), output);
            for (int i = 0; i < 4; i++){
                for (int j = 0; j < 4; j++){
                    if(j != 0){
                        ImGui::SameLine();
                    }
                    ImGui::Button(std::to_string(output[4 * j + i]).c_str(), ImVec2(60,20));
                }
            }

            ImGui::NextColumn();
        }

        ImGui::Columns(1);
        ImGui::Separator();
    }
}

void hardwareSpecs(){
/*https://www.khronos.org/opengl/wiki/OpenGL_Context#Context_information_queries*/
    if (ImGui::CollapsingHeader("Specs")){
        ImGui::Text("OpenGl specs:");
        const char* glver = (char*)glGetString(GL_VERSION);
        ImGui::BulletText("%s", glver);

        GLint version = 0;
        glGetIntegerv(GL_MAJOR_VERSION, &version);
        ImGui::BulletText("Major %i", version);
        glGetIntegerv(GL_MINOR_VERSION, &version);
        ImGui::BulletText("Minor %i", version);

        glver = (char*)glGetString(GL_RENDERER);
        ImGui::BulletText("%s", glver);

        ImGui::Separator();
        ImGui::Text("GLSL specs:");
        GLint n=0; 
        glGetIntegerv(GL_NUM_SHADING_LANGUAGE_VERSIONS, &n); 
        for (GLint i=0; i < n; i++) { 
            const char* glsls = (const char*)glGetStringi(GL_SHADING_LANGUAGE_VERSION, i);
            ImGui::BulletText("%s", glsls);
        }
    }
}

void extensions(){
/*https://www.khronos.org/opengl/wiki/OpenGL_Context#Context_information_queries*/
    if(ImGui::CollapsingHeader("Extensions")){
        GLint n=0; 
        glGetIntegerv(GL_NUM_EXTENSIONS, &n); 
        for (GLint i=0; i < n; i++) { 
            const char* extension = (const char*)glGetStringi(GL_EXTENSIONS, i);
            ImGui::BulletText("%s", extension);
        }
    }
}

#endif /* IMGUI_HELPER_H_ */

/*Widgets -> Plots Widgets in ImguiDemo*/