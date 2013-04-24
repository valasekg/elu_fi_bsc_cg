#include <osg/Geometry>
#include <osg/Geode>
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>
#include <iostream>
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>

#ifdef _DEBUG
	#pragma comment(lib, "osgd.lib")
	#pragma comment(lib, "osgGAd.lib")
	#pragma comment(lib, "osgDBd.lib")
	#pragma comment(lib, "osgViewerd.lib")
#else
	#pragma comment(lib, "osg.lib")
	#pragma comment(lib, "osgGA.lib")
	#pragma comment(lib, "osgDB.lib")
	#pragma comment(lib, "osgViewer.lib")
#endif

int main( int argc, char** argv )
{
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    vertices->push_back( osg::Vec3(0.0f, 0.0f, 0.0f) );
    vertices->push_back( osg::Vec3(1.0f, 0.0f, 0.0f) );
    vertices->push_back( osg::Vec3(1.0f, 0.0f, 1.0f) );
    vertices->push_back( osg::Vec3(0.0f, 0.0f, 1.0f) );
    
    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
    normals->push_back( osg::Vec3(0.0f,-1.0f, 0.0f) );
    
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    colors->push_back( osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f) );
    colors->push_back( osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f) );
    colors->push_back( osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f) );
    colors->push_back( osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) );
    
    osg::ref_ptr<osg::Geometry> quad = new osg::Geometry;
    quad->setVertexArray( vertices.get() );
    quad->setNormalArray( normals.get() );
    quad->setNormalBinding( osg::Geometry::BIND_OVERALL );
    quad->setColorArray( colors.get() );
    quad->setColorBinding( osg::Geometry::BIND_PER_VERTEX );
    quad->addPrimitiveSet( new osg::DrawArrays(GL_QUADS, 0, 4) );

	osg::ref_ptr<osg::Node> model = osgDB::readNodeFile( "cessna.osg" );

	osg::ref_ptr<osg::MatrixTransform> trafo1 = new osg::MatrixTransform;
	trafo1->setMatrix( osg::Matrix::rotate(0, osg::Vec3(0, 1, 0) ));
	trafo1->addChild( model.get() );

    osg::ref_ptr<osg::Group> root = new osg::Group;
	root->addChild( trafo1 );
    
    osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(20, 20, 640, 480);
	viewer.setCameraManipulator( new osgGA::TrackballManipulator );

    viewer.setSceneData( root.get() );
    while ( !viewer.done() )
    {
        viewer.frame();
		osg::Matrix mt = trafo1->getMatrix();
        std::cout << "Frame number: " << viewer.getFrameStamp()->getFrameNumber() << std::endl;
		mt *= osg::Matrix::rotate( 0.1f, osg::Vec3(0, 1, 0) );
		trafo1->setMatrix( mt );
    }
	return 0;
}
