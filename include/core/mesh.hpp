#ifndef MESH_CONTROLLER_HPP_
#define MESH_CONTROLLER_HPP_

#include <core/datatype.hpp>
#include <list>
#include <unordered_map>

namespace uppexo {
struct MeshInfo {
  std::string path;
  int id;
  uint32_t vertexStartAddress;
  uint32_t vertexEndAddress;
  uint32_t indexStartAddress;
  uint32_t indexEndAddress;
};

template <typename T> class Mesh {
public:
  Mesh();

  void addMesh(MeshInfo &info);

  MVP *getMVPList() { return MVPList.data(); }
  T *getVertexList() { return vertexList.data(); };
  uint32_t *getIndexList() { return indexList.data(); };

  int getVertexCount() { return vertexList.size(); }
  int getIndexCount() { return indexList.size(); }
  int getMVPCount() { return MVPList.size(); }

private:
  std::unordered_map<T, uint32_t> uniqueVertex;
  int meshCount;

  std::vector<uint32_t> indexList;
  std::vector<T> vertexList;
  std::vector<MVP> MVPList;
};
} // namespace uppexo

#endif
