/* Implementation du Pilote de Capteur */

/* === INCLUSIONS === */

/* inclusions systeme */
#include "stdlib.h"
#include "taskLib.h"
#include "sysLib.h"
#include "intLib.h"
#include "errnoLib.h"
#include "string.h"
#include "stdio.h"

/* inclusions projet */
#include "PIC.h"
#include "PIC_TacheScrutation.h"
#include "PIC_ListeCapteurs.h"

/* === DEFINITIONS DE CONSTANTES === */

#define NIVEAU_IT					   ( 42 )
#define PIC_VECTEUR_IT                 ( 0x666 )


/* === DECLARATIONS DE TYPES DE DONNEES === */


/* === DONNEES STATIQUES === */

static int numDriver     = -1;
static int nombreDevices = 0;

static MSG_Q_ID   idBalDrv;
static int        idTacheScrutation;

/* Adresse du buffer de la carte reseau */
static char * msgBuff = NULL;

/* Tableau de pointeurs sur les capteurs ajoutes */
static PIC_HEADER * * tabPointeurs = NULL;

/* === PROTOTYPES DES FONCTIONS LOCALES === */

/* prototypes des primitives d'utilisation du PIC */

/******************************************************************************/
int PIC_Open
(
	PIC_HEADER * desc,
	char       * remainder,
	int          mode
);

/******************************************************************************/
int PIC_Close
(
	PIC_HEADER * desc
);

/******************************************************************************/
int PIC_Read
(
	PIC_HEADER * dev,      /* device from which to read */
	char       * buffer,   /* pointer to buffer to receive bytes */
	size_t       maxbytes  /* max no. of bytes to read into buffer */
);

/******************************************************************************/
int PIC_IoCtl
(
	PIC_HEADER * desc,
	int          fonction,
	int          arg
);

/* Handler d'interruptions envoyees par la carte des capteurs */

/******************************************************************************/
int PIC_HandlerIT
(
	void
);

/* prototype des autres fonctions locales */

/******************************************************************************/
void PIC_DrvInit
(
	void
);

/******************************************************************************/
void PIC_DrvConclude
(
	void
);


/* === IMPLEMENTATION === */

/******************************************************************************/
int PIC_DrvInstall
(
	void
)
{
	int retour = -1;
	
	if ( numDriver == -1 )
	{
		numDriver = iosDrvInstall( 0,
								   0,
								   &PIC_Open,
								   &PIC_Close,
								   &PIC_Read,
								   0,
								   &PIC_IoCtl );

		if ( numDriver != ERROR )
		{
			retour = numDriver;
			
			PIC_DrvInit();
		}
	}
	
	return retour;
}

/******************************************************************************/
int PIC_DrvRemove
(
	void
)
{
	int retour = -1;
	
	if ( numDriver != -1 )
	{
		if ( iosDrvRemove( numDriver, TRUE ) == OK )
		{		
			numDriver = -1;
			
			PIC_DrvConclude();
			
			retour = 0;
		}
	}
	
	return retour;
}

/******************************************************************************/
int PIC_DevAdd
(
	char * const name,
	char   const adresseCapteur
)
{
	PIC_HEADER * desc;
	
	int i;
	
	if ( numDriver == -1 )
	{
		return -1;
	}

	
	for ( i = 0 ; i < PIC_N_CAPTEURS_MAX ; i++ )
	{
		if ( tabPointeurs[ i ] != NULL && 
				tabPointeurs[ i ]->specific.adresseCapteur == adresseCapteur )
		{
			return -1;
		}
	}
	
	if ( nombreDevices >= PIC_N_CAPTEURS_MAX )
	{
		errnoSet( PIC_E_TOOMANYDEV );

		return -1;
	}

	desc = ( PIC_HEADER * ) malloc( sizeof( PIC_HEADER ) );
	
	desc->specific.adresseCapteur = adresseCapteur;
	desc->specific.numero_driver  = nombreDevices++;
	
	desc->specific.idBAL = msgQCreate( PIC_N_MESSAGES_MAX, 
			sizeof( PIC_MESSAGE_CAPTEUR ), MSG_Q_FIFO );

	if ( desc->specific.idBAL == NULL )
	{
		nombreDevices--;
		
		free( desc );
		
		return -1;
	}
	
	if ( iosDevAdd ( ( DEV_HDR * )desc, name, numDriver) == ERROR )
	{
		nombreDevices--;
		
		msgQDelete( desc->specific.idBAL );
		
		free( desc );
		
		return -1;
	}
	
	for( i = 0 ; i < PIC_N_CAPTEURS_MAX ; i++ )
	{
		if( tabPointeurs[ i ] == NULL )
		{
			tabPointeurs[ i ] = desc;
			
			return nombreDevices;
		}
	}
	
	return nombreDevices;
}

