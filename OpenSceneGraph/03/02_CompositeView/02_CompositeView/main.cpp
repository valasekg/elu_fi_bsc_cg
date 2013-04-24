#include <osgDB/ReadFile>
#include <osgViewer/CompositeViewer>
#include <osg/MatrixTransform>

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
	//
	// a k�t modellb�l �ll� sz�nter�nket n�zz�k k�t k�l�nb�z� ablakban
	//

	// modellek bet�lt�se
	osg::ref_ptr<osg::Node> model1 = osgDB::readNodeFile("cessna.osg");
	osg::ref_ptr<osg::Node> model2 = osgDB::readNodeFile("cow.osg");

	// transzform�ci�s node-ok elk�sz�t�se
	osg::ref_ptr<osg::MatrixTransform> traf1 = new osg::MatrixTransform;
	traf1->setMatrix( osg::Matrix::translate( osg::Vec3( 5, 0, 0 ) ));
	traf1->addChild( model1.get() );

	osg::ref_ptr<osg::MatrixTransform> traf2 = new osg::MatrixTransform;
	traf2->setMatrix( osg::Matrix::translate( osg::Vec3(-3,-3,-0.5f ) ));
	traf2->addChild( model2.get() );

	// gy�k�relem elk�sz�t�se
	osg::ref_ptr<osg::Group> root = new osg::Group;
	root->addChild( traf1.get() );
	root->addChild( traf2.get() );

	// n�zetek l�trehoz�sa
	osg::ref_ptr<osgViewer::View> view1 = new osgViewer::View;
	view1->setSceneData( root );
	view1->setUpViewInWindow( 50, 50, 400, 400 );

	osg::ref_ptr<osgViewer::View> view2 = new osgViewer::View;
	view2->setSceneData( root );
	view2->setUpViewInWindow( 500, 50, 400, 400 );

	//
	// csin�ljunk egy m�sik sz�nt�rgr�fot is, amiben csak 1 modell van
	//
	osg::ref_ptr<osg::Node> model3 = osgDB::readNodeFile("glider.osg");
	osg::ref_ptr<osgViewer::View> view3 = new osgViewer::View;
	view3->setSceneData( model3 );
	view3->setUpViewInWindow( 250, 470, 400, 400 );

	//
	// v�g�l hozzuk l�tre a t�bbablakos n�z�t �s ind�tsuk el
	//
	osgViewer::CompositeViewer viewer;
	viewer.addView( view1 );
	viewer.addView( view2 );
	viewer.addView( view3 );

	return viewer.run();
}