// JAOUANNE Lilian et GARCON Bastian 20/10/2022
// Projet de GIF 1301

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Declaration des types
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////// Liste des villes ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	typedef struct ville
    {
		int codePostal;
		char nomVille[50];
		struct ville * villeSuivante;
	} Tville;

	typedef Tville * PTville;
	
/////////////////////////////////// Liste des lignes electriques //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	typedef struct lignesElectrique
    {
		int puissance;
		PTville villeDesservie;	
		struct lignesElectrique * ligneSuivante ;
	} TlignesElectrique;
	
	typedef TlignesElectrique * PTligneElectrique;
	
//////////////////////////////////////// Liste des centrales //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	typedef struct centrale
    {
		int codeCentrale; 
		PTligneElectrique villeDependante; // Pointeur sur la liste des lignes
		// Pointeur sur le bidon des lignes electriques de la centrale.
		// pfinLigne NE DESSER PAS DE VILLES !!!
		PTligneElectrique pfinLigne;
		struct centrale * ptsuivant;
		struct centrale * ptprecedent;
	}Tcentrale;
	
	typedef Tcentrale * PTcentrale;

//////////////////////// Structure contenant tout les bidons des listes poublelles ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct ben
{
	PTville pdebutV;
	PTville pfinV;
	PTcentrale pdebutC;
	PTcentrale pfinC;
	PTligneElectrique pdebutL;
	PTligneElectrique pfinL;
}Tben;

typedef Tben *PTben;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           Fonctions
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Met le curseur a la ligne "lig" et a la colonne "col" //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void gotoligcol(int lig , int col)
{
	COORD mycoord;
		
	mycoord.X = col;
	mycoord.Y = lig;
	SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ) , mycoord);
}

// Supprimer tout ce qu'il y a a l ecran //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ClearConsole(HANDLE hConsole){
	system("cls");//clean la console
	SetConsoleTextAttribute(hConsole, 16 * 2 + 15);//vert fonce sur noir
	gotoligcol(0,3);
	printf("---- Gestion du reseau electrique ----\n\n");
	SetConsoleTextAttribute(hConsole, 16 * 0 + 15);//blanc sur noir
}

// Rempli un rectangle de hauteur "haut" et de largeur "larg" avec le caractere "c" a partir de la ligne "lig" et de la colonne "col" /////////////////////////////////////////////////////////////////
void FillSquare(int lig, int col, int haut, int larg, char c){
	int i , j;
	gotoligcol(lig,col);
	i = 0; j = 0;
	while(i < haut){
		j = 0;
		while(j < larg){
			printf("%c", c);
			j++;
		}
		i++;
		gotoligcol(lig+i,col);
	}
}

// Efface "n" lignes de l'interface à partir de la ligne "lig" et remet le curseur sur la ligne "lig" /////////////////////////////////////////////////////////////////////////////////////////////////
void EmptyLine(int lig, int n){
	int i = 0;
	gotoligcol(lig , 0);
	while(i < n){
		printf("\33[2K\r\n");
		i = i + 1;
	}
	gotoligcol(lig , 0);
}

// Cree un Cadre de hauteur interieur "haut" et de largeur interieur "larg" a partir de la ligne "lig" et de la colonne "col" /////////////////////////////////////////////////////////////////////////
void Cadre(int lig, int col, int haut, int larg){
	int i, j;
	char c, cvertical, chorizontal, cangleGH, cangleGB, cangleDH, cangleDB;
	cvertical = 186;
   	chorizontal = 205;
   	cangleGH = 201;
   	cangleGB = 200;
   	cangleDH = 187;
   	cangleDB = 188;
	   	
	//ligne haut
    gotoligcol(lig,col);
	printf("%c",cangleGH);
	i = larg; 
	while(i > 0){	
		printf("%c",chorizontal);
		i = i - 1;
	};
	printf("%c" , cangleDH);
	// ligne intermediaires
	j = haut;
	
	while(j >= 0){
		gotoligcol(lig + 1 + j, col);	
		printf("%c" , cvertical);
		i = larg; 
		while(i > 0)	{	
			printf("%c",' ');
			i = i - 1;
		};
		j = j - 1;
		printf("%c" , cvertical);
	}
	//ligne bas
    gotoligcol(lig + 1 + haut , col);
	printf("%c" , cangleGB);
	i = larg; 
	while(i > 0)	{	
		printf("%c" , chorizontal);
		i = i - 1;
	};
	printf("%c" , cangleDB);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Affiche(HANDLE hConsole , char c , int fond , int couleur)
{
	int i;
		
	i = 16 * fond + couleur;
	SetConsoleTextAttribute(hConsole , i);
	printf("%c " , c);
}

// Afficher les MENUS /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AfficheMenu(HANDLE hConsole, int select)
{	
		ClearConsole(hConsole);
		SetConsoleTextAttribute(hConsole, 16 * 7 + 0);//noir sur gris clair
		gotoligcol(5,5);printf("*********************************");
		gotoligcol(6,5);printf("*             MENU              *");
		gotoligcol(7,5);printf("*********************************");		
		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);
		
		gotoligcol(10,9);printf("1 - ");
		if(select==0)SetConsoleTextAttribute(hConsole, 16 * 8 + 0);//noir sur gris foncé
		printf("Gestion des centrales");
		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);
		
		gotoligcol(11,9);printf("2 - ");
		if(select==1)SetConsoleTextAttribute(hConsole, 16 * 8 + 0);//noir sur gris foncé
		printf("Gestion des villes");
		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);
		
		gotoligcol(12,9);printf("3 - ");
		if(select==2)SetConsoleTextAttribute(hConsole, 16 * 8 + 0);//noir sur gris foncé
		printf("Gestion des lignes electriques");
		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);
		
		gotoligcol(13,9);printf("4 - ");
		if(select==3)SetConsoleTextAttribute(hConsole, 16 * 8 + 0);//noir sur gris foncé
		printf("Gestion du reseau");
		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);
		
		gotoligcol(15,9);printf("ESC - ");
		if(select==4)SetConsoleTextAttribute(hConsole, 16 * 8 + 0);//noir sur gris foncé
		printf("Quitter");
		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);
		
		SetConsoleTextAttribute(hConsole, 16 * 0 + 8);//gris sur noir
		printf("\n\n\n\n\tNaviguer avec les fleches 'Haut' et 'Bas' du clavier et selectionner avec 'Entrer'");
		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);//blanc sur noir
}

void AfficheMenuCentrales(HANDLE hConsole, int select)
{	
		ClearConsole(hConsole);
		SetConsoleTextAttribute(hConsole, 16 * 7 + 0);//noir sur gris clair
		gotoligcol(6,5);printf("---   Gestion Des Centrales   ---");	
		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);
		
		gotoligcol(10,9);printf("1 - ");
		if(select==0)SetConsoleTextAttribute(hConsole, 16 * 8 + 0);//noir sur gris foncé
		printf("Ajouter des centrales");
		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);
		
		gotoligcol(11,9);printf("2 - ");
		if(select==1)SetConsoleTextAttribute(hConsole, 16 * 8 + 0);//noir sur gris foncé
		printf("Supprimer une centrale");
		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);
		
		gotoligcol(12,9);printf("3 - ");
		if(select==2)SetConsoleTextAttribute(hConsole, 16 * 8 + 0);//noir sur gris foncé
		printf("Afficher les centrales");
		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);
		
		gotoligcol(14,9);printf("ESC - ");
		if(select==3)SetConsoleTextAttribute(hConsole, 16 * 8 + 0);//noir sur gris foncé
		printf("Retour");
		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);
		
		SetConsoleTextAttribute(hConsole, 16 * 0 + 8);//gris sur noir
		printf("\n\n\n\n\tNaviguer avec les fleches 'Haut' et 'Bas' du clavier et selectionner avec 'Entrer'");
		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);//blanc sur noir
}

