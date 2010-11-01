#ifndef PIC_DUMMY_CAPTEUR
#define PIC_DUMMY_CAPTEUR

/* === INCLUDES === */
#include "msgQLib.h"
#include "PIC.h"

/* === DEFINITION DE CONSTANTES === */
/* === PROTOTYPES DES FONCTIONS EXPOSEES === */


int PIC_DummyCapteur
( 
		int const balDrvInt, 
		char * tabAdressesCapteurs,
		int const nbCapteur
);

/******************************************************************************/
#endif
/* PIC_DUMMY_CAPTEUR */
