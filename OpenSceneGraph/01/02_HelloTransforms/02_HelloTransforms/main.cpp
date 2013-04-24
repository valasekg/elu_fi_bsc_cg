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
	// t�lts�nk be egy cessna modellt
    osg::ref_ptr<osg::Node> model = osgDB::readNodeFile( "cessna.osg" );
    
	// hozzunk l�tre egy eltol�st a (2,0,0) vektorral
    osg::ref_ptr<osg::MatrixTransform> trafo1 = new osg::MatrixTransform;
    trafo1->setMatrix( osg::Matrix::translate(2.0f, 0.0f, 0.0f) );
	// a transzform�ci�s cs�cs gyermeke legyen a fenti bet�lt�tt modell
    trafo1->addChild( model.get() );

	// a gy�k�relem legyen egy group
	osg::ref_ptr<osg::Group> root = new osg::Group;
	// aminek sz�rjuk be gyerekk�nt a transzform�ci�s cs�csot
    root->addChild( trafo1 );

    osgViewer::Viewer viewer;
    viewer.setSceneData( root.get() );

    viewer.setUpViewInWindow(20, 20, 640, 480); 
    viewer.realize(); 

    return viewer.run();
}