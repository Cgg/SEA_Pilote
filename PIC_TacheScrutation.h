/* Pilote de capteur :
 * 
 * Description de la tâche de scrutation des messages de capteur.
 * 
 * Le role de cette tache est de scruter les messages en provenance des capteurs
 * physiques. Lorsqu'un message arrive, il declenche une interruption. Le 
 * handler d'it place le message dans un buffer intermediaire ou cette tache 
 * vient le recuperer. Elle attache un numero de message et un timestamp au 
 * message et le place dans la boite au lettre correspondante au capteur.
 *
 * copyright 2010 Lize / Geiger
 */

#ifndef PIC_TACHE_SCRUTATION
#define PIC_TACHE_SCRUTATION

/* === INCLUDES === */
#include "msgQLib.h"

/* === PROTOTYPES DES FONCTIONS EXPOSEES === */

/******************************************************************************/
int PIC_TacheScrutation
(
	MSG_Q_ID const idBalDrv
	/* a passer egalement : pointeur vers la liste/le tableau des capteurs 
	 * installes 
	 */
);

#endif
/* PIC_TACHE_SCRUTATION */
