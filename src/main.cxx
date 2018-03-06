#include "../Common/Common.h"

using json = nlohmann::json;
using namespace std;

bool CreateScene(FbxManager* pSdkManager, FbxScene* pScene, json& j);

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		cout << "usage: " << argv[0] << " <json input> <fbx output> <format>\n";
		return 0;
	}
	
	int asciiFormat = 0;
	if (argc == 4 && argv[3] != "")
		asciiFormat = -1;

	// read the JSON file
	std::ifstream i(argv[1]);
	json j;
	i >> j;

    FbxManager* lSdkManager = NULL;
    FbxScene* lScene = NULL;

    // Prepare the FBX SDK.
    InitializeSdkObjects(lSdkManager, lScene);

    // Create the scene.
    if( !CreateScene(lSdkManager, lScene, j) )
    {
        FBXSDK_printf("\n\nAn error occurred while creating the scene...\n");
        DestroySdkObjects(lSdkManager, false);
        return 0;
    }

	const char* lSampleFileName = argv[2];
	SaveScene(lSdkManager, lScene, lSampleFileName, asciiFormat);
    // Destroy all objects created by the FBX SDK.
    DestroySdkObjects(lSdkManager, true);

    return 0;
}

bool CreateScene(FbxManager* pSdkManager, FbxScene* pScene, json& j)
{
    FbxTime lTime;
    FbxAnimCurveKey key;
    FbxAnimCurve* lCurve = NULL;

	// create markers
	FbxNode* lMarkerPos = CreateMarker(pScene, "PositionAnimation");
	FbxNode* lMarkerRot = CreateMarker(pScene, "RotationAnimation");

	// set the marker position
	SetMarkerDefaultPosition(lMarkerPos);
	SetMarkerDefaultPosition(lMarkerRot);

	// create a mesh
	FbxNode* lMesh = CreatePyramidWithMaterials(pScene, "Mesh");

	// create a camera
	FbxNode* lCamera = CreateCamera(pScene, "Camera");

    // Create one animation stack
    FbxAnimStack* lAnimStack = FbxAnimStack::Create(pScene, "Stack001");

    // this stack animation range is limited
    lAnimStack->LocalStop = FBXSDK_TIME_ONE_HOUR;
    lAnimStack->Description = "This is the animation stack description field.";

    // all animation stacks need, at least, one layer.
    FbxAnimLayer* lAnimLayer = FbxAnimLayer::Create(pScene, "Base Layer");	// the AnimLayer object name is "Base Layer"
    lAnimStack->AddMember(lAnimLayer);											// add the layer to the stack
			
	// set the camera position
	SetCameraDefaultPosition(lCamera);

	SetMeshDefaultPosition(lMesh, FbxVector4(0, 0, -100));

	// animate the camera
	AnimatePosition(lMarkerPos, lAnimLayer, j);
	AnimateRotation(lMarkerRot, lAnimLayer, j);

	// build a minimum scene graph
	FbxNode* lRootNode = pScene->GetRootNode();
	lRootNode->AddChild(lMarkerPos);
	lMarkerPos->AddChild(lMarkerRot);
	lMarkerRot->AddChild(lCamera);
	lMarkerRot->AddChild(lMesh);

	// set camera switcher as the default camera
	pScene->GetGlobalSettings().SetDefaultCamera((char *)lCamera->GetName());

    return true;
}

// Create a camera.
FbxNode* CreateCamera(FbxScene* pScene, char* pName)
{
	FbxCamera* lCamera = FbxCamera::Create(pScene, pName);

	// Set camera property for a classic TV projection with aspect ratio 4:3
	lCamera->SetFormat(FbxCamera::eHD);

	FbxNode* lNode = FbxNode::Create(pScene, pName);

	lNode->SetNodeAttribute(lCamera);

	return lNode;
}


