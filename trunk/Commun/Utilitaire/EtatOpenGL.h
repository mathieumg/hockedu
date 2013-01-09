//////////////////////////////////////////////////////////////////////////////
/// @file EtatOpenGL.h
/// @author Martin Bisson
/// @date 2007-07-27
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
//////////////////////////////////////////////////////////////////////////////
#ifndef __UTILITAIRE_ETATOPENGL_H__
#define __UTILITAIRE_ETATOPENGL_H__


#include "Utilitaire.h"

#include <windows.h>
#include "glew.h"
#include <iostream>
#include <string>
#include <strstream>
#include <vector>


//////////////////////////////////////////////////////////////////////////////
/// @class EtatOpenGL
/// @brief Classe qui représente l'état des variables de la machine OpenGL.
///
///        Cette classe permet d'aller chercher la valeur de la majorité des
///        variables de l'état de la machine OpenGL.  Elle permet d'afficher
///        ses valeurs et de comparer deux états différents afin d'afficher
///        seulement les différences entre deux états.  Le but de cette classe
///        est de faciliter le déboguage d'applications OpenGL.
///
///        À noter que seuls les variables définies dans la version 1.1 de
///        OpenGL sont lues par cette classe.  Il faudrait éventuellement
///        dériver d'autres classes de celle-ci afin d'ajouter les
///        fonctionnalités des versions plus récentes d'OpenGL.
///
/// @author Martin Bisson
/// @date 2007-07-27
//////////////////////////////////////////////////////////////////////////////
class EtatOpenGL
{
public:
   /// Constructeur par défaut.
   EtatOpenGL();

   /// Compare deux états OpenGL et affiche la différence entre les deux.
   static void obtenirDifference(
      std::ostream& o, const EtatOpenGL& etat1, const EtatOpenGL& etat2
      );

