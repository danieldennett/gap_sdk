
/* autogenerated - do not edit */
#include "jim.h"
#include "jimautoconf.h"
int Jim_InitStaticExtensions(Jim_Interp *interp)

{
	extern int Jim_stdlibInit(Jim_Interp *);
	extern int Jim_aioInit(Jim_Interp *);
	extern int Jim_arrayInit(Jim_Interp *);
	extern int Jim_clockInit(Jim_Interp *);
	extern int Jim_eventloopInit(Jim_Interp *);
	extern int Jim_execInit(Jim_Interp *);
	extern int Jim_fileInit(Jim_Interp *);
	extern int Jim_historyInit(Jim_Interp *);
	extern int Jim_interpInit(Jim_Interp *);
	extern int Jim_loadInit(Jim_Interp *);
	extern int Jim_namespaceInit(Jim_Interp *);
	extern int Jim_nshelperInit(Jim_Interp *);
	extern int Jim_ooInit(Jim_Interp *);
	extern int Jim_packInit(Jim_Interp *);
	extern int Jim_packageInit(Jim_Interp *);
	extern int Jim_posixInit(Jim_Interp *);
	extern int Jim_readdirInit(Jim_Interp *);
	extern int Jim_regexpInit(Jim_Interp *);
	extern int Jim_signalInit(Jim_Interp *);
	extern int Jim_syslogInit(Jim_Interp *);
	extern int Jim_tclcompatInit(Jim_Interp *);
	extern int Jim_tclprefixInit(Jim_Interp *);
	extern int Jim_binaryInit(Jim_Interp *);
	extern int Jim_globInit(Jim_Interp *);
	extern int Jim_treeInit(Jim_Interp *);
	Jim_stdlibInit(interp);
	Jim_aioInit(interp);
	Jim_arrayInit(interp);
	Jim_clockInit(interp);
	Jim_eventloopInit(interp);
	Jim_execInit(interp);
	Jim_fileInit(interp);
	Jim_historyInit(interp);
	Jim_interpInit(interp);
	Jim_loadInit(interp);
	Jim_namespaceInit(interp);
	Jim_nshelperInit(interp);
	Jim_ooInit(interp);
	Jim_packInit(interp);
	Jim_packageInit(interp);
	Jim_posixInit(interp);
	Jim_readdirInit(interp);
	Jim_regexpInit(interp);
	Jim_signalInit(interp);
	Jim_syslogInit(interp);
	Jim_tclcompatInit(interp);
	Jim_tclprefixInit(interp);
	Jim_binaryInit(interp);
	Jim_globInit(interp);
	Jim_treeInit(interp);
	return JIM_OK;
}
