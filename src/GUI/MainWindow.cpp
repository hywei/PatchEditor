#include "MainWindow.h"
#include "AdapterWidget.h"
#include "PatchEditorControl.h"
#include "../Patch/PolyPatch.h"

#include <osg/Drawable>
#include <osg/Geometry>
#include <osg/Material>
#include <osg/PolygonMode>
#include <osg/PolygonOffset>
#include <osg/LineWidth>

#include <QtGui>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;
using namespace PolyPatch;

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent)
{
	setMinimumSize(20, 20);
	resize(1200, 900); 

    editor_control = new PatchEditorControl(this, this);
    viewer_window = new ViewerQT;
    viewer_window ->setCameraManipulator(new osgGA::TrackballManipulator);

	createActions();
	createMenus();
	createToolBars();
    createStatusBar();

    QList <int> region_size;
    region_size << 900 << 300;

    QSplitter* splitter = new QSplitter(Qt::Horizontal);
    splitter -> addWidget(viewer_window);
    splitter -> addWidget(editor_control);
    splitter -> setSizes(region_size);
    
    setCentralWidget(splitter);
	setWindowTitle("OSG-QT Viewer");
}

void MainWindow::pointDisplay(){
    //osg_mesh_switch->setAllChildrenOff();
    osg_mesh_switch->setValue(1, false);
    osg_mesh_switch->setValue(0, true);
    osg::ref_ptr<osg::StateSet> osg_state = osg_solid_gp->getOrCreateStateSet();
    osg_state->setAttribute(new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::POINT));
}

void MainWindow::wireDisplay(){
    //osg_mesh_switch->setAllChildrenOff();
    osg_mesh_switch->setValue(0, false);
    osg_mesh_switch->setValue(1, true);
    osg::ref_ptr<osg::StateSet> osg_state = osg_wire_gp->getOrCreateStateSet();
    osg_state->setAttribute(new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE));
}

void MainWindow::solidDisplay(){
    //osg_mesh_switch->setAllChildrenOff();
    osg_mesh_switch->setValue(0, true);
    osg_mesh_switch->setValue(1, false);
    osg::ref_ptr<osg::StateSet> osg_state = osg_solid_gp->getOrCreateStateSet();
    osg_state->setAttribute(new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL));
}

void MainWindow::flatwireDisplay()
{
    //osg_mesh_switch->setAllChildrenOff();
    osg_mesh_switch->setValue(0, true);
    osg_mesh_switch->setValue(1, true);
    osg::ref_ptr<osg::StateSet> osg_state = osg_solid_gp->getOrCreateStateSet();
    osg_state->setAttribute(new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL));
    osg_state = osg_wire_gp->getOrCreateStateSet();
    osg_state->setAttribute(new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE));
}

void MainWindow::SetPNodeSwitch(bool toggled)
{
    osg_mesh_switch->setValue(2, toggled);
}

void MainWindow::SetPEdgeSwitch(bool toggled)
{
    osg_mesh_switch->setValue(3, toggled);
}

void MainWindow::SetPatchSwitch(bool toggled)
{
}

void MainWindow::createActions()
{
	// file menu actions
	openModelAct = new QAction(QIcon("../images/open.png"), tr("&Open Model"), this);
	openModelAct->setShortcut(QKeySequence::New);
	openModelAct->setStatusTip(tr("open a mesh model"));
	connect(openModelAct, SIGNAL(triggered()), this, SLOT(openModel()));
    
	saveModelAct = new QAction(QIcon("../images/save.png"), tr("&Save Model"), this);
	saveModelAct->setShortcut(QKeySequence::Save);
	saveModelAct->setStatusTip(tr("save this mesh model"));
	connect(saveModelAct, SIGNAL(triggered()), this, SLOT(saveModel()));

	exitAct = new QAction(tr("E&xit"), this);
	exitAct->setShortcuts(QKeySequence::Quit);
	exitAct->setStatusTip(tr("Exit the application"));
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
    
	// toolbar menu actions
	toolBarAct = new QAction(tr("ToolBar"), this);
	stateBarAct = new QAction(tr("StateBar"), this);
	
	// help menu actions
	aboutAct = new QAction(tr("About"), this);
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
    
	solidDisplayAct = new QAction(QIcon("../images/flat.png"),tr("Flat"),this);
	connect(solidDisplayAct, SIGNAL(triggered()), this, SLOT(solidDisplay()));

	wireDisplayAct = new QAction(QIcon("../images/wire.png"), tr("Wire"), this);
	connect(wireDisplayAct, SIGNAL(triggered()), this, SLOT(wireDisplay()));

	pointDisplayAct = new QAction(QIcon("../images/points.png"), tr("Point"), this);
	connect(pointDisplayAct, SIGNAL(triggered()), this, SLOT(pointDisplay()));
    flatwireDisplayAct = new QAction(QIcon("../images/flatlines.png"), tr("FlatWire"), this);
    connect(flatwireDisplayAct, SIGNAL(triggered()), this, SLOT(flatwireDisplay()));
}


