
#ifndef _DBGMSG_h_

/*
TODO: maybe include __func__(C99 standard) or __FUNCTION__ (later gcc 
versions), but may depend on compiler and its version...
https://gcc.gnu.org/onlinedocs/cpp/Standard-Predefined-Macros.html
*/

#include <stdio.h>
#define DBGMSG(msg) printf("[%s:%d] %s\n",__FILE__,__LINE__,msg);

#define _DBGMSG_h_
#endif //_DBGMSG_h_
