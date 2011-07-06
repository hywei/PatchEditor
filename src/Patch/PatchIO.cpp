#include "PatchIO.h"
#include "PatchMaker.h"
#include <fstream>
using namespace std;

namespace PolyPatch{
    namespace IO{
        bool ReadPatch(PolyPatch& poly_patch, const string& file_name)
        {
            ifstream fin(file_name.c_str());
            if(fin.fail()){
                cerr << "Read Patch fail, Cannot open file " << file_name << endl;
                return false;
            }
            int pnode_num(0), pedge_num(0), patch_num(0);
            fin >> pnode_num;
            for(int k=0; k<pnode_num; ++k){
                PNode pn;
                fin >> pn.vert_index;
                int adj_num;
                fin >> adj_num;
                for(int i=0; i<adj_num; ++i){
                    int pe_index;
                    fin >> pe_index;
                    pn.adj_pedge_index_vec.push_back(pe_index);
                }

                //poly_patch.pnode_vec.push_back(pn);
            }

            fin >> pedge_num;
            for(int k=0; k<pedge_num; ++k){
                
            }

            fin.close();
            return true;
        }

        bool WritePatch(const PolyPatch& poly_patch, const string& file_name)
        {
            ofstream fout(file_name.c_str());
            if(fout.fail()){
                cerr << "Write Patch fail, Cannot open file " << file_name << endl;
                return false;
            }

            fout << "# Corners: " << endl;
            const vector<PNode>& pnodes = poly_patch.GetPNodes();
            for(size_t k=0; k<pnodes.size(); ++k){
                fout << "C " << pnodes[k].vert_index << endl;
            }

            fout << "# Edges: " << endl;
            const vector<PEdge>& pedges = poly_patch.GetPEdges();
            for(size_t k=0; k<pedges.size(); ++k){
                fout << "E " ;
                const vector<int>& path = pedges[k].mesh_path;
                for(size_t i=0; i<path.size()-1; ++i) fout << path[i] <<" ";
                fout<< path[path.size()-1] << endl;
            }

            fout << "# Patchs: " << endl;
            const vector<Patch>& patchs = poly_patch.GetPatchs();
            for(size_t k=0; k<patchs.size(); ++k){
                fout << "P ";
                const vector<int>& pe_iv = patchs[k].pedge_index_vec;
                for(size_t i=0; i<pe_iv.size()-1; ++i) fout << pe_iv[i] <<" ";
                fout << pe_iv[pe_iv.size()-1] << endl;
            }

            fout.close();
            return true;
        }
    }
}
