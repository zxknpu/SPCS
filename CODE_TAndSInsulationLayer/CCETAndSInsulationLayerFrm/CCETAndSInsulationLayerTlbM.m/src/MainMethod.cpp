#include "Header.h"

CATDocument* piDocument = NULL;
CATIContainer_var spSpecCont = NULL_var;
CATIPrtContainer_var spPrtContainer = NULL_var;
CATISpecObject_var spPart = NULL_var;
CATIPrtPart_var spPrtPart = NULL_var;
CATIGSMFactory_var spGSMFact = NULL_var;
CATIDatumFactory_var spDatumFact=NULL_var;
CATICkeParmFactory_var spCkeParmFactory = NULL_var; //获得CATICkeParmFactory
CATICkeRelationFactory_var spRelationFactory = NULL_var;
CATICkeFunctionFactory_var FunctionFactory = NULL_var;
CATIParmDictionary_var spParmDictionary = NULL_var;
CATIMechanicalRootFactory_var spMechanicalRootFactory = NULL_var;
CATICkeMagnitude_var spMagnitudeDistance = NULL_var;
CATFrmEditor* _pEditor = NULL;

int GetMainContainer()//得到主要的Container
{
	CATSession* pSession = NULL;  
	HRESULT hr = ::GetPtrSession(pSession);	
	CATFrmEditor* pEditor = NULL;
	pEditor=CATFrmEditor::GetCurrentEditor(); 

	_pEditor = CATFrmEditor::GetCurrentEditor(); 
	if(pSession == NULL || pEditor == NULL)
	{
		cout<<"Error in pSession And pEditor! "<<endl;
		return 0;
	}
	piDocument = NULL;
	piDocument=pEditor->GetDocument();
	if(piDocument == NULL)
	{
		cout<<"Error in piDocument!"<<endl;
		::MessageBox(NULL,L"不能读取当前Part文件",L"错误",MB_OK);
		return 0;
	}

	CATPathElement path = pEditor->GetUIActiveObject();                             
	CATISpecObject* piSpecOnActive=NULL;
	path.Search (IID_CATISpecObject, (void **) &piSpecOnActive);
	if(piSpecOnActive == NULL)
	{
		cout<<"Error in piSpecOnActive!"<<endl;
		return 0;
	}

	spSpecCont = NULL_var;
	spSpecCont = piSpecOnActive->GetFeatContainer();   
	if(spSpecCont == NULL_var)
	{
		cout<<"Error in spSpecCont!"<<endl;
		return 0;
	}
	piSpecOnActive->Release();
	piSpecOnActive=NULL;

	spPrtContainer = NULL_var;
	spPrtContainer = spSpecCont;
	if(spPrtContainer == NULL_var)
	{
		cout<<"Error in spPrtContainer!"<<endl;
		return 0;
	}

	spPart = NULL_var;
	spPart = spPrtContainer->GetPart();
	if(spPart == NULL_var)
	{
		cout<<"Error in spPart!"<<endl;
		return 0;
	}

	spPrtPart = NULL_var;
	spPrtPart = spPart;
	if(spPrtPart == NULL_var)
	{
		cout<<"Error in spPrtPart!"<<endl;
		return 0;
	}

	spGSMFact = NULL_var;
	spGSMFact = spSpecCont;

	spDatumFact = NULL_var;
	spDatumFact = spSpecCont;

	spCkeParmFactory = NULL_var;
	spCkeParmFactory = spSpecCont;

	FunctionFactory = NULL_var;
	FunctionFactory= CATCkeGlobalFunctions::GetFunctionFactory();
	FunctionFactory->Methods();
	spParmDictionary = NULL_var;
	spParmDictionary=CATCkeGlobalFunctions::GetParmDictionary();
	spMechanicalRootFactory = NULL_var;
	spMechanicalRootFactory=spSpecCont;
	spRelationFactory = NULL_var;
	spRelationFactory = spSpecCont;
	if(!spGSMFact || !spDatumFact || !spCkeParmFactory || !FunctionFactory || !spParmDictionary || !spMechanicalRootFactory|| !spRelationFactory)
	{
		cout<<"Error in GetFactoryElement!"<<endl;
		return 0;
	}
	spMagnitudeDistance = NULL_var;
	spMagnitudeDistance = spParmDictionary->FindMagnitude("Length");
	if(!spMagnitudeDistance)
	{
		cout<<"Error in Magnitude!"<<endl;
		return 0;
	}

	return 1;
}

//新建Part文件
CATDocument* CreateNewPartFile()
{
	/*CATSession* pSession = NULL;  
	HRESULT hr = ::GetPtrSession(pSession);	*/
	CATDocument* pDoc = NULL;
	HRESULT rc = CATDocumentServices::New("Part",pDoc);

	return pDoc;
}

//得到Part文件的文件名
CATUnicodeString GetPartNumber(CATDocument *pDoc)
{
	CATIAlias_var spAlias = pDoc;
	if(spAlias == NULL)
		return "";
	CATUnicodeString USPartNumber = spAlias->GetAlias();
	return USPartNumber;
}

CATIDescendants_var CreateNewGSD(CATIMechanicalRootFactory_var spMechanicalRootFactory,CATIPrtPart_var spPrtPart,CATUnicodeString USNameOfNewGSD , int iType)//iType = 0为几何体，1为集合图形集
{
	if(spMechanicalRootFactory == NULL_var)
	{
		cout<<"Error in spMechanicalRootFactory!"<<endl;
		return NULL_var;
	}
	if(spPrtPart == NULL_var)
	{
		cout<<"Error in spPrtPart!"<<endl;
		return NULL_var;
	}

	CATISpecObject_var ospCAATempGSD = NULL_var;
	CATUnicodeString NameOfNewGSD(USNameOfNewGSD);
	if(iType == 1)
	{
		spMechanicalRootFactory->CreateGeometricalSet(NameOfNewGSD,spPrtPart,ospCAATempGSD,  -1);
	}
	else
	{
		ospCAATempGSD = spMechanicalRootFactory->CreatePRTTool(NameOfNewGSD,spPrtPart,0);
	}
	if(ospCAATempGSD == NULL_var)
	{
		cout<<"Error in Create New GSD"<<endl;
		return NULL_var;
	}
	
	spPrtPart->SetCurrentFeature(ospCAATempGSD);
	CATIBasicTool_var spBasicTool1=spPrtPart-> GetCurrentTool( ); 
	CATIDescendants_var spCAATempDescendant= NULL_var;
	spCAATempDescendant = spBasicTool1;
	if(spCAATempDescendant == NULL_var)
	{
		cout<<"Error in spCAATempDescendant!"<<endl;
		return NULL_var;
	}
	return spCAATempDescendant;
}

