#include "PatchMaker.h"
#include <fstream>
using namespace std;

namespace PolyPatch{

    PolyPatch::PolyPatch(const TriMesh& _mesh) : mesh(_mesh){}
    PolyPatch::~PolyPatch(){}

    bool PolyPatch::AddCorner(int mesh_vert_index)
    {
        if(! CheckMeshVertIsValidPNode( mesh_vert_index)) return false;
        pnode_vec.push_back(PNode(mesh_vert_index));
        return true;
    }

    bool PolyPatch::DelCorner(int mesh_vert_index)
    {
        //!TODO: this could be more efficient by map or hash table
        int corner_index = GetCornerIndex(mesh_vert_index);
        if(corner_index == -1) return false;
        
        const PNode& corner = pnode_vec[corner_index];
        const vector<int>& nb_pedges = corner.adj_pedge_index_vec;

        //! delete this corner's neighbor pedges
        for(size_t k=0; k<nb_pedges.size(); ++k){
            DelPEdgeInArray(nb_pedges[k]);
        }

        //! delete this corner
        pnode_vec.erase(pnode_vec.begin() + corner_index);

        return true;
    }

    bool PolyPatch::AddPEdge(const vector<int>& mesh_path)
    {
        if( !CheckMeshPathIsValidPEdge(mesh_path) ) return false;
        int pn_idx1 = GetCornerIndex(mesh_path[0]);
        int pn_idx2 = GetCornerIndex(mesh_path[mesh_path.size() -1]);
        pedge_vec.push_back(PEdge(pn_idx1, pn_idx2, mesh_path));
        return true;
    }

    bool PolyPatch::DelPEdge(int mesh_vert_index)
    {
        vector<size_t> pe_iv = GetPEdgeIndexArray(mesh_vert_index);

        for(size_t k=0; k<pe_iv.size(); ++k){
            DelPEdgeInArray(pe_iv[k]);
        }

        return true;
    }

    vector<size_t> PolyPatch::GetPEdgeIndexArray(int mesh_vert_index) const
    {
        vector<size_t> ret;
        for(size_t k=0; k<pedge_vec.size(); ++k){
            const PEdge& pe = pedge_vec[k];
            const vector<int>& path = pe.mesh_path;
            for(size_t i=0; i<path.size(); ++i){
                if(path[i] == mesh_vert_index){
                    ret.push_back(k);
                    break;
                }
            }
        }
        return ret;
    }

    void PolyPatch::DelPEdgeInArray(int pe_index)
    {
        const PEdge& pe = pedge_vec[pe_index];
        int pn_idx1(pe.pnode_index_vec[0]), pn_idx2(pe.pnode_index_vec[1]);

        //! delete the adjacent connection for the two corners
        vector<int>& pe_iv1 = pnode_vec[pn_idx1].adj_pedge_index_vec;
        vector<int>& pe_iv2 = pnode_vec[pn_idx2].adj_pedge_index_vec;
        vector<int>::iterator iter1 = find(pe_iv1.begin(), pe_iv1.end(), pe_index);
        vector<int>::iterator iter2 = find(pe_iv2.begin(), pe_iv2.end(), pe_index);
        if(iter1 != pe_iv1.end()) pe_iv1.erase(iter1);
        if(iter2 != pe_iv2.end()) pe_iv2.erase(iter2);

        //! remove this pedge
        pedge_vec.erase(pedge_vec.begin() + pe_index);
    }

    bool PolyPatch::CheckMeshVertIsValidPNode(int mesh_vert_index) const
    {
        return GetCornerIndex(mesh_vert_index) == -1;
    }

    bool PolyPatch::CheckMeshPathIsValidPEdge(const vector<int>& path) const
    {
        //! a point or a circle isnot valid
        if(path.size() <=1 || path[0] == path[path.size()-1])
            return false;

        //! the two endpoints should both be corners
        int ep1 = path[0], ep2 = path[path.size()-1];
        if(GetCornerIndex(ep1) == -1 || GetCornerIndex(ep2) == -1)
            return false;

        //! the path shouldn't have corners except at the two endpoints
        for(size_t k=1; k<path.size()-2; ++k){
            if(GetCornerIndex(path[k]) != -1)
                return false;
        }
        
        //! the path shouldn't have circle or cross with other pedge
        //! TODO: add later
        
        return true;
    }

