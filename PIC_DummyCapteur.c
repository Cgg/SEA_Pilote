/* Implementation d'un simulateur de capteur.
 * Paramètres: 	id de la boîte aux lettres du driver
 * 				numéro du capteur
 */

/* === INCLUDES === */

/* system includes */
#include "stdlib.h"
#include "stdio.h"
#include "taskLib.h"
#include "timers.h"

/* project includes */
#include "PIC_DummyCapteur.h"


/* === DONNEES STATIQUES === */

static int idTacheSimulation = 0;


/* === IMPLEMENTATION === */

/******************************************************************************/
int PIC_DummyCapteur
( 
	char * 		 tabAdressesCapteurs,
	int    const nbCapteurs
)
{
	struct timespec sleepTime;
	
	MSG_Q_ID balDrv = PIC_RecupererBal();
	
	PIC_MESSAGE_BRUTE msg;

	int nbCapteursUtilises = PIC_N_CAPTEURS_MAX;
	
	sleepTime.tv_sec = 0;
	
	if ( nbCapteurs >= PIC_N_CAPTEURS_MAX )
	{
		nbCapteursUtilises = nbCapteurs;
	}
	
	for( ;; )
	{
		msg.adresseCapteur = tabAdressesCapteurs[ ( rand() % nbCapteurs ) ],
		
		msg.message = 10 * msg.adresseCapteur;
		
		msgQSend( balDrv, ( char* )&msg, PIC_TAILLE_MSG_BRUTE, NO_WAIT, MSG_PRI_NORMAL );
		
		sleepTime.tv_nsec = rand() * 1000;  /* environ */
		nanosleep( &sleepTime, NULL );
	}
	
	return 0;
}

/******************************************************************************/
int PIC_SimStart
(
	char      * tabAdressesCapteurs,
	int const   nbCapteur
)
{
	if ( idTacheSimulation == 0 )
	{
		idTacheSimulation = taskSpawn( "PIC_TacheSimulation", 
				PIC_PRIORITE_SIMULATION, 0, PIC_STACK_SIMULATION, 
				( FUNCPTR )PIC_DummyCapteur, ( int )tabAdressesCapteurs, 
				nbCapteur, 0, 0, 0, 0, 0, 0, 0, 0 );
	}
}

/******************************************************************************/
int PIC_SimStop
(
	void
)
{
	if ( idTacheSimulation != 0 )
	{
		taskDelete( idTacheSimulation );
		idTacheSimulation = 0;
	}
}
