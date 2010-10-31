/* Implementation d'un simulateur de capteur.
 * Paramètres: 	id de la boîte aux lettres du driver
 * 				numéro du capteur
 */

/* === INCLUDES === */

/* system includes */
#include "stdlib.h"
#include "stdio.h"

/* project includes */
#include "PIC_DummyCapteur.h"
/* === IMPLEMENTATION === */

#define RAND_MAX 255 

/******************************************************************************/
int PIC_DummyCapteur
( 
		int balDrvInt 
)
{
	MSG_Q_ID balDrv = ( MSG_Q_ID ) balDrvInt;
	
	PIC_MESSAGE_BRUTE msg;

	
	//printf( "Capteur %c\n" ,msg.adresseCapteur );
	
	for(;;)
	{
		msg.adresseCapteur = rand(); 
		
		msg.message = rand() * 500;
		
		msgQSend( balDrv, (char*)&msg, PIC_TAILLE_MSG_BRUTE, NO_WAIT, MSG_PRI_NORMAL );
		
		//printf( "\tValeur capteur envoyee: %d", msg.message );
		
		sleep ( rand( ) / 100 );
	}
	
	return 0;
}
