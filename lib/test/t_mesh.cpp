#include <gtest/gtest.h>
#include <cstdio>
#include "meshRoutines.h"
#include "meshData.h"

TEST(Mesh, ConstructDestruct)
{
    ASSERT_NO_THROW
    (
        VTKMeshRoutines* vM = new VTKMeshRoutines();
        delete vM;
    );
}

TEST(Mesh, OpenOBJ)
{
    VTKMeshData* vM = new VTKMeshData();

    vtkSmartPointer<vtkPolyData> mesh = vM->importObjFile( "lib/test/data/torus.obj" );

    vtkIdType numberOfVertices = mesh->GetPoints()->GetNumberOfPoints();
    vtkIdType numberOfFaces = mesh->GetNumberOfCells();

    ASSERT_EQ( numberOfVertices, 3456 );
    ASSERT_EQ( numberOfFaces, 1152 );

    delete vM;
}

TEST(Mesh, ReduceMesh)
{
    VTKMeshData* vM = new VTKMeshData();
    VTKMeshRoutines* vR = new VTKMeshRoutines();

    vtkSmartPointer<vtkPolyData> mesh = vM->importObjFile( "lib/test/data/torus.obj" );
    vtkIdType numberOfVertices = mesh->GetPoints()->GetNumberOfPoints();

    for(int k = 0; k < 10; k++ )
    {
        vR->meshReduction(mesh, 0.1);
        vtkIdType newNumberOfVertices = mesh->GetPoints()->GetNumberOfPoints();
        ASSERT_LT( newNumberOfVertices, numberOfVertices );
        numberOfVertices = newNumberOfVertices;
    }

    delete vM;
    delete vR;
}

TEST(Mesh, CenterObject)
{
    VTKMeshData* vM = new VTKMeshData();
    VTKMeshRoutines* vR = new VTKMeshRoutines();

    vtkSmartPointer<vtkPolyData> mesh = vM->importObjFile( "lib/test/data/torus.obj" );
    vtkVector3d trans = vR->moveMeshToCOSCenter(mesh);

    ASSERT_NEAR(trans.GetX(), -1.0, 0.1);
    ASSERT_NEAR(trans.GetY(), -3.0, 0.1);
    ASSERT_NEAR(trans.GetZ(), +2.0, 0.1);

    delete vM;
    delete vR;
}

TEST(Mesh, TestSaveOpenFormats)
{
    VTKMeshData* vM = new VTKMeshData();

    // open obj file
    vtkSmartPointer<vtkPolyData> mesh = vM->importObjFile( "lib/test/data/torus.obj" );
    ASSERT_GT( mesh->GetPoints()->GetNumberOfPoints(), 0 );

    // export several different 3d formats
    vM->exportAsObjFile(mesh, "objExport.obj");
    vM->exportAsPlyFile(mesh, "plyExport.ply");
    vM->exportAsStlFile(mesh, "stlExport.stl", false);
    vM->exportAsStlFile(mesh, "stlBinaryExport.stl", true);

    // import the different format
    ASSERT_GT( vM->importObjFile("objExport.obj")->GetPoints()->GetNumberOfPoints(), 0);
    ASSERT_GT( vM->importPlyFile("plyExport.ply")->GetPoints()->GetNumberOfPoints(), 0);
    ASSERT_GT( vM->importStlFile("stlExport.stl")->GetPoints()->GetNumberOfPoints(), 0);
    ASSERT_GT( vM->importStlFile("stlBinaryExport.stl")->GetPoints()->GetNumberOfPoints(), 0);

    remove("objExport.obj");
    remove("plyExport.ply");
    remove("stlExport.stl");
    remove("stlBinaryExport.stl");

    delete vM;
}
