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

/* Définition de codes retours */
typedef enum 
{
	deja_installe,
	no_room
	
} PIC_CR_INSTALL;

typedef enum
{
	remove_ok,
	pas_installe,
	fichiers_ouverts
	
} PIC_CR_REMOVE;

typedef enum
{
	driver_pas_installe = -1,
	adresse_prise       = -2,
	nom_pris            = -3,
	n_capteurs_overflow = -4
	
} PIC_CR_ADD;


/* primitives de configuration du pilote */

/******************************************************************************/
PIC_CR_INSTALL PIC_DrvInstall
(
	void
);

/******************************************************************************/
PIC_CR_REMOVE PIC_DrvRemove
(
	void
);

/******************************************************************************/
PIC_CR_ADD PIC_DevAdd
(
	char * const name,
	int    const adresseCapteur
);

/******************************************************************************/
int PIC_DevDelete
(
	char * const name
);

#endif
/* PIC_H */