/******************************************************************************/
int PIC_DevDelete
(
	char * const name
)
{
	int   i = 0;
	int   retour = -1;
	
	DEV_HDR * pDevHdr;
	
	char * * suite;

	/* recherche du peripherique a supprimer */
	pDevHdr = iosDevFind( name, suite );
	
	if ( ( pDevHdr != NULL )  && ( *suite[ 0 ] == '\0' ) )
	{
		nombreDevices--;
		
		msgQDelete( ( ( PIC_HEADER * )pDevHdr )->specific.idBAL );
		
		while( i < PIC_N_CAPTEURS_MAX )
		{
			if( tabPointeurs[ i ] != NULL && tabPointeurs[ i ]->specific.adresseCapteur == ( ( PIC_HEADER * )pDevHdr )->specific.adresseCapteur )
			{
				tabPointeurs[ i ] = NULL;
				
				i = PIC_N_CAPTEURS_MAX;
			}
			
			i++;
		}
		
		iosDevDelete( pDevHdr );
		
		retour = 0;
	}
		
	return retour;
}

/******************************************************************************/
int PIC_Open
(
	PIC_HEADER * desc,
	char       * remainder,
	int          mode
)
{
	if ( *remainder != '\0' )
	{
		return ERROR;
	}
	else
	{
		return ( ( int ) desc ) ;
	}
}

/******************************************************************************/
int PIC_Close
(
	PIC_HEADER * desc
)
{
	return 0;
}

/******************************************************************************/
int PIC_Read
(
	PIC_HEADER * dev,      /* device from which to read */
	char       * buffer,   /* pointer to buffer to receive bytes */
	size_t       maxbytes  /* max no. of bytes to read into buffer */
)
{
	if ( sizeof( buffer ) != PIC_TAILLE_MSG_TRAITE || 
		 maxbytes != PIC_TAILLE_MSG_TRAITE )
	{
		errnoSet( PIC_E_PARAM_INCORRECTS );

		return -1;
	}
	
	msgQReceive( dev->specific.idBAL, buffer, maxbytes, NO_WAIT );
	
	return 0;
}

/******************************************************************************/
int PIC_IoCtl
(
	PIC_HEADER * desc,
	int          fonction,
	int          arg
)
{
	return 0;
}

/******************************************************************************/
int PIC_HandlerIT
(
	void
)
{
	sysIntDisable( NIVEAU_IT );
	
	msgQSend( idBalDrv, ( char * )msgBuff, PIC_TAILLE_MSG_BRUTE, NO_WAIT, MSG_PRI_NORMAL );
	
	sysIntEnable( NIVEAU_IT );
}

/******************************************************************************/
void PIC_DrvInit
(
	void
)
{
	int i;
	
	TIMESTAMP clockInit;
	
	tabPointeurs = malloc( PIC_N_CAPTEURS_MAX * sizeof( PIC_HEADER * ) );
	
	for( i = 0 ; i < PIC_N_CAPTEURS_MAX ; i++ )
	{
		tabPointeurs[ i ] = NULL;
	}
	
	idBalDrv = msgQCreate( PIC_N_MESSAGES_MAX, PIC_TAILLE_MSG_BRUTE, MSG_Q_FIFO );
	
	idTacheScrutation = taskSpawn( "PIC_TacheScrutation", 
			PIC_PRIORITE_SCRUTATION, 0, PIC_STACK_SCRUTATION, 
			( FUNCPTR )PIC_TacheScrutation, ( int ) idBalDrv, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
	
	intConnect( ( VOIDFUNCPTR * )PIC_VECTEUR_IT, ( VOIDFUNCPTR )PIC_HandlerIT, 0 );
	
	clockInit.tv_sec  = 0;
	clockInit.tv_nsec = 0;
	
	clock_settime( CLOCK_REALTIME, &clockInit );
}

/******************************************************************************/
void PIC_DrvConclude
(
	void
)
{
	taskDelete( idTacheScrutation );
	
	msgQDelete( idBalDrv );
	
	free( tabPointeurs );
}	

/******************************************************************************/
MSG_Q_ID PIC_RecupererBal
(
	void
)
{
	return idBalDrv;
}