    int PolyPatch::GetCornerIndex(int mesh_vert_index) const
    {
        int corner_index(-1);
        for(size_t k=0; k<pnode_vec.size(); ++k){
            if(pnode_vec[k].vert_index == mesh_vert_index){
                corner_index = k;
                break;
            }
        }
        return corner_index;
    }

    int PolyPatch::GetAnotherCornerIndex(const PEdge& pe, int one_corner_index) const{
        assert(pe.pnode_index_vec.size() == 2);
        assert(pe.pnode_index_vec[0] == one_corner_index || pe.pnode_index_vec[1] == one_corner_index);

        return pe.pnode_index_vec[0] == one_corner_index ? pe.pnode_index_vec[1] : pe.pnode_index_vec[0];
    }

    bool PolyPatch::RemoveAdjPEdge(PNode& pn, int pe_index) const
    {
        vector<int>& pe_iv = pn.adj_pedge_index_vec;
        vector<int>::iterator iter = find(pe_iv.begin(), pe_iv.end(), pe_index);
        if(iter == pe_iv.end()) return false;

        pe_iv.erase(iter);
        return true;
    }

    bool PolyPatch::BuildPolyPatch()
    {
        SetCornerAdjPEdgeIndexArray();
        SortAdjPEdges();
        FormPatchs();
        
        return true;
    }

    void PolyPatch::SetCornerAdjPEdgeIndexArray()
    {
        for(size_t k=0; k<pedge_vec.size(); ++k){
            const PEdge& pe = pedge_vec[k];
            int pn_idx1(pe.pnode_index_vec[0]), pn_idx2(pe.pnode_index_vec[1]);
            pnode_vec[pn_idx1].adj_pedge_index_vec.push_back((int) k);
            pnode_vec[pn_idx2].adj_pedge_index_vec.push_back((int) k);
        }
    }

    void PolyPatch::SortAdjPEdges() 
    {
        for(size_t k=0; k<pnode_vec.size(); ++k){
            SortAdjPEdges((int) k);
        }
    }
    
    void PolyPatch::SortAdjPEdges(int pn_index) 
    {
        //! copy the path array
        PNode& pn = pnode_vec[pn_index];
        size_t path_num = pn.adj_pedge_index_vec.size();
        vector< vector<int> > path_vec(path_num);
        map<int, vector<int> > mv_pei_mapping; // mesh vertex / pedge index
        
        for(size_t k=0; k<pn.adj_pedge_index_vec.size(); ++k){
            const PEdge& pe = pedge_vec[ pn.adj_pedge_index_vec[k]];
            path_vec[k].resize(pe.mesh_path.size());
            if(pe.pnode_index_vec[0] == pn_index){
                copy(pe.mesh_path.begin(), pe.mesh_path.end(), path_vec[k].begin());
            }else{
                reverse_copy(pe.mesh_path.begin(), pe.mesh_path.end(), path_vec[k].begin());
            }

            for(size_t i=0; i<path_vec[k].size(); ++i){
                mv_pei_mapping[path_vec[k][i]].push_back((int)k);
            }
        }

        //! build the tree
        map<int, vector<int> > children; // node and its children
        map<int, vector<int> > parents; // each child's parents
        for(size_t k=0; k<path_vec.size(); ++k){
            for(size_t i=0; i<path_vec[k].size()-1; ++i){
                int pa = path_vec[k][i]; // parent
                int ch = path_vec[k][(i+1)%path_vec[k].size()]; // child
                children[pa].push_back(ch);
                parents[ch].push_back(pa);
            }
        }

        //! sort children for each node
        for(map<int, vector<int> >::iterator im = children.begin(); im!=children.end(); ++im){
            int node = im->first;
            vector<int>& pa = parents[node];
            vector<int>& ch = im->second;
            vector<int> bak_ch = ch; //backup
            ch.clear();

            //int seprator = GetSepratorIndex(node, pa, bak_ch);
            
        }
    }

