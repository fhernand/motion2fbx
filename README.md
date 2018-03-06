# motion2fbx
Convert recorded VR motion from A-Frame to FBX file format using the FBX SDK

Use the [A-Frame motion capture components](https://github.com/dmarcos/aframe-motion-capture-components) to capture motion. The resulting JSON file can be converted with this project into a FBX file that contains the recorded animation.

Usage: 
```
motion2fbx <json input file> <fbx output file> <FBX format>
```
where "FBX format" is either 0 for binary or -1 for ASCII format.

Note:
- VS 2017 was used to build the executable (release executable available in bin\motion2fbx\win32\net2015\release)
- Currently only the camera motion is considered.
- A mesh is included to visualize the camera position (for example in FBX Review)
- Position and rotation animations are assigned to separate marker elements
- The following FBX SDK version is used: Autodesk FBX SDK version 2018.1.1 Release (246450)

*******************************************************************************************************************************************

This software contains Autodesk® FBX® code developed by Autodesk, Inc. Copyright 2017 Autodesk, Inc. All rights, reserved. Such code is provided “as is” and Autodesk, Inc. disclaims any and all warranties, whether express or implied, including without limitation the implied warranties of merchantability, fitness for a particular purpose or non-infringement of third party rights. In no event shall Autodesk, Inc. be liable for any direct, indirect, incidental, special, exemplary, or consequential damages (including, but not limited to, procurement of substitute goods or services; loss of use, data, or profits; or business interruption) however caused and on any theory of liability, whether in contract, strict liability, or tort (including negligence or otherwise) arising in any way out of such code.”

*******************************************************************************************************************************************