// Compute the camera position.
void SetCameraDefaultPosition(FbxNode* pCamera)
{
	// set the initial camera position
	FbxVector4 lCameraLocation(0.0, 0.0, 0.0);
	pCamera->LclTranslation.Set(lCameraLocation);
	pCamera->LclRotation.Set(FbxVector4(0,90,0));
	pCamera->LclScaling.Set(FbxVector4(100.0, 100.0, 100.0));
}

// Compute the camera position.
void SetMeshDefaultPosition(FbxNode* pMesh, FbxVector4& location)
{
	pMesh->LclTranslation.Set(location);
	pMesh->LclRotation.Set(FbxVector4(90,0,0));
	pMesh->LclScaling.Set(FbxVector4(1.0, 1.0, 1.0));
}

// Position animation
void AnimatePosition(FbxNode* pPosition, FbxAnimLayer* pAnimLayer, json& j)
{
	FbxAnimCurve* lCurve = NULL;
	FbxTime lTime;
	int lKeyIndex = 0;

	pPosition->LclTranslation.GetCurveNode(pAnimLayer, true);

	//get first timestamp
	FbxLongLong lStartTimestamp = 0;

	// iterate the array
	for (json::iterator it = j["camera"]["poses"].begin(); it != j["camera"]["poses"].end(); ++it) {
		if (lStartTimestamp == 0)
			lStartTimestamp = (*it)["timestamp"];

		// X translation.
		float xValue = float((*it)["position"]["x"]) * float(100);
		lCurve = pPosition->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X, true);
		if (lCurve)
		{
			lCurve->KeyModifyBegin();

			lTime.SetMilliSeconds(FbxLongLong((*it)["timestamp"]) - lStartTimestamp);
			lKeyIndex = lCurve->KeyAdd(lTime);
			lCurve->KeySet(lKeyIndex, lTime, xValue, FbxAnimCurveDef::eInterpolationLinear);

			lCurve->KeyModifyEnd();
		}

		// Y translation.
		float yValue = float((*it)["position"]["y"]) * float(100);
		lCurve = pPosition->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y, true);
		if (lCurve)
		{
			lCurve->KeyModifyBegin();

			lTime.SetMilliSeconds(FbxLongLong((*it)["timestamp"]) - lStartTimestamp);
			lKeyIndex = lCurve->KeyAdd(lTime);
			lCurve->KeySet(lKeyIndex, lTime, yValue, FbxAnimCurveDef::eInterpolationLinear);

			lCurve->KeyModifyEnd();
		}

		// Z translation.
		float zValue = float((*it)["position"]["z"]) * float(100);
		lCurve = pPosition->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z, true);
		if (lCurve)
		{
			lCurve->KeyModifyBegin();

			lTime.SetMilliSeconds(FbxLongLong((*it)["timestamp"]) - lStartTimestamp);
			lKeyIndex = lCurve->KeyAdd(lTime);
			lCurve->KeySet(lKeyIndex, lTime, zValue, FbxAnimCurveDef::eInterpolationLinear);

			lCurve->KeyModifyEnd();
		}
	}
}

