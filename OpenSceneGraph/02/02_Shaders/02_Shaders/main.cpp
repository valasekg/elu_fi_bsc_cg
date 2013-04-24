#include <osg/Program>
#include <osg/LineWidth>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>

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
    osg::ref_ptr<osg::Node> model = osgDB::readNodeFile( "cow.osg" );
    
    osg::ref_ptr<osg::Shader> vertShader = new osg::Shader( osg::Shader::VERTEX );
    osg::ref_ptr<osg::Shader> fragShader = new osg::Shader( osg::Shader::FRAGMENT );
	vertShader->loadShaderSourceFromFile("myVert.vert");
	fragShader->loadShaderSourceFromFile("myFrag.frag");
    
    osg::ref_ptr<osg::Program> program = new osg::Program;
    program->addShader( vertShader.get() );
    program->addShader( fragShader.get() );
    
    osg::StateSet* stateset = model->getOrCreateStateSet();
    stateset->setAttributeAndModes( program.get() );
    /*stateset->addUniform( 
		new osg::Uniform( "worldViewProj", 
						  osg::Vec4(1.0f, 0.5f, 0.5f, 1.0f)) );
    stateset->addUniform( new osg::Uniform("color2", osg::Vec4(0.5f, 0.2f, 0.2f, 1.0f)) );
    stateset->addUniform( new osg::Uniform("color3", osg::Vec4(0.2f, 0.1f, 0.1f, 1.0f)) );
    stateset->addUniform( new osg::Uniform("color4", osg::Vec4(0.1f, 0.05f, 0.05f, 1.0f)) );
    
	stateset->getOrCreateUniform("color1", osg::Uniform::Type::FLOAT_VEC4)->set( osg::Vec4(0, 1, 1, 1) );*/
	
	osgViewer::Viewer viewer;
	
	viewer.setUpViewInWindow(50, 50, 800, 600 );
    viewer.setSceneData( model.get() );

	//viewer.getCamera()->getViewMatrix();
	//viewer.getCamera()->getProjectionMatrix();

	viewer.realize();

	viewer.setCameraManipulator( new osgGA::TrackballManipulator );

    osg::State* state = viewer.getCamera()->getGraphicsContext()->getState(); 
    state->setUseModelViewAndProjectionUniforms(true); 
    state->setUseVertexAttributeAliasing(true); 

	while ( !viewer.done() )
	{
		viewer.frame();
	}

    return 0;
}

