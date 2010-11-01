/* Implementation d'un simulateur de capteur.
 * Paramètres: 	id de la boîte aux lettres du driver
 * 				numéro du capteur
 */

/* === INCLUDES === */

/* system includes */
#include "stdlib.h"
#include "stdio.h"
#include "taskLib.h"

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
	MSG_Q_ID balDrv = PIC_RecupererBal();
	
	PIC_MESSAGE_BRUTE msg;

	int nbCapteursUtilises = PIC_N_CAPTEURS_MAX;
	
	if ( nbCapteurs >= PIC_N_CAPTEURS_MAX )
	{
		nbCapteursUtilises = nbCapteurs;
	}
	
	for( ;; )
	{
		msg.adresseCapteur = tabAdressesCapteurs[ ( rand() % nbCapteurs ) - 1 ],
		
		msg.message = 10 * msg.adresseCapteur;
		
		msgQSend( balDrv, ( char* )&msg, PIC_TAILLE_MSG_BRUTE, NO_WAIT, MSG_PRI_NORMAL );
		
		sleep( rand() % ( ( RAND_MAX - 5 ) / 100 ) );
	}
	
	return 0;
}

/******************************************************************************/
int PIC_SimStart
(
	char * tabAdressesCapteurs,
	int const nbCapteur
)
{
	if ( idTacheSimulation == 0 )
	{
		idTacheSimulation = taskSpawn( "PIC_TacheSimulation", 
				PIC_PRIORITE_SIMULATION, 0, PIC_STACK_SIMULATION, 
				( FUNCPTR )PIC_DummyCapteur, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
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
		idTacheSimulation = NULL;
	}
}
