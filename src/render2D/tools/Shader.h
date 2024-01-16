#ifndef __SHADER_H__
#define __SHADER_H__

class Shader
{
public:
	Shader(const char* vertexFileName, const char* fragmentFileName);
	~Shader();

	void reload();
	void enable();
	void disable();

	void uniform1f(char* name, float value);
	void uniform1i(char* name, int value);

	void uniform2f(char* name, float value, float value2);
	void uniform2i(char* name, int value, int value2);

	void uniform2fv(char* name, float* value);
	void uniform2iv(char* name, int* value);


	static void reloadShaders();

protected:

	void init();
	bool checkShader(GLuint obj);

	char* vertexFileName;
	char* fragmentFileName;

	int vertexID;
	int fragmentID;
	int programID;

	static vector<Shader*> shaders;

};

#endif