   /// Retourne une chaîne représentant l'attribut GL_ACCUM_ALPHA_BITS.
   std::string obtenirChaineGlAccumAlphaBits() const;
   /// Retourne une chaîne représentant l'attribut GL_ACCUM_BLUE_BITS.
   std::string obtenirChaineGlAccumBlueBits() const;
   /// Retourne une chaîne représentant l'attribut GL_ACCUM_CLEAR_VALUE.
   std::string obtenirChaineGlAccumClearValue() const;
   /// Retourne une chaîne représentant l'attribut GL_ACCUM_GREEN_BITS.
   std::string obtenirChaineGlAccumGreenBits() const;
   /// Retourne une chaîne représentant l'attribut GL_ACCUM_RED_BITS.
   std::string obtenirChaineGlAccumRedBits() const;
   /// Retourne une chaîne représentant l'attribut GL_ALPHA_BIAS.
   std::string obtenirChaineGlAlphaBias() const;
   /// Retourne une chaîne représentant l'attribut GL_ALPHA_BITS.
   std::string obtenirChaineGlAlphaBits() const;
   /// Retourne une chaîne représentant l'attribut GL_ALPHA_SCALE.
   std::string obtenirChaineGlAlphaScale() const;
   /// Retourne une chaîne représentant l'attribut GL_ALPHA_TEST.
   std::string obtenirChaineGlAlphaTest() const;
   /// Retourne une chaîne représentant l'attribut GL_ALPHA_TEST_FUNC.
   std::string obtenirChaineGlAlphaTestFunc() const;
   /// Retourne une chaîne représentant l'attribut GL_ALPHA_TEST_REF.
   std::string obtenirChaineGlAlphaTestRef() const;
   /// Retourne une chaîne représentant l'attribut GL_ATTRIB_STACK_DEPTH.
   std::string obtenirChaineGlAttribStackDepth() const;
   /// Retourne une chaîne représentant l'attribut GL_AUTO_NORMAL.
   std::string obtenirChaineGlAutoNormal() const;
   /// Retourne une chaîne représentant l'attribut GL_AUX_BUFFERS.
   std::string obtenirChaineGlAuxBuffers() const;
   /// Retourne une chaîne représentant l'attribut GL_BLEND.
   std::string obtenirChaineGlBlend() const;
   /// Retourne une chaîne représentant l'attribut GL_BLEND_DST.
   std::string obtenirChaineGlBlendDst() const;
   /// Retourne une chaîne représentant l'attribut GL_BLEND_SRC.
   std::string obtenirChaineGlBlendSrc() const;
   /// Retourne une chaîne représentant l'attribut GL_BLUE_BIAS.
   std::string obtenirChaineGlBlueBias() const;
   /// Retourne une chaîne représentant l'attribut GL_BLUE_BITS.
   std::string obtenirChaineGlBlueBits() const;
   /// Retourne une chaîne représentant l'attribut GL_BLUE_SCALE.
   std::string obtenirChaineGlBlueScale() const;
   /// Retourne une chaîne représentant l'attribut GL_CLIENT_ATTRIB_STACK_DEPTH.
   std::string obtenirChaineGlClientAttribStackDepth() const;
   /// Retourne une chaîne représentant l'attribut GL_CLIP_PLANEi.
   std::string obtenirChaineGlClipPlanei() const;
   /// Retourne une chaîne représentant l'attribut GL_COLOR_ARRAY.
   std::string obtenirChaineGlColorArray() const;
   /// Retourne une chaîne représentant l'attribut GL_COLOR_ARRAY_SIZE.
   std::string obtenirChaineGlColorArraySize() const;
   /// Retourne une chaîne représentant l'attribut GL_COLOR_ARRAY_STRIDE.
   std::string obtenirChaineGlColorArrayStride() const;
   /// Retourne une chaîne représentant l'attribut GL_COLOR_ARRAY_TYPE.
   std::string obtenirChaineGlColorArrayType() const;
   /// Retourne une chaîne représentant l'attribut GL_COLOR_CLEAR_VALUE.
   std::string obtenirChaineGlColorClearValue() const;
   /// Retourne une chaîne représentant l'attribut GL_COLOR_LOGIC_OP.
   std::string obtenirChaineGlColorLogicOp() const;
   /// Retourne une chaîne représentant l'attribut GL_COLOR_MATERIAL.
   std::string obtenirChaineGlColorMaterial() const;
   /// Retourne une chaîne représentant l'attribut GL_COLOR_MATERIAL_FACE.
   std::string obtenirChaineGlColorMaterialFace() const;
   /// Retourne une chaîne représentant l'attribut GL_COLOR_MATERIAL_PARAMETER.
   std::string obtenirChaineGlColorMaterialParameter() const;
   /// Retourne une chaîne représentant l'attribut GL_COLOR_WRITEMASK.
   std::string obtenirChaineGlColorWritemask() const;
   /// Retourne une chaîne représentant l'attribut GL_CULL_FACE.
   std::string obtenirChaineGlCullFace() const;
   /// Retourne une chaîne représentant l'attribut GL_CULL_FACE_MODE.
   std::string obtenirChaineGlCullFaceMode() const;
   /// Retourne une chaîne représentant l'attribut GL_CURRENT_COLOR.
   std::string obtenirChaineGlCurrentColor() const;
   /// Retourne une chaîne représentant l'attribut GL_CURRENT_INDEX.
   std::string obtenirChaineGlCurrentIndex() const;
   /// Retourne une chaîne représentant l'attribut GL_CURRENT_NORMAL.
   std::string obtenirChaineGlCurrentNormal() const;
   /// Retourne une chaîne représentant l'attribut GL_CURRENT_RASTER_COLOR.
   std::string obtenirChaineGlCurrentRasterColor() const;
   /// Retourne une chaîne représentant l'attribut GL_CURRENT_RASTER_DISTANCE.
   std::string obtenirChaineGlCurrentRasterDistance() const;
   /// Retourne une chaîne représentant l'attribut GL_CURRENT_RASTER_INDEX.
   std::string obtenirChaineGlCurrentRasterIndex() const;
   /// Retourne une chaîne représentant l'attribut GL_CURRENT_RASTER_POSITION.
   std::string obtenirChaineGlCurrentRasterPosition() const;
   /// Retourne une chaîne représentant l'attribut GL_CURRENT_RASTER_POSITION_VALID.
   std::string obtenirChaineGlCurrentRasterPositionValid() const;
   /// Retourne une chaîne représentant l'attribut GL_CURRENT_RASTER_TEXTURE_COORDS.
   std::string obtenirChaineGlCurrentRasterTextureCoords() const;
   /// Retourne une chaîne représentant l'attribut GL_CURRENT_TEXTURE_COORDS.
   std::string obtenirChaineGlCurrentTextureCoords() const;
   /// Retourne une chaîne représentant l'attribut GL_DEPTH_BIAS.
   std::string obtenirChaineGlDepthBias() const;
   /// Retourne une chaîne représentant l'attribut GL_DEPTH_BITS.
   std::string obtenirChaineGlDepthBits() const;
   /// Retourne une chaîne représentant l'attribut GL_DEPTH_CLEAR_VALUE.
   std::string obtenirChaineGlDepthClearValue() const;
   /// Retourne une chaîne représentant l'attribut GL_DEPTH_FUNC.
   std::string obtenirChaineGlDepthFunc() const;
   /// Retourne une chaîne représentant l'attribut GL_DEPTH_RANGE.
   std::string obtenirChaineGlDepthRange() const;
   /// Retourne une chaîne représentant l'attribut GL_DEPTH_SCALE.
   std::string obtenirChaineGlDepthScale() const;
   /// Retourne une chaîne représentant l'attribut GL_DEPTH_TEST.
   std::string obtenirChaineGlDepthTest() const;
   /// Retourne une chaîne représentant l'attribut GL_DEPTH_WRITEMASK.
   std::string obtenirChaineGlDepthWritemask() const;
   /// Retourne une chaîne représentant l'attribut GL_DITHER.
   std::string obtenirChaineGlDither() const;
   /// Retourne une chaîne représentant l'attribut GL_DOUBLEBUFFER.
   std::string obtenirChaineGlDoublebuffer() const;
   /// Retourne une chaîne représentant l'attribut GL_DRAW_BUFFER.
   std::string obtenirChaineGlDrawBuffer() const;
   /// Retourne une chaîne représentant l'attribut GL_EDGE_FLAG.
   std::string obtenirChaineGlEdgeFlag() const;
   /// Retourne une chaîne représentant l'attribut GL_EDGE_FLAG_ARRAY.
   std::string obtenirChaineGlEdgeFlagArray() const;
   /// Retourne une chaîne représentant l'attribut GL_EDGE_FLAG_ARRAY_STRIDE.
   std::string obtenirChaineGlEdgeFlagArrayStride() const;
   /// Retourne une chaîne représentant l'attribut GL_FEEDBACK_BUFFER_SIZE.
   std::string obtenirChaineGlFeedbackBufferSize() const;
   /// Retourne une chaîne représentant l'attribut GL_FEEDBACK_BUFFER_TYPE.
   std::string obtenirChaineGlFeedbackBufferType() const;
   /// Retourne une chaîne représentant l'attribut GL_FOG.
   std::string obtenirChaineGlFog() const;
   /// Retourne une chaîne représentant l'attribut GL_FOG_COLOR.
   std::string obtenirChaineGlFogColor() const;
   /// Retourne une chaîne représentant l'attribut GL_FOG_DENSITY.
   std::string obtenirChaineGlFogDensity() const;
   /// Retourne une chaîne représentant l'attribut GL_FOG_END.
   std::string obtenirChaineGlFogEnd() const;
   /// Retourne une chaîne représentant l'attribut GL_FOG_HINT.
   std::string obtenirChaineGlFogHint() const;
   /// Retourne une chaîne représentant l'attribut GL_FOG_INDEX.
   std::string obtenirChaineGlFogIndex() const;
   /// Retourne une chaîne représentant l'attribut GL_FOG_MODE.
   std::string obtenirChaineGlFogMode() const;
   /// Retourne une chaîne représentant l'attribut GL_FOG_START.
   std::string obtenirChaineGlFogStart() const;
   /// Retourne une chaîne représentant l'attribut GL_FRONT_FACE.
   std::string obtenirChaineGlFrontFace() const;
   /// Retourne une chaîne représentant l'attribut GL_GREEN_BIAS.
   std::string obtenirChaineGlGreenBias() const;
   /// Retourne une chaîne représentant l'attribut GL_GREEN_BITS.
   std::string obtenirChaineGlGreenBits() const;
   /// Retourne une chaîne représentant l'attribut GL_GREEN_SCALE.
   std::string obtenirChaineGlGreenScale() const;
   /// Retourne une chaîne représentant l'attribut GL_INDEX_ARRAY.
   std::string obtenirChaineGlIndexArray() const;
   /// Retourne une chaîne représentant l'attribut GL_INDEX_ARRAY_STRIDE.
   std::string obtenirChaineGlIndexArrayStride() const;
   /// Retourne une chaîne représentant l'attribut GL_INDEX_ARRAY_TYPE.
   std::string obtenirChaineGlIndexArrayType() const;
   /// Retourne une chaîne représentant l'attribut GL_INDEX_BITS.
   std::string obtenirChaineGlIndexBits() const;
   /// Retourne une chaîne représentant l'attribut GL_INDEX_CLEAR_VALUE.
   std::string obtenirChaineGlIndexClearValue() const;
   /// Retourne une chaîne représentant l'attribut GL_INDEX_LOGIC_OP.
   std::string obtenirChaineGlIndexLogicOp() const;
   /// Retourne une chaîne représentant l'attribut GL_INDEX_MODE.
   std::string obtenirChaineGlIndexMode() const;
   /// Retourne une chaîne représentant l'attribut GL_INDEX_OFFSET.
   std::string obtenirChaineGlIndexOffset() const;
   /// Retourne une chaîne représentant l'attribut GL_INDEX_SHIFT.
   std::string obtenirChaineGlIndexShift() const;
   /// Retourne une chaîne représentant l'attribut GL_INDEX_WRITEMASK.
   std::string obtenirChaineGlIndexWritemask() const;
   /// Retourne une chaîne représentant l'attribut GL_LIGHTi.
   std::string obtenirChaineGlLighti() const;
   /// Retourne une chaîne représentant l'attribut GL_LIGHTING.
   std::string obtenirChaineGlLighting() const;
   /// Retourne une chaîne représentant l'attribut GL_LIGHT_MODEL_AMBIENT.
   std::string obtenirChaineGlLightModelAmbient() const;
   /// Retourne une chaîne représentant l'attribut GL_LIGHT_MODEL_LOCAL_VIEWER.
   std::string obtenirChaineGlLightModelLocalViewer() const;
   /// Retourne une chaîne représentant l'attribut GL_LIGHT_MODEL_TWO_SIDE.
   std::string obtenirChaineGlLightModelTwoSide() const;
   /// Retourne une chaîne représentant l'attribut GL_LINE_SMOOTH.
   std::string obtenirChaineGlLineSmooth() const;
   /// Retourne une chaîne représentant l'attribut GL_LINE_SMOOTH_HINT.
   std::string obtenirChaineGlLineSmoothHint() const;
   /// Retourne une chaîne représentant l'attribut GL_LINE_STIPPLE.
   std::string obtenirChaineGlLineStipple() const;
   /// Retourne une chaîne représentant l'attribut GL_LINE_STIPPLE_PATTERN.
   std::string obtenirChaineGlLineStipplePattern() const;
   /// Retourne une chaîne représentant l'attribut GL_LINE_STIPPLE_REPEAT.
   std::string obtenirChaineGlLineStippleRepeat() const;
   /// Retourne une chaîne représentant l'attribut GL_LINE_WIDTH.
   std::string obtenirChaineGlLineWidth() const;
   /// Retourne une chaîne représentant l'attribut GL_LINE_WIDTH_GRANULARITY.
   std::string obtenirChaineGlLineWidthGranularity() const;
   /// Retourne une chaîne représentant l'attribut GL_LINE_WIDTH_RANGE.
   std::string obtenirChaineGllineWidthRange() const;
   /// Retourne une chaîne représentant l'attribut GL_LIST_BASE.
   std::string obtenirChaineGlListBase() const;
   /// Retourne une chaîne représentant l'attribut GL_LIST_INDEX.
   std::string obtenirChaineGlListIndex() const;
   /// Retourne une chaîne représentant l'attribut GL_LIST_MODE.
   std::string obtenirChaineGlListMode() const;
   /// Retourne une chaîne représentant l'attribut GL_LOGIC_OP_MODE.
   std::string obtenirChaineGlLogicOpMode() const;
   /// Retourne une chaîne représentant l'attribut GL_MAP1_COLOR_4.
   std::string obtenirChaineGlMap1Color4() const;
   /// Retourne une chaîne représentant l'attribut GL_MAP1_GRID_DOMAIN.
   std::string obtenirChaineGlMap1GridDomain() const;
   /// Retourne une chaîne représentant l'attribut GL_MAP1_GRID_SEGMENTS.
   std::string obtenirChaineGlMap1GridSegments() const;
   /// Retourne une chaîne représentant l'attribut GL_MAP1_INDEX.
   std::string obtenirChaineGlMap1Index() const;
   /// Retourne une chaîne représentant l'attribut GL_MAP1_NORMAL.
   std::string obtenirChaineGlMap1Normal() const;
   /// Retourne une chaîne représentant l'attribut GL_MAP1_TEXTURE_COORD_1.
   std::string obtenirChaineGlMap1TextureCoord1() const;
   /// Retourne une chaîne représentant l'attribut GL_MAP1_TEXTURE_COORD_2.
   std::string obtenirChaineGlMap1TextureCoord2() const;
   /// Retourne une chaîne représentant l'attribut GL_MAP1_TEXTURE_COORD_3.
   std::string obtenirChaineGlMap1TextureCoord3() const;
   /// Retourne une chaîne représentant l'attribut GL_MAP1_TEXTURE_COORD_4.
   std::string obtenirChaineGlMap1TextureCoord4() const;
   /// Retourne une chaîne représentant l'attribut GL_MAP1_VERTEX_3.
   std::string obtenirChaineGlMap1Vertex3() const;
   /// Retourne une chaîne représentant l'attribut GL_MAP1_VERTEX_4.
   std::string obtenirChaineGlMap1Vertex4() const;
   /// Retourne une chaîne représentant l'attribut GL_MAP2_COLOR_4.
   std::string obtenirChaineGlMap2Color4() const;
   /// Retourne une chaîne représentant l'attribut GL_MAP2_GRID_DOMAIN.
   std::string obtenirChaineGlMap2GridDomain() const;
   /// Retourne une chaîne représentant l'attribut GL_MAP2_GRID_SEGMENTS.
   std::string obtenirChaineGlMap2GridSegments() const;
   /// Retourne une chaîne représentant l'attribut GL_MAP2_INDEX.
   std::string obtenirChaineGlMap2Index() const;
   /// Retourne une chaîne représentant l'attribut GL_MAP2_NORMAL.
   std::string obtenirChaineGlMap2Normal() const;
   /// Retourne une chaîne représentant l'attribut GL_MAP2_TEXTURE_COORD_1.
   std::string obtenirChaineGlMap2TextureCoord1() const;
   /// Retourne une chaîne représentant l'attribut GL_MAP2_TEXTURE_COORD_2.
   std::string obtenirChaineGlMap2TextureCoord2() const;
   /// Retourne une chaîne représentant l'attribut GL_MAP2_TEXTURE_COORD_3.
   std::string obtenirChaineGlMap2TextureCoord3() const;
   /// Retourne une chaîne représentant l'attribut GL_MAP2_TEXTURE_COORD_4.
   std::string obtenirChaineGlMap2TextureCoord4() const;
   /// Retourne une chaîne représentant l'attribut GL_MAP2_VERTEX_3.
   std::string obtenirChaineGlMap2Vertex3() const;
   /// Retourne une chaîne représentant l'attribut GL_MAP2_VERTEX_4.
   std::string obtenirChaineGlMap2Vertex4() const;
   /// Retourne une chaîne représentant l'attribut GL_MAP_COLOR.
   std::string obtenirChaineGlMapColor() const;
   /// Retourne une chaîne représentant l'attribut GL_MAP_STENCIL.
   std::string obtenirChaineGlMapStencil() const;
   /// Retourne une chaîne représentant l'attribut GL_MATRIX_MODE.
   std::string obtenirChaineGlMatrixMode() const;
   /// Retourne une chaîne représentant l'attribut GL_MAX_CLIENT_ATTRIB_STACK_DEPTH.
   std::string obtenirChaineGlMaxClientAttribStackDepth() const;
   /// Retourne une chaîne représentant l'attribut GL_MAX_ATTRIB_STACK_DEPTH.
   std::string obtenirChaineGlMaxAttribStackDepth() const;
   /// Retourne une chaîne représentant l'attribut GL_MAX_CLIP_PLANES.
   std::string obtenirChaineGlMaxClipPlanes() const;
   /// Retourne une chaîne représentant l'attribut GL_MAX_EVAL_ORDER.
   std::string obtenirChaineGlMaxEvalOrder() const;
   /// Retourne une chaîne représentant l'attribut GL_MAX_LIGHTS.
   std::string obtenirChaineGlMaxLights() const;
   /// Retourne une chaîne représentant l'attribut GL_MAX_LIST_NESTING.
   std::string obtenirChaineGlMaxListNesting() const;
   /// Retourne une chaîne représentant l'attribut GL_MAX_MODELVIEW_STACK_DEPTH.
   std::string obtenirChaineGlMaxModelviewStackDepth() const;
   /// Retourne une chaîne représentant l'attribut GL_MAX_NAME_STACK_DEPTH.
   std::string obtenirChaineGlMaxNameStackDepth() const;
   /// Retourne une chaîne représentant l'attribut GL_MAX_PIXEL_MAP_TABLE.
   std::string obtenirChaineGlMaxPixelMapTable() const;
   /// Retourne une chaîne représentant l'attribut GL_MAX_PROJECTION_STACK_DEPTH.
   std::string obtenirChaineGlMaxProjectionStackDepth() const;
   /// Retourne une chaîne représentant l'attribut GL_MAX_TEXTURE_SIZE.
   std::string obtenirChaineGlMaxTextureSize() const;
   /// Retourne une chaîne représentant l'attribut GL_MAX_TEXTURE_STACK_DEPTH.
   std::string obtenirChaineGlMaxTextureStackDepth() const;
   /// Retourne une chaîne représentant l'attribut GL_MAX_VIEWPORT_DIMS.
   std::string obtenirChaineGlMaxViewportDims() const;
   /// Retourne une chaîne représentant l'attribut GL_MODELVIEW_MATRIX.
   std::string obtenirChaineGlModelviewMatrix() const;
   /// Retourne une chaîne représentant l'attribut GL_MODELVIEW_STACK_DEPTH.
   std::string obtenirChaineGlModelviewStackDepth() const;
   /// Retourne une chaîne représentant l'attribut GL_NAME_STACK_DEPTH.
   std::string obtenirChaineGlNameStackDepth() const;
   /// Retourne une chaîne représentant l'attribut GL_NORMAL_ARRAY.
   std::string obtenirChaineGlNormalArray() const;
   /// Retourne une chaîne représentant l'attribut GL_NORMAL_ARRAY_STRIDE.
   std::string obtenirChaineGlNormalArrayStride() const;
   /// Retourne une chaîne représentant l'attribut GL_NORMAL_ARRAY_TYPE.
   std::string obtenirChaineGlNormalArrayType() const;
   /// Retourne une chaîne représentant l'attribut GL_NORMALIZE.
   std::string obtenirChaineGlNormalize() const;
   /// Retourne une chaîne représentant l'attribut GL_PACK_ALIGNMENT.
   std::string obtenirChaineGlPackAlignment() const;
   /// Retourne une chaîne représentant l'attribut GL_PACK_LSB_FIRST.
   std::string obtenirChaineGlPackLsbFirst() const;
   /// Retourne une chaîne représentant l'attribut GL_PACK_ROW_LENGTH.
   std::string obtenirChaineGlPackRowLength() const;
   /// Retourne une chaîne représentant l'attribut GL_PACK_SKIP_PIXELS.
   std::string obtenirChaineGlPackSkipPixels() const;
   /// Retourne une chaîne représentant l'attribut GL_PACK_SKIP_ROWS.
   std::string obtenirChaineGlPackSkipRows() const;
   /// Retourne une chaîne représentant l'attribut GL_PACK_SWAP_BYTES.
   std::string obtenirChaineGlPackSwapBytes() const;
   /// Retourne une chaîne représentant l'attribut GL_PERSPECTIVE_CORRECTION_HINT.
   std::string obtenirChaineGlPerspectiveCorrectionHint() const;
   /// Retourne une chaîne représentant l'attribut GL_PIXEL_MAP_A_TO_A_SIZE.
   std::string obtenirChaineGlPixelMapAToASize() const;
   /// Retourne une chaîne représentant l'attribut GL_PIXEL_MAP_B_TO_B_SIZE.
   std::string obtenirChaineGlPixelMapBToBSize() const;
   /// Retourne une chaîne représentant l'attribut GL_PIXEL_MAP_G_TO_G_SIZE.
   std::string obtenirChaineGlPixelMapGToGSize() const;
   /// Retourne une chaîne représentant l'attribut GL_PIXEL_MAP_I_TO_A_SIZE.
   std::string obtenirChaineGlPixelMapIToASize() const;
   /// Retourne une chaîne représentant l'attribut GL_PIXEL_MAP_I_TO_B_SIZE.
   std::string obtenirChaineGlPixelMapIToBSize() const;
   /// Retourne une chaîne représentant l'attribut GL_PIXEL_MAP_I_TO_G_SIZE.
   std::string obtenirChaineGlPixelMapIToGSize() const;
   /// Retourne une chaîne représentant l'attribut GL_PIXEL_MAP_I_TO_I_SIZE.
   std::string obtenirChaineGlPixelMapIToISize() const;
   /// Retourne une chaîne représentant l'attribut GL_PIXEL_MAP_I_TO_R_SIZE.
   std::string obtenirChaineGlPixelMapIToRSize() const;
   /// Retourne une chaîne représentant l'attribut GL_PIXEL_MAP_R_TO_R_SIZE.
   std::string obtenirChaineGlPixelMapRToRSize() const;
   /// Retourne une chaîne représentant l'attribut GL_PIXEL_MAP_S_TO_S_SIZE.
   std::string obtenirChaineGlPixelMapSToSSize() const;
   /// Retourne une chaîne représentant l'attribut GL_POINT_SIZE.
   std::string obtenirChaineGlPointSize() const;
   /// Retourne une chaîne représentant l'attribut GL_POINT_SIZE_GRANULARITY.
   std::string obtenirChaineGlPointSizeGranularity() const;
   /// Retourne une chaîne représentant l'attribut GL_POINT_SIZE_RANGE.
   std::string obtenirChaineGlPointSizeRange() const;
   /// Retourne une chaîne représentant l'attribut GL_POINT_SMOOTH.
   std::string obtenirChaineGlPointSmooth() const;
   /// Retourne une chaîne représentant l'attribut GL_POINT_SMOOTH_HINT.
   std::string obtenirChaineGlPointSmoothHint() const;
   /// Retourne une chaîne représentant l'attribut GL_POLYGON_MODE.
   std::string obtenirChaineGlPolygonMode() const;
   /// Retourne une chaîne représentant l'attribut GL_POLYGON_OFFSET_FACTOR.
   std::string obtenirChaineGlPolygonOffsetFactor() const;
   /// Retourne une chaîne représentant l'attribut GL_POLYGON_OFFSET_UNITS.
   std::string obtenirChaineGlPolygonOffsetUnits() const;
   /// Retourne une chaîne représentant l'attribut GL_POLYGON_OFFSET_FILL.
   std::string obtenirChaineGlPolygonOffsetFill() const;
   /// Retourne une chaîne représentant l'attribut GL_POLYGON_OFFSET_LINE.
   std::string obtenirChaineGlPolygonOffsetLine() const;
   /// Retourne une chaîne représentant l'attribut GL_POLYGON_OFFSET_POINT.
   std::string obtenirChaineGlPolygonOffsetPoint() const;
   /// Retourne une chaîne représentant l'attribut GL_POLYGON_SMOOTH.
   std::string obtenirChaineGlPolygonSmooth() const;
   /// Retourne une chaîne représentant l'attribut GL_POLYGON_SMOOTH_HINT.
   std::string obtenirChaineGlPolygonSmoothHint() const;
   /// Retourne une chaîne représentant l'attribut GL_POLYGON_STIPPLE.
   std::string obtenirChaineGlPolygonStipple() const;
   /// Retourne une chaîne représentant l'attribut GL_PROJECTION_MATRIX.
   std::string obtenirChaineGlProjectionMatrix() const;
   /// Retourne une chaîne représentant l'attribut GL_PROJECTION_STACK_DEPTH.
   std::string obtenirChaineGlProjectionStackDepth() const;
   /// Retourne une chaîne représentant l'attribut GL_READ_BUFFER.
   std::string obtenirChaineGlReadBuffer() const;
   /// Retourne une chaîne représentant l'attribut GL_RED_BIAS.
   std::string obtenirChaineGlRedBias() const;
   /// Retourne une chaîne représentant l'attribut GL_RED_BITS.
   std::string obtenirChaineGlRedBits() const;
   /// Retourne une chaîne représentant l'attribut GL_RED_SCALE.
   std::string obtenirChaineGlRedScale() const;
   /// Retourne une chaîne représentant l'attribut GL_RENDER_MODE.
   std::string obtenirChaineGlRenderMode() const;
   /// Retourne une chaîne représentant l'attribut GL_RGBA_MODE.
   std::string obtenirChaineGlRgbaMode() const;
   /// Retourne une chaîne représentant l'attribut GL_SCISSOR_BOX.
   std::string obtenirChaineGlScissorBox() const;
   /// Retourne une chaîne représentant l'attribut GL_SCISSOR_TEST.
   std::string obtenirChaineGlScissorTest() const;
   /// Retourne une chaîne représentant l'attribut GL_SELECTION_BUFFER_SIZE.
   std::string obtenirChaineGlSelectionBufferSize() const;
   /// Retourne une chaîne représentant l'attribut GL_SHADE_MODEL.
   std::string obtenirChaineGlShadeModel() const;
   /// Retourne une chaîne représentant l'attribut GL_STENCIL_BITS.
   std::string obtenirChaineGlStencilBits() const;
   /// Retourne une chaîne représentant l'attribut GL_STENCIL_CLEAR_VALUE.
   std::string obtenirChaineGlStencilClearValue() const;
   /// Retourne une chaîne représentant l'attribut GL_STENCIL_FAIL.
   std::string obtenirChaineGlStencilFail() const;
   /// Retourne une chaîne représentant l'attribut GL_STENCIL_FUNC.
   std::string obtenirChaineGlStencilFunc() const;
   /// Retourne une chaîne représentant l'attribut GL_STENCIL_PASS_DEPTH_FAIL.
   std::string obtenirChaineGlStencilPassDepthFail() const;
   /// Retourne une chaîne représentant l'attribut GL_STENCIL_PASS_DEPTH_PASS.
   std::string obtenirChaineGlStencilPassDepthPass() const;
   /// Retourne une chaîne représentant l'attribut GL_STENCIL_REF.
   std::string obtenirChaineGlStencilRef() const;
   /// Retourne une chaîne représentant l'attribut GL_STENCIL_TEST.
   std::string obtenirChaineGlStencilTest() const;
   /// Retourne une chaîne représentant l'attribut GL_STENCIL_VALUE_MASK.
   std::string obtenirChaineGlStencilValueMask() const;
   /// Retourne une chaîne représentant l'attribut GL_STENCIL_WRITEMASK.
   std::string obtenirChaineGlStencilWritemask() const;
   /// Retourne une chaîne représentant l'attribut GL_STEREO.
   std::string obtenirChaineGlStereo() const;
   /// Retourne une chaîne représentant l'attribut GL_SUBPIXEL_BITS.
   std::string obtenirChaineGlSubpixelBits() const;
   /// Retourne une chaîne représentant l'attribut GL_TEXTURE_1D.
   std::string obtenirChaineGlTexture1D() const;
   /// Retourne une chaîne représentant l'attribut GL_TEXTURE_BINDING_1D.
   std::string obtenirChaineGlTextureBinding1D() const;
   /// Retourne une chaîne représentant l'attribut GL_TEXTURE_2D.
   std::string obtenirChaineGlTexture2D() const;
   /// Retourne une chaîne représentant l'attribut GL_TEXTURE_BINDING_2D.
   std::string obtenirChaineGlTextureBinding2D() const;
   /// Retourne une chaîne représentant l'attribut GL_TEXTURE_COORD_ARRAY.
   std::string obtenirChaineGlTextureCoordArray() const;
   /// Retourne une chaîne représentant l'attribut GL_TEXTURE_COORD_ARRAY_SIZE.
   std::string obtenirChaineGlTextureCoordArraySize() const;
   /// Retourne une chaîne représentant l'attribut GL_TEXTURE_COORD_ARRAY_STRIDE.
   std::string obtenirChaineGlTextureCoordArrayStride() const;
   /// Retourne une chaîne représentant l'attribut GL_TEXTURE_COORD_ARRAY_TYPE.
   std::string obtenirChaineGlTextureCoordArrayType() const;
   /// Retourne une chaîne représentant l'attribut GL_TEXTURE_GEN_Q.
   std::string obtenirChaineGlTextureGenQ() const;
   /// Retourne une chaîne représentant l'attribut GL_TEXTURE_GEN_R.
   std::string obtenirChaineGlTextureGenR() const;
   /// Retourne une chaîne représentant l'attribut GL_TEXTURE_GEN_S.
   std::string obtenirChaineGlTextureGenS() const;
   /// Retourne une chaîne représentant l'attribut GL_TEXTURE_GEN_T.
   std::string obtenirChaineGlTextureGenT() const;
   /// Retourne une chaîne représentant l'attribut GL_TEXTURE_MATRIX.
   std::string obtenirChaineGlTextureMatrix() const;
   /// Retourne une chaîne représentant l'attribut GL_TEXTURE_STACK_DEPTH.
   std::string obtenirChaineGlTextureStackDepth() const;
   /// Retourne une chaîne représentant l'attribut GL_UNPACK_ALIGNMENT.
   std::string obtenirChaineGlUnpackAlignment() const;
   /// Retourne une chaîne représentant l'attribut GL_UNPACK_LSB_FIRST.
   std::string obtenirChaineGlUnpackLsbFirst() const;
   /// Retourne une chaîne représentant l'attribut GL_UNPACK_ROW_LENGTH.
   std::string obtenirChaineGlUnpackRowLength() const;
   /// Retourne une chaîne représentant l'attribut GL_UNPACK_SKIP_PIXELS.
   std::string obtenirChaineGlUnpackSkipPixels() const;
   /// Retourne une chaîne représentant l'attribut GL_UNPACK_SKIP_ROWS.
   std::string obtenirChaineGlUnpackSkipRows() const;
   /// Retourne une chaîne représentant l'attribut GL_UNPACK_SWAP_BYTES.
   std::string obtenirChaineGlUnpackSwapBytes() const;
   /// Retourne une chaîne représentant l'attribut GL_VERTEX_ARRAY.
   std::string obtenirChaineGlVertexArray() const;
   /// Retourne une chaîne représentant l'attribut GL_VERTEX_ARRAY_SIZE.
   std::string obtenirChaineGlVertexArraySize() const;
   /// Retourne une chaîne représentant l'attribut GL_VERTEX_ARRAY_STRIDE.
   std::string obtenirChaineGlVertexArrayStride() const;
   /// Retourne une chaîne représentant l'attribut GL_VERTEX_ARRAY_TYPE.
   std::string obtenirChaineGlVertexArrayType() const;
   /// Retourne une chaîne représentant l'attribut GL_VIEWPORT.
   std::string obtenirChaineGlViewport() const;
   /// Retourne une chaîne représentant l'attribut GL_ZOOM_X.
   std::string obtenirChaineGlZoomX() const;
   /// Retourne une chaîne représentant l'attribut GL_ZOOM_Y.
   std::string obtenirChaineGlZoomY() const;


private:
   /// Fonction qui retourne la chaîne d'un vecteur de valeurs.
   template<class T>
   static std::string obtenirChaineVecteur(const T* v, int size);
   /// Fonction qui affiche une valeur booléenne d'OpenGL.
   inline static std::string obtenirChaineBooleen(GLboolean v);

