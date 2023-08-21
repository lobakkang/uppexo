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

template <typename Tvert, typename Tmvp> class Mesh {
public:
  Mesh();

  void addMesh(MeshInfo &info);

  Tmvp *getMVPList() { return MVPList.data(); }
  Tvert *getVertexList() { return vertexList.data(); };
  uint32_t *getIndexList() { return indexList.data(); };
  Material *getMaterialList() { return MaterialList.data(); };

  int getVertexCount() { return vertexList.size(); }
  int getIndexCount() { return indexList.size(); }
  int getMVPCount() { return MVPList.size(); }
  int getMaterialCount() { return MaterialList.size(); }

private:
  std::unordered_map<Tvert, uint32_t> uniqueVertex;
  int meshCount;

  std::vector<uint32_t> indexList;
  std::vector<Tvert> vertexList;
  std::vector<Tmvp> MVPList;
  std::vector<Material> MaterialList;
};
} // namespace uppexo

#endif
