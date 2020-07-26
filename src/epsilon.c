#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "language_layer.h"
#include "opengl.c"
#include "mesh.c"

static HDC hdc = 0; // temp
static Mesh *mesh; // temp hot reloading is now broken as this need to be store in permanent memory

__declspec(dllexport) void init_game(HDC device_context)
{
	HGLRC rendering_context = init_opengl(device_context);

    const GLchar *vertex_source =
        "#version 330 core\n\
        layout(location = 0) in vec3 a_Position;\n\
        void main()\n\
        {\n\
            gl_Position = vec4(a_Position, 1.0);\n\
        }";

    const GLchar *fragment_source =
        "#version 330 core\n\
        out vec4 colour;\n\
        void main()\n\
        {\n\
            colour = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n\
        }";

    mesh = load_mesh("c:/dev/epsilon/assets/helmet.obj"); // temp
    mesh->shader = load_shader(vertex_source, fragment_source);
}

__declspec(dllexport) void update_game(HDC device_context)
{
    hdc = device_context; // temp
	glClearColor(0.7f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(mesh->vertex_array);
    glUseProgram(mesh->shader->id);
	glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, 0);

	SwapBuffers(hdc);
	//wglSwapLayerBuffers(device_context, WGL_SWAP_MAIN_PLANE);
}

__declspec(dllexport) void shutdown_game()
{

}
