/* Implementation de la tache de scrutation du Pilote de Capteur
 */

/* === INCLUDES === */

/* system includes */
#include "stdlib.h"

/* project includes */
#include "PIC_TacheScrutation.h"
#include "PIC.h"
#include "PIC_ListeCapteurs.h"


/* === DEFINITIONS DE CONSTANTES === */


/* === DECLARATIONS DE TYPES DE DONNEES === */


/* === DONNEES STATIQUES === */

static int compteurMessage = 0;


/* === IMPLEMENTATION === */

/******************************************************************************/
int PIC_TacheScrutation
(
	MSG_Q_ID const idBalDrv
)
{
	/* Algo :
	 * - recuperation du dernier message situe dans BalDrv 
	 * - numerotation, timestampage
	 * - recherche du capteur correspondant
	 * - si capteur trouve, depot du message dans la bal du capteur (retrait 
	 * d'un message si bal pleine)
	 * - goto le debut :D
	 */
	
	PIC_MESSAGE_BRUTE     messageRecu;	
	PIC_MESSAGE_CAPTEUR   messageTraite;
	
	PIC_HEADER * destinataire;
	
	for( ;; )
	{
		msgQReceive( idBalDrv, ( char * )&messageRecu, PIC_TAILLE_MSG_BRUTE, WAIT_FOREVER );
		
		messageTraite.message    = messageRecu.message;
		//messageTraite.tArrivee =
		messageTraite.numMessage = ++compteurMessage;
		
		destinataire = ChercherCapteur( messageRecu.adresseCapteur );

		if( destinataire != NULL )
		{
			if ( msgQNumMsgs( destinataire->specific.idBAL ) == PIC_N_MESSAGES_MAX )
			{
				msgQReceive( destinataire->specific.idBAL, NULL, 0, NO_WAIT );
			}
			
			msgQSend( destinataire->specific.idBAL, ( char * )&messageTraite, PIC_TAILLE_MSG_TRAITE, NO_WAIT, MSG_PRI_NORMAL );
		}
	}
}
