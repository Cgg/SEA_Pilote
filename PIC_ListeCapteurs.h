#include "PIC.h"

/******************************************************************************/
int AjouterCapteur
(
	PIC_HEADER * const capteur
);

/******************************************************************************/
int RetirerCapteur
(
	char const adresseCapteur
);

/******************************************************************************/
void AfficherCapteurs
(
	void
);

/******************************************************************************/
PIC_HEADER * ChercherCapteur
(
	char const adresseCapteur
);
