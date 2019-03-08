#include "Header.h"

//变量定义
extern CATDocument* piDocument;
extern CATIContainer_var spSpecCont ;
extern CATIPrtContainer_var spPrtContainer ;
extern CATISpecObject_var spPart ;
extern CATIPrtPart_var spPrtPart ;
extern CATIGSMFactory_var spGSMFact ;
extern CATIDatumFactory_var spDatumFact;
extern CATICkeParmFactory_var spCkeParmFactory ; //获得CATICkeParmFactory
extern CATICkeFunctionFactory_var FunctionFactory ;
extern CATIParmDictionary_var spParmDictionary ;
extern CATICkeRelationFactory_var spRelationFactory;
extern CATIMechanicalRootFactory_var spMechanicalRootFactory ;
extern CATICkeMagnitude_var spMagnitudeDistance ;
extern CATFrmEditor* _pEditor ;


extern int GetMainContainer();
extern CATDocument* CreateNewPartFile();
extern CATUnicodeString GetPartNumber(CATDocument *pDoc);
extern CATIDescendants_var CreateNewGSD(CATIMechanicalRootFactory_var spMechanicalRootFactory,CATIPrtPart_var spPrtPart,CATUnicodeString USNameOfNewGSD, int iType);
//extern CATIDescendants_var CreateGSDUnderNewPartFile(CATDocument* ipDoc,CATUnicodeString USName);
extern CATBoolean GetSpecObjShowAttr(CATISpecObject_var iSpecObj);
extern CATBoolean CopyPasteOperation(CATLISTV(CATISpecObject_var) ispListElements,CATISpecObject_var ispDestinatioin);
extern CATBoolean CopyPasteOperationWithLink(CATIPrtContainer* iSourcePrtContainer,CATISpecObject_var &iParentGSO,CATListValCATBaseUnknown_var listExtract);
extern double GetDistanceFromPTPTOnCurve(CATISpecObject_var spSpecCurve,CATISpecObject_var spSpecPT1,CATISpecObject_var spSpecPT2);
extern CATIDescendants_var GetGSDByName(CATIPrtPart_var ispPrtPart,CATUnicodeString iUSGSDName);
extern CATLISTV(CATISpecObject_var) TopoAnalysis(CATISpecObject_var   ispSpecObject,int iDomain);

extern void  myGetTime2(char* str) ;

extern BOOL SetPartNumberofPartFile(CATDocument *pNewDoc,CATUnicodeString USName);

extern void HideElements(CATLISTV(CATISpecObject_var) ipListPointObj);

extern BOOL InsertPC(/*CATDocument* pCurrentDoc,*/CATIPrtPart_var ispPrtPart,CATIDescendants_var spNewDes,CATUnicodeString PCFilePathName,int iType);//iType=1则不修改参数值;

extern CATListValCATISpecObject_var  GetAllElementsInGSD(CATISpecObject_var ispSpecGSD,int iDimension);

extern double GetDistanceBetweenLineSurface(CATISpecObject_var ispSpecLine,CATISpecObject_var ispSpecSurface);

extern CATListValCATISpecObject_var TopoDisAssemble(CATISpecObject_var spGSMBoundary);

extern CATISpecObject_var SmoothBoundary(CATISpecObject_var ispSpecBoundary);

extern CATISpecObject_var ExtractSurface(CATISpecObject_var ispSpecSurface);

extern double DistanceBetweenLinePT(CATISpecObject_var ispSpecLine,CATMathPoint iMathPT);


extern double GetCurveLength(CATISpecObject_var spSpecCurve);

extern BOOL InsertPC2(CATLISTV(CATISpecObject_var) ispListInput,CATIPrtPart_var ispPrtPart,CATIDescendants_var spNewDes,CATUnicodeString PCFilePathName);

extern CATLISTV(CATBaseUnknown_var) GetAllGSDsInPart(CATIPrtPart_var ispPrtPart);

extern CATISpecObject_var SmoothBoundary2(CATISpecObject_var ispSpecBoundary,double dCurvature);

extern BOOL InsertPC3(CATLISTV(CATISpecObject_var) ispListInput,CATIPrtPart_var ispPrtPart,CATIDescendants_var spNewDes,CATUnicodeString PCFilePathName);

