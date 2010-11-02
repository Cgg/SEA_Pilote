#ifndef PIC_DUMMY_CAPTEUR
#define PIC_DUMMY_CAPTEUR

/* === INCLUDES === */

/* include projet */
#include "PIC.h"


/* === DEFINITION DE CONSTANTES === */

#define PIC_PRIORITE_SIMULATION        ( 20 )
#define PIC_STACK_SIMULATION           ( 2000 )


/* === PROTOTYPES DES FONCTIONS EXPOSEES === */

/******************************************************************************/
int PIC_SimStart
(
	char      * tabAdressesCapteurs,
	int const   nbCapteur
);

/******************************************************************************/
int PIC_SimStop
(
	void
);

#endif
/* PIC_DUMMY_CAPTEUR */
