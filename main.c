#include "PIC.h"
#include "PIC_ListeCapteurs.h"
#include "stdioLib.h"


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
	
	if( PIC_DrvInstall() == deja_installe )
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
			" - desinstallation avec fichiers ouverts\n"
			" - desinstallation normale apres installation\n"
			" - deuxieme desinstallation devant echouer.\n\n");
	
	PIC_DrvInstall();
	
	/* TODO : Test avec fichiers ouverts. */
	
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
	
	if( PIC_DevAdd( "essai1", 0 ) == driver_pas_installe )
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
	
	if ( PIC_DevAdd( "essai2", 0 ) == adresse_prise )
	{
		printf( "Ajout de deux capteurs ayant la meme adresse echoue.\n" );
	}
	else
	{
		printf( "Ajout de deux capteurs ayant la meme adresse reussi.\n" );
		return -1;
	}
	
	if( PIC_DevAdd( "essai1", 2 ) == nom_pris )
	{
		printf( "Ajout de deux capteurs ayant le meme nom echoue.\n" );
	}
	else
	{
		printf( "Ajout de deux capteurs ayant le meme nom reussi.\n" );
		return -1;
	}
 
	/* TODO : Essayer d'ajouter plus de 255 capteurs */
	
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
	
}
