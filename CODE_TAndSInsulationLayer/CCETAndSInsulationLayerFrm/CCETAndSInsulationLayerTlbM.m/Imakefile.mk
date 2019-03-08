# COPYRIGHT Dassault Systemes 2019
#======================================================================
# Imakefile for module CCETAndSInsulationLayerTlbM.m
#======================================================================
#
#  Feb 2019  Creation: Code generated by the CAA wizard  Administrator
#======================================================================
#
# SHARED LIBRARY 
#
BUILT_OBJECT_TYPE=SHARED LIBRARY 
 
# DO NOT EDIT :: THE CAA2 WIZARDS WILL ADD CODE HERE
WIZARD_LINK_MODULES = JS0GROUP \
JS0FM JS0GROUP ApplicationFrame CATAfrUUID 
# END WIZARD EDITION ZONE

LINK_WITH = $(WIZARD_LINK_MODULES)   CATApplicationFrame \
                                     DI0PANV2 \
                                     CATObjectModelerBase \
                                     CATMecModInterfaces \
                                     CATObjectSpecsModeler \
                                     CATVisualization \
                                     CATMechanicalModeler \
                                     CATMechanicalModelerUI \
                                     KnowledgeItf \
                                     CATGitInterfaces \
                                     CATTopologicalObjects \
                                     CATMeasureGeometryInterfaces \
                                     CATProductStructure1 \
                                     CATUdfInterfaces \
                                     CATSaiSpaceAnalysisItf \
                                     CATAssemblyInterfaces \
                                     CATAssemblyInterfacesUUID \
                                     CATGeometricObjects \
                                     CATPartInterfaces \
                                     CATDraftingInterfaces \
                                     CATSketcherInterfaces \
                                     JS0GROUP \
                                     JS0GROUP JS0CORBA JS0FM CATDialogEngine  CATMechanicalModelerUI CATObjectSpecsModeler CATObjectModelerBase CATMecModInterfaces \
 CATVisualization CATDraftingInterfaces   CATObjectModelerBase   SystemUUID  CATSketcherInterfaces CATMathematics CATGeometricObjects CATMeasureGeometryInterfaces \
             CATProductStructure1 \
             CATInteractiveInterfaces \
             CATGotInterfaces \
             CATMecModInterfaces \
             ProductStructurePubIDL \
             

# System dependant variables
#
OS = AIX
#
OS = HP-UX
#
OS = IRIX
#
OS = SunOS
#
OS = Windows_NT
