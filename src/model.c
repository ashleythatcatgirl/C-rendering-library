
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>

#include "model.h"

void generateModelArrays(struct Model *model) {
	glGenVertexArrays(1, &model->VAO);
	glGenBuffers(1, &model->VBO);
	glGenBuffers(1, &model->EBO);

	glBindVertexArray(model->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, model->VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(struct Vertex) * model->vertexCount, &model->verticies[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * model->indiceCount, model->indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void initModel(struct Model *model) {
}

void generateSphere(float *verticies, int slices, int stacks) {
/*
SurfaceMesh uv_sphere(int n_slices, int n_stacks)
{
  SurfaceMesh mesh;

  // add top vertex
  auto v0 = mesh.add_vertex(Point(0, 1, 0));

  // generate vertices per stack / slice
  for (int i = 0; i < n_stacks - 1; i++)
  {
    auto phi = M_PI * double(i + 1) / double(n_stacks);
    for (int j = 0; j < n_slices; j++)
    {
      auto theta = 2.0 * M_PI * double(j) / double(n_slices);
      auto x = std::sin(phi) * std::cos(theta);
      auto y = std::cos(phi);
      auto z = std::sin(phi) * std::sin(theta);
      mesh.add_vertex(Point(x, y, z));
    }
  }

  // add bottom vertex
  auto v1 = mesh.add_vertex(Point(0, -1, 0));

  // add top / bottom triangles
  for (int i = 0; i < n_slices; ++i)
  {
    auto i0 = i + 1;
    auto i1 = (i + 1) % n_slices + 1;
    mesh.add_triangle(v0, Vertex(i1), Vertex(i0));
    i0 = i + n_slices * (n_stacks - 2) + 1;
    i1 = (i + 1) % n_slices + n_slices * (n_stacks - 2) + 1;
    mesh.add_triangle(v1, Vertex(i0), Vertex(i1));
  }

  // add quads per stack / slice
  for (int j = 0; j < n_stacks - 2; j++)
  {
    auto j0 = j * n_slices + 1;
    auto j1 = (j + 1) * n_slices + 1;
    for (int i = 0; i < n_slices; i++)
    {
      auto i0 = j0 + i;
      auto i1 = j0 + (i + 1) % n_slices;
      auto i2 = j1 + (i + 1) % n_slices;
      auto i3 = j1 + i;
      mesh.add_quad(Vertex(i0), Vertex(i1),
                    Vertex(i2), Vertex(i3));
    }
  }
  return mesh;
}
*/
}

struct Vertex *generateCube(float scale) {
	struct Vertex *verticies;
	verticies = malloc((sizeof(struct Vertex) * 8));

	float edge = scale / 2;

	glm_vec3_copy((vec3){-edge, -edge, edge}, verticies[0].position);
	glm_vec3_copy((vec3){edge, -edge, edge}, verticies[1].position);
	glm_vec3_copy((vec3){-edge, edge, edge}, verticies[2].position);
	glm_vec3_copy((vec3){edge, edge, edge}, verticies[3].position);
	glm_vec3_copy((vec3){-edge, -edge, -edge}, verticies[4].position);
	glm_vec3_copy((vec3){edge, -edge, -edge}, verticies[5].position);
	glm_vec3_copy((vec3){-edge, edge, -edge}, verticies[6].position);
	glm_vec3_copy((vec3){edge, edge, -edge}, verticies[7].position);

	return verticies;
}
