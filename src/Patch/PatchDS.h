#ifndef PATCH_DATASTRUCTURE_H_
#define PATCH_DATASTRUCTURE_H_

#include <vector>

namespace PolyPatch{
    
    struct PNode{
        PNode(int mv_idx=-1) : vert_index(mv_idx) {}
        int vert_index;
        std::vector<int> adj_pedge_index_vec;
    };

    struct PEdge{
        PEdge() {pnode_index_vec.resize(2); }
        PEdge(int pn_idx1, int pn_idx2, const std::vector<int>& _path) {
            pnode_index_vec.push_back(pn_idx1);
            pnode_index_vec.push_back(pn_idx2);
            mesh_path = _path;
        }
        
        std::vector<int> pnode_index_vec; //! the size must be 2;
        std::vector<int> adj_patch_index_vec; //! the size must be 1 or 2
        std::vector<int> mesh_path;        
    };

    struct Patch{
        std::vector<int> pnode_index_vec;
        std::vector<int> pedge_index_vec;
        std::vector<int> mesh_face_vec;
    };

    struct HalfPE{
        int pnode_index, pedge_index, patch_index;
        int prev_hpe, next_hpe, oppo_hpe;

    HalfPE(): pnode_index(-1), pedge_index(-1), patch_index(-1), prev_hpe(-1), next_hpe(-1), oppo_hpe(-1) {}
    };
}

#endif
