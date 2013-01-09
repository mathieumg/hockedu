//////////////////////////////////////////////////////////////////////////////
/// @file EtatOpenGL.cpp
/// @author Martin Bisson
/// @date 2007-07-27
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
//////////////////////////////////////////////////////////////////////////////
#include "EtatOpenGL.h"


/// Cette macro permet de retourner la chaîne associée à une valeur.
#define COMPARER_VALEUR(x,chaine) if (x == chaine) return #chaine ;
/// Cette macro retourne la valeur par défaut de la variable.
#define COMPARER_DEFAUT(x) return "GL_??? (" + utilitaire::convertirEnChaine(x) + ")";



///////////////////////////////////////////////////////////////////////////////
///
/// @fn EtatOpenGL::EtatOpenGL()
///
/// Ce constructeur lit toutes ses variables de l'état courant de la machine
/// OpenGL.
///
/// @return Aucune (constructeur).
///
///////////////////////////////////////////////////////////////////////////////
EtatOpenGL::EtatOpenGL()
{
   // On veut initialiser toutes les variables à 0, mais on ne veut pas
   // briser les objets vecteurs.  On les désalloue donc, on met toute la
   // mémoire à zéro puis on réinitialise.
   glClipPlanei_.~vector();
   glLighti_.~vector();
   memset(this, 0, sizeof(EtatOpenGL));
   new (&glClipPlanei_) std::vector<GLboolean>();
   new (&glLighti_) std::vector<GLboolean>();

   glGetIntegerv(GL_ACCUM_ALPHA_BITS , &glAccumAlphaBits_);
   glGetIntegerv(GL_ACCUM_BLUE_BITS , &glAccumBlueBits_);
   glGetFloatv(GL_ACCUM_CLEAR_VALUE , glAccumClearValue_);
   glGetIntegerv(GL_ACCUM_GREEN_BITS , &glAccumGreenBits_);
   glGetIntegerv(GL_ACCUM_RED_BITS , &glAccumRedBits_);
   glGetFloatv(GL_ALPHA_BIAS , &glAlphaBias_);
   glGetIntegerv(GL_ALPHA_BITS , &glAlphaBits_);
   glGetFloatv(GL_ALPHA_SCALE , &glAlphaScale_);
   glGetBooleanv(GL_ALPHA_TEST , &glAlphaTest_);
   glGetIntegerv(GL_ALPHA_TEST_FUNC , &glAlphaTestFunc_);
   glGetFloatv(GL_ALPHA_TEST_REF , &glAlphaTestRef_);
   glGetIntegerv(GL_ATTRIB_STACK_DEPTH , &glAttribStackDepth_);
   glGetBooleanv(GL_AUTO_NORMAL , &glAutoNormal_);
   glGetIntegerv(GL_AUX_BUFFERS , &glAuxBuffers_);

   glGetBooleanv(GL_BLEND , &glBlend_);
   glGetIntegerv(GL_BLEND_DST , &glBlendDst_);
   glGetIntegerv(GL_BLEND_SRC , &glBlendSrc_);
   glGetFloatv(GL_BLUE_BIAS , &glBlueBias_);
   glGetIntegerv(GL_BLUE_BITS , &glBlueBits_);
   glGetFloatv(GL_BLUE_SCALE , &glBlueScale_);

   glGetIntegerv(GL_CLIENT_ATTRIB_STACK_DEPTH , &glClientAttribStackDepth_);
   // On saute GL_CLIP_PLANEi.
   glGetBooleanv(GL_COLOR_ARRAY , &glColorArray_);
   glGetIntegerv(GL_COLOR_ARRAY_SIZE , &glColorArraySize_);
   glGetIntegerv(GL_COLOR_ARRAY_STRIDE , &glColorArrayStride_);
   glGetIntegerv(GL_COLOR_ARRAY_TYPE , &glColorArrayType_);
   glGetFloatv(GL_COLOR_CLEAR_VALUE , glColorClearValue_);
   glGetBooleanv(GL_COLOR_LOGIC_OP , &glColorLogicOp_);
   glGetBooleanv(GL_COLOR_MATERIAL , &glColorMaterial_);
   glGetIntegerv(GL_COLOR_MATERIAL_FACE , &glColorMaterialFace_);
   glGetIntegerv(GL_COLOR_MATERIAL_PARAMETER , &glColorMaterialParameter_);
   glGetBooleanv(GL_COLOR_WRITEMASK , glColorWritemask_);
   glGetBooleanv(GL_CULL_FACE , &glCullFace_);
   glGetIntegerv(GL_CULL_FACE_MODE , &glCullFaceMode_);
   glGetFloatv(GL_CURRENT_COLOR , glCurrentColor_);
   glGetIntegerv(GL_CURRENT_INDEX , &glCurrentIndex_);
   glGetFloatv(GL_CURRENT_NORMAL , glCurrentNormal_);
   glGetFloatv(GL_CURRENT_RASTER_COLOR , glCurrentRasterColor_);
   glGetFloatv(GL_CURRENT_RASTER_DISTANCE , &glCurrentRasterDistance_);
   glGetIntegerv(GL_CURRENT_RASTER_INDEX , &glCurrentRasterIndex_);
   glGetFloatv(GL_CURRENT_RASTER_POSITION , glCurrentRasterPosition_);
   glGetBooleanv(GL_CURRENT_RASTER_POSITION_VALID , &glCurrentRasterPositionValid_);
   glGetFloatv(GL_CURRENT_RASTER_TEXTURE_COORDS , glCurrentRasterTextureCoords_);
   glGetFloatv(GL_CURRENT_TEXTURE_COORDS , glCurrentTextureCoords_);

   glGetFloatv(GL_DEPTH_BIAS , &glDepthBias_);
   glGetIntegerv(GL_DEPTH_BITS , &glDepthBits_);
   glGetFloatv(GL_DEPTH_CLEAR_VALUE , &glDepthClearValue_);
   glGetIntegerv(GL_DEPTH_FUNC , &glDepthFunc_);
   glGetFloatv(GL_DEPTH_RANGE , glDepthRange_);
   glGetFloatv(GL_DEPTH_SCALE , &glDepthScale_);
   glGetBooleanv(GL_DEPTH_TEST , &glDepthTest_);
   glGetBooleanv(GL_DEPTH_WRITEMASK , &glDepthWritemask_);
   glGetBooleanv(GL_DITHER , &glDither_);
   glGetBooleanv(GL_DOUBLEBUFFER , &glDoublebuffer_);
   glGetIntegerv(GL_DRAW_BUFFER , &glDrawBuffer_);

   glGetBooleanv(GL_EDGE_FLAG , &glEdgeFlag_);
   glGetBooleanv(GL_EDGE_FLAG_ARRAY , &glEdgeFlagArray_);
   glGetIntegerv(GL_EDGE_FLAG_ARRAY_STRIDE , &glEdgeFlagArrayStride_);

   glGetIntegerv(GL_FEEDBACK_BUFFER_SIZE , &glFeedbackBufferSize_);
   glGetIntegerv(GL_FEEDBACK_BUFFER_TYPE , &glFeedbackBufferType_);

   glGetBooleanv(GL_FOG , &glFog_);
   glGetFloatv(GL_FOG_COLOR , glFogColor_);
   glGetFloatv(GL_FOG_DENSITY , &glFogDensity_);
   glGetFloatv(GL_FOG_END , &glFogEnd_);
   glGetIntegerv(GL_FOG_HINT , &glFogHint_);
   glGetIntegerv(GL_FOG_INDEX , &glFogIndex_);
   glGetIntegerv(GL_FOG_MODE , &glFogMode_);
   glGetFloatv(GL_FOG_START , &glFogStart_);
   glGetIntegerv(GL_FRONT_FACE , &glFrontFace_);

   glGetFloatv(GL_GREEN_BIAS , &glGreenBias_);
   glGetIntegerv(GL_GREEN_BITS , &glGreenBits_);
   glGetFloatv(GL_GREEN_SCALE , &glGreenScale_);

   glGetBooleanv(GL_INDEX_ARRAY , &glIndexArray_);
   glGetIntegerv(GL_INDEX_ARRAY_STRIDE , &glIndexArrayStride_);
   glGetIntegerv(GL_INDEX_ARRAY_TYPE , &glIndexArrayType_);
   glGetIntegerv(GL_INDEX_BITS , &glIndexBits_);
   glGetIntegerv(GL_INDEX_CLEAR_VALUE , &glIndexClearValue_);
   glGetBooleanv(GL_INDEX_LOGIC_OP , &glIndexLogicOp_);
   glGetBooleanv(GL_INDEX_MODE , &glIndexMode_);
   glGetIntegerv(GL_INDEX_OFFSET , &glIndexOffset_);
   glGetIntegerv(GL_INDEX_SHIFT , &glIndexShift_);
   glGetIntegerv(GL_INDEX_WRITEMASK , &glIndexWritemask_);

   // On saute GL_LIGHTi.
   glGetBooleanv(GL_LIGHTING , &glLighting_);
   glGetFloatv(GL_LIGHT_MODEL_AMBIENT , glLightModelAmbient_);
   glGetBooleanv(GL_LIGHT_MODEL_LOCAL_VIEWER , &glLightModelLocalViewer_);
   glGetBooleanv(GL_LIGHT_MODEL_TWO_SIDE , &glLightModelTwoSide_);
   glGetBooleanv(GL_LINE_SMOOTH , &glLineSmooth_);
   glGetIntegerv(GL_LINE_SMOOTH_HINT , &glLineSmoothHint_);
   glGetBooleanv(GL_LINE_STIPPLE , &glLineStipple_);
   glGetIntegerv(GL_LINE_STIPPLE_PATTERN , &glLineStipplePattern_);
   glGetIntegerv(GL_LINE_STIPPLE_REPEAT , &glLineStippleRepeat_);
   glGetIntegerv(GL_LINE_WIDTH , &glLineWidth_);
   glGetIntegerv(GL_LINE_WIDTH_GRANULARITY , &glLineWidthGranularity_);
   glGetIntegerv(GL_LINE_WIDTH_RANGE , gllineWidthRange_);
   glGetIntegerv(GL_LIST_BASE , &glListBase_);
   glGetIntegerv(GL_LIST_INDEX , &glListIndex_);
   glGetIntegerv(GL_LIST_MODE , &glListMode_);
   glGetIntegerv(GL_LOGIC_OP_MODE , &glLogicOpMode_);

   glGetBooleanv(GL_MAP1_COLOR_4 , &glMap1Color4_);
   glGetFloatv(GL_MAP1_GRID_DOMAIN , glMap1GridDomain_);
   glGetIntegerv(GL_MAP1_GRID_SEGMENTS , &glMap1GridSegments_);
   glGetBooleanv(GL_MAP1_INDEX , &glMap1Index_);
   glGetBooleanv(GL_MAP1_NORMAL , &glMap1Normal_);
   glGetBooleanv(GL_MAP1_TEXTURE_COORD_1 , &glMap1TextureCoord1_);
   glGetBooleanv(GL_MAP1_TEXTURE_COORD_2 , &glMap1TextureCoord2_);
   glGetBooleanv(GL_MAP1_TEXTURE_COORD_3 , &glMap1TextureCoord3_);
   glGetBooleanv(GL_MAP1_TEXTURE_COORD_4 , &glMap1TextureCoord4_);
   glGetBooleanv(GL_MAP1_VERTEX_3 , &glMap1Vertex3_);
   glGetBooleanv(GL_MAP1_VERTEX_4 , &glMap1Vertex4_);
   glGetBooleanv(GL_MAP2_COLOR_4 , &glMap2Color4_);
   glGetFloatv(GL_MAP2_GRID_DOMAIN , glMap2GridDomain_);
   glGetIntegerv(GL_MAP2_GRID_SEGMENTS , glMap2GridSegments_);
   glGetBooleanv(GL_MAP2_INDEX , &glMap2Index_);
   glGetBooleanv(GL_MAP2_NORMAL , &glMap2Normal_);
   glGetBooleanv(GL_MAP2_TEXTURE_COORD_1 , &glMap2TextureCoord1_);
   glGetBooleanv(GL_MAP2_TEXTURE_COORD_2 , &glMap2TextureCoord2_);
   glGetBooleanv(GL_MAP2_TEXTURE_COORD_3 , &glMap2TextureCoord3_);
   glGetBooleanv(GL_MAP2_TEXTURE_COORD_4 , &glMap2TextureCoord4_);
   glGetBooleanv(GL_MAP2_VERTEX_3 , &glMap2Vertex3_);
   glGetBooleanv(GL_MAP2_VERTEX_4 , &glMap2Vertex4_);
   glGetBooleanv(GL_MAP_COLOR , &glMapColor_);
   glGetBooleanv(GL_MAP_STENCIL , &glMapStencil_);
   glGetIntegerv(GL_MATRIX_MODE , &glMatrixMode_);
   glGetIntegerv(GL_MAX_CLIENT_ATTRIB_STACK_DEPTH , &glMaxClientAttribStackDepth_);
   glGetIntegerv(GL_MAX_ATTRIB_STACK_DEPTH , &glMaxAttribStackDepth_);
   glGetIntegerv(GL_MAX_CLIP_PLANES , &glMaxClipPlanes_);
   glGetIntegerv(GL_MAX_EVAL_ORDER , &glMaxEvalOrder_);
   glGetIntegerv(GL_MAX_LIGHTS , &glMaxLights_);
   glGetIntegerv(GL_MAX_LIST_NESTING , &glMaxListNesting_);
   glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH , &glMaxModelviewStackDepth_);
   glGetIntegerv(GL_MAX_NAME_STACK_DEPTH , &glMaxNameStackDepth_);
   glGetIntegerv(GL_MAX_PIXEL_MAP_TABLE , &glMaxPixelMapTable_);
   glGetIntegerv(GL_MAX_PROJECTION_STACK_DEPTH , &glMaxProjectionStackDepth_);
   glGetIntegerv(GL_MAX_TEXTURE_SIZE , &glMaxTextureSize_);
   glGetIntegerv(GL_MAX_TEXTURE_STACK_DEPTH , &glMaxTextureStackDepth_);
   glGetIntegerv(GL_MAX_VIEWPORT_DIMS , glMaxViewportDims_);
   glGetFloatv(GL_MODELVIEW_MATRIX , glModelviewMatrix_);
   glGetIntegerv(GL_MODELVIEW_STACK_DEPTH , &glModelviewStackDepth_);

   glGetIntegerv(GL_NAME_STACK_DEPTH , &glNameStackDepth_);
   glGetBooleanv(GL_NORMAL_ARRAY , &glNormalArray_);
   glGetIntegerv(GL_NORMAL_ARRAY_STRIDE , &glNormalArrayStride_);
   glGetIntegerv(GL_NORMAL_ARRAY_TYPE , &glNormalArrayType_);
   glGetBooleanv(GL_NORMALIZE , &glNormalize_);

   glGetIntegerv(GL_PACK_ALIGNMENT , &glPackAlignment_);
   glGetBooleanv(GL_PACK_LSB_FIRST , &glPackLsbFirst_);
   glGetIntegerv(GL_PACK_ROW_LENGTH , &glPackRowLength_);
   glGetIntegerv(GL_PACK_SKIP_PIXELS , &glPackSkipPixels_);
   glGetIntegerv(GL_PACK_SKIP_ROWS , &glPackSkipRows_);
   glGetBooleanv(GL_PACK_SWAP_BYTES , &glPackSwapBytes_);
   glGetIntegerv(GL_PERSPECTIVE_CORRECTION_HINT , &glPerspectiveCorrectionHint_);
   glGetIntegerv(GL_PIXEL_MAP_A_TO_A_SIZE , &glPixelMapAToASize_);
   glGetIntegerv(GL_PIXEL_MAP_B_TO_B_SIZE , &glPixelMapBToBSize_);
   glGetIntegerv(GL_PIXEL_MAP_G_TO_G_SIZE , &glPixelMapGToGSize_);
   glGetIntegerv(GL_PIXEL_MAP_I_TO_A_SIZE , &glPixelMapIToASize_);
   glGetIntegerv(GL_PIXEL_MAP_I_TO_B_SIZE , &glPixelMapIToBSize_);
   glGetIntegerv(GL_PIXEL_MAP_I_TO_G_SIZE , &glPixelMapIToGSize_);
   glGetIntegerv(GL_PIXEL_MAP_I_TO_I_SIZE , &glPixelMapIToISize_);
   glGetIntegerv(GL_PIXEL_MAP_I_TO_R_SIZE , &glPixelMapIToRSize_);
   glGetIntegerv(GL_PIXEL_MAP_R_TO_R_SIZE , &glPixelMapRToRSize_);
   glGetIntegerv(GL_PIXEL_MAP_S_TO_S_SIZE , &glPixelMapSToSSize_);
   glGetIntegerv(GL_POINT_SIZE , &glPointSize_);
   glGetIntegerv(GL_POINT_SIZE_GRANULARITY , &glPointSizeGranularity_);
   glGetIntegerv(GL_POINT_SIZE_RANGE , glPointSizeRange_);
   glGetBooleanv(GL_POINT_SMOOTH , &glPointSmooth_);
   glGetIntegerv(GL_POINT_SMOOTH_HINT , &glPointSmoothHint_);
   glGetIntegerv(GL_POLYGON_MODE , glPolygonMode_);
   glGetFloatv(GL_POLYGON_OFFSET_FACTOR , &glPolygonOffsetFactor_);
   glGetFloatv(GL_POLYGON_OFFSET_UNITS , &glPolygonOffsetUnits_);
   glGetBooleanv(GL_POLYGON_OFFSET_FILL , &glPolygonOffsetFill_);
   glGetBooleanv(GL_POLYGON_OFFSET_LINE , &glPolygonOffsetLine_);
   glGetBooleanv(GL_POLYGON_OFFSET_POINT , &glPolygonOffsetPoint_);
   glGetBooleanv(GL_POLYGON_SMOOTH , &glPolygonSmooth_);
   glGetIntegerv(GL_POLYGON_SMOOTH_HINT , &glPolygonSmoothHint_);
   glGetBooleanv(GL_POLYGON_STIPPLE , &glPolygonStipple_);
   glGetFloatv(GL_PROJECTION_MATRIX , glProjectionMatrix_);
   glGetIntegerv(GL_PROJECTION_STACK_DEPTH , &glProjectionStackDepth_);

   glGetIntegerv(GL_READ_BUFFER , &glReadBuffer_);
   glGetFloatv(GL_RED_BIAS , &glRedBias_);
   glGetIntegerv(GL_RED_BITS , &glRedBits_);
   glGetFloatv(GL_RED_SCALE , &glRedScale_);
   glGetIntegerv(GL_RENDER_MODE , &glRenderMode_);
   glGetBooleanv(GL_RGBA_MODE , &glRgbaMode_);

   glGetIntegerv(GL_SCISSOR_BOX , glScissorBox_);
   glGetBooleanv(GL_SCISSOR_TEST , &glScissorTest_);
   glGetIntegerv(GL_SELECTION_BUFFER_SIZE , &glSelectionBufferSize_);
   glGetIntegerv(GL_SHADE_MODEL , &glShadeModel_);
   glGetIntegerv(GL_STENCIL_BITS , &glStencilBits_);
   glGetIntegerv(GL_STENCIL_CLEAR_VALUE , &glStencilClearValue_);
   glGetIntegerv(GL_STENCIL_FAIL , &glStencilFail_);
   glGetIntegerv(GL_STENCIL_FUNC , &glStencilFunc_);
   glGetIntegerv(GL_STENCIL_PASS_DEPTH_FAIL , &glStencilPassDepthFail_);
   glGetIntegerv(GL_STENCIL_PASS_DEPTH_PASS , &glStencilPassDepthPass_);
   glGetIntegerv(GL_STENCIL_REF , &glStencilRef_);
   glGetBooleanv(GL_STENCIL_TEST , &glStencilTest_);
   glGetIntegerv(GL_STENCIL_VALUE_MASK , &glStencilValueMask_);
   glGetIntegerv(GL_STENCIL_WRITEMASK , &glStencilWritemask_);
   glGetBooleanv(GL_STEREO , &glStereo_);
   glGetIntegerv(GL_SUBPIXEL_BITS , &glSubpixelBits_);

   glGetBooleanv(GL_TEXTURE_1D , &glTexture1D_);
   glGetIntegerv(GL_TEXTURE_BINDING_1D , &glTextureBinding1D_);
   glGetBooleanv(GL_TEXTURE_2D , &glTexture2D_);
   glGetIntegerv(GL_TEXTURE_BINDING_2D , &glTextureBinding2D_);
   glGetBooleanv(GL_TEXTURE_COORD_ARRAY , &glTextureCoordArray_);
   glGetIntegerv(GL_TEXTURE_COORD_ARRAY_SIZE , &glTextureCoordArraySize_);
   glGetIntegerv(GL_TEXTURE_COORD_ARRAY_STRIDE , &glTextureCoordArrayStride_);
   glGetIntegerv(GL_TEXTURE_COORD_ARRAY_TYPE , &glTextureCoordArrayType_);
   glGetBooleanv(GL_TEXTURE_GEN_Q , &glTextureGenQ_);
   glGetBooleanv(GL_TEXTURE_GEN_R , &glTextureGenR_);
   glGetBooleanv(GL_TEXTURE_GEN_S , &glTextureGenS_);
   glGetBooleanv(GL_TEXTURE_GEN_T , &glTextureGenT_);
   glGetFloatv(GL_TEXTURE_MATRIX , glTextureMatrix_);
   glGetIntegerv(GL_TEXTURE_STACK_DEPTH , &glTextureStackDepth_);

   glGetIntegerv(GL_UNPACK_ALIGNMENT , &glUnpackAlignment_);
   glGetBooleanv(GL_UNPACK_LSB_FIRST , &glUnpackLsbFirst_);
   glGetIntegerv(GL_UNPACK_ROW_LENGTH , &glUnpackRowLength_);
   glGetIntegerv(GL_UNPACK_SKIP_PIXELS , &glUnpackSkipPixels_);
   glGetIntegerv(GL_UNPACK_SKIP_ROWS , &glUnpackSkipRows_);
   glGetBooleanv(GL_UNPACK_SWAP_BYTES , &glUnpackSwapBytes_);

   glGetBooleanv(GL_VERTEX_ARRAY , &glVertexArray_);
   glGetIntegerv(GL_VERTEX_ARRAY_SIZE , &glVertexArraySize_);
   glGetIntegerv(GL_VERTEX_ARRAY_STRIDE , &glVertexArrayStride_);
   glGetIntegerv(GL_VERTEX_ARRAY_TYPE , &glVertexArrayType_);
   glGetIntegerv(GL_VIEWPORT , glViewport_);

   glGetFloatv(GL_ZOOM_X , &glZoomX_);
   glGetFloatv(GL_ZOOM_Y , &glZoomY_);


   glClipPlanei_.resize(glMaxClipPlanes_);
   for (int i = 0; i < glMaxClipPlanes_; ++i)
      glGetBooleanv(GL_CLIP_PLANE0 + i , &glClipPlanei_[i]);

   glLighti_.resize(glMaxLights_);
   for (int i = 0; i < glMaxLights_; ++i)
      glGetBooleanv(GL_LIGHT0 + i , &glLighti_[i]);
}


