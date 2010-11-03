/* Description d'un pilote de capteur :
 * 
 * prefixe des fonctions : PIC
 * 
 * copyright 2010 Lize / Geiger
 */

#ifndef PIC_H
#define PIC_H

/* === INCLUSIONS === */

/* inclusions systeme */
#include "iosLib.h"
#include "msgQLib.h"
#include "timers.h"


 /* === DECLARATION DE CONSTANTES */

#define PIC_TAILLE_MSG_BRUTE           ( sizeof( PIC_MESSAGE_BRUTE ) )
#define PIC_TAILLE_MSG_TRAITE          ( sizeof( PIC_MESSAGE_CAPTEUR ) )

#define PIC_N_MESSAGES_MAX             ( 10 )

#define PIC_N_CAPTEURS_MAX             ( 15 )

#define PIC_E_TOOMANYDEV			   ( 90 )
#define PIC_E_PARAM_INCORRECTS         ( 91 )
#define PIC_E_PIC_DEJA_INSTALLE        ( 92 )
#define PIC_E_PIC_PAS_INSTALLE         ( 93 )
#define PIC_E_DEV_DEJA_PRESENT         ( 94 )
#define PIC_E_DEV_NON_PRESENT          ( 95 )

#define PIC_IOCTL_FONCTION_HOT_SWAP    ( 1 )

/* === DECLARATIONS DE TYPES DE DONNEES === */

/* temps */
typedef struct timespec TIMESTAMP;

/* type du message delivre par un capteur. */
typedef int MESSAGE;

typedef struct
{
	char      adresseCapteur;
	MESSAGE   message;
	
} PIC_MESSAGE_BRUTE;

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
	MSG_Q_ID   idBAL;
	char       adresseCapteur;
	int        numero_driver;

} PIC_DATA_STRUCTURE;


/* Structure de donnees standard pour un pilote VxWorks */
typedef struct
{
	DEV_HDR              dev_hdr;
	PIC_DATA_STRUCTURE   specific;

} PIC_HEADER;


/* === PROTOTYPES DES FONCTIONS EXPOSEES === */

/* primitives de configuration du pilote */

/******************************************************************************/
int PIC_DrvInstall
(
	void
);

/******************************************************************************/
int PIC_DrvRemove
(
	void
);

/******************************************************************************/
int PIC_DevAdd
(
	char * const name,
	char   const adresseCapteur
);

/******************************************************************************/
int PIC_DevDelete
(
	char * const name
);

/******************************************************************************/
MSG_Q_ID PIC_RecupererBal
(
	void
);

#endif
/* PIC_H */
