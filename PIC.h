/* Description d'un pilote de capteur :
 * 
 * prefixe des fonctions : PIC
 * 
 * copyright 2010 Lize / Geiger
 */

#ifndef PIC_H
#define PIC_H

/* system includes */
#include "iosLib.h"

/* primitives de configuration du pilote */

/******************************************************************************/
int PIC_DrvInstall
(
	void
);

/******************************************************************************/
int PIC_DrvRemove
(
	void
);

/******************************************************************************/
int PIC_DevAdd
(
	char * const name
);

/******************************************************************************/
int PIC_DevDelete
(
	char * const name
);

#endif
/* PIC_H */