///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::ostream& operator <<(std::ostream& o , const EtatOpenGL& etat)
///
/// Cet opérateur permet d'afficher l'état OpenGL sous une forme lisible.
///
/// @param[in] o    : Le flux de sortie.
/// @param[in] etat : L'état OpenGL à afficher.
///
/// @return Le flux de sortie.
///
///////////////////////////////////////////////////////////////////////////////
std::ostream& operator <<(std::ostream& o , const EtatOpenGL& etat)
{
   o << "GL_ACCUM_ALPHA_BITS : " << etat.obtenirChaineGlAccumAlphaBits() << std::endl;
   o << "GL_ACCUM_BLUE_BITS : " << etat.obtenirChaineGlAccumBlueBits() << std::endl;
   o << "GL_ACCUM_CLEAR_VALUE : " << etat.obtenirChaineGlAccumClearValue() << std::endl;
   o << "GL_ACCUM_GREEN_BITS : " << etat.obtenirChaineGlAccumGreenBits() << std::endl;
   o << "GL_ACCUM_RED_BITS : " << etat.obtenirChaineGlAccumRedBits() << std::endl;
   o << "GL_ALPHA_BIAS : " << etat.obtenirChaineGlAlphaBias() << std::endl;
   o << "GL_ALPHA_BITS : " << etat.obtenirChaineGlAlphaBits() << std::endl;
   o << "GL_ALPHA_SCALE : " << etat.obtenirChaineGlAlphaScale() << std::endl;
   o << "GL_ALPHA_TEST : " << etat.obtenirChaineGlAlphaTest() << std::endl;
   o << "GL_ALPHA_TEST_FUNC : " << etat.obtenirChaineGlAlphaTestFunc() << std::endl;
   o << "GL_ALPHA_TEST_REF : " << etat.obtenirChaineGlAlphaTestRef() << std::endl;
   o << "GL_ATTRIB_STACK_DEPTH : " << etat.obtenirChaineGlAttribStackDepth() << std::endl;
   o << "GL_AUTO_NORMAL : " << etat.obtenirChaineGlAutoNormal() << std::endl;
   o << "GL_AUX_BUFFERS : " << etat.obtenirChaineGlAuxBuffers() << std::endl;
   o << "GL_BLEND : " << etat.obtenirChaineGlBlend() << std::endl;
   o << "GL_BLEND_DST : " << etat.obtenirChaineGlBlendDst() << std::endl;
   o << "GL_BLEND_SRC : " << etat.obtenirChaineGlBlendSrc() << std::endl;
   o << "GL_BLUE_BIAS : " << etat.obtenirChaineGlBlueBias() << std::endl;
   o << "GL_BLUE_BITS : " << etat.obtenirChaineGlBlueBits() << std::endl;
   o << "GL_BLUE_SCALE : " << etat.obtenirChaineGlBlueScale() << std::endl;
   o << "GL_CLIENT_ATTRIB_STACK_DEPTH : " << etat.obtenirChaineGlClientAttribStackDepth() << std::endl;
   o << "GL_COLOR_ARRAY : " << etat.obtenirChaineGlColorArray() << std::endl;
   o << "GL_COLOR_ARRAY_SIZE : " << etat.obtenirChaineGlColorArraySize() << std::endl;
   o << "GL_COLOR_ARRAY_STRIDE : " << etat.obtenirChaineGlColorArrayStride() << std::endl;
   o << "GL_COLOR_ARRAY_TYPE : " << etat.obtenirChaineGlColorArrayType() << std::endl;
   o << "GL_COLOR_CLEAR_VALUE : " << etat.obtenirChaineGlColorClearValue() << std::endl;
   o << "GL_COLOR_LOGIC_OP : " << etat.obtenirChaineGlColorLogicOp() << std::endl;
   o << "GL_COLOR_MATERIAL : " << etat.obtenirChaineGlColorMaterial() << std::endl;
   o << "GL_COLOR_MATERIAL_FACE : " << etat.obtenirChaineGlColorMaterialFace() << std::endl;
   o << "GL_COLOR_MATERIAL_PARAMETER : " << etat.obtenirChaineGlColorMaterialParameter() << std::endl;
   o << "GL_COLOR_WRITEMASK : " << etat.obtenirChaineGlColorWritemask() << std::endl;
   o << "GL_CULL_FACE : " << etat.obtenirChaineGlCullFace() << std::endl;
   o << "GL_CULL_FACE_MODE : " << etat.obtenirChaineGlCullFaceMode() << std::endl;
   o << "GL_CURRENT_COLOR : " << etat.obtenirChaineGlCurrentColor() << std::endl;
   o << "GL_CURRENT_INDEX : " << etat.obtenirChaineGlCurrentIndex() << std::endl;
   o << "GL_CURRENT_NORMAL : " << etat.obtenirChaineGlCurrentNormal() << std::endl;
   o << "GL_CURRENT_RASTER_COLOR : " << etat.obtenirChaineGlCurrentRasterColor() << std::endl;
   o << "GL_CURRENT_RASTER_DISTANCE : " << etat.obtenirChaineGlCurrentRasterDistance() << std::endl;
   o << "GL_CURRENT_RASTER_INDEX : " << etat.obtenirChaineGlCurrentRasterIndex() << std::endl;
   o << "GL_CURRENT_RASTER_POSITION : " << etat.obtenirChaineGlCurrentRasterPosition() << std::endl;
   o << "GL_CURRENT_RASTER_POSITION_VALID : " << etat.obtenirChaineGlCurrentRasterPositionValid() << std::endl;
   o << "GL_CURRENT_RASTER_TEXTURE_COORDS : " << etat.obtenirChaineGlCurrentRasterTextureCoords() << std::endl;
   o << "GL_CURRENT_TEXTURE_COORDS : " << etat.obtenirChaineGlCurrentTextureCoords() << std::endl;
   o << "GL_DEPTH_BIAS : " << etat.obtenirChaineGlDepthBias() << std::endl;
   o << "GL_DEPTH_BITS : " << etat.obtenirChaineGlDepthBits() << std::endl;
   o << "GL_DEPTH_CLEAR_VALUE : " << etat.obtenirChaineGlDepthClearValue() << std::endl;
   o << "GL_DEPTH_FUNC : " << etat.obtenirChaineGlDepthFunc() << std::endl;
   o << "GL_DEPTH_RANGE : " << etat.obtenirChaineGlDepthRange() << std::endl;
   o << "GL_DEPTH_SCALE : " << etat.obtenirChaineGlDepthScale() << std::endl;
   o << "GL_DEPTH_TEST : " << etat.obtenirChaineGlDepthTest() << std::endl;
   o << "GL_DEPTH_WRITEMASK : " << etat.obtenirChaineGlDepthWritemask() << std::endl;
   o << "GL_DITHER : " << etat.obtenirChaineGlDither() << std::endl;
   o << "GL_DOUBLEBUFFER : " << etat.obtenirChaineGlDoublebuffer() << std::endl;
   o << "GL_DRAW_BUFFER : " << etat.obtenirChaineGlDrawBuffer() << std::endl;
   o << "GL_EDGE_FLAG : " << etat.obtenirChaineGlEdgeFlag() << std::endl;
   o << "GL_EDGE_FLAG_ARRAY : " << etat.obtenirChaineGlEdgeFlagArray() << std::endl;
   o << "GL_EDGE_FLAG_ARRAY_STRIDE : " << etat.obtenirChaineGlEdgeFlagArrayStride() << std::endl;
   o << "GL_FEEDBACK_BUFFER_SIZE : " << etat.obtenirChaineGlFeedbackBufferSize() << std::endl;
   o << "GL_FEEDBACK_BUFFER_TYPE : " << etat.obtenirChaineGlFeedbackBufferType() << std::endl;
   o << "GL_FOG : " << etat.obtenirChaineGlFog() << std::endl;
   o << "GL_FOG_COLOR : " << etat.obtenirChaineGlFogColor() << std::endl;
   o << "GL_FOG_DENSITY : " << etat.obtenirChaineGlFogDensity() << std::endl;
   o << "GL_FOG_END : " << etat.obtenirChaineGlFogEnd() << std::endl;
   o << "GL_FOG_HINT : " << etat.obtenirChaineGlFogHint() << std::endl;
   o << "GL_FOG_INDEX : " << etat.obtenirChaineGlFogIndex() << std::endl;
   o << "GL_FOG_MODE : " << etat.obtenirChaineGlFogMode() << std::endl;
   o << "GL_FOG_START : " << etat.obtenirChaineGlFogStart() << std::endl;
   o << "GL_FRONT_FACE : " << etat.obtenirChaineGlFrontFace() << std::endl;
   o << "GL_GREEN_BIAS : " << etat.obtenirChaineGlGreenBias() << std::endl;
   o << "GL_GREEN_BITS : " << etat.obtenirChaineGlGreenBits() << std::endl;
   o << "GL_GREEN_SCALE : " << etat.obtenirChaineGlGreenScale() << std::endl;
   o << "GL_INDEX_ARRAY : " << etat.obtenirChaineGlIndexArray() << std::endl;
   o << "GL_INDEX_ARRAY_STRIDE : " << etat.obtenirChaineGlIndexArrayStride() << std::endl;
   o << "GL_INDEX_ARRAY_TYPE : " << etat.obtenirChaineGlIndexArrayType() << std::endl;
   o << "GL_INDEX_BITS : " << etat.obtenirChaineGlIndexBits() << std::endl;
   o << "GL_INDEX_CLEAR_VALUE : " << etat.obtenirChaineGlIndexClearValue() << std::endl;
   o << "GL_INDEX_LOGIC_OP : " << etat.obtenirChaineGlIndexLogicOp() << std::endl;
   o << "GL_INDEX_MODE : " << etat.obtenirChaineGlIndexMode() << std::endl;
   o << "GL_INDEX_OFFSET : " << etat.obtenirChaineGlIndexOffset() << std::endl;
   o << "GL_INDEX_SHIFT : " << etat.obtenirChaineGlIndexShift() << std::endl;
   o << "GL_INDEX_WRITEMASK : " << etat.obtenirChaineGlIndexWritemask() << std::endl;
   o << "GL_LIGHTING : " << etat.obtenirChaineGlLighting() << std::endl;
   o << "GL_LIGHT_MODEL_AMBIENT : " << etat.obtenirChaineGlLightModelAmbient() << std::endl;
   o << "GL_LIGHT_MODEL_LOCAL_VIEWER : " << etat.obtenirChaineGlLightModelLocalViewer() << std::endl;
   o << "GL_LIGHT_MODEL_TWO_SIDE : " << etat.obtenirChaineGlLightModelTwoSide() << std::endl;
   o << "GL_LINE_SMOOTH : " << etat.obtenirChaineGlLineSmooth() << std::endl;
   o << "GL_LINE_SMOOTH_HINT : " << etat.obtenirChaineGlLineSmoothHint() << std::endl;
   o << "GL_LINE_STIPPLE : " << etat.obtenirChaineGlLineStipple() << std::endl;
   o << "GL_LINE_STIPPLE_PATTERN : " << etat.obtenirChaineGlLineStipplePattern() << std::endl;
   o << "GL_LINE_STIPPLE_REPEAT : " << etat.obtenirChaineGlLineStippleRepeat() << std::endl;
   o << "GL_LINE_WIDTH : " << etat.obtenirChaineGlLineWidth() << std::endl;
   o << "GL_LINE_WIDTH_GRANULARITY : " << etat.obtenirChaineGlLineWidthGranularity() << std::endl;
   o << "GL_LINE_WIDTH_RANGE : " << etat.obtenirChaineGllineWidthRange() << std::endl;
   o << "GL_LIST_BASE : " << etat.obtenirChaineGlListBase() << std::endl;
   o << "GL_LIST_INDEX : " << etat.obtenirChaineGlListIndex() << std::endl;
   o << "GL_LIST_MODE : " << etat.obtenirChaineGlListMode() << std::endl;
   o << "GL_LOGIC_OP_MODE : " << etat.obtenirChaineGlLogicOpMode() << std::endl;
   o << "GL_MAP1_COLOR_4 : " << etat.obtenirChaineGlMap1Color4() << std::endl;
   o << "GL_MAP1_GRID_DOMAIN : " << etat.obtenirChaineGlMap1GridDomain() << std::endl;
   o << "GL_MAP1_GRID_SEGMENTS : " << etat.obtenirChaineGlMap1GridSegments() << std::endl;
   o << "GL_MAP1_INDEX : " << etat.obtenirChaineGlMap1Index() << std::endl;
   o << "GL_MAP1_NORMAL : " << etat.obtenirChaineGlMap1Normal() << std::endl;
   o << "GL_MAP1_TEXTURE_COORD_1 : " << etat.obtenirChaineGlMap1TextureCoord1() << std::endl;
   o << "GL_MAP1_TEXTURE_COORD_2 : " << etat.obtenirChaineGlMap1TextureCoord2() << std::endl;
   o << "GL_MAP1_TEXTURE_COORD_3 : " << etat.obtenirChaineGlMap1TextureCoord3() << std::endl;
   o << "GL_MAP1_TEXTURE_COORD_4 : " << etat.obtenirChaineGlMap1TextureCoord4() << std::endl;
   o << "GL_MAP1_VERTEX_3 : " << etat.obtenirChaineGlMap1Vertex3() << std::endl;
   o << "GL_MAP1_VERTEX_4 : " << etat.obtenirChaineGlMap1Vertex4() << std::endl;
   o << "GL_MAP2_COLOR_4 : " << etat.obtenirChaineGlMap2Color4() << std::endl;
   o << "GL_MAP2_GRID_DOMAIN : " << etat.obtenirChaineGlMap2GridDomain() << std::endl;
   o << "GL_MAP2_GRID_SEGMENTS : " << etat.obtenirChaineGlMap2GridSegments() << std::endl;
   o << "GL_MAP2_INDEX : " << etat.obtenirChaineGlMap2Index() << std::endl;
   o << "GL_MAP2_NORMAL : " << etat.obtenirChaineGlMap2Normal() << std::endl;
   o << "GL_MAP2_TEXTURE_COORD_1 : " << etat.obtenirChaineGlMap2TextureCoord1() << std::endl;
   o << "GL_MAP2_TEXTURE_COORD_2 : " << etat.obtenirChaineGlMap2TextureCoord2() << std::endl;
   o << "GL_MAP2_TEXTURE_COORD_3 : " << etat.obtenirChaineGlMap2TextureCoord3() << std::endl;
   o << "GL_MAP2_TEXTURE_COORD_4 : " << etat.obtenirChaineGlMap2TextureCoord4() << std::endl;
   o << "GL_MAP2_VERTEX_3 : " << etat.obtenirChaineGlMap2Vertex3() << std::endl;
   o << "GL_MAP2_VERTEX_4 : " << etat.obtenirChaineGlMap2Vertex4() << std::endl;
   o << "GL_MAP_COLOR : " << etat.obtenirChaineGlMapColor() << std::endl;
   o << "GL_MAP_STENCIL : " << etat.obtenirChaineGlMapStencil() << std::endl;
   o << "GL_MATRIX_MODE : " << etat.obtenirChaineGlMatrixMode() << std::endl;
   o << "GL_MAX_CLIENT_ATTRIB_STACK_DEPTH : " << etat.obtenirChaineGlMaxClientAttribStackDepth() << std::endl;
   o << "GL_MAX_ATTRIB_STACK_DEPTH : " << etat.obtenirChaineGlMaxAttribStackDepth() << std::endl;
   o << "GL_MAX_CLIP_PLANES : " << etat.obtenirChaineGlMaxClipPlanes() << std::endl;
   o << "GL_MAX_EVAL_ORDER : " << etat.obtenirChaineGlMaxEvalOrder() << std::endl;
   o << "GL_MAX_LIGHTS : " << etat.obtenirChaineGlMaxLights() << std::endl;
   o << "GL_MAX_LIST_NESTING : " << etat.obtenirChaineGlMaxListNesting() << std::endl;
   o << "GL_MAX_MODELVIEW_STACK_DEPTH : " << etat.obtenirChaineGlMaxModelviewStackDepth() << std::endl;
   o << "GL_MAX_NAME_STACK_DEPTH : " << etat.obtenirChaineGlMaxNameStackDepth() << std::endl;
   o << "GL_MAX_PIXEL_MAP_TABLE : " << etat.obtenirChaineGlMaxPixelMapTable() << std::endl;
   o << "GL_MAX_PROJECTION_STACK_DEPTH : " << etat.obtenirChaineGlMaxProjectionStackDepth() << std::endl;
   o << "GL_MAX_TEXTURE_SIZE : " << etat.obtenirChaineGlMaxTextureSize() << std::endl;
   o << "GL_MAX_TEXTURE_STACK_DEPTH : " << etat.obtenirChaineGlMaxTextureStackDepth() << std::endl;
   o << "GL_MAX_VIEWPORT_DIMS : " << etat.obtenirChaineGlMaxViewportDims() << std::endl;
   o << "GL_MODELVIEW_MATRIX : " << etat.obtenirChaineGlModelviewMatrix() << std::endl;
   o << "GL_MODELVIEW_STACK_DEPTH : " << etat.obtenirChaineGlModelviewStackDepth() << std::endl;
   o << "GL_NAME_STACK_DEPTH : " << etat.obtenirChaineGlNameStackDepth() << std::endl;
   o << "GL_NORMAL_ARRAY : " << etat.obtenirChaineGlNormalArray() << std::endl;
   o << "GL_NORMAL_ARRAY_STRIDE : " << etat.obtenirChaineGlNormalArrayStride() << std::endl;
   o << "GL_NORMAL_ARRAY_TYPE : " << etat.obtenirChaineGlNormalArrayType() << std::endl;
   o << "GL_NORMALIZE : " << etat.obtenirChaineGlNormalize() << std::endl;
   o << "GL_PACK_ALIGNMENT : " << etat.obtenirChaineGlPackAlignment() << std::endl;
   o << "GL_PACK_LSB_FIRST : " << etat.obtenirChaineGlPackLsbFirst() << std::endl;
   o << "GL_PACK_ROW_LENGTH : " << etat.obtenirChaineGlPackRowLength() << std::endl;
   o << "GL_PACK_SKIP_PIXELS : " << etat.obtenirChaineGlPackSkipPixels() << std::endl;
   o << "GL_PACK_SKIP_ROWS : " << etat.obtenirChaineGlPackSkipRows() << std::endl;
   o << "GL_PACK_SWAP_BYTES : " << etat.obtenirChaineGlPackSwapBytes() << std::endl;
   o << "GL_PERSPECTIVE_CORRECTION_HINT : " << etat.obtenirChaineGlPerspectiveCorrectionHint() << std::endl;
   o << "GL_PIXEL_MAP_A_TO_A_SIZE : " << etat.obtenirChaineGlPixelMapAToASize() << std::endl;
   o << "GL_PIXEL_MAP_B_TO_B_SIZE : " << etat.obtenirChaineGlPixelMapBToBSize() << std::endl;
   o << "GL_PIXEL_MAP_G_TO_G_SIZE : " << etat.obtenirChaineGlPixelMapGToGSize() << std::endl;
   o << "GL_PIXEL_MAP_I_TO_A_SIZE : " << etat.obtenirChaineGlPixelMapIToASize() << std::endl;
   o << "GL_PIXEL_MAP_I_TO_B_SIZE : " << etat.obtenirChaineGlPixelMapIToBSize() << std::endl;
   o << "GL_PIXEL_MAP_I_TO_G_SIZE : " << etat.obtenirChaineGlPixelMapIToGSize() << std::endl;
   o << "GL_PIXEL_MAP_I_TO_I_SIZE : " << etat.obtenirChaineGlPixelMapIToISize() << std::endl;
   o << "GL_PIXEL_MAP_I_TO_R_SIZE : " << etat.obtenirChaineGlPixelMapIToRSize() << std::endl;
   o << "GL_PIXEL_MAP_R_TO_R_SIZE : " << etat.obtenirChaineGlPixelMapRToRSize() << std::endl;
   o << "GL_PIXEL_MAP_S_TO_S_SIZE : " << etat.obtenirChaineGlPixelMapSToSSize() << std::endl;
   o << "GL_POINT_SIZE : " << etat.obtenirChaineGlPointSize() << std::endl;
   o << "GL_POINT_SIZE_GRANULARITY : " << etat.obtenirChaineGlPointSizeGranularity() << std::endl;
   o << "GL_POINT_SIZE_RANGE : " << etat.obtenirChaineGlPointSizeRange() << std::endl;
   o << "GL_POINT_SMOOTH : " << etat.obtenirChaineGlPointSmooth() << std::endl;
   o << "GL_POINT_SMOOTH_HINT : " << etat.obtenirChaineGlPointSmoothHint() << std::endl;
   o << "GL_POLYGON_MODE : " << etat.obtenirChaineGlPolygonMode() << std::endl;
   o << "GL_POLYGON_OFFSET_FACTOR : " << etat.obtenirChaineGlPolygonOffsetFactor() << std::endl;
   o << "GL_POLYGON_OFFSET_UNITS : " << etat.obtenirChaineGlPolygonOffsetUnits() << std::endl;
   o << "GL_POLYGON_OFFSET_FILL : " << etat.obtenirChaineGlPolygonOffsetFill() << std::endl;
   o << "GL_POLYGON_OFFSET_LINE : " << etat.obtenirChaineGlPolygonOffsetLine() << std::endl;
   o << "GL_POLYGON_OFFSET_POINT : " << etat.obtenirChaineGlPolygonOffsetPoint() << std::endl;
   o << "GL_POLYGON_SMOOTH : " << etat.obtenirChaineGlPolygonSmooth() << std::endl;
   o << "GL_POLYGON_SMOOTH_HINT : " << etat.obtenirChaineGlPolygonSmoothHint() << std::endl;
   o << "GL_POLYGON_STIPPLE : " << etat.obtenirChaineGlPolygonStipple() << std::endl;
   o << "GL_PROJECTION_MATRIX : " << etat.obtenirChaineGlProjectionMatrix() << std::endl;
   o << "GL_PROJECTION_STACK_DEPTH : " << etat.obtenirChaineGlProjectionStackDepth() << std::endl;
   o << "GL_READ_BUFFER : " << etat.obtenirChaineGlReadBuffer() << std::endl;
   o << "GL_RED_BIAS : " << etat.obtenirChaineGlRedBias() << std::endl;
   o << "GL_RED_BITS : " << etat.obtenirChaineGlRedBits() << std::endl;
   o << "GL_RED_SCALE : " << etat.obtenirChaineGlRedScale() << std::endl;
   o << "GL_RENDER_MODE : " << etat.obtenirChaineGlRenderMode() << std::endl;
   o << "GL_RGBA_MODE : " << etat.obtenirChaineGlRgbaMode() << std::endl;
   o << "GL_SCISSOR_BOX : " << etat.obtenirChaineGlScissorBox() << std::endl;
   o << "GL_SCISSOR_TEST : " << etat.obtenirChaineGlScissorTest() << std::endl;
   o << "GL_SELECTION_BUFFER_SIZE : " << etat.obtenirChaineGlSelectionBufferSize() << std::endl;
   o << "GL_SHADE_MODEL : " << etat.obtenirChaineGlShadeModel() << std::endl;
   o << "GL_STENCIL_BITS : " << etat.obtenirChaineGlStencilBits() << std::endl;
   o << "GL_STENCIL_CLEAR_VALUE : " << etat.obtenirChaineGlStencilClearValue() << std::endl;
   o << "GL_STENCIL_FAIL : " << etat.obtenirChaineGlStencilFail() << std::endl;
   o << "GL_STENCIL_FUNC : " << etat.obtenirChaineGlStencilFunc() << std::endl;
   o << "GL_STENCIL_PASS_DEPTH_FAIL : " << etat.obtenirChaineGlStencilPassDepthFail() << std::endl;
   o << "GL_STENCIL_PASS_DEPTH_PASS : " << etat.obtenirChaineGlStencilPassDepthPass() << std::endl;
   o << "GL_STENCIL_REF : " << etat.obtenirChaineGlStencilRef() << std::endl;
   o << "GL_STENCIL_TEST : " << etat.obtenirChaineGlStencilTest() << std::endl;
   o << "GL_STENCIL_VALUE_MASK : " << etat.obtenirChaineGlStencilValueMask() << std::endl;
   o << "GL_STENCIL_WRITEMASK : " << etat.obtenirChaineGlStencilWritemask() << std::endl;
   o << "GL_STEREO : " << etat.obtenirChaineGlStereo() << std::endl;
   o << "GL_SUBPIXEL_BITS : " << etat.obtenirChaineGlSubpixelBits() << std::endl;
   o << "GL_TEXTURE_1D : " << etat.obtenirChaineGlTexture1D() << std::endl;
   o << "GL_TEXTURE_BINDING_1D : " << etat.obtenirChaineGlTextureBinding1D() << std::endl;
   o << "GL_TEXTURE_2D : " << etat.obtenirChaineGlTexture2D() << std::endl;
   o << "GL_TEXTURE_BINDING_2D : " << etat.obtenirChaineGlTextureBinding2D() << std::endl;
   o << "GL_TEXTURE_COORD_ARRAY : " << etat.obtenirChaineGlTextureCoordArray() << std::endl;
   o << "GL_TEXTURE_COORD_ARRAY_SIZE : " << etat.obtenirChaineGlTextureCoordArraySize() << std::endl;
   o << "GL_TEXTURE_COORD_ARRAY_STRIDE : " << etat.obtenirChaineGlTextureCoordArrayStride() << std::endl;
   o << "GL_TEXTURE_COORD_ARRAY_TYPE : " << etat.obtenirChaineGlTextureCoordArrayType() << std::endl;
   o << "GL_TEXTURE_GEN_Q : " << etat.obtenirChaineGlTextureGenQ() << std::endl;
   o << "GL_TEXTURE_GEN_R : " << etat.obtenirChaineGlTextureGenR() << std::endl;
   o << "GL_TEXTURE_GEN_S : " << etat.obtenirChaineGlTextureGenS() << std::endl;
   o << "GL_TEXTURE_GEN_T : " << etat.obtenirChaineGlTextureGenT() << std::endl;
   o << "GL_TEXTURE_MATRIX : " << etat.obtenirChaineGlTextureMatrix() << std::endl;
   o << "GL_TEXTURE_STACK_DEPTH : " << etat.obtenirChaineGlTextureStackDepth() << std::endl;
   o << "GL_UNPACK_ALIGNMENT : " << etat.obtenirChaineGlUnpackAlignment() << std::endl;
   o << "GL_UNPACK_LSB_FIRST : " << etat.obtenirChaineGlUnpackLsbFirst() << std::endl;
   o << "GL_UNPACK_ROW_LENGTH : " << etat.obtenirChaineGlUnpackRowLength() << std::endl;
   o << "GL_UNPACK_SKIP_PIXELS : " << etat.obtenirChaineGlUnpackSkipPixels() << std::endl;
   o << "GL_UNPACK_SKIP_ROWS : " << etat.obtenirChaineGlUnpackSkipRows() << std::endl;
   o << "GL_UNPACK_SWAP_BYTES : " << etat.obtenirChaineGlUnpackSwapBytes() << std::endl;
   o << "GL_VERTEX_ARRAY : " << etat.obtenirChaineGlVertexArray() << std::endl;
   o << "GL_VERTEX_ARRAY_SIZE : " << etat.obtenirChaineGlVertexArraySize() << std::endl;
   o << "GL_VERTEX_ARRAY_STRIDE : " << etat.obtenirChaineGlVertexArrayStride() << std::endl;
   o << "GL_VERTEX_ARRAY_TYPE : " << etat.obtenirChaineGlVertexArrayType() << std::endl;
   o << "GL_VIEWPORT : " << etat.obtenirChaineGlViewport() << std::endl;
   o << "GL_ZOOM_X : " << etat.obtenirChaineGlZoomX() << std::endl;
   o << "GL_ZOOM_Y : " << etat.obtenirChaineGlZoomY() << std::endl;

   return o;
}


