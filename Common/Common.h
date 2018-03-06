/****************************************************************************************

   Copyright (C) 2015 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/
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
    nlohmann::json& j
);

void AnimateRotation(
	FbxNode* pPosition,
	FbxAnimLayer* pAnimLayer,
	nlohmann::json& j
);

void CreateMaterials(
	FbxScene* pScene,
	FbxMesh* pMesh
);

FbxNode* CreatePyramidWithMaterials(
	FbxScene* pScene,
	char* pName
);

void SetMeshDefaultPosition(
	FbxNode* pMesh,
	FbxVector4& location
);

// Create a marker to use a point of interest for the camera. 
FbxNode* CreateMarker(
	FbxScene* pScene,
	char* pName
);

void SetMarkerDefaultPosition(
	FbxNode* pMarker
);

#endif // #ifndef _COMMON_H


