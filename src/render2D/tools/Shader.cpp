#include "render2D_common.h"
#include "glew.h"
#include "Shader.h" 

vector<Shader*> Shader::shaders;

Shader::Shader(const char* vertexFileName, const char* fragmentFileName) :
vertexFileName(copyText(vertexFileName)), fragmentFileName(copyText(fragmentFileName)),
vertexID(0), fragmentID(0), programID(0)
{
	init();
	shaders.push_back(this);
}

Shader::~Shader() {
	if(vertexID!=0) glDeleteShader (vertexID) ;
	if(fragmentID!=0) glDeleteShader (fragmentID) ;
	if(programID!=0) glDeleteProgram (programID) ;

	shaders.erase(remove(shaders.begin(), shaders.end(), this));
 
	delete[] vertexFileName;
	delete[] fragmentFileName;
}

void Shader::init() {
	programID = glCreateProgram () ;
	vertexID = glCreateShader (GL_VERTEX_SHADER) ;
	fragmentID = glCreateShader (GL_FRAGMENT_SHADER) ;

	LOG_FILE("--------- Vertex File : " << vertexFileName << " -------" << endl);
	const char * vertexSource = readFile (vertexFileName) ;
	LOG_FILE(vertexSource << endl);

	LOG_FILE("--------- Fragment File : " << fragmentFileName << " -----" << endl);
	const char * fragmentSource = readFile (fragmentFileName) ;
	LOG_FILE(fragmentSource << endl);

	if (vertexSource != NULL) {
		//printf("\nVERTEX [%d]: \n%s\n\n",vertexID ,vertexSource);
		glShaderSource (vertexID, 1, &vertexSource, NULL) ;
		glCompileShader (vertexID) ;

		if (checkShader(vertexID)) {
			glAttachShader (programID, vertexID) ;
		}

		delete[] vertexSource;
	}

	if (fragmentSource != NULL) {
		//printf("\nFRAGMENT[%d] : \n%s\n\n",fragmentID ,fragmentSource);
		glShaderSource (fragmentID, 1, &fragmentSource, NULL) ;
		glCompileShader (fragmentID) ;

		if (checkShader(fragmentID)) {
			glAttachShader (programID, fragmentID) ;
		}
		delete[] fragmentSource;
	}
	glLinkProgram (programID) ;
}

void Shader::enable() 
{
	  glUseProgram (programID);
}

void Shader::disable() 
{
	  glUseProgram (0);
}


void Shader::reload()
{

	if(vertexID!=0) glDeleteShader (vertexID) ;
	if(fragmentID!=0) glDeleteShader (fragmentID) ;
	if(programID!=0) glDeleteProgram (programID) ;

	init();
}

bool Shader::checkShader(GLuint obj)
{
	int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 1)
    {
        infoLog = new char[infologLength];
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("[ERROR, len %d]  glGetShaderInfoLog: %s\n",infologLength ,infoLog);
        delete infoLog;
		return false;
    }else{
 		printf("Program [%d] Info Log: OK\n", obj);
    }

	return true;
}

void Shader::uniform1i(char* name, int value) {
	GLint loc = glGetUniformLocation(programID,name);
	if (loc != -1) {
		glUniform1i(loc, value);
	} else {
		//printf("[Shader] warning : %s not found\n",name);
	}
}

void Shader::uniform1f(char* name, float value) {
	GLint loc = glGetUniformLocation(programID,name);
	if (loc != -1) {
		glUniform1f(loc, value);
	} else {
		//printf("[Shader] warning : %s not found\n",name);
	}
}

void Shader::uniform2i(char* name, int value, int value2) {
	GLint loc = glGetUniformLocation(programID,name);
	if (loc != -1) {
		glUniform2i(loc, value, value2);
	} else {
		//printf("[Shader] warning : %s not found\n",name);
	}
}

void Shader::uniform2f(char* name, float value, float value2) {
	GLint loc = glGetUniformLocation(programID,name);
	if (loc != -1) {
		glUniform2f(loc, value, value2);
	} else {
		//printf("[Shader] warning : %s not found\n",name);
	}
}

void Shader::reloadShaders() {
	for_each(shaders.begin(), shaders.end(), mem_fun(&Shader::reload));
}