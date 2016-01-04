#ifndef _IRIS_SHADER_H
#define _IRIS_SHADER_H

#include <GL/GL.h>

namespace iris {

	typedef struct {
		GLenum		m_type;
		const char* m_file;
		GLuint		m_shader;
	} ShaderInfo;

	class IrisShader {
	public:
		IrisShader();
		~IrisShader();

		void LoadShader(const char* file, GLenum type);
		void CreateProgram();
		void Process();
		void LinkProgram();
		void UseProgram();
	private:
		GLchar* m_source;
		GLenum	m_type;
		GLuint	m_program;
		GLuint	m_shader;
	};
}

#endif