   /// L'attribut GL_ACCUM_ALPHA_BITS.
   GLint                  glAccumAlphaBits_;
   /// L'attribut GL_ACCUM_BLUE_BITS.
   GLint                  glAccumBlueBits_;
   /// L'attribut GL_ACCUM_CLEAR_VALUE.
   GLfloat                glAccumClearValue_[4];
   /// L'attribut GL_ACCUM_GREEN_BITS.
   GLint                  glAccumGreenBits_;
   /// L'attribut GL_ACCUM_RED_BITS.
   GLint                  glAccumRedBits_;
   /// L'attribut GL_ALPHA_BIAS.
   GLfloat                glAlphaBias_;
   /// L'attribut GL_ALPHA_BITS.
   GLint                  glAlphaBits_;
   /// L'attribut GL_ALPHA_SCALE.
   GLfloat                glAlphaScale_;
   /// L'attribut GL_ALPHA_TEST.
   GLboolean              glAlphaTest_;
   /// L'attribut GL_ALPHA_TEST_FUNC.
   GLint                  glAlphaTestFunc_;
   /// L'attribut GL_ALPHA_TEST_REF.
   GLfloat                glAlphaTestRef_;
   /// L'attribut GL_ATTRIB_STACK_DEPTH.
   GLint                  glAttribStackDepth_;
   /// L'attribut GL_AUTO_NORMAL.
   GLboolean              glAutoNormal_;
   /// L'attribut GL_AUX_BUFFERS.
   GLint                  glAuxBuffers_;
   /// L'attribut GL_BLEND.
   GLboolean              glBlend_;
   /// L'attribut GL_BLEND_DST.
   GLint                  glBlendDst_;
   /// L'attribut GL_BLEND_SRC.
   GLint                  glBlendSrc_;
   /// L'attribut GL_BLUE_BIAS.
   GLfloat                glBlueBias_;
   /// L'attribut GL_BLUE_BITS.
   GLint                  glBlueBits_;
   /// L'attribut GL_BLUE_SCALE.
   GLfloat                glBlueScale_;
   /// L'attribut GL_CLIENT_ATTRIB_STACK_DEPTH.
   GLint                  glClientAttribStackDepth_;
   /// L'attribut GL_CLIP_PLANEi.
   std::vector<GLboolean> glClipPlanei_;
   /// L'attribut GL_COLOR_ARRAY.
   GLboolean              glColorArray_;
   /// L'attribut GL_COLOR_ARRAY_SIZE.
   GLint                  glColorArraySize_;
   /// L'attribut GL_COLOR_ARRAY_STRIDE.
   GLint                  glColorArrayStride_;
   /// L'attribut GL_COLOR_ARRAY_TYPE.
   GLint                  glColorArrayType_;
   /// L'attribut GL_COLOR_CLEAR_VALUE.
   GLfloat                glColorClearValue_[4];
   /// L'attribut GL_COLOR_LOGIC_OP.
   GLboolean              glColorLogicOp_;
   /// L'attribut GL_COLOR_MATERIAL.
   GLboolean              glColorMaterial_;
   /// L'attribut GL_COLOR_MATERIAL_FACE.
   GLint                  glColorMaterialFace_;
   /// L'attribut GL_COLOR_MATERIAL_PARAMETER.
   GLint                  glColorMaterialParameter_;
   /// L'attribut GL_COLOR_WRITEMASK.
   GLboolean              glColorWritemask_[4];
   /// L'attribut GL_CULL_FACE.
   GLboolean              glCullFace_;
   /// L'attribut GL_CULL_FACE_MODE.
   GLint                  glCullFaceMode_;
   /// L'attribut GL_CURRENT_COLOR.
   GLfloat                glCurrentColor_[4];
   /// L'attribut GL_CURRENT_INDEX.
   GLint                  glCurrentIndex_;
   /// L'attribut GL_CURRENT_NORMAL.
   GLfloat                glCurrentNormal_[3];
   /// L'attribut GL_CURRENT_RASTER_COLOR.
   GLfloat                glCurrentRasterColor_[4];
   /// L'attribut GL_CURRENT_RASTER_DISTANCE.
   GLfloat                glCurrentRasterDistance_;
   /// L'attribut GL_CURRENT_RASTER_INDEX.
   GLint                  glCurrentRasterIndex_;
   /// L'attribut GL_CURRENT_RASTER_POSITION.
   GLfloat                glCurrentRasterPosition_[4];
   /// L'attribut GL_CURRENT_RASTER_POSITION_VALID.
   GLboolean              glCurrentRasterPositionValid_;
   /// L'attribut GL_CURRENT_RASTER_TEXTURE_COORDS.
   GLfloat                glCurrentRasterTextureCoords_[4];
   /// L'attribut GL_CURRENT_TEXTURE_COORDS.
   GLfloat                glCurrentTextureCoords_[4];
   /// L'attribut GL_DEPTH_BIAS.
   GLfloat                glDepthBias_;
   /// L'attribut GL_DEPTH_BITS.
   GLint                  glDepthBits_;
   /// L'attribut GL_DEPTH_CLEAR_VALUE.
   GLfloat                glDepthClearValue_;
   /// L'attribut GL_DEPTH_FUNC.
   GLint                  glDepthFunc_;
   /// L'attribut GL_DEPTH_RANGE.
   GLfloat                glDepthRange_[2];
   /// L'attribut GL_DEPTH_SCALE.
   GLfloat                glDepthScale_;
   /// L'attribut GL_DEPTH_TEST.
   GLboolean              glDepthTest_;
   /// L'attribut GL_DEPTH_WRITEMASK.
   GLboolean              glDepthWritemask_;
   /// L'attribut GL_DITHER.
   GLboolean              glDither_;
   /// L'attribut GL_DOUBLEBUFFER.
   GLboolean              glDoublebuffer_;
   /// L'attribut GL_DRAW_BUFFER.
   GLint                  glDrawBuffer_;
   /// L'attribut GL_EDGE_FLAG.
   GLboolean              glEdgeFlag_;
   /// L'attribut GL_EDGE_FLAG_ARRAY.
   GLboolean              glEdgeFlagArray_;
   /// L'attribut GL_EDGE_FLAG_ARRAY_STRIDE.
   GLint                  glEdgeFlagArrayStride_;
   /// L'attribut GL_FEEDBACK_BUFFER_SIZE.
   GLint                  glFeedbackBufferSize_;
   /// L'attribut GL_FEEDBACK_BUFFER_TYPE.
   GLint                  glFeedbackBufferType_;
   /// L'attribut GL_FOG.
   GLboolean              glFog_;
   /// L'attribut GL_FOG_COLOR.
   GLfloat                glFogColor_[4];
   /// L'attribut GL_FOG_DENSITY.
   GLfloat                glFogDensity_;
   /// L'attribut GL_FOG_END.
   GLfloat                glFogEnd_;
   /// L'attribut GL_FOG_HINT.
   GLint                  glFogHint_;
   /// L'attribut GL_FOG_INDEX.
   GLint                  glFogIndex_;
   /// L'attribut GL_FOG_MODE.
   GLint                  glFogMode_;
   /// L'attribut GL_FOG_START.
   GLfloat                glFogStart_;
   /// L'attribut GL_FRONT_FACE.
   GLint                  glFrontFace_;
   /// L'attribut GL_GREEN_BIAS.
   GLfloat                glGreenBias_;
   /// L'attribut GL_GREEN_BITS.
   GLint                  glGreenBits_;
   /// L'attribut GL_GREEN_SCALE.
   GLfloat                glGreenScale_;
   /// L'attribut GL_INDEX_ARRAY.
   GLboolean              glIndexArray_;
   /// L'attribut GL_INDEX_ARRAY_STRIDE.
   GLint                  glIndexArrayStride_;
   /// L'attribut GL_INDEX_ARRAY_TYPE.
   GLint                  glIndexArrayType_;
   /// L'attribut GL_INDEX_BITS.
   GLint                  glIndexBits_;
   /// L'attribut GL_INDEX_CLEAR_VALUE.
   GLint                  glIndexClearValue_;
   /// L'attribut GL_INDEX_LOGIC_OP.
   GLboolean              glIndexLogicOp_;
   /// L'attribut GL_INDEX_MODE.
   GLboolean              glIndexMode_;
   /// L'attribut GL_INDEX_OFFSET.
   GLint                  glIndexOffset_;
   /// L'attribut GL_INDEX_SHIFT.
   GLint                  glIndexShift_;
   /// L'attribut GL_INDEX_WRITEMASK.
   GLint                  glIndexWritemask_;
   /// L'attribut GL_LIGHTi.
   std::vector<GLboolean> glLighti_;
   /// L'attribut GL_LIGHTING.
   GLboolean              glLighting_;
   /// L'attribut GL_LIGHT_MODEL_AMBIENT.
   GLfloat                glLightModelAmbient_[4];
   /// L'attribut GL_LIGHT_MODEL_LOCAL_VIEWER.
   GLboolean              glLightModelLocalViewer_;
   /// L'attribut GL_LIGHT_MODEL_TWO_SIDE.
   GLboolean              glLightModelTwoSide_;
   /// L'attribut GL_LINE_SMOOTH.
   GLboolean              glLineSmooth_;
   /// L'attribut GL_LINE_SMOOTH_HINT.
   GLint                  glLineSmoothHint_;
   /// L'attribut GL_LINE_STIPPLE.
   GLboolean              glLineStipple_;
   /// L'attribut GL_LINE_STIPPLE_PATTERN.
   GLint                  glLineStipplePattern_;
   /// L'attribut GL_LINE_STIPPLE_REPEAT.
   GLint                  glLineStippleRepeat_;
   /// L'attribut GL_LINE_WIDTH.
   GLint                  glLineWidth_;
   /// L'attribut GL_LINE_WIDTH_GRANULARITY.
   GLint                  glLineWidthGranularity_;
   /// L'attribut GL_LINE_WIDTH_RANGE.
   GLint                  gllineWidthRange_[2];
   /// L'attribut GL_LIST_BASE.
   GLint                  glListBase_;
   /// L'attribut GL_LIST_INDEX.
   GLint                  glListIndex_;
   /// L'attribut GL_LIST_MODE.
   GLint                  glListMode_;
   /// L'attribut GL_LOGIC_OP_MODE.
   GLint                  glLogicOpMode_;
   /// L'attribut GL_MAP1_COLOR_4.
   GLboolean              glMap1Color4_;
   /// L'attribut GL_MAP1_GRID_DOMAIN.
   GLfloat                glMap1GridDomain_[2];
   /// L'attribut GL_MAP1_GRID_SEGMENTS.
   GLint                  glMap1GridSegments_;
   /// L'attribut GL_MAP1_INDEX.
   GLboolean              glMap1Index_;
   /// L'attribut GL_MAP1_NORMAL.
   GLboolean              glMap1Normal_;
   /// L'attribut GL_MAP1_TEXTURE_COORD_1.
   GLboolean              glMap1TextureCoord1_;
   /// L'attribut GL_MAP1_TEXTURE_COORD_2.
   GLboolean              glMap1TextureCoord2_;
   /// L'attribut GL_MAP1_TEXTURE_COORD_3.
   GLboolean              glMap1TextureCoord3_;
   /// L'attribut GL_MAP1_TEXTURE_COORD_4.
   GLboolean              glMap1TextureCoord4_;
   /// L'attribut GL_MAP1_VERTEX_3.
   GLboolean              glMap1Vertex3_;
   /// L'attribut GL_MAP1_VERTEX_4.
   GLboolean              glMap1Vertex4_;
   /// L'attribut GL_MAP2_COLOR_4.
   GLboolean              glMap2Color4_;
   /// L'attribut GL_MAP2_GRID_DOMAIN.
   GLfloat                glMap2GridDomain_[4];
   /// L'attribut GL_MAP2_GRID_SEGMENTS.
   GLint                  glMap2GridSegments_[2];
   /// L'attribut GL_MAP2_INDEX.
   GLboolean              glMap2Index_;
   /// L'attribut GL_MAP2_NORMAL.
   GLboolean              glMap2Normal_;
   /// L'attribut GL_MAP2_TEXTURE_COORD_1.
   GLboolean              glMap2TextureCoord1_;
   /// L'attribut GL_MAP2_TEXTURE_COORD_2.
   GLboolean              glMap2TextureCoord2_;
   /// L'attribut GL_MAP2_TEXTURE_COORD_3.
   GLboolean              glMap2TextureCoord3_;
   /// L'attribut GL_MAP2_TEXTURE_COORD_4.
   GLboolean              glMap2TextureCoord4_;
   /// L'attribut GL_MAP2_VERTEX_3.
   GLboolean              glMap2Vertex3_;
   /// L'attribut GL_MAP2_VERTEX_4.
   GLboolean              glMap2Vertex4_;
   /// L'attribut GL_MAP_COLOR.
   GLboolean              glMapColor_;
   /// L'attribut GL_MAP_STENCIL.
   GLboolean              glMapStencil_;
   /// L'attribut GL_MATRIX_MODE.
   GLint                  glMatrixMode_;
   /// L'attribut GL_MAX_CLIENT_ATTRIB_STACK_DEPTH.
   GLint                  glMaxClientAttribStackDepth_;
   /// L'attribut GL_MAX_ATTRIB_STACK_DEPTH.
   GLint                  glMaxAttribStackDepth_;
   /// L'attribut GL_MAX_CLIP_PLANES.
   GLint                  glMaxClipPlanes_;
   /// L'attribut GL_MAX_EVAL_ORDER.
   GLint                  glMaxEvalOrder_;
   /// L'attribut GL_MAX_LIGHTS.
   GLint                  glMaxLights_;
   /// L'attribut GL_MAX_LIST_NESTING.
   GLint                  glMaxListNesting_;
   /// L'attribut GL_MAX_MODELVIEW_STACK_DEPTH.
   GLint                  glMaxModelviewStackDepth_;
   /// L'attribut GL_MAX_NAME_STACK_DEPTH.
   GLint                  glMaxNameStackDepth_;
   /// L'attribut GL_MAX_PIXEL_MAP_TABLE.
   GLint                  glMaxPixelMapTable_;
   /// L'attribut GL_MAX_PROJECTION_STACK_DEPTH.
   GLint                  glMaxProjectionStackDepth_;
   /// L'attribut GL_MAX_TEXTURE_SIZE.
   GLint                  glMaxTextureSize_;
   /// L'attribut GL_MAX_TEXTURE_STACK_DEPTH.
   GLint                  glMaxTextureStackDepth_;
   /// L'attribut GL_MAX_VIEWPORT_DIMS.
   GLint                  glMaxViewportDims_[2];
   /// L'attribut GL_MODELVIEW_MATRIX.
   GLfloat                glModelviewMatrix_[16];
   /// L'attribut GL_MODELVIEW_STACK_DEPTH.
   GLint                  glModelviewStackDepth_;
   /// L'attribut GL_NAME_STACK_DEPTH.
   GLint                  glNameStackDepth_;
   /// L'attribut GL_NORMAL_ARRAY.
   GLboolean              glNormalArray_;
   /// L'attribut GL_NORMAL_ARRAY_STRIDE.
   GLint                  glNormalArrayStride_;
   /// L'attribut GL_NORMAL_ARRAY_TYPE.
   GLint                  glNormalArrayType_;
   /// L'attribut GL_NORMALIZE.
   GLboolean              glNormalize_;
   /// L'attribut GL_PACK_ALIGNMENT.
   GLint                  glPackAlignment_;
   /// L'attribut GL_PACK_LSB_FIRST.
   GLboolean              glPackLsbFirst_;
   /// L'attribut GL_PACK_ROW_LENGTH.
   GLint                  glPackRowLength_;
   /// L'attribut GL_PACK_SKIP_PIXELS.
   GLint                  glPackSkipPixels_;
   /// L'attribut GL_PACK_SKIP_ROWS.
   GLint                  glPackSkipRows_;
   /// L'attribut GL_PACK_SWAP_BYTES.
   GLboolean              glPackSwapBytes_;
   /// L'attribut GL_PERSPECTIVE_CORRECTION_HINT.
   GLint                  glPerspectiveCorrectionHint_;
   /// L'attribut GL_PIXEL_MAP_A_TO_A_SIZE.
   GLint                  glPixelMapAToASize_;
   /// L'attribut GL_PIXEL_MAP_B_TO_B_SIZE.
   GLint                  glPixelMapBToBSize_;
   /// L'attribut GL_PIXEL_MAP_G_TO_G_SIZE.
   GLint                  glPixelMapGToGSize_;
   /// L'attribut GL_PIXEL_MAP_I_TO_A_SIZE.
   GLint                  glPixelMapIToASize_;
   /// L'attribut GL_PIXEL_MAP_I_TO_B_SIZE.
   GLint                  glPixelMapIToBSize_;
   /// L'attribut GL_PIXEL_MAP_I_TO_G_SIZE.
   GLint                  glPixelMapIToGSize_;
   /// L'attribut GL_PIXEL_MAP_I_TO_I_SIZE.
   GLint                  glPixelMapIToISize_;
   /// L'attribut GL_PIXEL_MAP_I_TO_R_SIZE.
   GLint                  glPixelMapIToRSize_;
   /// L'attribut GL_PIXEL_MAP_R_TO_R_SIZE.
   GLint                  glPixelMapRToRSize_;
   /// L'attribut GL_PIXEL_MAP_S_TO_S_SIZE.
   GLint                  glPixelMapSToSSize_;
   /// L'attribut GL_POINT_SIZE.
   GLint                  glPointSize_;
   /// L'attribut GL_POINT_SIZE_GRANULARITY.
   GLint                  glPointSizeGranularity_;
   /// L'attribut GL_POINT_SIZE_RANGE.
   GLint                  glPointSizeRange_[2];
   /// L'attribut GL_POINT_SMOOTH.
   GLboolean              glPointSmooth_;
   /// L'attribut GL_POINT_SMOOTH_HINT.
   GLint                  glPointSmoothHint_;
   /// L'attribut GL_POLYGON_MODE.
   GLint                  glPolygonMode_[2];
   /// L'attribut GL_POLYGON_OFFSET_FACTOR.
   GLfloat                glPolygonOffsetFactor_;
   /// L'attribut GL_POLYGON_OFFSET_UNITS.
   GLfloat                glPolygonOffsetUnits_;
   /// L'attribut GL_POLYGON_OFFSET_FILL.
   GLboolean              glPolygonOffsetFill_;
   /// L'attribut GL_POLYGON_OFFSET_LINE.
   GLboolean              glPolygonOffsetLine_;
   /// L'attribut GL_POLYGON_OFFSET_POINT.
   GLboolean              glPolygonOffsetPoint_;
   /// L'attribut GL_POLYGON_SMOOTH.
   GLboolean              glPolygonSmooth_;
   /// L'attribut GL_POLYGON_SMOOTH_HINT.
   GLint                  glPolygonSmoothHint_;
   /// L'attribut GL_POLYGON_STIPPLE.
   GLboolean              glPolygonStipple_;
   /// L'attribut GL_PROJECTION_MATRIX.
   GLfloat                glProjectionMatrix_[16];
   /// L'attribut GL_PROJECTION_STACK_DEPTH.
   GLint                  glProjectionStackDepth_;
   /// L'attribut GL_READ_BUFFER.
   GLint                  glReadBuffer_;
   /// L'attribut GL_RED_BIAS.
   GLfloat                glRedBias_;
   /// L'attribut GL_RED_BITS.
   GLint                  glRedBits_;
   /// L'attribut GL_RED_SCALE.
   GLfloat                glRedScale_;
   /// L'attribut GL_RENDER_MODE.
   GLint                  glRenderMode_;
   /// L'attribut GL_RGBA_MODE.
   GLboolean              glRgbaMode_;
   /// L'attribut GL_SCISSOR_BOX.
   GLint                  glScissorBox_[4];
   /// L'attribut GL_SCISSOR_TEST.
   GLboolean              glScissorTest_;
   /// L'attribut GL_SELECTION_BUFFER_SIZE.
   GLint                  glSelectionBufferSize_;
   /// L'attribut GL_SHADE_MODEL.
   GLint                  glShadeModel_;
   /// L'attribut GL_STENCIL_BITS.
   GLint                  glStencilBits_;
   /// L'attribut GL_STENCIL_CLEAR_VALUE.
   GLint                  glStencilClearValue_;
   /// L'attribut GL_STENCIL_FAIL.
   GLint                  glStencilFail_;
   /// L'attribut GL_STENCIL_FUNC.
   GLint                  glStencilFunc_;
   /// L'attribut GL_STENCIL_PASS_DEPTH_FAIL.
   GLint                  glStencilPassDepthFail_;
   /// L'attribut GL_STENCIL_PASS_DEPTH_PASS.
   GLint                  glStencilPassDepthPass_;
   /// L'attribut GL_STENCIL_REF.
   GLint                  glStencilRef_;
   /// L'attribut GL_STENCIL_TEST.
   GLboolean              glStencilTest_;
   /// L'attribut GL_STENCIL_VALUE_MASK.
   GLint                  glStencilValueMask_;
   /// L'attribut GL_STENCIL_WRITEMASK.
   GLint                  glStencilWritemask_;
   /// L'attribut GL_STEREO.
   GLboolean              glStereo_;
   /// L'attribut GL_SUBPIXEL_BITS.
   GLint                  glSubpixelBits_;
   /// L'attribut GL_TEXTURE_1D.
   GLboolean              glTexture1D_;
   /// L'attribut GL_TEXTURE_BINDING_1D.
   GLint                  glTextureBinding1D_;
   /// L'attribut GL_TEXTURE_2D.
   GLboolean              glTexture2D_;
   /// L'attribut GL_TEXTURE_BINDING_2D.
   GLint                  glTextureBinding2D_;
   /// L'attribut GL_TEXTURE_COORD_ARRAY.
   GLboolean              glTextureCoordArray_;
   /// L'attribut GL_TEXTURE_COORD_ARRAY_SIZE.
   GLint                  glTextureCoordArraySize_;
   /// L'attribut GL_TEXTURE_COORD_ARRAY_STRIDE.
   GLint                  glTextureCoordArrayStride_;
   /// L'attribut GL_TEXTURE_COORD_ARRAY_TYPE.
   GLint                  glTextureCoordArrayType_;
   /// L'attribut GL_TEXTURE_GEN_Q.
   GLboolean              glTextureGenQ_;
   /// L'attribut GL_TEXTURE_GEN_R.
   GLboolean              glTextureGenR_;
   /// L'attribut GL_TEXTURE_GEN_S.
   GLboolean              glTextureGenS_;
   /// L'attribut GL_TEXTURE_GEN_T.
   GLboolean              glTextureGenT_;
   /// L'attribut GL_TEXTURE_MATRIX.
   GLfloat                glTextureMatrix_[16];
   /// L'attribut GL_TEXTURE_STACK_DEPTH.
   GLint                  glTextureStackDepth_;
   /// L'attribut GL_UNPACK_ALIGNMENT.
   GLint                  glUnpackAlignment_;
   /// L'attribut GL_UNPACK_LSB_FIRST.
   GLboolean              glUnpackLsbFirst_;
   /// L'attribut GL_UNPACK_ROW_LENGTH.
   GLint                  glUnpackRowLength_;
   /// L'attribut GL_UNPACK_SKIP_PIXELS.
   GLint                  glUnpackSkipPixels_;
   /// L'attribut GL_UNPACK_SKIP_ROWS.
   GLint                  glUnpackSkipRows_;
   /// L'attribut GL_UNPACK_SWAP_BYTES.
   GLboolean              glUnpackSwapBytes_;
   /// L'attribut GL_VERTEX_ARRAY.
   GLboolean              glVertexArray_;
   /// L'attribut GL_VERTEX_ARRAY_SIZE.
   GLint                  glVertexArraySize_;
   /// L'attribut GL_VERTEX_ARRAY_STRIDE.
   GLint                  glVertexArrayStride_;
   /// L'attribut GL_VERTEX_ARRAY_TYPE.
   GLint                  glVertexArrayType_;
   /// L'attribut GL_VIEWPORT.
   GLint                  glViewport_[4];
   /// L'attribut GL_ZOOM_X.
   GLfloat                glZoomX_;
   /// L'attribut GL_ZOOM_Y.
   GLfloat                glZoomY_;

};