    void PolyPatch::FormPatchs()
    {
        set< pair<int, int> > visited_set;
        for(size_t pn_idx=0; pn_idx<pnode_vec.size(); ++pn_idx){
            const PNode& pn = pnode_vec[pn_idx];
            size_t adj_pe_num = pn.adj_pedge_index_vec.size();
            for(size_t k=0; k<adj_pe_num ; ++k){
                int pe_idx1 = pn.adj_pedge_index_vec[k];
                int pe_idx2 = pn.adj_pedge_index_vec[(k+1)%adj_pe_num];

                if(visited_set.find( make_pair(pe_idx1, pe_idx2)) != visited_set.end()) continue;
                
                Patch patch = FormSinglePatch((int)pn_idx, pe_idx2);
                size_t patch_pe_num = patch.pedge_index_vec.size();
                for(size_t i=0; i<patch_pe_num; ++i){
                    int _pe_idx1 = patch.pedge_index_vec[i];
                    int _pe_idx2 = patch.pedge_index_vec[(i+1)%patch_pe_num];

                    assert(visited_set.find(make_pair(_pe_idx1, _pe_idx2)) == visited_set.end() );
                    visited_set.insert(make_pair(_pe_idx1, _pe_idx2));
                }
            }
        }
    }

    Patch PolyPatch::FormSinglePatch(int start_pn_idx, int start_pe_idx) const
    {
        Patch new_patch;
        int cur_pe_idx(start_pe_idx), cur_pn_idx(start_pn_idx);
        int nxt_pe_idx, nxt_pn_idx;
        do{
            const PEdge& pe = pedge_vec[cur_pe_idx];
            const PNode& pn = pnode_vec[cur_pn_idx];
            nxt_pn_idx = GetAnotherCornerIndex(pe, cur_pn_idx);
            nxt_pe_idx = GetNextPEdgeIndex(pn, cur_pe_idx);

            new_patch.pnode_index_vec.push_back(cur_pn_idx);
            new_patch.pedge_index_vec.push_back(cur_pe_idx);
            
            cur_pn_idx = nxt_pn_idx;
            cur_pe_idx = nxt_pe_idx;
        }while(cur_pe_idx != start_pe_idx);

        return new_patch;
    }

    int PolyPatch::GetNextPEdgeIndex(const PNode& pn, int cur_pe_idx) const
    {
        const vector<int>& pe_iv = pn.adj_pedge_index_vec;
        vector<int>::const_iterator cur_iter = find(pe_iv.begin(), pe_iv.end(), cur_pe_idx);
        assert(cur_iter != pe_iv.end());
        vector<int>::const_iterator nxt_iter = ( cur_iter == pe_iv.end() -1) ? pe_iv.begin() : cur_iter + 1;

        return *nxt_iter;
    }

    int PolyPatch::GetPrevPEdgeIndex(const PNode& pn, int cur_pe_idx) const
    {
        const vector<int>& pe_iv = pn.adj_pedge_index_vec;
        vector<int>::const_iterator cur_iter = find(pe_iv.begin(), pe_iv.end(), cur_pe_idx);
        assert(cur_iter != pe_iv.end());
        vector<int>::const_iterator pre_iter = ( cur_iter == pe_iv.begin() ) ? pe_iv.end() -1 : cur_iter - 1;

        return *pre_iter;
    }


