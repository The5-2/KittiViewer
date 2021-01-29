#include "../include/shader.h"

Shader::~Shader(){
	glDeleteProgram(programId);
	programId = 0;
}

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	printf("Loading shader:\n %s \n %s \n", vertexPath, fragmentPath);
	
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream gShaderFile;

	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		// if geometry shader path is present, also load a geometry shader
		if (geometryPath != nullptr)
		{
			gShaderFile.open(geometryPath);
			std::stringstream gShaderStream;
			gShaderStream << gShaderFile.rdbuf();
			gShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::ifstream::failure e)
	{
		vShaderFile.close();
		fShaderFile.close();
		std::cout << "ERROR: Shader file could not be read, check if it is in the right folder!\n" << vertexPath << "\n" << fragmentPath << "\n" << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char * fShaderCode = fragmentCode.c_str();

	// 2. compile shaders
	GLuint vertex, fragment;
	int success;
	char infoLog[512];

	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");

	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");

	// if geometry shader is given, compile geometry shader
	GLuint geometry;
	if (geometryPath != nullptr){
		const char * gShaderCode = geometryCode.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);
		checkCompileErrors(geometry, "GEOMETRY");
	}

	// shader Program
	programId = glCreateProgram();
	glAttachShader(programId, vertex);
	glAttachShader(programId, fragment);
	if (geometryPath != nullptr){
		glAttachShader(programId, geometry);
	}
	glLinkProgram(programId);
	checkCompileErrors(programId, "PROGRAM");

	// detach shaders
	glDetachShader(programId, vertex);
	glDetachShader(programId, fragment);
	if (geometryPath != nullptr){
		glDetachShader(programId, geometry);
	}

	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometryPath != nullptr){
		glDeleteShader(geometry);
	}
}

Shader::Shader(const char *computePath) {
	GLint compiled;
	char infoLog[4096];
	int infoLogLength;
	std::string code = textFileRead(computePath);

	const char *CshaderCode = code.c_str();

	//compile vertex shader:
	GLuint Cshader = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(Cshader, 1, &CshaderCode, 0);
	glCompileShader(Cshader);
	glGetShaderiv(Cshader, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		// Print out the info log
		glGetShaderInfoLog(Cshader, sizeof(infoLog), &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			printf("CompileShader() infoLog %s \n%s\n", computePath, infoLog);
			glDeleteShader(Cshader);
			return;
		}
	}

	programId = glCreateProgram();
	glAttachShader(programId, Cshader);
	glDeleteShader(Cshader);

	glLinkProgram(programId);
}


// activate the shader
// ------------------------------------------------------------------------
void Shader::enable()
{
	glUseProgram(programId);
}
// activate the shader
// ------------------------------------------------------------------------
void Shader::disable()
{
	glUseProgram(0);
}

// utility function for checking shader compilation/linking errors.
// ------------------------------------------------------------------------
void Shader::checkCompileErrors(GLuint shader, std::string type)
{
	GLint success;
	GLchar infolog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infolog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infolog << "\n" << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infolog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infolog << "\n" << std::endl;
		}
	}
}

std::string Shader::textFileRead(const char *fileName)
{
	std::string fileString;
	std::string line;

	std::ifstream file(fileName, std::ios_base::in);

	if (file.is_open()){
		while (!file.eof()){
			getline(file, line);
			fileString.append(line);
			fileString.append("\n");
		}
		file.close();
	}
	else{
		std::cout << "Unable to open " << fileName << "\n";
    }

	return fileString;
}


/***********************************************
 *	Uniform Setter
 * ********************************************/
/************* Bool *************
Either the i, ui or f variants may be used to provide values for uniform variables of type bool, bvec2, bvec3, bvec4, or arrays of these. 
The uniform variable will be set to false if the input value is 0 or 0.0f, and it will be set to true otherwise.
*/
void Shader::uniform(const std::string& name, bool value) const{
	glUniform1i(glGetUniformLocation(programId, name.c_str()), value);
}

void Shader::uniform(const std::string& name, const glm::bvec2& value) const{
	glUniform2i(glGetUniformLocation(programId, name.c_str()), value.x, value.y);
}

void Shader::uniform(const std::string& name, bool x, bool y) const{
	glUniform2i(glGetUniformLocation(programId, name.c_str()), x, y);
}

void Shader::uniform(const std::string& name, const glm::bvec3& value) const{
	glUniform3i(glGetUniformLocation(programId, name.c_str()), value.x, value.y, value.z);
}

void Shader::uniform(const std::string& name, bool x, bool y, bool z) const{
	glUniform3i(glGetUniformLocation(programId, name.c_str()), x, y, z);
}

void Shader::uniform(const std::string& name, const glm::bvec4& value) const{
	glUniform4i(glGetUniformLocation(programId, name.c_str()), value.x, value.y, value.z, value.w);
}

