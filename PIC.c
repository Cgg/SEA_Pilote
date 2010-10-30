/* Implementation du Pilote de Capteur
 */

/* TODO :
 * Definir joliment le type booleen (avec des macros et tout et tout)
 * eventuellement
 * un jour peut-être
 */

/* system includes */
#include "stdlib.h"
#include "msgQLib.h"
#include "timers.h"

/* project includes */
#include "PIC.h"

/* === DEFINITIONS CONSTANTES === */

#define PIC_N_CAPTEURS_MAX             ( 255 )
#define PIC_N_MESSAGES_MAX             ( 10 )


/* === DECLARATIONS DE TYPES DE DONNEES === */

/* temps */
typedef struct
{
    time_t      tv_sec;         /* seconds */
    long        tv_nsec;        /* nanoseconds (0 -1,000,000,000) */

} timespec;


/* type du message delivre par un capteur. */
typedef int MESSAGE;


/* Structure de donnees du tampon situe entre le handler d'it et la tache de
 * scrutation. 
 */
typedef struct
{
	/* Gestion des engorgements de messages */
	int   bufferPlein;
	int   nMessagesPerdus;
	
	/* Message en lui-meme */
	int       adCapteur;
	MESSAGE   message;
	
} PIC_BUF_TEMP;


/* Structure de donnees des messages stockes par chaque device en attendant un 
 * iosRead.
 */
typedef struct
{
	int        numMessage;
	timespec   tArrivee;
	MESSAGE    message;

} PIC_MESSAGE_CAPTEUR;


/* Structure de donnees specifique au PIC. Contient :
 * - idBAL : identifiant de la boite aux lettre ou sont stockes les messages 
 * envoyes par le capteur et non lu. Capacite max : 10 messages. 
 * - adresseCapteur : adresse du capteur fournie par l'utilisateur.
 * - numDriver : index du driver utilise par le device dans la table des drivers
 */
typedef struct
{
	MSG_Q_ID   idBAL;
	int        adresseCapteur;
	int        numero_driver;

} PIC_DATA_STRUCTURE;


/* Structure de donnees standard pour un pilote VxWorks */
typedef struct
{
	DEV_HDR              dev_hdr;
	PIC_DATA_STRUCTURE   specific;

} PIC_HEADER;


/* === DONNEES STATIQUES === */

static int numDriver     = -1;
static int nombreDevices = 0;

static PIC_BUF_TEMP * tamponItScrutation = NULL;


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
	int      fd,      /* file descriptor from which to read */
	char   * buffer,  /* pointer to buffer to receive bytes */
	size_t   maxbytes /* max no. of bytes to read into buffer */
);

/******************************************************************************/
int PIC_IoCtl
(
	int const fileDescriptor, 
	int const functionToCall,
	int const arguments
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
		 * pas deja. */
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
void PIC_DrvInit
(
	void
)
{
	tamponItScrutation = malloc( sizeof( PIC_BUF_TEMP ) );
	
	/* TODO : 
	 * - Lancer la tâche de scrutation
	 * - Initialiser le temps
	 */
}

/******************************************************************************/
void PIC_DrvConclude
(
	void
)
{
	free( tamponItScrutation );
}
