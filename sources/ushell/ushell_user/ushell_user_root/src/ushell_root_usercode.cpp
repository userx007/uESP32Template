//#include "ushell_core.h"
#include "ushell_core_printout.h"


///////////////////////////////////////////////////////////////////
//                    USER IMPLEMENTATION                        //
///////////////////////////////////////////////////////////////////


int vtest ( void )
{
    uSHELL_PRINTF("vtest called ...");

    return 1;

} /* vtest */


///////////////////////////////////////////////////////////////////
//               USER SHORTCUTS HANDLERS                         //
///////////////////////////////////////////////////////////////////


#if (1 == uSHELL_IMPLEMENTS_USER_SHORTCUTS)

extern "C" void uShellUserHandleShortcut_Dot( const char *pstrArgs )
{
    uSHELL_PRINTF("[.] registered but not implemented | args[%s] ", pstrArgs);

} /* uShellUserHandleShortcut_Dot() */


/******************************************************************************/
extern "C" void uShellUserHandleShortcut_Slash( const char *pstrArgs )
{
    uSHELL_PRINTF("[/] registered but not implemented | args[%s] ", pstrArgs);

} /* uShellUserHandleShortcut_Slash() */

#endif /*(1 == uSHELL_IMPLEMENTS_USER_SHORTCUTS)*/
