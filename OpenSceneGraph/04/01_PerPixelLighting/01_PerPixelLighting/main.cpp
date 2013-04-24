#include <osg/Program>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osg/MatrixTransform>
#include <osgGA/TrackballManipulator>
#include <osgGA/GUIEventHandler>

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

class CameraController : public osgGA::GUIEventHandler
{
public:
	CameraController( osg::Camera* _camera ) : m_camera(_camera), m_lastPos(0.0f, 0.0f)
	{
		osg::Vec3 eye, center, up;
		m_camera->getViewMatrixAsLookAt( eye, center, up );
	}
    
    virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
    {
        if ( !m_camera) return 
			false;

		osg::Vec3 eye, center, up;
		m_camera->getViewMatrixAsLookAt( eye, center, up );
		up = osg::Vec3(0, 1, 0);	// to fix it

		osg::Vec3 fw = (center - eye);
		fw.normalize(); 
		osg::Vec3 st = fw ^ up;
		st.normalize();

        switch ( ea.getEventType() )
        {
			case osgGA::GUIEventAdapter::KEYDOWN:
				{
					switch ( ea.getKey() )
					{
					case 'a': case 'A':	// to the left
						break;
					case 'd': case 'D':	// to the right
						break;
					case 'w': case 'W':	// forward
						break;
					case 's': case 'S': // backward
						break;
					default:
						break;
					}
				break;
				}
			case osgGA::GUIEventAdapter::PUSH:
				{
					break;
				}
			case osgGA::GUIEventAdapter::DRAG:
				{
					break;
				}
			default:
				break;
			}

		m_camera->setViewMatrixAsLookAt( eye, center, up );

        return false;
    }

protected:
    osg::ref_ptr<osg::Camera>	m_camera;
	osg::Vec2					m_lastPos;
	float						m_u, m_v;
};

int main( int argc, char** argv )
{
    osg::ref_ptr<osg::Node> model1 = osgDB::readNodeFile( "cow.osg" );
    osg::ref_ptr<osg::Node> model2 = osgDB::readNodeFile( "cessna.osg" );
    
    osg::ref_ptr<osg::Shader> vertShader = new osg::Shader( osg::Shader::VERTEX );
    osg::ref_ptr<osg::Shader> fragShader = new osg::Shader( osg::Shader::FRAGMENT );
	vertShader->loadShaderSourceFromFile("myVert.vert");
	fragShader->loadShaderSourceFromFile("myFrag.frag");
    
    osg::ref_ptr<osg::Program> program = new osg::Program;
    program->addShader( vertShader.get() );
    program->addShader( fragShader.get() );
    
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

	// shader
    osg::StateSet* stateset = root->getOrCreateStateSet();
    stateset->setAttributeAndModes( program.get() );

	osg::Uniform* uni_eyePos = stateset->getOrCreateUniform("eye_pos", osg::Uniform::FLOAT_VEC3, 1);
	osg::Uniform* uni_lightDir = stateset->getOrCreateUniform("light_direction", osg::Uniform::FLOAT_VEC3, 1);

	osgViewer::Viewer viewer;
	
	viewer.setUpViewInWindow(50, 50, 800, 600 );
    viewer.setSceneData( root.get() );

	viewer.realize();

	// ask OSG to transfer uniform data and vertex attributes to GLSL shaders
    osg::State* state = viewer.getCamera()->getGraphicsContext()->getState(); 
    state->setUseModelViewAndProjectionUniforms(true); 
    state->setUseVertexAttributeAliasing(true); 

	// custom starting position camera manipulator - beware: it'd overtake the view matrix settings!
	/*osg::ref_ptr<osgGA::TrackballManipulator> track_manip = new osgGA::TrackballManipulator;
	track_manip->setHomePosition( osg::Vec3( 10, 10, 10), osg::Vec3(0, 0, 0), osg::Vec3(0, 1, 0) );
	 
	viewer.setCameraManipulator( track_manip.get() );*/
	viewer.setCameraManipulator( 0 );
	viewer.getCamera()->setAllowEventFocus( false );

	osg::Vec3 eye		= osg::Vec3(10,  0,  0);
	osg::Vec3 center	= osg::Vec3( 0,  0,  0);
	osg::Vec3 up		= osg::Vec3( 0,  1,  0);

	viewer.getCamera()->setViewMatrixAsLookAt( eye, center, up );

	osg::ref_ptr< CameraController > ctrler = new CameraController( viewer.getCamera() );
	viewer.addEventHandler( ctrler.get() );

	while ( !viewer.done() )
	{
		osg::Matrix m = viewer.getCamera()->getViewMatrix();
		eye = osg::Vec3( m(3, 0), m(3, 1), m(3, 2) );
		eye = -eye/m(3, 3);

		uni_eyePos->set( eye );
		
		osg::Vec3 ldir = -eye;
		ldir.normalize();
		uni_lightDir->set( ldir );

		viewer.frame();
	}

    return 0;
}