void AfficheMenuVilles(HANDLE hConsole, int select)
{	
		ClearConsole(hConsole);
		SetConsoleTextAttribute(hConsole, 16 * 7 + 0);//noir sur gris clair
		gotoligcol(6,5);printf("---   Gestion Des Villes   ---");	
		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);
		
		gotoligcol(10,9);printf("1 - ");	
		if(select==0)SetConsoleTextAttribute(hConsole, 16 * 8 + 0);//noir sur gris foncé
		printf("Ajouter des villes");	
		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);
	
		gotoligcol(11,9);printf("2 - ");	
		if(select==1)SetConsoleTextAttribute(hConsole, 16 * 8 + 0);//noir sur gris foncé
		printf("Supprimer une ville");
		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);
		
		gotoligcol(12,9);printf("3 - ");
		if(select==2)SetConsoleTextAttribute(hConsole, 16 * 8 + 0);//noir sur gris foncé
		printf("Afficher les villes");
		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);
		
		gotoligcol(14,9);printf("ESC - ");
		if(select==3)SetConsoleTextAttribute(hConsole, 16 * 8 + 0);//noir sur gris foncé
		printf("Retour");
		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);
		
		SetConsoleTextAttribute(hConsole, 16 * 0 + 8);//gris sur noir
		printf("\n\n\n\n\tNaviguer avec les fleches 'Haut' et 'Bas' du clavier et selectionner avec 'Entrer'");
		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);//blanc sur noir
}

void AfficheMenuLignes(HANDLE hConsole, int select)
{	
		ClearConsole(hConsole);
		SetConsoleTextAttribute(hConsole, 16 * 7 + 0);//noir sur gris clair
		gotoligcol(6,5);printf("--- Gestion Des Lignes Electriques ---");
		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);
		
		gotoligcol(10,9);printf("1 - ");	
		if(select==0)SetConsoleTextAttribute(hConsole, 16 * 8 + 0);//noir sur gris foncé
		printf("Ajouter une ligne electrique");	
		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);
		
		gotoligcol(11,9);printf("2 - ");
		if(select==1)SetConsoleTextAttribute(hConsole, 16 * 8 + 0);//noir sur gris foncé
		printf("Supprimer une ligne electrique");
		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);
		
		gotoligcol(12,9);printf("3 - ");
		if(select==2)SetConsoleTextAttribute(hConsole, 16 * 8 + 0);//noir sur gris foncé
		printf("Afficher les lignes des villes");
		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);
		
		gotoligcol(13,9);printf("4 - ");
		if(select==3)SetConsoleTextAttribute(hConsole, 16 * 8 + 0);//noir sur gris foncé
		printf("Afficher les lignes des centrales");
		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);
		
		gotoligcol(15,9);printf("ESC - ");
		if(select==4)SetConsoleTextAttribute(hConsole, 16 * 8 + 0);//noir sur gris foncé
		printf("Retour");
		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);
		
		SetConsoleTextAttribute(hConsole, 16 * 0 + 8);//gris sur noir
		printf("\n\n\n\n\tNaviguer avec les fleches 'Haut' et 'Bas' du clavier et selectionner avec 'Entrer'");
		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);//blanc sur noir
}
	
void AfficheMenuReseau(HANDLE hConsole, int select)
{	
		ClearConsole(hConsole);
		SetConsoleTextAttribute(hConsole, 16 * 7 + 0);//noir sur gris clair
		gotoligcol(6,5);printf("---     Gestion Du Reseau     ---");	
		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);
		gotoligcol(10,9);printf("1 - ");
		if(select==0)SetConsoleTextAttribute(hConsole, 16 * 8 + 0);//noir sur gris foncé
		printf("Charger un reseau");
		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);
		
		gotoligcol(11,9);printf("2 - ");
		if(select==1)SetConsoleTextAttribute(hConsole, 16 * 8 + 0);//noir sur gris foncé
		printf("Supprimer le reseau");
		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);
		
		gotoligcol(12,9);printf("3 - ");
		if(select==2)SetConsoleTextAttribute(hConsole, 16 * 8 + 0);//noir sur gris foncé
		printf("Enregistrer le reseau");
		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);
		
		gotoligcol(14,9);printf("ESC - ");
		if(select==3)SetConsoleTextAttribute(hConsole, 16 * 8 + 0);//noir sur gris foncé
		printf("Retour");
		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);
		
		SetConsoleTextAttribute(hConsole, 16 * 0 + 8);//gris sur noir
		printf("\n\n\n\n\tNaviguer avec les fleches 'Haut' et 'Bas' du clavier et selectionner avec 'Entrer'");
		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);//blanc sur noir
}

// Afficher les commandes de selection pour les fleches gauche/droite a partir d une ligne donnee //////////////////////////////////////////////////////////////////////////////////////////////////////
void AfficherCommandesSelectionGaucheDroite(int ligne)
{
	gotoligcol(ligne,16);printf("Precedent : <");
	gotoligcol(ligne+1,16);printf("Suivant : >");
	gotoligcol(ligne+2,16);printf("Selectionner : Entrer");
	gotoligcol(ligne+3,16);printf("Quitter : echap");
}

// Afficher commandes de selection pour fleches haut/bas a partir d une ligne donnee ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AfficherCommandesSelectionHautBas(int ligne)
{
	gotoligcol(ligne,16);printf("Precedent : fleche du haut");
	gotoligcol(ligne+1,16);printf("Suivant : fleche du bas");
	gotoligcol(ligne+2,16);printf("Selectionner : Entrer");
	gotoligcol(ligne+3,16);printf("Quitter : echap");
}

// Afficher commandes de defilement centrale/villes a partir d une ligne donnee ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AfficherCommandesDefilement(int ligne)
{
	gotoligcol(ligne,16);printf("Precedent : <");
	gotoligcol(ligne+1,16);printf("Suivant : >");
	gotoligcol(ligne+2,16);printf("Quitter : echap");
}

// Affiche le code de toutes les centrales a une ligne donnee /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AfficherCentrales(HANDLE hConsole, PTcentrale pDebutC , PTcentrale pFinC, int ligne)
{
	int i = 0, powPC = 0; // powPC puissance totale distribuee par la centrale pC
	PTligneElectrique pL;
	
	if(pDebutC->codeCentrale > 0)
	{
		SetConsoleTextAttribute(hConsole, 16 * 7 + 0);//noir sur gris clair
		gotoligcol(ligne+i,0);printf("\tCode centrale\n");
		gotoligcol(ligne+i,25);printf("Puissance distribuee (kW)\n");
		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);//blanc sur noir
		i = i + 2; // pour descendre de deux lignes
		
		PTcentrale pC;
		pC = pDebutC;
	
		while(pC->ptsuivant != pFinC)
		{
			powPC = 0;
			pC = pC->ptsuivant;
			pL = pC->villeDependante;
			while(pL != pC->pfinLigne){
				powPC = powPC + pL->puissance;
				pL = pL->ligneSuivante;
			}		
			gotoligcol(ligne+i,0);printf("\t%d", pC->codeCentrale);
			gotoligcol(ligne+i,25);printf("%d", powPC);
			i++;	
		}
	}
	else
	{
		printf("Il n'y a pas de centrales !\n");
	}
}

// Affiche le code postale et le nom de toutes les villes /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AfficherVilles(HANDLE hConsole, PTville pDebutV , PTville pFinV, PTcentrale pDebutC, PTcentrale pFinC, int ligne)	//On donne a quelle ligne afficher les villes
{	
	int i = 0, pow = 0;
	
	if(pDebutV->codePostal > 0)
	{
		PTville pV;
		pV = pDebutV;
		
		SetConsoleTextAttribute(hConsole, 16 * 7 + 0);//noir sur gris clair
		gotoligcol(ligne+i,0);printf("\tNom     \n");
		gotoligcol(ligne+i,30);printf("Code Postal\n");
		gotoligcol(ligne+i,50);printf("Puissance accordee (kW)\n");
		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);//blanc sur noir
		i = i + 2;// pour descendre de deux lignes
		
		while(pV->villeSuivante != pFinV)
		{
			pV = pV->villeSuivante;
			
			pow = PuissanceVille(pV, pDebutC, pFinC);
			gotoligcol(ligne+i,0);printf("\t%s" , pV->nomVille);
			gotoligcol(ligne+i,30);printf("%d\n" , pV->codePostal);
			gotoligcol(ligne+i,50);printf("%d\n" , pow);
			i++;	
		}
	}
	else
	{
		printf("Il n'y a pas de villes !\n");
	}
}

// Verifie que la centrale avec le codeCentrale = num existe //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int ExistanceCentrale(PTcentrale pDebutC , PTcentrale pFinC , int num)
{
	PTcentrale pX;
 	pX = pDebutC->ptsuivant;
    
    // pour etre sur que la boucle while s arrete
	pFinC->codeCentrale = num;
	while(pX->codeCentrale != num)
	{
		pX = pX->ptsuivant;
	}
	if(pX != pFinC)
		return 1;	//true : le codeCentrale existe
	else
		return 0;	//false : le codeCentrale n existe pas
}

