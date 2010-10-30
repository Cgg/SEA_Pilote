/* Description d'un pilote de capteur :
 * 
 * prefixe des fonctions : PIC
 * 
 * copyright 2010 Lize / Geiger
 */

#ifndef PIC_H
#define PIC_H

/* === INCLUDES === */

/* system includes */
#include "iosLib.h"


/* === DECLARATIONS DE TYPES DE DONNEES === */

/* Définition de codes retours */
typedef enum 
{
	deja_installe,
	no_room
	
} PIC_CR_INSTALL;

typedef enum
{
	remove_ok,
	pas_installe,
	fichiers_ouverts
	
} PIC_CR_REMOVE;

typedef enum
{
	driver_pas_installe = -1,
	adresse_prise       = -2,
	nom_pris            = -3,
	n_capteurs_overflow = -4,
	no_room_add         = -5
	
} PIC_CR_ADD;

/* temps */
typedef struct
{
    time_t      tv_sec;         /* seconds */
    long        tv_nsec;        /* nanoseconds (0 -1,000,000,000) */

} timespec;

/* type du message delivre par un capteur. */
typedef int MESSAGE;

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


/* === PROTOTYPES DES FONCTIONS EXPOSEES === */

/* primitives de configuration du pilote */

/******************************************************************************/
PIC_CR_INSTALL PIC_DrvInstall
(
	void
);

/******************************************************************************/
PIC_CR_REMOVE PIC_DrvRemove
(
	void
);

/******************************************************************************/
PIC_CR_ADD PIC_DevAdd
(
	char * const name,
	int    const adresseCapteur
);

/******************************************************************************/
int PIC_DevDelete
(
	char * const name
);

#endif
/* PIC_H */
