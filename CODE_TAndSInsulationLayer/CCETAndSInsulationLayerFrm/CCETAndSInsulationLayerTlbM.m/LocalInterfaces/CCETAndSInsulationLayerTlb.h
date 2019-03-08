// COPYRIGHT Dassault Systemes 2019
//===================================================================
//
// CCETAndSInsulationLayerTlb.h
// Provide implementation to interface
//    CATIAfrGeneralWksAddin

//
//===================================================================
//
// Usage notes:
//
//===================================================================
//CAA2 Wizard Generation Report
//IMPLEMENTATION
//    TIE: CATIAfrGeneralWksAddin

//End CAA2 Wizard Generation Report
//
//  Feb 2019  Creation: Code generated by the CAA wizard  Administrator
//===================================================================
#ifndef CCETAndSInsulationLayerTlb_H
#define CCETAndSInsulationLayerTlb_H

#include "CATBaseUnknown.h"
#include "Header.h"

//-----------------------------------------------------------------------

/**
* Class representing xxx.
*
* <br><b>Role</b>: Provide the basic class function...
* <p>
* It implements the interfaces :
*  <ol>
*  <li>@href CATIAfrGeneralWksAddin
*  </ol>
*
* @href ClassReference, Class#MethodReference, #InternalMethod...
*/
class CCETAndSInsulationLayerTlb: public CATBaseUnknown

{
  CATDeclareClass;

  public:

  // Standard constructors and destructors for an implementation class
  // -----------------------------------------------------------------
     CCETAndSInsulationLayerTlb ();
     virtual ~CCETAndSInsulationLayerTlb ();


  private:
  // The copy constructor and the equal operator must not be implemented
  // -------------------------------------------------------------------
  CCETAndSInsulationLayerTlb (CCETAndSInsulationLayerTlb &);
  CCETAndSInsulationLayerTlb& operator=(CCETAndSInsulationLayerTlb&);



public:
	CATCmdContainer * CreateToolbars();
	void CreateCommands()  ;

};

//-----------------------------------------------------------------------

#endif