///////////////////////////////////////////////////////////////////////////////
///
/// @fn void EtatOpenGL::obtenirDifference(std::ostream& o, const EtatOpenGL& etat1, const EtatOpenGL& etat2)
///
/// Cet opérateur permet d'afficher l'état OpenGL sous une forme lisible.
///
/// @param[in] o     : Le flux de sortie.
/// @param[in] etat1 : Le premier état OpenGL de la comparaison.
/// @param[in] etat2 : Le second état OpenGL de la comparaison.
///
/// @return Le flux de sortie.
///
///////////////////////////////////////////////////////////////////////////////
void EtatOpenGL::obtenirDifference(std::ostream& o,
                                   const EtatOpenGL& etat1,
                                   const EtatOpenGL& etat2)
{
   if (memcmp(&etat1.glAccumAlphaBits_, &etat2.glAccumAlphaBits_, sizeof(etat1.glAccumAlphaBits_))) {
      o << "GL_ACCUM_ALPHA_BITS :"
        << "\nAvant : " << etat1.obtenirChaineGlAccumAlphaBits()
        << "\nAprès : " << etat2.obtenirChaineGlAccumAlphaBits()
        << std::endl;
   }

   if (memcmp(&etat1.glAccumBlueBits_, &etat2.glAccumBlueBits_, sizeof(etat1.glAccumBlueBits_))) {
      o << "GL_ACCUM_BLUE_BITS :"
        << "\nAvant : " << etat1.obtenirChaineGlAccumBlueBits()
        << "\nAprès : " << etat2.obtenirChaineGlAccumBlueBits()
        << std::endl;
   }

   if (memcmp(etat1.glAccumClearValue_, etat2.glAccumClearValue_, sizeof(etat1.glAccumClearValue_))) {
      o << "GL_ACCUM_CLEAR_VALUE :"
        << "\nAvant : " << etat1.obtenirChaineGlAccumClearValue()
        << "\nAprès : " << etat2.obtenirChaineGlAccumClearValue()
        << std::endl;
   }

   if (memcmp(&etat1.glAccumGreenBits_, &etat2.glAccumGreenBits_, sizeof(etat1.glAccumGreenBits_))) {
      o << "GL_ACCUM_GREEN_BITS :"
        << "\nAvant : " << etat1.obtenirChaineGlAccumGreenBits()
        << "\nAprès : " << etat2.obtenirChaineGlAccumGreenBits()
        << std::endl;
   }

   if (memcmp(&etat1.glAccumRedBits_, &etat2.glAccumRedBits_, sizeof(etat1.glAccumRedBits_))) {
      o << "GL_ACCUM_RED_BITS :"
        << "\nAvant : " << etat1.obtenirChaineGlAccumRedBits()
        << "\nAprès : " << etat2.obtenirChaineGlAccumRedBits()
        << std::endl;
   }

   if (memcmp(&etat1.glAlphaBias_, &etat2.glAlphaBias_, sizeof(etat1.glAlphaBias_))) {
      o << "GL_ALPHA_BIAS :"
        << "\nAvant : " << etat1.obtenirChaineGlAlphaBias()
        << "\nAprès : " << etat2.obtenirChaineGlAlphaBias()
        << std::endl;
   }

   if (memcmp(&etat1.glAlphaBits_, &etat2.glAlphaBits_, sizeof(etat1.glAlphaBits_))) {
      o << "GL_ALPHA_BITS :"
        << "\nAvant : " << etat1.obtenirChaineGlAlphaBits()
        << "\nAprès : " << etat2.obtenirChaineGlAlphaBits()
        << std::endl;
   }

   if (memcmp(&etat1.glAlphaScale_, &etat2.glAlphaScale_, sizeof(etat1.glAlphaScale_))) {
      o << "GL_ALPHA_SCALE :"
        << "\nAvant : " << etat1.obtenirChaineGlAlphaScale()
        << "\nAprès : " << etat2.obtenirChaineGlAlphaScale()
        << std::endl;
   }

   if (memcmp(&etat1.glAlphaTest_, &etat2.glAlphaTest_, sizeof(etat1.glAlphaTest_))) {
      o << "GL_ALPHA_TEST :"
        << "\nAvant : " << etat1.obtenirChaineGlAlphaTest()
        << "\nAprès : " << etat2.obtenirChaineGlAlphaTest()
        << std::endl;
   }

   if (memcmp(&etat1.glAlphaTestFunc_, &etat2.glAlphaTestFunc_, sizeof(etat1.glAlphaTestFunc_))) {
      o << "GL_ALPHA_TEST_FUNC :"
        << "\nAvant : " << etat1.obtenirChaineGlAlphaTestFunc()
        << "\nAprès : " << etat2.obtenirChaineGlAlphaTestFunc()
        << std::endl;
   }

   if (memcmp(&etat1.glAlphaTestRef_, &etat2.glAlphaTestRef_, sizeof(etat1.glAlphaTestRef_))) {
      o << "GL_ALPHA_TEST_REF :"
        << "\nAvant : " << etat1.obtenirChaineGlAlphaTestRef()
        << "\nAprès : " << etat2.obtenirChaineGlAlphaTestRef()
        << std::endl;
   }

   if (memcmp(&etat1.glAttribStackDepth_, &etat2.glAttribStackDepth_, sizeof(etat1.glAttribStackDepth_))) {
      o << "GL_ATTRIB_STACK_DEPTH :"
        << "\nAvant : " << etat1.obtenirChaineGlAttribStackDepth()
        << "\nAprès : " << etat2.obtenirChaineGlAttribStackDepth()
        << std::endl;
   }

   if (memcmp(&etat1.glAutoNormal_, &etat2.glAutoNormal_, sizeof(etat1.glAutoNormal_))) {
      o << "GL_AUTO_NORMAL :"
        << "\nAvant : " << etat1.obtenirChaineGlAutoNormal()
        << "\nAprès : " << etat2.obtenirChaineGlAutoNormal()
        << std::endl;
   }

   if (memcmp(&etat1.glAuxBuffers_, &etat2.glAuxBuffers_, sizeof(etat1.glAuxBuffers_))) {
      o << "GL_AUX_BUFFERS :"
        << "\nAvant : " << etat1.obtenirChaineGlAuxBuffers()
        << "\nAprès : " << etat2.obtenirChaineGlAuxBuffers()
        << std::endl;
   }

   if (memcmp(&etat1.glBlend_, &etat2.glBlend_, sizeof(etat1.glBlend_))) {
      o << "GL_BLEND :"
        << "\nAvant : " << etat1.obtenirChaineGlBlend()
        << "\nAprès : " << etat2.obtenirChaineGlBlend()
        << std::endl;
   }

   if (memcmp(&etat1.glBlendDst_, &etat2.glBlendDst_, sizeof(etat1.glBlendDst_))) {
      o << "GL_BLEND_DST :"
        << "\nAvant : " << etat1.obtenirChaineGlBlendDst()
        << "\nAprès : " << etat2.obtenirChaineGlBlendDst()
        << std::endl;
   }

   if (memcmp(&etat1.glBlendSrc_, &etat2.glBlendSrc_, sizeof(etat1.glBlendSrc_))) {
      o << "GL_BLEND_SRC :"
        << "\nAvant : " << etat1.obtenirChaineGlBlendSrc()
        << "\nAprès : " << etat2.obtenirChaineGlBlendSrc()
        << std::endl;
   }

   if (memcmp(&etat1.glBlueBias_, &etat2.glBlueBias_, sizeof(etat1.glBlueBias_))) {
      o << "GL_BLUE_BIAS :"
        << "\nAvant : " << etat1.obtenirChaineGlBlueBias()
        << "\nAprès : " << etat2.obtenirChaineGlBlueBias()
        << std::endl;
   }

   if (memcmp(&etat1.glBlueBits_, &etat2.glBlueBits_, sizeof(etat1.glBlueBits_))) {
      o << "GL_BLUE_BITS :"
        << "\nAvant : " << etat1.obtenirChaineGlBlueBits()
        << "\nAprès : " << etat2.obtenirChaineGlBlueBits()
        << std::endl;
   }

   if (memcmp(&etat1.glBlueScale_, &etat2.glBlueScale_, sizeof(etat1.glBlueScale_))) {
      o << "GL_BLUE_SCALE :"
        << "\nAvant : " << etat1.obtenirChaineGlBlueScale()
        << "\nAprès : " << etat2.obtenirChaineGlBlueScale()
        << std::endl;
   }

   if (memcmp(&etat1.glClientAttribStackDepth_, &etat2.glClientAttribStackDepth_, sizeof(etat1.glClientAttribStackDepth_))) {
      o << "GL_CLIENT_ATTRIB_STACK_DEPTH :"
        << "\nAvant : " << etat1.obtenirChaineGlClientAttribStackDepth()
        << "\nAprès : " << etat2.obtenirChaineGlClientAttribStackDepth()
        << std::endl;
   }

   if (memcmp(&etat1.glColorArray_, &etat2.glColorArray_, sizeof(etat1.glColorArray_))) {
      o << "GL_COLOR_ARRAY :"
        << "\nAvant : " << etat1.obtenirChaineGlColorArray()
        << "\nAprès : " << etat2.obtenirChaineGlColorArray()
        << std::endl;
   }

   if (memcmp(&etat1.glColorArraySize_, &etat2.glColorArraySize_, sizeof(etat1.glColorArraySize_))) {
      o << "GL_COLOR_ARRAY_SIZE :"
        << "\nAvant : " << etat1.obtenirChaineGlColorArraySize()
        << "\nAprès : " << etat2.obtenirChaineGlColorArraySize()
        << std::endl;
   }

   if (memcmp(&etat1.glColorArrayStride_, &etat2.glColorArrayStride_, sizeof(etat1.glColorArrayStride_))) {
      o << "GL_COLOR_ARRAY_STRIDE :"
        << "\nAvant : " << etat1.obtenirChaineGlColorArrayStride()
        << "\nAprès : " << etat2.obtenirChaineGlColorArrayStride()
        << std::endl;
   }

   if (memcmp(&etat1.glColorArrayType_, &etat2.glColorArrayType_, sizeof(etat1.glColorArrayType_))) {
      o << "GL_COLOR_ARRAY_TYPE :"
        << "\nAvant : " << etat1.obtenirChaineGlColorArrayType()
        << "\nAprès : " << etat2.obtenirChaineGlColorArrayType()
        << std::endl;
   }

   if (memcmp(etat1.glColorClearValue_, etat2.glColorClearValue_, sizeof(etat1.glColorClearValue_))) {
      o << "GL_COLOR_CLEAR_VALUE :"
        << "\nAvant : " << etat1.obtenirChaineGlColorClearValue()
        << "\nAprès : " << etat2.obtenirChaineGlColorClearValue()
        << std::endl;
   }

   if (memcmp(&etat1.glColorLogicOp_, &etat2.glColorLogicOp_, sizeof(etat1.glColorLogicOp_))) {
      o << "GL_COLOR_LOGIC_OP :"
        << "\nAvant : " << etat1.obtenirChaineGlColorLogicOp()
        << "\nAprès : " << etat2.obtenirChaineGlColorLogicOp()
        << std::endl;
   }

   if (memcmp(&etat1.glColorMaterial_, &etat2.glColorMaterial_, sizeof(etat1.glColorMaterial_))) {
      o << "GL_COLOR_MATERIAL :"
        << "\nAvant : " << etat1.obtenirChaineGlColorMaterial()
        << "\nAprès : " << etat2.obtenirChaineGlColorMaterial()
        << std::endl;
   }

   if (memcmp(&etat1.glColorMaterialFace_, &etat2.glColorMaterialFace_, sizeof(etat1.glColorMaterialFace_))) {
      o << "GL_COLOR_MATERIAL_FACE :"
        << "\nAvant : " << etat1.obtenirChaineGlColorMaterialFace()
        << "\nAprès : " << etat2.obtenirChaineGlColorMaterialFace()
        << std::endl;
   }

   if (memcmp(&etat1.glColorMaterialParameter_, &etat2.glColorMaterialParameter_, sizeof(etat1.glColorMaterialParameter_))) {
      o << "GL_COLOR_MATERIAL_PARAMETER :"
        << "\nAvant : " << etat1.obtenirChaineGlColorMaterialParameter()
        << "\nAprès : " << etat2.obtenirChaineGlColorMaterialParameter()
        << std::endl;
   }

   if (memcmp(etat1.glColorWritemask_, etat2.glColorWritemask_, sizeof(etat1.glColorWritemask_))) {
      o << "GL_COLOR_WRITEMASK :"
        << "\nAvant : " << etat1.obtenirChaineGlColorWritemask()
        << "\nAprès : " << etat2.obtenirChaineGlColorWritemask()
        << std::endl;
   }

   if (memcmp(&etat1.glCullFace_, &etat2.glCullFace_, sizeof(etat1.glCullFace_))) {
      o << "GL_CULL_FACE :"
        << "\nAvant : " << etat1.obtenirChaineGlCullFace()
        << "\nAprès : " << etat2.obtenirChaineGlCullFace()
        << std::endl;
   }

   if (memcmp(&etat1.glCullFaceMode_, &etat2.glCullFaceMode_, sizeof(etat1.glCullFaceMode_))) {
      o << "GL_CULL_FACE_MODE :"
        << "\nAvant : " << etat1.obtenirChaineGlCullFaceMode()
        << "\nAprès : " << etat2.obtenirChaineGlCullFaceMode()
        << std::endl;
   }

   if (memcmp(etat1.glCurrentColor_, etat2.glCurrentColor_, sizeof(etat1.glCurrentColor_))) {
      o << "GL_CURRENT_COLOR :"
        << "\nAvant : " << etat1.obtenirChaineGlCurrentColor()
        << "\nAprès : " << etat2.obtenirChaineGlCurrentColor()
        << std::endl;
   }

   if (memcmp(&etat1.glCurrentIndex_, &etat2.glCurrentIndex_, sizeof(etat1.glCurrentIndex_))) {
      o << "GL_CURRENT_INDEX :"
        << "\nAvant : " << etat1.obtenirChaineGlCurrentIndex()
        << "\nAprès : " << etat2.obtenirChaineGlCurrentIndex()
        << std::endl;
   }

   if (memcmp(etat1.glCurrentNormal_, etat2.glCurrentNormal_, sizeof(etat1.glCurrentNormal_))) {
      o << "GL_CURRENT_NORMAL :"
        << "\nAvant : " << etat1.obtenirChaineGlCurrentNormal()
        << "\nAprès : " << etat2.obtenirChaineGlCurrentNormal()
        << std::endl;
   }

   if (memcmp(etat1.glCurrentRasterColor_, etat2.glCurrentRasterColor_, sizeof(etat1.glCurrentRasterColor_))) {
      o << "GL_CURRENT_RASTER_COLOR :"
        << "\nAvant : " << etat1.obtenirChaineGlCurrentRasterColor()
        << "\nAprès : " << etat2.obtenirChaineGlCurrentRasterColor()
        << std::endl;
   }

   if (memcmp(&etat1.glCurrentRasterDistance_, &etat2.glCurrentRasterDistance_, sizeof(etat1.glCurrentRasterDistance_))) {
      o << "GL_CURRENT_RASTER_DISTANCE :"
        << "\nAvant : " << etat1.obtenirChaineGlCurrentRasterDistance()
        << "\nAprès : " << etat2.obtenirChaineGlCurrentRasterDistance()
        << std::endl;
   }

   if (memcmp(&etat1.glCurrentRasterIndex_, &etat2.glCurrentRasterIndex_, sizeof(etat1.glCurrentRasterIndex_))) {
      o << "GL_CURRENT_RASTER_INDEX :"
        << "\nAvant : " << etat1.obtenirChaineGlCurrentRasterIndex()
        << "\nAprès : " << etat2.obtenirChaineGlCurrentRasterIndex()
        << std::endl;
   }

   if (memcmp(etat1.glCurrentRasterPosition_, etat2.glCurrentRasterPosition_, sizeof(etat1.glCurrentRasterPosition_))) {
      o << "GL_CURRENT_RASTER_POSITION :"
        << "\nAvant : " << etat1.obtenirChaineGlCurrentRasterPosition()
        << "\nAprès : " << etat2.obtenirChaineGlCurrentRasterPosition()
        << std::endl;
   }

   if (memcmp(&etat1.glCurrentRasterPositionValid_, &etat2.glCurrentRasterPositionValid_, sizeof(etat1.glCurrentRasterPositionValid_))) {
      o << "GL_CURRENT_RASTER_POSITION_VALID :"
        << "\nAvant : " << etat1.obtenirChaineGlCurrentRasterPositionValid()
        << "\nAprès : " << etat2.obtenirChaineGlCurrentRasterPositionValid()
        << std::endl;
   }

   if (memcmp(etat1.glCurrentRasterTextureCoords_, etat2.glCurrentRasterTextureCoords_, sizeof(etat1.glCurrentRasterTextureCoords_))) {
      o << "GL_CURRENT_RASTER_TEXTURE_COORDS :"
        << "\nAvant : " << etat1.obtenirChaineGlCurrentRasterTextureCoords()
        << "\nAprès : " << etat2.obtenirChaineGlCurrentRasterTextureCoords()
        << std::endl;
   }

   if (memcmp(etat1.glCurrentTextureCoords_, etat2.glCurrentTextureCoords_, sizeof(etat1.glCurrentTextureCoords_))) {
      o << "GL_CURRENT_TEXTURE_COORDS :"
        << "\nAvant : " << etat1.obtenirChaineGlCurrentTextureCoords()
        << "\nAprès : " << etat2.obtenirChaineGlCurrentTextureCoords()
        << std::endl;
   }

   if (memcmp(&etat1.glDepthBias_, &etat2.glDepthBias_, sizeof(etat1.glDepthBias_))) {
      o << "GL_DEPTH_BIAS :"
        << "\nAvant : " << etat1.obtenirChaineGlDepthBias()
        << "\nAprès : " << etat2.obtenirChaineGlDepthBias()
        << std::endl;
   }

   if (memcmp(&etat1.glDepthBits_, &etat2.glDepthBits_, sizeof(etat1.glDepthBits_))) {
      o << "GL_DEPTH_BITS :"
        << "\nAvant : " << etat1.obtenirChaineGlDepthBits()
        << "\nAprès : " << etat2.obtenirChaineGlDepthBits()
        << std::endl;
   }

   if (memcmp(&etat1.glDepthClearValue_, &etat2.glDepthClearValue_, sizeof(etat1.glDepthClearValue_))) {
      o << "GL_DEPTH_CLEAR_VALUE :"
        << "\nAvant : " << etat1.obtenirChaineGlDepthClearValue()
        << "\nAprès : " << etat2.obtenirChaineGlDepthClearValue()
        << std::endl;
   }

   if (memcmp(&etat1.glDepthFunc_, &etat2.glDepthFunc_, sizeof(etat1.glDepthFunc_))) {
      o << "GL_DEPTH_FUNC :"
        << "\nAvant : " << etat1.obtenirChaineGlDepthFunc()
        << "\nAprès : " << etat2.obtenirChaineGlDepthFunc()
        << std::endl;
   }

   if (memcmp(etat1.glDepthRange_, etat2.glDepthRange_, sizeof(etat1.glDepthRange_))) {
      o << "GL_DEPTH_RANGE :"
        << "\nAvant : " << etat1.obtenirChaineGlDepthRange()
        << "\nAprès : " << etat2.obtenirChaineGlDepthRange()
        << std::endl;
   }

   if (memcmp(&etat1.glDepthScale_, &etat2.glDepthScale_, sizeof(etat1.glDepthScale_))) {
      o << "GL_DEPTH_SCALE :"
        << "\nAvant : " << etat1.obtenirChaineGlDepthScale()
        << "\nAprès : " << etat2.obtenirChaineGlDepthScale()
        << std::endl;
   }

   if (memcmp(&etat1.glDepthTest_, &etat2.glDepthTest_, sizeof(etat1.glDepthTest_))) {
      o << "GL_DEPTH_TEST :"
        << "\nAvant : " << etat1.obtenirChaineGlDepthTest()
        << "\nAprès : " << etat2.obtenirChaineGlDepthTest()
        << std::endl;
   }

   if (memcmp(&etat1.glDepthWritemask_, &etat2.glDepthWritemask_, sizeof(etat1.glDepthWritemask_))) {
      o << "GL_DEPTH_WRITEMASK :"
        << "\nAvant : " << etat1.obtenirChaineGlDepthWritemask()
        << "\nAprès : " << etat2.obtenirChaineGlDepthWritemask()
        << std::endl;
   }

   if (memcmp(&etat1.glDither_, &etat2.glDither_, sizeof(etat1.glDither_))) {
      o << "GL_DITHER :"
        << "\nAvant : " << etat1.obtenirChaineGlDither()
        << "\nAprès : " << etat2.obtenirChaineGlDither()
        << std::endl;
   }

   if (memcmp(&etat1.glDoublebuffer_, &etat2.glDoublebuffer_, sizeof(etat1.glDoublebuffer_))) {
      o << "GL_DOUBLEBUFFER :"
        << "\nAvant : " << etat1.obtenirChaineGlDoublebuffer()
        << "\nAprès : " << etat2.obtenirChaineGlDoublebuffer()
        << std::endl;
   }

   if (memcmp(&etat1.glDrawBuffer_, &etat2.glDrawBuffer_, sizeof(etat1.glDrawBuffer_))) {
      o << "GL_DRAW_BUFFER :"
        << "\nAvant : " << etat1.obtenirChaineGlDrawBuffer()
        << "\nAprès : " << etat2.obtenirChaineGlDrawBuffer()
        << std::endl;
   }

   if (memcmp(&etat1.glEdgeFlag_, &etat2.glEdgeFlag_, sizeof(etat1.glEdgeFlag_))) {
      o << "GL_EDGE_FLAG :"
        << "\nAvant : " << etat1.obtenirChaineGlEdgeFlag()
        << "\nAprès : " << etat2.obtenirChaineGlEdgeFlag()
        << std::endl;
   }

   if (memcmp(&etat1.glEdgeFlagArray_, &etat2.glEdgeFlagArray_, sizeof(etat1.glEdgeFlagArray_))) {
      o << "GL_EDGE_FLAG_ARRAY :"
        << "\nAvant : " << etat1.obtenirChaineGlEdgeFlagArray()
        << "\nAprès : " << etat2.obtenirChaineGlEdgeFlagArray()
        << std::endl;
   }

   if (memcmp(&etat1.glEdgeFlagArrayStride_, &etat2.glEdgeFlagArrayStride_, sizeof(etat1.glEdgeFlagArrayStride_))) {
      o << "GL_EDGE_FLAG_ARRAY_STRIDE :"
        << "\nAvant : " << etat1.obtenirChaineGlEdgeFlagArrayStride()
        << "\nAprès : " << etat2.obtenirChaineGlEdgeFlagArrayStride()
        << std::endl;
   }

   if (memcmp(&etat1.glFeedbackBufferSize_, &etat2.glFeedbackBufferSize_, sizeof(etat1.glFeedbackBufferSize_))) {
      o << "GL_FEEDBACK_BUFFER_SIZE :"
        << "\nAvant : " << etat1.obtenirChaineGlFeedbackBufferSize()
        << "\nAprès : " << etat2.obtenirChaineGlFeedbackBufferSize()
        << std::endl;
   }

   if (memcmp(&etat1.glFeedbackBufferType_, &etat2.glFeedbackBufferType_, sizeof(etat1.glFeedbackBufferType_))) {
      o << "GL_FEEDBACK_BUFFER_TYPE :"
        << "\nAvant : " << etat1.obtenirChaineGlFeedbackBufferType()
        << "\nAprès : " << etat2.obtenirChaineGlFeedbackBufferType()
        << std::endl;
   }

   if (memcmp(&etat1.glFog_, &etat2.glFog_, sizeof(etat1.glFog_))) {
      o << "GL_FOG :"
        << "\nAvant : " << etat1.obtenirChaineGlFog()
        << "\nAprès : " << etat2.obtenirChaineGlFog()
        << std::endl;
   }

   if (memcmp(etat1.glFogColor_, etat2.glFogColor_, sizeof(etat1.glFogColor_))) {
      o << "GL_FOG_COLOR :"
        << "\nAvant : " << etat1.obtenirChaineGlFogColor()
        << "\nAprès : " << etat2.obtenirChaineGlFogColor()
        << std::endl;
   }

   if (memcmp(&etat1.glFogDensity_, &etat2.glFogDensity_, sizeof(etat1.glFogDensity_))) {
      o << "GL_FOG_DENSITY :"
        << "\nAvant : " << etat1.obtenirChaineGlFogDensity()
        << "\nAprès : " << etat2.obtenirChaineGlFogDensity()
        << std::endl;
   }

   if (memcmp(&etat1.glFogEnd_, &etat2.glFogEnd_, sizeof(etat1.glFogEnd_))) {
      o << "GL_FOG_END :"
        << "\nAvant : " << etat1.obtenirChaineGlFogEnd()
        << "\nAprès : " << etat2.obtenirChaineGlFogEnd()
        << std::endl;
   }

   if (memcmp(&etat1.glFogHint_, &etat2.glFogHint_, sizeof(etat1.glFogHint_))) {
      o << "GL_FOG_HINT :"
        << "\nAvant : " << etat1.obtenirChaineGlFogHint()
        << "\nAprès : " << etat2.obtenirChaineGlFogHint()
        << std::endl;
   }

   if (memcmp(&etat1.glFogIndex_, &etat2.glFogIndex_, sizeof(etat1.glFogIndex_))) {
      o << "GL_FOG_INDEX :"
        << "\nAvant : " << etat1.obtenirChaineGlFogIndex()
        << "\nAprès : " << etat2.obtenirChaineGlFogIndex()
        << std::endl;
   }

   if (memcmp(&etat1.glFogMode_, &etat2.glFogMode_, sizeof(etat1.glFogMode_))) {
      o << "GL_FOG_MODE :"
        << "\nAvant : " << etat1.obtenirChaineGlFogMode()
        << "\nAprès : " << etat2.obtenirChaineGlFogMode()
        << std::endl;
   }

   if (memcmp(&etat1.glFogStart_, &etat2.glFogStart_, sizeof(etat1.glFogStart_))) {
      o << "GL_FOG_START :"
        << "\nAvant : " << etat1.obtenirChaineGlFogStart()
        << "\nAprès : " << etat2.obtenirChaineGlFogStart()
        << std::endl;
   }

   if (memcmp(&etat1.glFrontFace_, &etat2.glFrontFace_, sizeof(etat1.glFrontFace_))) {
      o << "GL_FRONT_FACE :"
        << "\nAvant : " << etat1.obtenirChaineGlFrontFace()
        << "\nAprès : " << etat2.obtenirChaineGlFrontFace()
        << std::endl;
   }

   if (memcmp(&etat1.glGreenBias_, &etat2.glGreenBias_, sizeof(etat1.glGreenBias_))) {
      o << "GL_GREEN_BIAS :"
        << "\nAvant : " << etat1.obtenirChaineGlGreenBias()
        << "\nAprès : " << etat2.obtenirChaineGlGreenBias()
        << std::endl;
   }

   if (memcmp(&etat1.glGreenBits_, &etat2.glGreenBits_, sizeof(etat1.glGreenBits_))) {
      o << "GL_GREEN_BITS :"
        << "\nAvant : " << etat1.obtenirChaineGlGreenBits()
        << "\nAprès : " << etat2.obtenirChaineGlGreenBits()
        << std::endl;
   }

   if (memcmp(&etat1.glGreenScale_, &etat2.glGreenScale_, sizeof(etat1.glGreenScale_))) {
      o << "GL_GREEN_SCALE :"
        << "\nAvant : " << etat1.obtenirChaineGlGreenScale()
        << "\nAprès : " << etat2.obtenirChaineGlGreenScale()
        << std::endl;
   }

   if (memcmp(&etat1.glIndexArray_, &etat2.glIndexArray_, sizeof(etat1.glIndexArray_))) {
      o << "GL_INDEX_ARRAY :"
        << "\nAvant : " << etat1.obtenirChaineGlIndexArray()
        << "\nAprès : " << etat2.obtenirChaineGlIndexArray()
        << std::endl;
   }

   if (memcmp(&etat1.glIndexArrayStride_, &etat2.glIndexArrayStride_, sizeof(etat1.glIndexArrayStride_))) {
      o << "GL_INDEX_ARRAY_STRIDE :"
        << "\nAvant : " << etat1.obtenirChaineGlIndexArrayStride()
        << "\nAprès : " << etat2.obtenirChaineGlIndexArrayStride()
        << std::endl;
   }

   if (memcmp(&etat1.glIndexArrayType_, &etat2.glIndexArrayType_, sizeof(etat1.glIndexArrayType_))) {
      o << "GL_INDEX_ARRAY_TYPE :"
        << "\nAvant : " << etat1.obtenirChaineGlIndexArrayType()
        << "\nAprès : " << etat2.obtenirChaineGlIndexArrayType()
        << std::endl;
   }

   if (memcmp(&etat1.glIndexBits_, &etat2.glIndexBits_, sizeof(etat1.glIndexBits_))) {
      o << "GL_INDEX_BITS :"
        << "\nAvant : " << etat1.obtenirChaineGlIndexBits()
        << "\nAprès : " << etat2.obtenirChaineGlIndexBits()
        << std::endl;
   }

   if (memcmp(&etat1.glIndexClearValue_, &etat2.glIndexClearValue_, sizeof(etat1.glIndexClearValue_))) {
      o << "GL_INDEX_CLEAR_VALUE :"
        << "\nAvant : " << etat1.obtenirChaineGlIndexClearValue()
        << "\nAprès : " << etat2.obtenirChaineGlIndexClearValue()
        << std::endl;
   }

   if (memcmp(&etat1.glIndexLogicOp_, &etat2.glIndexLogicOp_, sizeof(etat1.glIndexLogicOp_))) {
      o << "GL_INDEX_LOGIC_OP :"
        << "\nAvant : " << etat1.obtenirChaineGlIndexLogicOp()
        << "\nAprès : " << etat2.obtenirChaineGlIndexLogicOp()
        << std::endl;
   }

   if (memcmp(&etat1.glIndexMode_, &etat2.glIndexMode_, sizeof(etat1.glIndexMode_))) {
      o << "GL_INDEX_MODE :"
        << "\nAvant : " << etat1.obtenirChaineGlIndexMode()
        << "\nAprès : " << etat2.obtenirChaineGlIndexMode()
        << std::endl;
   }

   if (memcmp(&etat1.glIndexOffset_, &etat2.glIndexOffset_, sizeof(etat1.glIndexOffset_))) {
      o << "GL_INDEX_OFFSET :"
        << "\nAvant : " << etat1.obtenirChaineGlIndexOffset()
        << "\nAprès : " << etat2.obtenirChaineGlIndexOffset()
        << std::endl;
   }

   if (memcmp(&etat1.glIndexShift_, &etat2.glIndexShift_, sizeof(etat1.glIndexShift_))) {
      o << "GL_INDEX_SHIFT :"
        << "\nAvant : " << etat1.obtenirChaineGlIndexShift()
        << "\nAprès : " << etat2.obtenirChaineGlIndexShift()
        << std::endl;
   }

   if (memcmp(&etat1.glIndexWritemask_, &etat2.glIndexWritemask_, sizeof(etat1.glIndexWritemask_))) {
      o << "GL_INDEX_WRITEMASK :"
        << "\nAvant : " << etat1.obtenirChaineGlIndexWritemask()
        << "\nAprès : " << etat2.obtenirChaineGlIndexWritemask()
        << std::endl;
   }

   if (memcmp(&etat1.glLighting_, &etat2.glLighting_, sizeof(etat1.glLighting_))) {
      o << "GL_LIGHTING :"
        << "\nAvant : " << etat1.obtenirChaineGlLighting()
        << "\nAprès : " << etat2.obtenirChaineGlLighting()
        << std::endl;
   }

   if (memcmp(etat1.glLightModelAmbient_, etat2.glLightModelAmbient_, sizeof(etat1.glLightModelAmbient_))) {
      o << "GL_LIGHT_MODEL_AMBIENT :"
        << "\nAvant : " << etat1.obtenirChaineGlLightModelAmbient()
        << "\nAprès : " << etat2.obtenirChaineGlLightModelAmbient()
        << std::endl;
   }

   if (memcmp(&etat1.glLightModelLocalViewer_, &etat2.glLightModelLocalViewer_, sizeof(etat1.glLightModelLocalViewer_))) {
      o << "GL_LIGHT_MODEL_LOCAL_VIEWER :"
        << "\nAvant : " << etat1.obtenirChaineGlLightModelLocalViewer()
        << "\nAprès : " << etat2.obtenirChaineGlLightModelLocalViewer()
        << std::endl;
   }

   if (memcmp(&etat1.glLightModelTwoSide_, &etat2.glLightModelTwoSide_, sizeof(etat1.glLightModelTwoSide_))) {
      o << "GL_LIGHT_MODEL_TWO_SIDE :"
        << "\nAvant : " << etat1.obtenirChaineGlLightModelTwoSide()
        << "\nAprès : " << etat2.obtenirChaineGlLightModelTwoSide()
        << std::endl;
   }

   if (memcmp(&etat1.glLineSmooth_, &etat2.glLineSmooth_, sizeof(etat1.glLineSmooth_))) {
      o << "GL_LINE_SMOOTH :"
        << "\nAvant : " << etat1.obtenirChaineGlLineSmooth()
        << "\nAprès : " << etat2.obtenirChaineGlLineSmooth()
        << std::endl;
   }

   if (memcmp(&etat1.glLineSmoothHint_, &etat2.glLineSmoothHint_, sizeof(etat1.glLineSmoothHint_))) {
      o << "GL_LINE_SMOOTH_HINT :"
        << "\nAvant : " << etat1.obtenirChaineGlLineSmoothHint()
        << "\nAprès : " << etat2.obtenirChaineGlLineSmoothHint()
        << std::endl;
   }

   if (memcmp(&etat1.glLineStipple_, &etat2.glLineStipple_, sizeof(etat1.glLineStipple_))) {
      o << "GL_LINE_STIPPLE :"
        << "\nAvant : " << etat1.obtenirChaineGlLineStipple()
        << "\nAprès : " << etat2.obtenirChaineGlLineStipple()
        << std::endl;
   }

   if (memcmp(&etat1.glLineStipplePattern_, &etat2.glLineStipplePattern_, sizeof(etat1.glLineStipplePattern_))) {
      o << "GL_LINE_STIPPLE_PATTERN :"
        << "\nAvant : " << etat1.obtenirChaineGlLineStipplePattern()
        << "\nAprès : " << etat2.obtenirChaineGlLineStipplePattern()
        << std::endl;
   }

   if (memcmp(&etat1.glLineStippleRepeat_, &etat2.glLineStippleRepeat_, sizeof(etat1.glLineStippleRepeat_))) {
      o << "GL_LINE_STIPPLE_REPEAT :"
        << "\nAvant : " << etat1.obtenirChaineGlLineStippleRepeat()
        << "\nAprès : " << etat2.obtenirChaineGlLineStippleRepeat()
        << std::endl;
   }

   if (memcmp(&etat1.glLineWidth_, &etat2.glLineWidth_, sizeof(etat1.glLineWidth_))) {
      o << "GL_LINE_WIDTH :"
        << "\nAvant : " << etat1.obtenirChaineGlLineWidth()
        << "\nAprès : " << etat2.obtenirChaineGlLineWidth()
        << std::endl;
   }

   if (memcmp(&etat1.glLineWidthGranularity_, &etat2.glLineWidthGranularity_, sizeof(etat1.glLineWidthGranularity_))) {
      o << "GL_LINE_WIDTH_GRANULARITY :"
        << "\nAvant : " << etat1.obtenirChaineGlLineWidthGranularity()
        << "\nAprès : " << etat2.obtenirChaineGlLineWidthGranularity()
        << std::endl;
   }

   if (memcmp(etat1.gllineWidthRange_, etat2.gllineWidthRange_, sizeof(etat1.gllineWidthRange_))) {
      o << "GL_LINE_WIDTH_RANGE :"
        << "\nAvant : " << etat1.obtenirChaineGllineWidthRange()
        << "\nAprès : " << etat2.obtenirChaineGllineWidthRange()
        << std::endl;
   }

   if (memcmp(&etat1.glListBase_, &etat2.glListBase_, sizeof(etat1.glListBase_))) {
      o << "GL_LIST_BASE :"
        << "\nAvant : " << etat1.obtenirChaineGlListBase()
        << "\nAprès : " << etat2.obtenirChaineGlListBase()
        << std::endl;
   }

   if (memcmp(&etat1.glListIndex_, &etat2.glListIndex_, sizeof(etat1.glListIndex_))) {
      o << "GL_LIST_INDEX :"
        << "\nAvant : " << etat1.obtenirChaineGlListIndex()
        << "\nAprès : " << etat2.obtenirChaineGlListIndex()
        << std::endl;
   }

   if (memcmp(&etat1.glListMode_, &etat2.glListMode_, sizeof(etat1.glListMode_))) {
      o << "GL_LIST_MODE :"
        << "\nAvant : " << etat1.obtenirChaineGlListMode()
        << "\nAprès : " << etat2.obtenirChaineGlListMode()
        << std::endl;
   }

   if (memcmp(&etat1.glLogicOpMode_, &etat2.glLogicOpMode_, sizeof(etat1.glLogicOpMode_))) {
      o << "GL_LOGIC_OP_MODE :"
        << "\nAvant : " << etat1.obtenirChaineGlLogicOpMode()
        << "\nAprès : " << etat2.obtenirChaineGlLogicOpMode()
        << std::endl;
   }

   if (memcmp(&etat1.glMap1Color4_, &etat2.glMap1Color4_, sizeof(etat1.glMap1Color4_))) {
      o << "GL_MAP1_COLOR_4 :"
        << "\nAvant : " << etat1.obtenirChaineGlMap1Color4()
        << "\nAprès : " << etat2.obtenirChaineGlMap1Color4()
        << std::endl;
   }

   if (memcmp(etat1.glMap1GridDomain_, etat2.glMap1GridDomain_, sizeof(etat1.glMap1GridDomain_))) {
      o << "GL_MAP1_GRID_DOMAIN :"
        << "\nAvant : " << etat1.obtenirChaineGlMap1GridDomain()
        << "\nAprès : " << etat2.obtenirChaineGlMap1GridDomain()
        << std::endl;
   }

   if (memcmp(&etat1.glMap1GridSegments_, &etat2.glMap1GridSegments_, sizeof(etat1.glMap1GridSegments_))) {
      o << "GL_MAP1_GRID_SEGMENTS :"
        << "\nAvant : " << etat1.obtenirChaineGlMap1GridSegments()
        << "\nAprès : " << etat2.obtenirChaineGlMap1GridSegments()
        << std::endl;
   }

   if (memcmp(&etat1.glMap1Index_, &etat2.glMap1Index_, sizeof(etat1.glMap1Index_))) {
      o << "GL_MAP1_INDEX :"
        << "\nAvant : " << etat1.obtenirChaineGlMap1Index()
        << "\nAprès : " << etat2.obtenirChaineGlMap1Index()
        << std::endl;
   }

   if (memcmp(&etat1.glMap1Normal_, &etat2.glMap1Normal_, sizeof(etat1.glMap1Normal_))) {
      o << "GL_MAP1_NORMAL :"
        << "\nAvant : " << etat1.obtenirChaineGlMap1Normal()
        << "\nAprès : " << etat2.obtenirChaineGlMap1Normal()
        << std::endl;
   }

   if (memcmp(&etat1.glMap1TextureCoord1_, &etat2.glMap1TextureCoord1_, sizeof(etat1.glMap1TextureCoord1_))) {
      o << "GL_MAP1_TEXTURE_COORD_1 :"
        << "\nAvant : " << etat1.obtenirChaineGlMap1TextureCoord1()
        << "\nAprès : " << etat2.obtenirChaineGlMap1TextureCoord1()
        << std::endl;
   }

   if (memcmp(&etat1.glMap1TextureCoord2_, &etat2.glMap1TextureCoord2_, sizeof(etat1.glMap1TextureCoord2_))) {
      o << "GL_MAP1_TEXTURE_COORD_2 :"
        << "\nAvant : " << etat1.obtenirChaineGlMap1TextureCoord2()
        << "\nAprès : " << etat2.obtenirChaineGlMap1TextureCoord2()
        << std::endl;
   }

   if (memcmp(&etat1.glMap1TextureCoord3_, &etat2.glMap1TextureCoord3_, sizeof(etat1.glMap1TextureCoord3_))) {
      o << "GL_MAP1_TEXTURE_COORD_3 :"
        << "\nAvant : " << etat1.obtenirChaineGlMap1TextureCoord3()
        << "\nAprès : " << etat2.obtenirChaineGlMap1TextureCoord3()
        << std::endl;
   }

   if (memcmp(&etat1.glMap1TextureCoord4_, &etat2.glMap1TextureCoord4_, sizeof(etat1.glMap1TextureCoord4_))) {
      o << "GL_MAP1_TEXTURE_COORD_4 :"
        << "\nAvant : " << etat1.obtenirChaineGlMap1TextureCoord4()
        << "\nAprès : " << etat2.obtenirChaineGlMap1TextureCoord4()
        << std::endl;
   }

   if (memcmp(&etat1.glMap1Vertex3_, &etat2.glMap1Vertex3_, sizeof(etat1.glMap1Vertex3_))) {
      o << "GL_MAP1_VERTEX_3 :"
        << "\nAvant : " << etat1.obtenirChaineGlMap1Vertex3()
        << "\nAprès : " << etat2.obtenirChaineGlMap1Vertex3()
        << std::endl;
   }

   if (memcmp(&etat1.glMap1Vertex4_, &etat2.glMap1Vertex4_, sizeof(etat1.glMap1Vertex4_))) {
      o << "GL_MAP1_VERTEX_4 :"
        << "\nAvant : " << etat1.obtenirChaineGlMap1Vertex4()
        << "\nAprès : " << etat2.obtenirChaineGlMap1Vertex4()
        << std::endl;
   }

   if (memcmp(&etat1.glMap2Color4_, &etat2.glMap2Color4_, sizeof(etat1.glMap2Color4_))) {
      o << "GL_MAP2_COLOR_4 :"
        << "\nAvant : " << etat1.obtenirChaineGlMap2Color4()
        << "\nAprès : " << etat2.obtenirChaineGlMap2Color4()
        << std::endl;
   }

   if (memcmp(etat1.glMap2GridDomain_, etat2.glMap2GridDomain_, sizeof(etat1.glMap2GridDomain_))) {
      o << "GL_MAP2_GRID_DOMAIN :"
        << "\nAvant : " << etat1.obtenirChaineGlMap2GridDomain()
        << "\nAprès : " << etat2.obtenirChaineGlMap2GridDomain()
        << std::endl;
   }

   if (memcmp(etat1.glMap2GridSegments_, etat2.glMap2GridSegments_, sizeof(etat1.glMap2GridSegments_))) {
      o << "GL_MAP2_GRID_SEGMENTS :"
        << "\nAvant : " << etat1.obtenirChaineGlMap2GridSegments()
        << "\nAprès : " << etat2.obtenirChaineGlMap2GridSegments()
        << std::endl;
   }

   if (memcmp(&etat1.glMap2Index_, &etat2.glMap2Index_, sizeof(etat1.glMap2Index_))) {
      o << "GL_MAP2_INDEX :"
        << "\nAvant : " << etat1.obtenirChaineGlMap2Index()
        << "\nAprès : " << etat2.obtenirChaineGlMap2Index()
        << std::endl;
   }

   if (memcmp(&etat1.glMap2Normal_, &etat2.glMap2Normal_, sizeof(etat1.glMap2Normal_))) {
      o << "GL_MAP2_NORMAL :"
        << "\nAvant : " << etat1.obtenirChaineGlMap2Normal()
        << "\nAprès : " << etat2.obtenirChaineGlMap2Normal()
        << std::endl;
   }

   if (memcmp(&etat1.glMap2TextureCoord1_, &etat2.glMap2TextureCoord1_, sizeof(etat1.glMap2TextureCoord1_))) {
      o << "GL_MAP2_TEXTURE_COORD_1 :"
        << "\nAvant : " << etat1.obtenirChaineGlMap2TextureCoord1()
        << "\nAprès : " << etat2.obtenirChaineGlMap2TextureCoord1()
        << std::endl;
   }

   if (memcmp(&etat1.glMap2TextureCoord2_, &etat2.glMap2TextureCoord2_, sizeof(etat1.glMap2TextureCoord2_))) {
      o << "GL_MAP2_TEXTURE_COORD_2 :"
        << "\nAvant : " << etat1.obtenirChaineGlMap2TextureCoord2()
        << "\nAprès : " << etat2.obtenirChaineGlMap2TextureCoord2()
        << std::endl;
   }

   if (memcmp(&etat1.glMap2TextureCoord3_, &etat2.glMap2TextureCoord3_, sizeof(etat1.glMap2TextureCoord3_))) {
      o << "GL_MAP2_TEXTURE_COORD_3 :"
        << "\nAvant : " << etat1.obtenirChaineGlMap2TextureCoord3()
        << "\nAprès : " << etat2.obtenirChaineGlMap2TextureCoord3()
        << std::endl;
   }

   if (memcmp(&etat1.glMap2TextureCoord4_, &etat2.glMap2TextureCoord4_, sizeof(etat1.glMap2TextureCoord4_))) {
      o << "GL_MAP2_TEXTURE_COORD_4 :"
        << "\nAvant : " << etat1.obtenirChaineGlMap2TextureCoord4()
        << "\nAprès : " << etat2.obtenirChaineGlMap2TextureCoord4()
        << std::endl;
   }

   if (memcmp(&etat1.glMap2Vertex3_, &etat2.glMap2Vertex3_, sizeof(etat1.glMap2Vertex3_))) {
      o << "GL_MAP2_VERTEX_3 :"
        << "\nAvant : " << etat1.obtenirChaineGlMap2Vertex3()
        << "\nAprès : " << etat2.obtenirChaineGlMap2Vertex3()
        << std::endl;
   }

   if (memcmp(&etat1.glMap2Vertex4_, &etat2.glMap2Vertex4_, sizeof(etat1.glMap2Vertex4_))) {
      o << "GL_MAP2_VERTEX_4 :"
        << "\nAvant : " << etat1.obtenirChaineGlMap2Vertex4()
        << "\nAprès : " << etat2.obtenirChaineGlMap2Vertex4()
        << std::endl;
   }

   if (memcmp(&etat1.glMapColor_, &etat2.glMapColor_, sizeof(etat1.glMapColor_))) {
      o << "GL_MAP_COLOR :"
        << "\nAvant : " << etat1.obtenirChaineGlMapColor()
        << "\nAprès : " << etat2.obtenirChaineGlMapColor()
        << std::endl;
   }

   if (memcmp(&etat1.glMapStencil_, &etat2.glMapStencil_, sizeof(etat1.glMapStencil_))) {
      o << "GL_MAP_STENCIL :"
        << "\nAvant : " << etat1.obtenirChaineGlMapStencil()
        << "\nAprès : " << etat2.obtenirChaineGlMapStencil()
        << std::endl;
   }

   if (memcmp(&etat1.glMatrixMode_, &etat2.glMatrixMode_, sizeof(etat1.glMatrixMode_))) {
      o << "GL_MATRIX_MODE :"
        << "\nAvant : " << etat1.obtenirChaineGlMatrixMode()
        << "\nAprès : " << etat2.obtenirChaineGlMatrixMode()
        << std::endl;
   }

   if (memcmp(&etat1.glMaxClientAttribStackDepth_, &etat2.glMaxClientAttribStackDepth_, sizeof(etat1.glMaxClientAttribStackDepth_))) {
      o << "GL_MAX_CLIENT_ATTRIB_STACK_DEPTH :"
        << "\nAvant : " << etat1.obtenirChaineGlMaxClientAttribStackDepth()
        << "\nAprès : " << etat2.obtenirChaineGlMaxClientAttribStackDepth()
        << std::endl;
   }

   if (memcmp(&etat1.glMaxAttribStackDepth_, &etat2.glMaxAttribStackDepth_, sizeof(etat1.glMaxAttribStackDepth_))) {
      o << "GL_MAX_ATTRIB_STACK_DEPTH :"
        << "\nAvant : " << etat1.obtenirChaineGlMaxAttribStackDepth()
        << "\nAprès : " << etat2.obtenirChaineGlMaxAttribStackDepth()
        << std::endl;
   }

   if (memcmp(&etat1.glMaxClipPlanes_, &etat2.glMaxClipPlanes_, sizeof(etat1.glMaxClipPlanes_))) {
      o << "GL_MAX_CLIP_PLANES :"
        << "\nAvant : " << etat1.obtenirChaineGlMaxClipPlanes()
        << "\nAprès : " << etat2.obtenirChaineGlMaxClipPlanes()
        << std::endl;
   }

   if (memcmp(&etat1.glMaxEvalOrder_, &etat2.glMaxEvalOrder_, sizeof(etat1.glMaxEvalOrder_))) {
      o << "GL_MAX_EVAL_ORDER :"
        << "\nAvant : " << etat1.obtenirChaineGlMaxEvalOrder()
        << "\nAprès : " << etat2.obtenirChaineGlMaxEvalOrder()
        << std::endl;
   }

   if (memcmp(&etat1.glMaxLights_, &etat2.glMaxLights_, sizeof(etat1.glMaxLights_))) {
      o << "GL_MAX_LIGHTS :"
        << "\nAvant : " << etat1.obtenirChaineGlMaxLights()
        << "\nAprès : " << etat2.obtenirChaineGlMaxLights()
        << std::endl;
   }

   if (memcmp(&etat1.glMaxListNesting_, &etat2.glMaxListNesting_, sizeof(etat1.glMaxListNesting_))) {
      o << "GL_MAX_LIST_NESTING :"
        << "\nAvant : " << etat1.obtenirChaineGlMaxListNesting()
        << "\nAprès : " << etat2.obtenirChaineGlMaxListNesting()
        << std::endl;
   }

   if (memcmp(&etat1.glMaxModelviewStackDepth_, &etat2.glMaxModelviewStackDepth_, sizeof(etat1.glMaxModelviewStackDepth_))) {
      o << "GL_MAX_MODELVIEW_STACK_DEPTH :"
        << "\nAvant : " << etat1.obtenirChaineGlMaxModelviewStackDepth()
        << "\nAprès : " << etat2.obtenirChaineGlMaxModelviewStackDepth()
        << std::endl;
   }

   if (memcmp(&etat1.glMaxNameStackDepth_, &etat2.glMaxNameStackDepth_, sizeof(etat1.glMaxNameStackDepth_))) {
      o << "GL_MAX_NAME_STACK_DEPTH :"
        << "\nAvant : " << etat1.obtenirChaineGlMaxNameStackDepth()
        << "\nAprès : " << etat2.obtenirChaineGlMaxNameStackDepth()
        << std::endl;
   }

   if (memcmp(&etat1.glMaxPixelMapTable_, &etat2.glMaxPixelMapTable_, sizeof(etat1.glMaxPixelMapTable_))) {
      o << "GL_MAX_PIXEL_MAP_TABLE :"
        << "\nAvant : " << etat1.obtenirChaineGlMaxPixelMapTable()
        << "\nAprès : " << etat2.obtenirChaineGlMaxPixelMapTable()
        << std::endl;
   }

   if (memcmp(&etat1.glMaxProjectionStackDepth_, &etat2.glMaxProjectionStackDepth_, sizeof(etat1.glMaxProjectionStackDepth_))) {
      o << "GL_MAX_PROJECTION_STACK_DEPTH :"
        << "\nAvant : " << etat1.obtenirChaineGlMaxProjectionStackDepth()
        << "\nAprès : " << etat2.obtenirChaineGlMaxProjectionStackDepth()
        << std::endl;
   }

   if (memcmp(&etat1.glMaxTextureSize_, &etat2.glMaxTextureSize_, sizeof(etat1.glMaxTextureSize_))) {
      o << "GL_MAX_TEXTURE_SIZE :"
        << "\nAvant : " << etat1.obtenirChaineGlMaxTextureSize()
        << "\nAprès : " << etat2.obtenirChaineGlMaxTextureSize()
        << std::endl;
   }

   if (memcmp(&etat1.glMaxTextureStackDepth_, &etat2.glMaxTextureStackDepth_, sizeof(etat1.glMaxTextureStackDepth_))) {
      o << "GL_MAX_TEXTURE_STACK_DEPTH :"
        << "\nAvant : " << etat1.obtenirChaineGlMaxTextureStackDepth()
        << "\nAprès : " << etat2.obtenirChaineGlMaxTextureStackDepth()
        << std::endl;
   }

   if (memcmp(etat1.glMaxViewportDims_, etat2.glMaxViewportDims_, sizeof(etat1.glMaxViewportDims_))) {
      o << "GL_MAX_VIEWPORT_DIMS :"
        << "\nAvant : " << etat1.obtenirChaineGlMaxViewportDims()
        << "\nAprès : " << etat2.obtenirChaineGlMaxViewportDims()
        << std::endl;
   }

   if (memcmp(etat1.glModelviewMatrix_, etat2.glModelviewMatrix_, sizeof(etat1.glModelviewMatrix_))) {
      o << "GL_MODELVIEW_MATRIX :"
        << "\nAvant : " << etat1.obtenirChaineGlModelviewMatrix()
        << "\nAprès : " << etat2.obtenirChaineGlModelviewMatrix()
        << std::endl;
   }

   if (memcmp(&etat1.glModelviewStackDepth_, &etat2.glModelviewStackDepth_, sizeof(etat1.glModelviewStackDepth_))) {
      o << "GL_MODELVIEW_STACK_DEPTH :"
        << "\nAvant : " << etat1.obtenirChaineGlModelviewStackDepth()
        << "\nAprès : " << etat2.obtenirChaineGlModelviewStackDepth()
        << std::endl;
   }

   if (memcmp(&etat1.glNameStackDepth_, &etat2.glNameStackDepth_, sizeof(etat1.glNameStackDepth_))) {
      o << "GL_NAME_STACK_DEPTH :"
        << "\nAvant : " << etat1.obtenirChaineGlNameStackDepth()
        << "\nAprès : " << etat2.obtenirChaineGlNameStackDepth()
        << std::endl;
   }

   if (memcmp(&etat1.glNormalArray_, &etat2.glNormalArray_, sizeof(etat1.glNormalArray_))) {
      o << "GL_NORMAL_ARRAY :"
        << "\nAvant : " << etat1.obtenirChaineGlNormalArray()
        << "\nAprès : " << etat2.obtenirChaineGlNormalArray()
        << std::endl;
   }

   if (memcmp(&etat1.glNormalArrayStride_, &etat2.glNormalArrayStride_, sizeof(etat1.glNormalArrayStride_))) {
      o << "GL_NORMAL_ARRAY_STRIDE :"
        << "\nAvant : " << etat1.obtenirChaineGlNormalArrayStride()
        << "\nAprès : " << etat2.obtenirChaineGlNormalArrayStride()
        << std::endl;
   }

   if (memcmp(&etat1.glNormalArrayType_, &etat2.glNormalArrayType_, sizeof(etat1.glNormalArrayType_))) {
      o << "GL_NORMAL_ARRAY_TYPE :"
        << "\nAvant : " << etat1.obtenirChaineGlNormalArrayType()
        << "\nAprès : " << etat2.obtenirChaineGlNormalArrayType()
        << std::endl;
   }

   if (memcmp(&etat1.glNormalize_, &etat2.glNormalize_, sizeof(etat1.glNormalize_))) {
      o << "GL_NORMALIZE :"
        << "\nAvant : " << etat1.obtenirChaineGlNormalize()
        << "\nAprès : " << etat2.obtenirChaineGlNormalize()
        << std::endl;
   }

   if (memcmp(&etat1.glPackAlignment_, &etat2.glPackAlignment_, sizeof(etat1.glPackAlignment_))) {
      o << "GL_PACK_ALIGNMENT :"
        << "\nAvant : " << etat1.obtenirChaineGlPackAlignment()
        << "\nAprès : " << etat2.obtenirChaineGlPackAlignment()
        << std::endl;
   }

   if (memcmp(&etat1.glPackLsbFirst_, &etat2.glPackLsbFirst_, sizeof(etat1.glPackLsbFirst_))) {
      o << "GL_PACK_LSB_FIRST :"
        << "\nAvant : " << etat1.obtenirChaineGlPackLsbFirst()
        << "\nAprès : " << etat2.obtenirChaineGlPackLsbFirst()
        << std::endl;
   }

   if (memcmp(&etat1.glPackRowLength_, &etat2.glPackRowLength_, sizeof(etat1.glPackRowLength_))) {
      o << "GL_PACK_ROW_LENGTH :"
        << "\nAvant : " << etat1.obtenirChaineGlPackRowLength()
        << "\nAprès : " << etat2.obtenirChaineGlPackRowLength()
        << std::endl;
   }

   if (memcmp(&etat1.glPackSkipPixels_, &etat2.glPackSkipPixels_, sizeof(etat1.glPackSkipPixels_))) {
      o << "GL_PACK_SKIP_PIXELS :"
        << "\nAvant : " << etat1.obtenirChaineGlPackSkipPixels()
        << "\nAprès : " << etat2.obtenirChaineGlPackSkipPixels()
        << std::endl;
   }

   if (memcmp(&etat1.glPackSkipRows_, &etat2.glPackSkipRows_, sizeof(etat1.glPackSkipRows_))) {
      o << "GL_PACK_SKIP_ROWS :"
        << "\nAvant : " << etat1.obtenirChaineGlPackSkipRows()
        << "\nAprès : " << etat2.obtenirChaineGlPackSkipRows()
        << std::endl;
   }

   if (memcmp(&etat1.glPackSwapBytes_, &etat2.glPackSwapBytes_, sizeof(etat1.glPackSwapBytes_))) {
      o << "GL_PACK_SWAP_BYTES :"
        << "\nAvant : " << etat1.obtenirChaineGlPackSwapBytes()
        << "\nAprès : " << etat2.obtenirChaineGlPackSwapBytes()
        << std::endl;
   }

   if (memcmp(&etat1.glPerspectiveCorrectionHint_, &etat2.glPerspectiveCorrectionHint_, sizeof(etat1.glPerspectiveCorrectionHint_))) {
      o << "GL_PERSPECTIVE_CORRECTION_HINT :"
        << "\nAvant : " << etat1.obtenirChaineGlPerspectiveCorrectionHint()
        << "\nAprès : " << etat2.obtenirChaineGlPerspectiveCorrectionHint()
        << std::endl;
   }

   if (memcmp(&etat1.glPixelMapAToASize_, &etat2.glPixelMapAToASize_, sizeof(etat1.glPixelMapAToASize_))) {
      o << "GL_PIXEL_MAP_A_TO_A_SIZE :"
        << "\nAvant : " << etat1.obtenirChaineGlPixelMapAToASize()
        << "\nAprès : " << etat2.obtenirChaineGlPixelMapAToASize()
        << std::endl;
   }

   if (memcmp(&etat1.glPixelMapBToBSize_, &etat2.glPixelMapBToBSize_, sizeof(etat1.glPixelMapBToBSize_))) {
      o << "GL_PIXEL_MAP_B_TO_B_SIZE :"
        << "\nAvant : " << etat1.obtenirChaineGlPixelMapBToBSize()
        << "\nAprès : " << etat2.obtenirChaineGlPixelMapBToBSize()
        << std::endl;
   }

   if (memcmp(&etat1.glPixelMapGToGSize_, &etat2.glPixelMapGToGSize_, sizeof(etat1.glPixelMapGToGSize_))) {
      o << "GL_PIXEL_MAP_G_TO_G_SIZE :"
        << "\nAvant : " << etat1.obtenirChaineGlPixelMapGToGSize()
        << "\nAprès : " << etat2.obtenirChaineGlPixelMapGToGSize()
        << std::endl;
   }

   if (memcmp(&etat1.glPixelMapIToASize_, &etat2.glPixelMapIToASize_, sizeof(etat1.glPixelMapIToASize_))) {
      o << "GL_PIXEL_MAP_I_TO_A_SIZE :"
        << "\nAvant : " << etat1.obtenirChaineGlPixelMapIToASize()
        << "\nAprès : " << etat2.obtenirChaineGlPixelMapIToASize()
        << std::endl;
   }

   if (memcmp(&etat1.glPixelMapIToBSize_, &etat2.glPixelMapIToBSize_, sizeof(etat1.glPixelMapIToBSize_))) {
      o << "GL_PIXEL_MAP_I_TO_B_SIZE :"
        << "\nAvant : " << etat1.obtenirChaineGlPixelMapIToBSize()
        << "\nAprès : " << etat2.obtenirChaineGlPixelMapIToBSize()
        << std::endl;
   }

   if (memcmp(&etat1.glPixelMapIToGSize_, &etat2.glPixelMapIToGSize_, sizeof(etat1.glPixelMapIToGSize_))) {
      o << "GL_PIXEL_MAP_I_TO_G_SIZE :"
        << "\nAvant : " << etat1.obtenirChaineGlPixelMapIToGSize()
        << "\nAprès : " << etat2.obtenirChaineGlPixelMapIToGSize()
        << std::endl;
   }

   if (memcmp(&etat1.glPixelMapIToISize_, &etat2.glPixelMapIToISize_, sizeof(etat1.glPixelMapIToISize_))) {
      o << "GL_PIXEL_MAP_I_TO_I_SIZE :"
        << "\nAvant : " << etat1.obtenirChaineGlPixelMapIToISize()
        << "\nAprès : " << etat2.obtenirChaineGlPixelMapIToISize()
        << std::endl;
   }

   if (memcmp(&etat1.glPixelMapIToRSize_, &etat2.glPixelMapIToRSize_, sizeof(etat1.glPixelMapIToRSize_))) {
      o << "GL_PIXEL_MAP_I_TO_R_SIZE :"
        << "\nAvant : " << etat1.obtenirChaineGlPixelMapIToRSize()
        << "\nAprès : " << etat2.obtenirChaineGlPixelMapIToRSize()
        << std::endl;
   }

   if (memcmp(&etat1.glPixelMapRToRSize_, &etat2.glPixelMapRToRSize_, sizeof(etat1.glPixelMapRToRSize_))) {
      o << "GL_PIXEL_MAP_R_TO_R_SIZE :"
        << "\nAvant : " << etat1.obtenirChaineGlPixelMapRToRSize()
        << "\nAprès : " << etat2.obtenirChaineGlPixelMapRToRSize()
        << std::endl;
   }

   if (memcmp(&etat1.glPixelMapSToSSize_, &etat2.glPixelMapSToSSize_, sizeof(etat1.glPixelMapSToSSize_))) {
      o << "GL_PIXEL_MAP_S_TO_S_SIZE :"
        << "\nAvant : " << etat1.obtenirChaineGlPixelMapSToSSize()
        << "\nAprès : " << etat2.obtenirChaineGlPixelMapSToSSize()
        << std::endl;
   }

   if (memcmp(&etat1.glPointSize_, &etat2.glPointSize_, sizeof(etat1.glPointSize_))) {
      o << "GL_POINT_SIZE :"
        << "\nAvant : " << etat1.obtenirChaineGlPointSize()
        << "\nAprès : " << etat2.obtenirChaineGlPointSize()
        << std::endl;
   }

   if (memcmp(&etat1.glPointSizeGranularity_, &etat2.glPointSizeGranularity_, sizeof(etat1.glPointSizeGranularity_))) {
      o << "GL_POINT_SIZE_GRANULARITY :"
        << "\nAvant : " << etat1.obtenirChaineGlPointSizeGranularity()
        << "\nAprès : " << etat2.obtenirChaineGlPointSizeGranularity()
        << std::endl;
   }

   if (memcmp(etat1.glPointSizeRange_, etat2.glPointSizeRange_, sizeof(etat1.glPointSizeRange_))) {
      o << "GL_POINT_SIZE_RANGE :"
        << "\nAvant : " << etat1.obtenirChaineGlPointSizeRange()
        << "\nAprès : " << etat2.obtenirChaineGlPointSizeRange()
        << std::endl;
   }

   if (memcmp(&etat1.glPointSmooth_, &etat2.glPointSmooth_, sizeof(etat1.glPointSmooth_))) {
      o << "GL_POINT_SMOOTH :"
        << "\nAvant : " << etat1.obtenirChaineGlPointSmooth()
        << "\nAprès : " << etat2.obtenirChaineGlPointSmooth()
        << std::endl;
   }

   if (memcmp(&etat1.glPointSmoothHint_, &etat2.glPointSmoothHint_, sizeof(etat1.glPointSmoothHint_))) {
      o << "GL_POINT_SMOOTH_HINT :"
        << "\nAvant : " << etat1.obtenirChaineGlPointSmoothHint()
        << "\nAprès : " << etat2.obtenirChaineGlPointSmoothHint()
        << std::endl;
   }

   if (memcmp(etat1.glPolygonMode_, etat2.glPolygonMode_, sizeof(etat1.glPolygonMode_))) {
      o << "GL_POLYGON_MODE :"
        << "\nAvant : " << etat1.obtenirChaineGlPolygonMode()
        << "\nAprès : " << etat2.obtenirChaineGlPolygonMode()
        << std::endl;
   }

   if (memcmp(&etat1.glPolygonOffsetFactor_, &etat2.glPolygonOffsetFactor_, sizeof(etat1.glPolygonOffsetFactor_))) {
      o << "GL_POLYGON_OFFSET_FACTOR :"
        << "\nAvant : " << etat1.obtenirChaineGlPolygonOffsetFactor()
        << "\nAprès : " << etat2.obtenirChaineGlPolygonOffsetFactor()
        << std::endl;
   }

   if (memcmp(&etat1.glPolygonOffsetUnits_, &etat2.glPolygonOffsetUnits_, sizeof(etat1.glPolygonOffsetUnits_))) {
      o << "GL_POLYGON_OFFSET_UNITS :"
        << "\nAvant : " << etat1.obtenirChaineGlPolygonOffsetUnits()
        << "\nAprès : " << etat2.obtenirChaineGlPolygonOffsetUnits()
        << std::endl;
   }

   if (memcmp(&etat1.glPolygonOffsetFill_, &etat2.glPolygonOffsetFill_, sizeof(etat1.glPolygonOffsetFill_))) {
      o << "GL_POLYGON_OFFSET_FILL :"
        << "\nAvant : " << etat1.obtenirChaineGlPolygonOffsetFill()
        << "\nAprès : " << etat2.obtenirChaineGlPolygonOffsetFill()
        << std::endl;
   }

   if (memcmp(&etat1.glPolygonOffsetLine_, &etat2.glPolygonOffsetLine_, sizeof(etat1.glPolygonOffsetLine_))) {
      o << "GL_POLYGON_OFFSET_LINE :"
        << "\nAvant : " << etat1.obtenirChaineGlPolygonOffsetLine()
        << "\nAprès : " << etat2.obtenirChaineGlPolygonOffsetLine()
        << std::endl;
   }

   if (memcmp(&etat1.glPolygonOffsetPoint_, &etat2.glPolygonOffsetPoint_, sizeof(etat1.glPolygonOffsetPoint_))) {
      o << "GL_POLYGON_OFFSET_POINT :"
        << "\nAvant : " << etat1.obtenirChaineGlPolygonOffsetPoint()
        << "\nAprès : " << etat2.obtenirChaineGlPolygonOffsetPoint()
        << std::endl;
   }

   if (memcmp(&etat1.glPolygonSmooth_, &etat2.glPolygonSmooth_, sizeof(etat1.glPolygonSmooth_))) {
      o << "GL_POLYGON_SMOOTH :"
        << "\nAvant : " << etat1.obtenirChaineGlPolygonSmooth()
        << "\nAprès : " << etat2.obtenirChaineGlPolygonSmooth()
        << std::endl;
   }

   if (memcmp(&etat1.glPolygonSmoothHint_, &etat2.glPolygonSmoothHint_, sizeof(etat1.glPolygonSmoothHint_))) {
      o << "GL_POLYGON_SMOOTH_HINT :"
        << "\nAvant : " << etat1.obtenirChaineGlPolygonSmoothHint()
        << "\nAprès : " << etat2.obtenirChaineGlPolygonSmoothHint()
        << std::endl;
   }

   if (memcmp(&etat1.glPolygonStipple_, &etat2.glPolygonStipple_, sizeof(etat1.glPolygonStipple_))) {
      o << "GL_POLYGON_STIPPLE :"
        << "\nAvant : " << etat1.obtenirChaineGlPolygonStipple()
        << "\nAprès : " << etat2.obtenirChaineGlPolygonStipple()
        << std::endl;
   }

   if (memcmp(etat1.glProjectionMatrix_, etat2.glProjectionMatrix_, sizeof(etat1.glProjectionMatrix_))) {
      o << "GL_PROJECTION_MATRIX :"
        << "\nAvant : " << etat1.obtenirChaineGlProjectionMatrix()
        << "\nAprès : " << etat2.obtenirChaineGlProjectionMatrix()
        << std::endl;
   }

   if (memcmp(&etat1.glProjectionStackDepth_, &etat2.glProjectionStackDepth_, sizeof(etat1.glProjectionStackDepth_))) {
      o << "GL_PROJECTION_STACK_DEPTH :"
        << "\nAvant : " << etat1.obtenirChaineGlProjectionStackDepth()
        << "\nAprès : " << etat2.obtenirChaineGlProjectionStackDepth()
        << std::endl;
   }

   if (memcmp(&etat1.glReadBuffer_, &etat2.glReadBuffer_, sizeof(etat1.glReadBuffer_))) {
      o << "GL_READ_BUFFER :"
        << "\nAvant : " << etat1.obtenirChaineGlReadBuffer()
        << "\nAprès : " << etat2.obtenirChaineGlReadBuffer()
        << std::endl;
   }

   if (memcmp(&etat1.glRedBias_, &etat2.glRedBias_, sizeof(etat1.glRedBias_))) {
      o << "GL_RED_BIAS :"
        << "\nAvant : " << etat1.obtenirChaineGlRedBias()
        << "\nAprès : " << etat2.obtenirChaineGlRedBias()
        << std::endl;
   }

   if (memcmp(&etat1.glRedBits_, &etat2.glRedBits_, sizeof(etat1.glRedBits_))) {
      o << "GL_RED_BITS :"
        << "\nAvant : " << etat1.obtenirChaineGlRedBits()
        << "\nAprès : " << etat2.obtenirChaineGlRedBits()
        << std::endl;
   }

   if (memcmp(&etat1.glRedScale_, &etat2.glRedScale_, sizeof(etat1.glRedScale_))) {
      o << "GL_RED_SCALE :"
        << "\nAvant : " << etat1.obtenirChaineGlRedScale()
        << "\nAprès : " << etat2.obtenirChaineGlRedScale()
        << std::endl;
   }

   if (memcmp(&etat1.glRenderMode_, &etat2.glRenderMode_, sizeof(etat1.glRenderMode_))) {
      o << "GL_RENDER_MODE :"
        << "\nAvant : " << etat1.obtenirChaineGlRenderMode()
        << "\nAprès : " << etat2.obtenirChaineGlRenderMode()
        << std::endl;
   }

   if (memcmp(&etat1.glRgbaMode_, &etat2.glRgbaMode_, sizeof(etat1.glRgbaMode_))) {
      o << "GL_RGBA_MODE :"
        << "\nAvant : " << etat1.obtenirChaineGlRgbaMode()
        << "\nAprès : " << etat2.obtenirChaineGlRgbaMode()
        << std::endl;
   }

   if (memcmp(etat1.glScissorBox_, etat2.glScissorBox_, sizeof(etat1.glScissorBox_))) {
      o << "GL_SCISSOR_BOX :"
        << "\nAvant : " << etat1.obtenirChaineGlScissorBox()
        << "\nAprès : " << etat2.obtenirChaineGlScissorBox()
        << std::endl;
   }

   if (memcmp(&etat1.glScissorTest_, &etat2.glScissorTest_, sizeof(etat1.glScissorTest_))) {
      o << "GL_SCISSOR_TEST :"
        << "\nAvant : " << etat1.obtenirChaineGlScissorTest()
        << "\nAprès : " << etat2.obtenirChaineGlScissorTest()
        << std::endl;
   }

   if (memcmp(&etat1.glSelectionBufferSize_, &etat2.glSelectionBufferSize_, sizeof(etat1.glSelectionBufferSize_))) {
      o << "GL_SELECTION_BUFFER_SIZE :"
        << "\nAvant : " << etat1.obtenirChaineGlSelectionBufferSize()
        << "\nAprès : " << etat2.obtenirChaineGlSelectionBufferSize()
        << std::endl;
   }

   if (memcmp(&etat1.glShadeModel_, &etat2.glShadeModel_, sizeof(etat1.glShadeModel_))) {
      o << "GL_SHADE_MODEL :"
        << "\nAvant : " << etat1.obtenirChaineGlShadeModel()
        << "\nAprès : " << etat2.obtenirChaineGlShadeModel()
        << std::endl;
   }

   if (memcmp(&etat1.glStencilBits_, &etat2.glStencilBits_, sizeof(etat1.glStencilBits_))) {
      o << "GL_STENCIL_BITS :"
        << "\nAvant : " << etat1.obtenirChaineGlStencilBits()
        << "\nAprès : " << etat2.obtenirChaineGlStencilBits()
        << std::endl;
   }

   if (memcmp(&etat1.glStencilClearValue_, &etat2.glStencilClearValue_, sizeof(etat1.glStencilClearValue_))) {
      o << "GL_STENCIL_CLEAR_VALUE :"
        << "\nAvant : " << etat1.obtenirChaineGlStencilClearValue()
        << "\nAprès : " << etat2.obtenirChaineGlStencilClearValue()
        << std::endl;
   }

   if (memcmp(&etat1.glStencilFail_, &etat2.glStencilFail_, sizeof(etat1.glStencilFail_))) {
      o << "GL_STENCIL_FAIL :"
        << "\nAvant : " << etat1.obtenirChaineGlStencilFail()
        << "\nAprès : " << etat2.obtenirChaineGlStencilFail()
        << std::endl;
   }

   if (memcmp(&etat1.glStencilFunc_, &etat2.glStencilFunc_, sizeof(etat1.glStencilFunc_))) {
      o << "GL_STENCIL_FUNC :"
        << "\nAvant : " << etat1.obtenirChaineGlStencilFunc()
        << "\nAprès : " << etat2.obtenirChaineGlStencilFunc()
        << std::endl;
   }

   if (memcmp(&etat1.glStencilPassDepthFail_, &etat2.glStencilPassDepthFail_, sizeof(etat1.glStencilPassDepthFail_))) {
      o << "GL_STENCIL_PASS_DEPTH_FAIL :"
        << "\nAvant : " << etat1.obtenirChaineGlStencilPassDepthFail()
        << "\nAprès : " << etat2.obtenirChaineGlStencilPassDepthFail()
        << std::endl;
   }

   if (memcmp(&etat1.glStencilPassDepthPass_, &etat2.glStencilPassDepthPass_, sizeof(etat1.glStencilPassDepthPass_))) {
      o << "GL_STENCIL_PASS_DEPTH_PASS :"
        << "\nAvant : " << etat1.obtenirChaineGlStencilPassDepthPass()
        << "\nAprès : " << etat2.obtenirChaineGlStencilPassDepthPass()
        << std::endl;
   }

   if (memcmp(&etat1.glStencilRef_, &etat2.glStencilRef_, sizeof(etat1.glStencilRef_))) {
      o << "GL_STENCIL_REF :"
        << "\nAvant : " << etat1.obtenirChaineGlStencilRef()
        << "\nAprès : " << etat2.obtenirChaineGlStencilRef()
        << std::endl;
   }

   if (memcmp(&etat1.glStencilTest_, &etat2.glStencilTest_, sizeof(etat1.glStencilTest_))) {
      o << "GL_STENCIL_TEST :"
        << "\nAvant : " << etat1.obtenirChaineGlStencilTest()
        << "\nAprès : " << etat2.obtenirChaineGlStencilTest()
        << std::endl;
   }

   if (memcmp(&etat1.glStencilValueMask_, &etat2.glStencilValueMask_, sizeof(etat1.glStencilValueMask_))) {
      o << "GL_STENCIL_VALUE_MASK :"
        << "\nAvant : " << etat1.obtenirChaineGlStencilValueMask()
        << "\nAprès : " << etat2.obtenirChaineGlStencilValueMask()
        << std::endl;
   }

   if (memcmp(&etat1.glStencilWritemask_, &etat2.glStencilWritemask_, sizeof(etat1.glStencilWritemask_))) {
      o << "GL_STENCIL_WRITEMASK :"
        << "\nAvant : " << etat1.obtenirChaineGlStencilWritemask()
        << "\nAprès : " << etat2.obtenirChaineGlStencilWritemask()
        << std::endl;
   }

   if (memcmp(&etat1.glStereo_, &etat2.glStereo_, sizeof(etat1.glStereo_))) {
      o << "GL_STEREO :"
        << "\nAvant : " << etat1.obtenirChaineGlStereo()
        << "\nAprès : " << etat2.obtenirChaineGlStereo()
        << std::endl;
   }

   if (memcmp(&etat1.glSubpixelBits_, &etat2.glSubpixelBits_, sizeof(etat1.glSubpixelBits_))) {
      o << "GL_SUBPIXEL_BITS :"
        << "\nAvant : " << etat1.obtenirChaineGlSubpixelBits()
        << "\nAprès : " << etat2.obtenirChaineGlSubpixelBits()
        << std::endl;
   }

   if (memcmp(&etat1.glTexture1D_, &etat2.glTexture1D_, sizeof(etat1.glTexture1D_))) {
      o << "GL_TEXTURE_1D :"
        << "\nAvant : " << etat1.obtenirChaineGlTexture1D()
        << "\nAprès : " << etat2.obtenirChaineGlTexture1D()
        << std::endl;
   }

   if (memcmp(&etat1.glTextureBinding1D_, &etat2.glTextureBinding1D_, sizeof(etat1.glTextureBinding1D_))) {
      o << "GL_TEXTURE_BINDING_1D :"
        << "\nAvant : " << etat1.obtenirChaineGlTextureBinding1D()
        << "\nAprès : " << etat2.obtenirChaineGlTextureBinding1D()
        << std::endl;
   }

   if (memcmp(&etat1.glTexture2D_, &etat2.glTexture2D_, sizeof(etat1.glTexture2D_))) {
      o << "GL_TEXTURE_2D :"
        << "\nAvant : " << etat1.obtenirChaineGlTexture2D()
        << "\nAprès : " << etat2.obtenirChaineGlTexture2D()
        << std::endl;
   }

   if (memcmp(&etat1.glTextureBinding2D_, &etat2.glTextureBinding2D_, sizeof(etat1.glTextureBinding2D_))) {
      o << "GL_TEXTURE_BINDING_2D :"
        << "\nAvant : " << etat1.obtenirChaineGlTextureBinding2D()
        << "\nAprès : " << etat2.obtenirChaineGlTextureBinding2D()
        << std::endl;
   }

   if (memcmp(&etat1.glTextureCoordArray_, &etat2.glTextureCoordArray_, sizeof(etat1.glTextureCoordArray_))) {
      o << "GL_TEXTURE_COORD_ARRAY :"
        << "\nAvant : " << etat1.obtenirChaineGlTextureCoordArray()
        << "\nAprès : " << etat2.obtenirChaineGlTextureCoordArray()
        << std::endl;
   }

   if (memcmp(&etat1.glTextureCoordArraySize_, &etat2.glTextureCoordArraySize_, sizeof(etat1.glTextureCoordArraySize_))) {
      o << "GL_TEXTURE_COORD_ARRAY_SIZE :"
        << "\nAvant : " << etat1.obtenirChaineGlTextureCoordArraySize()
        << "\nAprès : " << etat2.obtenirChaineGlTextureCoordArraySize()
        << std::endl;
   }

   if (memcmp(&etat1.glTextureCoordArrayStride_, &etat2.glTextureCoordArrayStride_, sizeof(etat1.glTextureCoordArrayStride_))) {
      o << "GL_TEXTURE_COORD_ARRAY_STRIDE :"
        << "\nAvant : " << etat1.obtenirChaineGlTextureCoordArrayStride()
        << "\nAprès : " << etat2.obtenirChaineGlTextureCoordArrayStride()
        << std::endl;
   }

   if (memcmp(&etat1.glTextureCoordArrayType_, &etat2.glTextureCoordArrayType_, sizeof(etat1.glTextureCoordArrayType_))) {
      o << "GL_TEXTURE_COORD_ARRAY_TYPE :"
        << "\nAvant : " << etat1.obtenirChaineGlTextureCoordArrayType()
        << "\nAprès : " << etat2.obtenirChaineGlTextureCoordArrayType()
        << std::endl;
   }

   if (memcmp(&etat1.glTextureGenQ_, &etat2.glTextureGenQ_, sizeof(etat1.glTextureGenQ_))) {
      o << "GL_TEXTURE_GEN_Q :"
        << "\nAvant : " << etat1.obtenirChaineGlTextureGenQ()
        << "\nAprès : " << etat2.obtenirChaineGlTextureGenQ()
        << std::endl;
   }

   if (memcmp(&etat1.glTextureGenR_, &etat2.glTextureGenR_, sizeof(etat1.glTextureGenR_))) {
      o << "GL_TEXTURE_GEN_R :"
        << "\nAvant : " << etat1.obtenirChaineGlTextureGenR()
        << "\nAprès : " << etat2.obtenirChaineGlTextureGenR()
        << std::endl;
   }

   if (memcmp(&etat1.glTextureGenS_, &etat2.glTextureGenS_, sizeof(etat1.glTextureGenS_))) {
      o << "GL_TEXTURE_GEN_S :"
        << "\nAvant : " << etat1.obtenirChaineGlTextureGenS()
        << "\nAprès : " << etat2.obtenirChaineGlTextureGenS()
        << std::endl;
   }

   if (memcmp(&etat1.glTextureGenT_, &etat2.glTextureGenT_, sizeof(etat1.glTextureGenT_))) {
      o << "GL_TEXTURE_GEN_T :"
        << "\nAvant : " << etat1.obtenirChaineGlTextureGenT()
        << "\nAprès : " << etat2.obtenirChaineGlTextureGenT()
        << std::endl;
   }

   if (memcmp(etat1.glTextureMatrix_, etat2.glTextureMatrix_, sizeof(etat1.glTextureMatrix_))) {
      o << "GL_TEXTURE_MATRIX :"
        << "\nAvant : " << etat1.obtenirChaineGlTextureMatrix()
        << "\nAprès : " << etat2.obtenirChaineGlTextureMatrix()
        << std::endl;
   }

   if (memcmp(&etat1.glTextureStackDepth_, &etat2.glTextureStackDepth_, sizeof(etat1.glTextureStackDepth_))) {
      o << "GL_TEXTURE_STACK_DEPTH :"
        << "\nAvant : " << etat1.obtenirChaineGlTextureStackDepth()
        << "\nAprès : " << etat2.obtenirChaineGlTextureStackDepth()
        << std::endl;
   }

   if (memcmp(&etat1.glUnpackAlignment_, &etat2.glUnpackAlignment_, sizeof(etat1.glUnpackAlignment_))) {
      o << "GL_UNPACK_ALIGNMENT :"
        << "\nAvant : " << etat1.obtenirChaineGlUnpackAlignment()
        << "\nAprès : " << etat2.obtenirChaineGlUnpackAlignment()
        << std::endl;
   }

   if (memcmp(&etat1.glUnpackLsbFirst_, &etat2.glUnpackLsbFirst_, sizeof(etat1.glUnpackLsbFirst_))) {
      o << "GL_UNPACK_LSB_FIRST :"
        << "\nAvant : " << etat1.obtenirChaineGlUnpackLsbFirst()
        << "\nAprès : " << etat2.obtenirChaineGlUnpackLsbFirst()
        << std::endl;
   }

   if (memcmp(&etat1.glUnpackRowLength_, &etat2.glUnpackRowLength_, sizeof(etat1.glUnpackRowLength_))) {
      o << "GL_UNPACK_ROW_LENGTH :"
        << "\nAvant : " << etat1.obtenirChaineGlUnpackRowLength()
        << "\nAprès : " << etat2.obtenirChaineGlUnpackRowLength()
        << std::endl;
   }

   if (memcmp(&etat1.glUnpackSkipPixels_, &etat2.glUnpackSkipPixels_, sizeof(etat1.glUnpackSkipPixels_))) {
      o << "GL_UNPACK_SKIP_PIXELS :"
        << "\nAvant : " << etat1.obtenirChaineGlUnpackSkipPixels()
        << "\nAprès : " << etat2.obtenirChaineGlUnpackSkipPixels()
        << std::endl;
   }

   if (memcmp(&etat1.glUnpackSkipRows_, &etat2.glUnpackSkipRows_, sizeof(etat1.glUnpackSkipRows_))) {
      o << "GL_UNPACK_SKIP_ROWS :"
        << "\nAvant : " << etat1.obtenirChaineGlUnpackSkipRows()
        << "\nAprès : " << etat2.obtenirChaineGlUnpackSkipRows()
        << std::endl;
   }

   if (memcmp(&etat1.glUnpackSwapBytes_, &etat2.glUnpackSwapBytes_, sizeof(etat1.glUnpackSwapBytes_))) {
      o << "GL_UNPACK_SWAP_BYTES :"
        << "\nAvant : " << etat1.obtenirChaineGlUnpackSwapBytes()
        << "\nAprès : " << etat2.obtenirChaineGlUnpackSwapBytes()
        << std::endl;
   }

   if (memcmp(&etat1.glVertexArray_, &etat2.glVertexArray_, sizeof(etat1.glVertexArray_))) {
      o << "GL_VERTEX_ARRAY :"
        << "\nAvant : " << etat1.obtenirChaineGlVertexArray()
        << "\nAprès : " << etat2.obtenirChaineGlVertexArray()
        << std::endl;
   }

   if (memcmp(&etat1.glVertexArraySize_, &etat2.glVertexArraySize_, sizeof(etat1.glVertexArraySize_))) {
      o << "GL_VERTEX_ARRAY_SIZE :"
        << "\nAvant : " << etat1.obtenirChaineGlVertexArraySize()
        << "\nAprès : " << etat2.obtenirChaineGlVertexArraySize()
        << std::endl;
   }

   if (memcmp(&etat1.glVertexArrayStride_, &etat2.glVertexArrayStride_, sizeof(etat1.glVertexArrayStride_))) {
      o << "GL_VERTEX_ARRAY_STRIDE :"
        << "\nAvant : " << etat1.obtenirChaineGlVertexArrayStride()
        << "\nAprès : " << etat2.obtenirChaineGlVertexArrayStride()
        << std::endl;
   }

   if (memcmp(&etat1.glVertexArrayType_, &etat2.glVertexArrayType_, sizeof(etat1.glVertexArrayType_))) {
      o << "GL_VERTEX_ARRAY_TYPE :"
        << "\nAvant : " << etat1.obtenirChaineGlVertexArrayType()
        << "\nAprès : " << etat2.obtenirChaineGlVertexArrayType()
        << std::endl;
   }

   if (memcmp(etat1.glViewport_, etat2.glViewport_, sizeof(etat1.glViewport_))) {
      o << "GL_VIEWPORT :"
        << "\nAvant : " << etat1.obtenirChaineGlViewport()
        << "\nAprès : " << etat2.obtenirChaineGlViewport()
        << std::endl;
   }

   if (memcmp(&etat1.glZoomX_, &etat2.glZoomX_, sizeof(etat1.glZoomX_))) {
      o << "GL_ZOOM_X :"
        << "\nAvant : " << etat1.obtenirChaineGlZoomX()
        << "\nAprès : " << etat2.obtenirChaineGlZoomX()
        << std::endl;
   }

   if (memcmp(&etat1.glZoomY_, &etat2.glZoomY_, sizeof(etat1.glZoomY_))) {
      o << "GL_ZOOM_Y :"
        << "\nAvant : " << etat1.obtenirChaineGlZoomY()
        << "\nAprès : " << etat2.obtenirChaineGlZoomY()
        << std::endl;
   }


   // On regarde pour les vecteurs.
   bool different;

   different = (etat1.glClipPlanei_.size() != etat2.glClipPlanei_.size());
   if (!different) {
      for (unsigned int i = 0; i < etat1.glClipPlanei_.size(); ++i) {
         if ( (etat1.glClipPlanei_[i] == GL_FALSE) ^
              (etat2.glClipPlanei_[i] == GL_FALSE) ) {
            different = true;
            break;
         }
      }
   }
   if (different) {
      o << "GL_CLIP_PLANEi :"
        << "\nAvant : " << etat1.obtenirChaineGlClipPlanei()
        << "\nAprès : " << etat2.obtenirChaineGlClipPlanei()
        << std::endl;
   }

   different = (etat1.glLighti_.size() != etat2.glLighti_.size());
   if (!different) {
      for (unsigned int i = 0; i < etat1.glLighti_.size(); ++i) {
         if ( (etat1.glLighti_[i] == GL_FALSE) ^
              (etat2.glLighti_[i] == GL_FALSE) ) {
            different = true;
            break;
         }
      }
   }
   if (different) {
      o << "GL_LIGHTi :"
        << "\nAvant : " << etat1.obtenirChaineGlLighti()
        << "\nAprès : " << etat2.obtenirChaineGlLighti()
        << std::endl;
   }
}


