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
int PIC_DummyCapteur
( 
		int const balDrvInt,
		char * tabAdressesCapteurs,
		int const nbCapteurs
)
{
	MSG_Q_ID balDrv = ( MSG_Q_ID ) balDrvInt;
	
	PIC_MESSAGE_BRUTE msg;

	int nbCapteursUtilises = PIC_N_CAPTEURS_MAX;
	//printf( "Capteur %c\n" ,msg.adresseCapteur );
	
	if ( nbCapteurs <= PIC_N_CAPTEURS_MAX )
		nbCapteursUtilises = nbCapteurs;
	
	for(;;)
	{
		msg.adresseCapteur = tabAdressesCapteurs[ rand() % nbCapteurs -1 ],
		
		msg.message = rand();
		
		msgQSend( balDrv, (char*)&msg, PIC_TAILLE_MSG_BRUTE, NO_WAIT, MSG_PRI_NORMAL );
		
		//printf( "\tValeur capteur envoyee: %d", msg.message );
		
		sleep ( rand( ) % ( RAND_MAX - 5 ) / 100 );
	}
	
	return 0;
}
/*
 	switch (fonction) {
		case 1:
			if ( idTacheSimulation == NULL )
				idTacheSimulation = taskSpawn( "PIC_TacheSimulation", 
						PIC_PRIORITE_SIMULATION, 0, PIC_STACK_SIMULATION, 
						( FUNCPTR )PIC_DummyCapteur, ( int ) idBalDrv, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
			break;
		case 2:
			if ( idTacheSimulation != NULL )
				{
					taskDelete( idTacheSimulation );
					idTacheSimulation = NULL;
				}
			break;
		default:
			break;
	}
 * *
 */
