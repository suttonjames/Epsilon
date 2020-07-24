#include "opengl.h"

PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
PFNWGLMAKECONTEXTCURRENTARBPROC wglMakeContextCurrentARB;
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;

static void Win32LoadOpenGLFunctions()
{
#define GLProc(name, proc) name = (PFN##proc##PROC)wglGetProcAddress(#name);
	GLProc(glAttachShader, GLATTACHSHADER);
	GLProc(glBindBuffer, GLBINDBUFFER);
	GLProc(glBindVertexArray, GLBINDVERTEXARRAY);
	GLProc(glBufferData, GLBUFFERDATA);
	GLProc(glCreateBuffers, GLCREATEBUFFERS);
	GLProc(glCreateProgram, GLCREATEPROGRAM);
	GLProc(glCreateShader, GLCREATESHADER);
	GLProc(glCreateVertexArrays, GLCREATEVERTEXARRAYS);
	GLProc(glCompileShader, GLCOMPILESHADER);
	GLProc(glDeleteBuffers, GLDELETEBUFFERS);
	GLProc(glDeleteProgram, GLDELETEPROGRAM);
	GLProc(glDeleteShader, GLDELETESHADER);
	GLProc(glDetachShader, GLDETACHSHADER);
	GLProc(glEnableVertexAttribArray, GLENABLEVERTEXATTRIBARRAY);
	GLProc(glGenBuffers, GLGENBUFFERS);
	GLProc(glGenVertexArrays, GLGENVERTEXARRAYS);
	GLProc(glGetProgramInfoLog, GLGETPROGRAMINFOLOG);
	GLProc(glGetProgramiv, GLGETPROGRAMIV);
	GLProc(glGetShaderInfoLog, GLGETSHADERINFOLOG);
	GLProc(glGetShaderiv, GLGETSHADERIV);
	GLProc(glLinkProgram, GLLINKPROGRAM);
	GLProc(glShaderSource, GLSHADERSOURCE);
	GLProc(glUseProgram, GLUSEPROGRAM);
	GLProc(glVertexAttribPointer, GLVERTEXATTRIBPOINTER);
}

HGLRC InitOpenGL(HDC device_context)
{
	//pfd for dummy context
	PIXELFORMATDESCRIPTOR pfd = {0};
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cAlphaBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;

	s32 pixel_format = ChoosePixelFormat(device_context, &pfd);
	SetPixelFormat(device_context, pixel_format, &pfd);
	HGLRC dummy_render_context = wglCreateContext(device_context);
	wglMakeCurrent(device_context, dummy_render_context);

	wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
	wglMakeContextCurrentARB = (PFNWGLMAKECONTEXTCURRENTARBPROC)wglGetProcAddress("wglMakeContextCurrentARB");
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");

	s32 pixel_format_attribs[] = {
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB, 32,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		0
	};

	UINT num_formats = 0;
	wglChoosePixelFormatARB(
		device_context,
		pixel_format_attribs,
		0,
		1,
		&pixel_format,
		&num_formats);

	s32 context_attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 6,
		WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};

	HGLRC render_context = wglCreateContextAttribsARB(device_context, dummy_render_context, context_attribs);

	wglMakeCurrent(device_context, 0);
	wglDeleteContext(dummy_render_context);
	wglMakeCurrent(device_context, render_context);
	wglSwapIntervalEXT(1);

	Win32LoadOpenGLFunctions();

	return render_context;
}
