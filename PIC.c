/* Implementation du Pilote de Capteur
 */

/* TODO :
 * Definir joliment le type booleen (avec des macros et tout et tout)
 * eventuellement
 * un jour peut-être
 */

/* === INCLUDES === */

/* system includes */
#include "stdlib.h"
#include "msgQLib.h"
#include "timers.h"

/* project includes */
#include "PIC.h"

/* === DEFINITIONS DE CONSTANTES === */

#define PIC_N_CAPTEURS_MAX             ( 255 )
#define PIC_N_MESSAGES_MAX             ( 10 )
#define PIC_RAW_MSG_SIZE			   ( 4 )
#define NIVEAU_IT					   ( 2 * sizeof( int ) )


/* === DECLARATIONS DE TYPES DE DONNEES === */


/* === DONNEES STATIQUES === */

static int numDriver     = -1;
static int nombreDevices = 0;

static MSG_Q_ID   idBalDrv;
static int        idTacheScrutation;

/* Adresse du buffer de la carte reseau */
static char * msg_buff = NULL;

/* === PROTOTYPES DES FONCTIONS LOCALES === */

/* prototypes des primitives d'utilisation du PIC */

/******************************************************************************/
int PIC_Open
(
	char * const name,
	int    const flag
);

/******************************************************************************/
int PIC_Close
(
	char * const name
);

/******************************************************************************/
int PIC_Read
(
	int      fd,      /* descripteur de fichier que l'on veut lire */
	char   * buffer,  /* pointeur vers le buffer de stockage des données lues */
	size_t   maxbytes /* taille max. à lire */
);

/******************************************************************************/
int PIC_IoCtl
(
	int const fileDescriptor, 
	int const functionToCall,
	int const arguments
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
PIC_CR_INSTALL PIC_DrvInstall
(
	void
)
{
	int retour = deja_installe;
	
	if ( numDriver == -1 )
	{
		numDriver = iosDrvInstall( &PIC_Open,
								   &PIC_Close,
								   0,
								   0,
								   &PIC_Read,
								   0,
								   &PIC_IoCtl );

		if ( numDriver == ERROR )
		{
			retour = no_room;
		}
		else
		{
			retour = numDriver;
			
			PIC_DrvInit();
		}
	}
	
	return retour;
}

/******************************************************************************/
PIC_CR_REMOVE PIC_DrvRemove
(
	void
)
{
	int retour = pas_installe;
	
	if ( numDriver != -1 )
	{
		if ( iosDrvRemove( numDriver, TRUE ) == OK )
		{		
			numDriver = -1;
			
			retour = remove_ok;
		}
		else
		{
			retour = fichiers_ouverts;
		}
	}
	
	return retour;
}

/******************************************************************************/
PIC_CR_ADD PIC_DevAdd
(
	char * const name,
	int    const adresseCapteur
)
{
	PIC_HEADER * desc;
	
	if ( numDriver != -1 )
	{
		return driver_pas_installe;
	}
	
	/* TODO : checker si un device avec meme nom ou meme adresse n'existe
	 * pas deja. 
	 */
	
	if ( nombreDevices >= PIC_N_CAPTEURS_MAX )
	{
		return n_capteurs_overflow;
	}

	desc = ( PIC_HEADER * ) malloc( sizeof( PIC_HEADER ) );
	
	desc->specific.numero_driver = nombreDevices++;
	
	desc->specific.idBAL = msgQCreate( PIC_N_MESSAGES_MAX, sizeof( PIC_MESSAGE_CAPTEUR ), MSG_Q_FIFO );

	if ( desc->specific.idBAL == NULL )
	{
		nombreDevices--;
		
		free( desc );
		
		return no_room_add;
	}
	
	if ( iosDevAdd ( ( DEV_HDR * )desc, name, numDriver) == ERROR )
	{
		nombreDevices--;
		
		msgQDelete( desc->specific.idBAL );
		
		free( desc );
		
		return nom_pris;
	}
}

/******************************************************************************/
int PIC_DevDelete
(
	char * const name
)
{
	int retour = -1;
	
	DEV_HDR * pDevHdr;
	
	char * suite = malloc( sizeof( char ) );

	/* recherche du peripherique a supprimer */
	pDevHdr = iosDevFind( name, &suite );
	
	if ( ( pDevHdr!= NULL )  && ( suite[0] == '\0' ) )
	{
		msgQDelete( ( ( PIC_HEADER * )pDevHdr )->specific.idBAL );
		
		iosDevDelete( pDevHdr );
		
		free( pDevHdr );
		
		retour = 0;
	}
	
	free ( suite );
	
	return retour;
}

/******************************************************************************/
int PIC_Open
(
	char * const name,
	int    const flag
)
{
	/* TODO : Ici allouer eventuellement des trucs */
	return 0;
}

/******************************************************************************/
int PIC_Close
(
	char * const name
)
{
	/* TODO : Ici desallouer eventuellement des trucs*/
	return 0;
}

/******************************************************************************/
int PIC_Read
(
	int      fd,      /* file descriptor from which to read */
	char   * buffer,  /* pointer to buffer to receive bytes */
	size_t   maxbytes /* max no. of bytes to read into buffer */
)
{
	return 0;
}

/******************************************************************************/
int PIC_IoCtl
(
	int const fileDescriptor,
	int const functionToCall,
	int const arguments
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
	char * msg;

	sysIntDisable( NIVEAU_IT );
	msg = malloc( PIC_RAW_MSG_SIZE );
	memcpy( msg, msg_buff, PIC_RAW_MSG_SIZE );
	msgQSend( idBalDrv, msg, PIC_RAW_MSG_SIZE );
	sysIntEnable( NIVEAU_IT );	
}

/******************************************************************************/
void PIC_DrvInit
(
	void
)
{
	idBalDrv = msgQCreate( PIC_N_MESSAGES_MAX, PIC_RAW_MSG_SIZE, MSG_Q_FIFO );
	
	/* TODO : 
	 * - Lancer la tâche de scrutation
	 * - Initialiser le handler d'it
	 * - Initialiser le temps
	 */
}

/******************************************************************************/
void PIC_DrvConclude
(
	void
)
{
	msgQDelete( idBalDrv );
}
