#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osgViewer/Viewer>
#include <osg/Group>
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>

#ifdef _DEBUG
	#pragma comment(lib, "osgd.lib")
	#pragma comment(lib, "osgDBd.lib")
	#pragma comment(lib, "osgViewerd.lib")
#else
	#pragma comment(lib, "osg.lib")
	#pragma comment(lib, "osgDB.lib")
	#pragma comment(lib, "osgViewer.lib")
#endif

int main( int argc, char** argv )
{
	// töltsünk be egy cessna modellt
    osg::ref_ptr<osg::Node> model = osgDB::readNodeFile( "cessna.osg" );
    
	// hozzunk létre egy eltolást a (2,0,0) vektorral
    osg::ref_ptr<osg::MatrixTransform> trafo1 = new osg::MatrixTransform;
    trafo1->setMatrix( osg::Matrix::translate(2.0f, 0.0f, 0.0f) );
	// a transzformációs csúcs gyermeke legyen a fenti betöltött modell
    trafo1->addChild( model.get() );

	// a gyökérelem legyen egy group
	osg::ref_ptr<osg::Group> root = new osg::Group;
	// aminek szúrjuk be gyerekként a transzformációs csúcsot
    root->addChild( trafo1 );

    osgViewer::Viewer viewer;
    viewer.setSceneData( root.get() );

    viewer.setUpViewInWindow(20, 20, 640, 480); 
    viewer.realize(); 

    return viewer.run();
}