extern BOOL InsertPC4(CATIPrtPart_var ispPrtPart,CATIDescendants_var spNewDes,CATUnicodeString PCFilePathName,int iRR);//iType=1则不修改参数值;
extern BOOL InsertPCForHKBZW(CATISpecObject_var ispSpecObject,CATIPrtPart_var ispPrtPart,CATIDescendants_var spNewDes,CATUnicodeString PCFilePathName,int inumber);

extern void GetLineColor(CATISpecObject_var spSpec,unsigned int &iRed,unsigned int &iGreen,unsigned int &iBlack);
extern void GetSurfaceColor(CATISpecObject_var spSpec,unsigned int &iRed,unsigned int &iGreen,unsigned int &iBlack);

extern BOOL GetCertainFilesFromFolder(CATLISTV(CATUnicodeString) &ioLstFilePath,CATUnicodeString istrSuffix,CATUnicodeString iStrFolderPath);

extern double DistanceBetweenPlanePT(CATISpecObject_var ispSpecPlane,CATMathPoint iMathPT);
extern CATMathPoint ProjectPTOnPlane(CATISpecObject_var ispSpecPlane,CATMathPoint iPT);

extern CATUnicodeString getPathNameWithoutSuffix(const CATUnicodeString &fullpath);
extern CATUnicodeString getSuffix(CATDocument *pDoc);
extern void CV5_getCATIAVersionInfo(int &ver, int &release, int &service_pack, int &hot_fix);
extern int CompareTwoPTs(CATISpecObject_var ispSpecPT1,CATISpecObject_var ispSpecPT2);
extern double GetDistanceFromPTPT(CATISpecObject_var spSpecPT1,CATISpecObject_var spSpecPT2);

extern double GetMaxFromList(CATListOfDouble   spListWidth);
extern double GetMinFromList(CATListOfDouble   spListWidth);
extern CATIDescendants_var CreateNewGSDUnderGSD(CATIMechanicalRootFactory_var spMechanicalRootFactory,CATISpecObject_var spDestination,CATUnicodeString USNameOfNewGSD);

extern double GetAngleBetweenLineLine2222(CATISpecObject_var ispSpecLine1,CATISpecObject_var ispSpecLine2);

extern HRESULT NewDraftingDoc(CATUnicodeString storePath);
extern HRESULT OpenDrawingDoc(CATUnicodeString storePath, CATDocument*& pDrawDoc);

extern CATIMechanicalTool_var GetBodyByName(CATIPrtPart_var ispPrtPart,CATUnicodeString iUSBodyName);

extern BOOL InsertPCWithIndex(CATIPrtPart_var ispPrtPart,CATIDescendants_var spNewDes,CATUnicodeString PCFilePathName,int iIndex);

extern HRESULT GetSpecObjShowAttr(CATISpecObject_var iSpecObj,CATBoolean &iShowOrHide);

extern CATISpecObject_var GetElementByNameInGSD(CATIDescendants_var ispSpecGSD,CATUnicodeString iUSGSD);
extern CATMathPlane GetProjectionPlaneForIsometricView();
extern CATISpecObject_var SmoothBoundaryCrv(CATISpecObject_var ispSpecBoundary);

extern HRESULT SetSpecObjectColor(CATISpecObject_var iSpecObj,int iColorR,int iColorG,int iColorB,int iOpacity);

extern CATLISTV(CATBaseUnknown_var) GetAllBodysInPart(CATIPrtPart_var ispPrtPart);


extern CATISpecObject_var FeatulizeCell(CATCell_var ipCell,CATIPrtContainer_var ispPrtContainer,CATIGeometricalElement_var  spGeoBoundary);


extern double GetSurfaceArea(CATISpecObject_var iSpecObj);


extern CATCell*  GetSurfaceTopLineCell(CATISpecObject_var  ispSpecSurface);
extern CATCell*  GetSurfaceBottomLineCell(CATISpecObject_var  ispSpecSurface);

extern BOOL InsertPCWithIndex2(CATIPrtPart_var ispPrtPart,CATLISTV(CATISpecObject_var) ispListInput,CATUnicodeString PCFilePathName,int iIndex);

extern BOOL InsertPCWithIndexWithParam(CATIPrtPart_var ispPrtPart,CATIDescendants_var spNewDes,CATUnicodeString PCFilePathName,int iIndex,CATUnicodeString USParamName1);
extern BOOL InsertPCWithIndexWithParamUseIndenticalName(CATIPrtPart_var ispPrtPart,CATIDescendants_var spNewDes,CATUnicodeString PCFilePathName,int iIndex );


