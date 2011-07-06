#ifndef FINDGEONAMEDNODE_H_
#define FINDGEONAMEDNODE_H_

#include <osg/NodeVisitor>
#include <osg/Geode>
#include <string>

class FindGeoNamedNode : public osg::NodeVisitor
{
 public:
    FindGeoNamedNode();
    FindGeoNamedNode(const std::string& _file_name)
        : osg::NodeVisitor(TRAVERSE_ALL_CHILDREN){
        result_node = NULL;
        name = _file_name;
    }

    virtual void apply(osg::ref_ptr<osg::Node> search_node){
        if(search_node == NULL) return;
        if(search_node->getName() == name){
            ref_ptr<osg::Geode> dynamic_try = dynamic_cast<
                ref_ptr<osg::Geode> >(search_node);
            if(dynamic_try) result_node = dynamic_try;
        }else{
            traverse(search_node);
        }
    }

 private:
    osg::ref_ptr<osg::Geode> result_node;
    std::string name;
};

#endif
