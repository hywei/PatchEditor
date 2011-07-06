#ifndef PATCHMAKER_H_
#define PATCHMAKER_H_

#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include "PatchDS.h"

#include <vector>
#include <map>
#include <set>

namespace PolyPatch{

    typedef OpenMesh::TriMesh_ArrayKernelT<> TriMesh;
    
    class PolyPatch
    {
    public:
        PolyPatch(const TriMesh& _mesh);
        ~PolyPatch();

        bool AddCorner(int mesh_vert_index); 
        bool DelCorner(int mesh_vert_index);

        bool AddPEdge(const std::vector<int>& a_path);
        //! delete pedges including this vertex
        bool DelPEdge(int mesh_vert_index);

        bool AddPatch(const Patch& temp_patch);
        //! delete patchs includeing this vertex
        bool DelPatch(int mesh_vert_index);

        bool BuildPolyPatch();

        //! get data
        const std::vector<PNode>& GetPNodes() const { return pnode_vec; }
        const std::vector<PEdge>& GetPEdges() const { return pedge_vec; }
        const std::vector<Patch>& GetPatchs() const { return patch_vec; }
        const TriMesh& GetMesh() const { return mesh; }
        
        bool LoadPatchFile(const std::string& file_name);
        bool SavePatchFile(const std::string& file_name) const;
    private:
        //! check a mesh vertex could be a valid corner or not 
        bool CheckMeshVertIsValidPNode(int mesh_vert_index) const;
        bool CheckMeshPathIsValidPEdge(const std::vector<int>& a_path) const;

        //! get the corner index which vertex index is $PARAM, return -1 if no
        int GetCornerIndex(int mesh_vert_index) const;
        //! get another corner index of one pedge
        int GetAnotherCornerIndex(const PEdge& pe, int one_corner_index) const;
        //! remove one adj-pedge for a corner
        bool RemoveAdjPEdge(PNode& pn, int pe_index) const;

        //! get pedge index array which including the vertex
        std::vector<size_t> GetPEdgeIndexArray(int mesh_vert_index) const;
        //! delete a pedge in pedge_veco
        void DelPEdgeInArray(int pe_index);

        //! find and set corners' adjacent pedges
        void SetCornerAdjPEdgeIndexArray();
        //! sort corners' adjacent pedges
        void SortAdjPEdges();
        void SortAdjPEdges(int pn_index);
        //! build the half-pedge data structure (a quasi halfedge ds)
        void BuildHalfPEDS();
        //! form each patch by the halfpe ds
        void FormPatchs();
        //! form a single patch from a halfpe
        Patch FormSinglePatch(int start_pn_index, int start_pe_index) const;

        //! get next/prev pedge index of a corner, call after sort neightbors
        int GetNextPEdgeIndex(const PNode& pn, int cur_pe_index) const;
        int GetPrevPEdgeIndex(const PNode& pe, int cur_pe_index) const;
    private:
        const TriMesh& mesh;
        
        std::vector<PNode> pnode_vec;
        std::vector<PEdge> pedge_vec;
        std::vector<Patch> patch_vec;
        std::vector<HalfPE> halfpe_vec;
    };
}

#endif
