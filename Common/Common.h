/****************************************************************************************

   Copyright (C) 2015 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

#define DEBUG

#ifndef _COMMON_H
#define _COMMON_H

#include <fbxsdk.h>
#include "nlohmann/json.hpp"
#include<iostream>
#include<fstream>

void InitializeSdkObjects(FbxManager*& pManager, FbxScene*& pScene);
void DestroySdkObjects(FbxManager* pManager, bool pExitStatus);

bool SaveScene(FbxManager* pManager, FbxDocument* pScene, const char* pFilename, int pFileFormat=-1, bool pEmbedMedia=false);
bool LoadScene(FbxManager* pManager, FbxDocument* pScene, const char* pFilename);

// Create a camera
FbxNode* CreateCamera(
	FbxScene* pScene,
	char* pName
);

void SetCameraDefaultPosition(
	FbxNode* pCamera
);

void AnimatePosition(
	FbxNode* pPosition,
	FbxAnimLayer* pAnimLayer,
    nlohmann::json& j,
	std::string& nodeName
);

void AnimateRotation(
	FbxNode* pPosition,
	FbxAnimLayer* pAnimLayer,
	nlohmann::json& j,
	std::string & nodeName
);

void CreateMaterials(
	FbxScene* pScene,
	FbxMesh* pMesh
);

FbxNode* CreatePyramidWithMaterials(
	FbxScene* pScene,
	char* pName,
	const double& side, 
	const double& height
);

FbxNode* CreatePyramidWithMaterialsRightHand(
	FbxScene* pScene,
	char* pName,
	const double& side,
	const double& height
);

void SetMeshDefaultPosition(
	FbxNode* pMesh,
	FbxVector4& location,
	FbxVector4& rotation
);

// Create a marker to use a point of interest for the camera. 
FbxNode* CreateMarker(
	FbxScene* pScene,
	char* pName
);

void SetMarkerDefaultPosition(
	FbxNode* pMarker,
	const FbxVector4& rotation
);

#endif // #ifndef _COMMON_H


