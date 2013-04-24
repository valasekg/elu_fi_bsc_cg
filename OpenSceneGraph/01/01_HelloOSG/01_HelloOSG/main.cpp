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
	// hozzunk létre egy ShapeDrawable-t
    osg::ref_ptr<osg::ShapeDrawable> shape1 = new osg::ShapeDrawable;
	// amibe rakjunk bele egy...
    shape1->setShape( 
				new osg::Box(osg::Vec3(0.0f, 0.0f, 0.0f),	// origó középpontú...
				4.0f, 3.0f, 2.0f) );						// 4x3x2-es téglatestet

	// a színtérgráfunk gyökere legyen egy Geode
	osg::ref_ptr<osg::Geode> root = new osg::Geode;
	// aminek kirajzolandó elemként adjuk hozzá a fenti téglatestet
    root->addDrawable( shape1.get() );
    
	// hozzuk létre a viewer-t és állítsuk be a gyökeret megjelenítendõ adatnak
    osgViewer::Viewer viewer;
    viewer.setSceneData( root.get() );
	
	// a (20,20) kezdeti pozícióba hozzunk létre egy 640x480-as ablakot
    viewer.setUpViewInWindow(20, 20, 640, 480); 
    viewer.realize(); 

	// adjuk át a vezérlést a viewer-nek
    return viewer.run();
}