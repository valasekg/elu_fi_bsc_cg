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
	// poz�ci�k
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;

	// a piramis �t cs�cspontja
	osg::Vec3 pa(-1.0f, 0.0f,-1.0f); osg::Vec3 ca(1.0f, 0.0f, 0.0f);
	osg::Vec3 pb( 1.0f, 0.0f,-1.0f); osg::Vec3 cb(0.0f, 1.0f, 0.0f);
	osg::Vec3 pc(-1.0f, 0.0f, 1.0f); osg::Vec3 cc(0.0f, 0.0f, 1.0f);
	osg::Vec3 pd( 1.0f, 0.0f, 1.0f); osg::Vec3 cd(1.0f, 1.0f, 0.0f);
	osg::Vec3 pe( 0.0f, 1.0f, 0.0f); osg::Vec3 ce(1.0f, 0.0f, 1.0f);

	// alaplap - 1. h�romsz�g
	vertices->push_back( pa );
	vertices->push_back( pb );
	vertices->push_back( pc );
	// alaplap - 2. h�romsz�g
	vertices->push_back( pc );
	vertices->push_back( pb );
	vertices->push_back( pd );

	// norm�lisok
	osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
	// alaplap - 1. h�romsz�g norm�lisai
	normals->push_back( osg::Vec3(0.0f,-1.0f, 0.0f) );
	normals->push_back( osg::Vec3(0.0f,-1.0f, 0.0f) );
	normals->push_back( osg::Vec3(0.0f,-1.0f, 0.0f) );
	// alaplap - 2. h�romsz�g norm�lisai
	normals->push_back( osg::Vec3(0.0f,-1.0f, 0.0f) );
	normals->push_back( osg::Vec3(0.0f,-1.0f, 0.0f) );
	normals->push_back( osg::Vec3(0.0f,-1.0f, 0.0f) );

	// sz�nek
	osg::ref_ptr<osg::Vec3Array> colarray = new osg::Vec3Array;
	// alaplap - 1. h�romsz�g sz�nei
	colarray->push_back( ca );
	colarray->push_back( cb );
	colarray->push_back( cc );
	// alaplap - 2. h�romsz�g sz�nei
	colarray->push_back( cc );
	colarray->push_back( cb );
	colarray->push_back( cd );

	// geometria
	osg::ref_ptr<osg::Geometry> quad = new osg::Geometry;
	// azt akarjuk, h VBO-ban t�rolja az OSG a geometri�nkat, ehhez
	// ki kell kapcsolni a display list-es (elavult OGL-es) t�mogat�st ( setUseDisplayList(false) )
	// �s k�l�n meg kell k�rni a VBO haszn�latot ( setUseVertexBufferObjects(true) )
	quad->setUseDisplayList(false);
	quad->setUseVertexBufferObjects(true);
	// a 0. vertex attrib�tumra rakjuk r� a cs�cspontokat
	quad->setVertexAttribBinding(0, osg::Geometry::AttributeBinding::BIND_PER_VERTEX);
	quad->setVertexAttribArray(0, vertices.get() );
	// a 2-es inedx� vertex attrib�tumra tegy�k r� a norm�lisokat
	quad->setVertexAttribBinding(2, osg::Geometry::AttributeBinding::BIND_PER_VERTEX);
	quad->setVertexAttribArray(2, normals.get() );
	// a 3-as inedx� vertex attrib�tumra pedig a sz�neket - vegy�k �szre: ezek az FFP csatorn�knak felelnek meg!
	quad->setVertexAttribBinding(3, osg::Geometry::AttributeBinding::BIND_PER_VERTEX);
	quad->setVertexAttribArray(3, colarray.get() );

	// kirajzoland� primit�v meghat�roz�sa
	quad->addPrimitiveSet( new osg::DrawArrays(GL_TRIANGLES, 0, 6) );

	// rakjuk be egy geode-ba a piramist, mint kirajzoland� elemet!
	osg::ref_ptr<osg::Geode> root = new osg::Geode;
	root->addDrawable( quad.get() );

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