// Verifie que la ville avec le codePostale = cp existe ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int ExistanceVille(PTville pDebutV , PTville pFinV , int cp)
{
	PTville pX;
 	pX = pDebutV->villeSuivante;

    // pour etre sur que la boucle while s arrete
	pFinV->codePostal = cp;
	while(pX->codePostal != cp)
	{
		pX = pX->villeSuivante;
	}
	if(pX != pFinV)
		return 1;	//true : le codePostal existe
	else
		return 0;	//false : le codePostal n existe pas
}

// Renvoie un PTcentrale avec le codeCentrale = num ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PTcentrale NumCentrale(PTcentrale pDebutC , PTcentrale pFinC , int num)
{
	PTcentrale pX;
	pX = pDebutC->ptsuivant;

	// pour etre sur que la boucle while s arrete
	pFinC->codeCentrale = num;
	while(pX->codeCentrale != num)
	{
		pX = pX->ptsuivant;
	}
	if(pX != pFinC)
		return(pX);
	else
		return(NULL);
}

// Renvoie un PTville avec le codePostal = cp ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PTville NumVille(PTville pDebutV , PTville pFinV , int cp)
{
	PTville pX;
	pX = pDebutV->villeSuivante;

	// pour etre sur que la boucle while s arrete
	pFinV->codePostal = cp;
	while(pX->codePostal != cp)
	{
		pX = pX->villeSuivante;
	}
	if(pX != pFinV)
		return(pX);
	else
		return(NULL);
}

// Renvoie un PTville qui precede pV dans la liste pDebutV /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PTville PtPreVille(PTville pDebutV , PTville pV)
{
	PTville pPrePV;
	pPrePV = pDebutV;

	while(pPrePV->villeSuivante != pV)
	{ pPrePV = pPrePV->villeSuivante; }

	return(pPrePV);
}

// Renvoie un NOUVEAU PTville juste avant pFin ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PTville InsertionVille(PTville pDebutV , PTville pFinV , PTben BEN)
{
	PTville pV;
	PTville pPrePV;

	if (BEN->pdebutV->codePostal == 0){ // si il n y a rien dans la poubelle -> malloc
	pV = (PTville)malloc(sizeof(Tville));
	}
	else{ // sinon on recycle le maillon
		pV = BEN->pdebutV->villeSuivante;
		BEN->pdebutV->villeSuivante = pV->villeSuivante;
		BEN->pdebutV->codePostal = BEN->pdebutV->codePostal - 1;
	}
	pPrePV = PtPreVille(pDebutV , pFinV);

	pV->villeSuivante = pFinV;
	pPrePV->villeSuivante = pV;

	pDebutV->codePostal = pDebutV->codePostal + 1;
	return(pV);
}

// Renvoie un NOUVEAU PTcentrale juste avant pFin /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PTcentrale InsertionCentrale(PTcentrale pDebutC , PTcentrale pFinC , PTben BEN)
{
	PTcentrale pC;
	PTligneElectrique pfinL; // pointeur sur le bidon des lignes electriques de la centrale
	if(BEN->pdebutC->codeCentrale == 0){ // pas de centrale dans la poubelle on prend de la memoire libre
		pfinL = (PTligneElectrique)malloc(sizeof(TlignesElectrique));
		pC = (PTcentrale)malloc(sizeof(Tcentrale));
		pfinL->ligneSuivante = NULL;
		pfinL->villeDesservie = NULL;
		pC->pfinLigne = pfinL;
	}
	else{ // centrale dans la poubelle, on la recycle
		pC = BEN->pdebutC->ptsuivant;
		BEN->pdebutC->ptsuivant = pC->ptsuivant;
		pC->ptsuivant->ptprecedent = BEN->pdebutC;
		BEN->pdebutC->codeCentrale = BEN->pdebutC->codeCentrale - 1;
	}

	pC->ptsuivant = pFinC;
	pC->ptprecedent = pFinC->ptprecedent;
	pFinC->ptprecedent->ptsuivant = pC;
	pFinC->ptprecedent = pC;

	//il n y a pas encore de lignes donc "pC->villeDependante" est equivalent a "pC->pfinLigne"
	pC->villeDependante = pC->pfinLigne;
	
	pDebutC->codeCentrale = pDebutC->codeCentrale + 1;
	pC->pfinLigne->puissance = 0;
	return(pC);
}

// Renvoie un PTligneElectrique qui precede pL dans une liste de ligne. /!\ Si pL est la premiere ligne, renvoie pL /!\ ////////////////////////////////////////////////////////////////////////////
PTligneElectrique PtPreLigne(PTligneElectrique pDebutL , PTligneElectrique pL)
{
	if (pL != pDebutL){ // pDebutL peut contenir des informations donc si pL = pDebut, on retrourne directement pL
		PTligneElectrique pPrePL;
		pPrePL = pDebutL;
		while(pPrePL->ligneSuivante != pL && pPrePL->ligneSuivante != NULL){
			pPrePL = pPrePL->ligneSuivante;
		}
		return(pPrePL);
	}
	else{
		return(pL);
	}
}

// Renvoie un NOUVEAU PTligneElectrique avant pfinLigne (dans pC) /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PTligneElectrique InsertionLigneElec(PTcentrale pC , PTben BEN)
{
	PTligneElectrique pL;

	if (BEN->pdebutL->puissance == 0){ // si il n y a rien dans la poubelle -> malloc
		pL = (PTligneElectrique)malloc(sizeof(TlignesElectrique));
	}
	else{ // sinon on recycle le maillon
		pL = BEN->pdebutL->ligneSuivante;
		BEN->pdebutL->ligneSuivante = pL->ligneSuivante;
		BEN->pdebutL->puissance = BEN->pdebutL->puissance - 1; // On met a jour le nombre de ligne dans la poubelle
	}

	if (pC->villeDependante != pC->pfinLigne) { // on regarde si il y a des lignes electriques
		//renvoie un pointeur sur la ligne electrique se trouvant avant pfinLigne
		PTligneElectrique pPrePL = PtPreLigne(pC->villeDependante , pC->pfinLigne);
		pPrePL->ligneSuivante = pL;
	}
	else{
		pC->villeDependante = pL;
	}
	pL->ligneSuivante = pC->pfinLigne;
	pL->villeDesservie = NULL;

	pC->pfinLigne->puissance = pC->pfinLigne->puissance + 1; // On met a jour le nombre de ligne dans pC
	return(pL);
}

// Ajouter "n" centrales //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RemplirCentrales(PTcentrale pDebutC , PTcentrale pFinC , int n , PTben BEN)
{
	int i = 0, existance = 0, code = 0;
	PTcentrale pX;
 
	while(i < n)
	{
		printf("Code centrale : ");
		scanf("%d" , &code);
		existance = ExistanceCentrale(pDebutC , pFinC , code);
		
		while(existance == 1)
		{
			/* si le numero de la centrale a deja ete ajoute au reseau, on redemande un
			numero => eviter les doublons de num centrales */
			printf("La centrale %d fait deja partie du reseau\nCode centrale : ", code);
			scanf("%d" , &code);
			existance = ExistanceCentrale(pDebutC , pFinC , code);
		}
		
		pX = InsertionCentrale(pDebutC , pFinC , BEN);
		pX->codeCentrale = code;
		i++;
	}	
}

// Ajouter "n" villes /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RemplirVilles(PTville pDebutV , PTville pFinV, int n , PTben BEN)
{
	int i = 0, cp = 0;
	int existance = 0;	//1 si l element existe et 0 si l element n existe pas
	PTville px;

	while(i < n)
	{
		printf("\nCode postal : ");
		scanf("%d" , &cp);
		existance = ExistanceVille(pDebutV , pFinV , cp);
		
		while(existance == 1)
		{
			/* si le code postal de la ville a deja ete ajoute au reseau, on redemande le code
			postal => eviter les doublons */
			printf("Une ville de code postal %d fait deja partie du reseau\nCode postal : ", cp);
			scanf("%d" , &cp);
			existance = ExistanceVille(pDebutV , pFinV , cp);
		}
		
		px = InsertionVille(pDebutV , pFinV , BEN);
		printf("Nom ville : ");
		scanf("%s", px->nomVille);
		px->codePostal = cp;
		i++;
	}
}

