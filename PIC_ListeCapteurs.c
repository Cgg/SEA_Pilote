#include "PIC_ListeCapteurs.h"
#include "stdlib.h"
#include "stdioLib.h"

#define T_ENTREE                       ( sizeof( ENTREE ) )

typedef struct entree
{
	struct entree * entreePrecedente;
	struct entree * entreeSuivante;
	PIC_HEADER    * capteur;

} ENTREE;

static ENTREE * teteDeListe = NULL;
static ENTREE * finDeListe  = NULL;

/******************************************************************************/
ENTREE * ChercherEntree
(
	char const adresseCapteur
);

/******************************************************************************/
int InitialiserListe
(
		PIC_HEADER * const capteur
);

/******************************************************************************/
int AjouterCapteur
(
	PIC_HEADER * const capteur
)
{
	int retour = -1;
	
	ENTREE * nouvelleEntree;

	if( teteDeListe == NULL )
	{
		return InitialiserListe( capteur );
	}
	
	if( ChercherEntree( capteur->specific.adresseCapteur ) == NULL )
	{
		nouvelleEntree = malloc( T_ENTREE );
		
		nouvelleEntree->capteur          = capteur;
		nouvelleEntree->entreePrecedente = finDeListe;
		nouvelleEntree->entreeSuivante   = NULL;
		
		finDeListe->entreeSuivante = nouvelleEntree;
		
		finDeListe = nouvelleEntree;
		
		retour = 0;
	}
	
	return retour;
}

/******************************************************************************/
int RetirerCapteur
(
	char const adresseCapteur
)
{
	ENTREE * entree = ChercherEntree( adresseCapteur );
	
	if ( entree == NULL )
	{
		return -1;
	}
	
	entree->entreePrecedente->entreeSuivante = entree->entreeSuivante;
	entree->entreeSuivante->entreePrecedente = entree->entreePrecedente;
	
	free( entree );
	
	return 0;
}

/******************************************************************************/
PIC_HEADER * ChercherCapteur
(
	char const adresseCapteur
)
{
	ENTREE * entree = ChercherEntree( adresseCapteur );
	
	return ( entree != NULL ? entree->capteur : NULL );
}

/******************************************************************************/
void AfficherCapteurs
(
	void
)
{
	ENTREE * presente = teteDeListe;

	while( presente != NULL )
	{
		printf( "%c\n",presente->capteur->specific.adresseCapteur );
		
		presente = presente->entreeSuivante;
	}

	printf( "Rien a afficher.\n" );
}

/******************************************************************************/
ENTREE * ChercherEntree
(
	char const adresseCapteur
)
{
	ENTREE * presente;
	ENTREE * suivante;
	
	if( teteDeListe == NULL )
	{
		return NULL;
	}
	else
	{
		presente = teteDeListe;
		suivante = teteDeListe->entreeSuivante;
	}
	
	if( presente->capteur->specific.adresseCapteur == adresseCapteur )
	{
		return presente;
	}
	
	while( suivante != NULL )
	{
		presente = suivante;
		suivante = presente->entreeSuivante;
		
		if( presente->capteur->specific.adresseCapteur == adresseCapteur )
		{
			return presente;
		}
	}
	
	return NULL;
}

/******************************************************************************/
int InitialiserListe
(
	PIC_HEADER * const capteur
)
{
	teteDeListe = malloc( sizeof( T_ENTREE ) );
	
	teteDeListe->capteur          = capteur;
	teteDeListe->entreeSuivante   = NULL;
	teteDeListe->entreePrecedente = NULL;
	
	finDeListe = teteDeListe;
}
