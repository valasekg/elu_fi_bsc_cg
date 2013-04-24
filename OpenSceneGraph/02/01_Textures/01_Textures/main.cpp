#include <osg/Texture2D>
#include <osg/Geometry>
#include <osgDB/ReadFile>
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

#pragma warning(disable : 4482 )

int main( int argc, char** argv )
{
	// poz�ci�k
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
	vertices->push_back( osg::Vec3(-1.0f, 0.0f,-1.0f) );
	vertices->push_back( osg::Vec3( 1.0f, 0.0f,-1.0f) );
	vertices->push_back( osg::Vec3(-1.0f, 0.0f, 1.0f) );
	vertices->push_back( osg::Vec3( 1.0f, 0.0f, 1.0f) );

	// norm�lisok
	osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
	normals->push_back( osg::Vec3(0.0f,-1.0f, 0.0f) );
	normals->push_back( osg::Vec3(0.0f,-1.0f, 0.0f) );
	normals->push_back( osg::Vec3(0.0f,-1.0f, 0.0f) );
	normals->push_back( osg::Vec3(0.0f,-1.0f, 0.0f) );

	// text�rakoordin�t�k
	osg::ref_ptr<osg::Vec2Array> texcoords = new osg::Vec2Array;
	texcoords->push_back( osg::Vec2(0.0f, 0.0f) );
	texcoords->push_back( osg::Vec2(3.0f, 0.0f) );
	texcoords->push_back( osg::Vec2(0.0f, 3.0f) );
	texcoords->push_back( osg::Vec2(3.0f, 3.0f) );

	// n�gysz�g geometria
	osg::ref_ptr<osg::Geometry> quad = new osg::Geometry;
	quad->setUseVertexBufferObjects(true);

	// �ll�tsuk be, hogy a VBO-ba milyen adatok ker�ljenek
	quad->setVertexArray( vertices.get() );
	quad->setNormalArray( normals.get() );
	quad->setNormalBinding( osg::Geometry::BIND_PER_VERTEX );
	quad->setTexCoordArray( 0, texcoords.get() );
	// kirajzoland� primit�v meghat�roz�sa
	quad->addPrimitiveSet( new osg::DrawArrays(GL_TRIANGLE_STRIP, 0, 4) );

	// text�ra bet�lt�se
	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
	osg::ref_ptr<osg::Image> image = osgDB::readImageFile( "Images/land_shallow_topo_2048.jpg" );
	texture->setImage( image.get() );
	texture->setFilter( osg::Texture::FilterParameter::MIN_FILTER, osg::Texture::FilterMode::LINEAR_MIPMAP_LINEAR );
	texture->setFilter( osg::Texture::FilterParameter::MAG_FILTER, osg::Texture::FilterMode::LINEAR );
	texture->setWrap( osg::Texture::WRAP_S, osg::Texture::WrapMode::REPEAT );
	texture->setWrap( osg::Texture::WRAP_T, osg::Texture::WrapMode::REPEAT );

	// rakjuk be egy geode-ba a quad-ot, mint kirajzoland� elemet!
	osg::ref_ptr<osg::Geode> root = new osg::Geode;
	root->addDrawable( quad.get() );

	// 0-�s mintav�telez�re rakjuk r� a text�r�t
	root->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture.get() );
    osg::StateSet* state = root->getOrCreateStateSet();
    state->setMode( GL_LIGHTING,
        osg::StateAttribute::OFF |
        osg::StateAttribute::PROTECTED );

	// hozzuk l�tre a viewer-t �s �ll�tsuk be a gy�keret megjelen�tend� adatnak
    osgViewer::Viewer viewer;
    viewer.setSceneData( root.get() );
	
	// a (20,20) kezdeti poz�ci�ba hozzunk l�tre egy 640x480-as ablakot
    viewer.setUpViewInWindow(20, 20, 640, 480); 
    viewer.realize(); 

	// adjuk �t a vez�rl�st a viewer-nek
    return viewer.run();
}