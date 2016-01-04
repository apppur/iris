#include <cstdlib>
#include <iostream>
#include <stdio.h>

#include <gl/glew.h>
#include "IrisShader.h"

namespace iris {
	
	IrisShader::IrisShader() : m_source(NULL), m_type(GL_NONE), m_program(0)
	{
	}
	IrisShader::~IrisShader()
	{
	}
	void IrisShader::LoadShader(const char* file, GLenum type)
	{
#ifdef WIN32
		FILE* infile;
		fopen_s(&infile, file, "rb");
		//infile = fopen(file, "rb");
#else
		FILE* infile = fopen(file, "rb");
#endif
		if (infile == NULL) {
#ifdef _DEBUG
			std::cerr << "unable to open file: " << file << std::endl;
#endif
			return;
		}

		fseek(infile, 0, SEEK_END);
		int len = ftell(infile);
		fseek(infile, 0, SEEK_SET);

		m_source = new GLchar[len+1];
		fread(m_source, 1, len, infile);
		fclose(infile);
		m_source[len] = 0;
		m_type = type;

		return;
	}
	void IrisShader::CreateProgram()
	{
		m_program = glCreateProgram();
		if (m_program == 0) {
			std::cerr << "create shader program error." << std::endl;
		}
		else {
			std::cout << "create shader program succ." << std::endl;
		}
		return;
	}
	void IrisShader::Process()
	{
		if (m_type != GL_NONE) {
			GLuint m_shader = glCreateShader(m_type);
			if (m_source == NULL) {
				glDeleteShader(m_shader);
				m_shader = 0;
				m_type = GL_NONE;
				return;
			}
			glShaderSource(m_shader, 1, &m_source, NULL);
			delete[] m_source;
			glCompileShader(m_shader);

			GLint compile;
			glGetShaderiv(m_shader, GL_COMPILE_STATUS, &compile);
			if (!compile) {
#ifdef _DEBUG
				GLsizei len;
				glGetShaderiv(m_shader, GL_INFO_LOG_LENGTH, &len);
				GLchar* log = new GLchar[len+1];
				glGetShaderInfoLog(m_shader, len, &len, log);
				std::cerr << "shader compile failed: " << log << std::endl;
				delete[] log;
#endif
				return;
			}
			glAttachShader(m_program, m_shader);
		}
		return;
	}
	void IrisShader::LinkProgram()
	{
		glLinkProgram(m_program);
		GLint link;
		glGetProgramiv(m_program, GL_LINK_STATUS, &link);
		if (!link) {
#ifdef _DEBUG
			GLsizei len;
			glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &len);
			GLchar* log = new GLchar[len + 1];
			glGetProgramInfoLog(m_program, len, &len, log);
			std::cerr << "shader link failed: " << log << std::endl;
			delete[] log;
#endif
			glDeleteShader(m_shader);
			m_shader = 0;
			return;
		}
		return;
	}
	void IrisShader::UseProgram()
	{
		glUseProgram(m_program);
	}
}