void AnimateRotation(FbxNode* pRotation, FbxAnimLayer* pAnimLayer, json& j)
{
	FbxAnimCurve* lCurve = NULL;
	FbxTime lTime;
	int lKeyIndex = 0;

	pRotation->LclRotation.GetCurveNode(pAnimLayer, true);

	//get first timestamp
	FbxLongLong lStartTimestamp = 0;

	// iterate the array
	for (json::iterator it = j["camera"]["poses"].begin(); it != j["camera"]["poses"].end(); ++it) {
		if (lStartTimestamp == 0)
			lStartTimestamp = (*it)["timestamp"];

		// X rotation.
		float xValue = float((*it)["rotation"]["x"]);
		lCurve = pRotation->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X, true);
		if (lCurve)
		{
			lCurve->KeyModifyBegin();

			lTime.SetMilliSeconds(FbxLongLong((*it)["timestamp"]) - lStartTimestamp);
			lKeyIndex = lCurve->KeyAdd(lTime);
			lCurve->KeySet(lKeyIndex, lTime, xValue, FbxAnimCurveDef::eInterpolationLinear);

			lCurve->KeyModifyEnd();
		}

		// Y rotation.
		float yValue = float((*it)["rotation"]["y"]);
		lCurve = pRotation->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y, true);
		if (lCurve)
		{
			lCurve->KeyModifyBegin();

			lTime.SetMilliSeconds(FbxLongLong((*it)["timestamp"]) - lStartTimestamp);
			lKeyIndex = lCurve->KeyAdd(lTime);
			lCurve->KeySet(lKeyIndex, lTime, yValue, FbxAnimCurveDef::eInterpolationLinear);

			lCurve->KeyModifyEnd();
		}

		// Z rotation.
		float zValue = float((*it)["rotation"]["z"]);
		lCurve = pRotation->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z, true);
		if (lCurve)
		{
			lCurve->KeyModifyBegin();

			lTime.SetMilliSeconds(FbxLongLong((*it)["timestamp"]) - lStartTimestamp);
			lKeyIndex = lCurve->KeyAdd(lTime);
			lCurve->KeySet(lKeyIndex, lTime, zValue, FbxAnimCurveDef::eInterpolationLinear);

			lCurve->KeyModifyEnd();
		}
	}
}

// Create materials for pyramid.
void CreateMaterials(FbxScene* pScene, FbxMesh* pMesh)
{
	int i;

	for (i = 0; i < 5; i++)
	{
		FbxString lMaterialName = "material";
		FbxString lShadingName = "Phong";
		lMaterialName += i;
		FbxDouble3 lBlack(0.0, 0.0, 0.0);
		FbxDouble3 lRed(1.0, 0.0, 0.0);
		FbxDouble3 lColor;
		FbxSurfacePhong *lMaterial = FbxSurfacePhong::Create(pScene, lMaterialName.Buffer());


		// Generate primary and secondary colors.
		lMaterial->Emissive.Set(lBlack);
		lMaterial->Ambient.Set(lRed);
		lColor = FbxDouble3(i > 2 ? 1.0 : 0.0,
			i > 0 && i < 4 ? 1.0 : 0.0,
			i % 2 ? 0.0 : 1.0);
		lMaterial->Diffuse.Set(lColor);
		lMaterial->TransparencyFactor.Set(0.0);
		lMaterial->ShadingModel.Set(lShadingName);
		lMaterial->Shininess.Set(0.5);

		//get the node of mesh, add material for it.
		FbxNode* lNode = pMesh->GetNode();
		if (lNode)
			lNode->AddMaterial(lMaterial);
	}
}

