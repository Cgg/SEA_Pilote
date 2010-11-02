/* === INCLUDE === */

#include "stdioLib.h"
#include "timers.h"
#include "stdlib.h"

#include "PIC.h"
#include "PIC_ListeCapteurs.h"
#include "PIC_DummyCapteur.h"


/* === PROTOTYPES DES FONCTIONS LOCALES === */

/******************************************************************************/
int main
(
	void
);

/* Fonctions de test */

/******************************************************************************/
int PIC_TestInstallation
(
	void
);

/******************************************************************************/
int PIC_TestDesinstallation
(
	void
);

/******************************************************************************/
int PIC_TestAjout
(
	void
);

/******************************************************************************/
int PIC_TestEnlevement
(
	void
);


/* === IMPLEMENTATION === */

/******************************************************************************/
int main
(
	void
)
{
	if( PIC_TestInstallation() == 0 )
	{
		printf( "\nTest Installation reussi !\n\n" );
	}
	
	if( PIC_TestDesinstallation() == 0 )
	{
		printf( "\nTest Desinstallation reussi !\n\n");
	}
	
	if( PIC_TestAjout() == 0 )
	{
		printf( "\nTest Ajout reussi !\n\n" );
	}
	
	return 0;
}

/******************************************************************************/
int PIC_TestInstallation
(
	void
)
{
	int result = -1;
	
	printf( "======\n"
			"Test d'installation du driver.\n"
			" - installation normale\n"
			" - deuxieme installation devant echouer.\n\n");
	
	if( PIC_DrvInstall() != 0 )
	{		
		printf( "Installation normale reussie.\n" );
		result = 0;
	}
	else
	{
		printf( "Installation normale echouee.\n");
	}
	
	if( PIC_DrvInstall() == -1 )
	{
		printf( "Deuxieme installation echouee.\n" );
	}
	else
	{
		printf( "Deuxieme installation reussie.\n" );
		result = -1;
	}
	
	PIC_DrvRemove();
	
	return result;
}

/******************************************************************************/
int PIC_TestDesinstallation
(
	void
)
{
	int result = -1;
	
	printf( "======\n"
			"Test de desinstallation du driver.\n"
			" - desinstallation normale apres installation\n"
			" - deuxieme desinstallation devant echouer.\n\n");
	
	PIC_DrvInstall();
	
	if( PIC_DrvRemove() == 0 )
	{
		result = 0;
		
		printf( "Desinstallation normale reussi.\n" );
	}
	else
	{
		printf( "Desinstallation normale echouee.\n" );
	}
	
	if( PIC_DrvRemove() != 0 )
	{
		printf( "Deuxieme desinstallation echouee.\n" );
	}
	else
	{
		printf( "Deuxieme desinstallation reussie.\n" );
		result = -1;
	}
	
	return result;
}

/******************************************************************************/
int PIC_TestAjout
(
	void
)
{
	printf( "======\n"
			"Test d'ajouts de device.\n"
			" - ajout normal\n"
			" - ajout sans installation du driver correspondant\n"
			" - ajout de deux capteurs ayant la meme adresse\n"
			" - ajout de deux capteurs ayant le meme nom\n"
			" - ajout de plus de 255 capteurs.\n\n");
	
	PIC_DrvInstall();
	
	if( PIC_DevAdd( "essai1", 0 ) > 0 )
	{
		printf( "Ajout normal reussi.\n");
	}
	else
	{
		printf( "Ajout normal echoue" );
		return -1;
	}
	
	PIC_DevDelete( "essai1" );
	
	PIC_DrvRemove();
	
	if( PIC_DevAdd( "essai1", 0 ) == -1 )
	{
		printf( "Ajout sans driver installe echoue.\n" );
	}
	else
	{
		printf( "Ajout sans driver installe reussie.\n" );
		return -1;
	}
	
	PIC_DrvInstall();
	
	PIC_DevAdd( "essai1", 0 );
	
	if ( PIC_DevAdd( "essai2", 0 ) == -1 )
	{
		printf( "Ajout de deux capteurs ayant la meme adresse echoue.\n" );
	}
	else
	{
		printf( "Ajout de deux capteurs ayant la meme adresse reussi.\n" );
		return -1;
	}
	
	if( PIC_DevAdd( "essai1", 2 ) == -1 )
	{
		printf( "Ajout de deux capteurs ayant le meme nom echoue.\n" );
	}
	else
	{
		printf( "Ajout de deux capteurs ayant le meme nom reussi.\n" );
		return -1;
	}
 
	/* TODO : Essayer d'ajouter plus de 15 capteurs */
	
	PIC_DevDelete( "essai1" );
	
	PIC_DrvRemove();
	
	return 0;
}

/******************************************************************************/
int PIC_TestEnlevement
(
	void
)
{
	printf( "======\n"
			"Test d'enlevements de device.\n"
			" - enlevement normal\n"
			" - enlevement d'un device inexistant.\n\n");
	
	PIC_DrvInstall();
	
	PIC_DevAdd( "a", 42 );
	
	if( PIC_DevDelete( "a" ) == 0 )
	{
		printf( "Enlevement normal reussi." );
	}
	else
	{
		printf( "Enlevement normal echoue." );
		return -1;
	}
	
	if( PIC_DevDelete( "a" ) == 0 )
	{
		printf( "Enlevement d'un device inexistant echoue." );
	}
	else
	{
		printf( "Enlevement d'un device inexistant reussi." );
		return -1;
	}
	
	PIC_DrvRemove();
	
	return 0;
}

int q
(
	void
)
{
	int fdC1;
	int fdC2;
	int i;
	
	struct timespec time;
	
	PIC_MESSAGE_CAPTEUR * messageCapteur = 
			( PIC_MESSAGE_CAPTEUR * )malloc( PIC_TAILLE_MSG_TRAITE );
	
	char * tabAdresseCapteurs = malloc( 2 * sizeof( char ) );
	
	time.tv_nsec = 0;
	time.tv_sec  = 1;
	
	PIC_DrvInstall();
	
	PIC_DevAdd( "A", 65 );
	PIC_DevAdd( "B", 66 );
	
	fdC1 = open( "A", O_RDONLY, 777 );
	fdC2 = open( "B", O_RDONLY, 777 );
	
	tabAdresseCapteurs[ 0 ] = 65;
	tabAdresseCapteurs[ 1 ] = 66;
	
	for( i = 2 ; i < PIC_N_CAPTEURS_MAX ; i++ )
	{
		tabAdresseCapteurs[ i ] = 0;
	}
	
	PIC_SimStart( tabAdresseCapteurs, 2 );
	
	for( i = 0 ; i < 10 ; i++ )
	{
		printf( "lecture du capteur d'adresse 15 puis 17.\n");
		
		printf( "%d bytes lus du capteur 15.\n", read( fdC1,
				( char * )messageCapteur, PIC_TAILLE_MSG_TRAITE ) );
		
		printf( "message : %d.\n", ( int )( messageCapteur->message ) );
		
		printf( "%d bytes lus du capteur 17.\n", read( fdC2,
				( char * )messageCapteur, PIC_TAILLE_MSG_TRAITE ) );
		
		printf( "message : %d.\n", ( int )( messageCapteur->message ) );
		
		nanosleep( &time, NULL );
	}	

	close( fdC1 );
	close( fdC2 );
	
	PIC_SimStop();
	
	PIC_DevDelete( "A" );
	PIC_DevDelete( "B" );
	
	PIC_DrvRemove();
	return 0;
}
