#include "PIC.h"
#include "PIC_ListeCapteurs.h"
#include "stdioLib.h"


/* === PROTOTYPES DES FONCTIONS LOCALES === */

/* Fonctions de test */

/******************************************************************************/
int PIC_TestInstallation
(
	void
)
{
	int result = -1;
	
	printf( "Test d'installation du driver.\n"
			" - installation normale\n"
			" - deuxieme installation devant echouer.\n\n");
	
	if( PIC_DrvInstall() == 0 )
	{
		printf( "Installation normale reussie.\n" );
		
		result = 0;
	}
	
	if( PIC_DrvInstall() != 0 )
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
	
	printf( "Test de desinstallation du driver.\n"
			" - desinstallation avec fichiers ouverts\n"
			" - desinstallation normale apres installation\n"
			" - deuxieme desinstallation devant echouer.\n\n");
}

/******************************************************************************/
int TestAjout
(
	void
)
{
	
}

void main
(
	void
)
{
	PIC_CR_INSTALL   crInstall;
	PIC_CR_REMOVE    crRemove;

	crInstall = PIC_DrvInstall();

	printf( "Installation du PIC : %d \n", crInstall );
	
	AfficherCapteurs();
	
	crRemove = PIC_DrvRemove();

	printf( "desinstallation du PIC : %d \n", crRemove );
}
