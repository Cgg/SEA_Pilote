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

/******************************************************************************/
int main(int argc, char** argv)
{
	MSG_Q_ID bal = (MSG_Q_ID) atoi( argv[ 1 ] );
	
	PIC_MESSAGE_BRUTE msg;

	msg.adresseCapteur = atoi( argv[ 2 ] );
	
	printf( "Capteur %c\n" ,msg.adresseCapteur );
	
	for(;;)
	{
		msg.message = rand();
		
		msgQSend( bal, (char*)&msg, PIC_TAILLE_MSG_BRUTE, NO_WAIT, MSG_PRI_NORMAL );
		
		printf( "\tValeur capteur envoyee: %d", msg.message );
		
		sleep ( rand( ) );
	}
	
	return 0;
}