void Shader::uniform(const std::string& name, bool x, bool y, bool z, bool w) const{
	glUniform4i(glGetUniformLocation(programId, name.c_str()), x, y, z, w);
}

/************* Int *************/
void Shader::uniform(const std::string& name, int value) const{
	glUniform1i(glGetUniformLocation(programId, name.c_str()), value);
}

void Shader::uniform(const std::string& name, const glm::ivec2& value) const{
	glUniform2iv(glGetUniformLocation(programId, name.c_str()), 1, &value[0]);
}

void Shader::uniform(const std::string& name, int x, int y) const{
	glUniform2i(glGetUniformLocation(programId, name.c_str()), x, y);
}

void Shader::uniform(const std::string& name, const glm::ivec3& value) const{
	glUniform3iv(glGetUniformLocation(programId, name.c_str()), 1, &value[0]);
}

void Shader::uniform(const std::string& name, int x, int y, int z) const{
	glUniform3i(glGetUniformLocation(programId, name.c_str()), x, y, z);
}

void Shader::uniform(const std::string& name, const glm::ivec4& value) const{
	glUniform3iv(glGetUniformLocation(programId, name.c_str()), 1, &value[0]);
}

void Shader::uniform(const std::string& name, int x, int y, int z, int w) const{
	glUniform4i(glGetUniformLocation(programId, name.c_str()), x, y, z, w);
}

/************** Unsigned Int **************/
void Shader::uniform(const std::string& name, unsigned int value) const{
	glUniform1ui(glGetUniformLocation(programId, name.c_str()), value);
}

void Shader::uniform(const std::string& name, const glm::uvec2& value) const{
	glUniform2uiv(glGetUniformLocation(programId, name.c_str()), 1, &value[0]);
}

void Shader::uniform(const std::string& name, unsigned int x, unsigned int y) const{
	glUniform2ui(glGetUniformLocation(programId, name.c_str()), x, y);
}

void Shader::uniform(const std::string& name, const glm::uvec3& value) const{
	glUniform3uiv(glGetUniformLocation(programId, name.c_str()), 1, &value[0]);
}

void Shader::uniform(const std::string& name, unsigned int x, unsigned int y, unsigned int z) const{
	glUniform3ui(glGetUniformLocation(programId, name.c_str()), x, y, z);
}

void Shader::uniform(const std::string& name, const glm::uvec4& value) const{
	glUniform4uiv(glGetUniformLocation(programId, name.c_str()), 1, &value[0]);
}

void Shader::uniform(const std::string& name, unsigned int x, unsigned int y, unsigned int z, unsigned int w) const{
	glUniform4ui(glGetUniformLocation(programId, name.c_str()), x, y, z, w);
}

/************** Float **************/
void Shader::uniform(const std::string& name, float value) const{
	glUniform1f(glGetUniformLocation(programId, name.c_str()), value);
}

void Shader::uniform(const std::string& name, const glm::vec2& value) const{
	glUniform2fv(glGetUniformLocation(programId, name.c_str()), 1, &value[0]);
}

void Shader::uniform(const std::string& name, float x, float y) const{
	glUniform2f(glGetUniformLocation(programId, name.c_str()), x, y);
}

void Shader::uniform(const std::string& name, const glm::vec3& value) const{
	glUniform3fv(glGetUniformLocation(programId, name.c_str()), 1, &value[0]);
}

void Shader::uniform(const std::string& name, float x, float y, float z) const{
	glUniform3f(glGetUniformLocation(programId, name.c_str()), x, y, z);
}

void Shader::uniform(const std::string& name, const glm::vec4& value) const{
	glUniform4fv(glGetUniformLocation(programId, name.c_str()), 1, &value[0]);
}

void Shader::uniform(const std::string& name, float x, float y, float z, float w){
	glUniform4f(glGetUniformLocation(programId, name.c_str()), x, y, z, w);
}

void Shader::uniform(const std::string& name, const glm::mat2x2&mat, GLboolean transpose) const{
	glUniformMatrix2fv(glGetUniformLocation(programId, name.c_str()), 1, transpose, &mat[0][0]);
}

void Shader::uniform(const std::string& name, const glm::mat2x3&mat, GLboolean transpose) const{
	glUniformMatrix2x3fv(glGetUniformLocation(programId, name.c_str()), 1, transpose, &mat[0][0]);
}

void Shader::uniform(const std::string& name, const glm::mat2x4&mat, GLboolean transpose) const{
	glUniformMatrix2x4fv(glGetUniformLocation(programId, name.c_str()), 1, transpose, &mat[0][0]);
}

void Shader::uniform(const std::string& name, const glm::mat3x2&mat, GLboolean transpose) const{
	glUniformMatrix3x2fv(glGetUniformLocation(programId, name.c_str()), 1, transpose, &mat[0][0]);
}

void Shader::uniform(const std::string& name, const glm::mat3x3&mat, GLboolean transpose) const{
	glUniformMatrix3fv(glGetUniformLocation(programId, name.c_str()), 1, transpose, &mat[0][0]);
}

