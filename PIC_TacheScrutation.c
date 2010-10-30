/* Implementation de la tache de scrutation du Pilote de Capteur
 */

/* === INCLUDES === */

/* system includes */
#include "stdlib.h"

/* project includes */
#include "PIC_TacheScrutation.h"
#include "PIC.h"


/* === DEFINITIONS DE CONSTANTES === */

#define PRIORITE                       ( 1 )


/* === DECLARATIONS DE TYPES DE DONNEES === */


/* === DONNEES STATIQUES === */

static int compteurMessage = 0;


/* === PROTOTYPES DES FONCTIONS LOCALES === */

/******************************************************************************/
PIC_DATA_STRUCTURE * PIC_RechercheCapteur
(
	int const adresseCapteur
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
	
	char              * bufferReception;
	PIC_MESSAGE_BRUTE * messageRecu;
	
	 bufferReception = ( char * )malloc( PIC_TAILLE_MSG_BRUTE );
	
	for( ;; )
	{
		msgQReceive( idBalDrv, bufferReception, PIC_TAILLE_MSG_BRUTE, WAIT_FOREVER );

		messageRecu = ( PIC_MESSAGE_BRUTE * )bufferReception;
	}
	
	free( bufferReception );
}