// Met la ligne pL du reseau dans la poubelle /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SupLigne(PTcentrale pC , PTligneElectrique pL , PTben BEN)
{
	PTligneElectrique ptPrePL;
	// On commence par delier la ligne electrique pL de la liste
	if (pL != pC->villeDependante){ // on regarde si pL n est pas la premiere ligne electrique
		ptPrePL = PtPreLigne(pC->villeDependante , pL);
		ptPrePL->ligneSuivante = pL->ligneSuivante;
	}
	else // sinon ca veut dire que pL est la première ligne electrique
		pC->villeDependante = pL->ligneSuivante; // dans ce cas, on change le pointeur villeDependante sur la ville d apres
	
	// Ici on insert la ligne electrique pL au debut de la liste poubelle
	pL->ligneSuivante = BEN->pdebutL->ligneSuivante;
	BEN->pdebutL->ligneSuivante = pL;
	pL->villeDesservie = NULL;
	BEN->pdebutL->puissance = BEN->pdebutL->puissance + 1;

	pC->pfinLigne->puissance = pC->pfinLigne->puissance - 1; // On met a jour le nombre de ligne dans pC
}

// Met la centrale avec le codeCentrale = num dans la liste poubelle + ses lignes /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SupCentrales(PTcentrale pDebutC , PTcentrale pFinC , int num , PTben BEN)
{
	PTligneElectrique pLx , pLy;
	PTcentrale pC;
 	pC = NumCentrale(pDebutC , pFinC , num);
	pLx = pC->villeDependante;
	
	// On commence par enlever la centrale pC de la liste
 	pC->ptprecedent->ptsuivant = pC->ptsuivant;
	pC->ptsuivant->ptprecedent = pC->ptprecedent;
	pDebutC->codeCentrale = pDebutC->codeCentrale - 1;

	// on met dans la liste poubelle les lignes de la centrales pC
	while(pLx != pC->pfinLigne){
		pLy = pLx->ligneSuivante;
		SupLigne(pC , pLx , BEN);
		pLx = pLy;
	}
	// Insertion de la centrale pC dans la liste poubelle
	pC->ptsuivant = BEN->pdebutC->ptsuivant;
	BEN->pdebutC->ptsuivant = pC;
	pC->ptprecedent = BEN->pdebutC;
	pC->ptsuivant->ptprecedent = pC;
	BEN->pdebutC->codeCentrale = BEN->pdebutC->codeCentrale + 1;
}

// Met la ville avec le codePostale = cp dans la liste poubelle + les lignes pointants sur la ville ///////////////////////////////////////////////////////////////////////////////////////////////////
void SupVilles(PTville pDebutV , PTville pFinV , PTcentrale pDebutC , PTcentrale pFinC , int cp , PTben BEN)
{
	PTville pV , ptPrePV;
	PTcentrale pC;
	PTligneElectrique pLx , pLy;
	pV = NumVille(pDebutV , pFinV , cp);

	// Ici on enleve la ville pV de la liste
	ptPrePV = PtPreVille(pDebutV , pV);
	ptPrePV->villeSuivante = pV->villeSuivante;
	pDebutV->codePostal = pDebutV->codePostal - 1;

	// On met dans la poubelle les lignes pointants sur la ville pV
	pC = pDebutC->ptsuivant;
	while(pC != pFinC){ // pour cela une boucle while qui parcours les centrales ...
		pLx = pC->villeDependante;
		while(pLx != pC->pfinLigne){ // ... et une autre qui parcours les lignes de la centrales
			pLy = pLx->ligneSuivante;
			if(pLx->villeDesservie == pV){
				SupLigne(pC , pLx , BEN);
			}
			pLx = pLy;
		}
		pC = pC->ptsuivant;
	}

	// On insert la ville pV dans la poubelle
	pV->villeSuivante = BEN->pdebutV->villeSuivante;
	BEN->pdebutV->villeSuivante = pV;
	BEN->pdebutV->codePostal = BEN->pdebutV->codePostal + 1;
}

// Met TOUT le reseau dans les listes poubelles ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SupprimerReseau(PTcentrale pDebutC , PTcentrale pFinC , PTville pDebutV , PTville pFinV , PTben BEN)
{
	int numC, cp;
	PTcentrale pC;
	PTville pV;
	
	while(pDebutC->codeCentrale > 0){
		pC = pDebutC->ptsuivant;
		pC->codeCentrale = numC;
		SupCentrales(pDebutC , pFinC , numC , BEN);
	}
	while(pDebutV->codePostal > 0){
		pV = pDebutV->villeSuivante;
		pV->codePostal = cp;
		SupVilles(pDebutV , pFinV , pDebutC , pFinC , cp , BEN);
	}
}

// Fonction qui demande une validation a l utilisateur, retourne 1 si validee et 0 si rejetee /////////////////////////////////////////////////////////////////////////////////////////////////////////
int Validation()
{
	char a = ' ';
	while(a != 'O' && a != 'o' && a != 'N' && a != 'n'){
		a = getch();
		if(a == 'O' || a == 'o')
			return 1;
		else if(a == 'N' || a == 'n')			
			return 0;
	}
}

// Lit un caractère entré au clavier et retourne son code ASCII ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int lireCaract()
{
    char c = 10;
    int fnt = 0;	
    c = getch();
    fnt = c;
    if (c == -32) { c = getch();fnt = 400+c; }
    if (c == 0)  { c = getch();fnt = 500+c; }
    return (fnt);
}

// Renvoie la centrale suivante ou precedent pC si < ou > /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PTcentrale CentraleSuivantePrecedente(PTcentrale pDebutC , PTcentrale pFinC, PTcentrale pC, int i){
	if (i == 475 && pC->ptprecedent != pDebutC) // fleche gauche
		pC = pC->ptprecedent;
    else if (i == 477 && pC->ptsuivant != pFinC) // fleche droite
		pC = pC->ptsuivant;

	return(pC);
}

// Renvoie la centrale selectionnee dans le Cadre /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int SelectCentrale(PTcentrale pDebutC , PTcentrale pFinC)
{
   	int i;
	// au milieu a gauche dans le Cadre, l'affichage se fait par rapport a ces valeurs
	int lig = 16, col = 12;
	int nbLignesPc = 0;
	PTcentrale pC;
	PTcentrale pcPre; // va garder en memoire l encienne valeur de pC
    
	pC = pDebutC->ptsuivant;
    
    AfficherCommandesSelectionGaucheDroite(lig-7);
	Cadre(lig-2 , col-2 , 4 , 30);
    while (pDebutC->codeCentrale != 0){
   		gotoligcol(lig , col);
	    printf("Centrale : %d", pC->codeCentrale);
	    gotoligcol(lig+5 , 0);
		printf("\tVilles raccordees a la centrale %d : \n", pC->codeCentrale);
		nbLignesPc = AfficherLignesCentrale(pC);

		pcPre = pC;
		while(pC == pcPre){
			i = lireCaract();
			pC = CentraleSuivantePrecedente(pDebutC, pFinC, pC, i);
			if(i == 13){ // touhce Entre
				EmptyLine(lig+5 , nbLignesPc+5);
				gotoligcol(lig+2 , 0);
				return(pC->codeCentrale);
			}
			else if(i == 27){ // touche ESC
				EmptyLine(lig+5 , nbLignesPc+5);
				gotoligcol(lig+2 , 0);
				return(-1);
			}
		}
		EmptyLine(lig+5 , nbLignesPc+5);
		FillSquare(lig , col , 2 , 29 , ' ');
  	}
}

// Renvoie la ville suivante ou precedent pV si < ou > ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PTville VilleSuivantePrecedente(PTville pDebutV, PTville pFinV, PTville pV, int i){
	PTville pPrePV = PtPreVille(pDebutV, pV);

	if (i == 475 && pPrePV != pDebutV) // fleche gauche
		pV = pPrePV;
    else if (i == 477 && pV->villeSuivante != pFinV) // fleche droite
		pV = pV->villeSuivante;

	return(pV);
}

