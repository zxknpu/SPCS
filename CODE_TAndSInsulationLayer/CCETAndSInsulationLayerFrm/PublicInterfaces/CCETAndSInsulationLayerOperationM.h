#ifdef  _WINDOWS_SOURCE
#ifdef  __CCETAndSInsulationLayerOperationM
#define ExportedByCCETAndSInsulationLayerOperationM     __declspec(dllexport)
#else
#define ExportedByCCETAndSInsulationLayerOperationM     __declspec(dllimport)
#endif
#else
#define ExportedByCCETAndSInsulationLayerOperationM
#endif
