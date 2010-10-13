/* Implementation du Pilote de Capteur
 */

/* TODO :
 * Definir joliment le type booleen (avec des macros et tout et tout)
 */

/* system includes */
#include "stdlib.h"
#include "timers.h"

/* project includes */
#include "PIC.h"


/* DECLARATIONS DE TYPES DE DONNEES */

/* type du message delivre par un capteur. */
typedef int MESSAGE;

typedef timespec TIMESTAMP;

/* Structure de donnees des messages stockes par chaque device en attendant un 
 * iosRead.
 */
typedef struct
{
	int         numMessage;
	TIMESTAMP   tArrivee;
	MESSAGE     message;

} PIC_MESSAGE_CAPTEUR;

/* Structure de donnees specifique au PIC. Contient :
 * - idBAL : identifiant de la boite aux lettre ou sont stockes les messages 
 * envoyes par le capteur et non lu. Capacite max : 10 messages. 
 * - adresseCapteur : adresse du capteur fournie par l'utilisateur.
 * - numDriver : index du driver utilise par le device dans la table des drivers
 */
typedef struct
{
	int   idBAL;
	int   adresseCapteur;
	int   numero_driver;
	
} PIC_DATA_STRUCTURE;

/* Structure de donnees standard pour un pilote VxWorks */
typedef struct
{
	DEV_HDR              dev_hdr;
	PIC_DATA_STRUCTURE   specific;

} PIC_HEADER;


/* DONNEES STATIQUES */

static int numDriver     = -1;
static int nombreDevices = 0;


/* PROTOTYPES DES FONCTIONS LOCALES */

/* prototypes des primitives d'utilisation du PIC 
 * 
 */

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


/* IMPLEMENTATION */

/******************************************************************************/
int PIC_DrvInstall
(
	void
)
{
	int retour = -1;
	
	if ( numDriver == -1 )
	{
		numDriver = iosDrvInstall( &PIC_Open,
								   &PIC_Close,
								   0,
								   0,
								   &PIC_Read,
								   0,
								   &PIC_IoCtl );

		retour = numDriver;
		
		/* TODO : Initialiser le temps */
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
		iosDrvRemove( numDriver, TRUE );
		
		numDriver = -1;
		
		retour = 0;
	}
	
	return retour;
}

/******************************************************************************/
int PIC_DevAdd
(
	char * const name,
	int    const adresseCapteur
)
{
	int retour = -1;
	
	if ( numDriver != -1 )
	{
		/* TODO : checker si un device qvec meme nom ou meme adresse n'existe
		 * pas deja. */
		WDR_HEADER * desc = ( WDR_HEADER * ) malloc( sizeof( WDR_HEADER ) );
		
		( desc->specific ).numero_driver = nombreDevices++ ;

		iosDevAdd ( ( DEV_HDR * )desc, name, numDriver);
		
		retour = nombreDevices ;
	}

	return retour;
}

/******************************************************************************/
int PIC_DevDelete
(
	char * const name
)
{
	int retour = -1;
	
	DEV_HDR * pDevHdr;
	
	char suite[1];

	/* recherche du peripherique a supprimer */
	pDevHdr = iosDevFind( name, &suite ); 
	
	if ( ( pDevHdr!= NULL )  && ( suite[0] == '\0' ) )
	{
		iosDevDelete( pDevHdr );
		free( pDevHdr );
		
		retour = 0;
	}
	
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