void MainWindow::createMenus()
{
	// create file menu
	QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(openModelAct);
	fileMenu->addAction(saveModelAct);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAct);

	// create view menu
	QMenu* viewMenu = menuBar()->addMenu(tr("&View"));
	viewMenu->addAction(toolBarAct);
	viewMenu->addAction(stateBarAct);
    
	// create help menu
	QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(aboutAct);
}

void MainWindow::createToolBars()
{
	QToolBar* fileToolBar = addToolBar(tr("File"));
	fileToolBar->addAction(openModelAct);
	fileToolBar->addAction(saveModelAct);

	QToolBar* mouseActToolBar = addToolBar(tr("Mouse Action"));
	mouseActToolBar->addAction(solidDisplayAct);
    mouseActToolBar->addAction(flatwireDisplayAct);
	mouseActToolBar->addAction(wireDisplayAct);
	mouseActToolBar->addAction(pointDisplayAct);
}

void MainWindow::createStatusBar(){}

void MainWindow::openModel()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open mesh file"), tr("/home/hywei/Data"), tr("OBJ Files (*.obj);;" "All Files (*)"));

    std::string fn = std::string((const char*) fileName.toLocal8Bit());
    osg::Node* loadedModel = osgDB::readNodeFile(fn);

    if(!loadedModel ){
        std::cout << "No data loaded." << std::endl;
        return ;
    }

    mesh.request_vertex_normals();
    /// read mesh info by openmesh
    if(! OpenMesh::IO::read_mesh(mesh, fn)){
        std::cout <<"Can't read mesh !" << std::endl;
        return ;
    }else{
        QString q_fn = fn.c_str();
        statusBar()->showMessage(tr("Open Model") + QString(fn.c_str()));
    }
    mesh.request_face_normals();
    mesh.update_normals();
    std::cout << mesh.n_vertices() << std::endl;

    osg_root = new osg::Group;
    CreateOsgMeshNode(mesh);
    osg_root->addChild(osg_mesh_switch);

    solidDisplay();
    statusBar()->showMessage(tr("Current Model : ") + QString(fn.c_str()));

    viewer_window->setSceneData(osg_root.get());
    viewer_window->show();
    
    p_poly_patch = boost::shared_ptr<PolyPatch::PolyPatch> (new PolyPatch::PolyPatch(mesh));
}

void MainWindow::CreateOsgMeshNode(TriMesh& mesh)
{
    osg_mesh_switch = new osg::Switch;
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    osg::ref_ptr<osg::Geometry> osg_geo = new osg::Geometry;
    geode->addDrawable(osg_geo.get());
    osg::ref_ptr<osg::Vec3Array> osg_vert_vec = new osg::Vec3Array;
    osg::ref_ptr<osg::Vec3Array> osg_norm_vec = new osg::Vec3Array;
    osg_geo->setVertexArray(osg_vert_vec.get());
    osg_geo->setNormalArray(osg_norm_vec.get());

    for(TriMesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it){
        for(TriMesh::FaceVertexIter fv_it = mesh.fv_iter(f_it.handle()); fv_it; ++fv_it){
            TriMesh::Point coord = mesh.point(fv_it);
            TriMesh::Point norm = mesh.normal(fv_it);
            osg_vert_vec->push_back(osg::Vec3(coord[0], coord[1], coord[2]));
            osg_norm_vec->push_back(osg::Vec3(norm[0], norm[1], norm[2]));
        }
    }
    
    osg_geo->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
    osg_geo->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::TRIANGLES, 0, osg_vert_vec->size() ) );

    osg_solid_gp = CreateOsgSolidGroup(geode);
    osg_wire_gp = CreateOsgWireGroup(geode);
    osg_mesh_switch->addChild(osg_solid_gp);
    osg_mesh_switch->addChild(osg_wire_gp);
}

osg::ref_ptr<osg::Group> MainWindow::CreateOsgSolidGroup(osg::ref_ptr<osg::Geode> geode)
{
    osg::ref_ptr<osg::Group> solid_gp = new osg::Group;
    solid_gp->addChild(geode.get());
    osg::ref_ptr<osg::StateSet> solid_st = solid_gp->getOrCreateStateSet();
    solid_st->setAttribute(new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL));
    solid_st->setMode(GL_LIGHTING, osg::StateAttribute::ON);
    osg::ref_ptr<osg::Material> mat = new osg::Material;
    //mat->setDiffuse(osg::Material::FRONT,osg::Vec4(col[0],col[1],col[2],1.f) );
    mat->setShininess( osg::Material::FRONT, 96.f );
    solid_st->setAttribute(mat.get());

    return solid_gp;
}

