#include <osg/Program>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osg/MatrixTransform>

#ifdef _DEBUG
	#pragma comment(lib, "osgd.lib")
	#pragma comment(lib, "osgDBd.lib")
#else
	#pragma comment(lib, "osg.lib")
	#pragma comment(lib, "osgDB.lib")
#endif


int main( int argc, char** argv )
{
    osg::ref_ptr<osg::Node> model1 = osgDB::readNodeFile( "cow.osg" );
    osg::ref_ptr<osg::Node> model2 = osgDB::readNodeFile( "cessna.osg" );
    
	// gyoker
	osg::ref_ptr<osg::Group> root = new osg::Group;

	osg::ref_ptr<osg::MatrixTransform> trans1 = new osg::MatrixTransform;
	trans1->setMatrix( osg::Matrix::identity() );
	trans1->addChild( model1 );

	osg::ref_ptr<osg::MatrixTransform> trans2 = new osg::MatrixTransform;
	trans2->setMatrix( osg::Matrix::translate( 5, 0, 0) );
	trans2->addChild( model2 );

	root->addChild( trans1.get() );
	root->addChild( trans2.get() );

	osgDB::writeNodeFile(*(root.get()), "cowcessna.obj");

    return 0;
}

