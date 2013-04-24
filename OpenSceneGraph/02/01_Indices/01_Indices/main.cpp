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
	// poz�ci�k
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
	vertices->push_back( osg::Vec3(-1.0f, 0.0f,-1.0f) );
	vertices->push_back( osg::Vec3( 1.0f, 0.0f,-1.0f) );
	vertices->push_back( osg::Vec3(-1.0f, 0.0f, 1.0f) );
	vertices->push_back( osg::Vec3( 1.0f, 0.0f, 1.0f) );
	vertices->push_back( osg::Vec3( 0.0f, 1.0f, 0.0f) );

	// norm�lisok
	osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
	normals->push_back( osg::Vec3(0.0f,-1.0f, 0.0f) );
	normals->push_back( osg::Vec3(0.0f,-1.0f, 0.0f) );
	normals->push_back( osg::Vec3(0.0f,-1.0f, 0.0f) );
	normals->push_back( osg::Vec3(0.0f,-1.0f, 0.0f) );
	normals->push_back( osg::Vec3(0.0f, 1.0f, 0.0f) );

	// sz�nek
	osg::ref_ptr<osg::Vec3Array> colarray = new osg::Vec3Array;
	colarray->push_back( osg::Vec3(1.0f, 0.0f, 0.0f) );
	colarray->push_back( osg::Vec3(0.0f, 1.0f, 0.0f) );
	colarray->push_back( osg::Vec3(0.0f, 0.0f, 1.0f) );
	colarray->push_back( osg::Vec3(1.0f, 1.0f, 0.0f) );
	colarray->push_back( osg::Vec3(0.0f, 1.0f, 1.0f) );

	// geometria
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
	// azt akarjuk, h VBO-ban t�rolja az OSG a geometri�nkat, ehhez
	// ki kell kapcsolni a display list-es (elavult OGL-es) t�mogat�st ( setUseDisplayList(false) )
	// �s k�l�n meg kell k�rni a VBO haszn�latot ( setUseVertexBufferObjects(true) )
	geom->setUseDisplayList(false);
	geom->setUseVertexBufferObjects(true);
	// a 0. vertex attrib�tumra rakjuk r� a cs�cspontokat
	geom->setVertexAttribBinding(0, osg::Geometry::AttributeBinding::BIND_PER_VERTEX);
	geom->setVertexAttribArray(0, vertices.get() );
	// a 2-es inedx� vertex attrib�tumra tegy�k r� a norm�lisokat
	geom->setVertexAttribBinding(2, osg::Geometry::AttributeBinding::BIND_PER_VERTEX);
	geom->setVertexAttribArray(2, normals.get() );
	// a 3-as inedx� vertex attrib�tumra pedig a sz�neket - vegy�k �szre: ezek az FFP csatorn�knak felelnek meg!
	geom->setVertexAttribBinding(3, osg::Geometry::AttributeBinding::BIND_PER_VERTEX);
	geom->setVertexAttribArray(3, colarray.get() );

	// a piramis alaplapj�nak indexel�se
	osg::ref_ptr<osg::DrawElementsUShort> pyramidIndices = new osg::DrawElementsUShort( GL_TRIANGLES );
	// alaplap - 1. h�romsz�g
	pyramidIndices->push_back(0);
	pyramidIndices->push_back(1);
	pyramidIndices->push_back(2);
	// alaplap - 2. h�romsz�g
	pyramidIndices->push_back(2);
	pyramidIndices->push_back(1);
	pyramidIndices->push_back(3);
	// 1. oldal h�romsz�g
	pyramidIndices->push_back(4);
	pyramidIndices->push_back(1);
	pyramidIndices->push_back(0);

	// kirajzoland� primit�v meghat�roz�sa
	geom->addPrimitiveSet( pyramidIndices.get() );

	// rakjuk be egy geode-ba a piramist, mint kirajzoland� elemet!
	osg::ref_ptr<osg::Geode> root = new osg::Geode;
	root->addDrawable( geom.get() );

	// a h�trafel� n�z� lapokat rajzoljuk ki vonalasan csak
	osg::ref_ptr<osg::PolygonMode> pm = new osg::PolygonMode;
	pm->setMode(	osg::PolygonMode::BACK,
					osg::PolygonMode::LINE);
	root->getOrCreateStateSet()->setAttribute( pm.get() );

	// hozzuk l�tre a viewer-t �s �ll�tsuk be a gy�keret megjelen�tend� adatnak
    osgViewer::Viewer viewer;
    viewer.setSceneData( root.get() );
	
	// a (20,20) kezdeti poz�ci�ba hozzunk l�tre egy 640x480-as ablakot
    viewer.setUpViewInWindow(30, 30, 640, 480); 
    viewer.realize(); 

	// adjuk �t a vez�rl�st a viewer-nek
    return viewer.run();
}