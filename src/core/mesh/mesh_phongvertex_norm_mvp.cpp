#include <core/datatype.hpp>
#include <core/mesh.hpp>

#include <iostream>
#include <utils/log.hpp>

template <>
uppexo::Mesh<uppexo::PhongVertex, uppexo::MVP_with_normalized_matrix>::Mesh() {
  meshCount = 0;
  indexList.clear();
  vertexList.clear();
}

template <>
void uppexo::Mesh<uppexo::PhongVertex, uppexo::MVP_with_normalized_matrix>::
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
    mat.ambient.r = 0.0f;//material.ambient.b;
    mat.ambient.g = 1.0f;//material.ambient[1];
    mat.ambient.b = 1.0f;//material.ambient.r;

    mat.diffuse.r = 1.0f; // material.diffuse.b;
    mat.diffuse.g = 1.0f;    // material.diffuse[1];
    mat.diffuse.b = 0;    // material.diffuse.r;

    mat.specular.r = 1.0f;//material.specular.g;
    mat.specular.g = 0;//material.specular[1];
    mat.specular.b = 1.0f;//material.specular.g;

    mat.shininess.r = material.shininess;
    mat.shininess.g = material.shininess;
    mat.shininess.b = material.shininess;

    MaterialList.push_back(mat);
  }

  for (const auto &shape : shapes) {
    int i = 0;
    for (const auto &index : shape.mesh.indices) {
      uppexo::PhongVertex vertex;
      vertex.pos[0] = attrib.vertices[index.vertex_index * 3 + 0];
      vertex.pos[1] = attrib.vertices[index.vertex_index * 3 + 1];
      vertex.pos[2] = attrib.vertices[index.vertex_index * 3 + 2];

      vertex.normal[0] = attrib.normals[index.normal_index * 3 + 0];
      vertex.normal[1] = attrib.normals[index.normal_index * 3 + 1];
      vertex.normal[2] = attrib.normals[index.normal_index * 3 + 2];

      vertex.uv[0] = attrib.texcoords[index.texcoord_index * 2 + 0];
      vertex.uv[1] = 1.0f - attrib.texcoords[index.texcoord_index * 2 + 1];

      vertex.mat = shape.mesh.material_ids[i];
      //uppexo::Log::GetInstance().logVerbose("MAT: %i", vertex.mat);

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
  uppexo::Log::GetInstance().logVerbose("%i Material is loaded\n",
                                        MaterialList.size());
  MVPList.push_back((MVP_with_normalized_matrix){});
  meshCount++;
}