// Renvoie la ville selectionnee dans le Cadre ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int SelectVille(PTville pDebutV, PTville pFinV, PTcentrale pDebutC, PTcentrale pFinC)
{
   	int i;
	// au milieu a gauche dans le Cadre, l'affichage se fait par rapport a ces valeurs
	int lig = 16, col = 12;
	int nbLignesPv = 0;
	PTville pV;
	PTville pvPre; // va garder en memoire l encienne valeur de pV

	pV = pDebutV->villeSuivante;

    AfficherCommandesSelectionGaucheDroite(lig-7);
	Cadre(lig-2 , col-2 , 4 , 30);
	while (pDebutV->codePostal != 0){
  		gotoligcol(lig , col);
		printf("%s", pV->nomVille);
		gotoligcol(lig+1 , col);
		printf("%d", pV->codePostal);
	    gotoligcol(lig+5 , 0);
		printf("\tCentrales alimentants %s : \n", pV->nomVille);
		nbLignesPv = AfficherLignesVille(pV, pDebutC, pFinC);

		pvPre = pV;
		while(pV == pvPre){
			i = lireCaract();
			pV = VilleSuivantePrecedente(pDebutV, pFinV, pV, i);
			if(i == 13){ // touhce Entre
				EmptyLine(lig+5 , nbLignesPv+4);
				gotoligcol(lig+2 , 0);
				return(pV->codePostal);
			}
			else if(i == 27){ // touche ESC
				EmptyLine(lig+5 , nbLignesPv+4);
				gotoligcol(lig+2 , 0);
				return(-1); // on renvoie un nombre négatif pour signifier qu on annule la selection
			}
		}
		EmptyLine(lig+5 , nbLignesPv+4);
		FillSquare(lig , col , 2 , 29 , ' ');
	}
}

// Renvoie la ligne suivante ou precedent pL si /\ ou \/ //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PTligneElectrique LigneSuivantePrecedente(PTcentrale pC, PTligneElectrique pL, char i){
	PTligneElectrique pPrePL = PtPreLigne(pC->villeDependante, pL);

	if (i == 72) // fleche du haut
		pL = pPrePL;
    else if (i == 80 && pL->ligneSuivante != pC->pfinLigne) // fleche du bas
		pL = pL->ligneSuivante;

	return(pL);
}

// Renvoie la ligne selectionee dans le Cadre a partir d une centrale /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PTligneElectrique SelectLigne(PTcentrale pC, HANDLE hConsole){
	char i;
	// au milieu a gauche dans le Cadre, l'affichage se fait par rapport a ces valeurs
	int lig = 16, col = 12;
	int nbLignesPC;
	PTligneElectrique pL = pC->villeDependante;
	PTligneElectrique plPre = pL; // va garder en memoire l encienne valeur de pL
	PTligneElectrique affichePL;

	AfficherCommandesSelectionHautBas(lig-7);
	gotoligcol(lig+5 , 0);
	printf("\tVilles raccordees a la centrale %d : \n", pC->codeCentrale);
	while(1 == 1){
		gotoligcol(lig+6 , 0);
		affichePL = pC->villeDependante;
		while(affichePL != pC->pfinLigne){
			if(affichePL == pL){
				SetConsoleTextAttribute(hConsole, 16 * 8 + 0);// noir sur gris foncé
			}
			printf("\n\t%d kW   ->     %s   %d",affichePL->puissance, affichePL->villeDesservie->nomVille, affichePL->villeDesservie->codePostal);
			SetConsoleTextAttribute(hConsole, 16 * 0 + 15);// couleur normale
			affichePL = affichePL->ligneSuivante;
		}

		plPre = pL;
		while(pL == plPre){
			i = getch();
			pL = LigneSuivantePrecedente(pC , pL , i);
			if(i == 13){ // entre
				return(pL);
			}
			else if (i == 27){// echap
				return(NULL);
			}
		}
	}
}

// Met les pointeur/variable de la ligne electrique au bon endroit/valeur /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CreationLigne(PTcentrale pDebutC, PTcentrale pFinC, PTville pDebutV, PTville pFinV, int num, int cp, int pow , PTben BEN)
{
	PTligneElectrique pL;
	PTcentrale pC;
	
	//retourne le pointeur sur une structure centrale a partir de son code
	pC = NumCentrale(pDebutC, pFinC, num);

	pL = InsertionLigneElec(pC , BEN);

	pL->puissance = pow;
	pL->villeDesservie = NumVille(pDebutV , pFinV , cp);
}

// Affiche les lignes electriques d une centrale et la puissance totale + renvoie le nombre de lignes total ///////////////////////////////////////////////////////////////////////////////////////////
int AfficherLignesCentrale(PTcentrale pC)
{
	int nbLigne = 0;
	int allPow = 0; // puissance totale distribué par la centrale
	PTligneElectrique pL = pC->villeDependante;
	while(pL != pC->pfinLigne){
		printf("\n\t%d kW   ->     %s   %d",pL->puissance, pL->villeDesservie->nomVille, pL->villeDesservie->codePostal);
		allPow = allPow + pL->puissance;
		nbLigne = nbLigne + 1;
		pL = pL->ligneSuivante;
	}
	
	if(nbLigne == 0){
		printf("\n\tAucune ...");
	}
	printf("\n\n\tPuissance totale distribuee : %d kW", allPow);	
	return(nbLigne);
}

// Fait defiler les centrale dans un cadre et affiche les villes deservies ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DefilerLignesCentrales(PTcentrale pDebutC, PTcentrale pFinC){
	int i = 0;
	// au milieu a gauche dans le Cadre, l'affichage se fait par rapport a ces valeurs
	int lig = 15, col = 12;
	int nbLignesPC = 0;
	PTcentrale pC;
	PTcentrale pcPre; // va garder en mémoire l'encienne valeur de pC
	
	// On verifie si il y a des elements dans la liste
	if(pDebutC->codeCentrale != 0){
		printf("\n\n\tSelectionner une centrale :");
		pC = pDebutC->ptsuivant;
	
    	AfficherCommandesDefilement(lig-7);
		Cadre(lig-2 , col-2 , 4 , 30);
		while (i != 27){ // touche ESC : sortir du programme echap
   			gotoligcol(lig , col);
	    	printf("Centrale : %d", pC->codeCentrale);
	    	gotoligcol(lig+5 , 0);
			printf("\tVilles raccordees a la centrale %d : \n", pC->codeCentrale);
			nbLignesPC = AfficherLignesCentrale(pC);

			pcPre = pC;
			while(pC == pcPre && i != 27){
				i = lireCaract();
				pC = CentraleSuivantePrecedente(pDebutC, pFinC, pC, i);
			}
			EmptyLine(lig+5 , nbLignesPC+5);
			FillSquare(lig , col , 1 , 29 , ' ');
	  	}
		gotoligcol(lig+2 , 0);
	}
	else
		printf("\nLe reseau ne contient aucune centrale");
}

// Renvoie la puissance totale accordee a une ville //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int PuissanceVille(PTville pV, PTcentrale pDebutC, PTcentrale pFinC){
	int allPow = 0;
	PTcentrale pC = pDebutC->ptsuivant;
	PTligneElectrique pL;

	while(pC != pFinC){
		pL = pC->villeDependante;
		while(pL != pC->pfinLigne){
			if(pL->villeDesservie == pV)
				allPow = allPow + pL->puissance;
			pL = pL->ligneSuivante;
		}
		pC = pC->ptsuivant;
	}
	return(allPow);
}

// Affiche la puissance recu par les centrales et la puissance total recu + renvoie le nombre de lignes total /////////////////////////////////////////////////////////////////////////////////////////
int AfficherLignesVille(PTville pV, PTcentrale pDebutC, PTcentrale pFinC){
	int allPow = 0;
	int nbLigne = 0;
	PTcentrale pC = pDebutC->ptsuivant;
	PTligneElectrique pL;

	while(pC != pFinC){
		pL = pC->villeDependante;
		while(pL != pC->pfinLigne){
			if(pL->villeDesservie == pV){
				printf("\n\t%d    ->    %d KW", pC->codeCentrale, pL->puissance);
				allPow = allPow + pL->puissance;
				nbLigne = nbLigne + 1;
			}
			pL = pL->ligneSuivante;
		}
		pC = pC->ptsuivant;
	}
	if(nbLigne == 0){
		printf("\n\tAucune ...");
	}
	printf("\n\n\tPuissance totale recue : %d kW", allPow);
	return(nbLigne);
}