// Create a pyramid with materials.
FbxNode* CreatePyramidWithMaterials(FbxScene* pScene, char* pName)
{
	int i, j;
	FbxMesh* lMesh = FbxMesh::Create(pScene, pName);

	FbxVector4 vertex0(-50, 0, 50);
	FbxVector4 vertex1(50, 0, 50);
	FbxVector4 vertex2(50, 0, -50);
	FbxVector4 vertex3(-50, 0, -50);
	FbxVector4 vertex4(0, 100, 0);

	FbxVector4 lNormalP0(0, 1, 0);
	FbxVector4 lNormalP1(0, 0.447, 0.894);
	FbxVector4 lNormalP2(0.894, 0.447, 0);
	FbxVector4 lNormalP3(0, 0.447, -0.894);
	FbxVector4 lNormalP4(-0.894, 0.447, 0);

	// Create control points.
	lMesh->InitControlPoints(16);
	FbxVector4* lControlPoints = lMesh->GetControlPoints();

	lControlPoints[0] = vertex0;
	lControlPoints[1] = vertex1;
	lControlPoints[2] = vertex2;
	lControlPoints[3] = vertex3;
	lControlPoints[4] = vertex0;
	lControlPoints[5] = vertex1;
	lControlPoints[6] = vertex4;
	lControlPoints[7] = vertex1;
	lControlPoints[8] = vertex2;
	lControlPoints[9] = vertex4;
	lControlPoints[10] = vertex2;
	lControlPoints[11] = vertex3;
	lControlPoints[12] = vertex4;
	lControlPoints[13] = vertex3;
	lControlPoints[14] = vertex0;
	lControlPoints[15] = vertex4;

	// specify normals per control point.

	FbxGeometryElementNormal* lNormalElement = lMesh->CreateElementNormal();
	lNormalElement->SetMappingMode(FbxGeometryElement::eByControlPoint);
	lNormalElement->SetReferenceMode(FbxGeometryElement::eDirect);

	lNormalElement->GetDirectArray().Add(lNormalP0);
	lNormalElement->GetDirectArray().Add(lNormalP0);
	lNormalElement->GetDirectArray().Add(lNormalP0);
	lNormalElement->GetDirectArray().Add(lNormalP0);
	lNormalElement->GetDirectArray().Add(lNormalP1);
	lNormalElement->GetDirectArray().Add(lNormalP1);
	lNormalElement->GetDirectArray().Add(lNormalP1);
	lNormalElement->GetDirectArray().Add(lNormalP2);
	lNormalElement->GetDirectArray().Add(lNormalP2);
	lNormalElement->GetDirectArray().Add(lNormalP2);
	lNormalElement->GetDirectArray().Add(lNormalP3);
	lNormalElement->GetDirectArray().Add(lNormalP3);
	lNormalElement->GetDirectArray().Add(lNormalP3);
	lNormalElement->GetDirectArray().Add(lNormalP4);
	lNormalElement->GetDirectArray().Add(lNormalP4);
	lNormalElement->GetDirectArray().Add(lNormalP4);

	// Array of polygon vertices.
	int lPolygonVertices[] = { 0, 3, 2, 1,
		4, 5, 6,
		7, 8, 9,
		10, 11, 12,
		13, 14, 15 };

	// Set material mapping.
	FbxGeometryElementMaterial* lMaterialElement = lMesh->CreateElementMaterial();
	lMaterialElement->SetMappingMode(FbxGeometryElement::eByPolygon);
	lMaterialElement->SetReferenceMode(FbxGeometryElement::eIndexToDirect);

	// Create polygons. Assign material indices.

	// Pyramid base.
	lMesh->BeginPolygon(0); // Material index.

	for (j = 0; j < 4; j++)
	{
		lMesh->AddPolygon(lPolygonVertices[j]); // Control point index.
	}

	lMesh->EndPolygon();

	// Pyramid sides.
	for (i = 1; i < 5; i++)
	{
		lMesh->BeginPolygon(i); // Material index.

		for (j = 0; j < 3; j++)
		{
			lMesh->AddPolygon(lPolygonVertices[4 + 3 * (i - 1) + j]); // Control point index.
		}

		lMesh->EndPolygon();
	}


	FbxNode* lNode = FbxNode::Create(pScene, pName);

	lNode->SetNodeAttribute(lMesh);

	CreateMaterials(pScene, lMesh);

	return lNode;
}

// Create a marker to use a point of interest for the camera. 
FbxNode* CreateMarker(FbxScene* pScene, char* pName)
{
	FbxMarker* lMarker = FbxMarker::Create(pScene, pName);

	FbxNode* lNode = FbxNode::Create(pScene, pName);

	lNode->SetNodeAttribute(lMarker);

	return lNode;
}

// Set marker default position.
void SetMarkerDefaultPosition(FbxNode* pMarker)
{
	// The marker is positioned above the origin. There is no rotation and no scaling.
	pMarker->LclTranslation.Set(FbxVector4(0.0, 0.0, 0.0));
	pMarker->LclRotation.Set(FbxVector4(0.0, 0.0, 0.0));
	pMarker->LclScaling.Set(FbxVector4(1.0, 1.0, 1.0));
}
