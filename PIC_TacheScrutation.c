/* Implementation de la tache de scrutation du Pilote de Capteur
 */

/* === INCLUDES === */

/* system includes */
#include "stdlib.h"

/* project includes */
#include "PIC_TacheScrutation.h"
#include "PIC.h"


/* === DEFINITIONS DE CONSTANTES === */


/* === DECLARATIONS DE TYPES DE DONNEES === */


/* === DONNEES STATIQUES === */

static int compteurMessage = 0;


/* === PROTOTYPES DES FONCTIONS LOCALES === */

/******************************************************************************/
PIC_DATA_STRUCTURE * PIC_RechercheCapteur
(
	char const adresseCapteur
);


/* === IMPLEMENTATION === */

/******************************************************************************/
int PIC_TacheScrutation
(
	MSG_Q_ID const idBalDrv
	/* a passer egalement : pointeur vers la liste/le tableau des capteurs 
	 * installes 
	 */
)
{
	/* Algo :
	 * - recuperation du dernier message situe dans BalDrv 
	 * - numerotation, timestampage
	 * - recherche du capteur correspondant
	 * - si capteur trouve, depot du message dans la bal du capteur
	 * - goto le debut :D
	 */
	
	PIC_MESSAGE_BRUTE     messageRecu;	
	PIC_MESSAGE_CAPTEUR   messageTraite;
	
	PIC_DATA_STRUCTURE * destinataire;
	
	for( ;; )
	{
		msgQReceive( idBalDrv, ( char * )&messageRecu, PIC_TAILLE_MSG_BRUTE, WAIT_FOREVER );
		
		messageTraite.message    = messageRecu.message;
		//messageTraite.tArrivee =
		messageTraite.numMessage = ++compteurMessage;
		
		destinataire = PIC_RechercheCapteur( messageRecu.adresseCapteur );

		if( destinataire != NULL )
		{
			msgQSend( destinataire->idBAL, ( char * )&messageTraite, PIC_TAILLE_MSG_TRAITE, NO_WAIT, MSG_PRI_NORMAL );
		}
	}
}

/******************************************************************************/
PIC_DATA_STRUCTURE * PIC_RechercheCapteur
(
	char const adresseCapteur
)
{
	
}
