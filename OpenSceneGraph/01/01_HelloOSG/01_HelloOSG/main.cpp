#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osgViewer/Viewer>

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
	// hozzunk l�tre egy ShapeDrawable-t
    osg::ref_ptr<osg::ShapeDrawable> shape1 = new osg::ShapeDrawable;
	// amibe rakjunk bele egy...
    shape1->setShape( 
				new osg::Box(osg::Vec3(0.0f, 0.0f, 0.0f),	// orig� k�z�ppont�...
				4.0f, 3.0f, 2.0f) );						// 4x3x2-es t�glatestet

	// a sz�nt�rgr�funk gy�kere legyen egy Geode
	osg::ref_ptr<osg::Geode> root = new osg::Geode;
	// aminek kirajzoland� elemk�nt adjuk hozz� a fenti t�glatestet
    root->addDrawable( shape1.get() );
    
	// hozzuk l�tre a viewer-t �s �ll�tsuk be a gy�keret megjelen�tend� adatnak
    osgViewer::Viewer viewer;
    viewer.setSceneData( root.get() );
	
	// a (20,20) kezdeti poz�ci�ba hozzunk l�tre egy 640x480-as ablakot
    viewer.setUpViewInWindow(20, 20, 640, 480); 
    viewer.realize(); 

	// adjuk �t a vez�rl�st a viewer-nek
    return viewer.run();
}