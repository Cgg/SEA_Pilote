#include "PIC.h"
#include "PIC_ListeCapteurs.h"
#include "stdioLib.h"

void main
(
	void
)
{
	PIC_CR_INSTALL   crInstall;
	PIC_CR_REMOVE    crRemove;

	iosDrvShow();

	crInstall = PIC_DrvInstall();

	printf( "Installation du PIC : %d \n", crInstall );
	
	iosDrvShow();

	crRemove = PIC_DrvRemove();

	printf( "desinstallation du PIC : %d \n", crRemove );

	crRemove = PIC_DrvRemove();

	printf( "desinstallation du PIC : %d \n", crRemove );
	
	iosDrvShow();
}