///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlAccumAlphaBits() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_ACCUM_ALPHA_BITS.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the number of alpha bitplanes in the accumulation
/// buffer.
///
/// @return Une chaîne représentant l'attribut GL_ACCUM_ALPHA_BITS.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlAccumAlphaBits() const
{
   return utilitaire::convertirEnChaine(glAccumAlphaBits_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlAccumBlueBits() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_ACCUM_BLUE_BITS.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the number of blue bitplanes in the accumulation
/// buffer.
///
/// @return Une chaîne représentant l'attribut GL_ACCUM_BLUE_BITS.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlAccumBlueBits() const
{
   return utilitaire::convertirEnChaine(glAccumBlueBits_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlAccumClearValue() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_ACCUM_CLEAR_VALUE.
///
/// Selon le man page, cet attribut:
///
/// params returns four values: the red, green, blue, and alpha values used to
/// clear the accumulation buffer. Integer values, if requested, are linearly
/// mapped from the internal floating-point representation such that 1.0
/// returns the most positive representable integer value, and -1.0 returns the
/// most negative representable integer value. The initial value is
/// (0, 0, 0, 0). See glClearAccum.
///
/// @return Une chaîne représentant l'attribut GL_ACCUM_CLEAR_VALUE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlAccumClearValue() const
{
   return obtenirChaineVecteur(glAccumClearValue_, 4);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlAccumGreenBits() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_ACCUM_GREEN_BITS.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the number of green bitplanes in the accumulation
/// buffer.
///
/// @return Une chaîne représentant l'attribut GL_ACCUM_GREEN_BITS.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlAccumGreenBits() const
{
   return utilitaire::convertirEnChaine(glAccumGreenBits_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlAccumRedBits() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_ACCUM_RED_BITS.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the number of red bitplanes in the accumulation
/// buffer.
///
/// @return Une chaîne représentant l'attribut GL_ACCUM_RED_BITS.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlAccumRedBits() const
{
   return utilitaire::convertirEnChaine(glAccumRedBits_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlAlphaBias() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_ALPHA_BIAS.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the alpha bias factor used during pixel
/// transfers. The initial value is 0. See glPixelTransfer.
///
/// @return Une chaîne représentant l'attribut GL_ALPHA_BIAS.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlAlphaBias() const
{
   return utilitaire::convertirEnChaine(glAlphaBias_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlAlphaBits() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_ALPHA_BITS.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the number of alpha bitplanes in each color
/// buffer.
///
/// @return Une chaîne représentant l'attribut GL_ALPHA_BITS.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlAlphaBits() const
{
   return utilitaire::convertirEnChaine(glAlphaBits_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlAlphaScale() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_ALPHA_SCALE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the alpha scale factor used during pixel
/// transfers. The initial value is 1. See glPixelTransfer.
///
/// @return Une chaîne représentant l'attribut GL_ALPHA_SCALE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlAlphaScale() const
{
   return utilitaire::convertirEnChaine(glAlphaScale_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlAlphaTest() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_ALPHA_TEST.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether alpha testing of
/// fragments is enabled. The initial value is GL_FALSE. See glAlphaFunc.
///
/// @return Une chaîne représentant l'attribut GL_ALPHA_TEST.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlAlphaTest() const
{
   return obtenirChaineBooleen(glAlphaTest_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlAlphaTestFunc() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_ALPHA_TEST_FUNC.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the symbolic name of the alpha test function. The
/// initial value is GL_ALWAYS. See glAlphaFunc.
///
/// @return Une chaîne représentant l'attribut GL_ALPHA_TEST_FUNC.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlAlphaTestFunc() const
{
   COMPARER_VALEUR(glAlphaTestFunc_, GL_NEVER);
   COMPARER_VALEUR(glAlphaTestFunc_, GL_LESS);
   COMPARER_VALEUR(glAlphaTestFunc_, GL_EQUAL);
   COMPARER_VALEUR(glAlphaTestFunc_, GL_LEQUAL);
   COMPARER_VALEUR(glAlphaTestFunc_, GL_GREATER);
   COMPARER_VALEUR(glAlphaTestFunc_, GL_NOTEQUAL);
   COMPARER_VALEUR(glAlphaTestFunc_, GL_GEQUAL);
   COMPARER_VALEUR(glAlphaTestFunc_, GL_ALWAYS);
   COMPARER_DEFAUT(glAlphaTestFunc_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlAlphaTestRef() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_ALPHA_TEST_REF.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the reference value for the alpha test. The
/// initial value is 0. See glAlphaFunc. An integer value, if requested, is
/// linearly mapped from the internal floating-point representation such that
/// 1.0 returns the most positive representable integer value, and -1.0 returns
/// the most negative representable integer value.
///
/// @return Une chaîne représentant l'attribut GL_ALPHA_TEST_REF.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlAlphaTestRef() const
{
   return utilitaire::convertirEnChaine(glAlphaTestRef_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlAttribStackDepth() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_ATTRIB_STACK_DEPTH.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the depth of the attribute stack. If the stack is
/// empty, 0 is returned. The initial value is 0. See glPushAttrib.
///
/// @return Une chaîne représentant l'attribut GL_ATTRIB_STACK_DEPTH.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlAttribStackDepth() const
{
   return utilitaire::convertirEnChaine(glAttribStackDepth_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlAutoNormal() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_AUTO_NORMAL.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether 2D map evaluation
/// automatically generates surface normals. The initial value is GL_FALSE.
/// See glMap2.
///
/// @return Une chaîne représentant l'attribut GL_AUTO_NORMAL.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlAutoNormal() const
{
   return obtenirChaineBooleen(glAutoNormal_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlAuxBuffers() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_AUX_BUFFERS.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the number of auxiliary color buffers. The
/// initial value is 0.
///
/// @return Une chaîne représentant l'attribut GL_AUX_BUFFERS.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlAuxBuffers() const
{
   return utilitaire::convertirEnChaine(glAuxBuffers_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlBlend() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_BLEND.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether blending is
/// enabled. The initial value is GL_FALSE. See glBlendFunc.
///
/// @return Une chaîne représentant l'attribut GL_BLEND.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlBlend() const
{
   return obtenirChaineBooleen(glBlend_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlBlendDst() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_BLEND_DST.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the symbolic constant identifying the destination
/// blend function. The initial value is GL_ZERO. See glBlendFunc.
///
/// @return Une chaîne représentant l'attribut GL_BLEND_DST.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlBlendDst() const
{
   COMPARER_VALEUR(glBlendDst_, GL_ZERO);
   COMPARER_VALEUR(glBlendDst_, GL_ONE);
   COMPARER_VALEUR(glBlendDst_, GL_SRC_COLOR);
   COMPARER_VALEUR(glBlendDst_, GL_ONE_MINUS_SRC_COLOR);
   COMPARER_VALEUR(glBlendDst_, GL_SRC_ALPHA);
   COMPARER_VALEUR(glBlendDst_, GL_ONE_MINUS_SRC_ALPHA);
   COMPARER_VALEUR(glBlendDst_, GL_DST_ALPHA);
   COMPARER_VALEUR(glBlendDst_, GL_ONE_MINUS_DST_ALPHA);
   COMPARER_DEFAUT(glBlendDst_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlBlendSrc() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_BLEND_SRC.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the symbolic constant identifying the source
/// blend function. The initial value is GL_ONE. See glBlendFunc.
///
/// @return Une chaîne représentant l'attribut GL_BLEND_SRC.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlBlendSrc() const
{
   COMPARER_VALEUR(glBlendSrc_, GL_ZERO);
   COMPARER_VALEUR(glBlendSrc_, GL_ONE);
   COMPARER_VALEUR(glBlendSrc_, GL_DST_COLOR);
   COMPARER_VALEUR(glBlendSrc_, GL_ONE_MINUS_DST_COLOR);
   COMPARER_VALEUR(glBlendSrc_, GL_SRC_ALPHA);
   COMPARER_VALEUR(glBlendSrc_, GL_ONE_MINUS_SRC_ALPHA);
   COMPARER_VALEUR(glBlendSrc_, GL_DST_ALPHA);
   COMPARER_VALEUR(glBlendSrc_, GL_ONE_MINUS_DST_ALPHA);
   COMPARER_VALEUR(glBlendSrc_, GL_SRC_ALPHA_SATURATE);
   COMPARER_DEFAUT(glBlendSrc_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlBlueBias() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_BLUE_BIAS.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the blue bias factor used during pixel transfers.
/// The initial value is 0. See glPixelTransfer.
///
/// @return Une chaîne représentant l'attribut GL_BLUE_BIAS.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlBlueBias() const
{
   return utilitaire::convertirEnChaine(glBlueBias_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlBlueBits() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_BLUE_BITS.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the number of blue bitplanes in each color
/// buffer.
///
/// @return Une chaîne représentant l'attribut GL_BLUE_BITS.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlBlueBits() const
{
   return utilitaire::convertirEnChaine(glBlueBits_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlBlueScale() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_BLUE_SCALE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the blue scale factor used during pixel
/// transfers. The initial value is 1. See glPixelTransfer.
///
/// @return Une chaîne représentant l'attribut GL_BLUE_SCALE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlBlueScale() const
{
   return utilitaire::convertirEnChaine(glBlueScale_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlClientAttribStackDepth() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_CLIENT_ATTRIB_STACK_DEPTH.
///
/// Selon le man page, cet attribut:
///
/// params returns one value indicating the depth of the attribute stack. The
/// initial value is 0. See glPushClientAttrib.
///
/// @return Une chaîne représentant l'attribut GL_CLIENT_ATTRIB_STACK_DEPTH.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlClientAttribStackDepth() const
{
   return utilitaire::convertirEnChaine(glClientAttribStackDepth_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlClipPlanei() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_CLIP_PLANEi.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether the specified
/// clipping plane is enabled. The initial value is GL_FALSE. See glClipPlane.
///
/// @return Une chaîne représentant l'attribut GL_CLIP_PLANEi.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlClipPlanei() const
{
   std::ostringstream o;
   o << "(";
   for (unsigned int i = 0 ; i < glClipPlanei_.size(); ++i) {
      if (i != 0)
         o << " , ";
      o << obtenirChaineBooleen(glClipPlanei_[i]);
   }
   o << " )";
   return o.str();
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlColorArray() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_COLOR_ARRAY.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether the color array
/// is enabled. The initial value is GL_FALSE. See glColorPointer.
///
/// @return Une chaîne représentant l'attribut GL_COLOR_ARRAY.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlColorArray() const
{
   return obtenirChaineBooleen(glColorArray_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlColorArraySize() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_COLOR_ARRAY_SIZE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the number of components per color in the color
/// array. The initial value is 4. See glColorPointer.
///
/// @return Une chaîne représentant l'attribut GL_COLOR_ARRAY_SIZE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlColorArraySize() const
{
   return utilitaire::convertirEnChaine(glColorArraySize_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlColorArrayStride() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_COLOR_ARRAY_STRIDE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the byte offset between consecutive colors in
/// the color array. The initial value is 0. See glColorPointer.
///
/// @return Une chaîne représentant l'attribut GL_COLOR_ARRAY_STRIDE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlColorArrayStride() const
{
   return utilitaire::convertirEnChaine(glColorArrayStride_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlColorArrayType() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_COLOR_ARRAY_TYPE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the data type of each component in the color
/// array. The initial value is GL_FLOAT. See glColorPointer.
///
/// @return Une chaîne représentant l'attribut GL_COLOR_ARRAY_TYPE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlColorArrayType() const
{
   COMPARER_VALEUR(glColorArrayType_, GL_BYTE);
   COMPARER_VALEUR(glColorArrayType_, GL_UNSIGNED_BYTE);
   COMPARER_VALEUR(glColorArrayType_, GL_SHORT);
   COMPARER_VALEUR(glColorArrayType_, GL_UNSIGNED_SHORT);
   COMPARER_VALEUR(glColorArrayType_, GL_INT);
   COMPARER_VALEUR(glColorArrayType_, GL_UNSIGNED_INT);
   COMPARER_VALEUR(glColorArrayType_, GL_FLOAT);
   COMPARER_VALEUR(glColorArrayType_, GL_DOUBLE);
   COMPARER_DEFAUT(glColorArrayType_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlColorClearValue() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_COLOR_CLEAR_VALUE.
///
/// Selon le man page, cet attribut:
///
/// params returns four values: the red, green, blue, and alpha values used to
/// clear the color buffers. Integer values, if requested, are linearly mapped
/// from the internal floating-point representation such that 1.0 returns the
/// most positive representable integer value, and -1.0 returns the most
/// negative representable integer value. The initial value is (0, 0, 0, 0).
/// See glClearColor.
///
/// @return Une chaîne représentant l'attribut GL_COLOR_CLEAR_VALUE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlColorClearValue() const
{
   return obtenirChaineVecteur(glColorClearValue_, 4);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlColorLogicOp() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_COLOR_LOGIC_OP.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether a fragment's RGBA
/// color values are merged into the framebuffer using a logical operation.
/// The initial value is GL_FALSE. See glLogicOp.
///
/// @return Une chaîne représentant l'attribut GL_COLOR_LOGIC_OP.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlColorLogicOp() const
{
   return obtenirChaineBooleen(glColorLogicOp_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlColorMaterial() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_COLOR_MATERIAL.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether one or more
/// material parameters are tracking the current color. The initial value is
/// GL_FALSE. See glColorMaterial.
///
/// @return Une chaîne représentant l'attribut GL_COLOR_MATERIAL.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlColorMaterial() const
{
   return obtenirChaineBooleen(glColorMaterial_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlColorMaterialFace() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_COLOR_MATERIAL_FACE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, a symbolic constant indicating which materials
/// have a parameter that is tracking the current color. The initial value is
/// GL_FRONT_AND_BACK. See glColorMaterial.
///
/// @return Une chaîne représentant l'attribut GL_COLOR_MATERIAL_FACE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlColorMaterialFace() const
{
   COMPARER_VALEUR(glColorMaterialFace_, GL_FRONT);
   COMPARER_VALEUR(glColorMaterialFace_, GL_BACK);
   COMPARER_VALEUR(glColorMaterialFace_, GL_FRONT_AND_BACK);
   COMPARER_DEFAUT(glColorMaterialFace_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlColorMaterialParameter() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_COLOR_MATERIAL_PARAMETER.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, a symbolic constant indicating which material
/// parameters are tracking the current color. The initial value is
/// GL_AMBIENT_AND_DIFFUSE. See glColorMaterial.
///
/// @return Une chaîne représentant l'attribut GL_COLOR_MATERIAL_PARAMETER.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlColorMaterialParameter() const
{
   COMPARER_VALEUR(glColorMaterialParameter_, GL_EMISSION);
   COMPARER_VALEUR(glColorMaterialParameter_, GL_AMBIENT);
   COMPARER_VALEUR(glColorMaterialParameter_, GL_DIFFUSE);
   COMPARER_VALEUR(glColorMaterialParameter_, GL_SPECULAR);
   COMPARER_VALEUR(glColorMaterialParameter_, GL_AMBIENT_AND_DIFFUSE);
   COMPARER_DEFAUT(glColorMaterialParameter_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlColorWriteMask() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_COLOR_WRITEMASK.
///
/// Selon le man page, cet attribut:
///
/// params returns four boolean values: the red, green, blue, and alpha write
/// enables for the color buffers. The initial value is
/// (GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE). See glColorMask.
///
/// @return Une chaîne représentant l'attribut GL_COLOR_WRITEMASK.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlColorWritemask() const
{
   std::ostringstream o;
   o << "(";
   for (unsigned int i = 0 ; i < 4; ++i) {
      if (i != 0)
         o << " , ";
      o << obtenirChaineBooleen(glColorWritemask_[i]);
   }
   o << " )";
   return o.str();
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlCullFace() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_CULL_FACE.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether polygon culling is
/// enabled. The initial value is GL_FALSE. See glCullFace.
///
/// @return Une chaîne représentant l'attribut GL_CULL_FACE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlCullFace() const
{
   return obtenirChaineBooleen(glCullFace_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlCullFaceMode() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_CULL_FACE_MODE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, a symbolic constant indicating which polygon
/// faces are to be culled. The initial value is GL_BACK. See glCullFace.
///
/// @return Une chaîne représentant l'attribut GL_CULL_FACE_MODE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlCullFaceMode() const
{
   COMPARER_VALEUR(glCullFaceMode_, GL_FRONT);
   COMPARER_VALEUR(glCullFaceMode_, GL_BACK);
   COMPARER_VALEUR(glCullFaceMode_, GL_FRONT_AND_BACK);
   COMPARER_DEFAUT(glCullFaceMode_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlCurrentColor() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_CURRENT_COLOR.
///
/// Selon le man page, cet attribut:
///
/// params returns four values: the red, green, blue, and alpha values of the
/// current color. Integer values, if requested, are linearly mapped from the
/// internal floating-point representation such that 1.0 returns the most
/// positive representable integer value, and -1.0 returns the most negative
/// representable integer value. See glColor. The initial value is
/// (1, 1, 1, 1).
///
/// @return Une chaîne représentant l'attribut GL_CURRENT_COLOR.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlCurrentColor() const
{
   return obtenirChaineVecteur(glCurrentColor_, 4);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlCurrentIndex() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_CURRENT_INDEX.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the current color index. The initial value is 1.
/// See glIndex.
///
/// @return Une chaîne représentant l'attribut GL_CURRENT_INDEX.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlCurrentIndex() const
{
   return utilitaire::convertirEnChaine(glCurrentIndex_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlCurrentNormal() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_CURRENT_NORMAL.
///
/// Selon le man page, cet attribut:
///
/// params returns three values: the x, y, and z values of the current normal.
/// Integer values, if requested, are linearly mapped from the internal
/// floating-point representation such that 1.0 returns the most positive
/// representable integer value, and -1.0 returns the most negative
/// representable integer value. The initial value is (0, 0, 1). See glNormal.
///
/// @return Une chaîne représentant l'attribut GL_CURRENT_NORMAL.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlCurrentNormal() const
{
   return obtenirChaineVecteur(glCurrentNormal_, 3);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlCurrentRasterColor() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_CURRENT_RASTER_COLOR.
///
/// Selon le man page, cet attribut:
///
/// params returns four values: the red, green, blue, and alpha values of the
/// current raster position. Integer values, if requested, are linearly mapped
/// from the internal floating-point representation such that 1.0 returns the
/// most positive representable integer value, and -1.0 returns the most
/// negative representable integer value. The initial value is (1, 1, 1, 1).
/// See glRasterPos.
///
/// @return Une chaîne représentant l'attribut GL_CURRENT_RASTER_COLOR.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlCurrentRasterColor() const
{
   return obtenirChaineVecteur(glCurrentRasterColor_, 4);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlCurrentRasterDistance() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_CURRENT_RASTER_DISTANCE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the distance from the eye to the current raster
/// position. The initial value is 0. See glRasterPos.
///
/// @return Une chaîne représentant l'attribut GL_CURRENT_RASTER_DISTANCE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlCurrentRasterDistance() const
{
   return utilitaire::convertirEnChaine(glCurrentRasterDistance_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlCurrentRasterIndex() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_CURRENT_RASTER_INDEX.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the color index of the current raster position.
/// The initial value is 1. See glRasterPos.
///
/// @return Une chaîne représentant l'attribut GL_CURRENT_RASTER_INDEX.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlCurrentRasterIndex() const
{
   return utilitaire::convertirEnChaine(glCurrentRasterIndex_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlCurrentRasterPosition() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_CURRENT_RASTER_POSITION.
///
/// Selon le man page, cet attribut:
///
/// params returns four values: the x, y, z, and w components of the current
/// raster position. x, y, and z are in window coordinates, and w is in clip
/// coordinates. The initial value is (0, 0, 0, 1). See glRasterPos.
///
/// @return Une chaîne représentant l'attribut GL_CURRENT_RASTER_POSITION.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlCurrentRasterPosition() const
{
   return obtenirChaineVecteur(glCurrentRasterPosition_, 4);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlCurrentRasterPositionValid() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_CURRENT_RASTER_POSITION_VALID.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether the current raster
/// position is valid. The initial value is GL_TRUE. See glRasterPos.
///
/// @return Une chaîne représentant l'attribut GL_CURRENT_RASTER_POSITION_VALID.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlCurrentRasterPositionValid() const
{
   return obtenirChaineBooleen(glCurrentRasterPositionValid_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlCurrentRasterTextureCoords() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_CURRENT_RASTER_TEXTURE_COORDS.
///
/// Selon le man page, cet attribut:
///
/// params returns four values: the s, t, r, and q current raster texture
/// coordinates. The initial value is (0, 0, 0, 1). See glRasterPos and
/// glTexCoord.
///
/// @return Une chaîne représentant l'attribut GL_CURRENT_RASTER_TEXTURE_COORDS.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlCurrentRasterTextureCoords() const
{
   return obtenirChaineVecteur(glCurrentRasterTextureCoords_, 4);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlCurrentTextureCoords() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_CURRENT_TEXTURE_COORDS.
///
/// Selon le man page, cet attribut:
///
/// params returns four values: the s, t, r, and q current texture
/// coordinates. The initial value is (0, 0, 0, 1). See glTexCoord.
///
/// @return Une chaîne représentant l'attribut GL_CURRENT_TEXTURE_COORDS.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlCurrentTextureCoords() const
{
   return obtenirChaineVecteur(glCurrentTextureCoords_, 4);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlDepthBias() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_DEPTH_BIAS.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the depth bias factor used during pixel
/// transfers. The initial value is 0. See glPixelTransfer.
///
/// @return Une chaîne représentant l'attribut GL_DEPTH_BIAS.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlDepthBias() const
{
   return utilitaire::convertirEnChaine(glDepthBias_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlDepthBits() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_DEPTH_BITS.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the number of bitplanes in the depth buffer.
///
/// @return Une chaîne représentant l'attribut GL_DEPTH_BITS.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlDepthBits() const
{
   return utilitaire::convertirEnChaine(glDepthBits_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlDepthClearValue() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_DEPTH_CLEAR_VALUE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the value that is used to clear the depth buffer.
/// Integer values, if requested, are linearly mapped from the internal
/// floating-point representation such that 1.0 returns the most positive
/// representable integer value, and -1.0 returns the most negative
/// representable integer value. The initial value is 1. See glClearDepth.
///
/// @return Une chaîne représentant l'attribut GL_DEPTH_CLEAR_VALUE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlDepthClearValue() const
{
   return utilitaire::convertirEnChaine(glDepthClearValue_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlDepthFunc() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_DEPTH_FUNC.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the symbolic constant that indicates the depth
/// comparison function. The initial value is GL_LESS. See glDepthFunc.
///
/// @return Une chaîne représentant l'attribut GL_DEPTH_FUNC.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlDepthFunc() const
{
   COMPARER_VALEUR(glDepthFunc_, GL_NEVER);
   COMPARER_VALEUR(glDepthFunc_, GL_LESS);
   COMPARER_VALEUR(glDepthFunc_, GL_EQUAL);
   COMPARER_VALEUR(glDepthFunc_, GL_LEQUAL);
   COMPARER_VALEUR(glDepthFunc_, GL_GREATER);
   COMPARER_VALEUR(glDepthFunc_, GL_NOTEQUAL);
   COMPARER_VALEUR(glDepthFunc_, GL_GEQUAL);
   COMPARER_VALEUR(glDepthFunc_, GL_ALWAYS);
   COMPARER_DEFAUT(glDepthFunc_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlDepthRange() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_DEPTH_RANGE.
///
/// Selon le man page, cet attribut:
///
/// params returns two values: the near and far mapping limits for the depth
/// buffer. Integer values, if requested, are linearly mapped from the internal
/// floating-point representation such that 1.0 returns the most positive
/// representable integer value, and -1.0 returns the most negative
/// representable integer value. The initial value is (0, 1). See glDepthRange.
///
/// @return Une chaîne représentant l'attribut GL_DEPTH_RANGE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlDepthRange() const
{
   return obtenirChaineVecteur(glDepthRange_, 2);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlDepthScale() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_DEPTH_SCALE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the depth scale factor used during pixel
/// transfers. The initial value is 1. See glPixelTransfer.
///
/// @return Une chaîne représentant l'attribut GL_DEPTH_SCALE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlDepthScale() const
{
   return utilitaire::convertirEnChaine(glDepthScale_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlDepthTest() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_DEPTH_TEST.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether depth testing of
/// fragments is enabled. The initial value is GL_FALSE. See glDepthFunc and
/// glDepthRange.
///
/// @return Une chaîne représentant l'attribut GL_DEPTH_TEST.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlDepthTest() const
{
   return obtenirChaineBooleen(glDepthTest_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlDepthWritemask() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_DEPTH_WRITEMASK.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating if the depth buffer is
/// enabled for writing. The initial value is GL_TRUE. See glDepthMask.
///
/// @return Une chaîne représentant l'attribut GL_DEPTH_WRITEMASK.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlDepthWritemask() const
{
   return obtenirChaineBooleen(glDepthWritemask_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlDither() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_DITHER.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether dithering of
/// fragment colors and indices is enabled. The initial value is GL_TRUE.
///
/// @return Une chaîne représentant l'attribut GL_DITHER.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlDither() const
{
   return obtenirChaineBooleen(glDither_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlDoublebuffer() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_DOUBLEBUFFER.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether double buffering
/// is supported.
///
/// @return Une chaîne représentant l'attribut GL_DOUBLEBUFFER.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlDoublebuffer() const
{
   return obtenirChaineBooleen(glDoublebuffer_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlDrawBuffer() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_DRAW_BUFFER.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, a symbolic constant indicating which buffers are
/// being drawn to. See glDrawBuffer. The initial value is GL_BACK if there
/// are back buffers, otherwise it is GL_FRONT.
///
/// @return Une chaîne représentant l'attribut GL_DRAW_BUFFER.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlDrawBuffer() const
{
   COMPARER_VALEUR(glDrawBuffer_, GL_NONE);
   COMPARER_VALEUR(glDrawBuffer_, GL_FRONT_LEFT);
   COMPARER_VALEUR(glDrawBuffer_, GL_FRONT_RIGHT);
   COMPARER_VALEUR(glDrawBuffer_, GL_BACK_LEFT);
   COMPARER_VALEUR(glDrawBuffer_, GL_BACK_RIGHT);
   COMPARER_VALEUR(glDrawBuffer_, GL_FRONT);
   COMPARER_VALEUR(glDrawBuffer_, GL_BACK);
   COMPARER_VALEUR(glDrawBuffer_, GL_LEFT);
   COMPARER_VALEUR(glDrawBuffer_, GL_RIGHT);
   COMPARER_VALEUR(glDrawBuffer_, GL_FRONT_AND_BACK);
   COMPARER_VALEUR(glDrawBuffer_, GL_AUX0);
   COMPARER_VALEUR(glDrawBuffer_, GL_AUX1);
   COMPARER_VALEUR(glDrawBuffer_, GL_AUX2);
   COMPARER_VALEUR(glDrawBuffer_, GL_AUX3);
   COMPARER_DEFAUT(glDrawBuffer_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlEdgeFlag() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_EDGE_FLAG.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether the current edge
/// flag is GL_TRUE or GL_FALSE. The initial value is GL_TRUE. See glEdgeFlag.
///
/// @return Une chaîne représentant l'attribut GL_EDGE_FLAG.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlEdgeFlag() const
{
   return obtenirChaineBooleen(glEdgeFlag_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlEdgeFlagArray() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_EDGE_FLAG_ARRAY.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether the edge flag
/// array is enabled. The initial value is GL_FALSE. See glEdgeFlagPointer.
///
/// @return Une chaîne représentant l'attribut GL_EDGE_FLAG_ARRAY.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlEdgeFlagArray() const
{
   return obtenirChaineBooleen(glEdgeFlagArray_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlEdgeFlagArrayStride() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_EDGE_FLAG_ARRAY_STRIDE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the byte offset between consecutive edge flags
/// in the edge flag array. The initial value is 0. See glEdgeFlagPointer.
///
/// @return Une chaîne représentant l'attribut GL_EDGE_FLAG_ARRAY_STRIDE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlEdgeFlagArrayStride() const
{
   return utilitaire::convertirEnChaine(glEdgeFlagArrayStride_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlFeedbackBufferSize() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_FEEDBACK_BUFFER_SIZE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the size of the feedback buffer. See
/// glFeedbackBuffer.
///
/// @return Une chaîne représentant l'attribut GL_FEEDBACK_BUFFER_SIZE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlFeedbackBufferSize() const
{
   return utilitaire::convertirEnChaine(glFeedbackBufferSize_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlFeedbackBufferType() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_FEEDBACK_BUFFER_TYPE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the type of the feedback buffer. See
/// glFeedbackBuffer.
///
/// @return Une chaîne représentant l'attribut GL_FEEDBACK_BUFFER_TYPE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlFeedbackBufferType() const
{
   COMPARER_VALEUR(glFeedbackBufferType_, GL_2D);
   COMPARER_VALEUR(glFeedbackBufferType_, GL_3D);
   COMPARER_VALEUR(glFeedbackBufferType_, GL_3D_COLOR);
   COMPARER_VALEUR(glFeedbackBufferType_, GL_3D_COLOR_TEXTURE);
   COMPARER_VALEUR(glFeedbackBufferType_, GL_4D_COLOR_TEXTURE);
   COMPARER_DEFAUT(glFeedbackBufferType_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlFog() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_FOG.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether fogging is
/// enabled. The initial value is GL_FALSE. See glFog.
///
/// @return Une chaîne représentant l'attribut GL_FOG.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlFog() const
{
   return obtenirChaineBooleen(glFog_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlFogColor() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_FOG_COLOR.
///
/// Selon le man page, cet attribut:
///
/// params returns four values: the red, green, blue, and alpha components of
/// the fog color. Integer values, if requested, are linearly mapped from the
/// internal floating-point representation such that 1.0 returns the most
/// positive representable integer value, and -1.0 returns the most negative
/// representable integer value. The initial value is (0, 0, 0, 0). See glFog.
///
/// @return Une chaîne représentant l'attribut GL_FOG_COLOR.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlFogColor() const
{
   return obtenirChaineVecteur(glFogColor_, 4);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlFogDensity() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_FOG_DENSITY.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the fog density parameter. The initial value is
/// 1. See glFog.
///
/// @return Une chaîne représentant l'attribut GL_FOG_DENSITY.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlFogDensity() const
{
   return utilitaire::convertirEnChaine(glFogDensity_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlFogEnd() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_FOG_END.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the end factor for the linear fog equation. The
/// initial value is 1. See glFog.
///
/// @return Une chaîne représentant l'attribut GL_FOG_END.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlFogEnd() const
{
   return utilitaire::convertirEnChaine(glFogEnd_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlFogHint() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_FOG_HINT.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, a symbolic constant indicating the mode of the
/// fog hint. The initial value is GL_DONT_CARE. See glHint.
///
/// @return Une chaîne représentant l'attribut GL_FOG_HINT.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlFogHint() const
{
   COMPARER_VALEUR(glFogHint_, GL_FASTEST);
   COMPARER_VALEUR(glFogHint_, GL_NICEST);
   COMPARER_VALEUR(glFogHint_, GL_DONT_CARE);
   COMPARER_DEFAUT(glFogHint_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlFogIndex() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_FOG_INDEX.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the fog color index. The initial value is 0.
/// See glFog.
///
/// @return Une chaîne représentant l'attribut GL_FOG_INDEX.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlFogIndex() const
{
   return utilitaire::convertirEnChaine(glFogIndex_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlFogMode() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_FOG_MODE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, a symbolic constant indicating which fog
/// equation is selected. The initial value is GL_EXP. See glFog.
///
/// @return Une chaîne représentant l'attribut GL_FOG_MODE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlFogMode() const
{
   COMPARER_VALEUR(glFogMode_, GL_LINEAR);
   COMPARER_VALEUR(glFogMode_, GL_EXP);
   COMPARER_VALEUR(glFogMode_, GL_EXP2);
   COMPARER_DEFAUT(glFogMode_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlFogStart() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_FOG_START.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the start factor for the linear fog equation.
/// The initial value is 0. See glFog.
///
/// @return Une chaîne représentant l'attribut GL_FOG_START.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlFogStart() const
{
   return utilitaire::convertirEnChaine(glFogStart_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlFrontFace() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_FRONT_FACE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, a symbolic constant indicating whether clockwise
/// or counterclockwise polygon winding is treated as front-facing. The
/// initial value is GL_CCW. See glFrontFace.
///
/// @return Une chaîne représentant l'attribut GL_FRONT_FACE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlFrontFace() const
{
   COMPARER_VALEUR(glFrontFace_, GL_CW);
   COMPARER_VALEUR(glFrontFace_, GL_CCW);
   COMPARER_DEFAUT(glFrontFace_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlGreenBias() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_GREEN_BIAS.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the green bias factor used during pixel
/// transfers. The initial value is 0.
///
/// @return Une chaîne représentant l'attribut GL_GREEN_BIAS.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlGreenBias() const
{
   return utilitaire::convertirEnChaine(glGreenBias_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlGreenBits() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_GREEN_BITS.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the number of green bitplanes in each color
/// buffer.
///
/// @return Une chaîne représentant l'attribut GL_GREEN_BITS.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlGreenBits() const
{
   return utilitaire::convertirEnChaine(glGreenBits_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlGreenScale() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_GREEN_SCALE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the green scale factor used during pixel
/// transfers. The initial value is 1. See glPixelTransfer.
///
/// @return Une chaîne représentant l'attribut GL_GREEN_SCALE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlGreenScale() const
{
   return utilitaire::convertirEnChaine(glGreenScale_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlIndexArray() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_INDEX_ARRAY.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether the color index
/// array is enabled. The initial value is GL_FALSE. See glIndexPointer.
///
/// @return Une chaîne représentant l'attribut GL_INDEX_ARRAY.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlIndexArray() const
{
   return obtenirChaineBooleen(glIndexArray_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlIndexArrayStride() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_INDEX_ARRAY_STRIDE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the byte offset between consecutive color
/// indexes in the color index array. The initial value is 0. See
/// glIndexPointer.
///
/// @return Une chaîne représentant l'attribut GL_INDEX_ARRAY_STRIDE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlIndexArrayStride() const
{
   return utilitaire::convertirEnChaine(glIndexArrayStride_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlIndexArrayType() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_INDEX_ARRAY_TYPE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the data type of indexes in the color index
/// array. The initial value is GL_FLOAT. See glIndexPointer.
///
/// @return Une chaîne représentant l'attribut GL_INDEX_ARRAY_TYPE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlIndexArrayType() const
{
   COMPARER_VALEUR(glIndexArrayType_, GL_UNSIGNED_BYTE);
   COMPARER_VALEUR(glIndexArrayType_, GL_SHORT);
   COMPARER_VALEUR(glIndexArrayType_, GL_INT);
   COMPARER_VALEUR(glIndexArrayType_, GL_FLOAT);
   COMPARER_VALEUR(glIndexArrayType_, GL_DOUBLE);
   COMPARER_DEFAUT(glIndexArrayType_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlIndexBits() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_INDEX_BITS.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the number of bitplanes in each color index
/// buffer.
///
/// @return Une chaîne représentant l'attribut GL_INDEX_BITS.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlIndexBits() const
{
   return utilitaire::convertirEnChaine(glIndexBits_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlIndexClearValue() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_INDEX_CLEAR_VALUE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the color index used to clear the color index
/// buffers. The initial value is 0. See glClearIndex.
///
/// @return Une chaîne représentant l'attribut GL_INDEX_CLEAR_VALUE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlIndexClearValue() const
{
   return utilitaire::convertirEnChaine(glIndexClearValue_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlIndexLogicOp() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_INDEX_LOGIC_OP.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether a fragment's
/// index values are merged into the framebuffer using a logical operation.
/// The initial value is GL_FALSE. See glLogicOp.
///
/// @return Une chaîne représentant l'attribut GL_INDEX_LOGIC_OP.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlIndexLogicOp() const
{
   return obtenirChaineBooleen(glIndexLogicOp_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlIndexMode() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_INDEX_MODE.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether the GL is in
/// color index mode (GL_TRUE) or RGBA mode (GL_FALSE).
///
/// @return Une chaîne représentant l'attribut GL_INDEX_MODE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlIndexMode() const
{
   return obtenirChaineBooleen(glIndexMode_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlIndexOffset() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_INDEX_OFFSET.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the offset added to color and stencil indices
/// during pixel transfers. The initial value is 0. See glPixelTransfer.
///
/// @return Une chaîne représentant l'attribut GL_INDEX_OFFSET.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlIndexOffset() const
{
   return utilitaire::convertirEnChaine(glIndexOffset_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlIndexShift() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_INDEX_SHIFT.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the amount that color and stencil indices are
/// shifted during pixel transfers. The initial value is 0. See
/// glPixelTransfer.
///
/// @return Une chaîne représentant l'attribut GL_INDEX_SHIFT.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlIndexShift() const
{
   return utilitaire::convertirEnChaine(glIndexShift_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlIndexWritemask() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_INDEX_WRITEMASK.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, a mask indicating which bitplanes of each color
/// index buffer can be written. The initial value is all 1's. See glIndexMask.
///
/// @return Une chaîne représentant l'attribut GL_INDEX_WRITEMASK.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlIndexWritemask() const
{
   return utilitaire::convertirEnChaine(glIndexWritemask_, std::hex);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlLighti() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_LIGHTi.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether the specified
/// light is enabled. The initial value is GL_FALSE. See glLight and
/// glLightModel.
///
/// @return Une chaîne représentant l'attribut GL_LIGHTi.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlLighti() const
{
   std::ostringstream o;
   o << "(";
   for (unsigned int i = 0 ; i < glLighti_.size(); ++i) {
      if (i != 0)
         o << " , ";
      o << obtenirChaineBooleen(glLighti_[i]);
   }
   o << " )";
   return o.str();
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlLighting() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_LIGHTING.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether lighting is
/// enabled. The initial value is GL_FALSE. See glLightModel.
///
/// @return Une chaîne représentant l'attribut GL_LIGHTING.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlLighting() const
{
   return obtenirChaineBooleen(glLighting_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlLightModelAmbient() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_LIGHT_MODEL_AMBIENT.
///
/// Selon le man page, cet attribut:
///
/// params returns four values: the red, green, blue, and alpha components of
/// the ambient intensity of the entire scene. Integer values, if requested,
/// are linearly mapped from the internal floating-point representation such
/// that 1.0 returns the most positive representable integer value, and -1.0
/// returns the most negative representable integer value. The initial value
/// is (0.2, 0.2, 0.2, 1.0). See glLightModel.
///
/// @return Une chaîne représentant l'attribut GL_LIGHT_MODEL_AMBIENT.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlLightModelAmbient() const
{
   return obtenirChaineVecteur(glLightModelAmbient_, 4);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlLightModelLocalViewer() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_LIGHT_MODEL_LOCAL_VIEWER.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether specular
/// reflection calculations treat the viewer as being local to the scene. The
/// initial value is GL_FALSE. See glLightModel.
///
/// @return Une chaîne représentant l'attribut GL_LIGHT_MODEL_LOCAL_VIEWER.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlLightModelLocalViewer() const
{
   return obtenirChaineBooleen(glLightModelLocalViewer_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlLightModelTwoSide() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_LIGHT_MODEL_TWO_SIDE.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether separate
/// materials are used to compute lighting for front- and back-facing
/// polygons. The initial value is GL_FALSE. See glLightModel.
///
/// @return Une chaîne représentant l'attribut GL_LIGHT_MODEL_TWO_SIDE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlLightModelTwoSide() const
{
   return obtenirChaineBooleen(glLightModelTwoSide_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlLineSmooth() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_LINE_SMOOTH.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether antialiasing of
/// lines is enabled. The initial value is GL_FALSE. See glLineWidth.
///
/// @return Une chaîne représentant l'attribut GL_LINE_SMOOTH.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlLineSmooth() const
{
   return obtenirChaineBooleen(glLineSmooth_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlLineSmoothHint() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_LINE_SMOOTH_HINT.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, a symbolic constant indicating the mode of the
/// line antialiasing hint. The initial value is GL_DONT_CARE. See glHint.
///
/// @return Une chaîne représentant l'attribut GL_LINE_SMOOTH_HINT.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlLineSmoothHint() const
{
   COMPARER_VALEUR(glLineSmoothHint_, GL_FASTEST);
   COMPARER_VALEUR(glLineSmoothHint_, GL_NICEST);
   COMPARER_VALEUR(glLineSmoothHint_, GL_DONT_CARE);
   COMPARER_DEFAUT(glLineSmoothHint_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlLineStipple() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_LINE_STIPPLE.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether stippling of
/// lines is enabled. The initial value is GL_FALSE. See glLineStipple.
///
/// @return Une chaîne représentant l'attribut GL_LINE_STIPPLE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlLineStipple() const
{
   return obtenirChaineBooleen(glLineStipple_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlLineStipplePattern() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_LINE_STIPPLE_PATTERN.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the 16-bit line stipple pattern. The initial
/// value is all 1's. See glLineStipple.
///
/// @return Une chaîne représentant l'attribut GL_LINE_STIPPLE_PATTERN.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlLineStipplePattern() const
{
   return utilitaire::convertirEnChaine(glLineStipplePattern_, std::hex);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlLineStippleRepeat() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_LINE_STIPPLE_REPEAT.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the line stipple repeat factor. The initial value
/// is 1. See glLineStipple.
///
/// @return Une chaîne représentant l'attribut GL_LINE_STIPPLE_REPEAT.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlLineStippleRepeat() const
{
   return utilitaire::convertirEnChaine(glLineStippleRepeat_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlLineWidth() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_LINE_WIDTH.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the line width as specified with glLineWidth.
/// The initial value is 1.
///
/// @return Une chaîne représentant l'attribut GL_LINE_WIDTH.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlLineWidth() const
{
   return utilitaire::convertirEnChaine(glLineWidth_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlLineWidthGranularity() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_LINE_WIDTH_GRANULARITY.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the width difference between adjacent supported
/// widths for antialiased lines. See glLineWidth.
///
/// @return Une chaîne représentant l'attribut GL_LINE_WIDTH_GRANULARITY.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlLineWidthGranularity() const
{
   return utilitaire::convertirEnChaine(glLineWidthGranularity_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGllineWidthRange() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_LINE_WIDTH_RANGE.
///
/// Selon le man page, cet attribut:
///
/// params returns two values: the smallest and largest supported widths for
/// antialiased lines. See glLineWidth.
///
/// @return Une chaîne représentant l'attribut GL_LINE_WIDTH_RANGE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGllineWidthRange() const
{
   return utilitaire::convertirEnChaine(gllineWidthRange_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlListBase() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_LIST_BASE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the base offset added to all names in arrays
/// presented to glCallLists. The initial value is 0. See glListBase.
///
/// @return Une chaîne représentant l'attribut GL_LIST_BASE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlListBase() const
{
   return utilitaire::convertirEnChaine(glListBase_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlListIndex() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_LIST_INDEX.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the name of the display list currently under
/// construction. 0 is returned if no display list is currently under
/// construction. The initial value is 0. See glNewList.
///
/// @return Une chaîne représentant l'attribut GL_LIST_INDEX.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlListIndex() const
{
   return utilitaire::convertirEnChaine(glListIndex_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlListMode() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_LIST_MODE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, a symbolic constant indicating the construction
/// mode of the display list currently under construction. The initial value
/// is 0. See glNewList.
///
/// @return Une chaîne représentant l'attribut GL_LIST_MODE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlListMode() const
{
   COMPARER_VALEUR(glListMode_, GL_COMPILE);
   COMPARER_VALEUR(glListMode_, GL_COMPILE_AND_EXECUTE);
   COMPARER_DEFAUT(glListMode_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlLogicOpMode() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_LOGIC_OP_MODE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, a symbolic constant indicating the selected
/// logic operation mode. The initial value is GL_COPY. See glLogicOp.
///
/// @return Une chaîne représentant l'attribut GL_LOGIC_OP_MODE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlLogicOpMode() const
{
   COMPARER_VALEUR(glLogicOpMode_, GL_CLEAR);
   COMPARER_VALEUR(glLogicOpMode_, GL_SET);
   COMPARER_VALEUR(glLogicOpMode_, GL_COPY);
   COMPARER_VALEUR(glLogicOpMode_, GL_COPY_INVERTED);
   COMPARER_VALEUR(glLogicOpMode_, GL_NOOP);
   COMPARER_VALEUR(glLogicOpMode_, GL_INVERT);
   COMPARER_VALEUR(glLogicOpMode_, GL_AND);
   COMPARER_VALEUR(glLogicOpMode_, GL_NAND);
   COMPARER_VALEUR(glLogicOpMode_, GL_OR);
   COMPARER_VALEUR(glLogicOpMode_, GL_NOR);
   COMPARER_VALEUR(glLogicOpMode_, GL_XOR);
   COMPARER_VALEUR(glLogicOpMode_, GL_EQUIV);
   COMPARER_VALEUR(glLogicOpMode_, GL_AND_REVERSE);
   COMPARER_VALEUR(glLogicOpMode_, GL_AND_INVERTED);
   COMPARER_VALEUR(glLogicOpMode_, GL_OR_REVERSE);
   COMPARER_VALEUR(glLogicOpMode_, GL_OR_INVERTED);
   COMPARER_DEFAUT(glLogicOpMode_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlMap1Color4() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_MAP1_COLOR_4.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether 1D evaluation
/// generates colors. The initial value is GL_FALSE. See glMap1.
///
/// @return Une chaîne représentant l'attribut GL_MAP1_COLOR_4.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlMap1Color4() const
{
   return obtenirChaineBooleen(glMap1Color4_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlMap1GridDomain() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_MAP1_GRID_DOMAIN.
///
/// Selon le man page, cet attribut:
///
/// params returns two values: the endpoints of the 1D map's grid domain. The
/// initial value is (0, 1). See glMapGrid.
///
/// @return Une chaîne représentant l'attribut GL_MAP1_GRID_DOMAIN.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlMap1GridDomain() const
{
   return obtenirChaineVecteur(glMap1GridDomain_, 2);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlMap1GridSegments() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_MAP1_GRID_SEGMENTS.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the number of partitions in the 1D map's grid
/// domain. The initial value is 1. See glMapGrid.
///
/// @return Une chaîne représentant l'attribut GL_MAP1_GRID_SEGMENTS.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlMap1GridSegments() const
{
   return utilitaire::convertirEnChaine(glMap1GridSegments_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlMap1Index() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_MAP1_INDEX.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether 1D evaluation
/// generates color indices. The initial value is GL_FALSE. See glMap1.
///
/// @return Une chaîne représentant l'attribut GL_MAP1_INDEX.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlMap1Index() const
{
   return obtenirChaineBooleen(glMap1Index_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlMap1Normal() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_MAP1_NORMAL.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether 1D evaluation
/// generates normals. The initial value is GL_FALSE. See glMap1.
///
/// @return Une chaîne représentant l'attribut GL_MAP1_NORMAL.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlMap1Normal() const
{
   return obtenirChaineBooleen(glMap1Normal_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlMap1TextureCoord1() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_MAP1_TEXTURE_COORD_1.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether 1D evaluation
/// generates 1D texture coordinates. The initial value is GL_FALSE. See
/// glMap1.
///
/// @return Une chaîne représentant l'attribut GL_MAP1_TEXTURE_COORD_1.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlMap1TextureCoord1() const
{
   return obtenirChaineBooleen(glMap1TextureCoord1_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlMap1TextureCoord2() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_MAP1_TEXTURE_COORD_2.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether 1D evaluation
/// generates 2D texture coordinates. The initial value is GL_FALSE. See
/// glMap1.
///
/// @return Une chaîne représentant l'attribut GL_MAP1_TEXTURE_COORD_2.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlMap1TextureCoord2() const
{
   return obtenirChaineBooleen(glMap1TextureCoord2_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlMap1TextureCoord3() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_MAP1_TEXTURE_COORD_3.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether 1D evaluation
/// generates 3D texture coordinates. The initial value is GL_FALSE. See
/// glMap1.
///
/// @return Une chaîne représentant l'attribut GL_MAP1_TEXTURE_COORD_3.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlMap1TextureCoord3() const
{
   return obtenirChaineBooleen(glMap1TextureCoord3_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlMap1TextureCoord4() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_MAP1_TEXTURE_COORD_4.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether 1D evaluation
/// generates 4D texture coordinates. The initial value is GL_FALSE. See
/// glMap1.
///
/// @return Une chaîne représentant l'attribut GL_MAP1_TEXTURE_COORD_4.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlMap1TextureCoord4() const
{
   return obtenirChaineBooleen(glMap1TextureCoord4_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlMap1Vertex3() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_MAP1_VERTEX_3.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether 1D evaluation
/// generates 3D vertex coordinates. The initial value is GL_FALSE. See
/// glMap1.
///
/// @return Une chaîne représentant l'attribut GL_MAP1_VERTEX_3.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlMap1Vertex3() const
{
   return obtenirChaineBooleen(glMap1Vertex3_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlMap1Vertex4() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_MAP1_VERTEX_4.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether 1D evaluation
/// generates 4D vertex coordinates. The initial value is GL_FALSE. See
/// glMap1.
///
/// @return Une chaîne représentant l'attribut GL_MAP1_VERTEX_4.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlMap1Vertex4() const
{
   return obtenirChaineBooleen(glMap1Vertex4_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlMap2Color4() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_MAP2_COLOR_4.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether 2D evaluation
/// generates colors. The initial value is GL_FALSE. See glMap2.
///
/// @return Une chaîne représentant l'attribut GL_MAP2_COLOR_4.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlMap2Color4() const
{
   return obtenirChaineBooleen(glMap2Color4_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlMap2GridDomain() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_MAP2_GRID_DOMAIN.
///
/// Selon le man page, cet attribut:
///
/// params returns four values: the endpoints of the 2D map's i and j grid
/// domains. The initial value is (0,1; 0,1). See glMapGrid.
///
/// @return Une chaîne représentant l'attribut GL_MAP2_GRID_DOMAIN.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlMap2GridDomain() const
{
   return obtenirChaineVecteur(glMap2GridDomain_, 4);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlMap2GridSegments() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_MAP2_GRID_SEGMENTS.
///
/// Selon le man page, cet attribut:
///
/// params returns two values: the number of partitions in the 2D map's i and
/// j grid domains. The initial value is (1,1). See glMapGrid.
///
/// @return Une chaîne représentant l'attribut GL_MAP2_GRID_SEGMENTS.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlMap2GridSegments() const
{
   return obtenirChaineVecteur(glMap2GridSegments_, 2);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlMap2Index() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_MAP2_INDEX.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether 2D evaluation
/// generates color indices. The initial value is GL_FALSE. See glMap2.
///
/// @return Une chaîne représentant l'attribut GL_MAP2_INDEX.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlMap2Index() const
{
   return obtenirChaineBooleen(glMap2Index_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlMap2Normal() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_MAP2_NORMAL.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether 2D evaluation
/// generates normals. The initial value is GL_FALSE. See glMap2.
///
/// @return Une chaîne représentant l'attribut GL_MAP2_NORMAL.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlMap2Normal() const
{
   return obtenirChaineBooleen(glMap2Normal_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlMap2TextureCoord1() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_MAP2_TEXTURE_COORD_1.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether 2D evaluation
/// generates 1D texture coordinates. The initial value is GL_FALSE. See glMap2.
///
///
/// @return Une chaîne représentant l'attribut GL_MAP2_TEXTURE_COORD_1.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlMap2TextureCoord1() const
{
   return obtenirChaineBooleen(glMap2TextureCoord1_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlMap2TextureCoord2() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_MAP2_TEXTURE_COORD_2.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether 2D evaluation
/// generates 2D texture coordinates. The initial value is GL_FALSE. See glMap2.
///
/// @return Une chaîne représentant l'attribut GL_MAP2_TEXTURE_COORD_2.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlMap2TextureCoord2() const
{
   return obtenirChaineBooleen(glMap2TextureCoord2_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlMap2TextureCoord3() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_MAP2_TEXTURE_COORD_3.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether 2D evaluation
/// generates 3D texture coordinates. The initial value is GL_FALSE. See
/// glMap2.
///
/// @return Une chaîne représentant l'attribut GL_MAP2_TEXTURE_COORD_3.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlMap2TextureCoord3() const
{
   return obtenirChaineBooleen(glMap2TextureCoord3_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlMap2TextureCoord4() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_MAP2_TEXTURE_COORD_4.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether 2D evaluation
/// generates 4D texture coordinates. The initial value is GL_FALSE. See
/// glMap2.
///
/// @return Une chaîne représentant l'attribut GL_MAP2_TEXTURE_COORD_4.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlMap2TextureCoord4() const
{
   return obtenirChaineBooleen(glMap2TextureCoord4_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlMap2Vertex3() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_MAP2_VERTEX_3.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether 2D evaluation
/// generates 3D vertex coordinates. The initial value is GL_FALSE. See glMap2.
///
/// @return Une chaîne représentant l'attribut GL_MAP2_VERTEX_3.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlMap2Vertex3() const
{
   return obtenirChaineBooleen(glMap2Vertex3_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlMap2Vertex4() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_MAP2_VERTEX_4.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether 2D evaluation
/// generates 4D vertex coordinates. The initial value is GL_FALSE. See glMap2.
///
/// @return Une chaîne représentant l'attribut GL_MAP2_VERTEX_4.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlMap2Vertex4() const
{
   return obtenirChaineBooleen(glMap2Vertex4_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlMapColor() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_MAP_COLOR.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating if colors and color
/// indices are to be replaced by table lookup during pixel transfers. The
/// initial value is GL_FALSE. See glPixelTransfer.
///
/// @return Une chaîne représentant l'attribut GL_MAP_COLOR.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlMapColor() const
{
   return obtenirChaineBooleen(glMapColor_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlMapStencil() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_MAP_STENCIL.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating if stencil indices are to
/// be replaced by table lookup during pixel transfers. The initial value is
/// GL_FALSE. See glPixelTransfer.
///
/// @return Une chaîne représentant l'attribut GL_MAP_STENCIL.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlMapStencil() const
{
   return obtenirChaineBooleen(glMapStencil_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlMatrixMode() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_MATRIX_MODE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, a symbolic constant indicating which matrix
/// stack is currently the target of all matrix operations. The initial value
/// is GL_MODELVIEW. See glMatrixMode.
///
/// @return Une chaîne représentant l'attribut GL_MATRIX_MODE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlMatrixMode() const
{
   COMPARER_VALEUR(glMatrixMode_, GL_MODELVIEW);
   COMPARER_VALEUR(glMatrixMode_, GL_PROJECTION);
   COMPARER_VALEUR(glMatrixMode_, GL_TEXTURE);
   COMPARER_VALEUR(glMatrixMode_, GL_COLOR);
   COMPARER_DEFAUT(glMatrixMode_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlMaxClientAttribStackDepth() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_MAX_CLIENT_ATTRIB_STACK_DEPTH.
///
/// Selon le man page, cet attribut:
///
/// params returns one value indicating the maximum supported depth of the
/// client attribute stack. See glPushClientAttrib.
///
/// @return Une chaîne représentant l'attribut GL_MAX_CLIENT_ATTRIB_STACK_DEPTH.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlMaxClientAttribStackDepth() const
{
   return utilitaire::convertirEnChaine(glMaxClientAttribStackDepth_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlMaxAttribStackDepth() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_MAX_ATTRIB_STACK_DEPTH.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the maximum supported depth of the attribute
/// stack. The value must be at least 16. See glPushAttrib.
///
/// @return Une chaîne représentant l'attribut GL_MAX_ATTRIB_STACK_DEPTH.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlMaxAttribStackDepth() const
{
   return utilitaire::convertirEnChaine(glMaxAttribStackDepth_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlMaxClipPlanes() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_MAX_CLIP_PLANES.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the maximum number of application-defined
/// clipping planes. The value must be at least 6. See glClipPlane.
///
/// @return Une chaîne représentant l'attribut GL_MAX_CLIP_PLANES.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlMaxClipPlanes() const
{
   return utilitaire::convertirEnChaine(glMaxClipPlanes_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlMaxEvalOrder() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_MAX_EVAL_ORDER.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the maximum equation order supported by 1D and
/// 2D evaluators. The value must be at least 8. See glMap1 and glMap2.
///
/// @return Une chaîne représentant l'attribut GL_MAX_EVAL_ORDER.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlMaxEvalOrder() const
{
   return utilitaire::convertirEnChaine(glMaxEvalOrder_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlMaxLights() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_MAX_LIGHTS.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the maximum number of lights. The value must be
/// at least 8. See glLight.
///
/// @return Une chaîne représentant l'attribut GL_MAX_LIGHTS.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlMaxLights() const
{
   return utilitaire::convertirEnChaine(glMaxLights_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlMaxListNesting() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_MAX_LIST_NESTING.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the maximum recursion depth allowed during
/// display-list traversal. The value must be at least 64. See glCallList.
///
/// @return Une chaîne représentant l'attribut GL_MAX_LIST_NESTING.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlMaxListNesting() const
{
   return utilitaire::convertirEnChaine(glMaxListNesting_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlMaxModelviewStackDepth() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_MAX_MODELVIEW_STACK_DEPTH.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the maximum supported depth of the modelview
/// matrix stack. The value must be at least 32. See glPushMatrix.
///
/// @return Une chaîne représentant l'attribut GL_MAX_MODELVIEW_STACK_DEPTH.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlMaxModelviewStackDepth() const
{
   return utilitaire::convertirEnChaine(glMaxModelviewStackDepth_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlMaxNameStackDepth() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_MAX_NAME_STACK_DEPTH.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the maximum supported depth of the selection
/// name stack. The value must be at least 64. See glPushName.
///
/// @return Une chaîne représentant l'attribut GL_MAX_NAME_STACK_DEPTH.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlMaxNameStackDepth() const
{
   return utilitaire::convertirEnChaine(glMaxNameStackDepth_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlMaxPixelMapTable() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_MAX_PIXEL_MAP_TABLE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the maximum supported size of a glPixelMap
/// lookup table. The value must be at least 32. See glPixelMap.
///
/// @return Une chaîne représentant l'attribut GL_MAX_PIXEL_MAP_TABLE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlMaxPixelMapTable() const
{
   return utilitaire::convertirEnChaine(glMaxPixelMapTable_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlMaxProjectionStackDepth() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_MAX_PROJECTION_STACK_DEPTH.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the maximum supported depth of the projection
/// matrix stack. The value must be at least 2. See glPushMatrix.
///
/// @return Une chaîne représentant l'attribut GL_MAX_PROJECTION_STACK_DEPTH.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlMaxProjectionStackDepth() const
{
   return utilitaire::convertirEnChaine(glMaxProjectionStackDepth_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlMaxTextureSize() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_MAX_TEXTURE_SIZE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value. The value gives a rough estimate of the largest
/// texture that the GL can handle. If the GL version is 1.1 or greater, use
/// GL_PROXY_TEXTURE_1D or GL_PROXY_TEXTURE_2D to determine if a texture is
/// too large. See glTexImage1D and glTexImage2D.
///
/// @return Une chaîne représentant l'attribut GL_MAX_TEXTURE_SIZE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlMaxTextureSize() const
{
   return utilitaire::convertirEnChaine(glMaxTextureSize_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlMaxTextureStackDepth() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_MAX_TEXTURE_STACK_DEPTH.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the maximum supported depth of the texture
/// matrix stack. The value must be at least 2. See glPushMatrix.
///
/// @return Une chaîne représentant l'attribut GL_MAX_TEXTURE_STACK_DEPTH.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlMaxTextureStackDepth() const
{
   return utilitaire::convertirEnChaine(glMaxTextureStackDepth_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlMaxViewportDims() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_MAX_VIEWPORT_DIMS.
///
/// Selon le man page, cet attribut:
///
/// params returns two values: the maximum supported width and height of the
/// viewport. These must be at least as large as the visible dimensions of the
/// display being rendered to. See glViewport.
///
/// @return Une chaîne représentant l'attribut GL_MAX_VIEWPORT_DIMS.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlMaxViewportDims() const
{
   return utilitaire::convertirEnChaine(glMaxViewportDims_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlModelviewMatrix() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_MODELVIEW_MATRIX.
///
/// Selon le man page, cet attribut:
///
/// params returns sixteen values: the modelview matrix on the top of the
/// modelview matrix stack. Initially this matrix is the identity matrix. See
/// glPushMatrix.
///
/// @return Une chaîne représentant l'attribut GL_MODELVIEW_MATRIX.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlModelviewMatrix() const
{
   return obtenirChaineVecteur(glModelviewMatrix_, 16);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlModelviewStackDepth() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_MODELVIEW_STACK_DEPTH.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the number of matrices on the modelview matrix
/// stack. The initial value is 1. See glPushMatrix.
///
/// @return Une chaîne représentant l'attribut GL_MODELVIEW_STACK_DEPTH.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlModelviewStackDepth() const
{
   return utilitaire::convertirEnChaine(glModelviewStackDepth_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlNameStackDepth() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_NAME_STACK_DEPTH.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the number of names on the selection name stack.
/// The initial value is 0. See glPushName.
///
/// @return Une chaîne représentant l'attribut GL_NAME_STACK_DEPTH.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlNameStackDepth() const
{
   return utilitaire::convertirEnChaine(glNameStackDepth_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlNormalArray() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_NORMAL_ARRAY.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value, indicating whether the normal array
/// is enabled. The initial value is GL_FALSE. See glNormalPointer.
///
/// @return Une chaîne représentant l'attribut GL_NORMAL_ARRAY.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlNormalArray() const
{
   return obtenirChaineBooleen(glNormalArray_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlNormalArrayStride() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_NORMAL_ARRAY_STRIDE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the byte offset between consecutive normals in
/// the normal array. The initial value is 0. See glNormalPointer.
///
/// @return Une chaîne représentant l'attribut GL_NORMAL_ARRAY_STRIDE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlNormalArrayStride() const
{
   return utilitaire::convertirEnChaine(glNormalArrayStride_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlNormalArrayType() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_NORMAL_ARRAY_TYPE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the data type of each coordinate in the normal
/// array. The initial value is GL_FLOAT. See glNormalPointer.
///
/// @return Une chaîne représentant l'attribut GL_NORMAL_ARRAY_TYPE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlNormalArrayType() const
{
   COMPARER_VALEUR(glNormalArrayType_, GL_BYTE);
   COMPARER_VALEUR(glNormalArrayType_, GL_SHORT);
   COMPARER_VALEUR(glNormalArrayType_, GL_INT);
   COMPARER_VALEUR(glNormalArrayType_, GL_FLOAT);
   COMPARER_VALEUR(glNormalArrayType_, GL_DOUBLE);
   COMPARER_DEFAUT(glNormalArrayType_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlNormalize() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_NORMALIZE.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether normals are
/// automatically scaled to unit length after they have been transformed to
/// eye coordinates. The initial value is GL_FALSE. See glNormal.
///
/// @return Une chaîne représentant l'attribut GL_NORMALIZE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlNormalize() const
{
   return obtenirChaineBooleen(glNormalize_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlPackAlignment() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_PACK_ALIGNMENT.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the byte alignment used for writing pixel data
/// to memory. The initial value is 4. See glPixelStore.
///
/// @return Une chaîne représentant l'attribut GL_PACK_ALIGNMENT.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlPackAlignment() const
{
   return utilitaire::convertirEnChaine(glPackAlignment_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlPackLsbFirst() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_PACK_LSB_FIRST.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether single-bit pixels
/// being written to memory are written first to the least significant bit of
/// each unsigned byte. The initial value is GL_FALSE. See glPixelStore.
///
/// @return Une chaîne représentant l'attribut GL_PACK_LSB_FIRST.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlPackLsbFirst() const
{
   return obtenirChaineBooleen(glPackLsbFirst_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlPackRowLength() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_PACK_ROW_LENGTH.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the row length used for writing pixel data to
/// memory. The initial value is 0. See glPixelStore.
///
/// @return Une chaîne représentant l'attribut GL_PACK_ROW_LENGTH.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlPackRowLength() const
{
   return utilitaire::convertirEnChaine(glPackRowLength_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlPackSkipPixels() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_PACK_SKIP_PIXELS.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the number of pixel locations skipped before the
/// first pixel is written into memory. The initial value is 0. See
/// glPixelStore.
///
/// @return Une chaîne représentant l'attribut GL_PACK_SKIP_PIXELS.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlPackSkipPixels() const
{
   return utilitaire::convertirEnChaine(glPackSkipPixels_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlPackSkipRows() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_PACK_SKIP_ROWS.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the number of rows of pixel locations skipped
/// before the first pixel is written into memory. The initial value is 0. See
/// glPixelStore.
///
/// @return Une chaîne représentant l'attribut GL_PACK_SKIP_ROWS.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlPackSkipRows() const
{
   return utilitaire::convertirEnChaine(glPackSkipRows_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlPackSwapBytes() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_PACK_SWAP_BYTES.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether the bytes of
/// two-byte and four-byte pixel indices and components are swapped before
/// being written to memory. The initial value is GL_FALSE. See glPixelStore.
///
/// @return Une chaîne représentant l'attribut GL_PACK_SWAP_BYTES.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlPackSwapBytes() const
{
   return obtenirChaineBooleen(glPackSwapBytes_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlPerspectiveCorrectionHint() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_PERSPECTIVE_CORRECTION_HINT.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, a symbolic constant indicating the mode of the
/// perspective correction hint. The initial value is GL_DONT_CARE. See
/// glHint.
///
/// @return Une chaîne représentant l'attribut GL_PERSPECTIVE_CORRECTION_HINT.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlPerspectiveCorrectionHint() const
{
   COMPARER_VALEUR(glPerspectiveCorrectionHint_, GL_FASTEST);
   COMPARER_VALEUR(glPerspectiveCorrectionHint_, GL_NICEST);
   COMPARER_VALEUR(glPerspectiveCorrectionHint_, GL_DONT_CARE);
   COMPARER_DEFAUT(glPerspectiveCorrectionHint_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlPixelMapAToASize() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_PIXEL_MAP_A_TO_A_SIZE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the size of the alpha-to-alpha pixel translation
/// table. The initial value is 1. See glPixelMap.
///
/// @return Une chaîne représentant l'attribut GL_PIXEL_MAP_A_TO_A_SIZE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlPixelMapAToASize() const
{
   return utilitaire::convertirEnChaine(glPixelMapAToASize_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlPixelMapBToBSize() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_PIXEL_MAP_B_TO_B_SIZE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the size of the blue-to-blue pixel translation
/// table. The initial value is 1. See glPixelMap.
///
/// @return Une chaîne représentant l'attribut GL_PIXEL_MAP_B_TO_B_SIZE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlPixelMapBToBSize() const
{
   return utilitaire::convertirEnChaine(glPixelMapBToBSize_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlPixelMapGToGSize() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_PIXEL_MAP_G_TO_G_SIZE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the size of the green-to-green pixel translation
/// table. The initial value is 1. See glPixelMap.
///
/// @return Une chaîne représentant l'attribut GL_PIXEL_MAP_G_TO_G_SIZE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlPixelMapGToGSize() const
{
   return utilitaire::convertirEnChaine(glPixelMapGToGSize_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlPixelMapIToASize() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_PIXEL_MAP_I_TO_A_SIZE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the size of the index-to-alpha pixel translation
/// table. The initial value is 1. See glPixelMap.
///
/// @return Une chaîne représentant l'attribut GL_PIXEL_MAP_I_TO_A_SIZE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlPixelMapIToASize() const
{
   return utilitaire::convertirEnChaine(glPixelMapIToASize_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlPixelMapIToBSize() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_PIXEL_MAP_I_TO_B_SIZE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the size of the index-to-blue pixel translation
/// table. The initial value is 1. See glPixelMap.
///
/// @return Une chaîne représentant l'attribut GL_PIXEL_MAP_I_TO_B_SIZE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlPixelMapIToBSize() const
{
   return utilitaire::convertirEnChaine(glPixelMapIToBSize_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlPixelMapIToGSize() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_PIXEL_MAP_I_TO_G_SIZE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the size of the index-to-green pixel translation
/// table. The initial value is 1. See glPixelMap.
///
/// @return Une chaîne représentant l'attribut GL_PIXEL_MAP_I_TO_G_SIZE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlPixelMapIToGSize() const
{
   return utilitaire::convertirEnChaine(glPixelMapIToGSize_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlPixelMapIToISize() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_PIXEL_MAP_I_TO_I_SIZE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the size of the index-to-index pixel translation
/// table. The initial value is 1. See glPixelMap.
///
/// @return Une chaîne représentant l'attribut GL_PIXEL_MAP_I_TO_I_SIZE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlPixelMapIToISize() const
{
   return utilitaire::convertirEnChaine(glPixelMapIToISize_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlPixelMapIToRSize() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_PIXEL_MAP_I_TO_R_SIZE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the size of the index-to-red pixel translation
/// table. The initial value is 1. See glPixelMap.
///
/// @return Une chaîne représentant l'attribut GL_PIXEL_MAP_I_TO_R_SIZE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlPixelMapIToRSize() const
{
   return utilitaire::convertirEnChaine(glPixelMapIToRSize_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlPixelMapRToRSize() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_PIXEL_MAP_R_TO_R_SIZE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the size of the red-to-red pixel translation
/// table. The initial value is 1. See glPixelMap.
///
/// @return Une chaîne représentant l'attribut GL_PIXEL_MAP_R_TO_R_SIZE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlPixelMapRToRSize() const
{
   return utilitaire::convertirEnChaine(glPixelMapRToRSize_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlPixelMapSToSSize() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_PIXEL_MAP_S_TO_S_SIZE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the size of the stencil-to-stencil pixel
/// translation table. The initial value is 1. See glPixelMap.
///
/// @return Une chaîne représentant l'attribut GL_PIXEL_MAP_S_TO_S_SIZE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlPixelMapSToSSize() const
{
   return utilitaire::convertirEnChaine(glPixelMapSToSSize_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlPointSize() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_POINT_SIZE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the point size as specified by glPointSize. The
/// initial value is 1.
///
/// @return Une chaîne représentant l'attribut GL_POINT_SIZE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlPointSize() const
{
   return utilitaire::convertirEnChaine(glPointSize_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlPointSizeGranularity() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_POINT_SIZE_GRANULARITY.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the size difference between adjacent supported
/// sizes for antialiased points. See glPointSize.
///
/// @return Une chaîne représentant l'attribut GL_POINT_SIZE_GRANULARITY.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlPointSizeGranularity() const
{
   return utilitaire::convertirEnChaine(glPointSizeGranularity_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlPointSizeRange() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_POINT_SIZE_RANGE.
///
/// Selon le man page, cet attribut:
///
/// params returns two values: the smallest and largest supported sizes for
/// antialiased points. The smallest size must be at most 1, and the largest
/// size must be at least 1. See glPointSize.
///
/// @return Une chaîne représentant l'attribut GL_POINT_SIZE_RANGE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlPointSizeRange() const
{
   return obtenirChaineVecteur(glPointSizeRange_, 2);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlPointSmooth() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_POINT_SMOOTH.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether antialiasing of
/// points is enabled. The initial value is GL_FALSE. See glPointSize.
///
/// @return Une chaîne représentant l'attribut GL_POINT_SMOOTH.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlPointSmooth() const
{
   return obtenirChaineBooleen(glPointSmooth_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlPointSmoothHint() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_POINT_SMOOTH_HINT.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, a symbolic constant indicating the mode of the
/// point antialiasing hint. The initial value is GL_DONT_CARE. See glHint.
///
/// @return Une chaîne représentant l'attribut GL_POINT_SMOOTH_HINT.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlPointSmoothHint() const
{
   COMPARER_VALEUR(glPointSmoothHint_, GL_FASTEST);
   COMPARER_VALEUR(glPointSmoothHint_, GL_NICEST);
   COMPARER_VALEUR(glPointSmoothHint_, GL_DONT_CARE);
   COMPARER_DEFAUT(glPointSmoothHint_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlPolygonMode() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_POLYGON_MODE.
///
/// Selon le man page, cet attribut:
///
/// params returns two values: symbolic constants indicating whether
/// front-facing and back-facing polygons are rasterized as points, lines, or
/// filled polygons. The initial value is GL_FILL. See glPolygonMode.
///
/// @return Une chaîne représentant l'attribut GL_POLYGON_MODE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlPolygonMode() const
{
   std::ostringstream o;
   o << "(";
   for (unsigned int i = 0 ; i < 2; ++i) {
      if (i != 0)
         o << " , ";

      switch (glPolygonMode_[i])
      {
      case GL_POINT: o << "GL_POINT"; break;
      case GL_LINE : o << "GL_LINE"; break;
      case GL_FILL : o << "GL_FILL"; break;
      default:
         o << "GL_??? ("
           << utilitaire::convertirEnChaine(glPolygonMode_[i])
           << ")";
         break;
      }
   }
   o << " )";
   return o.str();
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlPolygonOffsetFactor() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_POLYGON_OFFSET_FACTOR.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the scaling factor used to determine the
/// variable offset that is added to the depth value of each fragment
/// generated when a polygon is rasterized. The initial value is 0. See
/// glPolygonOffset.
///
/// @return Une chaîne représentant l'attribut GL_POLYGON_OFFSET_FACTOR.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlPolygonOffsetFactor() const
{
   return utilitaire::convertirEnChaine(glPolygonOffsetFactor_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlPolygonOffsetUnits() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_POLYGON_OFFSET_UNITS.
///
/// Selon le man page, cet attribut:
///
/// params returns one value. This value is multiplied by an
/// implementation-specific value and then added to the depth value of each
/// fragment generated when a polygon is rasterized. The initial value is 0.
/// See glPolygonOffset.
///
/// @return Une chaîne représentant l'attribut GL_POLYGON_OFFSET_UNITS.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlPolygonOffsetUnits() const
{
   return utilitaire::convertirEnChaine(glPolygonOffsetUnits_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlPolygonOffsetFill() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_POLYGON_OFFSET_FILL.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether polygon offset is
/// enabled for polygons in fill mode. The initial value is GL_FALSE. See
/// glPolygonOffset.
///
/// @return Une chaîne représentant l'attribut GL_POLYGON_OFFSET_FILL.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlPolygonOffsetFill() const
{
   return obtenirChaineBooleen(glPolygonOffsetFill_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlPolygonOffsetLine() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_POLYGON_OFFSET_LINE.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether polygon offset is
/// enabled for polygons in line mode. The initial value is GL_FALSE. See
/// glPolygonOffset.
///
/// @return Une chaîne représentant l'attribut GL_POLYGON_OFFSET_LINE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlPolygonOffsetLine() const
{
   return obtenirChaineBooleen(glPolygonOffsetLine_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlPolygonOffsetPoint() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_POLYGON_OFFSET_POINT.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether polygon offset is
/// enabled for polygons in point mode. The initial value is GL_FALSE. See
/// glPolygonOffset.
///
/// @return Une chaîne représentant l'attribut GL_POLYGON_OFFSET_POINT.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlPolygonOffsetPoint() const
{
   return obtenirChaineBooleen(glPolygonOffsetPoint_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlPolygonSmooth() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_POLYGON_SMOOTH.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether antialiasing of
/// polygons is enabled. The initial value is GL_FALSE. See glPolygonMode.
///
/// @return Une chaîne représentant l'attribut GL_POLYGON_SMOOTH.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlPolygonSmooth() const
{
   return obtenirChaineBooleen(glPolygonSmooth_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlPolygonSmoothHint() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_POLYGON_SMOOTH_HINT.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, a symbolic constant indicating the mode of the
/// polygon antialiasing hint. The initial value is GL_DONT_CARE. See glHint.
///
/// @return Une chaîne représentant l'attribut GL_POLYGON_SMOOTH_HINT.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlPolygonSmoothHint() const
{
   COMPARER_VALEUR(glPolygonSmoothHint_, GL_FASTEST);
   COMPARER_VALEUR(glPolygonSmoothHint_, GL_NICEST);
   COMPARER_VALEUR(glPolygonSmoothHint_, GL_DONT_CARE);
   COMPARER_DEFAUT(glPolygonSmoothHint_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlPolygonStipple() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_POLYGON_STIPPLE.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether polygon stippling
/// is enabled. The initial value is GL_FALSE. See glPolygonStipple.
///
/// @return Une chaîne représentant l'attribut GL_POLYGON_STIPPLE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlPolygonStipple() const
{
   return obtenirChaineBooleen(glPolygonStipple_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlProjectionMatrix() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_PROJECTION_MATRIX.
///
/// Selon le man page, cet attribut:
///
/// params returns sixteen values: the projection matrix on the top of the
/// projection matrix stack. Initially this matrix is the identity matrix. See glPushMatrix.
///
/// @return Une chaîne représentant l'attribut GL_PROJECTION_MATRIX.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlProjectionMatrix() const
{
   return obtenirChaineVecteur(glProjectionMatrix_, 16);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlProjectionStackDepth() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_PROJECTION_STACK_DEPTH.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the number of matrices on the projection matrix
/// stack. The initial value is 1. See glPushMatrix.
///
/// @return Une chaîne représentant l'attribut GL_PROJECTION_STACK_DEPTH.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlProjectionStackDepth() const
{
   return utilitaire::convertirEnChaine(glProjectionStackDepth_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlReadBuffer() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_READ_BUFFER.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, a symbolic constant indicating which color
/// buffer is selected for reading. The initial value is GL_BACK if there is a
/// back buffer, otherwise it is GL_FRONT. See glReadPixels and glAccum.
///
/// @return Une chaîne représentant l'attribut GL_READ_BUFFER.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlReadBuffer() const
{
   COMPARER_VALEUR(glReadBuffer_, GL_FRONT_LEFT);
   COMPARER_VALEUR(glReadBuffer_, GL_FRONT_RIGHT);
   COMPARER_VALEUR(glReadBuffer_, GL_BACK_LEFT);
   COMPARER_VALEUR(glReadBuffer_, GL_BACK_RIGHT);
   COMPARER_VALEUR(glReadBuffer_, GL_FRONT);
   COMPARER_VALEUR(glReadBuffer_, GL_BACK);
   COMPARER_VALEUR(glReadBuffer_, GL_LEFT);
   COMPARER_VALEUR(glReadBuffer_, GL_RIGHT);
   COMPARER_VALEUR(glReadBuffer_, GL_AUX0);
   COMPARER_VALEUR(glReadBuffer_, GL_AUX1);
   COMPARER_VALEUR(glReadBuffer_, GL_AUX2);
   COMPARER_VALEUR(glReadBuffer_, GL_AUX3);
   COMPARER_DEFAUT(glReadBuffer_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlRedBias() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_RED_BIAS.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the red bias factor used during pixel transfers.
/// The initial value is 0.
///
/// @return Une chaîne représentant l'attribut GL_RED_BIAS.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlRedBias() const
{
   return utilitaire::convertirEnChaine(glRedBias_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlRedBits() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_RED_BITS.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the number of red bitplanes in each color
/// buffer.
///
/// @return Une chaîne représentant l'attribut GL_RED_BITS.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlRedBits() const
{
   return utilitaire::convertirEnChaine(glRedBits_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlRedScale() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_RED_SCALE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the red scale factor used during pixel
/// transfers. The initial value is 1. See glPixelTransfer.
///
/// @return Une chaîne représentant l'attribut GL_RED_SCALE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlRedScale() const
{
   return utilitaire::convertirEnChaine(glRedScale_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlRenderMode() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_RENDER_MODE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, a symbolic constant indicating whether the GL is
/// in render, select, or feedback mode. The initial value is GL_RENDER. See
/// glRenderMode.
///
/// @return Une chaîne représentant l'attribut GL_RENDER_MODE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlRenderMode() const
{
   COMPARER_VALEUR(glRenderMode_, GL_RENDER);
   COMPARER_VALEUR(glRenderMode_, GL_SELECT);
   COMPARER_VALEUR(glRenderMode_, GL_FEEDBACK);
   COMPARER_DEFAUT(glRenderMode_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlRgbaMode() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_RGBA_MODE.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether the GL is in RGBA
/// mode (true) or color index mode (false). See glColor.
///
/// @return Une chaîne représentant l'attribut GL_RGBA_MODE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlRgbaMode() const
{
   return obtenirChaineBooleen(glRgbaMode_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlScissorBox() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_SCISSOR_BOX.
///
/// Selon le man page, cet attribut:
///
/// params returns four values: the x and y window coordinates of the scissor
/// box, followed by its width and height. Initially the x and y window
/// coordinates are both 0 and the width and height are set to the size of the
/// window. See glScissor.
///
/// @return Une chaîne représentant l'attribut GL_SCISSOR_BOX.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlScissorBox() const
{
   return obtenirChaineVecteur(glScissorBox_, 4);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlScissorTest() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_SCISSOR_TEST.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether scissoring is
/// enabled. The initial value is GL_FALSE. See glScissor.
///
/// @return Une chaîne représentant l'attribut GL_SCISSOR_TEST.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlScissorTest() const
{
   return obtenirChaineBooleen(glScissorTest_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlSelectionBufferSize() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_SELECTION_BUFFER_SIZE.
///
/// Selon le man page, cet attribut:
///
/// params return one value, the size of the selection buffer. See
/// glSelectBuffer.
///
/// @return Une chaîne représentant l'attribut GL_SELECTION_BUFFER_SIZE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlSelectionBufferSize() const
{
   return utilitaire::convertirEnChaine(glSelectionBufferSize_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlShadeModel() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_SHADE_MODEL.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, a symbolic constant indicating whether the
/// shading mode is flat or smooth. The initial value is GL_SMOOTH. See
/// glShadeModel.
///
/// @return Une chaîne représentant l'attribut GL_SHADE_MODEL.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlShadeModel() const
{
   COMPARER_VALEUR(glShadeModel_, GL_FLAT);
   COMPARER_VALEUR(glShadeModel_, GL_SMOOTH);
   COMPARER_DEFAUT(glShadeModel_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlStencilBits() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_STENCIL_BITS.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the number of bitplanes in the stencil buffer.
///
/// @return Une chaîne représentant l'attribut GL_STENCIL_BITS.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlStencilBits() const
{
   return utilitaire::convertirEnChaine(glStencilBits_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlStencilClearValue() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_STENCIL_CLEAR_VALUE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the index to which the stencil bitplanes are
/// cleared. The initial value is 0. See glClearStencil.
///
/// @return Une chaîne représentant l'attribut GL_STENCIL_CLEAR_VALUE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlStencilClearValue() const
{
   return utilitaire::convertirEnChaine(glStencilClearValue_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlStencilFail() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_STENCIL_FAIL.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, a symbolic constant indicating what action is
/// taken when the stencil test fails. The initial value is GL_KEEP. See
/// glStencilOp.
///
/// @return Une chaîne représentant l'attribut GL_STENCIL_FAIL.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlStencilFail() const
{
   COMPARER_VALEUR(glStencilFail_, GL_KEEP);
   COMPARER_VALEUR(glStencilFail_, GL_ZERO);
   COMPARER_VALEUR(glStencilFail_, GL_REPLACE);
   COMPARER_VALEUR(glStencilFail_, GL_INCR);
   COMPARER_VALEUR(glStencilFail_, GL_DECR);
   COMPARER_VALEUR(glStencilFail_, GL_INVERT);
   COMPARER_DEFAUT(glStencilFail_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlStencilFunc() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_STENCIL_FUNC.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, a symbolic constant indicating what function is
/// used to compare the stencil reference value with the stencil buffer value.
/// The initial value is GL_ALWAYS. See glStencilFunc.
///
/// @return Une chaîne représentant l'attribut GL_STENCIL_FUNC.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlStencilFunc() const
{
   COMPARER_VALEUR(glStencilFunc_, GL_NEVER);
   COMPARER_VALEUR(glStencilFunc_, GL_LESS);
   COMPARER_VALEUR(glStencilFunc_, GL_LEQUAL);
   COMPARER_VALEUR(glStencilFunc_, GL_GREATER);
   COMPARER_VALEUR(glStencilFunc_, GL_GEQUAL);
   COMPARER_VALEUR(glStencilFunc_, GL_EQUAL);
   COMPARER_VALEUR(glStencilFunc_, GL_NOTEQUAL);
   COMPARER_VALEUR(glStencilFunc_, GL_ALWAYS);
   COMPARER_DEFAUT(glStencilFunc_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlStencilPassDepthFail() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_STENCIL_PASS_DEPTH_FAIL.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, a symbolic constant indicating what action is
/// taken when the stencil test passes, but the depth test fails. The initial
/// value is GL_KEEP. See glStencilOp.
///
/// @return Une chaîne représentant l'attribut GL_STENCIL_PASS_DEPTH_FAIL.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlStencilPassDepthFail() const
{
   COMPARER_VALEUR(glStencilPassDepthFail_, GL_KEEP);
   COMPARER_VALEUR(glStencilPassDepthFail_, GL_ZERO);
   COMPARER_VALEUR(glStencilPassDepthFail_, GL_REPLACE);
   COMPARER_VALEUR(glStencilPassDepthFail_, GL_INCR);
   COMPARER_VALEUR(glStencilPassDepthFail_, GL_DECR);
   COMPARER_VALEUR(glStencilPassDepthFail_, GL_INVERT);
   COMPARER_DEFAUT(glStencilPassDepthFail_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlStencilPassDepthPass() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_STENCIL_PASS_DEPTH_PASS.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, a symbolic constant indicating what action is
/// taken when the stencil test passes and the depth test passes. The initial
/// value is GL_KEEP. See glStencilOp.
///
/// @return Une chaîne représentant l'attribut GL_STENCIL_PASS_DEPTH_PASS.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlStencilPassDepthPass() const
{
   COMPARER_VALEUR(glStencilPassDepthPass_, GL_KEEP);
   COMPARER_VALEUR(glStencilPassDepthPass_, GL_ZERO);
   COMPARER_VALEUR(glStencilPassDepthPass_, GL_REPLACE);
   COMPARER_VALEUR(glStencilPassDepthPass_, GL_INCR);
   COMPARER_VALEUR(glStencilPassDepthPass_, GL_DECR);
   COMPARER_VALEUR(glStencilPassDepthPass_, GL_INVERT);
   COMPARER_DEFAUT(glStencilPassDepthPass_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlStencilRef() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_STENCIL_REF.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the reference value that is compared with the
/// contents of the stencil buffer. The initial value is 0. See glStencilFunc.
///
/// @return Une chaîne représentant l'attribut GL_STENCIL_REF.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlStencilRef() const
{
   return utilitaire::convertirEnChaine(glStencilRef_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlStencilTest() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_STENCIL_TEST.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether stencil testing
/// of fragments is enabled. The initial value is GL_FALSE. See glStencilFunc
/// and glStencilOp.
///
/// @return Une chaîne représentant l'attribut GL_STENCIL_TEST.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlStencilTest() const
{
   return obtenirChaineBooleen(glStencilTest_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlStencilValueMask() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_STENCIL_VALUE_MASK.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the mask that is used to mask both the stencil
/// reference value and the stencil buffer value before they are compared. The
/// initial value is all 1's. See glStencilFunc.
///
/// @return Une chaîne représentant l'attribut GL_STENCIL_VALUE_MASK.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlStencilValueMask() const
{
   return utilitaire::convertirEnChaine(glStencilValueMask_, std::hex);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlStencilWritemask() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_STENCIL_WRITEMASK.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the mask that controls writing of the stencil
/// bitplanes. The initial value is all 1's. See glStencilMask.
///
/// @return Une chaîne représentant l'attribut GL_STENCIL_WRITEMASK.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlStencilWritemask() const
{
   return utilitaire::convertirEnChaine(glStencilWritemask_, std::hex);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlStereo() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_STEREO.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether stereo buffers
/// (left and right) are supported.
///
/// @return Une chaîne représentant l'attribut GL_STEREO.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlStereo() const
{
   return obtenirChaineBooleen(glStereo_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlSubpixelBits() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_SUBPIXEL_BITS.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, an estimate of the number of bits of subpixel
/// resolution that are used to position rasterized geometry in window
/// coordinates. The initial value is 4.
///
/// @return Une chaîne représentant l'attribut GL_SUBPIXEL_BITS.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlSubpixelBits() const
{
   return utilitaire::convertirEnChaine(glSubpixelBits_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlTexture1D() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_TEXTURE_1D.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether 1D texture
/// mapping is enabled. The initial value is GL_FALSE. See glTexImage1D.
///
/// @return Une chaîne représentant l'attribut GL_TEXTURE_1D.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlTexture1D() const
{
   return obtenirChaineBooleen(glTexture1D_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlTextureBinding1D() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_TEXTURE_BINDING_1D.
///
/// Selon le man page, cet attribut:
///
/// params returns a single value, the name of the texture currently bound to
/// the target GL_TEXTURE_1D. The initial value is 0. See glBindTexture.
///
/// @return Une chaîne représentant l'attribut GL_TEXTURE_BINDING_1D.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlTextureBinding1D() const
{
   return utilitaire::convertirEnChaine(glTextureBinding1D_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlTexture2D() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_TEXTURE_2D.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether 2D texture
/// mapping is enabled. The initial value is GL_FALSE. See glTexImage2D.
///
/// @return Une chaîne représentant l'attribut GL_TEXTURE_2D.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlTexture2D() const
{
   return obtenirChaineBooleen(glTexture2D_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlTextureBinding2D() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_TEXTURE_BINDING_2D.
///
/// Selon le man page, cet attribut:
///
/// params returns a single value, the name of the texture currently bound to
/// the target GL_TEXTURE_2D. The initial value is 0. See glBindTexture.
///
/// @return Une chaîne représentant l'attribut GL_TEXTURE_BINDING_2D.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlTextureBinding2D() const
{
   return utilitaire::convertirEnChaine(glTextureBinding2D_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlTextureCoordArray() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_TEXTURE_COORD_ARRAY.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether the texture
/// coordinate array is enabled. The initial value is GL_FALSE. See
/// glTexCoordPointer.
///
/// @return Une chaîne représentant l'attribut GL_TEXTURE_COORD_ARRAY.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlTextureCoordArray() const
{
   return obtenirChaineBooleen(glTextureCoordArray_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlTextureCoordArraySize() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_TEXTURE_COORD_ARRAY_SIZE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the number of coordinates per element in the
/// texture coordinate array. The initial value is 4. See glTexCoordPointer.
///
/// @return Une chaîne représentant l'attribut GL_TEXTURE_COORD_ARRAY_SIZE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlTextureCoordArraySize() const
{
   return utilitaire::convertirEnChaine(glTextureCoordArraySize_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlTextureCoordArrayStride() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_TEXTURE_COORD_ARRAY_STRIDE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the byte offset between consecutive elements in
/// the texture coordinate array. The initial value is 0. See
/// glTexCoordPointer.
///
/// @return Une chaîne représentant l'attribut GL_TEXTURE_COORD_ARRAY_STRIDE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlTextureCoordArrayStride() const
{
   return utilitaire::convertirEnChaine(glTextureCoordArrayStride_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlTextureCoordArrayType() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_TEXTURE_COORD_ARRAY_TYPE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the data type of the coordinates in the texture
/// coordinate array. The initial value is GL_FLOAT. See glTexCoordPointer.
///
/// @return Une chaîne représentant l'attribut GL_TEXTURE_COORD_ARRAY_TYPE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlTextureCoordArrayType() const
{
   COMPARER_VALEUR(glTextureCoordArrayType_, GL_SHORT);
   COMPARER_VALEUR(glTextureCoordArrayType_, GL_INT);
   COMPARER_VALEUR(glTextureCoordArrayType_, GL_FLOAT);
   COMPARER_VALEUR(glTextureCoordArrayType_, GL_DOUBLE);
   COMPARER_DEFAUT(glTextureCoordArrayType_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlTextureGenQ() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_TEXTURE_GEN_Q.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether automatic
/// generation of the q texture coordinate is enabled. The initial value is
/// GL_FALSE. See glTexGen.
///
/// @return Une chaîne représentant l'attribut GL_TEXTURE_GEN_Q.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlTextureGenQ() const
{
   return obtenirChaineBooleen(glTextureGenQ_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlTextureGenR() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_TEXTURE_GEN_R.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether automatic
/// generation of the r texture coordinate is enabled. The initial value is
/// GL_FALSE. See glTexGen.
///
/// @return Une chaîne représentant l'attribut GL_TEXTURE_GEN_R.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlTextureGenR() const
{
   return obtenirChaineBooleen(glTextureGenR_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlTextureGenS() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_TEXTURE_GEN_S.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether automatic
/// generation of the S texture coordinate is enabled. The initial value is
/// GL_FALSE. See glTexGen.
///
/// @return Une chaîne représentant l'attribut GL_TEXTURE_GEN_S.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlTextureGenS() const
{
   return obtenirChaineBooleen(glTextureGenS_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlTextureGenT() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_TEXTURE_GEN_T.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether automatic
/// generation of the T texture coordinate is enabled. The initial value is
/// GL_FALSE. See glTexGen. 
///
/// @return Une chaîne représentant l'attribut GL_TEXTURE_GEN_T.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlTextureGenT() const
{
   return obtenirChaineBooleen(glTextureGenT_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlTextureMatrix() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_TEXTURE_MATRIX.
///
/// Selon le man page, cet attribut:
///
/// params returns sixteen values: the texture matrix on the top of the
/// texture matrix stack. Initially this matrix is the identity matrix. See
/// glPushMatrix.
///
/// @return Une chaîne représentant l'attribut GL_TEXTURE_MATRIX.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlTextureMatrix() const
{
   return obtenirChaineVecteur(glTextureMatrix_, 16);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlTextureStackDepth() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_TEXTURE_STACK_DEPTH.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the number of matrices on the texture matrix
/// stack. The initial value is 1. See glPushMatrix.
///
/// @return Une chaîne représentant l'attribut GL_TEXTURE_STACK_DEPTH.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlTextureStackDepth() const
{
   return utilitaire::convertirEnChaine(glTextureStackDepth_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlUnpackAlignment() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_UNPACK_ALIGNMENT.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the byte alignment used for reading pixel data
/// from memory. The initial value is 4. See glPixelStore.
///
/// @return Une chaîne représentant l'attribut GL_UNPACK_ALIGNMENT.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlUnpackAlignment() const
{
   return utilitaire::convertirEnChaine(glUnpackAlignment_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlUnpackLsbFirst() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_UNPACK_LSB_FIRST.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether single-bit pixels
/// being read from memory are read first from the least significant bit of
/// each unsigned byte. The initial value is GL_FALSE. See glPixelStore.
///
/// @return Une chaîne représentant l'attribut GL_UNPACK_LSB_FIRST.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlUnpackLsbFirst() const
{
   return obtenirChaineBooleen(glUnpackLsbFirst_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlUnpackRowLength() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_UNPACK_ROW_LENGTH.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the row length used for reading pixel data from
/// memory. The initial value is 0. See glPixelStore.
///
/// @return Une chaîne représentant l'attribut GL_UNPACK_ROW_LENGTH.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlUnpackRowLength() const
{
   return utilitaire::convertirEnChaine(glUnpackRowLength_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlUnpackSkipPixels() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_UNPACK_SKIP_PIXELS.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the number of pixel locations skipped before the
/// first pixel is read from memory. The initial value is 0. See glPixelStore.
///
/// @return Une chaîne représentant l'attribut GL_UNPACK_SKIP_PIXELS.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlUnpackSkipPixels() const
{
   return utilitaire::convertirEnChaine(glUnpackSkipPixels_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlUnpackSkipRows() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_UNPACK_SKIP_ROWS.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the number of rows of pixel locations skipped
/// before the first pixel is read from memory. The initial value is 0.
/// See glPixelStore.
///
/// @return Une chaîne représentant l'attribut GL_UNPACK_SKIP_ROWS.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlUnpackSkipRows() const
{
   return utilitaire::convertirEnChaine(glUnpackSkipRows_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlUnpackSwapBytes() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_UNPACK_SWAP_BYTES.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether the bytes of
/// two-byte and four-byte pixel indices and components are swapped after
/// being read from memory. The initial value is GL_FALSE. See glPixelStore.
///
/// @return Une chaîne représentant l'attribut GL_UNPACK_SWAP_BYTES.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlUnpackSwapBytes() const
{
   return obtenirChaineBooleen(glUnpackSwapBytes_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlVertexArray() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_VERTEX_ARRAY.
///
/// Selon le man page, cet attribut:
///
/// params returns a single boolean value indicating whether the vertex array
/// is enabled. The initial value is GL_FALSE. See glVertexPointer.
///
/// @return Une chaîne représentant l'attribut GL_VERTEX_ARRAY.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlVertexArray() const
{
   return obtenirChaineBooleen(glVertexArray_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlVertexArraySize() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_VERTEX_ARRAY_SIZE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the number of coordinates per vertex in the
/// vertex array. The initial value is 4. See glVertexPointer.
///
/// @return Une chaîne représentant l'attribut GL_VERTEX_ARRAY_SIZE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlVertexArraySize() const
{
   return utilitaire::convertirEnChaine(glVertexArraySize_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlVertexArrayStride() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_VERTEX_ARRAY_STRIDE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the byte offset between consecutive vertices in
/// the vertex array. The initial value is 0. See glVertexPointer.
///
/// @return Une chaîne représentant l'attribut GL_VERTEX_ARRAY_STRIDE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlVertexArrayStride() const
{
   return utilitaire::convertirEnChaine(glVertexArrayStride_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlVertexArrayType() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_VERTEX_ARRAY_TYPE.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the data type of each coordinate in the vertex
/// array. The initial value is GL_FLOAT. See glVertexPointer.
///
/// @return Une chaîne représentant l'attribut GL_VERTEX_ARRAY_TYPE.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlVertexArrayType() const
{
   COMPARER_VALEUR(glVertexArrayType_, GL_SHORT);
   COMPARER_VALEUR(glVertexArrayType_, GL_INT);
   COMPARER_VALEUR(glVertexArrayType_, GL_FLOAT);
   COMPARER_VALEUR(glVertexArrayType_, GL_DOUBLE);
   COMPARER_DEFAUT(glVertexArrayType_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlViewport() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_VIEWPORT.
///
/// Selon le man page, cet attribut:
///
/// params returns four values: the x and y window coordinates of the
/// viewport, followed by its width and height. Initially the x and y window
/// coordinates are both set to 0, and the width and height are set to the
/// width and height of the window into which the GL will do its rendering.
/// See glViewport.
///
/// @return Une chaîne représentant l'attribut GL_VIEWPORT.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlViewport() const
{
   return obtenirChaineVecteur(glViewport_, 4);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlZoomX() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_ZOOM_X.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the x pixel zoom factor. The initial value is 1.
/// See glPixelZoom.
///
/// @return Une chaîne représentant l'attribut GL_ZOOM_X.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlZoomX() const
{
   return utilitaire::convertirEnChaine(glZoomX_);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @fn std::string EtatOpenGL::obtenirChaineGlZoomY() const
///
/// Cette fonction retourne une chaîne représentant l'attribut GL_ZOOM_Y.
///
/// Selon le man page, cet attribut:
///
/// params returns one value, the $y$ pixel zoom factor. The initial value is
/// 1. See glPixelZoom.
///
/// @return Une chaîne représentant l'attribut GL_ZOOM_Y.
///
///////////////////////////////////////////////////////////////////////////////
std::string EtatOpenGL::obtenirChaineGlZoomY() const
{
   return utilitaire::convertirEnChaine(glZoomY_);
}


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
