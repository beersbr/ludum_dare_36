#include "common.hpp"

#define RANDOM(n) (rand()/(float)RAND_MAX * (n))

char * const loadBinaryFile(const char * const filename) {
	FILE *fp = fopen(filename, "rb+");
	if(!fp) {
		#ifdef DEBUG
		std::cout << "Could not open file named " << filename << std::endl;
		#endif
		return 0;
	}
	fseek(fp, 0, SEEK_END);
	long flength = ftell(fp);

	fseek(fp, 0, SEEK_SET);

	#ifdef DEBUG
	std::cout << "Size of file " << filename << " " << flength << std::endl;
	#endif

	char *data = (char*)malloc(sizeof(char)*flength+1);
	long read = fread(data, sizeof(char), flength+1, fp);

	if(read != flength) {
		#ifdef DEBUG
		std::cout << "Warning: did not read entire file named " << filename << std::endl;
		#endif
		free(data);
		return 0;
	}

	data[flength] = '\0';

	return data;
}


shader_t loadShader(const char * const vertex_name, const char * const fragment_name, const char * const name) {

	char *vertexData = loadBinaryFile(vertex_name);
	char *fragmentData = loadBinaryFile(fragment_name);

	#ifdef DEBUG
	// std::cout << ">> read vertex " << vertex_name << std::endl << vertexData <<std::endl;
	// std::cout << ">> read fragment " << fragment_name << std::endl << fragmentData <<std::endl;
	#endif

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &vertexData, 0);
	glCompileShader(vertexShader);

	GLint compilationResult = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compilationResult);

	if(!compilationResult) {
		GLint logLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);
		char *log = (char*)malloc(sizeof(char)*logLength);
		glGetShaderInfoLog(vertexShader, logLength, 0, (GLchar*)log);

		std::cout << ">> Vertex shader compilation issue: " << log << std::endl;
	}

	glShaderSource(fragmentShader, 1, &fragmentData, 0);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compilationResult);

	if(!compilationResult) {
		GLint logLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);
		char *log = (char*)malloc(sizeof(char)*logLength);
		glGetShaderInfoLog(fragmentShader, logLength, 0, (GLchar*)log);

		std::cout << ">> Fragment shader compilation issue: " << log << std::endl;
	}

	GLuint shaderId = glCreateProgram();
	if(!shaderId) {
		#ifdef DEBUG
		std::cout << "Could not create a shader program:" << glGetError() << std::endl;
		#endif
	}


	glAttachShader(shaderId, vertexShader);
	glAttachShader(shaderId, fragmentShader);
	glLinkProgram(shaderId);

	GLint linkResult;

	glGetProgramiv(shaderId, GL_LINK_STATUS, &linkResult);
	if(!linkResult) {
		#ifdef DEBUG
		std::cout << "Coud not link program: " << glGetError() << std::endl;
		#endif
	}

	shader_t shader = { .id = shaderId };
	strncpy(shader.name, name, RESOURCE_NAME_SZ-1);

	return shader;
}
