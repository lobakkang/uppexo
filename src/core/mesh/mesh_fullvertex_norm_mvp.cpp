#include <core/datatype.hpp>
#include <core/mesh.hpp>

#include <iostream>
#include <utils/log.hpp>

template <>
uppexo::Mesh<uppexo::FullVertex, uppexo::MVP_with_normalized_matrix>::Mesh() {
  meshCount = 0;
  indexList.clear();
  vertexList.clear();
}

template <>
void uppexo::Mesh<uppexo::FullVertex, uppexo::MVP_with_normalized_matrix>::
    addMesh(uppexo::MeshInfo &info) {
  uppexo::Log::GetInstance().logInfo("Loading mesh from %s\n",
                                     info.path.c_str());

  tinyobj::attrib_t attrib;
  std::string warn, err;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;

  if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
                        info.path.c_str())) {
    uppexo::Log::GetInstance().logError("Failed to load OBJ file %s %s\n",
                                        err.c_str(), warn.c_str());
  }

  for (const auto &material : materials) {
    Material mat;
    mat.ambient[0] = material.ambient[0];
    mat.ambient[1] = material.ambient[1];
    mat.ambient[2] = material.ambient[2];

    mat.diffuse[0] = material.diffuse[0];
    mat.diffuse[1] = material.diffuse[1];
    mat.diffuse[2] = material.diffuse[2];

    mat.specular[0] = material.specular[0];
    mat.specular[1] = material.specular[1];
    mat.specular[2] = material.specular[2];

    mat.shininess = material.shininess;

    MaterialList.push_back(mat);
  }

  for (const auto &shape : shapes) {
    int i = 0;
    for (const auto &index : shape.mesh.indices) {
      uppexo::FullVertex vertex;
      vertex.pos[0] = attrib.vertices[index.vertex_index * 3 + 0];
      vertex.pos[1] = attrib.vertices[index.vertex_index * 3 + 1];
      vertex.pos[2] = attrib.vertices[index.vertex_index * 3 + 2];

      vertex.normal[0] = attrib.normals[index.normal_index * 3 + 0];
      vertex.normal[1] = attrib.normals[index.normal_index * 3 + 1];
      vertex.normal[2] = attrib.normals[index.normal_index * 3 + 2];

      vertex.uv[0] = attrib.texcoords[index.texcoord_index * 2 + 0];
      vertex.uv[1] = 1.0f - attrib.texcoords[index.texcoord_index * 2 + 1];

      vertex.color[0] = 1.0f;
      vertex.color[1] = 1.0f;
      vertex.color[2] = 1.0f;
      vertex.color[3] = 1.0f;

      if (uniqueVertex.count(vertex) == 0) {
        uniqueVertex[vertex] = static_cast<uint32_t>(vertexList.size());
        vertexList.push_back(vertex);
      }
      indexList.push_back(uniqueVertex[vertex]);
    }
  }

  uppexo::Log::GetInstance().logVerbose(
      "%i unique vertex is loaded with %i vertex indices\n", vertexList.size(),
      indexList.size());

  MVPList.push_back((MVP_with_normalized_matrix){});
  meshCount++;
}