///////////////////////////////////////////////////////////////////////////////
///
/// @fn template<class T> std::string EtatOpenGL::obtenirChaineVecteur(const T* v, int size)
///
/// Cette fonction retourne une chaîne représentant un attribut qui est un
/// tableau.
///
/// @param[in] v    : Le tableau d'attributs.
/// @param[in] size : Le nombre d'attributs dans le tableau.
///
/// @return Une chaîne représentant le tableau d'attributs.
///
///////////////////////////////////////////////////////////////////////////////
template<class T>
std::string EtatOpenGL::obtenirChaineVecteur(const T* v, int size)
{
   std::ostringstream o;
   o << "( ";
   for (int i = 0 ; i < size; ++i) {
      if (i != 0)
         o << " , ";
      o << utilitaire::convertirEnChaine(v[i]);
   }
   o << " )";
   return o.str();
}


///////////////////////////////////////////////////////////////////////////////
///
/// @fn inline std::string EtatOpenGL::obtenirChaineBooleen(GLboolean v)
///
/// Cette fonction retourne une chaîne représentant un attribut qui est une
/// valeur booléenne.
///
/// @param[in] v : L'attribut booléen.
///
/// @return Une chaîne représentant l'attribut booléen.
///
///////////////////////////////////////////////////////////////////////////////
inline std::string EtatOpenGL::obtenirChaineBooleen(GLboolean v)
{
   if (v)
      return "GL_TRUE";
   else
      return "GL_FALSE";
}


// Opérateur d'extraction de flux.
std::ostream& operator <<(std::ostream& o , const EtatOpenGL& etat);


#endif // __UTILITAIRE_ETATOPENGL_H__


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
