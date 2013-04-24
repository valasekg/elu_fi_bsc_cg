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
	vertices->push_back( osg::Vec3(-1.0f, 0.0f,-1.0f) );
	vertices->push_back( osg::Vec3( 1.0f, 0.0f,-1.0f) );
	vertices->push_back( osg::Vec3(-1.0f, 0.0f, 1.0f) );
	vertices->push_back( osg::Vec3( 1.0f, 0.0f, 1.0f) );
	vertices->push_back( osg::Vec3( 0.0f, 1.0f, 0.0f) );

	// normálisok
	osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
	normals->push_back( osg::Vec3(0.0f,-1.0f, 0.0f) );
	normals->push_back( osg::Vec3(0.0f,-1.0f, 0.0f) );
	normals->push_back( osg::Vec3(0.0f,-1.0f, 0.0f) );
	normals->push_back( osg::Vec3(0.0f,-1.0f, 0.0f) );
	normals->push_back( osg::Vec3(0.0f, 1.0f, 0.0f) );

	// színek
	osg::ref_ptr<osg::Vec3Array> colarray = new osg::Vec3Array;
	colarray->push_back( osg::Vec3(1.0f, 0.0f, 0.0f) );
	colarray->push_back( osg::Vec3(0.0f, 1.0f, 0.0f) );
	colarray->push_back( osg::Vec3(0.0f, 0.0f, 1.0f) );
	colarray->push_back( osg::Vec3(1.0f, 1.0f, 0.0f) );
	colarray->push_back( osg::Vec3(0.0f, 1.0f, 1.0f) );

	// geometria
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
	// azt akarjuk, h VBO-ban tárolja az OSG a geometriánkat, ehhez
	// ki kell kapcsolni a display list-es (elavult OGL-es) támogatást ( setUseDisplayList(false) )
	// és külön meg kell kérni a VBO használatot ( setUseVertexBufferObjects(true) )
	geom->setUseDisplayList(false);
	geom->setUseVertexBufferObjects(true);
	// a 0. vertex attribútumra rakjuk rá a csúcspontokat
	geom->setVertexAttribBinding(0, osg::Geometry::AttributeBinding::BIND_PER_VERTEX);
	geom->setVertexAttribArray(0, vertices.get() );
	// a 2-es inedxû vertex attribútumra tegyük rá a normálisokat
	geom->setVertexAttribBinding(2, osg::Geometry::AttributeBinding::BIND_PER_VERTEX);
	geom->setVertexAttribArray(2, normals.get() );
	// a 3-as inedxû vertex attribútumra pedig a színeket - vegyük észre: ezek az FFP csatornáknak felelnek meg!
	geom->setVertexAttribBinding(3, osg::Geometry::AttributeBinding::BIND_PER_VERTEX);
	geom->setVertexAttribArray(3, colarray.get() );

	// a piramis alaplapjának indexelése
	osg::ref_ptr<osg::DrawElementsUShort> pyramidIndices = new osg::DrawElementsUShort( GL_TRIANGLES );
	// alaplap - 1. háromszög
	pyramidIndices->push_back(0);
	pyramidIndices->push_back(1);
	pyramidIndices->push_back(2);
	// alaplap - 2. háromszög
	pyramidIndices->push_back(2);
	pyramidIndices->push_back(1);
	pyramidIndices->push_back(3);
	// 1. oldal háromszög
	pyramidIndices->push_back(4);
	pyramidIndices->push_back(1);
	pyramidIndices->push_back(0);

	// kirajzolandó primitív meghatározása
	geom->addPrimitiveSet( pyramidIndices.get() );

	// rakjuk be egy geode-ba a piramist, mint kirajzolandó elemet!
	osg::ref_ptr<osg::Geode> root = new osg::Geode;
	root->addDrawable( geom.get() );

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