osg::ref_ptr<osg::Group> MainWindow::CreateOsgWireGroup(osg::ref_ptr<osg::Geode> geode)
{
    osg::ref_ptr<osg::Group> wire_gp = new osg::Group;
    wire_gp->addChild(geode.get());
    osg::ref_ptr<osg::StateSet> wire_st = wire_gp->getOrCreateStateSet();
    wire_st->setAttribute(new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE));
    wire_st->setAttribute(new osg::PolygonOffset(-0.5f, 0));
    wire_st->setMode(GL_POLYGON_OFFSET_LINE, osg::StateAttribute::ON);
    wire_st->setMode( GL_LIGHTING, osg::StateAttribute::ON );
    osg::ref_ptr<osg::Material> mat = new osg::Material;
    mat->setDiffuse(osg::Material::FRONT,osg::Vec4(0.0, 0.0, 1.0, 1.0));
    mat->setShininess( osg::Material::FRONT, 96.f );
    wire_st->setAttribute(mat.get());
    return wire_gp;
}

osg::ref_ptr<osg::Group> MainWindow::CreateOsgPointGroup(osg::ref_ptr<osg::Geode> geode)
{
    osg::ref_ptr<osg::Group> point_gp = new osg::Group;
    return point_gp;
}

void MainWindow::saveModel(){}
void MainWindow::about(){}
MainWindow::~MainWindow(){}

void MainWindow::loadPatchFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("load patch file"), tr("/home/hywei/Data"), tr("Patch Files (*.patch);;" "All Files (*)"));
    std::string fn = std::string((const char*) fileName.toLocal8Bit());
    if(!p_poly_patch->LoadPatchFile(fn)) return;
    CreateOsgPatchNode();
    osg_root->addChild(osg_patch_switch);
}

void MainWindow::savePatchFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("save patch file"), tr("/home/hywei/Data"), tr("Patch Files (*.patch);;" "All Files (*)"));
    std::string fn = std::string((const char*) fileName.toLocal8Bit());
    if(!p_poly_patch->SavePatchFile(fn)) return;
}
    
void MainWindow::CreateOsgPatchNode()
{
    osg_mesh_switch->addChild(CreateOsgPNodeGeode());
    osg_mesh_switch->addChild(CreateOsgPEdgeGeode());
}

osg::ref_ptr<osg::Geode> MainWindow::CreateOsgPNodeGeode()
{
    osg::ref_ptr<osg::Geode> pnode_geode = new osg::Geode;
    const TriMesh& mesh = p_poly_patch->GetMesh();
    const vector<PNode>& pnode_vec = p_poly_patch->GetPNodes();    
    
    for(size_t k=0; k<pnode_vec.size(); ++k){
        int v_id = pnode_vec[k].vert_index;
        TriMesh::VertexHandle vh = mesh.vertex_handle(v_id);
        TriMesh::Point coord = mesh.point(vh);
        osg::Sphere* corner_sphere = new osg::Sphere( osg::Vec3( coord[0], coord[1], coord[2]), 0.1f);
        osg::ShapeDrawable* corner_sphere_drawable = new osg::ShapeDrawable(corner_sphere);
        corner_sphere_drawable->setColor(osg::Vec4(1.0, 0.0, 0.0, 1.0));
        pnode_geode->addDrawable(corner_sphere_drawable);
    }

    return pnode_geode;   
}

osg::ref_ptr<osg::Geode> MainWindow::CreateOsgPEdgeGeode()
{
    osg::ref_ptr<osg::Geode> pedge_geode = new osg::Geode;
    const TriMesh& mesh = p_poly_patch->GetMesh();
    const vector<PEdge>& pedge_vec = p_poly_patch->GetPEdges();

    osg::ref_ptr<osg::StateSet> pedge_st = pedge_geode->getOrCreateStateSet();
    pedge_st->setAttribute(new osg::LineWidth(4.0f));
    
    for(size_t k=0; k<pedge_vec.size(); ++k){
        const vector<int>& path = pedge_vec[k].mesh_path;
        osg::Geometry* geom = new osg::Geometry;
        osg::DrawArrays* drawArrayLines = new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP);
        osg::Vec3Array* vert_data = new osg::Vec3Array;
        
        geom->addPrimitiveSet(drawArrayLines);
        geom->setVertexArray(vert_data);

        osg::Vec4Array* line_colors = new osg::Vec4Array;
        line_colors->push_back(osg::Vec4(0.0f, 0.5f, 0.0f, 1.0f));
        geom->setColorArray(line_colors);
        geom->setColorBinding(osg::Geometry::BIND_OVERALL);
        
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