//获取特征为隐藏或显示
CATBoolean GetSpecObjShowAttr(CATISpecObject_var iSpecObj)
{
	if (NULL_var==iSpecObj) return FALSE;

	CATIVisProperties *piVisP=NULL;
	HRESULT rc = iSpecObj->QueryInterface(IID_CATIVisProperties,(void**)&piVisP);
	if( FAILED(rc) || piVisP == NULL ) return FALSE;

	CATVisPropertiesValues Attribut;
	piVisP->GetPropertiesAtt(Attribut,CATVPShow);
	piVisP->Release();
	piVisP=NULL;

	CATShowAttribut oShow;
	Attribut.GetShowAttr(oShow);
	if (oShow == CATShowAttr)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

CATLISTV(CATISpecObject_var)  CopyPasteOperationV2(CATLISTV(CATISpecObject_var) ispListElements,CATISpecObject_var ispDestinatioin)
{
	CATLISTV(CATISpecObject_var)  spListForReturn;
	spListForReturn.RemoveAll();
	CATMmrInterPartCopy *  ptCATMmrInterPartCopy = NULL;
	for(int i=1;i<=ispListElements.Size();i++)
	{
		CATISpecObject_var SourceToCopy = ispListElements[i] ;
		if(SourceToCopy == NULL_var)
		{
			cout<<"Error in SourceToCopy"<<endl;
			continue;
		}
		ptCATMmrInterPartCopy =  new CATMmrInterPartCopy (SourceToCopy,ispDestinatioin);
		if(ptCATMmrInterPartCopy == NULL)
		{
			cout<<"Error in ptCATMmrInterPartCopy"<<endl;
			continue;
		}
		ptCATMmrInterPartCopy ->SetAttributeMode(3); 

		ptCATMmrInterPartCopy ->SetLinkMode(FALSE) ;   
		HRESULT hr = ptCATMmrInterPartCopy ->Run();

		CATISpecObject_var Result ;
		hr = ptCATMmrInterPartCopy ->GetResult(Result);
		if(Result == NULL_var)
		{
			cout<<"Error in Result"<<endl;
			continue;
		}
		else
		{
			Result->Update();
			CATIAlias_var spAliasAfterPaste = Result;
			spAliasAfterPaste->SetAlias(CATIAlias_var(SourceToCopy)->GetAlias());
		}
		spListForReturn.Append(Result);

	}
	ptCATMmrInterPartCopy = NULL;


	return spListForReturn;
}



CATBoolean CopyPasteOperationWithLink(CATIPrtContainer* iSourcePrtContainer,CATISpecObject_var &iParentGSO,CATListValCATBaseUnknown_var listExtract)
{
	cout<<"CopyPasteOperationWithLink"<<endl;
		const CATFormat* pFormat=::SpecBindNativeFormat("CATPrtResult");//设置特殊粘贴的形式

		//copy model
		CATListValCATBaseUnknown_var listToCopy;
		CATListValCATBaseUnknown_var listAlreadyInBoundary;

		CATICutAndPastable_var spSourceCP = iSourcePrtContainer;

		spSourceCP->BoundaryExtract(listAlreadyInBoundary,&listExtract,pFormat);

		CATBaseUnknown_var spEltCopy = spSourceCP->Extract(listAlreadyInBoundary,pFormat);
		CATICutAndPastable_var spCopyCP(spEltCopy);
		CATListValCATBaseUnknown_var listCopyAlreadyInBoundary;
		spCopyCP->BoundaryExtract(listCopyAlreadyInBoundary,NULL,pFormat);
		listToCopy.Append(listCopyAlreadyInBoundary);
		cout<<"2"<<endl;

		//get target GS list
		CATListValCATBaseUnknown_var * listTarget = new CATListValCATBaseUnknown_var;
		CATPathElement pathTarget(iParentGSO); 
		pathTarget.AddFatherElement(iParentGSO);
		listTarget->Append(&pathTarget);
		cout<<"3"<<endl;

		//paste model
		CATIContainer_var spTargetContainer = iParentGSO->GetFeatContainer();
		CATICutAndPastable_var spTargetCP=spTargetContainer;
		spTargetCP->Paste(listToCopy,listTarget,pFormat);
		cout<<"4"<<endl;


		return TRUE;
}




double GetDistanceFromPTPTOnCurve(CATISpecObject_var spSpecCurve,CATISpecObject_var spSpecPT1,CATISpecObject_var spSpecPT2)
{
	//
	CATUnicodeString  iComment; 
	CATUnicodeString  iFamily; ;
	CATUnicodeString  iBodyDistance("length(a1,a2,a3)");
	CATCkeListOf(Parm) ParamListForDistance;
	ParamListForDistance.RemoveAll();

	if(!spSpecCurve || !spSpecPT1 || !spSpecPT2)
	{
		cout<<"Error in Inputs Of DistanceMeasure!"<<endl;
		return 0.;
	}
	spSpecCurve->Update();
	spSpecPT1->Update();
	spSpecPT2->Update();
	CATICkeParm_var spParamElement1ForMeasure = spCkeParmFactory->CreateObjectReference(spSpecCurve);
	ParamListForDistance.Append(spParamElement1ForMeasure);
	CATICkeParm_var spParamElement2ForMeasure = spCkeParmFactory->CreateObjectReference(spSpecPT1);
	ParamListForDistance.Append(spParamElement2ForMeasure);
	CATICkeParm_var spParamElement3ForMeasure = spCkeParmFactory->CreateObjectReference(spSpecPT2);
	ParamListForDistance.Append(spParamElement3ForMeasure);

	CATICkeParm_var spCkeParmDistance = spCkeParmFactory->CreateDimension(spMagnitudeDistance, "Distance", 10.2);
	CATICkeRelation_var spCkeRelation = NULL_var;
	spCkeRelation=spCkeParmFactory->CreateFormula("Distance",iComment,iFamily,spCkeParmDistance,&ParamListForDistance,iBodyDistance,NULL_var, CATCke::False );
	ParamListForDistance.RemoveAll();

	double oDoubleDistance = spCkeParmDistance->Value()->AsReal();

	return oDoubleDistance*1000.;
}


CATIDescendants_var GetGSDByName(CATIPrtPart_var ispPrtPart,CATUnicodeString iUSGSDName)//通过名字得到GSD，返回该GSD的SpecObject类型
{
	CATLISTV(CATBaseUnknown_var) surfBodies; 
	surfBodies.RemoveAll();

	CATIPartRequest *pPartAsRequest = 0;
	HRESULT rc = ispPrtPart->QueryInterface(IID_CATIPartRequest, (void**)&pPartAsRequest) ;

	rc = pPartAsRequest->GetSurfBodies ("MfDefault3DView",surfBodies);
	CATISpecObject_var ospSpecOnGSD = NULL_var;
	int sizeofGSD = surfBodies.Size();
	if(sizeofGSD<1)
	{
		cout<<"Error in Get GSD Of Current Part!"<<endl;
		return NULL_var;
	}
	for(int i = 1;i<=sizeofGSD;i++)
	{
		CATISpecObject_var spSpecOnGSD = surfBodies[i] ;
		CATIAlias_var AliasofGSDName=spSpecOnGSD;	
		CATUnicodeString NameOfGSD=AliasofGSDName->GetAlias();
		if(NameOfGSD ==iUSGSDName)
		{
			ospSpecOnGSD=spSpecOnGSD;	
			break;
		}
	}	
	/*if(ospSpecOnGSD == NULL_var)
	{
		CATUnicodeString USInfor = "不存在名为"+iUSGSDName+"的GSD!";
		::MessageBox(NULL,USInfor,L"GSD 错误",MB_OK);
		return NULL_var;
	}*/
	surfBodies.RemoveAll();


	CATIDescendants_var spForReturn = ospSpecOnGSD;

	return spForReturn;

}

CATLISTV(CATBaseUnknown_var) GetAllGSDsInPart(CATIPrtPart_var ispPrtPart)
{
	CATLISTV(CATBaseUnknown_var) surfBodies; 
	surfBodies.RemoveAll();

	CATIPartRequest *pPartAsRequest = 0;
	HRESULT rc = ispPrtPart->QueryInterface(IID_CATIPartRequest, (void**)&pPartAsRequest) ;

	rc = pPartAsRequest->GetSurfBodies ("MfDefault3DView",surfBodies);

	return surfBodies;

}

CATLISTV(CATBaseUnknown_var) GetAllBodysInPart(CATIPrtPart_var ispPrtPart)
{
	CATLISTV(CATBaseUnknown_var) surfBodies; 
	surfBodies.RemoveAll();

	CATIPartRequest *pPartAsRequest = 0;
	HRESULT rc = ispPrtPart->QueryInterface(IID_CATIPartRequest, (void**)&pPartAsRequest) ;

	rc = pPartAsRequest->GetSolidBodies("MfDefault3DView",surfBodies);

	return surfBodies;

}

CATLISTV(CATISpecObject_var) TopoAnalysis(CATISpecObject_var   ispSpecObject,int iDomain)
{
		ispSpecObject->Update();

		CATIGeometricalElement_var  spGeoBoundary = ispSpecObject;
		CATBody_var spBodyBoundary = spGeoBoundary->GetBodyResult();	

		CATLISTP(CATDomain) ListDomainOfBoundary;
		spBodyBoundary->GetAllDomains(iDomain,3,ListDomainOfBoundary);
		int  NumberOfDomain = ListDomainOfBoundary.Size(); 
		//cout<<"NumberOfDomain  "<<NumberOfDomain<<endl;

		CATListValCATISpecObject_var spListOfCells; 
		spListOfCells.RemoveAll();

		/*if(NumberOfDomain == 1)
		{
			spListOfCells.Append(ispSpecObject);
			return spListOfCells;
		}*/
		for(int i=1;i<=NumberOfDomain;i++)
		{
			CATDomain_var spDomainBoundary = ListDomainOfBoundary[i];
			
			long NbCell = spDomainBoundary->GetNbCellUses();
			
			for(int j=1;j<=NbCell;j++)
			{
				CATCell * pCell=spDomainBoundary->GetCell(j);	
				
				CATCell_var spCellBoundary = pCell;
				CATIBRepAccess_var spBRepAccess= CATBRepDecode(spCellBoundary,spGeoBoundary);			

				CATIFeaturize_var spFratureCell = spBRepAccess;			 
				CATISpecObject_var spObjectFromCell = spFratureCell->FeaturizeR(MfPermanentBody);
			 
				CATIMfBRep_var spBRepOnFeature = spObjectFromCell; 
				CATBody_var spBodyFromObject=spBRepOnFeature->GetBody();
			

				CATIDatumFactory_var spDatumFactry = spPrtContainer;

				CATISpecObject *piObjectFromBody=NULL;
				spDatumFactry->InstanciateDatum(spBodyFromObject,piObjectFromBody);
				CATISpecObject_var spObjectFromBody=piObjectFromBody;
				spObjectFromBody->Update();
			
				spListOfCells.Append(spObjectFromBody);
			}	
		}

		return spListOfCells;
}

void  myGetTime2(char* str) 
{
	struct tm *loc ;
	time_t    t      ;

	t   = time(NULL) ;
	loc = localtime(&t) ;


	sprintf(str, "%4d%02d%02d%02d%02d%02d" , \
		loc->tm_year + 1900       ,        \
		loc->tm_mon  + 1          ,        \
		loc->tm_mday              ,        \
		loc->tm_hour              ,        \
		loc->tm_min               ,        \
		loc->tm_sec                         ) ;

}

BOOL SetPartNumberofPartFile(CATDocument *pNewDoc,CATUnicodeString USName)
{
	CATIDocRoots *piDocRootsOnNewDoc = NULL;
	HRESULT rc = pNewDoc->QueryInterface(IID_CATIDocRoots,(void**) &piDocRootsOnNewDoc);
	if ( SUCCEEDED( rc ) )
	{			
		CATListValCATBaseUnknown_var *pRootProducts =piDocRootsOnNewDoc->GiveDocRoots(); 
		CATIProduct_var spRootProduct;
		if (pRootProducts->Size())
		{  
			spRootProduct = (*pRootProducts)[1];
			delete pRootProducts;
			pRootProducts = NULL;
		}
		if(spRootProduct != NULL_var)
		{
			spRootProduct->SetPartNumber(USName);
		}
	}
	piDocRootsOnNewDoc->Release();
	piDocRootsOnNewDoc=NULL;

	return TRUE;
}

void HideElements(CATLISTV(CATISpecObject_var) ipListPointObj)
{
	for(int i = 1;i<=ipListPointObj.Size();i++)
	{
		CATIVisProperties * pPropOnPoint = NULL ;
		HRESULT rc = ipListPointObj[i]->QueryInterface(IID_CATIVisProperties, (void**)&pPropOnPoint);
		if(NULL!=pPropOnPoint) 
		{
			CATVisPropertiesValues PropValue;
			CATVisPropertyType PropTypeOnOnPtObj = CATVPShow ;
			CATVisGeomType GeomTypeOnPtObj = CATVPGlobalType ;
			PropValue.SetShowAttr(CATNoShowAttr);
			rc = pPropOnPoint->SetPropertiesAtt(PropValue, PropTypeOnOnPtObj, GeomTypeOnPtObj);												
			pPropOnPoint->Release();
			pPropOnPoint=NULL;	

			CATIModelEvents *piME = NULL;
			rc = ipListPointObj[i]->QueryInterface( IID_CATIModelEvents,(void **) &piME);
			if( FAILED(rc) || piME == NULL ) return ;
			CATModifyVisProperties notif(ipListPointObj[i], CATPathElement(ipListPointObj[i]), CATVPGlobalType,CATVPShow,PropValue);
			piME->Dispatch(notif);
			piME->Release();
			piME= NULL ;
		}
		

	}

}



void ShowElements(CATLISTV(CATISpecObject_var) ipListPointObj)
{
	for(int i = 1;i<=ipListPointObj.Size();i++)
	{
		CATIVisProperties * pPropOnPoint = NULL ;
		HRESULT rc = ipListPointObj[i]->QueryInterface(IID_CATIVisProperties, (void**)&pPropOnPoint);
		if(NULL!=pPropOnPoint) 
		{
			CATVisPropertiesValues PropValue;
			CATVisPropertyType PropTypeOnOnPtObj = CATVPShow ;
			CATVisGeomType GeomTypeOnPtObj = CATVPGlobalType ;
			PropValue.SetShowAttr(CATShowAttr);
			rc = pPropOnPoint->SetPropertiesAtt(PropValue, PropTypeOnOnPtObj, GeomTypeOnPtObj);												
			pPropOnPoint->Release();
			pPropOnPoint=NULL;	

			CATIModelEvents *piME = NULL;
			rc = ipListPointObj[i]->QueryInterface( IID_CATIModelEvents,(void **) &piME);
			if( FAILED(rc) || piME == NULL ) return ;
			CATModifyVisProperties notif(ipListPointObj[i], CATPathElement(ipListPointObj[i]), CATVPGlobalType,CATVPShow,PropValue);
			piME->Dispatch(notif);
			piME->Release();
			piME= NULL ;
		}


	}

}



BOOL InsertPC(CATIPrtPart_var ispPrtPart,CATIDescendants_var spNewDes,CATUnicodeString PCFilePathName,int iType)//iType=1则不修改参数值
{
	//得到当前文件下的INPUT GSD
	CATLISTV(CATISpecObject_var) ispListInput;
	ispListInput.RemoveAll();
	spNewDes->GetDirectChildren("CATISpecObject",ispListInput);

	//第这个文件下插入powerCopy
	CATISpecObject_var spSpecPrtPartNew = ispPrtPart;   
	CATBaseUnknown_var spBaseUnknownPrtPartNew = spSpecPrtPartNew ;
	CATBaseUnknown* pBaseUnknownPrtPartNew= (CATBaseUnknown*)spBaseUnknownPrtPartNew;
	CATPathElement PathNewPart(pBaseUnknownPrtPartNew); 
	CATBaseUnknown_var spBaseUnknownPrtPartNew1 = pBaseUnknownPrtPartNew;
	if(spBaseUnknownPrtPartNew1 == NULL_var)
	{
		cout<<"Error in spBaseUnknownPrtPartNew1"<<endl;
		return FALSE;
	}
	pBaseUnknownPrtPartNew->Release();
	pBaseUnknownPrtPartNew = NULL;


	CATTry
	{
		CATDocument*  oDocContainPC = NULL;
		HRESULT hr = CATDocumentServices::OpenDocument(PCFilePathName,oDocContainPC); 
		if(oDocContainPC == NULL)
		{
			cout<<"Error in oDocContainPC!"<<endl;
			//::MessageBox(NULL,L"请确定选择PC文件的路径是否正确",L"Error",MB_OK);
			cout<<"PC Path Error,Error in Insert PC"<<endl;
			return FALSE;
		}
		//锁定文件
		::CATLockDocument(*oDocContainPC);

		CATInit *pInitOnDoc2G = NULL ;
		hr = oDocContainPC->QueryInterface(IID_CATInit,(void **) &pInitOnDoc2G);
		if ( FAILED(hr) )return 1 ;
		CATIPrtContainer *pIPrtContOnUDFDoc2G= NULL ;
		pIPrtContOnUDFDoc2G= (CATIPrtContainer*)pInitOnDoc2G->GetRootContainer("CATIPrtContainer");
		if ( NULL == pIPrtContOnUDFDoc2G) return 1 ; 

		pInitOnDoc2G->Release();
		pInitOnDoc2G = NULL ;  
		CATIPrtPart_var spPrtPartOnUDFPart2G = pIPrtContOnUDFDoc2G->GetPart();

		CATIUdfFactory *pIUdfFactory = NULL ;
		hr = pIPrtContOnUDFDoc2G->QueryInterface(IID_CATIUdfFactory,(void **) &pIUdfFactory);
		pIPrtContOnUDFDoc2G->Release();
		pIPrtContOnUDFDoc2G = NULL;

		CATListValCATISpecObject_var* pListPowerCopy = NULL;
		pListPowerCopy = pIUdfFactory->GetPowerCopyList( );
		int SizeOfPowerCopy = pListPowerCopy->Size();
		if(SizeOfPowerCopy == 0)
		{
			cout<<"No PC Exists in PC File,Error in Insert PC"<<endl;
			//::MessageBox(NULL,L"当前的Part文件中木有PC，请检查文件",L"Error",MB_OK);
			return FALSE;
		}
		CATIUdfInstantiate * pIUdfInstantiate = NULL ;
		if( (NULL != pListPowerCopy) /*&& (SizeOfPowerCopy ==1 )*/ &&(NULL_var != (*pListPowerCopy)[1]) )
		{
			//cout<<"Suucess"<<endl;
			hr = (*pListPowerCopy)[1]->QueryInterface(IID_CATIUdfInstantiate,(void **)&pIUdfInstantiate);
			if ( SUCCEEDED(hr) )
			{
				delete pListPowerCopy ;
				pListPowerCopy = NULL ;
			}
		}
		pIUdfFactory->Release();
		pIUdfFactory = NULL ;

		CATPathElement * FirstUIactiveObject1 = NULL ;
		CATBaseUnknown_var FirstDest1 = NULL_var ; 
		if(PathNewPart == NULL)
		{
			cout<<"Error in PathNewPart"<<endl;
			return FALSE;
		}

		hr = pIUdfInstantiate->SetDestinationPath(&PathNewPart,FirstUIactiveObject1,FirstDest1);  
		if(FAILED(hr))
		{
			cout<<"Error in SetDestinationPath!"<<endl;
			return FALSE;
		}
		if ( NULL != FirstUIactiveObject1 ) 
		{
			FirstUIactiveObject1->Release();
			FirstUIactiveObject1 = NULL ;
		}

		////找同名的，然后赋值
		//for(int i = 1;i<=ispListInput.Size();i++)
		//{ 
		//	CATBaseUnknown_var spBaseOldInput = pIUdfInstantiate->GetOldInput(i);
		//	CATIAlias_var spAlias = spBaseOldInput;

		//	CATUnicodeString  USOldName = spAlias->GetAlias();
		//	for(int j = 1;j<=ispListInput.Size();j++)
		//	{
		//		CATIAlias_var spAlias2 = ispListInput[j];
		//		CATUnicodeString  USNewName = spAlias2->GetAlias();
		//		if(USNewName.SearchSubString(USOldName) != -1)
		//		{
		//			CATBaseUnknown *pInputLine1 = NULL;
		//			hr = ispListInput[j]->QueryInterface(IID_CATBaseUnknown,(void**)&pInputLine1);
		//			CATPathElement * pInput1 = new CATPathElement(pInputLine1);
		//			pInputLine1->Release();
		//			pInputLine1 = NULL;

		//			hr = pIUdfInstantiate->SetNewInput(i, pInput1);
		//			pInput1->Release();
		//			pInput1 = NULL;

		//			break;
		//		}
		//	}
		//}

		////直接赋值
		//CATBaseUnknown_var spBaseOldInput = pIUdfInstantiate->GetOldInput(1);
		//CATBaseUnknown *pInputLine1 = NULL;
		//hr = ispListInput[1]->QueryInterface(IID_CATBaseUnknown,(void**)&pInputLine1);
		//CATPathElement * pInput1 = new CATPathElement(pInputLine1);
		//pInputLine1->Release();
		//pInputLine1 = NULL;

		//hr = pIUdfInstantiate->SetNewInput(1, pInput1);
		//pInput1->Release();
		//pInput1 = NULL;

		//直接赋值
		for(int i = 1;i<=ispListInput.Size();i++)
		{ 
			CATBaseUnknown_var spBaseOldInput = pIUdfInstantiate->GetOldInput(i);
			CATBaseUnknown *pInputLine1 = NULL;
			hr = ispListInput[i]->QueryInterface(IID_CATBaseUnknown,(void**)&pInputLine1);
			CATPathElement * pInput1 = new CATPathElement(pInputLine1);
			pInputLine1->Release();
			pInputLine1 = NULL;

			hr = pIUdfInstantiate->SetNewInput(i, pInput1);
			pInput1->Release();
			pInput1 = NULL;
		}

		////修改参数
		//CATListValCATBaseUnknown_var * pParameterList = NULL;
		//CATListOfCATUnicodeString * pParameterRoleList = NULL ;
		//hr = pInstantiate->GetParameters(pParameterList,pParameterRoleList);
		//if ( NULL != pParameterList )
		//{
		//	int NbPublishedParameters = pParameterList->Size();
		//	for ( int i= 1 ; i <= NbPublishedParameters ; i++ )
		//	{
		//		CATICkeParm_var spCkeParmOnParameter = (*pParameterList)[i];
		//		CATIAlias_var spAliasCKE = spCkeParmOnParameter;

		//		if(spAliasCKE->GetAlias() == "JD_N")
		//		{
		//			spCkeParmOnParameter->Valuate(USForPCParam);
		//		}
		//	}
		//}

		//cout<<"    Input Finished and begin to Instantiate"<<endl;
		hr = pIUdfInstantiate->Instantiate(spBaseUnknownPrtPartNew1);
		//cout<<"    Instantiate end"<<endl;
		if(FAILED(hr))
		{
			cout<<"Error in Instantiate!"<<endl;
		}
cout<<"7"<<endl;
		if (pIUdfInstantiate != NULL)
		{
			hr = pIUdfInstantiate->EndInstantiate();	
			cout<<"71"<<endl;
			pIUdfInstantiate->Reset();
		}
	
cout<<"8"<<endl;
		::CATUnLockDocument(*oDocContainPC);
		CATDocumentServices::Remove(*oDocContainPC);

		if (pIUdfInstantiate != NULL)
		{
			pIUdfInstantiate->Release(); 
			pIUdfInstantiate=NULL;
		}

		/*if (oDocContainPC != NULL)
		{
			oDocContainPC = NULL;
		}*/
		

		//spSpecPrtPartNew->Update();
	}
	CATCatch(CATMfErrUpdate,pErrorUpdate)
	{
		cout<<"Can  not update!"<<endl;
		return FALSE;

	}
	CATCatch(CATError,pError)
	{
		cout<<"Can  not update!"<<endl;
		return FALSE;
	}
	CATCatchOthers
	{
		cout<<"Can  not update!"<<endl;
		return FALSE;
	}
	CATEndTry;


//cout<<"9"<<endl;



	return TRUE;

}
BOOL InsertPC4(CATIPrtPart_var ispPrtPart,CATIDescendants_var spNewDes,CATUnicodeString PCFilePathName,int iRR)//iType=1则不修改参数值
{
	//得到当前文件下的INPUT GSD
	CATLISTV(CATISpecObject_var) ispListInput;
	ispListInput.RemoveAll();
	spNewDes->GetDirectChildren("CATISpecObject",ispListInput);

	//第这个文件下插入powerCopy
	CATISpecObject_var spSpecPrtPartNew = ispPrtPart;   
	CATBaseUnknown_var spBaseUnknownPrtPartNew = spSpecPrtPartNew ;
	CATBaseUnknown* pBaseUnknownPrtPartNew= (CATBaseUnknown*)spBaseUnknownPrtPartNew;
	CATPathElement PathNewPart(pBaseUnknownPrtPartNew); 
	CATBaseUnknown_var spBaseUnknownPrtPartNew1=pBaseUnknownPrtPartNew;
	pBaseUnknownPrtPartNew->Release();
	pBaseUnknownPrtPartNew = NULL;


	CATTry
	{
		CATDocument*  oDocContainPC = NULL;
		HRESULT hr = CATDocumentServices::OpenDocument(PCFilePathName,oDocContainPC); 
		if(oDocContainPC == NULL)
		{
			cout<<"Error in oDocContainPC!"<<endl;
			//::MessageBox(NULL,L"请确定选择PC文件的路径是否正确",L"Error",MB_OK);
			cout<<"PC Path Error,Error in Insert PC"<<endl;
			return FALSE;
		}
		//锁定文件
		::CATLockDocument(*oDocContainPC);


		CATInit *pInitOnDoc2G = NULL ;
		hr = oDocContainPC->QueryInterface(IID_CATInit,(void **) &pInitOnDoc2G);
		if ( FAILED(hr) )return 1 ;
		CATIPrtContainer *pIPrtContOnUDFDoc2G= NULL ;
		pIPrtContOnUDFDoc2G= (CATIPrtContainer*)pInitOnDoc2G->GetRootContainer("CATIPrtContainer");
		if ( NULL == pIPrtContOnUDFDoc2G) return 1 ; 

		pInitOnDoc2G->Release();
		pInitOnDoc2G = NULL ;  

		CATIPrtPart_var spPrtPartOnUDFPart2G = pIPrtContOnUDFDoc2G->GetPart();

		CATIUdfFactory *pIUdfFactory = NULL ;
		hr = pIPrtContOnUDFDoc2G->QueryInterface(IID_CATIUdfFactory,(void **) &pIUdfFactory);
		pIPrtContOnUDFDoc2G->Release();
		pIPrtContOnUDFDoc2G = NULL;

		CATListValCATISpecObject_var* pListPowerCopy = NULL;
		pListPowerCopy = pIUdfFactory->GetPowerCopyList( );
		int SizeOfPowerCopy = pListPowerCopy->Size();
		if(SizeOfPowerCopy == 0)
		{
			cout<<"No PC Exists in PC File,Error in Insert PC"<<endl;
			//::MessageBox(NULL,L"当前的Part文件中木有PC，请检查文件",L"Error",MB_OK);
			return FALSE;
		}
		CATIUdfInstantiate * pIUdfInstantiate = NULL ;
		if( (NULL != pListPowerCopy) /*&& (SizeOfPowerCopy ==1 )*/ &&(NULL_var != (*pListPowerCopy)[1]) )
		{
			//cout<<"Suucess"<<endl;
			hr = (*pListPowerCopy)[1]->QueryInterface(IID_CATIUdfInstantiate,(void **)&pIUdfInstantiate);
			if ( SUCCEEDED(hr) )
			{
				delete pListPowerCopy ;
				pListPowerCopy = NULL ;
			}
		}
		pIUdfFactory->Release();
		pIUdfFactory = NULL ;

		CATPathElement * FirstUIactiveObject1 = NULL ;
		CATBaseUnknown_var FirstDest1 = NULL_var ; 
		/*if(PathNewPart == NULL)
		{
			cout<<"Error in PathNewPart"<<endl;
			return FALSE;
		}*/

	/*	if(pIUdfInstantiate == NULL)
		{
			cout<<"Error in pIUdfInstantiate"<<endl;
			return FALSE;
		}*/
		hr = pIUdfInstantiate->SetDestinationPath(&PathNewPart,FirstUIactiveObject1,FirstDest1);  
		if(FAILED(hr))
		{
			cout<<"Error in SetDestinationPath!"<<endl;
			return FALSE;
		}
		if ( NULL != FirstUIactiveObject1 ) 
		{
			FirstUIactiveObject1->Release();
			FirstUIactiveObject1 = NULL ;
		}

		//找同名的，然后赋值
		for(int i = 1;i<=ispListInput.Size();i++)
		{ 
			CATBaseUnknown_var spBaseOldInput = pIUdfInstantiate->GetOldInput(i);
			CATIAlias_var spAlias = spBaseOldInput;

			CATUnicodeString  USOldName = spAlias->GetAlias();
			for(int j = 1;j<=ispListInput.Size();j++)
			{
				CATIAlias_var spAlias2 = ispListInput[j];
				CATUnicodeString  USNewName = spAlias2->GetAlias();
				if(USNewName.SearchSubString(USOldName) != -1)
				{
					CATBaseUnknown *pInputLine1 = NULL;
					hr = ispListInput[j]->QueryInterface(IID_CATBaseUnknown,(void**)&pInputLine1);
					CATPathElement * pInput1 = new CATPathElement(pInputLine1);
					/*pInputLine1->Release();
					pInputLine1 = NULL;*/

					hr = pIUdfInstantiate->SetNewInput(i, pInput1);
					/*pInput1->Release();
					pInput1 = NULL;*/

					break;
				}
			}
		}

		//如果输入参数为0，则修改参数值
		//if(iType == 0)
		//{
			//
			//CATListValCATBaseUnknown_var * pParameterList = NULL;
			//CATListOfCATUnicodeString * pParameterRoleList = NULL ;
			//hr = pIUdfInstantiate->GetParameters(pParameterList,pParameterRoleList);
			//if ( NULL != pParameterList )
			//{
			//	int NbPublishedParameters = pParameterList->Size();
			//	for ( int i= 1 ; i <= NbPublishedParameters ; i++ )
			//	{
			//		CATICkeParm_var spCkeParmOnParameter = (*pParameterList)[i];
			//		CATIAlias_var spAliasCKE = spCkeParmOnParameter;

			//		if(spAliasCKE->GetAlias() == "RI")
			//		{
			//			cout<<"iRR  "<<iRR<<endl;
			//			spCkeParmOnParameter->Valuate(iRR/1000.0);
			//		}
			//		else if(spAliasCKE->GetAlias() == "n")
			//		{
			//			cout<<"iRR  "<<iRR<<endl;
			//			spCkeParmOnParameter->Valuate(iRR/1000.0);
			//		}
			//	}
			//}
		//}

		hr = pIUdfInstantiate->Instantiate(spBaseUnknownPrtPartNew1);
		if(FAILED(hr))
		{
			cout<<"Error in Instantiate!"<<endl;
		}

		hr = pIUdfInstantiate->EndInstantiate();
		pIUdfInstantiate->Release(); 
		pIUdfInstantiate=NULL;

		::CATUnLockDocument(*oDocContainPC);
		CATDocumentServices::Remove(*oDocContainPC);

		spSpecPrtPartNew->Update();
	}
	CATCatch(CATMfErrUpdate,pErrorUpdate)
	{
		cout<<"Can  not update!"<<endl;
		return FALSE;

	}
	CATCatch(CATError,pError)
	{
		cout<<"Can  not update!"<<endl;
		return FALSE;
	}
	CATCatchOthers
	{
		cout<<"Can  not update!"<<endl;
		return FALSE;
	}
	CATEndTry;






	return TRUE;

}



CATListValCATISpecObject_var  GetAllElementsInGSD(CATISpecObject_var ispSpecGSD,int iDimension)
{
	CATListValCATISpecObject_var  oListForReturn;
	oListForReturn.RemoveAll();

	CATIDescendants_var spDesSurfaceGSD = ispSpecGSD;
	if(spDesSurfaceGSD == NULL_var)
	{
		/*::MessageBox(NULL,L"请确定面集正确选择",L"Error",MB_OK);*/
		return oListForReturn;
	}
	if(iDimension == 0)
	{
		spDesSurfaceGSD->GetDirectChildren("CATIMfZeroDimResult",oListForReturn);
	}
	else if(iDimension == 1)
	{
		spDesSurfaceGSD->GetDirectChildren("CATIMfMonoDimResult",oListForReturn);
	}
	else if(iDimension == 2)
	{
		spDesSurfaceGSD->GetDirectChildren("CATIMfBiDimResult",oListForReturn);
	}
	return oListForReturn;
}

double GetDistanceBetweenLineSurface(CATISpecObject_var ispSpecLine,CATISpecObject_var ispSpecSurface)
{
	double dDistance = -1.0;

	CATIMeasurableInContext_var spMea1 = ispSpecLine;
	CATIMeasurable_var spMea2 = ispSpecSurface;
	if(!spMea1  || !spMea2)
	{
		cout<<"Error in Mea"<<endl;
		return  dDistance;
	}
	CATMathPoint pMathPT1;
	CATMathPoint pMathPT2;
	CATMathAxis pMathAxis;
	
	spMea1->MinimumDistance(spMea2,pMathAxis,dDistance,pMathPT1,pMathPT2);


	return dDistance;
}
double GetDistanceBetweenPTSurface(CATISpecObject_var ispSpecPT,CATISpecObject_var ispSpecSurface)
{
	double dDistance = -1.0;

	CATIMeasurableInContext_var spMea1 = ispSpecSurface;
	CATIMeasurablePoint_var spMea2 = ispSpecPT;
	if(!spMea1  || !spMea2)
	{
		cout<<"Error in Mea"<<endl;
		return  dDistance;
	}
	CATMathPoint pMathPT1,pMathPT2;
	spMea2->GetPoint(pMathPT1);

	spMea1->MinimumDistance(pMathPT1,dDistance,pMathPT2);


	return dDistance;
}
double DistanceBetweenLinePT(CATISpecObject_var ispSpecLine,CATMathPoint iMathPT)
{
	double dDistance = -1.0;


	CATIMeasurableCurve_var spMea2 = ispSpecLine;
	if(!spMea2)
	{
		return  dDistance;
	}
	CATMathPoint pMathPTStart;
	CATMathPoint pMathPTMid;
	CATMathPoint pMathPTEnd;
	spMea2->GetPoints(pMathPTStart,pMathPTMid,pMathPTEnd);

	dDistance = iMathPT.DistanceTo(pMathPTMid);


	return dDistance;
}



double DistanceBetweenPlanePT(CATISpecObject_var ispSpecPlane,CATMathPoint iMathPT)
{
	double dDistance = -1.0;


	CATIMeasurablePlane_var spMea2 = ispSpecPlane;
	if(!spMea2)
	{
		return  dDistance;
	}
	CATMathPlane pMathPlane;
	spMea2->GetPlane(pMathPlane);

	dDistance = pMathPlane.DistanceTo(iMathPT);


	return dDistance;
}




CATListValCATISpecObject_var TopoDisAssemble(CATISpecObject_var spGSMBoundary)
{
	CATListValCATISpecObject_var spListOfCells; 
	spListOfCells.RemoveAll();
	CATTry
	{
		CATIGeometricalElement_var  spGeoBoundary = spGSMBoundary;
		CATBody_var spBodyBoundary=spGeoBoundary->GetBodyResult();	

		CATLISTP(CATDomain) ListDomainOfBoundary;
		spBodyBoundary->GetAllDomains(1,3,ListDomainOfBoundary);
		int  NumberOfDomain=ListDomainOfBoundary.Size(); 


		for(int i=1;i<=NumberOfDomain;i++)
		{
			CATDomain_var spDomainBoundary = ListDomainOfBoundary[i];

			long NbCell = spDomainBoundary->GetNbCellUses();
			//cout<<"Cell Number = "<<NbCell<<endl;

			for(int j=1;j<=NbCell;j++)
			{
				CATCell * pCell = spDomainBoundary->GetCell(j);	

				CATCell_var spCellBoundary = pCell;
				CATIBRepAccess_var spBRepAccess= CATBRepDecode(spCellBoundary,spGeoBoundary);			

				CATIFeaturize_var spFratureCell = spBRepAccess;			 
				CATISpecObject_var spObjectFromCell = spFratureCell->FeaturizeR(MfPermanentBody);

				CATIMfBRep_var spBRepOnFeature = spObjectFromCell; 
				CATBody_var spBodyFromObject=spBRepOnFeature->GetBody();

				CATISpecObject *piObjectFromBody = NULL;
				spDatumFact->InstanciateDatum(spBodyFromObject,piObjectFromBody);
				CATISpecObject_var spObjectFromBody = piObjectFromBody;
				spObjectFromBody->Update();

				spListOfCells.Append(spObjectFromBody);
			}	
		}
	}
	CATCatch(CATMfErrUpdate,pErrorUpdate)
	{
		cout<<"Error in Topo Analysis"<<endl;
		return NULL;
	}
	CATCatch(CATError,pError)
	{
		cout<<"Error in Topo Analysis"<<endl;
		return NULL;
	}
	CATEndTry;

	return spListOfCells;
}



CATISpecObject_var SmoothBoundary(CATISpecObject_var ispSpecBoundary)
{
	CATIGSMCurveSmooth_var spGSMSmooth = spGSMFact->CreateCurveSmooth(ispSpecBoundary);  
	//设置相切阀值
	CATICkeParm_var spCkeAngle = spCkeParmFactory->CreateAngle("Angle",5*CATPI/180); 
	spGSMSmooth->SetTangencyThreshold(spCkeAngle);

	CATICkeParm_var spCkeRatio = spCkeParmFactory->CreateReal("Ratio",0.98); 
	spGSMSmooth->SetCurvatureThreshold(spCkeRatio);
	CATISpecObject_var spSpecSmooth = spGSMSmooth;
	CATTry
	{
		spSpecSmooth->Update();
	}
	CATCatch(CATMfErrUpdate,pErrorUpdate)
	{
		cout<<"Error in Boundary Extract"<<endl;
		return NULL_var;
	}
	CATCatch(CATError,pError)
	{
		cout<<"Error in Boundary Extract"<<endl;
		return NULL_var;
	}
	CATEndTry;


	return spSpecSmooth;
}




CATISpecObject_var SmoothBoundary2(CATISpecObject_var ispSpecBoundary,double dCurvature)
{
	CATIGSMCurveSmooth_var spGSMSmooth = spGSMFact->CreateCurveSmooth(ispSpecBoundary);  
	////设置相切阀值
	CATICkeParm_var spCkeAngle = spCkeParmFactory->CreateAngle("Angle",5*CATPI/180); 
	spGSMSmooth->SetTangencyThreshold(spCkeAngle);

	CATICkeParm_var spCkeRatio = spCkeParmFactory->CreateReal("Ratio",dCurvature); 
	//spGSMSmooth->SetCurvatureThreshold(spCkeRatio);
	CATISpecObject_var spSpecSmooth = spGSMSmooth;
	CATTry
	{
		spSpecSmooth->Update();
	}
	CATCatch(CATMfErrUpdate,pErrorUpdate)
	{
		cout<<"Error in Boundary Smooth"<<endl;
		return NULL_var;
	}
	CATCatch(CATError,pError)
	{
		cout<<"Error in Boundary Smooth"<<endl;
		return NULL_var;
	}
	CATEndTry;


	return spSpecSmooth;
}

CATISpecObject_var SmoothBoundaryCrv(CATISpecObject_var ispSpecBoundary)
{
	CATIGSMCurveSmooth_var spGSMSmooth = spGSMFact->CreateCurveSmooth(ispSpecBoundary);  
	spGSMSmooth->SetCorrectionMode( CATGSMCSCorrectionMode_Curvature); 
	spGSMSmooth->SetMaximumDeviation(0.001);
	CATISpecObject_var spSpecSmooth = spGSMSmooth;
	CATTry
	{
		spSpecSmooth->Update();
	}
	CATCatch(CATMfErrUpdate,pErrorUpdate)
	{
		cout<<"Error in Boundary Extract"<<endl;
		return NULL_var;
	}
	CATCatch(CATError,pError)
	{
		cout<<"Error in Boundary Extract"<<endl;
		return NULL_var;
	}
	CATEndTry;


	return spSpecSmooth;
}



CATISpecObject_var ExtractSurface(CATISpecObject_var ispSpecSurface)
{
	CATIGSMBoundary_var spGSMBoundary = spGSMFact->CreateBoundary(ispSpecSurface,NULL_var,BOUNDARY_FROM_SPEC);

	CATISpecObject_var spSpecBoundary = spGSMBoundary;
	CATTry
	{
		spSpecBoundary->Update();
	}
	CATCatch(CATMfErrUpdate,pErrorUpdate)
	{
		//cout<<"Error in Boundary Extract"<<endl;
		return NULL_var;
	}
	CATCatch(CATError,pError)
	{
		cout<<"Error in Boundary Extract"<<endl;
		return NULL_var;
	}
	CATEndTry;


	return spSpecBoundary;
}



BOOL InsertPC2(CATLISTV(CATISpecObject_var) ispListInput,CATIPrtPart_var ispPrtPart,CATIDescendants_var spNewDes,CATUnicodeString PCFilePathName)
{
	//第这个文件下插入powerCopy
	CATISpecObject_var spSpecPrtPartNew = ispPrtPart;   
	CATBaseUnknown_var spBaseUnknownPrtPartNew = spSpecPrtPartNew ;
	CATPathElement PathNewPart(spBaseUnknownPrtPartNew); 


	CATTry
	{

		CATDocument*  oDocContainPC = NULL;
		HRESULT hr = CATDocumentServices::OpenDocument(PCFilePathName,oDocContainPC); 
		if(oDocContainPC == NULL)
		{
			cout<<"Error in oDocContainPC!"<<endl;
			::MessageBox(NULL,L"请确定选择PC文件的路径是否正确",L"Error",MB_OK);
			return FALSE;
		}
		//锁定文件
		::CATLockDocument(*oDocContainPC);


		CATInit *pInitOnDoc2G = NULL ;
		hr = oDocContainPC->QueryInterface(IID_CATInit,(void **) &pInitOnDoc2G);
		if ( FAILED(hr) )return 1 ;
		CATIPrtContainer *pIPrtContOnUDFDoc2G= NULL ;
		pIPrtContOnUDFDoc2G= (CATIPrtContainer*)pInitOnDoc2G->GetRootContainer("CATIPrtContainer");
		if ( NULL == pIPrtContOnUDFDoc2G) return 1 ; 
		pInitOnDoc2G->Release();
		pInitOnDoc2G = NULL ;  
		CATIPrtPart_var spPrtPartOnUDFPart2G = pIPrtContOnUDFDoc2G->GetPart();

		CATIUdfFactory *pIUdfFactory = NULL ;
		hr = pIPrtContOnUDFDoc2G->QueryInterface(IID_CATIUdfFactory,(void **) &pIUdfFactory);
		pIPrtContOnUDFDoc2G->Release();
		pIPrtContOnUDFDoc2G = NULL;

		CATListValCATISpecObject_var* pListPowerCopy = NULL;
		pListPowerCopy = pIUdfFactory->GetPowerCopyList( );
		int SizeOfPowerCopy = pListPowerCopy->Size();
		if(SizeOfPowerCopy == 0)
		{
			::MessageBox(NULL,L"当前的Part文件中木有PC，请检查文件",L"Error",MB_OK);
			return FALSE;
		}
		CATIUdfInstantiate * pIUdfInstantiate = NULL ;
		if( (NULL != pListPowerCopy) && (SizeOfPowerCopy ==1 ) &&(NULL_var != (*pListPowerCopy)[1]) )
		{
			hr = (*pListPowerCopy)[1]->QueryInterface(IID_CATIUdfInstantiate,(void **)&pIUdfInstantiate);
			if ( SUCCEEDED(hr) )
			{
				delete pListPowerCopy ;
				pListPowerCopy = NULL ;
			}
		}
		pIUdfFactory->Release();
		pIUdfFactory = NULL ;

		CATPathElement * FirstUIactiveObject1 = NULL ;
		CATBaseUnknown_var FirstDest1 = NULL_var ; 
		hr = pIUdfInstantiate->SetDestinationPath(&PathNewPart,FirstUIactiveObject1,FirstDest1);  
		if(FAILED(hr))
		{
			cout<<"Error in SetDestinationPath!"<<endl;
			return FALSE;
		}
		//cout<<"2"<<endl;

		//找同名的，然后赋值
		for(int i = 1;i<=ispListInput.Size();i++)
		{ 
			//cout<<"i   "<<i<<endl;
			CATBaseUnknown *pInputLine1 = NULL;
			hr = ispListInput[i]->QueryInterface(IID_CATBaseUnknown,(void**)&pInputLine1);
			CATPathElement * pInput1 = new CATPathElement(pInputLine1);
			pInputLine1->Release();
			pInputLine1 = NULL;

			hr = pIUdfInstantiate->SetNewInput(i, pInput1);
			pInput1->Release();
			pInput1 = NULL;

		}
		//cout<<"3"<<endl;
		hr = pIUdfInstantiate->Instantiate(spBaseUnknownPrtPartNew);
		if(FAILED(hr))
		{
			cout<<"Error in Instantiate!"<<endl;
		}


		//cout<<"4"<<endl;

		hr = pIUdfInstantiate->EndInstantiate();
		//cout<<"5"<<endl;


		pIUdfInstantiate->Release();
		pIUdfInstantiate=NULL;

		::CATUnLockDocument(*oDocContainPC);
		CATDocumentServices::Remove(*oDocContainPC);

		//spSpecPrtPartNew->Update();
	}
	CATCatch(CATMfErrUpdate,pErrorUpdate)
	{
		cout<<"Can  not update!"<<endl;
		return FALSE;

	}
	CATCatch(CATError,pError)
	{
		cout<<"Can  not update!"<<endl;
		return FALSE;
	}
	CATCatchOthers
	{
		cout<<"Can  not update!"<<endl;
		return FALSE;
	}
	CATEndTry;
	//cout<<"6"<<endl;






	return TRUE;

}

BOOL InsertPC3(CATLISTV(CATISpecObject_var) ispListInput,CATIPrtPart_var ispPrtPart,CATIDescendants_var spNewDes,CATUnicodeString PCFilePathName)
{
	//第这个文件下插入powerCopy
	CATISpecObject_var spSpecPrtPartNew = ispPrtPart;   
	CATBaseUnknown_var spBaseUnknownPrtPartNew = spSpecPrtPartNew ;
	CATBaseUnknown* pBaseUnknownPrtPartNew= (CATBaseUnknown*)spBaseUnknownPrtPartNew;
	CATPathElement PathNewPart(pBaseUnknownPrtPartNew); 
	CATBaseUnknown_var spBaseUnknownPrtPartNew1=pBaseUnknownPrtPartNew;
	pBaseUnknownPrtPartNew->Release();
	pBaseUnknownPrtPartNew = NULL;


	CATTry
	{

		CATDocument*  oDocContainPC = NULL;
		HRESULT hr = CATDocumentServices::OpenDocument(PCFilePathName,oDocContainPC); 
		if(oDocContainPC == NULL)
		{
			cout<<"Error in oDocContainPC!"<<endl;
			::MessageBox(NULL,L"请确定选择PC文件的路径是否正确",L"Error",MB_OK);
			return FALSE;
		}
		//锁定文件
		cout<<"OpenDocument Finished"<<endl;
		::CATLockDocument(*oDocContainPC);


		CATInit *pInitOnDoc2G = NULL ;
		hr = oDocContainPC->QueryInterface(IID_CATInit,(void **) &pInitOnDoc2G);
		if ( FAILED(hr) )return 1 ;
		CATIPrtContainer *pIPrtContOnUDFDoc2G= NULL ;
		pIPrtContOnUDFDoc2G= (CATIPrtContainer*)pInitOnDoc2G->GetRootContainer("CATIPrtContainer");
		if ( NULL == pIPrtContOnUDFDoc2G) return 1 ; 
		pInitOnDoc2G->Release();
		pInitOnDoc2G = NULL ;  
		CATIPrtPart_var spPrtPartOnUDFPart2G = pIPrtContOnUDFDoc2G->GetPart();

		CATIUdfFactory *pIUdfFactory = NULL ;
		hr = pIPrtContOnUDFDoc2G->QueryInterface(IID_CATIUdfFactory,(void **) &pIUdfFactory);
		pIPrtContOnUDFDoc2G->Release();
		pIPrtContOnUDFDoc2G = NULL;

		CATListValCATISpecObject_var* pListPowerCopy = NULL;
		pListPowerCopy = pIUdfFactory->GetPowerCopyList( );
		int SizeOfPowerCopy = pListPowerCopy->Size();
		if(SizeOfPowerCopy == 0)
		{
			::MessageBox(NULL,L"当前的Part文件中木有PC，请检查文件",L"Error",MB_OK);
			return FALSE;
		}
		cout<<"SizeOfPowerCopy  "<<SizeOfPowerCopy<<endl;
		CATIUdfInstantiate * pIUdfInstantiate = NULL ;
		if( (NULL != pListPowerCopy) && (SizeOfPowerCopy ==1 ) &&(NULL_var != (*pListPowerCopy)[1]) )
		{
			hr = (*pListPowerCopy)[1]->QueryInterface(IID_CATIUdfInstantiate,(void **)&pIUdfInstantiate);
			if ( SUCCEEDED(hr) )
			{
				delete pListPowerCopy ;
				pListPowerCopy = NULL ;
			}
		}
		pIUdfFactory->Release();
		pIUdfFactory = NULL ;

		cout<<"SetDestinationPath  Begin"<<endl;		
		CATPathElement * FirstUIactiveObject1 = NULL ;
		CATBaseUnknown_var FirstDest1 = NULL_var ; 
		hr = pIUdfInstantiate->SetDestinationPath(&PathNewPart,FirstUIactiveObject1,FirstDest1);  
		if(FAILED(hr))
		{
			cout<<"Error in SetDestinationPath!"<<endl;
			return FALSE;
		}

		//找同名的，然后赋值
		cout<<"Input Begin"<<endl;
		for(int i = 1;i<=ispListInput.Size();i++)
		{ 
			CATBaseUnknown_var spBaseOldInput = pIUdfInstantiate->GetOldInput(i);
			CATIAlias_var spAlias = spBaseOldInput;
			CATUnicodeString  USOldName = spAlias->GetAlias();
			for(int j = 1;j<=ispListInput.Size();j++)
			{
				CATIAlias_var spAlias2 = ispListInput[j];
				CATUnicodeString  USNewName = spAlias2->GetAlias();
				if(USNewName.SearchSubString(USOldName) != -1)
				{
					CATBaseUnknown *pInputLine1 = NULL;
					hr = ispListInput[j]->QueryInterface(IID_CATBaseUnknown,(void**)&pInputLine1);
					CATPathElement * pInput1 = new CATPathElement(pInputLine1);
					pInputLine1->Release();
					pInputLine1 = NULL;

					hr = pIUdfInstantiate->SetNewInput(i, pInput1);
					pInput1->Release();
					pInput1 = NULL;

					break;
				}
			}

		}
		cout<<"Input Finished"<<endl;

		hr = pIUdfInstantiate->Instantiate(spBaseUnknownPrtPartNew1);
		if(FAILED(hr))
		{
			cout<<"Error in Instantiate!"<<endl;
		}

		cout<<"Instantiate Finished"<<endl;
		hr = pIUdfInstantiate->EndInstantiate();
		cout<<"EndInstantiate Finished"<<endl;

		pIUdfInstantiate->Release();
		pIUdfInstantiate=NULL;

		::CATUnLockDocument(*oDocContainPC);
		CATDocumentServices::Remove(*oDocContainPC);

		spSpecPrtPartNew->Update();
	}
	CATCatch(CATMfErrUpdate,pErrorUpdate)
	{
		cout<<"Can  not update!"<<endl;
		return FALSE;

	}
	CATCatch(CATError,pError)
	{
		cout<<"Can  not update!"<<endl;
		return FALSE;
	}
	CATCatchOthers
	{
		cout<<"Can  not update!"<<endl;
		return FALSE;
	}
	CATEndTry;






	return TRUE;

}


//BOOL InsertPCForHKBZW(CATLISTV(CATISpecObject_var) ispListInput,CATIPrtPart_var ispPrtPart,CATIDescendants_var spNewDes,CATUnicodeString PCFilePathName)
//
BOOL InsertPCForHKBZW(CATISpecObject_var ispSpecObject,CATIPrtPart_var ispPrtPart,CATIDescendants_var spNewDes,CATUnicodeString PCFilePathName,int inumber)//iType=1则不修改参数值
{

//第这个文件下插入powerCopy
CATISpecObject_var spSpecPrtPartNew = ispPrtPart;   
CATBaseUnknown_var spBaseUnknownPrtPartNew = spSpecPrtPartNew ;
CATBaseUnknown* pBaseUnknownPrtPartNew= (CATBaseUnknown*)spBaseUnknownPrtPartNew;
CATPathElement PathNewPart(pBaseUnknownPrtPartNew); 
CATBaseUnknown_var spBaseUnknownPrtPartNew1=pBaseUnknownPrtPartNew;
pBaseUnknownPrtPartNew->Release();
pBaseUnknownPrtPartNew = NULL;


CATTry
{
	CATDocument*  oDocContainPC = NULL;
	HRESULT hr = CATDocumentServices::OpenDocument(PCFilePathName,oDocContainPC); 
	if(oDocContainPC == NULL)
	{
		cout<<"Error in oDocContainPC!"<<endl;
		//::MessageBox(NULL,L"请确定选择PC文件的路径是否正确",L"Error",MB_OK);
		cout<<"PC Path Error,Error in Insert PC"<<endl;
		return FALSE;
	}
	//锁定文件
	::CATLockDocument(*oDocContainPC);


	CATInit *pInitOnDoc2G = NULL ;
	hr = oDocContainPC->QueryInterface(IID_CATInit,(void **) &pInitOnDoc2G);
	if ( FAILED(hr) )return 1 ;
	CATIPrtContainer *pIPrtContOnUDFDoc2G= NULL ;
	pIPrtContOnUDFDoc2G= (CATIPrtContainer*)pInitOnDoc2G->GetRootContainer("CATIPrtContainer");
	if ( NULL == pIPrtContOnUDFDoc2G) return 1 ; 

	pInitOnDoc2G->Release();
	pInitOnDoc2G = NULL ;  

	CATIPrtPart_var spPrtPartOnUDFPart2G = pIPrtContOnUDFDoc2G->GetPart();

	CATIUdfFactory *pIUdfFactory = NULL ;
	hr = pIPrtContOnUDFDoc2G->QueryInterface(IID_CATIUdfFactory,(void **) &pIUdfFactory);
	pIPrtContOnUDFDoc2G->Release();
	pIPrtContOnUDFDoc2G = NULL;

	CATListValCATISpecObject_var* pListPowerCopy = NULL;
	pListPowerCopy = pIUdfFactory->GetPowerCopyList( );
	int SizeOfPowerCopy = pListPowerCopy->Size();
	if(SizeOfPowerCopy == 0)
	{
		cout<<"No PC Exists in PC File,Error in Insert PC"<<endl;
		//::MessageBox(NULL,L"当前的Part文件中木有PC，请检查文件",L"Error",MB_OK);
		return FALSE;
	}
	CATIUdfInstantiate * pIUdfInstantiate = NULL ;
	if( (NULL != pListPowerCopy) /*&& (SizeOfPowerCopy ==1 )*/ &&(NULL_var != (*pListPowerCopy)[1]) )
	{
		//cout<<"Suucess"<<endl;
		hr = (*pListPowerCopy)[1]->QueryInterface(IID_CATIUdfInstantiate,(void **)&pIUdfInstantiate);
		if ( SUCCEEDED(hr) )
		{
			delete pListPowerCopy ;
			pListPowerCopy = NULL ;
		}
	}
	pIUdfFactory->Release();
	pIUdfFactory = NULL ;

	CATPathElement * FirstUIactiveObject1 = NULL ;
	CATBaseUnknown_var FirstDest1 = NULL_var ; 
	if(PathNewPart == NULL)
	{
		cout<<"Error in PathNewPart"<<endl;
		return FALSE;
	}

	if(pIUdfInstantiate == NULL)
	{
		cout<<"Error in pIUdfInstantiate"<<endl;
		return FALSE;
	}
	hr = pIUdfInstantiate->SetDestinationPath(&PathNewPart,FirstUIactiveObject1,FirstDest1);  
	if(FAILED(hr))
	{
		cout<<"Error in SetDestinationPath!"<<endl;
		return FALSE;
	}
	if ( NULL != FirstUIactiveObject1 ) 
	{
		FirstUIactiveObject1->Release();
		FirstUIactiveObject1 = NULL ;
	}


	CATBaseUnknown *pInputLine1 = NULL;
	hr = ispSpecObject->QueryInterface(IID_CATBaseUnknown,(void**)&pInputLine1);
	CATPathElement * pInput1 = new CATPathElement(pInputLine1);
	pInputLine1->Release();
	pInputLine1 = NULL;

	hr = pIUdfInstantiate->SetNewInput(1, pInput1);
	pInput1->Release();
	pInput1 = NULL;



	//如果输入参数为0，则修改参数值
	CATListValCATBaseUnknown_var * pParameterList = NULL;
	CATListOfCATUnicodeString * pParameterRoleList = NULL ;
	hr = pIUdfInstantiate->GetParameters(pParameterList,pParameterRoleList);
	if ( NULL != pParameterList )
	{
		int NbPublishedParameters = pParameterList->Size();
		//cout<<"NbPublishedParameters   "<<NbPublishedParameters<<endl;
		for ( int i= 1 ; i <= NbPublishedParameters ; i++ )
		{
			CATICkeParm_var spCkeParmOnParameter = (*pParameterList)[i];
			CATIAlias_var spAliasCKE = spCkeParmOnParameter;
			if(spAliasCKE->GetAlias() == "n")
			{
				/*cout<<"Parameter Found"<<endl;
				cout<<"iRR  "<<inumber<<endl;*/
				spCkeParmOnParameter->Valuate(inumber);
				//cout<<"Value is "<<spCkeParmOnParameter->Value()->AsInteger()<<endl;
			}
		}
	}

	hr = pIUdfInstantiate->Instantiate(spBaseUnknownPrtPartNew1);
	if(FAILED(hr))
	{
		cout<<"Error in Instantiate!"<<endl;
	}

	hr = pIUdfInstantiate->EndInstantiate();
	pIUdfInstantiate->Release(); 
	pIUdfInstantiate=NULL;

	::CATUnLockDocument(*oDocContainPC);
	CATDocumentServices::Remove(*oDocContainPC);

	spSpecPrtPartNew->Update();
}
CATCatch(CATMfErrUpdate,pErrorUpdate)
{
	cout<<"Can  not update!"<<endl;
	return FALSE;

}
CATCatch(CATError,pError)
{
	cout<<"Can  not update!"<<endl;
	return FALSE;
}
CATCatchOthers
{
	cout<<"Can  not update!"<<endl;
	return FALSE;
}
CATEndTry;


return TRUE;


}


double GetCurveLength(CATISpecObject_var spSpecCurve)
{
	double dValue = -1.0;
	CATIMeasurableCurve_var spMeaCurve = spSpecCurve;
	if(spMeaCurve != NULL_var)
	{
		spMeaCurve->GetLength(dValue);
	}

	return dValue;
}


void GetLineColor(CATISpecObject_var spSpec,unsigned int &iRed,unsigned int &iGreen,unsigned int &iBlack)
{
		HRESULT rc = S_OK;

		CATIVisProperties *piGraphProp=NULL;
		rc = spSpec->QueryInterface(IID_CATIVisProperties,(void**)&piGraphProp);
		if( FAILED(rc) || spSpec == NULL )
		{
			cout<<"Error in piGraphProp"<<endl;
			return ;
		}
		CATVisPropertiesValues               MyPropertyOnMesh;
		rc = piGraphProp->GetPropertiesAtt(MyPropertyOnMesh,CATVPColor,CATVPLine  );
		MyPropertyOnMesh.GetColor(iRed,iGreen,iBlack);

		piGraphProp->Release();
		piGraphProp = NULL;

}

void GetSurfaceColor(CATISpecObject_var spSpec,unsigned int &iRed,unsigned int &iGreen,unsigned int &iBlack)
{
	HRESULT rc = S_OK;

	CATIVisProperties *piGraphProp=NULL;
	rc = spSpec->QueryInterface(IID_CATIVisProperties,(void**)&piGraphProp);
	if( FAILED(rc) || spSpec == NULL )
	{
		cout<<"Error in piGraphProp"<<endl;
		return ;
	}
	CATVisPropertiesValues               MyPropertyOnMesh;
	rc = piGraphProp->GetPropertiesAtt(MyPropertyOnMesh,CATVPColor,CATVPMesh );
	MyPropertyOnMesh.GetColor(iRed,iGreen,iBlack);

	piGraphProp->Release();
	piGraphProp = NULL;

}

BOOL GetCertainFilesFromFolder(CATLISTV(CATUnicodeString) &ioLstFilePath,CATUnicodeString istrSuffix,CATUnicodeString iStrFolderPath)
{
	ioLstFilePath.RemoveAll();
	CATDirectory Dir;
	CATLibStatus status = ::CATOpenDirectory (iStrFolderPath, &Dir);
	if ( CATLibSuccess != status)
	{
		return FALSE;
	}

	int EndOfDir=0;
	status = CATLibSuccess ;
	CATDirectoryEntry Entry;
	int i=0;
	while (( EndOfDir !=1) && ( CATLibSuccess == status))
	{
		status= ::CATReadDirectory ( &Dir, &Entry, &EndOfDir);
		if (( CATLibSuccess == status ))
		{
			CATUnicodeString StrFileName(Entry.name);
			if(StrFileName.SearchSubString(istrSuffix, 0, CATUnicodeString::CATSearchModeForward)!=-1)
			{
				CATUnicodeString StrTemp = iStrFolderPath + "\\" + StrFileName;
				ioLstFilePath.Append(StrTemp);
			}
		}
	}
	status = ::CATCloseDirectory (&Dir);
	if ( CATLibSuccess != status)
	{
		return FALSE;
	}

	return TRUE;
}

CATMathPoint ProjectPTOnPlane(CATISpecObject_var ispSpecPlane,CATMathPoint iPT)
{
	CATMathPoint pPTResult;


	CATIMeasurablePlane_var spMea2 = ispSpecPlane;

	CATMathPlane pMathPlane;
	spMea2->GetPlane(pMathPlane);

	pMathPlane.Project(iPT,pPTResult);

	return pPTResult;
}

CATUnicodeString getPathNameWithoutSuffix(const CATUnicodeString &fullpath)
{
	int posDot =  fullpath.SearchSubString(".",0,CATUnicodeString::CATSearchModeBackward);
	CATUnicodeString noSuffix(fullpath);
	if (posDot > 0)
	{
		noSuffix = fullpath.SubString(0,posDot) ;
	}
	return noSuffix;
}

CATUnicodeString getSuffix(CATDocument *pDoc)
{
	// no profile logging here
	CATUnicodeString suffix;
	if (pDoc != NULL)
	{
		CATUnicodeString completeDocPath = pDoc->StorageName();
		int posDot =  completeDocPath.SearchSubString(".",0,CATUnicodeString::CATSearchModeBackward);
		if (posDot > 0)
		{
			int lengthOfSuffix = completeDocPath.GetLengthInChar() - posDot - 1 ;
			suffix = completeDocPath.SubString(posDot+1,lengthOfSuffix) ;
		}
	}
	return suffix;
}


void CV5_getCATIAVersionInfo(int &ver, int &release, int &service_pack, int &hot_fix)
{

	CATIALevelInfo cat_info;
	CATGetCATIALevelInfo(&cat_info);


	ver = cat_info.Version;
	release = cat_info.Release;
	service_pack = cat_info.ServicePack;
	hot_fix = cat_info.HotFix;


}

int CompareTwoPTs(CATISpecObject_var ispSpecPT1,CATISpecObject_var ispSpecPT2)
{
	int iReturn  = 0;
	CATIMeasurablePoint_var spMeaPT1 = ispSpecPT1;
	CATIMeasurablePoint_var spMeaPT2 = ispSpecPT2;

	CATMathPoint iPt1;
	CATMathPoint iPt2;
	spMeaPT1->GetPoint(iPt1);
	spMeaPT2->GetPoint(iPt2);

	if (abs(iPt1.GetX()-iPt2.GetX())<=0.01 && abs(iPt1.GetY()-iPt2.GetY())<=0.01 && abs(iPt1.GetZ()-iPt2.GetZ())<=0.01)
	{
		//两点相同
		iReturn = 1;
	}

	return iReturn;
	
}
double GetDistanceFromPTPT(CATISpecObject_var spSpecPT1,CATISpecObject_var spSpecPT2)
{
	CATIMeasurablePoint_var spMeaPT1 = spSpecPT1;
	CATIMeasurablePoint_var spMeaPT2 = spSpecPT2;

	CATMathPoint iPt1;
	CATMathPoint iPt2;
	spMeaPT1->GetPoint(iPt1);
	spMeaPT2->GetPoint(iPt2);

	double dDistance = iPt1.DistanceTo(iPt2);

	return dDistance;
	
}




double GetMaxFromList(CATListOfDouble   spListWidth)
{
	int iSize = spListWidth.Size();
	if (iSize == 0)
	{
		return -1.0;
	}


	double dForMax = -1000.0;
	//double dMax = 0.0;
	for (int i = 1;i<=iSize;i++)
	{
		if (spListWidth[i]>=dForMax)
		{
			dForMax = spListWidth[i];
		}
	}
	return dForMax;
}

double GetMinFromList(CATListOfDouble   spListWidth)
{
	int iSize = spListWidth.Size();
	if (iSize == 0)
	{
		return -1.0;
	}


	double dForMin = 10000000.0;
	//double dMax = 0.0;
	for (int i = 1;i<=iSize;i++)
	{
		if (spListWidth[i]<=dForMin)
		{
			dForMin = spListWidth[i];
		}
	}
	return dForMin;
}

CATIDescendants_var CreateNewGSDUnderGSD(CATIMechanicalRootFactory_var spMechanicalRootFactory,CATISpecObject_var spDestination,CATUnicodeString USNameOfNewGSD)
{
	if(spMechanicalRootFactory == NULL_var)
	{
		cout<<"Error in spMechanicalRootFactory!"<<endl;
		return NULL_var;
	}

	CATISpecObject_var ospCAATempGSD = NULL_var;
	CATUnicodeString NameOfNewGSD(USNameOfNewGSD);
	spMechanicalRootFactory->CreateGeometricalSet(NameOfNewGSD,spDestination,ospCAATempGSD,1);
	
	if(ospCAATempGSD == NULL_var)
	{
		cout<<"Error in Create New GSD"<<endl;
		return NULL_var;
	}

	spPrtPart->SetCurrentFeature(ospCAATempGSD);
	CATIBasicTool_var spBasicTool1=spPrtPart-> GetCurrentTool( ); 
	CATIDescendants_var spCAATempDescendant= NULL_var;
	spCAATempDescendant = spBasicTool1;
	if(spCAATempDescendant == NULL_var)
	{
		cout<<"Error in spCAATempDescendant!"<<endl;
		return NULL_var;
	}
	return spCAATempDescendant;
}


double GetAngleBetweenLineLine2222(CATISpecObject_var ispSpecLine1,CATISpecObject_var ispSpecLine2)
{
	double dDistance = -1.0;

	CATIMeasurableInContext_var spMea1 = ispSpecLine1;
	CATIMeasurable_var spMea2 = ispSpecLine2;
	if(!spMea1  || !spMea2)
	{
		return  dDistance;
	}
	CATMathPoint pMathPT1;
	CATMathPoint pMathPT2;
	CATMathAxis pMathAxis;

	spMea1->Angle(spMea2,pMathAxis,dDistance,&pMathPT1,&pMathPT2);


	return dDistance;
}
HRESULT NewDraftingDoc(CATUnicodeString storePath)
{
	//MessageBox(NULL, L"NewDraftingDoc", L"Test", FALSE);
	//cout << endl;
	//cout << "DialogboxBasedCommandDrawing::NewDraftingDoc begin" << endl;

	/**********************************************************************************/

	CATDocument *pDrawingDoc = NULL;
	HRESULT hr = CATDocumentServices::New("CATDrawing", pDrawingDoc);
	if (FAILED(hr) || !pDrawingDoc)
	{
		return E_FAIL;
	}
	// a: Gets the drawing feature and drawing container
	CATIDftDocumentServices *piDftDocServices = NULL;
	CATIDftDrawing *piDftDrawingTemp = NULL;
	CATIContainer_var spDrwContTemp;
	if (SUCCEEDED(pDrawingDoc->QueryInterface(IID_CATIDftDocumentServices, (void **)&piDftDocServices)))
	{
		if (SUCCEEDED(piDftDocServices->GetDrawing(IID_CATIDftDrawing, (void **)&piDftDrawingTemp)))
		{
			if (piDftDrawingTemp)
			{
				// Gets the drawing container
				CATISpecObject *piSpecObj=NULL;
				if (SUCCEEDED(piDftDrawingTemp->QueryInterface(IID_CATISpecObject,(void **)&piSpecObj)))
				{
					spDrwContTemp = piSpecObj->GetFeatContainer();
					piSpecObj->Release();
					piSpecObj=NULL;
				}
			}
		}
		piDftDocServices->Release();
		piDftDocServices=NULL;
	}

	if (NULL_var == spDrwContTemp || !piDftDrawingTemp)
	{
		CATDocumentServices::Remove (*pDrawingDoc);
		return E_FAIL;
	}

	// b: Gets standard manager from the drawing container.
	CATIDftStandardManager *piStdmgr = NULL;

	if (SUCCEEDED(spDrwContTemp->QueryInterface(IID_CATIDftStandardManager,(void**)&piStdmgr)))
	{
		CATIStringList *piListstd = NULL;
		if ( SUCCEEDED(piStdmgr->GetAvailableStandards(&piListstd)) && piListstd )
		{
			unsigned int  nbrstd = 0;
			piListstd->Count(&nbrstd);
			for (unsigned int indice = 0; indice < nbrstd; indice ++)
			{
				wchar_t  *wstd = NULL;
				if ( SUCCEEDED ( piListstd->Item ( indice, &wstd ) )  && wstd )
				{
					const CATUnicodeString ANSI_UncS = "ISO";
					CATUnicodeString stdname;
					stdname.BuildFromWChar(wstd);
					if ( stdname == ANSI_UncS )  
					{
						// Import the ANSI standard in the document
						piStdmgr->ImportStandard (wstd);
						break;
					}
				}
				if (wstd) 
				{
					delete[] wstd; 
					wstd = NULL;
				}
			}
			piListstd->Release(); piListstd=NULL;            
		}
		piStdmgr->Release (); piStdmgr=NULL;
	}

	//Standard:  ANSI
	//Standard:  ASME
	//Standard:  ASME_3D
	//Standard:  ISO
	//Standard:  ISO_3D
	//Standard:  JIS
	//Standard:  JIS_3D

	// c: Gets available formats from drawing
	CATIDftDrawingFormats *piDftFormats = NULL;
	CATUnicodeString myFormatName;
	if (SUCCEEDED(piDftDrawingTemp->QueryInterface(IID_CATIDftDrawingFormats,(void **)&piDftFormats)))
	{
		CATLISTV(CATISpecObject_var) spListFormat;
		if (SUCCEEDED(piDftFormats->GetAvailableFormats(spListFormat)))
		{
			int nbformats= spListFormat.Size();
			// Gets the first format in the list.
			if (nbformats >= 1)
			{
				CATIDftFormat_var spFormat = spListFormat[4];
				if (FAILED(spFormat->GetFormatName(myFormatName)))
				{
					piDftFormats->Release();
					piDftFormats=NULL;
					piDftDrawingTemp->Release();
					piDftDrawingTemp=NULL;
					CATDocumentServices::Remove (*pDrawingDoc);
					return E_FAIL;
				}

				//spFormat ->GetFormatHeight(yLoc);//我去掉的2013.8.8
				//spFormat ->GetFormatWidth(xLoc);//我去掉的2013.8.8
				//xLoc = xLoc/2;//我去掉的2013.8.8
				//yLoc = yLoc/2;//我去掉的2013.8.8
				
			}
		}
		piDftFormats->Release();
		piDftFormats=NULL;
	}

	//每种标准对应的Format不同，下面是ISO对应的Format
	//  Format  A0 ISO
	//	Format  A1 ISO
	//	Format  A2 ISO
	//	Format  A3 ISO
	//	Format  A4 ISO
	//	Format  B4 ISO
	//	Format  B5 ISO
	//	Format  C5 ISO

	// d: Adds the format to the current sheet.
	CATIDftSheet *piDftSheet = NULL;
	CATIDftSheetFormat *piDftSheetFormat = NULL;

	// Gets active sheet.
	if (SUCCEEDED(piDftDrawingTemp->GetActiveSheet(&piDftSheet)))
	{
		CATIDftSheetFormat *piDftSheetFormat = NULL;
		if (piDftSheet && SUCCEEDED(piDftSheet->QueryInterface (IID_CATIDftSheetFormat,(void **)&piDftSheetFormat)))
		{
			if (FAILED(piDftSheetFormat->SetSheetFormat(myFormatName)))
			{
				piDftSheetFormat->Release();
				piDftSheetFormat=NULL;
				piDftSheet->Release();
				piDftSheet=NULL;
				piDftDrawingTemp->Release();
				piDftDrawingTemp=NULL;
				CATDocumentServices::Remove (*pDrawingDoc);
				return E_FAIL;
			}
			piDftSheetFormat->Release();
			piDftSheetFormat=NULL;
		}
 		piDftSheet->Release();
 		piDftSheet = NULL;
	}

	// e: save the drawing document
	hr = CATDocumentServices::SaveAs(*pDrawingDoc, storePath);
	if (FAILED(hr))
	{
		CATDocumentServices::Remove (*pDrawingDoc);
		return E_FAIL;
	}   
	CATDocumentServices::Remove (*pDrawingDoc);

	piDftDrawingTemp->Release(); piDftDrawingTemp=NULL;
	spDrwContTemp ->Release(); spDrwContTemp = NULL;

	return S_OK;
}


//*****************************************************
//功能：打开drafting document
//*****************************************************
HRESULT OpenDrawingDoc(CATUnicodeString storePath, CATDocument*& pDrawDoc)
{
	//MessageBox(NULL, L"OpenDrawingDoc", L"Test", FALSE);
	//cout << endl;
	//cout << "DialogboxBasedCommandDrawing::OpenDrawingDoc begin" << endl;

	/***********************************************************************/

	CATSession *pSampleSession = NULL;
	HRESULT hr = GetPtrSession(pSampleSession);
	if (FAILED(hr)) return E_FAIL;

	CATIIniInteractiveSession *piIntSession = NULL;
	hr = pSampleSession ->QueryInterface(IID_CATIIniInteractiveSession, (void **)&piIntSession);
	if(FAILED(hr))  return E_FAIL;

	CATIEditor * piEditor = NULL ;
	hr = piIntSession ->Open(storePath, FALSE, &piEditor);
	if(FAILED(hr))  return E_FAIL;

	CATFrmEditor *pEditor = piEditor ->GetEditor();
	pDrawDoc = pEditor ->GetDocument();

	return S_OK;
}




CATIMechanicalTool_var GetBodyByName(CATIPrtPart_var ispPrtPart,CATUnicodeString iUSBodyName)//通过名字得到GSD，返回该GSD的SpecObject类型
{
	CATLISTV(CATBaseUnknown_var) surfBodies; 
	surfBodies.RemoveAll();

	CATIPartRequest *pPartAsRequest = 0;
	HRESULT rc = ispPrtPart->QueryInterface(IID_CATIPartRequest, (void**)&pPartAsRequest) ;

	rc = pPartAsRequest->GetSolidBodies("MfDefault3DView",surfBodies);
	CATISpecObject_var ospSpecOnGSD = NULL_var;
	int sizeofGSD = surfBodies.Size();
	if(sizeofGSD<1)
	{
		cout<<"Error in Get GSD Of Current Part!"<<endl;
		return NULL_var;
	}
	for(int i = 1;i<=sizeofGSD;i++)
	{
		CATISpecObject_var spSpecOnGSD = surfBodies[i] ;
		CATIAlias_var AliasofGSDName=spSpecOnGSD;	
		CATUnicodeString NameOfGSD=AliasofGSDName->GetAlias();
		if(NameOfGSD ==iUSBodyName)
		{
			ospSpecOnGSD=spSpecOnGSD;	
			break;
		}
	}	

	surfBodies.RemoveAll();


	CATIMechanicalTool_var spForReturn = ospSpecOnGSD;

	return spForReturn;

}

BOOL InsertPCWithIndexWithParam(CATIPrtPart_var ispPrtPart,CATIDescendants_var spNewDes,CATUnicodeString PCFilePathName,int iIndex,CATUnicodeString USParamName1)
{
	//得到当前文件下的INPUT GSD
	CATLISTV(CATISpecObject_var) ispListInput;
	ispListInput.RemoveAll();
	spNewDes->GetDirectChildren("CATISpecObject",ispListInput);

	CATISpecObject_var spSpecPrtPartNew = ispPrtPart;   
	CATBaseUnknown_var spBaseUnknownPrtPartNew = spSpecPrtPartNew ;
	CATPathElement PathNewPart(spBaseUnknownPrtPartNew); 


	CATTry
	{
		CATDocument*  oDocContainPC = NULL;
		HRESULT hr = CATDocumentServices::OpenDocument(PCFilePathName,oDocContainPC); 
		if(oDocContainPC == NULL)
		{
			cout<<"Error in oDocContainPC!"<<endl;
			//::MessageBox(NULL,L"请确定选择PC文件的路径是否正确",L"Error",MB_OK);
			cout<<"PC Path Error,Error in Insert PC"<<endl;
			return FALSE;
		}
		//锁定文件
		::CATLockDocument(*oDocContainPC);

		CATInit *pInitOnDoc2G = NULL ;
		hr = oDocContainPC->QueryInterface(IID_CATInit,(void **) &pInitOnDoc2G);
		if ( FAILED(hr) )return 1 ;
		CATIPrtContainer *pIPrtContOnUDFDoc2G= NULL ;
		pIPrtContOnUDFDoc2G= (CATIPrtContainer*)pInitOnDoc2G->GetRootContainer("CATIPrtContainer");
		if ( NULL == pIPrtContOnUDFDoc2G) return 1 ; 

		pInitOnDoc2G->Release();
		pInitOnDoc2G = NULL ;  
		CATIPrtPart_var spPrtPartOnUDFPart2G = pIPrtContOnUDFDoc2G->GetPart();

		CATIUdfFactory *pIUdfFactory = NULL ;
		hr = pIPrtContOnUDFDoc2G->QueryInterface(IID_CATIUdfFactory,(void **) &pIUdfFactory);
		pIPrtContOnUDFDoc2G->Release();
		pIPrtContOnUDFDoc2G = NULL;

		CATListValCATISpecObject_var* pListPowerCopy = NULL;
		pListPowerCopy = pIUdfFactory->GetPowerCopyList( );
		int SizeOfPowerCopy = pListPowerCopy->Size();
		if(SizeOfPowerCopy == 0)
		{
			return FALSE;
		}
		CATIUdfInstantiate * pIUdfInstantiate = NULL ;
		if( (NULL != pListPowerCopy) &&(NULL_var != (*pListPowerCopy)[iIndex]) )
		{
			hr = (*pListPowerCopy)[iIndex]->QueryInterface(IID_CATIUdfInstantiate,(void **)&pIUdfInstantiate);
			if ( SUCCEEDED(hr) )
			{
				delete pListPowerCopy ;
				pListPowerCopy = NULL ;
			}
		}
		pIUdfFactory->Release();
		pIUdfFactory = NULL ;

		CATPathElement * FirstUIactiveObject1 = NULL ;
		CATBaseUnknown_var FirstDest1 = NULL_var ; 
		if(PathNewPart == NULL)
		{
			cout<<"Error in PathNewPart"<<endl;
			return FALSE;
		}

		hr = pIUdfInstantiate->SetDestinationPath(&PathNewPart,FirstUIactiveObject1,FirstDest1);  
		if(FAILED(hr))
		{
			cout<<"Error in SetDestinationPath!"<<endl;
			return FALSE;
		}
		if ( NULL != FirstUIactiveObject1 ) 
		{
			FirstUIactiveObject1->Release();
			FirstUIactiveObject1 = NULL ;
		}


		//直接赋值
		for(int i = 1;i<=ispListInput.Size();i++)
		{ 
			CATBaseUnknown_var spBaseOldInput = pIUdfInstantiate->GetOldInput(i);
			CATBaseUnknown *pInputLine1 = NULL;
			hr = ispListInput[i]->QueryInterface(IID_CATBaseUnknown,(void**)&pInputLine1);
			CATPathElement * pInput1 = new CATPathElement(pInputLine1);
			pInputLine1->Release();
			pInputLine1 = NULL;

			hr = pIUdfInstantiate->SetNewInput(i, pInput1);
			pInput1->Release();
			pInput1 = NULL;
		}

		CATListValCATBaseUnknown_var * pParameterList = NULL;
		CATListOfCATUnicodeString * pParameterRoleList = NULL ;
		hr = pIUdfInstantiate->GetParameters(pParameterList,pParameterRoleList);
		if ( NULL != pParameterList )
		{
			int NbPublishedParameters = pParameterList->Size();
			for ( int i= 1 ; i <= NbPublishedParameters ; i++ )
			{
				CATICkeParm_var spCkeParmOnParameter = (*pParameterList)[i];
				//CATIAlias_var spAliasCKE = spCkeParmOnParameter;

				/*cout<<"spCkeParmOnParameter->InternalRole()  "<<spCkeParmOnParameter->InternalRole()<<endl;
				if(spCkeParmOnParameter->InternalRole() == "U-TYPE")
				{
					cout<<"Matched"<<endl;*/
					spCkeParmOnParameter->Valuate(USParamName1);
				/*}*/
			}
		}


		hr = pIUdfInstantiate->Instantiate(spBaseUnknownPrtPartNew);
		if(FAILED(hr))
		{
			cout<<"Error in Instantiate!"<<endl;
		}
		if (pIUdfInstantiate != NULL)
		{
			hr = pIUdfInstantiate->EndInstantiate();	
			pIUdfInstantiate->Reset();
		}

		::CATUnLockDocument(*oDocContainPC);
		CATDocumentServices::Remove(*oDocContainPC);

		if (pIUdfInstantiate != NULL)
		{
			pIUdfInstantiate->Release(); 
			pIUdfInstantiate=NULL;
		}
	}
	CATCatch(CATMfErrUpdate,pErrorUpdate)
	{
		cout<<"Can  not update!"<<endl;
		return FALSE;

	}
	CATCatch(CATError,pError)
	{
		cout<<"Can  not update!"<<endl;
		return FALSE;
	}
	CATCatchOthers
	{
		cout<<"Can  not update!"<<endl;
		return FALSE;
	}
	CATEndTry;



	return TRUE;

}

BOOL InsertPCWithIndex(CATIPrtPart_var ispPrtPart,CATIDescendants_var spNewDes,CATUnicodeString PCFilePathName,int iIndex)
{
	//得到当前文件下的INPUT GSD
	CATLISTV(CATISpecObject_var) ispListInput;
	ispListInput.RemoveAll();
	spNewDes->GetDirectChildren("CATISpecObject",ispListInput);

	CATISpecObject_var spSpecPrtPartNew = ispPrtPart;   
	CATBaseUnknown_var spBaseUnknownPrtPartNew = spSpecPrtPartNew ;
	CATPathElement PathNewPart(spBaseUnknownPrtPartNew); 


	CATTry
	{
		CATDocument*  oDocContainPC = NULL;
		HRESULT hr = CATDocumentServices::OpenDocument(PCFilePathName,oDocContainPC); 
		if(oDocContainPC == NULL)
		{
			cout<<"Error in oDocContainPC!"<<endl;
			//::MessageBox(NULL,L"请确定选择PC文件的路径是否正确",L"Error",MB_OK);
			cout<<"PC Path Error,Error in Insert PC"<<endl;
			return FALSE;
		}
		//锁定文件
		::CATLockDocument(*oDocContainPC);

		CATInit *pInitOnDoc2G = NULL ;
		hr = oDocContainPC->QueryInterface(IID_CATInit,(void **) &pInitOnDoc2G);
		if ( FAILED(hr) )return 1 ;
		CATIPrtContainer *pIPrtContOnUDFDoc2G= NULL ;
		pIPrtContOnUDFDoc2G= (CATIPrtContainer*)pInitOnDoc2G->GetRootContainer("CATIPrtContainer");
		if ( NULL == pIPrtContOnUDFDoc2G) return 1 ; 

		pInitOnDoc2G->Release();
		pInitOnDoc2G = NULL ;  
		CATIPrtPart_var spPrtPartOnUDFPart2G = pIPrtContOnUDFDoc2G->GetPart();

		CATIUdfFactory *pIUdfFactory = NULL ;
		hr = pIPrtContOnUDFDoc2G->QueryInterface(IID_CATIUdfFactory,(void **) &pIUdfFactory);
		pIPrtContOnUDFDoc2G->Release();
		pIPrtContOnUDFDoc2G = NULL;

		CATListValCATISpecObject_var* pListPowerCopy = NULL;
		pListPowerCopy = pIUdfFactory->GetPowerCopyList( );
		int SizeOfPowerCopy = pListPowerCopy->Size();
		if(SizeOfPowerCopy == 0)
		{
			return FALSE;
		}
		CATIUdfInstantiate * pIUdfInstantiate = NULL ;
		if( (NULL != pListPowerCopy) &&(NULL_var != (*pListPowerCopy)[iIndex]) )
		{
			hr = (*pListPowerCopy)[iIndex]->QueryInterface(IID_CATIUdfInstantiate,(void **)&pIUdfInstantiate);
			if ( SUCCEEDED(hr) )
			{
				delete pListPowerCopy ;
				pListPowerCopy = NULL ;
			}
		}
		pIUdfFactory->Release();
		pIUdfFactory = NULL ;

		CATPathElement * FirstUIactiveObject1 = NULL ;
		CATBaseUnknown_var FirstDest1 = NULL_var ; 
		if(PathNewPart == NULL)
		{
			cout<<"Error in PathNewPart"<<endl;
			return FALSE;
		}

		hr = pIUdfInstantiate->SetDestinationPath(&PathNewPart,FirstUIactiveObject1,FirstDest1);  
		if(FAILED(hr))
		{
			cout<<"Error in SetDestinationPath!"<<endl;
			return FALSE;
		}
		if ( NULL != FirstUIactiveObject1 ) 
		{
			FirstUIactiveObject1->Release();
			FirstUIactiveObject1 = NULL ;
		}


		//直接赋值
		for(int i = 1;i<=ispListInput.Size();i++)
		{ 
			CATBaseUnknown_var spBaseOldInput = pIUdfInstantiate->GetOldInput(i);
			CATBaseUnknown *pInputLine1 = NULL;
			hr = ispListInput[i]->QueryInterface(IID_CATBaseUnknown,(void**)&pInputLine1);
			CATPathElement * pInput1 = new CATPathElement(pInputLine1);
			pInputLine1->Release();
			pInputLine1 = NULL;

			hr = pIUdfInstantiate->SetNewInput(i, pInput1);
			pInput1->Release();
			pInput1 = NULL;
		}

		
		hr = pIUdfInstantiate->Instantiate(spBaseUnknownPrtPartNew);
		if(FAILED(hr))
		{
			cout<<"Error in Instantiate!"<<endl;
		}
		if (pIUdfInstantiate != NULL)
		{
			hr = pIUdfInstantiate->EndInstantiate();	
			pIUdfInstantiate->Reset();
		}
	
		::CATUnLockDocument(*oDocContainPC);
		CATDocumentServices::Remove(*oDocContainPC);

		if (pIUdfInstantiate != NULL)
		{
			pIUdfInstantiate->Release(); 
			pIUdfInstantiate=NULL;
		}
	}
	CATCatch(CATMfErrUpdate,pErrorUpdate)
	{
		cout<<"Can  not update!"<<endl;
		return FALSE;

	}
	CATCatch(CATError,pError)
	{
		cout<<"Can  not update!"<<endl;
		return FALSE;
	}
	CATCatchOthers
	{
		cout<<"Can  not update!"<<endl;
		return FALSE;
	}
	CATEndTry;



	return TRUE;

}


BOOL InsertPCWithIndex2(CATIPrtPart_var ispPrtPart,CATLISTV(CATISpecObject_var) ispListInput,CATUnicodeString PCFilePathName,int iIndex)
{
	//得到当前文件下的INPUT GSD

	CATISpecObject_var spSpecPrtPartNew = ispPrtPart;   
	CATBaseUnknown_var spBaseUnknownPrtPartNew = spSpecPrtPartNew ;
	CATPathElement PathNewPart(spBaseUnknownPrtPartNew); 


	CATTry
	{
		CATDocument*  oDocContainPC = NULL;
		HRESULT hr = CATDocumentServices::OpenDocument(PCFilePathName,oDocContainPC); 
		if(oDocContainPC == NULL)
		{
			cout<<"Error in oDocContainPC!"<<endl;
			//::MessageBox(NULL,L"请确定选择PC文件的路径是否正确",L"Error",MB_OK);
			cout<<"PC Path Error,Error in Insert PC"<<endl;
			return FALSE;
		}
		//锁定文件
		::CATLockDocument(*oDocContainPC);

		CATInit *pInitOnDoc2G = NULL ;
		hr = oDocContainPC->QueryInterface(IID_CATInit,(void **) &pInitOnDoc2G);
		if ( FAILED(hr) )return 1 ;
		CATIPrtContainer *pIPrtContOnUDFDoc2G= NULL ;
		pIPrtContOnUDFDoc2G= (CATIPrtContainer*)pInitOnDoc2G->GetRootContainer("CATIPrtContainer");
		if ( NULL == pIPrtContOnUDFDoc2G) return 1 ; 

		pInitOnDoc2G->Release();
		pInitOnDoc2G = NULL ;  
		CATIPrtPart_var spPrtPartOnUDFPart2G = pIPrtContOnUDFDoc2G->GetPart();

		CATIUdfFactory *pIUdfFactory = NULL ;
		hr = pIPrtContOnUDFDoc2G->QueryInterface(IID_CATIUdfFactory,(void **) &pIUdfFactory);
		pIPrtContOnUDFDoc2G->Release();
		pIPrtContOnUDFDoc2G = NULL;

		CATListValCATISpecObject_var* pListPowerCopy = NULL;
		pListPowerCopy = pIUdfFactory->GetPowerCopyList( );
		int SizeOfPowerCopy = pListPowerCopy->Size();
		if(SizeOfPowerCopy == 0)
		{
			return FALSE;
		}
		CATIUdfInstantiate * pIUdfInstantiate = NULL ;
		if( (NULL != pListPowerCopy) &&(NULL_var != (*pListPowerCopy)[iIndex]) )
		{
			hr = (*pListPowerCopy)[iIndex]->QueryInterface(IID_CATIUdfInstantiate,(void **)&pIUdfInstantiate);
			if ( SUCCEEDED(hr) )
			{
				delete pListPowerCopy ;
				pListPowerCopy = NULL ;
			}
		}
		pIUdfFactory->Release();
		pIUdfFactory = NULL ;

		CATPathElement * FirstUIactiveObject1 = NULL ;
		CATBaseUnknown_var FirstDest1 = NULL_var ; 
		if(PathNewPart == NULL)
		{
			cout<<"Error in PathNewPart"<<endl;
			return FALSE;
		}

		hr = pIUdfInstantiate->SetDestinationPath(&PathNewPart,FirstUIactiveObject1,FirstDest1);  
		if(FAILED(hr))
		{
			cout<<"Error in SetDestinationPath!"<<endl;
			return FALSE;
		}
		if ( NULL != FirstUIactiveObject1 ) 
		{
			FirstUIactiveObject1->Release();
			FirstUIactiveObject1 = NULL ;
		}


		//直接赋值
		for(int i = 1;i<=ispListInput.Size();i++)
		{ 
			CATBaseUnknown_var spBaseOldInput = pIUdfInstantiate->GetOldInput(i);
			CATBaseUnknown *pInputLine1 = NULL;
			hr = ispListInput[i]->QueryInterface(IID_CATBaseUnknown,(void**)&pInputLine1);
			CATPathElement * pInput1 = new CATPathElement(pInputLine1);
			pInputLine1->Release();
			pInputLine1 = NULL;

			hr = pIUdfInstantiate->SetNewInput(i, pInput1);
			pInput1->Release();
			pInput1 = NULL;
		}


		hr = pIUdfInstantiate->Instantiate(spBaseUnknownPrtPartNew);
		if(FAILED(hr))
		{
			cout<<"Error in Instantiate!"<<endl;
		}
		if (pIUdfInstantiate != NULL)
		{
			hr = pIUdfInstantiate->EndInstantiate();	
			pIUdfInstantiate->Reset();
		}

		::CATUnLockDocument(*oDocContainPC);
		CATDocumentServices::Remove(*oDocContainPC);

		if (pIUdfInstantiate != NULL)
		{
			pIUdfInstantiate->Release(); 
			pIUdfInstantiate=NULL;
		}
	}
	CATCatch(CATMfErrUpdate,pErrorUpdate)
	{
		cout<<"Can  not update!"<<endl;
		return FALSE;

	}
	CATCatch(CATError,pError)
	{
		cout<<"Can  not update!"<<endl;
		return FALSE;
	}
	CATCatchOthers
	{
		cout<<"Can  not update!"<<endl;
		return FALSE;
	}
	CATEndTry;



	return TRUE;

}

HRESULT GetSpecObjShowAttr(CATISpecObject_var iSpecObj,CATBoolean &iShowOrHide)
{
	if (NULL_var==iSpecObj) return E_FAIL;

	CATIVisProperties *piVisP=NULL;
	HRESULT rc = iSpecObj->QueryInterface(IID_CATIVisProperties,(void**)&piVisP);
	if( FAILED(rc) || piVisP == NULL ) return rc;

	CATVisPropertiesValues Attribut;
	piVisP->GetPropertiesAtt(Attribut,CATVPShow);
	piVisP->Release();
	piVisP=NULL;

	CATShowAttribut oShow;
	Attribut.GetShowAttr(oShow);
	if (oShow == CATShowAttr)
	{
		iShowOrHide = TRUE;
	}
	else
	{
		iShowOrHide = FALSE;
	}

	return S_OK;
}

CATISpecObject_var GetElementByNameInGSD(CATIDescendants_var ispSpecGSD,CATUnicodeString iUSGSD)
{
	CATISpecObject_var ospForReturn = NULL_var;
	for (int i = 1;i<=ispSpecGSD->GetNumberOfChildren();i++)
	{
		CATIAlias_var spAlias = ispSpecGSD->GetChildAtPosition(i);
		//cout<<"Ele name is "<<spAlias->GetAlias()<<endl;
		if (spAlias->GetAlias() == iUSGSD)
		{
			ospForReturn = ispSpecGSD->GetChildAtPosition(i);
			break;
		}

	}


	return ospForReturn;

}

CATMathPlane GetProjectionPlaneForIsometricView()
{
	CATFrmLayout * pCurrentLayout= CATFrmLayout::GetCurrentLayout();
	CATFrmWindow * pCurrentWindow = pCurrentLayout->GetCurrentWindow();
	CATViewer *pCurrentViewer = pCurrentWindow->GetViewer();

	CAT3DViewpoint &viewPoint =pCurrentViewer->GetMain3DViewpoint();
	CATMathPointf     originF   = viewPoint.GetOrigin();
	CATMathDirectionf sightDirF = viewPoint.GetSightDirection();
	CATMathDirectionf upDirF    = viewPoint.GetUpDirection();

	CATMathDirection normalDir;
	sightDirF.GetValue(normalDir);

	CATMathPoint origin;
	originF.GetValue(origin);

	CATMathDirection UDir;
	CATMathDirection VDir;
	upDirF.GetValue(UDir);
	VDir = normalDir^UDir;

	CATMathPlane AxonometricDrawingPlane(origin, UDir, VDir);
	return AxonometricDrawingPlane;

}


HRESULT SetSpecObjectColor(CATISpecObject_var iSpecObj,int iColorR,int iColorG,int iColorB,int iOpacity)
{
	CATIVisProperties *piGraphProp=NULL;
	HRESULT rc = iSpecObj->QueryInterface(IID_CATIVisProperties,(void**)&piGraphProp);
	if( FAILED(rc) || piGraphProp == NULL ) return rc;

	CATVisPropertiesValues Attribut;
	Attribut.SetColor(iColorR, iColorG, iColorB);
	//Attribut.SetOpacity(iOpacity);

	piGraphProp->SetPropertiesAtt(Attribut,CATVPColor,CATVPMesh,0,0);
	//piGraphProp->SetPropertiesAtt(Attribut,CATVPOpacity,CATVPMesh,0,0);
	piGraphProp->Release();
	piGraphProp=NULL;

	CATIModelEvents *piME = NULL;
	rc = iSpecObj->QueryInterface( IID_CATIModelEvents,(void **) &piME);
	if( FAILED(rc) || piME == NULL ) return rc;
	CATModifyVisProperties notif(iSpecObj, CATPathElement(iSpecObj), CATVPGlobalType,CATVPColor,Attribut);
	piME->Dispatch(notif);
	piME->Release();
	piME= NULL ;

	return rc;
}
HRESULT SetCurveColor(CATISpecObject_var iSpecObj,int iColorR,int iColorG,int iColorB,int iOpacity)
{
	CATIVisProperties *piGraphProp=NULL;
	HRESULT rc = iSpecObj->QueryInterface(IID_CATIVisProperties,(void**)&piGraphProp);
	if( FAILED(rc) || piGraphProp == NULL ) return rc;

	CATVisPropertiesValues Attribut;
	Attribut.SetColor(iColorR, iColorG, iColorB);
	//Attribut.SetOpacity(iOpacity);

	piGraphProp->SetPropertiesAtt(Attribut,CATVPColor,CATVPLine,0,0);
	//piGraphProp->SetPropertiesAtt(Attribut,CATVPOpacity,CATVPMesh,0,0);
	piGraphProp->Release();
	piGraphProp=NULL;

	CATIModelEvents *piME = NULL;
	rc = iSpecObj->QueryInterface( IID_CATIModelEvents,(void **) &piME);
	if( FAILED(rc) || piME == NULL ) return rc;
	CATModifyVisProperties notif(iSpecObj, CATPathElement(iSpecObj), CATVPGlobalType,CATVPColor,Attribut);
	piME->Dispatch(notif);
	piME->Release();
	piME= NULL ;

	return rc;
}

//特征化Cell

CATBoolean CopyPasteOperation(CATLISTV(CATISpecObject_var) ispListElements,CATISpecObject_var ispDestinatioin)
{
	CATMmrInterPartCopy *  ptCATMmrInterPartCopy = NULL;
	for(int i=1;i<=ispListElements.Size();i++)
	{
		CATISpecObject_var SourceToCopy = ispListElements[i] ;
		if(SourceToCopy == NULL_var)
		{
			cout<<"Error in SourceToCopy"<<endl;
			return FALSE;
		}
		ptCATMmrInterPartCopy =  new CATMmrInterPartCopy (SourceToCopy,ispDestinatioin);
		if(ptCATMmrInterPartCopy == NULL)
		{
			cout<<"Error in ptCATMmrInterPartCopy"<<endl;
			return FALSE;
		}
		ptCATMmrInterPartCopy ->SetAttributeMode(3); 

		ptCATMmrInterPartCopy ->SetLinkMode(FALSE) ;   
		HRESULT hr = ptCATMmrInterPartCopy ->Run();

		CATISpecObject_var Result ;
		hr = ptCATMmrInterPartCopy ->GetResult(Result);
		if(Result == NULL_var)
		{
			cout<<"Error in Result"<<endl;
			return FALSE;
		}
		else
		{
			Result->Update();
			CATIAlias_var spAliasAfterPaste = Result;
			spAliasAfterPaste->SetAlias(CATIAlias_var(SourceToCopy)->GetAlias());

			//获取复制前的颜色
			unsigned int iRed,iGreen,iBlue;
			GetLineColor(SourceToCopy,iRed,iGreen,iBlue);
			SetSpecObjectColor(Result,iRed,iGreen,iBlue,0);
		}



	}
	ptCATMmrInterPartCopy = NULL;


	return TRUE;
}


//CATLISTV(CATISpecObject_var)  CopyPasteOperationV2(CATLISTV(CATISpecObject_var) ispListElements,CATISpecObject_var ispDestinatioin)
//{
//	CATLISTV(CATISpecObject_var)  spListForReturn;
//	spListForReturn.RemoveAll();
//	CATMmrInterPartCopy *  ptCATMmrInterPartCopy = NULL;
//	for(int i=1;i<=ispListElements.Size();i++)
//	{
//		CATISpecObject_var SourceToCopy = ispListElements[i] ;
//		if(SourceToCopy == NULL_var)
//		{
//			cout<<"Error in SourceToCopy"<<endl;
//			return FALSE;
//		}
//		ptCATMmrInterPartCopy =  new CATMmrInterPartCopy (SourceToCopy,ispDestinatioin);
//		if(ptCATMmrInterPartCopy == NULL)
//		{
//			cout<<"Error in ptCATMmrInterPartCopy"<<endl;
//			return FALSE;
//		}
//		ptCATMmrInterPartCopy ->SetAttributeMode(3); 
//
//		ptCATMmrInterPartCopy ->SetLinkMode(FALSE) ;   
//		HRESULT hr = ptCATMmrInterPartCopy ->Run();
//
//		CATISpecObject_var Result ;
//		hr = ptCATMmrInterPartCopy ->GetResult(Result);
//		if(Result == NULL_var)
//		{
//			cout<<"Error in Result"<<endl;
//			return FALSE;
//		}
//		else
//		{
//			Result->Update();
//			CATIAlias_var spAliasAfterPaste = Result;
//			spAliasAfterPaste->SetAlias(CATIAlias_var(SourceToCopy)->GetAlias());
//			spListForReturn.Append(Result);
//
//			//获取复制前的颜色
//			unsigned int iRed,iGreen,iBlue;
//			GetLineColor(SourceToCopy,iRed,iGreen,iBlue);
//			SetSpecObjectColor(Result,iRed,iGreen,iBlue,0);
//		}
//
//
//
//	}
//	ptCATMmrInterPartCopy = NULL;
//
//
//	return spListForReturn;
//}

CATISpecObject_var FeatulizeCell(CATCell_var ipCell,CATIPrtContainer_var ispPrtContainer,CATIGeometricalElement_var  spGeoBoundary)
{

	CATCell_var spCellBoundary = ipCell;
	CATIBRepAccess_var spBRepAccess= CATBRepDecode(spCellBoundary,spGeoBoundary);			

	CATIFeaturize_var spFratureCell = spBRepAccess;			 
	CATISpecObject_var spObjectFromCell = spFratureCell->FeaturizeR(MfPermanentBody);

	CATIMfBRep_var spBRepOnFeature = spObjectFromCell; 
	CATBody_var spBodyFromObject=spBRepOnFeature->GetBody();


	CATIDatumFactory_var spDatumFactry = ispPrtContainer;

	CATISpecObject *piObjectFromBody=NULL;
	spDatumFactry->InstanciateDatum(spBodyFromObject,piObjectFromBody);
	CATISpecObject_var spObjectFromBody=piObjectFromBody;
	spObjectFromBody->Update();

	return spObjectFromBody;
}


double GetSurfaceArea(CATISpecObject_var iSpecObj)
{
	CATIMeasurableSurface_var spMeaSurface = iSpecObj;
	double dArea = 0.0;
	if (spMeaSurface != NULL_var)
	{
		spMeaSurface->GetArea(dArea);
	}
	return dArea;

}


CATCell*  GetSurfaceBottomLineCell(CATISpecObject_var  ispSpecSurface)
{
	CATCell* ospSpecTopLine = NULL;

	ispSpecSurface->Update();
	double dRef = 15000000.0;
	int iNeed = 0;
	CATIGeometricalElement_var  spGeoBoundary = ispSpecSurface;
	CATBody_var spBodyBoundary = spGeoBoundary->GetBodyResult();	
	CATLISTP(CATCell) oListCells;
	oListCells.RemoveAll();
	spBodyBoundary->GetAllCells(oListCells,1);

	for (int i = 1;i<=oListCells.Size();i++)
	{
		CATEdge_var spEdge1 = oListCells[i];

		CATVertex * pStart = NULL;
		CATVertex * pEnd = NULL;
		spEdge1->GetVertices(&pStart,&pEnd);


		CATPoint* pPt11 = pStart->GetPoint();
		CATPoint* pPt12 = pEnd->GetPoint();

		double  dzzz = (pPt11->GetZ()+pPt12->GetZ())/2;
		if(dzzz<=dRef)
		{
			dRef = dzzz ;
			ospSpecTopLine =  oListCells[i];
		}
	}


	//将这个特征实例化




	return ospSpecTopLine;
}


CATCell*  GetSurfaceTopLineCell(CATISpecObject_var  ispSpecSurface)
{
	CATCell* ospSpecTopLine = NULL;

	ispSpecSurface->Update();
	double dRef = -5000000.0;
	int iNeed = 0;
	CATIGeometricalElement_var  spGeoBoundary = ispSpecSurface;
	CATBody_var spBodyBoundary = spGeoBoundary->GetBodyResult();	
	CATLISTP(CATCell) oListCells;
	oListCells.RemoveAll();
	spBodyBoundary->GetAllCells(oListCells,1);

	for (int i = 1;i<=oListCells.Size();i++)
	{
		CATEdge_var spEdge1 = oListCells[i];

		CATVertex * pStart = NULL;
		CATVertex * pEnd = NULL;
		spEdge1->GetVertices(&pStart,&pEnd);


		CATPoint* pPt11 = pStart->GetPoint();
		CATPoint* pPt12 = pEnd->GetPoint();

		double  dzzz = (pPt11->GetZ()+pPt12->GetZ())/2;
		if(dzzz>=dRef)
		{
			dRef = dzzz ;
			ospSpecTopLine =  oListCells[i];
		}
	}


	//将这个特征实例化




	return ospSpecTopLine;
}

BOOL InsertPCWithIndexWithParamUseIndenticalName(CATIPrtPart_var ispPrtPart,CATIDescendants_var spNewDes,CATUnicodeString PCFilePathName,int iIndex )
{
	//得到当前文件下的INPUT GSD
	CATLISTV(CATISpecObject_var) ispListInput;
	ispListInput.RemoveAll();
	spNewDes->GetDirectChildren("CATISpecObject",ispListInput);

	CATISpecObject_var spSpecPrtPartNew = ispPrtPart;   
	CATBaseUnknown_var spBaseUnknownPrtPartNew = spSpecPrtPartNew ;
	CATPathElement PathNewPart(spBaseUnknownPrtPartNew); 


	CATTry
	{
		CATDocument*  oDocContainPC = NULL;
		HRESULT hr = CATDocumentServices::OpenDocument(PCFilePathName,oDocContainPC); 
		if(oDocContainPC == NULL)
		{
			cout<<"Error in oDocContainPC!"<<endl;
			//::MessageBox(NULL,L"请确定选择PC文件的路径是否正确",L"Error",MB_OK);
			cout<<"PC Path Error,Error in Insert PC"<<endl;
			return FALSE;
		}
		//锁定文件
		::CATLockDocument(*oDocContainPC);

		CATInit *pInitOnDoc2G = NULL ;
		hr = oDocContainPC->QueryInterface(IID_CATInit,(void **) &pInitOnDoc2G);
		if ( FAILED(hr) )return 1 ;
		CATIPrtContainer *pIPrtContOnUDFDoc2G= NULL ;
		pIPrtContOnUDFDoc2G= (CATIPrtContainer*)pInitOnDoc2G->GetRootContainer("CATIPrtContainer");
		if ( NULL == pIPrtContOnUDFDoc2G) return 1 ; 

		pInitOnDoc2G->Release();
		pInitOnDoc2G = NULL ;  
		CATIPrtPart_var spPrtPartOnUDFPart2G = pIPrtContOnUDFDoc2G->GetPart();

		CATIUdfFactory *pIUdfFactory = NULL ;
		hr = pIPrtContOnUDFDoc2G->QueryInterface(IID_CATIUdfFactory,(void **) &pIUdfFactory);
		pIPrtContOnUDFDoc2G->Release();
		pIPrtContOnUDFDoc2G = NULL;

		CATListValCATISpecObject_var* pListPowerCopy = NULL;
		pListPowerCopy = pIUdfFactory->GetPowerCopyList( );
		int SizeOfPowerCopy = pListPowerCopy->Size();
		if(SizeOfPowerCopy == 0)
		{
			return FALSE;
		}
		CATIUdfInstantiate * pIUdfInstantiate = NULL ;
		if( (NULL != pListPowerCopy) &&(NULL_var != (*pListPowerCopy)[iIndex]) )
		{
			hr = (*pListPowerCopy)[iIndex]->QueryInterface(IID_CATIUdfInstantiate,(void **)&pIUdfInstantiate);
			if ( SUCCEEDED(hr) )
			{
				delete pListPowerCopy ;
				pListPowerCopy = NULL ;
			}
		}
		pIUdfFactory->Release();
		pIUdfFactory = NULL ;

		CATPathElement * FirstUIactiveObject1 = NULL ;
		CATBaseUnknown_var FirstDest1 = NULL_var ; 
		if(PathNewPart == NULL)
		{
			cout<<"Error in PathNewPart"<<endl;
			return FALSE;
		}

		hr = pIUdfInstantiate->SetDestinationPath(&PathNewPart,FirstUIactiveObject1,FirstDest1);  
		if(FAILED(hr))
		{
			cout<<"Error in SetDestinationPath!"<<endl;
			return FALSE;
		}
		if ( NULL != FirstUIactiveObject1 ) 
		{
			FirstUIactiveObject1->Release();
			FirstUIactiveObject1 = NULL ;
		}
		CATBaseUnknown_var spCAAUdfModelPart = ispPrtPart ;
        pIUdfInstantiate->UseIdenticalName(spCAAUdfModelPart);
		


		hr = pIUdfInstantiate->Instantiate(spBaseUnknownPrtPartNew);
		if(FAILED(hr))
		{
			cout<<"Error in Instantiate!"<<endl;
		}
		if (pIUdfInstantiate != NULL)
		{
			hr = pIUdfInstantiate->EndInstantiate();	
			pIUdfInstantiate->Reset();
		}

		::CATUnLockDocument(*oDocContainPC);
		CATDocumentServices::Remove(*oDocContainPC);

		if (pIUdfInstantiate != NULL)
		{
			pIUdfInstantiate->Release(); 
			pIUdfInstantiate=NULL;
		}
	}
	CATCatch(CATMfErrUpdate,pErrorUpdate)
	{
		cout<<"Can  not update!"<<endl;
		return FALSE;

	}
	CATCatch(CATError,pError)
	{
		cout<<"Can  not update!"<<endl;
		return FALSE;
	}
	CATCatchOthers
	{
		cout<<"Can  not update!"<<endl;
		return FALSE;
	}
	CATEndTry;



	return TRUE;

}


void GetSurfacewoLongLineCell(CATISpecObject_var  ispSpecSurface,CATCell* &pCell1,CATCell* &pCell2)
{
	CATCell* ospSpecTopLine = NULL;

	ispSpecSurface->Update();
	double dRef = -5000000.0;
	int iNeed = 0;
	CATIGeometricalElement_var  spGeoBoundary = ispSpecSurface;
	CATBody_var spBodyBoundary = spGeoBoundary->GetBodyResult();	
	CATLISTP(CATCell) oListCells;
	oListCells.RemoveAll();
	spBodyBoundary->GetAllCells(oListCells,1);

	CATCell* pCell1Ref = NULL;
	CATCell* pCell2Ref = NULL;

	for (int i = 1;i<=oListCells.Size();i++)
	{
		CATEdge_var spEdge1 = oListCells[i];
		if (spEdge1->CalcLength()>dRef)
		{
			dRef = spEdge1->CalcLength();
			iNeed = i;
		}
	}
	pCell1Ref = oListCells[iNeed];


	dRef = -5000000.0;
	for (int i = 1;i<=oListCells.Size();i++)
	{
		if (oListCells[i] == pCell1Ref)
		{
			continue;
		}
		CATEdge_var spEdge1 = oListCells[i];
		if (spEdge1->CalcLength()>dRef)
		{
			dRef = spEdge1->CalcLength();
			iNeed = i;
		}
	}

	//iNeed = (iNeed+2)%4;
	//if (iNeed == 0)
	//{
	//	iNeed = 4;
	//}
	pCell2Ref = oListCells[iNeed];


	//将这个特征实例化
	//根据Z值，得出Z值最高的，为cell1
	CATEdge_var spEdge1 = pCell1Ref;
	CATEdge_var spEdge2 = pCell2Ref;

	CATVertex * pStart1 = NULL;
	CATVertex * pEnd1 = NULL;
	spEdge1->GetVertices(&pStart1,&pEnd1);


	CATPoint* pPt11 = pStart1->GetPoint();
	CATPoint* pPt12 = pEnd1->GetPoint();

	double  dzzz1 = (pPt11->GetZ()+pPt12->GetZ())/2;

	spEdge2->GetVertices(&pStart1,&pEnd1);
	pPt11 = pStart1->GetPoint();
	pPt12 = pEnd1->GetPoint();

	double  dzzz2 = (pPt11->GetZ()+pPt12->GetZ())/2;
	if (dzzz1>dzzz2)
	{
		pCell1 = pCell1Ref;
		pCell2 = pCell2Ref;
	}
	else
	{
		pCell2 = pCell1Ref;
		pCell1 = pCell2Ref;
	}






	return ;
}











void GetSurfacewoLongLineSpec(CATISpecObject_var  ispSpecSurface,CATISpecObject_var &spLong1,CATISpecObject_var &spLong2)
{
	CATISpecObject_var spSpecLong1Ref = NULL_var;
	CATISpecObject_var spSpecLong2Ref = NULL_var;

	ispSpecSurface->Update();
	CATISpecObject_var spSpecExtract = ExtractSurface(ispSpecSurface);
	if(spSpecExtract == NULL_var)
	{
		return ;
	}
	spSpecExtract->Update();

	//2、将面的边界Smooth
	CATISpecObject_var spSpecSmooth = SmoothBoundary2(spSpecExtract,0.4);
	if(spSpecSmooth == NULL_var)
	{
		return ;
	}
	spSpecSmooth->Update();

	CATListValCATISpecObject_var spListCell = TopoDisAssemble(spSpecSmooth);
	int iCellSize = spListCell.Size();
	/*if(iCellSize != 4)
	{
		return ;
	}*/

	//
	double dRef = -500.0;
	for (int i = 1;i<=spListCell.Size();i++)
	{
		CATIMeasurableCurve_var spMea1 = spListCell[i];
		if (spMea1 != NULL_var)
		{
			double dLength = 0.0;
			spMea1->GetLength(dLength);
			if (dLength>dRef)
			{
				dRef = dLength;
				spSpecLong1Ref = spListCell[i];
			}
		}
	}

	dRef = -500.0;
	for (int i = 1;i<=spListCell.Size();i++)
	{
		if (spListCell[i] == spSpecLong1Ref)
		{
			continue;
		}
		CATIMeasurableCurve_var spMea1 = spListCell[i];
		if (spMea1 != NULL_var)
		{
			double dLength = 0.0;
			spMea1->GetLength(dLength);
			if (dLength>dRef)
			{
				dRef = dLength;
				spSpecLong2Ref = spListCell[i];
			}
		}
	}

	//比较两条线的中点Z值
	CATIMeasurableCurve_var spMeaRef1 = spSpecLong1Ref;
	CATIMeasurableCurve_var spMeaRef2 = spSpecLong2Ref;
	CATMathPoint pStart,pMid1,pMid2,pEnd;
	spMeaRef1->GetPoints(pStart,pMid1,pEnd);
	spMeaRef2->GetPoints(pStart,pMid2,pEnd);
	if (pMid1.GetZ()>pMid2.GetZ())
	{
		spLong1 = spSpecLong1Ref;
		spLong2 = spSpecLong2Ref;
	}
	else
	{
		spLong2 = spSpecLong1Ref;
		spLong1 = spSpecLong2Ref;
	}


	return ;
}


void GetSurfacewoLongLineSpecForLize(CATISpecObject_var  ispSpecSurface,CATISpecObject_var &spLong1,CATISpecObject_var &spLong2)
{
	CATISpecObject_var spSpecLong1Ref = NULL_var;
	CATISpecObject_var spSpecLong2Ref = NULL_var;

	ispSpecSurface->Update();
	CATISpecObject_var spSpecExtract = ExtractSurface(ispSpecSurface);
	if(spSpecExtract == NULL_var)
	{
		return ;
	}
	spSpecExtract->Update();

	//2、将面的边界Smooth
	CATISpecObject_var spSpecSmooth = SmoothBoundary2(spSpecExtract,0.4);
	if(spSpecExtract == NULL_var)
	{
		return ;
	}
	spSpecSmooth->Update();

	CATListValCATISpecObject_var spListCell = TopoDisAssemble(spSpecSmooth);
	int iCellSize = spListCell.Size();


	//
	double dRef = -500.0;
	for (int i = 1;i<=spListCell.Size();i++)
	{
		CATIMeasurableCurve_var spMea1 = spListCell[i];
		if (spMea1 != NULL_var)
		{
			double dLength = 0.0;
			spMea1->GetLength(dLength);
			if (dLength>dRef)
			{
				dRef = dLength;
				spLong1 = spListCell[i];
			}
		}
	}

	dRef = -500.0;
	for (int i = 1;i<=spListCell.Size();i++)
	{
		if (spListCell[i] == spLong1)
		{
			continue;
		}
		CATIMeasurableCurve_var spMea1 = spListCell[i];
		if (spMea1 != NULL_var)
		{
			double dLength = 0.0;
			spMea1->GetLength(dLength);
			if (dLength>dRef)
			{
				dRef = dLength;
				spLong2 = spListCell[i];
			}
		}
	}


	return ;
}












double GetCurveMidPTCoord(CATISpecObject_var ispSpecLine,int iType)//iType为1 2 3分别代表X Y Z
{
	double dForReturn = 0.0;
	CATIMeasurableCurve_var spMea = ispSpecLine;
	if (spMea != NULL_var)
	{
		CATMathPoint pStart,pMid,pEnd;
		spMea->GetPoints(pStart,pMid,pEnd);
		if (iType == 1)
		{
			dForReturn = pMid.GetX();
		}
		else if (iType == 2)
		{
			dForReturn = pMid.GetY();
		}
		else if (iType == 3)
		{
			dForReturn = pMid.GetZ();
		}
	}
	return dForReturn;
};


CATISpecObject_var FillTriangle(CATISpecObject_var ispSpecTriangle,CATIDescendants_var ispSpecGSD)
{
	//填充这个面
	CATLISTV(CATISpecObject_var) ispListForFill;
	ispListForFill.RemoveAll();
	ispListForFill.Append(ispSpecTriangle);

	CATLISTV(CATISpecObject_var) ispListForSupport;
	ispListForSupport.RemoveAll();
	ispListForSupport.Append(NULL_var);


	CATISpecObject_var spSpecFill = spGSMFact->CreateFill(ispListForFill, ispListForSupport,CATGSMContinuity_Point); 
	ispSpecGSD->Append(spSpecFill);

	return spSpecFill;
}



double GetDistanceFromMathPTPT(CATISpecObject_var spSpecPT1,CATMathPoint iPt2)
{
	CATIMeasurablePoint_var spMeaPT1 = spSpecPT1;
	CATMathPoint iPt1;
	spMeaPT1->GetPoint(iPt1);

	

	double dDistance = iPt1.DistanceTo(iPt2);

	return dDistance;

}


double GetDistanceBetweenMeasurableMathPoint(CATISpecObject_var ispSpecLine,CATMathPoint pPT)
{
	double dDistance = -1.0;

	CATIMeasurableInContext_var spMea1 = ispSpecLine;
	CATMathPoint pMathPT2;
	CATMathAxis pMathAxis;

	spMea1->MinimumDistance(pPT,dDistance,pMathPT2);


	return dDistance;
}








void InsertPCAndCreateNewPartWithIndex(CATUnicodeString iusPathPC,CATUnicodeString iUSPartNumber,CATListValCATISpecObject_var  ispListForCopy,
											   CATUnicodeString USForPCOutputDirectory,CATUnicodeString USForPCOutputError,int iIndex)
{
	CATDocument* pNewDoc = NULL;
	pNewDoc = CreateNewPartFile();
	CATUnicodeString USPartNumber = iUSPartNumber;
	cout<<"USPartNumber  "<<USPartNumber<<endl;

	//设置文件的PartNumber
	BOOL bSetPartNumberofPartFile = SetPartNumberofPartFile(pNewDoc,USPartNumber);


	//得到Part文件的主要接口
	//得到这个新创建文件的相关接口
	CATInit *piInitOnDoc = NULL;
	HRESULT rc = pNewDoc -> QueryInterface (IID_CATInit,(void**) &piInitOnDoc);
	if (FAILED(rc))
	{
		cout << "ERROR in QueryInterface on CATInit for doc" << endl << flush;
		return ;
	}

	const CATIdent idCATIContainer = "CATIContainer";   
	CATIContainer *piRootContainer = NULL;
	piRootContainer = (CATIContainer*)piInitOnDoc -> GetRootContainer(idCATIContainer);
	if (NULL == piRootContainer)
	{
		cout << "ERROR in GetRootContainer" << endl << flush;
		return;
	}

	CATIPrtContainer_var spPartSpecCont = piRootContainer; 
	CATIPrtPart_var spPartNew = spPartSpecCont->GetPart();
	CATIMechanicalRootFactory_var spMechanicalRootFact = spPartSpecCont;



	//2、在PART中新建几何图形集INPUT
	CATIDescendants_var spNewDes = CreateNewGSD(spMechanicalRootFact,spPartNew,"INPUT" ,1);//iType = 0为几何体，1为集合图形集
	if(spNewDes == NULL_var)
	{
		cout<<"Error in spNewDes"<<endl;
		return;
	}



	//3、将元素copy到该几何图形集下 
	CATBoolean bCopyPasteOperation = CopyPasteOperation(ispListForCopy,CATISpecObject_var(spNewDes));

	cout<<"Begin to insert pc"<<endl;
	BOOL bInsertPC2 = InsertPCWithIndex(spPartNew,spNewDes,iusPathPC,iIndex);
	cout<<"End InsertPC"<<endl;

	CATLISTV(CATISpecObject_var) iiispListElementsForPC;
	iiispListElementsForPC.RemoveAll();
	spNewDes->GetDirectChildren("CATISpecObject",iiispListElementsForPC);
	HideElements(iiispListElementsForPC);

	////文件更新
	CATTry
	{

		CATISpecObject_var(spPartNew)->Update();
	}
	CATCatch(CATMfErrUpdate,pErrorUpdate)
	{
		//文件另存为
		cout<<"Can not Update"<<endl;
		CATUnicodeString USSaveName11 = USForPCOutputError+"\\"+USPartNumber+".CATPart";
		CATDocumentServices::SaveAs(*pNewDoc,USSaveName11);
		CATDocumentServices::Remove(*pNewDoc); 
		return;
	}
	CATEndTry;

	CATUnicodeString USSaveName = USForPCOutputDirectory+"\\"+USPartNumber+".CATPart";
	cout<<"USSaveName  "<<USSaveName<<endl;
	CATDocumentServices::SaveAs(*pNewDoc,USSaveName,"CATPart");
	CATDocumentServices::Remove(*pNewDoc);
}


CATMathPoint GetCOGOf(CATISpecObject_var ispSpec)
{
	CATMathPoint pPT;
	CATIMeasurableInContext_var spMea = ispSpec;
	if (spMea != NULL_var)
	{
		spMea->GetCOG(pPT);
	}
	return pPT;

}


void DeleteEmptyGSDs(CATISpecObject_var spSpecFather)
{
	CATListValCATISpecObject_var spListDirectGSDs;
	spListDirectGSDs.RemoveAll();
	CATIDescendants_var(spSpecFather)->GetAllChildren("CATIMmiNonOrderedGeometricalSet", spListDirectGSDs);
	for (int iSize = 1;iSize<=spListDirectGSDs.Size();iSize++)
	{
		CATIDescendants_var spDesEach = spListDirectGSDs[iSize];
		if (spDesEach->GetNumberOfChildren() == 0)
		{
			CATISpecObject_var spSpecEach = spDesEach;
			spSpecEach->GetFather()->Remove(spSpecEach);
		}
	}

}

void getFolderDayFolder(CString pathStr, CStringArray& arrStrFile)//找到所有的文件夹
{
	CString myDataPath,fdPath;
	myDataPath=pathStr + "\\*.*";
	CString strTmp;

	CFileFind find;
	BOOL bf = find.FindFile(myDataPath);
	while(bf)
	{    
		bf = find.FindNextFile();
		if(!find.IsDots())
		{
			fdPath=find.GetFilePath();
			if (find.IsDirectory())
			{
				//如果是文件夹，递归，继续往下找  
				arrStrFile.Add(fdPath);
				getFolderDayFolder(fdPath, arrStrFile);
			}
		}
	}
	find.Close();
}  
CATUnicodeString CStringToUS(CString iCString)
{
	if ("" == iCString) return FALSE;
	CATUnicodeString StrBSTR ;
	BSTR bstrText = iCString.AllocSysString();
	StrBSTR.BuildFromBSTR(bstrText);
	SysFreeString(bstrText); // 用完释放

	return StrBSTR;
}

CString USToCString(CATUnicodeString iUString)
{
	CATString sTemp = iUString.ConvertToChar();
	char *cTemp = sTemp.CastToCharPtr();
	CString csTemp = cTemp;
	return csTemp;
}



void getFolderDayFile(CString pathStr, CStringArray& arrStrFile)//找到所有的文件
{
	CString myDataPath,fdPath;
	myDataPath=pathStr + "\\*.*";
	CString strTmp;

	CFileFind find;
	BOOL bf = find.FindFile(myDataPath);
	while(bf)
	{                        
		bf = find.FindNextFile();
		if(!find.IsDots())
		{
			fdPath=find.GetFilePath();
			if (find.IsDirectory())
			{
				//如果是文件夹，递归，继续往下找  
				getFolderDayFile(fdPath, arrStrFile);
			}
			else
			{
				//如果是文件,判断是否是*.txt
				strTmp=fdPath.Right(8);
				CATUnicodeString USTmp = CStringToUS(strTmp);
				if (USTmp.SearchSubString("CATPart") != -1 )
				{
					arrStrFile.Add(fdPath);
				}
			}
		}
	}
	find.Close();
}    

void getFolderExcelFile(CString pathStr, CStringArray& arrStrFile)//找到所有的Excel文件
{
	CString myDataPath,fdPath;
	myDataPath=pathStr + "\\*.*";
	CString strTmp;

	CFileFind find;
	BOOL bf = find.FindFile(myDataPath);
	while(bf)
	{                        
		bf = find.FindNextFile();
		if(!find.IsDots())
		{
			fdPath=find.GetFilePath();
			if (find.IsDirectory())
			{
				//如果是文件夹，递归，继续往下找  
				getFolderExcelFile(fdPath, arrStrFile);
			}
			else
			{
				//如果是文件,判断是否是*.txt
				strTmp = fdPath.Right(5);
				CATUnicodeString USTmp = CStringToUS(strTmp);
				if (USTmp.SearchSubString("xlsx") != -1 )
				{
					arrStrFile.Add(fdPath);
				}
			}
		}
	}
	find.Close();
}   

//复制粘贴方法



BOOL ChangePrdInstName(CATIProduct_var spPrd,CATUnicodeString StrInstName)
{
	if(NULL_var==spPrd)
		return FALSE;

	//	
	CATIProduct_var spRefPrd =  spPrd->GetReferenceProduct( );
	if(NULL_var==spRefPrd)
		return FALSE;


	//	
	CATIProduct_var spFatherPrd = spPrd->GetFatherProduct();
	if(NULL_var==spFatherPrd)
		return FALSE;

	CATIProduct_var spFatherRefPrd = spFatherPrd->GetReferenceProduct();
	if(NULL_var==spFatherRefPrd)
		return FALSE;

	CATIProduct_var spFirstInstPrd = spPrd->FindInstance(spFatherRefPrd); 
	if(NULL_var==spFirstInstPrd)
		return FALSE;

	spFirstInstPrd->SetPrdInstanceName(StrInstName);

	return FALSE;
}


HRESULT AddNewExternalComponent(CATIProduct* iThisProd, const CATUnicodeString iDocumentType,const CATUnicodeString iPartNumber, CATIProduct** oNewProduct,CATDocument *&pNewDoc)
{
	if ( ! iThisProd ) return 1;

	HRESULT rc = E_FAIL;
	*oNewProduct = NULL;

	CATUnicodeString product = "Product";
	CATUnicodeString part = "Part";

	if ( (iDocumentType != product) &&  (iDocumentType != part) )
	{
		return rc;
	}

	rc = CATDocumentServices::New(iDocumentType,pNewDoc );
	if ( FAILED(rc) || (NULL==pNewDoc) )
	{
		cout << "ERROR : document Creation Failed" << endl<< flush;
		return rc;
	}

	if ( SUCCEEDED( rc ) )
	{
		CATIDocRoots *piDocRootsOnNewDoc = NULL;
		rc = pNewDoc->QueryInterface(IID_CATIDocRoots,(void**) &piDocRootsOnNewDoc);
		if ( SUCCEEDED( rc ) )
		{			
			CATListValCATBaseUnknown_var *pRootProducts =piDocRootsOnNewDoc->GiveDocRoots();
			CATIProduct_var spRootProduct;
			if (NULL != pRootProducts)
				if (pRootProducts->Size())
				{  
					spRootProduct = (*pRootProducts)[1];
					delete pRootProducts;
					pRootProducts = NULL;
				}

				piDocRootsOnNewDoc->Release();
				piDocRootsOnNewDoc=NULL;

				if (NULL_var != spRootProduct)
				{
					spRootProduct -> SetPartNumber(iPartNumber);
					CATIProduct_var spProduct;
					spProduct = iThisProd->AddProduct   (spRootProduct);
					if (NULL_var ==  spProduct ) return 2;
					rc = spProduct->QueryInterface(IID_CATIProduct, (void**) &*oNewProduct);



				}
		}
	}
	return rc; 

}



BOOL InsertPCWithIndex3(CATIPrtPart_var ispPrtPart,CATIDescendants_var spNewDes,CATLISTV(CATISpecObject_var) ispListInput,CATUnicodeString PCFilePathName,int iIndex)
{
	//得到当前文件下的INPUT GSD

	CATISpecObject_var spSpecPrtPartNew = ispPrtPart;   
	CATBaseUnknown_var spBaseUnknownPrtPartNew = spSpecPrtPartNew ;
	CATPathElement PathNewPart(spBaseUnknownPrtPartNew); 

	CATISpecObject_var spSpecGSDNew = spNewDes;   
	CATBaseUnknown_var spBaseUnknownGSDNew = spSpecGSDNew ;
	CATPathElement PathNewGSD(spBaseUnknownGSDNew); 


	CATTry
	{
		CATDocument*  oDocContainPC = NULL;
		HRESULT hr = CATDocumentServices::OpenDocument(PCFilePathName,oDocContainPC); 
		if(oDocContainPC == NULL)
		{
			cout<<"Error in oDocContainPC!"<<endl;
			//::MessageBox(NULL,L"请确定选择PC文件的路径是否正确",L"Error",MB_OK);
			cout<<"PC Path Error,Error in Insert PC"<<endl;
			return FALSE;
		}
		//锁定文件
		::CATLockDocument(*oDocContainPC);

		CATInit *pInitOnDoc2G = NULL ;
		hr = oDocContainPC->QueryInterface(IID_CATInit,(void **) &pInitOnDoc2G);
		if ( FAILED(hr) )return 1 ;
		CATIPrtContainer *pIPrtContOnUDFDoc2G= NULL ;
		pIPrtContOnUDFDoc2G= (CATIPrtContainer*)pInitOnDoc2G->GetRootContainer("CATIPrtContainer");
		if ( NULL == pIPrtContOnUDFDoc2G) return 1 ; 

		pInitOnDoc2G->Release();
		pInitOnDoc2G = NULL ;  
		CATIPrtPart_var spPrtPartOnUDFPart2G = pIPrtContOnUDFDoc2G->GetPart();

		CATIUdfFactory *pIUdfFactory = NULL ;
		hr = pIPrtContOnUDFDoc2G->QueryInterface(IID_CATIUdfFactory,(void **) &pIUdfFactory);
		pIPrtContOnUDFDoc2G->Release();
		pIPrtContOnUDFDoc2G = NULL;

		CATListValCATISpecObject_var* pListPowerCopy = NULL;
		pListPowerCopy = pIUdfFactory->GetPowerCopyList( );
		int SizeOfPowerCopy = pListPowerCopy->Size();
		if(SizeOfPowerCopy == 0)
		{
			return FALSE;
		}
		CATIUdfInstantiate * pIUdfInstantiate = NULL ;
		if( (NULL != pListPowerCopy) &&(NULL_var != (*pListPowerCopy)[iIndex]) )
		{
			hr = (*pListPowerCopy)[iIndex]->QueryInterface(IID_CATIUdfInstantiate,(void **)&pIUdfInstantiate);
			if ( SUCCEEDED(hr) )
			{
				delete pListPowerCopy ;
				pListPowerCopy = NULL ;
			}
		}
		pIUdfFactory->Release();
		pIUdfFactory = NULL ;

		CATPathElement * FirstUIactiveObject1 = NULL ;
		CATBaseUnknown_var FirstDest1 = NULL_var ; 
		if(PathNewPart == NULL)
		{
			cout<<"Error in PathNewPart"<<endl;
			return FALSE;
		}

		hr = pIUdfInstantiate->SetDestinationPath(&PathNewPart,FirstUIactiveObject1,FirstDest1);  
		if(FAILED(hr))
		{
			cout<<"Error in SetDestinationPath!"<<endl;
			return FALSE;
		}
		if ( NULL != FirstUIactiveObject1 ) 
		{
			FirstUIactiveObject1->Release();
			FirstUIactiveObject1 = NULL ;
		}


		//直接赋值
		for(int i = 1;i<=ispListInput.Size();i++)
		{ 
			CATBaseUnknown_var spBaseOldInput = pIUdfInstantiate->GetOldInput(i);
			CATBaseUnknown *pInputLine1 = NULL;
			hr = ispListInput[i]->QueryInterface(IID_CATBaseUnknown,(void**)&pInputLine1);
			CATPathElement * pInput1 = new CATPathElement(pInputLine1);
			pInputLine1->Release();
			pInputLine1 = NULL;

			hr = pIUdfInstantiate->SetNewInput(i, pInput1);
			pInput1->Release();
			pInput1 = NULL;
		}


		hr = pIUdfInstantiate->Instantiate(spBaseUnknownGSDNew);
		if(FAILED(hr))
		{
			cout<<"Error in Instantiate!"<<endl;
		}
		if (pIUdfInstantiate != NULL)
		{
			hr = pIUdfInstantiate->EndInstantiate();	
			pIUdfInstantiate->Reset();
		}

		::CATUnLockDocument(*oDocContainPC);
		CATDocumentServices::Remove(*oDocContainPC);

		if (pIUdfInstantiate != NULL)
		{
			pIUdfInstantiate->Release(); 
			pIUdfInstantiate=NULL;
		}
	}
	CATCatch(CATMfErrUpdate,pErrorUpdate)
	{
		cout<<"Can  not update!"<<endl;
		return FALSE;

	}
	CATCatch(CATError,pError)
	{
		cout<<"Can  not update!"<<endl;
		return FALSE;
	}
	CATCatchOthers
	{
		cout<<"Can  not update!"<<endl;
		return FALSE;
	}
	CATEndTry;



	return TRUE;

}



HRESULT GetViewBoundingBox(CATIView_var ispView,  double& dXLength,double& dYLength)
{
	CATIView_var spEachView = ispView;
	if(spEachView != NULL_var)
	{
		CATIGenerSpec_var spGenerSpec = spEachView->GetGenerSpec();
		if(spGenerSpec==NULL_var)
		{
			cout<<"failed in getting CATIGenerSpec interface"<<endl;
			return S_OK;
		}
		//
		CATMathBox2D* pMathBox2D;
		pMathBox2D = spGenerSpec->GetBBoxOf2DGeneratedGeom();


		double dXMin = 0.0;
		double dXMax = 0.0;
		double dYMin = 0.0;
		double dYMax = 0.0;
		pMathBox2D->GetExtremities(dXMin,dXMax,dYMin,dYMax);

		dXLength = dXMax-dXMin;
		dYLength = dYMax-dYMin;
	}
	return S_OK;
}




void CopyElementsAndCreateNewPart(CATLISTV(CATISpecObject_var) ispListForCopy,CATUnicodeString iUSPartNumber,CATUnicodeString USForPCOutputDirectory)
{
	CATDocument* pNewDoc = NULL;
	pNewDoc = CreateNewPartFile();
	CATUnicodeString USPartNumber = iUSPartNumber;
	cout<<"USPartNumber  "<<USPartNumber<<endl;

	//设置文件的PartNumber
	BOOL bSetPartNumberofPartFile = SetPartNumberofPartFile(pNewDoc,USPartNumber);


	//得到Part文件的主要接口
	//得到这个新创建文件的相关接口
	CATInit *piInitOnDoc = NULL;
	HRESULT rc = pNewDoc -> QueryInterface (IID_CATInit,(void**) &piInitOnDoc);
	if (FAILED(rc))
	{
		cout << "ERROR in QueryInterface on CATInit for doc" << endl << flush;
		return ;
	}

	const CATIdent idCATIContainer = "CATIContainer";   
	CATIContainer *piRootContainer = NULL;
	piRootContainer = (CATIContainer*)piInitOnDoc -> GetRootContainer(idCATIContainer);
	if (NULL == piRootContainer)
	{
		cout << "ERROR in GetRootContainer" << endl << flush;
		return;
	}

	CATIPrtContainer_var spPartSpecCont = piRootContainer; 
	CATIPrtPart_var spPartNew = spPartSpecCont->GetPart();
	CATIMechanicalRootFactory_var spMechanicalRootFact = spPartSpecCont;



	//2、在PART中新建几何图形集INPUT
	CATIDescendants_var spNewDes = CreateNewGSD(spMechanicalRootFact,spPartNew,"INPUT" ,1);//iType = 0为几何体，1为集合图形集
	if(spNewDes == NULL_var)
	{
		cout<<"Error in spNewDes"<<endl;
		return;
	}

	CATLISTV(CATISpecObject_var) ispListForCopyBody;
	CATLISTV(CATISpecObject_var) ispListForElement;
	ispListForCopyBody.RemoveAll();
	ispListForElement.RemoveAll();
	for (int iForCopy = 1;iForCopy<=ispListForCopy.Size();iForCopy++)
	{
		CATIMechanicalTool_var spMechanicalTool = ispListForCopy[iForCopy];
		CATIMmiNonOrderedGeometricalSet_var spSpecGSD = ispListForCopy[iForCopy];
		if (spMechanicalTool != NULL_var || spSpecGSD != NULL_var)
		{
			ispListForCopyBody.Append(ispListForCopy[iForCopy]);
		}
		else
		{
			ispListForElement.Append(ispListForCopy[iForCopy]);
		}
		
	}
	if (ispListForElement.Size()>0)
	{
		CATBoolean bCopyPasteOperation = CopyPasteOperation(ispListForElement,CATISpecObject_var(spNewDes));
	}
	if (ispListForCopyBody.Size()>0)
	{
		CATBoolean bCopyPasteOperation = CopyPasteOperation(ispListForCopyBody,CATISpecObject_var(spPartNew));
	}





	CATUnicodeString USSaveName = USForPCOutputDirectory+"\\"+USPartNumber+".CATPart";
	cout<<"USSaveName  "<<USSaveName<<endl;
	CATDocumentServices::SaveAs(*pNewDoc,USSaveName,"CATPart");
	CATDocumentServices::Remove(*pNewDoc);
}

CATIParmPublisher_var CreateParamSetUnderGSD(CATISpecObject_var ispFather,CATICkeRelationFactory_var ispCkeRelationFactory,CATUnicodeString iUSParamSetName)
{
	CATIParmPublisher_var ospSpec = NULL_var;
	if (ispFather != NULL_var && ispCkeRelationFactory != NULL_var)
	{
		CATIParmPublisher_var spParmPublisher = ispFather;
		ospSpec= ispCkeRelationFactory->CreateParameterSet(iUSParamSetName);
		if (spParmPublisher != NULL_var && ospSpec != NULL_var)
		{
			spParmPublisher->Append(ospSpec);
		}
	}
	return ospSpec;
}



CATICkeParm_var CreateLengthParam(CATUnicodeString iUSParamName,double idValue,CATIParmPublisher_var ispFatherParamSet)
{
	CATICkeParm_var ospParam = NULL_var;
	if (spCkeParmFactory != NULL_var)
	{
		ospParam = spCkeParmFactory->CreateLength(iUSParamName,idValue/1000.0);
		if (ispFatherParamSet != NULL_var)
		{
			ispFatherParamSet->Append(ospParam);
		}
	}
	return ospParam;
}

CATICkeParm_var CreateAngleParam(CATUnicodeString iUSParamName,double idValue,CATIParmPublisher_var ispFatherParamSet)
{
	CATICkeParm_var ospParam = NULL_var;
	if (spCkeParmFactory != NULL_var)
	{
		ospParam = spCkeParmFactory->CreateAngle(iUSParamName,idValue*CATPI/180);
		if (ispFatherParamSet != NULL_var)
		{
			ispFatherParamSet->Append(ospParam);
		}
	}
	return ospParam;
}

CATISpecObject_var GetReferencePlane(CATIPrtPart_var ispPrt,int iXYZ)
{
	CATISpecObject_var ospSpecReturn = NULL_var;
	if (ispPrt != NULL_var)
	{
		CATListValCATISpecObject_var spListPlanes = ispPrt->GetReferencePlanes( ); 
		ospSpecReturn = spListPlanes[iXYZ];
	}
	return ospSpecReturn;

}



CATICkeParm_var CreateParmWithRelation(CATUnicodeString iUSParamName,CATUnicodeString iUSRelationName,CATCkeListOf(Parm) ParamList1,CATUnicodeString USFormula,int iAddRelationToSet,CATIParmPublisher_var ispFatherParamSet)
{
	CATICkeParm_var ospParam = CreateLengthParam(iUSParamName,0.0,ispFatherParamSet);
	CATICkeRelation_var CkeRelation = spCkeParmFactory->CreateFormula(iUSRelationName, "","",ospParam, &ParamList1,USFormula, NULL_var,CATCke::False);
	if (iAddRelationToSet == 1)
	{
		FunctionFactory->AddToCurrentRelationSet(CkeRelation);
	}
	//CATISpecObject_var CkeRelationHalfSW1 = CkeRelationHalfSW;
	//CkeRelationHalfSW1->Update();
	return ospParam;
}









CATICkeParm_var CreateAngleParmWithRelation(CATUnicodeString iUSParamName,CATUnicodeString iUSRelationName,CATCkeListOf(Parm) ParamList1,CATUnicodeString USFormula,int iAddRelationToSet,CATIParmPublisher_var ispFatherParamSet)
{
	CATICkeParm_var ospParam = CreateAngleParam(iUSParamName,0.0,ispFatherParamSet);
	CATICkeRelation_var CkeRelation = spCkeParmFactory->CreateFormula(iUSRelationName, "","",ospParam, &ParamList1,USFormula, NULL_var,CATCke::False);
	if (iAddRelationToSet == 1)
	{
		FunctionFactory->AddToCurrentRelationSet(CkeRelation);
	}
	//CATISpecObject_var CkeRelationHalfSW1 = CkeRelationHalfSW;
	//CkeRelationHalfSW1->Update();
	return ospParam;
}






void SetReferenceLineProperties(CATISpecObject_var iSpecObj,int iColorR,int iColorG,int iColorB,int iLineWidth,int iLineType)
{
	if (iSpecObj == NULL_var)
	{
		return ;
	}
	CATIVisProperties *piGraphProp=NULL;
	HRESULT rc = iSpecObj->QueryInterface(IID_CATIVisProperties,(void**)&piGraphProp);
	if( FAILED(rc) || piGraphProp == NULL ) return ;

	CATVisPropertiesValues Attribut;
	CATVisPropertiesValues Attribut2;
	CATVisPropertiesValues Attribut3;
	Attribut.SetColor(iColorR, iColorG, iColorB);
	Attribut2.SetWidth(2,1); 
	Attribut3.SetLineType(2,1);
	

	piGraphProp->SetPropertiesAtt(Attribut,CATVPColor,CATVPLine,0,0);
	piGraphProp->SetPropertiesAtt(Attribut2,CATVPWidth,CATVPLine,0,0);
	piGraphProp->SetPropertiesAtt(Attribut3,CATVPLineType,CATVPLine,0,0);
	piGraphProp->Release();
	piGraphProp=NULL;

	CATIModelEvents *piME = NULL;
	rc = iSpecObj->QueryInterface( IID_CATIModelEvents,(void **) &piME);
	if( FAILED(rc) || piME == NULL ) return ;
	CATModifyVisProperties notif(iSpecObj, CATPathElement(iSpecObj), CATVPLine,CATVPColor,Attribut);
	piME->Dispatch(notif);

	CATModifyVisProperties notif2(iSpecObj, CATPathElement(iSpecObj), CATVPLine,CATVPWidth,Attribut2);
	piME->Dispatch(notif2);

	CATModifyVisProperties notif3(iSpecObj, CATPathElement(iSpecObj), CATVPLine,CATVPLineType,Attribut3);
	piME->Dispatch(notif3);

	piME->Release();
	piME= NULL ;

}



void HideElement(CATISpecObject_var ispSpecObject)
{
		CATIVisProperties * pPropOnPoint = NULL ;
		HRESULT rc = ispSpecObject->QueryInterface(IID_CATIVisProperties, (void**)&pPropOnPoint);
		if(NULL!=pPropOnPoint) 
		{
			CATVisPropertiesValues PropValue;
			CATVisPropertyType PropTypeOnOnPtObj = CATVPShow ;
			CATVisGeomType GeomTypeOnPtObj = CATVPGlobalType ;
			PropValue.SetShowAttr(CATNoShowAttr);
			rc = pPropOnPoint->SetPropertiesAtt(PropValue, PropTypeOnOnPtObj, GeomTypeOnPtObj);												
			pPropOnPoint->Release();
			pPropOnPoint=NULL;	

			CATIModelEvents *piME = NULL;
			rc = ispSpecObject->QueryInterface( IID_CATIModelEvents,(void **) &piME);
			if( FAILED(rc) || piME == NULL ) return ;
			CATModifyVisProperties notif(ispSpecObject, CATPathElement(ispSpecObject), CATVPGlobalType,CATVPShow,PropValue);
			piME->Dispatch(notif);
			piME->Release();
			piME= NULL ;

		}

}


void ChangeNameAndAppendAndHideElement(CATUnicodeString iUSName,CATIDescendants_var ispDesFather,CATISpecObject_var ispSpecObject,int iAppend,int iHide)
{
	if (iUSName != "")
	{
		if (ispSpecObject != NULL_var)
		{
			CATIAlias_var spAlias = ispSpecObject;
			if (spAlias != NULL_var)
			{
				spAlias->SetAlias(iUSName);
			}
		}
	}
	if (iAppend == 1)//说明不添加
	{
		if (ispDesFather != NULL_var && ispSpecObject != NULL_var)
		{
			ispDesFather->Append(ispSpecObject);
		}
	}


	if (iHide == 1)
	{
		HideElement(ispSpecObject);
	}


	//if (iUSName  == "")
	//{
	//	HideElement(ispSpecObject);
	//}
	//else
	//{
	//	if (ispDesFather != NULL_var && ispSpecObject != NULL_var)
	//	{
	//		ispDesFather->Append(ispSpecObject);
	//	}
	//}
	

}


int UpdateTest(CATISpecObject_var ispSpec)
{
	int iForReturn = 1;
	if (ispSpec == NULL_var)
	{
		return -1;
	}
	CATTry
	{
		ispSpec->Update();
	}
	CATCatch(CATMfErrUpdate,pErrorUpdate)
	{
		iForReturn = -1;
	}
	CATCatch(CATError,pError)
	{
		iForReturn = -1;
	}
	CATCatchOthers
	{
		iForReturn = -1;
	}
	CATEndTry;

	return iForReturn;
}

CATIDescendants_var GetDesTA( )
{
	CATIDescendants_var ospDesForReturn = NULL_var;
	ospDesForReturn = GetGSDByName(spPrtPart,"TA_Config01");
	if (ospDesForReturn == NULL_var)
	{
		ospDesForReturn = GetGSDByName(spPrtPart,"TA_Config02");
		if (ospDesForReturn == NULL_var)
		{
			ospDesForReturn = GetGSDByName(spPrtPart,"TA_Config03");
			if (ospDesForReturn == NULL_var)
			{
				ospDesForReturn = GetGSDByName(spPrtPart,"TA_Config04");
				if (ospDesForReturn == NULL_var)
				{
					ospDesForReturn = GetGSDByName(spPrtPart,"TA_Config05");
					if (ospDesForReturn == NULL_var)
					{
						ospDesForReturn = GetGSDByName(spPrtPart,"TA_Config06");
					}
				}
			}
		}
	}
	return ospDesForReturn;

}
CATIDescendants_var GetDesSHO( )
{
	CATIDescendants_var ospDesForReturn = NULL_var;
	ospDesForReturn = GetGSDByName(spPrtPart,"SHO_Config02");
	if (ospDesForReturn == NULL_var)
	{
		ospDesForReturn = GetGSDByName(spPrtPart,"SHO_Config03");
	}
	return ospDesForReturn;

}

CATIDescendants_var GetDesUSR( )
{
	CATIDescendants_var ospDesForReturn = NULL_var;
	ospDesForReturn = GetGSDByName(spPrtPart,"USR_Config01");
	if (ospDesForReturn == NULL_var)
	{
		ospDesForReturn = GetGSDByName(spPrtPart,"USR_Config02");
		if (ospDesForReturn == NULL_var)
		{
			ospDesForReturn = GetGSDByName(spPrtPart,"USR_Config03");
			if (ospDesForReturn == NULL_var)
			{
				ospDesForReturn = GetGSDByName(spPrtPart,"USR_Config04");
				if (ospDesForReturn == NULL_var)
				{
					ospDesForReturn = GetGSDByName(spPrtPart,"USR_Config05");
					if (ospDesForReturn == NULL_var)
					{
						ospDesForReturn = GetGSDByName(spPrtPart,"USR_Config06");
						if (ospDesForReturn == NULL_var)
						{
							ospDesForReturn = GetGSDByName(spPrtPart,"USR_Config07");
							if (ospDesForReturn == NULL_var)
							{
								ospDesForReturn = GetGSDByName(spPrtPart,"USR_Config08");
								if (ospDesForReturn == NULL_var)
								{
									ospDesForReturn = GetGSDByName(spPrtPart,"USR_Config09");
								}
							}
						}
					}
				}
			}
		}
	}
	return ospDesForReturn;

}


CATIDescendants_var GetDesNSD_UG_ARC( )
{
	CATIDescendants_var ospDesForReturn = NULL_var;
	ospDesForReturn = GetGSDByName(spPrtPart,"NSD_UG ARC_Config01");
	if (ospDesForReturn == NULL_var)
	{
		ospDesForReturn = GetGSDByName(spPrtPart,"NSD_UG ARC_Config02");
		if (ospDesForReturn == NULL_var)
		{
			ospDesForReturn = GetGSDByName(spPrtPart,"NSD_UG ARC_Config03");
			if (ospDesForReturn == NULL_var)
			{
				ospDesForReturn = GetGSDByName(spPrtPart,"NSD_UG ARC_Config04");
			}
		}
	}
	return ospDesForReturn;

}


CATIDescendants_var GetDesNSD_SHO( )
{
	CATIDescendants_var ospDesForReturn = NULL_var;
	ospDesForReturn = GetGSDByName(spPrtPart,"NSD_SHO ARC_Config01");
	if (ospDesForReturn == NULL_var)
	{
		ospDesForReturn = GetGSDByName(spPrtPart,"NSD_SHO ARC_Config02");
	}
	return ospDesForReturn;

}

CATIDescendants_var GetDesBR( )
{
	CATIDescendants_var ospDesForReturn = NULL_var;
	ospDesForReturn = GetGSDByName(spPrtPart,"BR_Config01");
	if (ospDesForReturn == NULL_var)
	{
		ospDesForReturn = GetGSDByName(spPrtPart,"BR_Config02");
	}
	return ospDesForReturn;
}

CATIDescendants_var GetDesLSR( )
{
	CATIDescendants_var ospDesForReturn = NULL_var;
	ospDesForReturn = GetGSDByName(spPrtPart,"LSR_Config01");
	if (ospDesForReturn == NULL_var)
	{
		ospDesForReturn = GetGSDByName(spPrtPart,"LSR_Config02");
		if (ospDesForReturn == NULL_var)
		{
			ospDesForReturn = GetGSDByName(spPrtPart,"LSR_Config03");
		}
	}
	return ospDesForReturn;
}
CATIDescendants_var GetDesBR_Split( )
{
	CATIDescendants_var ospDesForReturn = NULL_var;
	ospDesForReturn = GetGSDByName(spPrtPart,"BR SPLIT_Config01");
	if (ospDesForReturn == NULL_var)
	{
		ospDesForReturn = GetGSDByName(spPrtPart,"BR SPLIT_Config02");
		if (ospDesForReturn == NULL_var)
		{
			ospDesForReturn = GetGSDByName(spPrtPart,"BR SPLIT_Config03");
		}
	}
	return ospDesForReturn;
}
CATIDescendants_var GetDesReference( )
{
	CATIDescendants_var ospDesForReturn = NULL_var;
	ospDesForReturn = GetGSDByName(spPrtPart,"Reference");
	if (ospDesForReturn == NULL_var)
	{
		ospDesForReturn = GetGSDByName(spPrtPart,"ReferenceLine");
	}

	return ospDesForReturn;
}


CATIDescendants_var GetDesCenterCarcassLine( )
{
	CATIDescendants_var ospDesForReturn = NULL_var;
	ospDesForReturn = GetGSDByName(spPrtPart,"CenterCarcassLine");
	if (ospDesForReturn == NULL_var)
	{
		ospDesForReturn = GetGSDByName(spPrtPart,"CenterCarcassLine-2nd");
	}

	return ospDesForReturn;
}

CATISpecObject_var GetSW_Line()
{
	CATISpecObject_var  oForReturn = NULL_var;
	CATIDescendants_var spDesRef = GetDesCenterCarcassLine();
	if (spDesRef == NULL_var)
	{
		spDesRef = GetDesReference();
	}
	if (spDesRef != NULL_var)
	{
		oForReturn = GetElementByNameInGSD(spDesRef,"SW_Line");
	}
	return oForReturn;

}

CATISpecObject_var GetSW_PT()
{
	CATISpecObject_var  oForReturn = NULL_var;
	CATIDescendants_var spDesRef = GetDesLSR();
	if (spDesRef != NULL_var)
	{
		oForReturn = GetElementByNameInGSD(spDesRef,"SW_PT");
	}

	if (oForReturn == NULL_var)
	{
		spDesRef = GetDesReference();
		if (spDesRef != NULL_var)
		{
			oForReturn = GetElementByNameInGSD(spDesRef,"SW_PT");
		}
	}

	return oForReturn;

}

CATIParmPublisher_var  GetParamSetUnderGSD(CATIDescendants_var ispSpecGSD,CATUnicodeString iUSParamSetName)
{
	CATIParmPublisher_var ospSpecForReturn = NULL_var;
	if (ispSpecGSD != NULL_var)
	{
		CATListValCATISpecObject_var oLst;
		oLst.RemoveAll();
		ispSpecGSD->GetDirectChildren("CATIParmPublisher", oLst) ;
		if (oLst.Size()>0)
		{
			for (int iParamSet = 1;iParamSet<=oLst.Size();iParamSet++)
			{
				if (CATIAlias_var(oLst[iParamSet])->GetAlias() == iUSParamSetName)
				{
					ospSpecForReturn = oLst[iParamSet];
					break;
				}
			}
		}

	}
	return ospSpecForReturn;

}

CATICkeParm_var GetParamFromParamSetUnderGSD(CATIDescendants_var ispSpecGSD,CATUnicodeString iUSParamSetName,CATUnicodeString iUSParamName)
{
	//
	CATICkeParm_var ospParamForReturn = NULL_var;
	if (ispSpecGSD != NULL_var)
	{
		//获取GSD下边的几何图形集
		CATIParmPublisher_var  spParamSetNeed = GetParamSetUnderGSD( ispSpecGSD, iUSParamSetName);
		if (spParamSetNeed != NULL_var)
		{
			CATListValCATISpecObject_var ListFound;
			ListFound.RemoveAll();
			spParamSetNeed->GetAllChildren("CATICkeParm", ListFound) ;
			if (ListFound.Size()>0)
			{
				for (int iParam = 1;iParam<=ListFound.Size();iParam++)
				{
					CATICkeParm_var spSpecEachParam = ListFound[iParam];
					if (spSpecEachParam != NULL_var)
					{
						if (spSpecEachParam->InternalRole() == iUSParamName)
						{
							ospParamForReturn = ListFound[iParam];
							break;
						}
					}

				}
			}

		}
	}
	return ospParamForReturn;
}







CATISpecObject_var CreateParaLine(CATISpecObject_var ispSpecSupport,
								  CATISpecObject_var ispSpecForParallel,
								  CATICkeParm_var ispCkeParam1,
								  CATICkeParm_var ispCkeParam2,
								  CATISpecObject_var ispSpecRef,
								  int iMax)
{
	CATISpecObject_var ospSpecParallel = NULL_var;
	CATGSMOrientation Orientation_1 = CATGSMSameOrientation;	
	CATISpecObject_var spSpecParallel_1 = spGSMFact->CreateCurvePar(ispSpecForParallel,ispSpecSupport,ispCkeParam1,Orientation_1);
	CATISpecObject_var spSpecParallel_2 = spGSMFact->CreateCurvePar(ispSpecForParallel,ispSpecSupport,ispCkeParam2,Orientation_1);
	CATISpecObject_var spSpecParallel = spSpecParallel_1;
	int iUpdate1 = UpdateTest(spSpecParallel_1);
	int iUpdate2 = UpdateTest(spSpecParallel_2);
	if (iUpdate1 != -1 && iUpdate2 != -1)
	{
		if (ispSpecRef != NULL_var)
		{
			CATIMeasurablePoint_var spMeaPT = ispSpecRef;
			if (spMeaPT != NULL_var)
			{
				CATMathPoint pPTRef;
				spMeaPT->GetPoint(pPTRef);
				double d11 = GetDistanceBetweenMeasurableMathPoint(spSpecParallel_1,pPTRef);
				double d12 = GetDistanceBetweenMeasurableMathPoint(spSpecParallel_2,pPTRef);
				cout<<"d11  "<<d11<<endl;
				cout<<"d12  "<<d12<<endl;

				if (d11 <d12)
				{
					if (iMax != 1)
					{
						ospSpecParallel = spSpecParallel_1;
					}
					else
					{
						ospSpecParallel = spSpecParallel_2;
					}
				}
				else if (d11 >d12)
				{
					if (iMax != 1)
					{
						ospSpecParallel = spSpecParallel_2;
					}
					else
					{
						ospSpecParallel = spSpecParallel_1;
					}
				}
			}
		}
	}
	return ospSpecParallel;
}