/* Implementation de la tache de scrutation du Pilote de Capteur
 */

/* === INCLUDES === */

/* system includes */
#include "stdlib.h"
#include "stdio.h"

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
	int const idBalDrvInt,
	int const tabPointeursIN
)
{
	/* Algo :
	 * - recuperation du dernier message situe dans BalDrv 
	 * - numerotation, timestampage
	 * - recherche du capteur correspondant
	 * - si capteur trouve, depot du message dans la bal du capteur (retrait 
	 * d'un message si bal pleine)
	 * - goto le debut
	 */
	
	int i;
	
	PIC_MESSAGE_BRUTE     messageRecu;	
	PIC_MESSAGE_CAPTEUR   messageTraite;
	
	PIC_HEADER * * tabPointeurs;
	PIC_HEADER *   destinataire;
	
	TIMESTAMP tempsArrivee;
	
	MSG_Q_ID idBalDrv = ( MSG_Q_ID ) idBalDrvInt;
	
	tabPointeurs = ( PIC_HEADER * * )tabPointeursIN;
	
	printf("%d",tabPointeursIN);
	
	i = 0;
	
	for( ;; )
	{
		if( msgQReceive( idBalDrv, ( char * )&messageRecu, PIC_TAILLE_MSG_BRUTE, 2 ) != -1 )
		{
			clock_gettime( CLOCK_REALTIME, &tempsArrivee );
			
			messageTraite.message    = messageRecu.message;
			messageTraite.tArrivee   = tempsArrivee;
			messageTraite.numMessage = ++compteurMessage;
			
			while( i < PIC_N_CAPTEURS_MAX )
			{
				if( tabPointeurs[ i ] != NULL && messageRecu.adresseCapteur == tabPointeurs[ i ]->specific.adresseCapteur )
				{
					destinataire = tabPointeurs[ i ];
					
					i = PIC_N_CAPTEURS_MAX;
				}
				
				i++;
			}
	
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
}
