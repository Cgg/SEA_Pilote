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
		printf( "\n Test Desinstallation reussi !\n\n");
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
		result = 0;
		
		printf( "Installation normale reussie.\n" );
	}
	
	if( PIC_DrvInstall() == deja_installe )
	{
		printf( "Deuxieme installation echouee.\n" );
	}
	else
	{
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
	
	if( PIC_DrvRemove() != 0 )
	{
		printf( "Deuxieme desinstallation echouee.\n" );
	}
	else
	{
		result = -1;
	}
	
	return result;
}