extern void GetSurfacewoLongLineCell(CATISpecObject_var  ispSpecSurface,CATCell* &pCell1,CATCell* &pCell2);


extern void GetSurfacewoLongLineSpec(CATISpecObject_var  ispSpecSurface,CATISpecObject_var &spLong1,CATISpecObject_var &spLong2);

extern CATLISTV(CATISpecObject_var)  CopyPasteOperationV2(CATLISTV(CATISpecObject_var) ispListElements,CATISpecObject_var ispDestinatioin);

extern double GetCurveMidPTCoord(CATISpecObject_var ispSpecLine,int iType);

extern void GetSurfacewoLongLineSpecForLize(CATISpecObject_var  ispSpecSurface,CATISpecObject_var &spLong1,CATISpecObject_var &spLong2);

extern double GetDistanceBetweenPTSurface(CATISpecObject_var ispSpecPT,CATISpecObject_var ispSpecSurface);

extern CATISpecObject_var FillTriangle(CATISpecObject_var ispSpecTriangle,CATIDescendants_var ispSpecGSD);

extern double GetDistanceFromMathPTPT(CATISpecObject_var spSpecPT1,CATMathPoint iPt2);

extern double GetDistanceBetweenMeasurableMathPoint(CATISpecObject_var ispSpecLine,CATMathPoint pPT);

extern void InsertPCAndCreateNewPartWithIndex(CATUnicodeString iusPathPC,CATUnicodeString iUSPartNumber,CATListValCATISpecObject_var  ispListForCopy,
															   CATUnicodeString USForPCOutputDirectory,CATUnicodeString USForPCOutputError,int iIndex);

extern CATMathPoint GetCOGOf(CATISpecObject_var ispSpec);
extern void DeleteEmptyGSDs(CATISpecObject_var spSpecFather);

extern void getFolderDayFolder(CString pathStr, CStringArray& arrStrFile);//找到所有的文件夹;
extern void getFolderDayFile(CString pathStr, CStringArray& arrStrFile);//找到所有的文件

extern CATUnicodeString CStringToUS(CString iCString);

extern CString USToCString(CATUnicodeString iUString);

extern void getFolderExcelFile(CString pathStr, CStringArray& arrStrFile);

extern void ShowElements(CATLISTV(CATISpecObject_var) ipListPointObj);

extern BOOL ChangePrdInstName(CATIProduct_var spPrd,CATUnicodeString StrInstName);

extern HRESULT AddNewExternalComponent(CATIProduct* iThisProd, const CATUnicodeString iDocumentType,const CATUnicodeString iPartNumber, CATIProduct** oNewProduct,CATDocument *&pNewDoc);

//extern CATLISTV(CATISpecObject_var)  CopyPasteOperationV2(CATLISTV(CATISpecObject_var) ispListElements,CATISpecObject_var ispDestinatioin);

extern BOOL InsertPCWithIndex3(CATIPrtPart_var ispPrtPart,CATIDescendants_var spNewDes,CATLISTV(CATISpecObject_var) ispListInput,CATUnicodeString PCFilePathName,int iIndex);

extern HRESULT GetViewBoundingBox(CATIView_var ispView,   double & dXLength,double& dYLength);

extern void CopyElementsAndCreateNewPart(CATLISTV(CATISpecObject_var) ispListForCopy,CATUnicodeString iUSPartNumber,CATUnicodeString USForPCOutputDirectory);

extern HRESULT SetCurveColor(CATISpecObject_var iSpecObj,int iColorR,int iColorG,int iColorB,int iOpacity);

extern CATIParmPublisher_var CreateParamSetUnderGSD(CATISpecObject_var ispFather,CATICkeRelationFactory_var ispCkeRelationFactory,CATUnicodeString iUSParamSetName);

extern CATICkeParm_var CreateLengthParam(CATUnicodeString iUSParamName,double idValue,CATIParmPublisher_var ispFatherParamSet);

extern CATISpecObject_var GetReferencePlane(CATIPrtPart_var ispPrt,int iXYZ);

extern CATICkeParm_var CreateParmWithRelation(CATUnicodeString iUSParamName,CATUnicodeString iUSRelationName,CATCkeListOf(Parm) ParamList1,CATUnicodeString USFormula,int iAddRelationToSet,CATIParmPublisher_var ispFatherParamSet);