// Fait defiler les villes dans un cadre et affiche les centrales allimentant la ville ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DefilerLignesVilles(PTcentrale pDebutC, PTcentrale pFinC, PTville pDebutV, PTville pFinV){
	int i = 0;
	// au milieu a gauche dans le Cadre, l'affichage se fait par rapport a ces valeurs
	int lig = 15, col = 12;
	int nbLignesPv = 0;
	PTville pV;
	PTville pvPre; // va garder en mémoire l'encienne valeur de pV
	
	// On verifie si il y a des elements dans la liste
	if(pDebutV->codePostal != 0){
		printf("\n\n\tSelectionner une ville :");
		pV = pDebutV->villeSuivante;
	
    	AfficherCommandesDefilement(lig-7);
		Cadre(lig-2 , col-2 , 4 , 30);
		while (i != 27){ // touche ESC : sortir du programme echap
   			gotoligcol(lig , col);
	    	printf("%s", pV->nomVille);
			gotoligcol(lig+1 , col);
	    	printf("%d", pV->codePostal);
	    	gotoligcol(lig+5 , 0);
			printf("\tCentrales alimentants %s : \n", pV->nomVille);
			nbLignesPv = AfficherLignesVille(pV, pDebutC, pFinC);

			pvPre = pV;
			while(pV == pvPre && i != 27){
				i = lireCaract();
				pV = VilleSuivantePrecedente(pDebutV, pFinV, pV, i);
			}
			EmptyLine(lig+5 , nbLignesPv+5);
			FillSquare(lig , col , 2 , 29 , ' ');
	  	}
		gotoligcol(lig+2 , 0);
	}
	else
		printf("\nLe reseau ne contient aucune ville");
}

// Charger un reseau a partir d un fichier texte nomme "nomFichier[100]" //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Charger(PTcentrale pDebutC , PTcentrale pFinC , PTville pDebutV , PTville pFinV , char nomFichier[100] , PTben BEN)
{
	char c , str_temp[50];
	int i = 0 , puissance = 0 , cp = 0;
	PTville pV;
	PTcentrale pC;
	PTligneElectrique pL;
  
	FILE *fIn = NULL;	// file input
	fIn = fopen(nomFichier , "rt"); // read texte

	if(fIn != NULL){
		fscanf(fIn, "%c", &c);
		//le fichier texte se termine par un "."
		while (c != '.'){
			// "$" indique qu on a fini d entrer les villes meme si il n y en a pas
			while (c != '$'){
				pV = InsertionVille(pDebutV , pFinV , BEN);
				i = 0;
				while (c != 9){	// 9 correspond au caractere ASCII de la tabulation \t
				  	str_temp[i] = c;
					fscanf(fIn, "%c", &c);
					i++;
				}
				str_temp[i] = '\0';	// on met '\0' pour dire que c est la fin de la chaine
				/*la fonction atoi() fait partie de la bibliothèque <stdlib.h> et permet
				de convertir une chaine de caractères en un entier*/
				pV->codePostal = atoi(str_temp);
				
				fscanf(fIn, "%c", &c);
				i = 0;
				while (c != 10){ // 10 correspond au caractere ASCII du retour a la ligne \n
				  	pV->nomVille[i] = c;
					fscanf(fIn, "%c", &c);
					i++;
				}
				pV->nomVille[i]='\0'; // on met '\0' pour dire que c est la fin de la chaine
				fscanf(fIn, "%c", &c);
			}
			fscanf(fIn, "%c", &c);
			fscanf(fIn, "%c", &c); // pour passer au caractere en dessous du "$"
			// "." indique la fin du fichier
			while(c != '.'){
				pC = InsertionCentrale(pDebutC , pFinC , BEN);
				i = 0;
				while (c != 10){ // 10 correspond au caractere ASCII du retour a la ligne \n
				  	str_temp[i] = c;
					fscanf(fIn, "%c", &c);
					i++;
				}
				str_temp[i] = '\0';
				pC->codeCentrale = atoi(str_temp);

				fscanf(fIn, "%c", &c);
				// "#" indique que l on passe a la declaration de la centrale suivant (avec ses lignes)
				while (c != '#'){
					i = 0;
					while (c != ' '){ // tant que c different d un espace
				  		str_temp[i] = c;
						fscanf(fIn, "%c", &c);
						i++;
					}
					str_temp[i] = '\0';
					cp = atoi(str_temp);

					i = 0;
					while (c != 10){ // 10 correspond au caractere ASCII du retour a la ligne \n
				  		str_temp[i] = c;
						fscanf(fIn, "%c", &c);
						i++;
					}
					str_temp[i] = '\0';
					puissance = atoi(str_temp);
					CreationLigne(pDebutC , pFinC , pDebutV , pFinV, pC->codeCentrale , cp , puissance , BEN);

					fscanf(fIn, "%c", &c);
				}
				fscanf(fIn, "%c", &c);
				fscanf(fIn, "%c", &c); // pour passer au caractere en dessous du "#"
			}
		}
		printf("\nLe fichier %s a bien ete charge", nomFichier);
	}
	else
		printf("Le fichier %s n'existe pas" , nomFichier);

	fclose(fIn);	// fermeture du fichier d entree
}

// Enregistrer un fichier sous le nom de "nomFichier" /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Enregistrer(PTcentrale pDebutC , PTcentrale pFinC , PTville pDebutV , PTville pfinV , char nomFichier[100])
{
	int i = 0;
	PTville pV = pDebutV->villeSuivante;
	PTcentrale pC = pDebutC->ptsuivant;
	PTligneElectrique pL;
	FILE *fOut = NULL;	// file Output

	fOut = fopen(nomFichier , "rt"); // on ouvre en mode lecture pour voir si le fichier existe deja
	if(fOut == NULL){
		fclose(fOut);
		fOut = fopen(nomFichier , "wt"); // write texte
		// On commence par enregistrer les villes
		while(pV != pfinV){
			fprintf(fOut , "%d\t%s\n" , pV->codePostal , pV->nomVille);
			pV = pV->villeSuivante;
		}
		fprintf(fOut , "$\n");
		// Puis on enregistre les centrales ...
		while(pC != pFinC){
			fprintf(fOut , "%d\n" , pC->codeCentrale);
			pL = pC->villeDependante;
			// avec leurs lignes électrique
			while(pL != pC->pfinLigne){
				fprintf(fOut , "%d %d\n" , pL->villeDesservie->codePostal , pL->puissance);
				pL = pL->ligneSuivante;
			}
			fprintf(fOut , "#\n");
			pC = pC->ptsuivant;
		}
		fprintf(fOut , ".");
		printf("\nLe fichier a bien ete enregistre sous le nom : %s" , nomFichier);
	}
	else{
		printf("\n%s existe deja" , nomFichier);
	}
	fclose(fOut);
}

