#ifdef  _WINDOWS_SOURCE
#ifdef  __CCETAndSInsulationLayerTlbM
#define ExportedByCCETAndSInsulationLayerTlbM     __declspec(dllexport)
#else
#define ExportedByCCETAndSInsulationLayerTlbM     __declspec(dllimport)
#endif
#else
#define ExportedByCCETAndSInsulationLayerTlbM
#endif
