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
	
	int i = 0;
	
	PIC_MESSAGE_BRUTE     messageRecu;	
	PIC_MESSAGE_CAPTEUR   messageTraite;
	
	PIC_HEADER * * tabPointeurs;
	PIC_HEADER *   destinataire;
	
	TIMESTAMP tempsArrivee;
	
	MSG_Q_ID idBalDrv = ( MSG_Q_ID ) idBalDrvInt;
	
	tabPointeurs = ( PIC_HEADER * * )tabPointeursIN;
	
	for( ;; )
	{
		/* Recuperation du dernier message envoye par les capteurs */
		if( msgQReceive( idBalDrv, ( char * )&messageRecu, PIC_TAILLE_MSG_BRUTE,
				WAIT_FOREVER ) != -1 )
		{
			/* Traitement du message */
			clock_gettime( CLOCK_REALTIME, &tempsArrivee );
			
			messageTraite.message    = messageRecu.message;
			messageTraite.tArrivee   = tempsArrivee;
			messageTraite.numMessage = ++compteurMessage;
			
			/* Recherche du device correspondant */
			for( i = 0 ; i < PIC_N_CAPTEURS_MAX ; i++ )
			{
				if( tabPointeurs[ i ] != NULL && 
						messageRecu.adresseCapteur == 
						tabPointeurs[ i ]->specific.adresseCapteur )
				{
					destinataire = tabPointeurs[ i ];
					
					i = PIC_N_CAPTEURS_MAX;
				}
			}
			
			/* Si le device est trouve, le message traite est poste dans sa
			 * bal.
			 */
			if( destinataire != NULL )
			{
				if ( msgQNumMsgs( destinataire->specific.idBAL ) == 
						PIC_N_MESSAGES_MAX )
				{
					msgQReceive( destinataire->specific.idBAL, NULL, 0,
							NO_WAIT );
				}
				
				msgQSend( destinataire->specific.idBAL, 
						( char * )&messageTraite, PIC_TAILLE_MSG_TRAITE, NO_WAIT
						, MSG_PRI_NORMAL );
			}
		}
	}
}
