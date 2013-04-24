#include <osg/Texture2D>
#include <osg/Geometry>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osg/PolygonMode>

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
	// pozíciók
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;

	// a piramis öt csúcspontja
	osg::Vec3 pa(-1.0f, 0.0f,-1.0f); osg::Vec3 ca(1.0f, 0.0f, 0.0f);
	osg::Vec3 pb( 1.0f, 0.0f,-1.0f); osg::Vec3 cb(0.0f, 1.0f, 0.0f);
	osg::Vec3 pc(-1.0f, 0.0f, 1.0f); osg::Vec3 cc(0.0f, 0.0f, 1.0f);
	osg::Vec3 pd( 1.0f, 0.0f, 1.0f); osg::Vec3 cd(1.0f, 1.0f, 0.0f);
	osg::Vec3 pe( 0.0f, 1.0f, 0.0f); osg::Vec3 ce(1.0f, 0.0f, 1.0f);

	// alaplap - 1. háromszög
	vertices->push_back( pa );
	vertices->push_back( pb );
	vertices->push_back( pc );
	// alaplap - 2. háromszög
	vertices->push_back( pc );
	vertices->push_back( pb );
	vertices->push_back( pd );

	// normálisok
	osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
	// alaplap - 1. háromszög normálisai
	normals->push_back( osg::Vec3(0.0f,-1.0f, 0.0f) );
	normals->push_back( osg::Vec3(0.0f,-1.0f, 0.0f) );
	normals->push_back( osg::Vec3(0.0f,-1.0f, 0.0f) );
	// alaplap - 2. háromszög normálisai
	normals->push_back( osg::Vec3(0.0f,-1.0f, 0.0f) );
	normals->push_back( osg::Vec3(0.0f,-1.0f, 0.0f) );
	normals->push_back( osg::Vec3(0.0f,-1.0f, 0.0f) );

	// színek
	osg::ref_ptr<osg::Vec3Array> colarray = new osg::Vec3Array;
	// alaplap - 1. háromszög színei
	colarray->push_back( ca );
	colarray->push_back( cb );
	colarray->push_back( cc );
	// alaplap - 2. háromszög színei
	colarray->push_back( cc );
	colarray->push_back( cb );
	colarray->push_back( cd );

	// geometria
	osg::ref_ptr<osg::Geometry> quad = new osg::Geometry;
	// azt akarjuk, h VBO-ban tárolja az OSG a geometriánkat, ehhez
	// ki kell kapcsolni a display list-es (elavult OGL-es) támogatást ( setUseDisplayList(false) )
	// és külön meg kell kérni a VBO használatot ( setUseVertexBufferObjects(true) )
	quad->setUseDisplayList(false);
	quad->setUseVertexBufferObjects(true);
	// a 0. vertex attribútumra rakjuk rá a csúcspontokat
	quad->setVertexAttribBinding(0, osg::Geometry::AttributeBinding::BIND_PER_VERTEX);
	quad->setVertexAttribArray(0, vertices.get() );
	// a 2-es inedxû vertex attribútumra tegyük rá a normálisokat
	quad->setVertexAttribBinding(2, osg::Geometry::AttributeBinding::BIND_PER_VERTEX);
	quad->setVertexAttribArray(2, normals.get() );
	// a 3-as inedxû vertex attribútumra pedig a színeket - vegyük észre: ezek az FFP csatornáknak felelnek meg!
	quad->setVertexAttribBinding(3, osg::Geometry::AttributeBinding::BIND_PER_VERTEX);
	quad->setVertexAttribArray(3, colarray.get() );

	// kirajzolandó primitív meghatározása
	quad->addPrimitiveSet( new osg::DrawArrays(GL_TRIANGLES, 0, 6) );

	// rakjuk be egy geode-ba a piramist, mint kirajzolandó elemet!
	osg::ref_ptr<osg::Geode> root = new osg::Geode;
	root->addDrawable( quad.get() );

	// a hátrafelé nézõ lapokat rajzoljuk ki vonalasan csak
	osg::ref_ptr<osg::PolygonMode> pm = new osg::PolygonMode;
	pm->setMode(	osg::PolygonMode::BACK,
					osg::PolygonMode::LINE);
	root->getOrCreateStateSet()->setAttribute( pm.get() );

	// hozzuk létre a viewer-t és állítsuk be a gyökeret megjelenítendõ adatnak
    osgViewer::Viewer viewer;
    viewer.setSceneData( root.get() );
	
	// a (20,20) kezdeti pozícióba hozzunk létre egy 640x480-as ablakot
    viewer.setUpViewInWindow(30, 30, 640, 480); 
    viewer.realize(); 

	// adjuk át a vezérlést a viewer-nek
    return viewer.run();
}