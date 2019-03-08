// COPYRIGHT Dassault Systemes 2019
//===================================================================
//
// CCEFrameInSoundproofCottonGenerationCmd.h
// The state chart based command: CCEFrameInSoundproofCottonGenerationCmd
//
//===================================================================
//
// Usage notes:
//
//===================================================================
//CAA2 Wizard Generation Report
//   STATECOMMAND
//End CAA2 Wizard Generation Report
//
//  Feb 2019  Creation: Code generated by the CAA wizard  Administrator
//===================================================================
#ifndef CCEFrameInSoundproofCottonGenerationCmd_H
#define CCEFrameInSoundproofCottonGenerationCmd_H

#include "CATStateCommand.h"
#include "CATBoolean.h"


#include "Header.h"
#include "ExternHeader.h"
#include "CCEFrameInSoundproofCottonGenerationDlg.h"

//----------------------------------------------------------------------

/**
 * Describe your command here.
 * <p>
 * Using this prefered syntax will enable mkdoc to document your class.
 * <p>
 * refer to programming resources of Dialog engine framework.
 * (consult base class description).
 */
class CCEFrameInSoundproofCottonGenerationCmd: public CATStateCommand
{
  // Allows customization/internationalization of command's messages
  // ---------------------------------------------------------------
  DeclareResource( CCEFrameInSoundproofCottonGenerationCmd, CATStateCommand )

  public:

  CCEFrameInSoundproofCottonGenerationCmd();
  virtual ~CCEFrameInSoundproofCottonGenerationCmd();

    /**
     * Defines the command automata.
     * <br>
     * <i>(states, transitions and agents)</i>
     */
  virtual void     BuildGraph();

    /**
     * Action associated to state transitions.
     */
  virtual CATBoolean  ActionOne(void * data);


public:
	CCEFrameInSoundproofCottonGenerationDlg* _Panel;
	CATBoolean CancelAction(void *data);
};

//----------------------------------------------------------------------

#endif