void Shader::uniform(const std::string& name, const glm::mat3x4&mat, GLboolean transpose) const{
	glUniformMatrix3x4fv(glGetUniformLocation(programId, name.c_str()), 1, transpose, &mat[0][0]);
}

void Shader::uniform(const std::string& name, const glm::mat4x2&mat, GLboolean transpose) const{
	glUniformMatrix4x2fv(glGetUniformLocation(programId, name.c_str()), 1, transpose, &mat[0][0]);
}

void Shader::uniform(const std::string& name, const glm::mat4x3&mat, GLboolean transpose) const{
	glUniformMatrix4x3fv(glGetUniformLocation(programId, name.c_str()), 1, transpose, &mat[0][0]);
}

void Shader::uniform(const std::string& name, const glm::mat4x4&mat, GLboolean transpose) const{
	glUniformMatrix4fv(glGetUniformLocation(programId, name.c_str()), 1, transpose, &mat[0][0]);	
}

/************** Double **************/
void Shader::uniform(const std::string& name, double value) const{
	glUniform1f(glGetUniformLocation(programId, name.c_str()), value);
}

void Shader::uniform(const std::string& name, const glm::dvec2& value) const{
	glm::vec2 tempCast = value;
	glUniform2fv(glGetUniformLocation(programId, name.c_str()), 1, &tempCast[0]);
}

void Shader::uniform(const std::string& name, double x, double y) const{
	glUniform2f(glGetUniformLocation(programId, name.c_str()), x, y);
}

void Shader::uniform(const std::string& name, const glm::dvec3& value) const{
	glm::vec3 tempCast = value;
	glUniform3fv(glGetUniformLocation(programId, name.c_str()), 1, &tempCast[0]);
}

void Shader::uniform(const std::string& name, double x, double y, double z) const{
	glUniform3f(glGetUniformLocation(programId, name.c_str()), x, y, z);
}

void Shader::uniform(const std::string& name, const glm::dvec4& value) const{
	glm::vec4 tempCast = value;
	glUniform4fv(glGetUniformLocation(programId, name.c_str()), 1, &tempCast[0]);
}

void Shader::uniform(const std::string& name, double x, double y, double z, double w){
	glUniform4f(glGetUniformLocation(programId, name.c_str()), x, y, z, w);
}

void Shader::uniform(const std::string& name, const glm::dmat2x2& mat, GLboolean transpose) const{
	glm::mat2x2 tempCast = mat;
	glUniformMatrix2fv(glGetUniformLocation(programId, name.c_str()), 1, transpose, &tempCast[0][0]);
}

void Shader::uniform(const std::string& name, const glm::dmat2x3&mat, GLboolean transpose) const{
	glm::mat2x3 tempCast = mat;
	glUniformMatrix2x3fv(glGetUniformLocation(programId, name.c_str()), 1, transpose, &tempCast[0][0]);
}

void Shader::uniform(const std::string& name, const glm::dmat2x4&mat, GLboolean transpose) const{
	glm::mat2x4 tempCast = mat;
	glUniformMatrix2x4fv(glGetUniformLocation(programId, name.c_str()), 1, transpose, &tempCast[0][0]);
}

void Shader::uniform(const std::string& name, const glm::dmat3x2&mat, GLboolean transpose) const{
	glm::mat3x2 tempCast = mat;
	glUniformMatrix3x2fv(glGetUniformLocation(programId, name.c_str()), 1, transpose, &tempCast[0][0]);
}

void Shader::uniform(const std::string& name, const glm::dmat3x3&mat, GLboolean transpose) const{
	glm::mat3x3 tempCast = mat;
	glUniformMatrix3fv(glGetUniformLocation(programId, name.c_str()), 1, transpose, &tempCast[0][0]);
}

void Shader::uniform(const std::string& name, const glm::dmat3x4&mat, GLboolean transpose) const{
	glm::mat3x4 tempCast = mat;
	glUniformMatrix3x4fv(glGetUniformLocation(programId, name.c_str()), 1, transpose, &tempCast[0][0]);
}

void Shader::uniform(const std::string& name, const glm::dmat4x2&mat, GLboolean transpose) const{
	glm::mat4x2 tempCast = mat;
	glUniformMatrix4x2fv(glGetUniformLocation(programId, name.c_str()), 1, transpose, &tempCast[0][0]);
}

void Shader::uniform(const std::string& name, const glm::dmat4x3&mat, GLboolean transpose) const{
	glm::mat4x3 tempCast = mat;
	glUniformMatrix4x3fv(glGetUniformLocation(programId, name.c_str()), 1, transpose, &tempCast[0][0]);
}

void Shader::uniform(const std::string& name, const glm::dmat4x4&mat, GLboolean transpose) const{
	glm::mat4x4 tempCast = mat;
	glUniformMatrix4fv(glGetUniformLocation(programId, name.c_str()), 1, transpose, &tempCast[0][0]);	
}