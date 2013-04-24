#include <osg/Program>
#include <osg/LineWidth>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>
#include <osg/MatrixTransform>
#include <osg/Switch>
#include <osg/AnimationPath>

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

class SwitchingCallback : public osg::NodeCallback
{
public:
    SwitchingCallback() : m_lastTime(0) {}
    
    virtual void operator()( osg::Node* node, osg::NodeVisitor* nv )
    {
        osg::Switch* switchNode = static_cast<osg::Switch*>( node );
		double nowTime = nv->getFrameStamp()->getReferenceTime();

		if ( abs( nowTime - m_lastTime ) > 2 )
        {
            switchNode->setValue( 0, !switchNode->getValue(0) );
            switchNode->setValue( 1, !switchNode->getValue(1) );
			m_lastTime = nowTime;
        }
        traverse( node, nv );
    }
    
protected:
    double m_lastTime;
};

osg::AnimationPath* createAnimationPath( float radius, float time )
{
    osg::ref_ptr<osg::AnimationPath> path = new osg::AnimationPath;
    path->setLoopMode( osg::AnimationPath::LOOP );
    
    unsigned int numSamples = 32;
    float delta_yaw = 2.0f * osg::PI/((float)numSamples - 1.0f);
    float delta_time = time / (float)numSamples;
    for ( unsigned int i=0; i<numSamples; ++i )
    {
        float yaw = delta_yaw * (float)i;
        osg::Vec3 pos( sinf(yaw)*radius, cosf(yaw)*radius, 0.0f );
        osg::Quat rot1( -yaw, osg::Z_AXIS );
		osg::Quat rot2( -yaw, osg::X_AXIS );
        path->insert( delta_time * (float)i, osg::AnimationPath::ControlPoint(pos, rot2*rot1) );
    }
    return path.release();    
}

class UpdateLightnessCallback : public osg::Uniform::Callback
{
public:
	UpdateLightnessCallback()
	{
		t = 0;
	}
    virtual void operator() ( osg::Uniform* _uniform, osg::NodeVisitor* _nodeVisitor )
    {
        t = 0.5f + sinf(2*3.1415f*_nodeVisitor->getFrameStamp()->getReferenceTime()/4);
        _uniform->set(t);
    }
private:
	float t;
};

int main( int argc, char** argv )
{
    osg::ref_ptr<osg::Node> model = osgDB::readNodeFile( "cow.osg" );
	osg::ref_ptr<osg::Node> model2 = osgDB::readNodeFile( "cessna.osg" );
    
    osg::ref_ptr<osg::Shader> vertShader = new osg::Shader( osg::Shader::VERTEX );
    osg::ref_ptr<osg::Shader> fragShader = new osg::Shader( osg::Shader::FRAGMENT );
	vertShader->loadShaderSourceFromFile("myVert.vert");
	fragShader->loadShaderSourceFromFile("myFrag.frag");
    
    osg::ref_ptr<osg::Program> program = new osg::Program;
    program->addShader( vertShader.get() );
    program->addShader( fragShader.get() );

    osg::ref_ptr<osg::Switch> n_switch = new osg::Switch;
    n_switch->addChild( model.get(), false );
    n_switch->addChild( model2.get(), true );
    n_switch->setUpdateCallback( new SwitchingCallback );

    osg::StateSet* stateset = n_switch->getOrCreateStateSet();
    stateset->setAttributeAndModes( program.get() );
	osg::Uniform *uni_lightness = stateset->getOrCreateUniform( "lightness", osg::Uniform::FLOAT, 1 );
	/*

	//	vagy másképp:

	osg::Uniform* uni_lightness = new osg::Uniform( osg::Uniform::FLOAT, // uniform típusa
													"lightness",		 // neve
													1);					 // elemszáma
	stateset->addUniform( uni_lightness );
	*/

	uni_lightness->setUpdateCallback(new UpdateLightnessCallback() );

	// anim path
    osg::ref_ptr<osg::MatrixTransform> root = new osg::MatrixTransform;
    root->addChild( n_switch.get() );

    osg::ref_ptr<osg::AnimationPathCallback> apcb = new osg::AnimationPathCallback;
    apcb->setAnimationPath( createAnimationPath(10.0f, 6.0f) );
    root->setUpdateCallback( apcb.get() );

	// viewer
	
	osgViewer::Viewer viewer;
	
	viewer.setUpViewInWindow(50, 50, 800, 600 );
    viewer.setSceneData( root.get() );

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