extern void SetReferenceLineProperties(CATISpecObject_var iSpecObj,int iColorR,int iColorG,int iColorB,int iLineWidth,int iLineType);

extern void HideElement(CATISpecObject_var ispSpecObject);

CATICkeParm_var CreateAngleParam(CATUnicodeString iUSParamName,double idValue,CATIParmPublisher_var ispFatherParamSet);
CATICkeParm_var CreateAngleParmWithRelation(CATUnicodeString iUSParamName,CATUnicodeString iUSRelationName,CATCkeListOf(Parm) ParamList1,CATUnicodeString USFormula,int iAddRelationToSet,CATIParmPublisher_var ispFatherParamSet);

extern void ChangeNameAndAppendAndHideElement(CATUnicodeString iUSName,CATIDescendants_var ispDesFather,CATISpecObject_var ispSpecObject,int iAppend,int iHide);

extern int UpdateTest(CATISpecObject_var ispSpec);

extern CATIDescendants_var GetDesTA( );
extern CATIDescendants_var GetDesUSR( );
extern CATIDescendants_var GetDesSHO( );
extern CATIDescendants_var GetDesNSD_UG_ARC( );
extern CATIDescendants_var GetDesNSD_SHO();
extern CATIDescendants_var GetDesBR( );
extern CATIDescendants_var GetDesLSR( );
extern CATIDescendants_var GetDesBR_Split( );
extern CATIDescendants_var GetDesRefTreadAndBelt( );
extern CATIDescendants_var GetDesBead( );
extern CATIDescendants_var GetDesCenterCarcassLine( );

extern CATIParmPublisher_var  GetParamSetUnderGSD(CATIDescendants_var ispSpecGSD,CATUnicodeString iUSParamSetName);
extern CATICkeParm_var GetParamFromParamSetUnderGSD(CATIDescendants_var ispSpecGSD,CATUnicodeString iUSParamSetName,CATUnicodeString iUSParamName);

extern CATICkeParm_var CreateStringParam(CATUnicodeString iUSParamName,CATUnicodeString iUSValue,CATIParmPublisher_var ispFatherParamSet);
extern CATICkeParm_var CreateIntegerParam(CATUnicodeString iUSParamName,int iValue,CATIParmPublisher_var ispFatherParamSet);
extern CATICkeParm_var CreateIntParmWithRelationWithInputStringParams(CATUnicodeString iUSParamName,CATUnicodeString iUSRelationName,CATCkeListOf(Parm) ParamList1,CATUnicodeString USFormula,int iAddRelationToSet,CATIParmPublisher_var ispFatherParamSet);
extern CATIDescendants_var GetDesCarcass( );
extern CATIDescendants_var GetDesCarcassGS( );
extern CATIDescendants_var GetDesApex( );
extern CATIDescendants_var GetDesApex_GS( );
extern CATIDescendants_var GetDesCarcassTurnUp( );
extern CATIDescendants_var GetDesChipperA( );
extern CATIDescendants_var GetDesLinerInsert( );
extern CATIDescendants_var GetDesLiner( );
extern CATIDescendants_var GetDesBelt( );
extern CATIDescendants_var GetDesOuterProfile( );
extern CATIDescendants_var GetDesRimCushion( );
extern CATIDescendants_var GetDesWedge( );
extern CATIDescendants_var GetDesSideWall( );
extern CATIDescendants_var GetDesCapPly( );
extern CATIDescendants_var GetDesTread( );  
extern CATIDescendants_var GetDesButtress( );
extern CATIDescendants_var GetDesChipperB( );
extern CATIDescendants_var GetDesReference( );
CATISpecObject_var GetSW_Line();
CATISpecObject_var GetSW_PT();

extern CATICkeParm_var CreateRatioParam(CATUnicodeString iUSParamName,double dValue,CATIParmPublisher_var ispFatherParamSet);

extern CATICkeParm_var CreateParmWithRelation(CATUnicodeString iUSParamName,CATUnicodeString iUSRelationName,CATCkeListOf(Parm) ParamList1,CATUnicodeString USFormula,int iAddRelationToSet,CATIParmPublisher_var ispFatherParamSet);

CATISpecObject_var CreateParaLine(CATISpecObject_var ispSpecSupport,
								  CATISpecObject_var ispSpecForParallel,
								  CATICkeParm_var ispCkeParam1,
								  CATICkeParm_var ispCkeParam2,
								  CATISpecObject_var ispSpecRef,
								  int iMax);