// ajoute l'extension .txt si elle n est pas presente /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void NormalisationFichierTXT(char * nomFichier){ // nomFichier est un pointeur sur le premier element de la chaine
	int i = 0;
	while (nomFichier[i]!='\0')
		i++; // on se place sur le dernier caractere de la chaine
	if(nomFichier[i-4]!='.' || nomFichier[i-3]!='t' || nomFichier[i-2]!='x' || nomFichier[i-1]!='t'){
		nomFichier[i] = '.'; // ajout de l extension .txt
		nomFichier[i+1] = 't';
		nomFichier[i+2] = 'x';
		nomFichier[i+3] = 't';
		nomFichier[i+4] = '\0'; // fin chaine de caracteres
	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                             Main                               
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void main()
{
	int i = 32, select = 0, selectf = 0, nbrVilles = 0, nbrCentrales = 0, numCentrale = 0, existance = 32, cp = 0, puissance = 0;
	char c = 32, k=32, nomFichier[100];

	//Creation des poiteurs qui serviront a PARCOURIR les differentes listes
	PTligneElectrique pParcLigne = NULL;
	PTville pParcVille = NULL;
	PTcentrale pParcCentrale = NULL;
	
	//Creation des bidons
    PTville pdebutVille;
	PTville pfinVille;
	PTcentrale pdebutCentrale;
	PTcentrale pfinCentrale;

	//Creation de la poubelle
	PTben BEN;

	// Initalistaion des bidons
	pdebutVille = (PTville)malloc(sizeof(Tville));
	pfinVille = (PTville)malloc(sizeof(Tville));
	pdebutCentrale = (PTcentrale)malloc(sizeof(Tcentrale));
	pfinCentrale = (PTcentrale)malloc(sizeof(Tcentrale));

	pdebutVille->villeSuivante = pfinVille;
	pfinVille->villeSuivante = NULL;
	pdebutVille->codePostal = 0;

	pdebutCentrale->ptsuivant = pfinCentrale;
	pdebutCentrale->ptprecedent = NULL;
	pfinCentrale->ptsuivant = NULL;
	pfinCentrale->ptprecedent = pdebutCentrale;
	pdebutCentrale->villeDependante = NULL;
	pfinCentrale->villeDependante = NULL;
	pdebutCentrale->codeCentrale = 0;

	// Initialisation des poubelles
	BEN = (PTben)malloc(sizeof(Tben));

	BEN->pdebutV = (PTville)malloc(sizeof(Tville));
	BEN->pfinV = (PTville)malloc(sizeof(Tville));
	BEN->pdebutC = (PTcentrale)malloc(sizeof(Tcentrale));
	BEN->pfinC = (PTcentrale)malloc(sizeof(Tcentrale));
	BEN->pdebutL = (PTligneElectrique)malloc(sizeof(TlignesElectrique));
	BEN->pfinL = (PTligneElectrique)malloc(sizeof(TlignesElectrique));

	BEN->pdebutV->villeSuivante = BEN->pfinV;
	BEN->pfinV->villeSuivante = NULL;
	BEN->pdebutV->codePostal = 0;

	BEN->pdebutC->ptsuivant = BEN->pfinC;
	BEN->pdebutC->ptprecedent = NULL;
	BEN->pfinC->ptsuivant = NULL;
	BEN->pfinC->ptprecedent = BEN->pdebutC;
	BEN->pdebutC->villeDependante = NULL;
	BEN->pfinC->villeDependante = NULL;
	BEN->pdebutC->codeCentrale = 0;
	
	BEN->pdebutL->ligneSuivante = BEN->pfinL;
	BEN->pfinL->ligneSuivante = NULL;
	BEN->pdebutL->puissance = 0;
	
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// Ligne a decommenter pour le developpement du code //////////////////////
	// char temp[100] = {'r','e','s','e','a','u','.','t','x','t','\0'};
	// Charger(pdebutCentrale, pfinCentrale, pdebutVille, pfinVille, temp, BEN);

	while(i != 0)
	{
		select = 0;
	  	AfficheMenu(hConsole, select);
	  	
	  	while(c != '1' && c != '2' && c != '3' && c != '4' && c != 27 && c!=13){//boucle tant que la touche tappee n est pas definie
			if(c==72 && select!=0){//fleche haut
				select--;
				AfficheMenu(hConsole, select);
			}
			else if(c==80 && select!=4){//fleche bas
				select++;
				AfficheMenu(hConsole, select);
			}
			c = getch();
		}
		//si on entre 1 ou si on tappe entrer avec le selectionneur sur 0
	  	if(c == '1' || (c == 13 && select == 0))//Menu gestion des centrales //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		{
			selectf = 0;
			AfficheMenuCentrales(hConsole, selectf);
			
			k = getch();
			//boucle tant que la touche tappee n est pas definie
			while(k!='1' && k!='2' && k!='3' && k!=27 && k!=13){
				if(k==72 && selectf!=0){//fleche haut
					selectf--;
					AfficheMenuCentrales(hConsole, selectf);
				}
				else if(k==80 && selectf!=3){//fleche bas
					selectf++;
					AfficheMenuCentrales(hConsole, selectf);
				}
				k = getch();
			}
			
			if(k == '1' || (k == 13 && selectf == 0))//Ajouter des centrales ////////////////////////////////////////////////////
			{
				SetConsoleTextAttribute(hConsole, 16 * 0 + 10);//vert sur noir
		  		printf("\n\n\tAjout de centrales\n");
		  		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);//blanc sur noir
		  		printf("\nNombre de centrales a ajouter : ");
				scanf("%d" , &nbrCentrales);
				printf("\n");
		  		RemplirCentrales(pdebutCentrale , pfinCentrale , nbrCentrales , BEN);
			}	
			if(k == '2' || (k == 13 && selectf == 1))//Supprimer des centrales //////////////////////////////////////////////////
			{
				ClearConsole(hConsole);
				SetConsoleTextAttribute(hConsole, 16 * 0 + 10);//vert sur noir
		  		printf("\n\n\tSuppression de centrales\n");
		  		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);//blanc sur noir
		  		if(pdebutCentrale->codeCentrale!=0){
		  			printf("\n\nSelectionner une centrale :");
			  		numCentrale = SelectCentrale(pdebutCentrale , pfinCentrale);
			  		gotoligcol(20,0);
			  		if(numCentrale!=-1){	// l utilisateur a bien selectionne une centrale
			  			printf("\nEtes-vous sur de vouloir supprimer la centrale %d\n\tTappez :\tO : Oui \t N : Non", numCentrale);
						if(Validation()==1){
							SupCentrales(pdebutCentrale , pfinCentrale , numCentrale , BEN);
							printf("\n\nLa centrale %d a etee retiree du reseau", numCentrale);
						}
						else
							printf("\n\nSuppression annulee");
					}
			  		else
			  			printf("\n\nSuppression annulee");
				}
				else
					printf("\n\nLe reseau ne contient aucune centrale");
			}
			if(k == '3' || (k == 13 && selectf == 2))//Afficher les centrales ///////////////////////////////////////////////////
			{
				ClearConsole(hConsole);
				SetConsoleTextAttribute(hConsole, 16 * 0 + 10);//vert sur noir
		  		printf("\n\n\tAffichage de la liste des centrales\n\n\n");
		  		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);//blanc sur noir	
		  		AfficherCentrales(hConsole, pdebutCentrale , pfinCentrale, 7);
			}
		}
		if(c == '2' || (c == 13 && select == 1))//Menu gestion des villes /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		{
			selectf = 0;
			AfficheMenuVilles(hConsole, selectf);
			
			k = getch();
			//boucle tant que la touche tappee n est pas definie
			while(k != '1' && k != '2' && k != '3' && k != 27 && k != 13){
				if(k == 72 && selectf != 0){//fleche haut
					selectf--;
					AfficheMenuVilles(hConsole, selectf);
				}
				else if(k == 80 && selectf != 3){//fleche bas
					selectf++;
					AfficheMenuVilles(hConsole, selectf);
				}
				k = getch();
			}
			
			if(k == '1' || (k == 13 && selectf == 0))//Ajouter des villes ///////////////////////////////////////////////////////
			{
				SetConsoleTextAttribute(hConsole, 16 * 0 + 10);//vert sur noir
		  		printf("\n\n\tAjout de villes\n");
		  		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);//blanc sur noir
				printf("\nNombre de villes a ajouter : ");
				scanf("%d", &nbrVilles);
		  		RemplirVilles(pdebutVille , pfinVille , nbrVilles , BEN);
			}	
			if(k == '2' || (k == 13 && selectf == 1))//Supprimer des villes /////////////////////////////////////////////////////
			{
				ClearConsole(hConsole);
				SetConsoleTextAttribute(hConsole, 16 * 0 + 10);//vert sur noir
		  		printf("\n\n\tSuppression de villes\n");
		  		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);//blanc sur noir
		  		if(pdebutVille->codePostal != 0){
		  			printf("\n\nSelectionner la ville que vous voulez supprimer :");
			  		cp = SelectVille(pdebutVille , pfinVille, pdebutCentrale, pfinCentrale);
			  		gotoligcol(20,0);
			  		if(cp != -1){	// l utilisateur a bien selectionne une ville
				  		printf("\nEtes-vous sur de vouloir supprimer la ville de code postal %d\n\tTappez :\tO : Oui \t N : Non", cp);
						if(Validation()==1){
							SupVilles(pdebutVille , pfinVille , pdebutCentrale , pfinCentrale , cp , BEN);
							printf("\n\nLa ville de code postal %d a etee retiree du reseau", cp);
						}
						else
							printf("\n\nSuppression annulee");
					}
					else
						printf("\n\nSuppression annulee");
				}
				else
					printf("\n\nLe reseau ne contient aucune ville");
			}
			if(k == '3' || (k == 13 && selectf == 2))//Afficher les villes //////////////////////////////////////////////////////
			{
				ClearConsole(hConsole);
				SetConsoleTextAttribute(hConsole, 16 * 0 + 10);//vert sur noir
		  		printf("\n\n\tAffichage de la liste des villes\n\n");
		  		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);//blanc sur noir
				AfficherVilles(hConsole, pdebutVille , pfinVille, pdebutCentrale , pfinCentrale, 7);//On affiche la liste a la ligne 7
			}
		}
		if(c == '3' || (c == 13 && select == 2))//Menu gestion des lignes electriques /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		{
			selectf = 0;
			AfficheMenuLignes(hConsole, selectf);
			
			k = getch();
			//boucle tant que la touche tappee n est pas definie
			while(k!='1' && k!='2' && k!='3' && k!=27 && k!=13){
				if(k==72 && selectf!=0){//fleche haut
					selectf--;
					AfficheMenuLignes(hConsole, selectf);
				}
				else if(k==80 && selectf!=4){//fleche bas
					selectf++;
					AfficheMenuLignes(hConsole, selectf);
				}
				k = getch();
			}
			
			if(k == '1' || (k == 13 && selectf == 0))//Ajouter des lignes ///////////////////////////////////////////////////////
			{
				ClearConsole(hConsole);
				SetConsoleTextAttribute(hConsole, 16 * 0 + 10);//vert sur noir
				printf("\n\n\tAjout de lignes electriques\n");
				SetConsoleTextAttribute(hConsole, 16 * 0 + 15);//blanc sur noir
	
				/* L'info du nombre de centrales dans le reseau a ete stockee dans
				pdebutCentrale->codeCentrale => verif que la liste n est pas vide */
				if(pdebutCentrale->codeCentrale != 0 && pdebutVille->codePostal !=0 )
				{
					printf("\n\nSelectionner la centrale d'alimentation :");
			  		numCentrale = SelectCentrale(pdebutCentrale , pfinCentrale);
			  		gotoligcol(20,0);
			  		if(numCentrale != -1){	// l utilisateur a bien selectionne une centrale
			  			ClearConsole(hConsole);
						SetConsoleTextAttribute(hConsole, 16 * 0 + 10);//vert sur noir
						printf("\n\n\tAjout de lignes electriques\n");
						SetConsoleTextAttribute(hConsole, 16 * 0 + 15);//blanc sur noir
	
			  			printf("\n\nSelectionner la ville a desservir avec la centrale %d :", numCentrale);
				  		cp = SelectVille(pdebutVille , pfinVille, pdebutCentrale, pfinCentrale);
				  		gotoligcol(20,0);
				  		if(cp != -1){	// l utilisateur a bien selectionne une ville
				  			printf("\n\nPuissance (en kW) a alouer a la ville : ");
				  			scanf("%d",&puissance);
				  			CreationLigne(pdebutCentrale, pfinCentrale, pdebutVille, pfinVille, numCentrale, cp, puissance, BEN);
				  			printf("\n\nLa ligne electrique a bien ete cree");
					  	}
				  		else
				  			printf("\n\nCreation de ligne electrique annulee");
					}
			  		else
			  			printf("\n\nCreation de ligne electrique annulee");
			  	}
			  	else
			  		printf("\n\nIl n y a aucune centrale ou aucue ville dans le reseau");
			}
			if(k == '2' || (k == 13 && selectf == 1))//Supprimer des lignes /////////////////////////////////////////////////////
			{
				ClearConsole(hConsole);
				SetConsoleTextAttribute(hConsole, 16 * 0 + 10);//vert sur noir
		  		printf("\n\n\tSuppression de lignes electriques\n");
		  		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);//blanc sur noir
		  		if(pdebutCentrale->codeCentrale != 0){
		  			printf("\n\nSelectionner la centrale ayant la ligne que vous voulez supprimer :");
			  		numCentrale = SelectCentrale(pdebutCentrale, pfinCentrale);
			  		if(numCentrale != -1){	// l utilisateur a bien selectionne une centrale
				  		pParcCentrale = NumCentrale(pdebutCentrale, pfinCentrale, numCentrale);
						if(pParcCentrale->villeDependante != pParcCentrale->pfinLigne){
							EmptyLine(7 , 1);
							printf("Selectionner la ligne que vous voulez supprimer :");
							pParcLigne = SelectLigne(pParcCentrale, hConsole);
							if(pParcLigne != NULL){
								printf("\n\nLa ligne tiree depuis la centrale %d allimentant %s d'une puissance de %d kW, a ete retiree du reseau", pParcCentrale->codeCentrale, pParcLigne->villeDesservie->nomVille, pParcLigne->puissance);
								SupLigne(pParcCentrale, pParcLigne, BEN);
							}
							else
								printf("\n\nSuppression annulee");
						}
						else
							printf("\n\nLa centrale ne dessert aucune ville");
					}
					else
						printf("\n\nSuppression annulee");
				}
				else
					printf("\n\nLe reseau ne contient pas de centrales");
			}
			if(k == '3' || (k == 13 && selectf == 2))//Afficher les lignes par rapport aux villes ///////////////////////////////
			{
			ClearConsole(hConsole);
			SetConsoleTextAttribute(hConsole, 16 * 0 + 10);//vert sur noir
			printf("\n\n\tAffichage des lignes electriques raccordees a une ville\n");
	  		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);//blanc sur noir	
	  		DefilerLignesVilles(pdebutCentrale, pfinCentrale, pdebutVille, pfinVille);
			}
			if(k == '4' || (k == 13 && selectf == 3))//Afficher les lignes par rapport aux centrales ////////////////////////////
			{
				ClearConsole(hConsole);
				SetConsoleTextAttribute(hConsole, 16 * 0 + 10);//vert sur noir
				printf("\n\n\tAffichage des lignes electriques raccordees a une centrale\n");
		  		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);//blanc sur noir	
		  		DefilerLignesCentrales(pdebutCentrale, pfinCentrale);
				k = 27; // car "AfficherLignesCentrale" est une fcontion bloquante donc lorsqu'on l'on en sort, on quitte
			}
		}
		if(c == '4' || (c == 13 && select == 3))//Menu gestion du reseau //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		{
			selectf = 0;
			AfficheMenuReseau(hConsole, selectf);
			
			k = getch();
			//boucle tant que la touche tappee n est pas definie
			while(k!='1' && k!='2' && k!='3' && k!=27 && k!=13){
				if(k==72 && selectf!=0){//fleche haut
					selectf--;
					AfficheMenuReseau(hConsole, selectf);
				}
				else if(k==80 && selectf!=3){//fleche bas
					selectf++;
					AfficheMenuReseau(hConsole, selectf);
				}
				k = getch();
			}
			
			if(k == '1' || (k == 13 && selectf == 0))//Charger un reseau ////////////////////////////////////////////////////////
			{
				SetConsoleTextAttribute(hConsole, 16 * 0 + 10);//vert sur noir
		  		printf("\n\n\tCharger un reseau\n");
		  		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);//blanc sur noir
		  		SupprimerReseau(pdebutCentrale, pfinCentrale, pdebutVille, pfinVille, BEN);	//le reseau existant est ecrase
		  		printf("\nNom du fichier a charger : ");
		  		scanf("%s",&nomFichier);
		  		NormalisationFichierTXT(nomFichier); // ajoute l'extension .txt si elle n est pas presente
		  		Charger(pdebutCentrale, pfinCentrale, pdebutVille, pfinVille, nomFichier, BEN);
			}	
			if(k == '2' || (k == 13 && selectf == 1))//Supprimer le reseau actuel ///////////////////////////////////////////////
			{
				SetConsoleTextAttribute(hConsole, 16 * 0 + 10);//vert sur noir
		  		printf("\n\n\tSupprimer le reseau\n");
		  		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);//blanc sur noir
		  		printf("\nEtes vous sur de vouloir supprimer le reseau\n\tTappez :\tO : Oui \t N : Non");
		  		if(Validation()==1){
					SupprimerReseau(pdebutCentrale, pfinCentrale, pdebutVille, pfinVille, BEN);
					printf("\n\nReseau supprime");
				}
				else
					printf("\n\nReseau non supprime");
			}
			if(k == '3' || (k == 13 && selectf == 2))//Enregistrer le reseau actuel dans un fichier texte ///////////////////////
			{
				SetConsoleTextAttribute(hConsole, 16 * 0 + 10);//vert sur noir
		  		printf("\n\n\tEnregistrer le reseau\n");
		  		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);//blanc sur noir
				printf("\nDonnez un nom au fichier : ");
		  		scanf("%s" , nomFichier);
		  		NormalisationFichierTXT(nomFichier); // ajoute l'extension .txt si elle n est pas presente -> forcer la creation d un fichier texte
		  		Enregistrer(pdebutCentrale, pfinCentrale, pdebutVille, pfinVille, nomFichier);
			}
		}
		if(c == 27 || (c == 13 && select == 4)) // echap pour quitter /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			break;

		SetConsoleTextAttribute(hConsole, 16 * 8 + 15);//gris sur noir
		printf("\n\n\n\tAppuyer sur une touche pour continuer");
		SetConsoleTextAttribute(hConsole, 16 * 0 + 15);//blanc sur noir
		c = getch(); c = 32;//on donne a c une valeur inutilisee
	}
}
