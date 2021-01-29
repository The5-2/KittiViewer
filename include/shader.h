#ifndef SHADER_H
#define SHADER_H
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>

class Shader
{
public:
	Shader() = default;
	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
	Shader(const char* computePath);
	~Shader();
	
	void enable();
	void disable();

	GLuint getProgramId() const{
		return programId;
	}

	/*Bool*/
	void uniform(const std::string& name, bool value) const;
	void uniform(const std::string& name, const glm::bvec2& value) const;
	void uniform(const std::string& name, bool x, bool y) const;
	void uniform(const std::string& name, const glm::bvec3& value) const;
	void uniform(const std::string& name, bool x, bool y, bool z) const;
	void uniform(const std::string& name, const glm::bvec4& value) const;
	void uniform(const std::string& name, bool x, bool y, bool z, bool w) const;

	/*Int*/
	void uniform(const std::string& name, int value) const;
	void uniform(const std::string& name, const glm::ivec2& value) const;
	void uniform(const std::string& name, int x, int y) const;
	void uniform(const std::string& name, const glm::ivec3& value) const;
	void uniform(const std::string& name, int x, int y, int z) const;
	void uniform(const std::string& name, const glm::ivec4& value) const;
	void uniform(const std::string& name, int x, int y, int z, int w) const;

	/*Unsigned Int*/
	void uniform(const std::string& name, unsigned int value) const;
	void uniform(const std::string& name, const glm::uvec2& value) const;
	void uniform(const std::string& name, unsigned int x, unsigned int y) const;
	void uniform(const std::string& name, const glm::uvec3& value) const;
	void uniform(const std::string& name, unsigned int x, unsigned int y, unsigned int z) const;
	void uniform(const std::string& name, const glm::uvec4& value) const;
	void uniform(const std::string& name, unsigned int x, unsigned int y, unsigned int z, unsigned int w) const;

	/*Float*/
	void uniform(const std::string& name, float value) const;
	void uniform(const std::string& name, const glm::vec2& value) const;
	void uniform(const std::string& name, float x, float y) const;
	void uniform(const std::string& name, const glm::vec3& value) const;
	void uniform(const std::string& name, float x, float y, float z) const;
	void uniform(const std::string& name, const glm::vec4& value) const;
	void uniform(const std::string& name, float x, float y, float z, float w);

	void uniform(const std::string& name, const glm::mat2x2& mat, GLboolean transpose = GL_FALSE) const;
	void uniform(const std::string& name, const glm::mat2x3& mat, GLboolean transpose = GL_FALSE) const;
	void uniform(const std::string& name, const glm::mat2x4& mat, GLboolean transpose = GL_FALSE) const;

	void uniform(const std::string& name, const glm::mat3x2& mat, GLboolean transpose = GL_FALSE) const;
	void uniform(const std::string& name, const glm::mat3x3& mat, GLboolean transpose = GL_FALSE) const;
	void uniform(const std::string& name, const glm::mat3x4& mat, GLboolean transpose = GL_FALSE) const;

	void uniform(const std::string& name, const glm::mat4x2& mat, GLboolean transpose = GL_FALSE) const;
	void uniform(const std::string& name, const glm::mat4x3& mat, GLboolean transpose = GL_FALSE) const;
	void uniform(const std::string& name, const glm::mat4x4& mat, GLboolean transpose = GL_FALSE) const;

	/*Double*/
	void uniform(const std::string& name, double value) const;
	void uniform(const std::string& name, const glm::dvec2& value) const;
	void uniform(const std::string& name, double x, double y) const;
	void uniform(const std::string& name, const glm::dvec3& value) const;
	void uniform(const std::string& name, double x, double y, double z) const;
	void uniform(const std::string& name, const glm::dvec4& value) const;
	void uniform(const std::string& name, double x, double y, double z, double w);

	void uniform(const std::string& name, const glm::dmat2x2& mat, GLboolean transpose = GL_FALSE) const;
	void uniform(const std::string& name, const glm::dmat2x3& mat, GLboolean transpose = GL_FALSE) const;
	void uniform(const std::string& name, const glm::dmat2x4& mat, GLboolean transpose = GL_FALSE) const;

	void uniform(const std::string& name, const glm::dmat3x2& mat, GLboolean transpose = GL_FALSE) const;
	void uniform(const std::string& name, const glm::dmat3x3& mat, GLboolean transpose = GL_FALSE) const;
	void uniform(const std::string& name, const glm::dmat3x4& mat, GLboolean transpose = GL_FALSE) const;

	void uniform(const std::string& name, const glm::dmat4x2& mat, GLboolean transpose = GL_FALSE) const;
	void uniform(const std::string& name, const glm::dmat4x3& mat, GLboolean transpose = GL_FALSE) const;
	void uniform(const std::string& name, const glm::dmat4x4& mat, GLboolean transpose = GL_FALSE) const;

private:
	GLuint programId = 0;

	void checkCompileErrors(GLuint shader, std::string type);
	static std::string textFileRead(const char *fileName);
};
#endif