    bool PolyPatch::LoadPatchFile(const std::string& file_name) 
    {
        ifstream fin(file_name.c_str());
        if(fin.fail()){
            cerr << "Cannot load patch file: " << file_name << endl;
            return false;
        }
        pnode_vec.clear(); pedge_vec.clear(); patch_vec.clear();
        int pnode_num, pedge_num, patch_num;
        fin >> pnode_num;
        for(int k=0; k<pnode_num; ++k){
            PNode pn;
            fin >> pn.vert_index;
            int adj_num;
            fin >> adj_num;
            pn.adj_pedge_index_vec.resize(adj_num);
            for(int i=0; i<adj_num; ++i){
                fin >> pn.adj_pedge_index_vec[i];
            }
            pnode_vec.push_back(pn);
        }

        int adj_patch_num, path_vert_num;
        fin >> pedge_num;
        for(int k=0; k<pedge_num; ++k){
            PEdge pe;
            fin >> pe.pnode_index_vec[0] >>  pe.pnode_index_vec[1];
            fin >> adj_patch_num;
            pe.adj_patch_index_vec.resize(adj_patch_num);
            for(int i=0; i<adj_patch_num; ++i) fin >> pe.adj_patch_index_vec[i];
            fin >> path_vert_num;
            pe.mesh_path.resize(path_vert_num);
            for(int i=0; i<path_vert_num; ++i) fin >> pe.mesh_path[i];
            pedge_vec.push_back(pe);
        }

        int pn_num, pe_num, face_num;
        fin >> patch_num;
        for(int k=0; k<patch_num; ++k){
            Patch ph;
            fin >> pn_num;
            ph.pnode_index_vec.resize(pn_num);
            for(int i=0; i<pn_num; ++i) fin >> ph.pnode_index_vec[i];
            fin >> pe_num;
            ph.pedge_index_vec.resize(pe_num);
            for(int i=0; i<pe_num; ++i) fin >> ph.pedge_index_vec[i];
            fin >> face_num;
            ph.mesh_face_vec.resize(face_num);
            for(int i=0; i<face_num; ++i) fin >> ph.mesh_face_vec[i];
        }
        fin.close();
        return true;
    }

    bool PolyPatch::SavePatchFile(const string& file_name) const
    {
        ofstream fout(file_name.c_str());
        if(fout.fail()) {
            cerr << " Save patch file fail, canont open " << file_name << endl;
            return false;
        }

        fout << pnode_vec.size();
        for(size_t k = 0; k<pnode_vec.size(); ++k){
            const PNode& pn = pnode_vec[k];
            fout << pn.vert_index << endl;
            size_t adj_pe_num = pn.adj_pedge_index_vec.size();
            fout << adj_pe_num << endl;
            for(size_t i=0; i < adj_pe_num -1; ++i) fout << pn.adj_pedge_index_vec[i] <<" ";
            fout << pn.adj_pedge_index_vec[adj_pe_num - 1] << endl;
        }

        size_t adj_pnum, vert_num;
        fout << pedge_vec.size();
        for(size_t k=0; k<pedge_vec.size(); ++k){
            const PEdge& pe = pedge_vec[k];
            fout << pe.pnode_index_vec[0] <<" "<< pe.pnode_index_vec[1] << endl;
            adj_pnum = pe.adj_patch_index_vec.size();
            fout << adj_pnum;
            for(size_t i=0; i<adj_pnum-1; ++i) fout << pe.adj_patch_index_vec[i] << " ";
            fout << pe.adj_patch_index_vec[adj_pnum-1] << endl;
            vert_num = pe.mesh_path.size();
            fout << vert_num;
            for(size_t i=0; i<vert_num-1; ++i) fout << pe.mesh_path[i] <<" ";
            fout << pe.mesh_path[vert_num-1] << endl;
        }
        
        fout << patch_vec.size();
        for(size_t k=0; k<patch_vec.size(); ++k){
            const Patch& ph = patch_vec[k];
            size_t pn_num = ph.pnode_index_vec.size();
            fout << pn_num << endl;
            for(size_t i=0; i<pn_num-1; ++i) fout << ph.pnode_index_vec[i]<<" ";
            fout << ph.pnode_index_vec[pn_num-1] << endl;
            size_t pe_num = ph.pedge_index_vec.size();
            fout << pe_num << endl;
            for(size_t i=0; i<pe_num-1; ++i) fout << ph.pedge_index_vec[i]<<" ";
            fout << ph.pedge_index_vec[pe_num-1] << endl;
            size_t face_num = ph.mesh_face_vec.size();
            for(size_t i=0; i<face_num; ++i) fout << ph.mesh_face_vec[i] << " ";
            fout << ph.mesh_face_vec[face_num-1] << endl;
        }
        fout.close();
        return true;
    }
}
