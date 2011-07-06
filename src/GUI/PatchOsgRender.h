#ifndef PATCHOSGRENDER_H_
#define PATCHOSGRENDER_H_

#include <osg/Group>
#include <osg/Switch>
#include "../Patch/PolyPatch.h"

class PatchOsgRender{
 public:
    PatchOsgRender(const PolyPatch::PolyPatch& poly_patch);
    ~PatchOsgRender();

    osg::ref_ptr<osg::Switch> GetOsgSwitch() { return osg_switch; }

    void CreateOSGPatchNode();

 private:
    osg::ref_ptr<osg::Geode> CreatePNodeGeode();
    osg::ref_ptr<osg::Geode> CreatePEdgeGeode();
    
 private:
    osg::ref_ptr<osg::Switch> osg_switch;
    const PolyPatch::PolyPatch& poly_patch;
};

#endif
