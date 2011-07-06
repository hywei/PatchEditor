#include "PatchOsgRender.h"
#include <vector>

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Drawable>
#include <osg/ShapeDrawable>

using namespace std;
using namespace PolyPatch;

PatchOsgRender::PatchOsgRender(const PolyPatch::PolyPatch& _poly_patch) : poly_patch(_poly_patch)
{
    osg_switch = new osg::Switch;
}
PatchOsgRender::~PatchOsgRender(){}

void PatchOsgRender::CreateOSGPatchNode()
{
    osg_switch->addChild(CreatePNodeGeode());
    osg_switch->addChild(CreatePEdgeGeode());
}

osg::ref_ptr<osg::Geode> PatchOsgRender::CreatePNodeGeode()
{
    osg::ref_ptr<osg::Geode> pnode_geode = new osg::Geode;
    const TriMesh& mesh = poly_patch.GetMesh();
    const vector<PNode>& pnode_vec = poly_patch.GetPNodes();

    for(size_t k=0; k<pnode_vec.size(); ++k){
        int v_id = pnode_vec[k].vert_index;
        TriMesh::VertexHandle vh = mesh.vertex_handle(v_id);
        TriMesh::Point coord = mesh.point(vh);
        osg::Sphere* corner_sphere = new osg::Sphere( osg::Vec3( coord[0], coord[1], coord[2]), 1.0f);
        osg::ShapeDrawable* corner_sphere_drawable = new osg::ShapeDrawable(corner_sphere);
        pnode_geode->addDrawable(corner_sphere_drawable);
    }

    return pnode_geode;
}

osg::ref_ptr<osg::Geode> PatchOsgRender::CreatePEdgeGeode()
{
    osg::ref_ptr<osg::Geode> pedge_geode = new osg::Geode;
    const TriMesh& mesh = poly_patch.GetMesh();
    const vector<PEdge>& pedge_vec = poly_patch.GetPEdges();
    
    for(size_t k=0; k<pedge_vec.size(); ++k){
        const vector<int>& path = pedge_vec[k].mesh_path;
        osg::Geometry* geom = new osg::Geometry;
        osg::DrawArrays* drawArrayLines = new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP);
        osg::Vec3Array* vert_data = new osg::Vec3Array;
        
        geom->addPrimitiveSet(drawArrayLines);
        geom->setVertexArray(vert_data);

        for(size_t i=0; i<path.size(); ++i){
            int v_id = path[i];
            TriMesh::VertexHandle vh = mesh.vertex_handle(v_id);
            TriMesh::Point coord = mesh.point(vh);
            vert_data->push_back(osg::Vec3(coord[0], coord[1], coord[2]));
        }
        drawArrayLines->setFirst(0);
        drawArrayLines->setCount(vert_data->size());
        
        pedge_geode->addDrawable(geom);
    }

    return pedge_geode;
}



