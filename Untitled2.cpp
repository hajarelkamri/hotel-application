#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define Nch 50
#include <windows.h>
#include <time.h>
#include <unistd.h>

void menuC();
void menu();
void menuA();
void affReservations();
void met_chambre();
void affclient();
void modification(int m);
void suppression(int m);
void reservationA();

typedef struct
{
    int jour;
    int mois;
    int annee;
} date;

typedef struct
{
    int Numchambre ;
    int type;
    float prix;
    int etat;
} chambre;

typedef struct
{
    int Id;
    char NP[40];
    int Ntele;
    int Nchambre;
}client;


typedef struct {
    int Nchambre;
    date debut;
    date fin;
    int duree;
    int IdClient;
} Reservation;

//************************************

int estDateValide(int jour, int mois, int annee) {
    // Verrifier si le jour, le mois et l'annee sont dans des plages valides
    if (annee < 1000 || annee > 2050 || mois < 1 || mois > 12 || jour < 1 || jour > 31) 
        return 0; // Les valeurs ne sont pas valides

    time_t tempsActuelUnix;
    struct tm *tempsActuel;

    // Obtenir la date et l'heure actuelles
    time(&tempsActuelUnix);
    tempsActuel = localtime(&tempsActuelUnix);

    // Verifier si la date saisie est valide par rapport a  la date actuelle
    if (annee < tempsActuel->tm_year + 1900) {
        return 0; // Annee passee
    } else if (annee == tempsActuel->tm_year + 1900) {
        if (mois < tempsActuel->tm_mon + 1) {
            return 0; // Mois passee
        } else if (mois == tempsActuel->tm_mon + 1) {
            if (jour < tempsActuel->tm_mday) {
                return 0; // Jour passee
            }
        }
    }

    return 1; // La date est valide
}

int estChambreDisponible(Reservation NR) {
    FILE *file = fopen("reservations.txt", "rb");
    if (file == NULL) {
        return 1;  // La chambre est disponible car le fichier des reservations n'existe pas
    }

    Reservation R;
    while (fread(&R, sizeof(Reservation), 1, file) == 1) {
        // Verifier si la chambre est occupee pendant la periode specifique
	    if (NR.Nchambre == R.Nchambre &&
            ((NR.debut.annee < R.fin.annee) ||
             (NR.debut.annee == R.fin.annee && NR.debut.mois < R.fin.mois) ||
             (NR.debut.annee == R.fin.annee && NR.debut.mois == R.fin.mois && NR.debut.jour <= R.fin.jour)) &&
            ((NR.fin.annee > R.debut.annee) ||
             (NR.fin.annee == R.debut.annee && NR.fin.mois > R.debut.mois) ||
             (NR.fin.annee == R.debut.annee && NR.fin.mois == R.debut.mois && NR.fin.jour >= R.debut.jour))) {
            fclose(file);
            return 0;  // La chambre n'est pas disponible pour la periode specifique
        }
    }

    fclose(file);
    return 1;  // La chambre est disponible
}

float genererSoldeAleatoire()
{
    return (rand() % 4000) + 1000;
}

void genererFacture(client *c, chambre *ch)
{
    // Afficher les details de la facture
    printf("\n\n\t\t\t\t\t\t       _________________________________________________________");
    printf("\n\t\t\t\t\t\t         |                        FACTURE                         |");
    printf("\n\n\t\t\t\t\t\t\t\tClient : %s\n", c->NP);
    printf("\t\t\t\t\t\t\t\tChambre reservee : %d\n", ch->Numchambre);
    printf("\t\t\t\t\t\t\t\tMontant a payer : %.2f\n", ch->prix);
    printf("\n\t\t\t\t\t\t         _________________________________________________________\n");
}


void mdp()
{
    FILE *f;
    f=fopen("mot de passe.txt","r");
    char a[20];
    char b[20];
    fscanf(f,"%s",a);
    int c=0;
    int tentative=1;
    printf("\n\n\n\n\n\t\t\t\t\t\t    ______________________________________________________");
    printf("\n");
    do
    {
        printf("\t\t\t\t\t\t\t\tSaisir votre mot de passe: ");
        scanf("%s",b);
        if(strcmp(a,b)==0)
        {
            Sleep(100);
            printf("\n\n\t\t\t\t\t\t\t         == mot de passe correct ==");
            c=1;
        }
        else
        {
            printf("\n\t\t\t\t\t\t    _________________________________________________________");
            printf("\n\t\t\t\t\t\t\t\t* Mot de passe incorrect *\n ");
            tentative++;
        }
    }
    while(c==0);

    fclose(f);
}
//************************************




//****************client****************
void reservation()
{
    int counter,id;
    FILE* f;
    FILE* g;
    FILE* h;
    client E;
    chambre C;
    Reservation R;
    f = fopen("cptclient.txt", "r");
    fscanf(f, "%d", &counter);
    fclose(f);
    h= fopen("cptId.txt", "r");
    fscanf(h, "%d", &id);
    fclose(h);
    printf("\n\n\t\t\t\t\t\t        _________________________________________________________");
    printf("\n\n\t\t\t\t\t\t\t\tSaisir votre nom complet: ");
    fflush(stdin);
    gets(E.NP);
    printf("\t\t\t\t\t\t\t\tSaisir votre numero de telephone:(+212)");
    scanf("%d",&E.Ntele);
    printf("\t\t\t\t\t\t\t\tEntrer le numero de la chambre a reserver :");
    scanf("%d", &R.Nchambre);
    E.Nchambre=R.Nchambre;
    printf("\t\t\t\t\t\t\t\tQuand voulez vous commencer votre reservation ?");
    here:
	printf("\n\t\t\t\t\t\t\t\tDonnez le jour : ");
    scanf("%d", &R.debut.jour);
    printf("\t\t\t\t\t\t\t\tDonnez le mois : ");
    scanf("%d", &R.debut.mois);
    printf("\t\t\t\t\t\t\t\tDonnez l'annee : ");
    scanf("%d", &R.debut.annee);
    printf("\t\t\t\t\t\t\t\tNombre de jours a reserver: ");
    scanf("%d", &R.duree);
    if (!estDateValide(R.debut.jour,R.debut.mois,R.debut.annee))
    {
    	printf("\t\t\t\t\t\t\t\tVeuillez saisie une date valide: ");
    	goto here;
	}
    R.fin.jour = R.debut.jour + R.duree - 1;
   	R.fin.mois = R.debut.mois;
	R.fin.annee = R.debut.annee;
	if(estChambreDisponible(R))
	{
    	float prix;
		float solde= genererSoldeAleatoire();
		FILE* i=fopen("chambres.txt","rb");
    	while (fread(&C, sizeof(chambre), 1, i) == 1) 
		{
        	if (C.Numchambre == R.Nchambre) 
			{
	
        		prix = C.prix*R.duree;
				break;
			}	
        }
        fclose(i);        
	    // Afficher les details de la facture
    	system("cls");
		printf("\n\n\t\t\t\t\t\t       _________________________________________________________");
    	printf("\n\t\t\t\t\t\t         |                        FACTURE                         |");
    	printf("\n\n\t\t\t\t\t\t\t\tClient : %s\n", E.NP);
    	printf("\t\t\t\t\t\t\t\tChambre reservee : %d\n", R.Nchambre);
   		printf("\t\t\t\t\t\t\t\tMontant a payer : %.2f\n", prix);
    	printf("\n\t\t\t\t\t\t         _________________________________________________________\n\n\n");		
    	printf("\n\t\t\t\t\t\tVeuillerz saisir votre numero de compte: ");
		int N;
		scanf("%d",&N);			
		if (solde >= prix)
        	printf("\n\t\t\t\t\t\tPaiement effectue avec succes.");
    	else
    	{
        	printf("\n\t\t\t\t\t\tLe solde n'est pas suffisant pour effectuer ce paiement.\n");
			solde= genererSoldeAleatoire();
			return;
			
		}
	
		g = fopen("clients.txt", "a");

    	
		E.Id=id+1;
    	fwrite(&E, sizeof(client), 1, g);
    	fclose(g);
    	printf("\n\n\t\t\t\t\t\t        _________________________________________________________");
    	printf("\n\n\n\t\t\t\t\t\t\t\tVous avez reserve la chambre %d avec succes.\n\t\t\t\t\t\t\t\t\t  == Votre Id est %d ==",R.Nchambre,E.Id);
        
		FILE *j = fopen("reservations.txt", "ab");
    	if (j == NULL) 
		{
        	printf("Erreur lors de l'ouverture du fichier des reservations.\n");
        	return;
    	}
		R.IdClient= E.Id;
    	// Enregistrer la reservation dans le fichier
    	fwrite(&R, sizeof(Reservation), 1, j);
    	
    	fclose(j);
			
		f = fopen("cptclient.txt", "w");
    	fprintf(f, "%d", counter+1);
    	fclose(f);
        h = fopen("cptId.txt", "w");
    	fprintf(h, "%d", id+1);
    	fclose(h);
    	f=fopen("chambres.txt","rb");
    	g=fopen("tppp.txt","wb");
    	do
    	{
    	    fread(&C, sizeof(chambre), 1, f);
       		if (C.Numchambre!=R.Nchambre) fwrite(&C, sizeof(chambre), 1, g);
    	}
    	while(C.Numchambre!=R.Nchambre);
    	C.etat=E.Id;
    	fwrite(&C, sizeof(chambre), 1, g);
    	while(!feof(f))
    	{
    	    fread(&C, sizeof(chambre), 1, f);
        	fwrite(&C, sizeof(chambre), 1, g);
    	}
    	fclose(f);
    	fclose(g);
    	remove("chambres.txt");
    	rename("tppp.txt","chambres.txt");		
	}
	else 
	   	printf("\n\n\n\t\t\t\t\t\t\t\tLa chambre %d est deja reservee.\n",R.Nchambre);	
}
void aff_vide()
{
    chambre E;
    FILE *f;
    int i;
    f = fopen("chambres.txt", "rb");
    for (i=0; i<Nch; i++)
    {
        fread(&E, sizeof(chambre), 1, f);
        if (E.etat==0)
        {
            printf("\n\t\t\t\t\t\t\t   ________________________________________________________");
            printf("\n\n\t\t\t\t\t\t\t\t\t le numero de la chambre : %d", E.Numchambre);
            printf("\n\t\t\t\t\t\t\t\t\t le type de la chambre :");
            if (E.type==1) printf("Individuelle");
            if (E.type==2) printf("a deux");
            printf("\n\t\t\t\t\t\t\t\t\t le prix de la chambre : %.2f Dhs/j\n", E.prix);
        }
    }
    fclose(f);
}

void met_reservation()
{
    int m,c;
    char Np[50];
    int i, w, counter;
    client E, F;
    FILE *f;
    FILE *ww;

    printf("\n\t\t\t\tBonjour, saisir votre Id et votre nom pour effectuer des modifications ou supprimer votre reservation: ");
h:
    printf("\n\t\t\t\tSaisir votre Id: ");
    scanf("%d", &m);
    printf("\t\t\t\tSaisir votre nom complet: ");
    fflush(stdin);
    gets(Np);
    f = fopen("clients.txt","rb");
    ww = fopen("cptclient.txt", "r");
    fscanf(ww, "%d", &counter);
    //tester si l'Id(m) est valide******************************************************************
    fclose(ww);
    for (i=1; i<=counter; i++)
    {
        fread(&E, sizeof(client), 1, f);
        if (E.Id==m && strcmp(E.NP,Np)==0)
		{
			c++;
			break;
		} 
    }
    fclose(f);
	if(c!=0)
	{
    printf("\n\t\t\t\t\t\t    _________________________________________________________");
    printf("\n\n\n\t\t\t\t\t\t\t\t    Que voulez-vous faire?");
    printf("\n\t\t\t\t\t\t\t\t 1-Modifier votre reservation.");
    printf("\n\t\t\t\t\t\t\t\t 2-Supprimer votre reservation.");
    printf("\n\t\t\t\t\t\t\t\t 3-Revenir au menu.");
    printf("\n\n\n\t\t\t\t\t\t\t\t     Choisir une option:");
start:
    scanf("%d",&w);
    if(w!=1&&w!=2&&w!=3)
    {
        printf("\n\t\t\t\t\t\tOption non valide, choisir une option valide:");
        goto start;
    }
    else if(w==3)
    {
        system("cls");
        menuC();
    }
    else if(w==1)
    {
        system("cls");
        printf("\n\n\t\t\t\t\t\t    _________________________________________________________");
        printf("\n\n\t\t\t\t\t\t\t Salut cher client voici vos donnees initiales:\n\n");
        printf("\t\t\t\t\t\t\t ID: %d\n", E.Id);
        printf("\t\t\t\t\t\t\t Le nom et le prenom: %s\n", E.NP);
        printf("\t\t\t\t\t\t\t Numero de telephone: %d\n",E.Ntele);
        //printf("\t\t\t\t\t\t\t Numero de chambre: %d\n",E.Nchambre);

        modification(m);
    }
    else
    {
        system("cls");
        suppression(m);
    }
	}
	else printf("\t\t\t\t\t\t\t Pas de client possedant ce id.");
}
//***************client*******************

void suppression(int m)
{
    int counter, i, k,x=0;
    client E;
    chambre C;
    FILE *f=NULL;
    FILE *g=NULL;
    FILE *aa=NULL;
    aa = fopen("cptclient.txt", "r");
    fscanf(aa, "%d", &counter);
    fclose(aa);
    f = fopen("clients.txt","rb");
    g = fopen("temp.txt","wb");
    for (i=1; i<=counter; i++)
    {
        fread(&E, sizeof(client), 1, f);
        if (E.Id!=m)
        {
            fwrite(&E, sizeof(client), 1,g);
        }
        else {
        	k=E.Nchambre;
        	x++;
        }
    }
    fclose(f);
    fclose(g);
    remove("clients.txt");
    rename("temp.txt", "clients.txt");
    if(x!=0){
		printf("\n\n\n\n\t\t\t\t\t\t    _________________________________________________________");
    	printf("\n\n\t\t\t\t\t\t\tVotre reservation est supprimee avec succes");
    	printf("\n\n\t\t\t\t\t\t    _________________________________________________________");
	}
	else printf("\n\n\t\t\t\t\t\t\tPas de Client possedant ce id.");

	f = fopen("cptclient.txt", "w");
    fprintf(f, "%d", counter-1);
    fclose(f);
	f = fopen("chambres.txt", "rb");
	g = fopen("ppp.txt", "wb");

	while (fread(&C, sizeof(chambre), 1, f) == 1) {
    	if (C.Numchambre == k) {
        C.etat = 0;
    	}
    	fwrite(&C, sizeof(chambre), 1, g);
	}

	fclose(f);
	fclose(g);

	// Maintenant, vous pouvez supprimer et renommer les fichiers si necessaire
	remove("chambres.txt");
	rename("ppp.txt", "chambres.txt");
    // Ouvrir le fichier des reservations
    Reservation R;
    FILE* o = fopen("reservations.txt", "rb");
    FILE* p = fopen("ttt.txt", "wb");
    for (i = 1; i <= counter; i++)
    {
        // Supprimer la reservation associe a  ce client
        while (fread(&R, sizeof(Reservation), 1, o) == 1)
       {
	        if (R.IdClient != m )
           	{
               fwrite(&R, sizeof(Reservation), 1, p);
            }
        }    
    }
    fclose(o);
    fclose(p);
   	remove("reservations.txt");
	rename("ttt.txt", "reservations.txt");


}
void modification(int m)
{

    char x;
    int i,c=0;
    client E,F;
    Reservation R,S,B;
    int counter;
    FILE *f = fopen("clients.txt","rb");
    FILE *g = fopen("temp.txt","wb");
    FILE *h = fopen("cptclient.txt", "r");
    FILE *k = fopen("reservations.txt", "rb");
    FILE *j = fopen("temp2.txt", "wb");
    fscanf(h, "%d", &counter);
    fclose(h);
    for (i=1; i<=counter; i++)
    {
        fread(&E, sizeof(client), 1, f);
        if (E.Id!=m)
        {
            fwrite(&E, sizeof(client), 1, g);
        }
    }
    while (fread(&S, sizeof(Reservation), 1, k)==1)
    {
        if (S.IdClient!=m)
        {
            fwrite(&S, sizeof(Reservation), 1, g);
        }
        else c++;
    }
    if(x!=0){
    F=E;
    S=R;
    printf("\n\n\t\t\t\t\t\t    _________________________________________________________");
    printf("\n\n\t\t\t\t\t\t\tVoulez-vous modifier le nom et le prenom ? (o/n): ");
    fflush(stdin);
    scanf("%c",&x);
    if(x=='o')
    {
        printf("\t\t\t\t\t\t\tSaisir un nouveau nom et prenom: ");
        fflush(stdin);
        gets(F.NP);
    }

    printf("\n\t\t\t\t\t\t\tVoulez-vous modifier le numero de telephone ? (o/n): ");
    fflush(stdin);
    scanf("%c",&x);
    if(x=='o')
    {
        printf("\t\t\t\t\t\t\tSaisir un nouveau Num de tele: ");
        scanf("%d",&F.Ntele);
    }

    printf("\n\t\t\t\t\t\t\tVoulez-vous modifier la date du debut de la reservation ? (o/n): ");
    fflush(stdin);
    scanf("%c",&x);
    if(x=='o')
    {
    	B=R;
H:
        printf("\t\t\t\t\t\t\tSaisir une nouvelle date:\n");
        printf("\t\t\t\t\t\t\tDonnez le jour: ");
        scanf("%d", &B.debut.jour);
        printf("\t\t\t\t\t\t\tDonnez le mois: ");
        scanf("%d", &B.debut.mois);
        printf("\t\t\t\t\t\t\tDonnez l'annee: ");
        scanf("%d", &B.debut.annee);
  		if (!estDateValide(B.debut.jour,B.debut.mois,B.debut.annee))
 		{
   			printf("\t\t\t\t\t\t\t\tVeuillez saisie une date valide: ");
 			goto H;
		}        
       	if(estChambreDisponible(B))
        {
  			R=B;          	
		}
		else
		{
			printf("\n\n\n\t\t\t\t\t\t\t\tLa chambre %d est deja reservee pendant cette date.\n",B.Nchambre);	
			goto H;
		}
	}

    printf("\n\t\t\t\t\t\t\tVoulez-vous modifier le nombre de jours a reserver ? (o/n): ");
    fflush(stdin);
    scanf("%c",&x);
    if(x=='o')
    {
        printf("\t\t\t\t\t\t\tSaisir une nouvelle duree: ");
        scanf("%d",&R.duree);
    }
    R.fin.jour = R.debut.jour + R.duree - 1;
   	R.fin.mois = R.debut.mois;
	R.fin.annee = R.debut.annee;
	fwrite(&R, sizeof(Reservation), 1, j);
    fwrite(&F, sizeof(client), 1, g);
    printf("\n\n\n\t\t\t\t\t\t\tVotre modification est effectuee avec succes.");
    do
    {
        fread(&E, sizeof(client), 1, f);
        fwrite(&E, sizeof(client), 1, g);
    }
    while (!feof(f));
    do
    {
        fread(&S, sizeof(Reservation), 1, k);
        fwrite(&E, sizeof(client), 1, j);
    }
    while (!feof(k));
    fclose(f);
    fclose(g);
    fclose(k);
    fclose(j);
    remove("reservations.txt");
	rename("temp2.txt", "reservations.txt");
    remove("clients.txt");
	rename("temp.txt", "clients.txt");
	}
	else printf("\n\n\t\t\t\t\t\t\tPas de clent possedant ce id.");

}


//***************admin******************
void affchambre()
{
    chambre E;  
    FILE *f;
    int i;
    f = fopen("chambres.txt", "rb");
    for (i=0; i<Nch; i++)
    {
        fread(&E, sizeof(chambre), 1, f);
        printf("\n\n\t\t\t\t\t\t    _________________________________________________________");
        printf("\n\n\t\t\t\t\t\t\t\tle numero de la chambre : %d", E.Numchambre);
        printf("\n\t\t\t\t\t\t\t\tle type de la chambre :");
        if (E.type==1) printf("Individuelle");
        else  printf("a deux");
        printf("\n\t\t\t\t\t\t\t\tla chambre est ");
        if(E.etat==0)printf("vide.");
        else printf("reservee.");
        printf("\n\t\t\t\t\t\t\t\tle prix de la chambre : %f Dhs/j", E.prix);
        printf("\n");
    }
    fclose(f);
}
void chambres()
{
    int n=Nch, i;
    FILE *f;
    chambre E;
    f = fopen("chambres.txt", "wb");
    for (i=1; i<n+1; i++)
    {
        E.Numchambre=i;
        if(i<26) E.type=1;
        else E.type=2;
        E.prix=200+i*(i-1);
		E.etat=0;
        fwrite(&E, sizeof(chambre), 1, f);
    }
    fclose(f);
    f = fopen("cptchambre.txt", "w");
    fprintf(f, "%d", n);
    fclose(f);
    f = fopen("cptclient.txt", "w");
    fprintf(f, "%d", 0);
    fclose(f);
    f = fopen("cptId.txt", "w");
    fprintf(f, "%d", 0);
    fclose(f);
    remove("reservations.txt");

}

void modif_reservation()
{

    int m;
    printf("\n\n\n\t\t\t\t\t\t  _________________________________________________________");
    printf("\n\n\t\t\t\t\t\t\tEntrer l'Id du client reservant: ");
    scanf("%d", &m);

    Reservation R,S;
    FILE *f;
    FILE *g, *aa;
    int counter;

    // Ouvrir le fichier des reservations en mode lecture
    f = fopen("reservations.txt", "rb");
    if (f == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier des reservations.");
        return;
    }

    // Ouvrir un fichier temporaire en mode ecriture
    g = fopen("temp.txt", "wb");
    if (g == NULL)
    {
        printf("Erreur lors de la crÃ©ation du fichier temporaire.");
        fclose(f);
        return;
    }

    aa = fopen("cptclient.txt", "r");
    if (aa == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier cptclient.");
        fclose(f);
        fclose(g);
        return;
    }

    // Lire le counter depuis le fichier cptclient.txt
    fscanf(aa, "%d", &counter);
    fclose(aa);

    // Parcourir le fichier des rÃ©servations
    while (fread(&R, sizeof(Reservation), 1, f) == 1)
    {
        // Si l'Id du client correspond, effectuer les modifications
        if (R.IdClient == m)
        {
            char x;

            printf("\n\n\n\t\t\t\t\t\tVoulez-vous modifier la date du debut de la reservation ? (o/n): ");
            zz:
            fflush(stdin);
            scanf("%c", &x);
            if (x != 'o' && x != 'n')
            {
                printf("\t\t\t\t\t\t\tEntrer un choix valide: ");
                goto zz;
            }
            if (x == 'o')
            {
         		S=R;
H:           
			    printf("\t\t\t\t\t\tSaisir une nouvelle date:\n");
                printf("\t\t\t\t\t\tDonnez le jour: ");
                scanf("%d", &S.debut.jour);
                printf("\t\t\t\t\t\tDonnez le mois: ");
                scanf("%d", &S.debut.mois);
                printf("\t\t\t\t\t\tDonnez l'annee: ");
                scanf("%d", &S.debut.annee);
        		if (!estDateValide(S.debut.jour,S.debut.mois,S.debut.annee))
    			{
    				printf("\t\t\t\t\t\tVeuillez saisie une date valide: \n");
    				goto H;
				}        
            	if(estChambreDisponible(S))
            	{
  					R=S;          	
				}
				else
				{
					printf("\n\n\n\t\t\t\t\t\t\t\tLa chambre %d est deja reservee pendant cette date.\n",S.Nchambre);	
					goto H;
				}
            }
   		
			printf("\n\t\t\t\t\t\t\t\tVoulez-vous modifier le nombre de jours a reserver ? (o/n): ");
            rr:
            fflush(stdin);
            scanf("%c", &x);
            if (x != 'o' && x != 'n')
            {
                printf("\t\t\t\t\t\tEntrer un choix valide: ");
                goto rr;
            }
            if (x == 'o')
            {
                printf("\t\t\t\t\t\tSaisir une nouvelle duree: ");
                scanf("%d", &R.duree);

                // Mise a  jour de la date de fin en fonction de la nouvelle duree
                R.fin.jour = R.debut.jour + R.duree - 1;
                R.fin.mois = R.debut.mois;
                R.fin.annee = R.debut.annee;
            }
            else
            {
            	R.fin.jour = R.debut.jour + R.duree - 1;
                R.fin.mois = R.debut.mois;
                R.fin.annee = R.debut.annee;
			}
        }
        else printf("\t\t\t\t\t\tPas de Client possedant ce id.");

        // Ã‰crire la reservation dans le fichier temporaire
        fwrite(&R, sizeof(Reservation), 1, g);
    }

    // Fermer les fichiers
    fclose(f);
    fclose(g);

    // Supprimer le fichier des reservations
    remove("reservations.txt");

    // Renommer le fichier temporaire
    int ret = rename("temp.txt", "reservations.txt");
    if (ret != 0)
    {
        printf("Erreur lors du renommage du fichier temporaire.");
    }
}

void met_reserv_admin()
{
    int x;
    printf("\n\n\n\t\t\t\t\t\t  _________________________________________________________");
    printf("\n\n\t\t\t\t\t\t\t\t   Que voulez-vous faire?\n\n\n");
    printf("\t\t\t\t\t\t\t\t1-Ajouter un client\n");
    printf("\t\t\t\t\t\t\t\t2-Modifier une reservation\n");
    printf("\t\t\t\t\t\t\t\t3-Supprimer une reservation\n");
    printf("\t\t\t\t\t\t\t\t4-Retour\n");
    printf("\n\n\n\t\t\t\t\t\t\t\t    Choisir une option: ");

re:
    scanf("%d",&x);
    int m;
    switch(x)
    {
    case 1:
        system("cls");
        reservationA();
        break;
    case 2:
        system("cls");
  		modif_reservation();
        break;
    case 3:
        system("cls");
        printf("\n\n\n\t\t\t\t\t\t  _________________________________________________________");
        printf("\n\n\t\t\t\t\t\t\tSaisir l'Id du client que vous voulez supprimer: ");
        scanf("%d",&m);
        suppression(m);
        break;
    case 4:
        system("cls");
        menuA();
        break;
    default:
        printf("\nChoisir une option valide:");
        goto re;
        break;
    }
}

void met_chambre()
{
    int x;
    printf("\n\n\n\t\t\t\t\t\t  _________________________________________________________");
    printf("\n\n\t\t\t\t\t\tSaisir le numero de la chambre que vous voulez mettre a jour:");
    scanf("%d", &x);

    FILE *f;
    FILE *g;
    chambre c;

    f = fopen("chambres.txt", "rb");
    if (f == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier des chambres.");
        return;
    }

    g = fopen("tempch.txt", "ab");
    if (g == NULL)
    {
        printf("Erreur lors de la crÃ©ation du fichier temporaire.");
        fclose(f);
        return;
    }

    while (fread(&c, sizeof(chambre), 1, f) == 1)
    {
        if (c.Numchambre == x)
        {
            char k;

            printf("\n\n\t\t\t\t\t\t\tVoulez-vous modifier le prix de la chambre ? (o/n): ");
            g:
            fflush(stdin);
            scanf("%c", &k);
            if (k != 'o' && k != 'n')
            {
                printf("\n\t\t\t\t\t\t\tChoisir une option valide: ");
                goto g;
            }
            else if (k == 'o')
            {
                printf("\t\t\t\t\t\t\tSaisir le nouveau prix de la chambre:");
                scanf("%f", &c.prix);
            }

            printf("\n\t\t\t\t\t\t\tVoulez-vous modifier l'etat de la chambre ? (o/n): ");
            j:
            fflush(stdin);
            scanf("%c", &k);
            if (k != 'o' && k != 'n')
            {
                printf("\n\t\t\t\t\t\t\tChoisir une option valide: ");
			goto j;
			}
			else if (k == 'o')
			{
				printf("\t\t\t\t\t\t\tSaisir le nouveau etat de la chambre:");
				scanf("%d", &c.etat);
			}
		}
		
		fwrite(&c, sizeof(chambre), 1, g);
			    
	}

			printf("\n\n\n\t\t\t\t\t\t  _________________________________________________________");
			printf("\n\n\t\t\t\t\t\t\tLa chambre est mise a jour avec succes.");

			fclose(f);
			fclose(g);

			remove("chambres.txt");
			rename("tempch.txt", "chambres.txt");
}

void affReservations() 
{
    Reservation R;
    FILE *f;
    int counter, i;

    // Lire le nombre de reservations a  partir du fichier cptclients.txt
    f = fopen("cptclient.txt", "r");
    if (f == NULL) {
        printf("Erreur lors de l'ouverture du fichier cptclient.txt.\n");
        return;
    }
    fscanf(f, "%d", &counter);
    fclose(f);

    // Ouvrir le fichier des reservations
    f = fopen("reservations.txt", "rb");
    if (f == NULL) {
        printf("Erreur lors de l'ouverture du fichier des rÃ©servations.\n");
        return;
    }

    // Afficher les reservations
    for (i = 0; i < counter; i++) {
        fread(&R, sizeof(Reservation), 1, f);
        printf("\n\n\t\t\t\t\t\t    _________________________________________________________");
        printf("\n\n\t\t\t\t\t\t\t\t\t== Reservation %d ==", i + 1);
        printf("\n\n\t\t\t\t\t\t\t\tNumero de chambre: %d", R.Nchambre);
        printf("\n\t\t\t\t\t\t\t\tDate de debut: %d/%d/%d", R.debut.jour, R.debut.mois, R.debut.annee);
        printf("\n\t\t\t\t\t\t\t\tDate de fin: %d/%d/%d", R.fin.jour, R.fin.mois, R.fin.annee);
        printf("\n\t\t\t\t\t\t\t\tDuree de reservation: %d jours", R.duree);
        printf("\n\t\t\t\t\t\t\t\tID du client: %d", R.IdClient);
    }

    fclose(f);
}
   
void affclient()
{
    client E;
    FILE *f;
    int counter, i;
    f= fopen("cptclient.txt", "r");
    fscanf(f, "%d", &counter);
    fclose(f);
    f = fopen("clients.txt", "rb");
    for (i=0; i<counter; i++)
    {
        fread(&E, sizeof(client), 1, f);
        printf("\n\n\t\t\t\t\t\t    _________________________________________________________");
        printf("\n\n\t\t\t\t\t\t\t\t\t== Client %d ==", i+1);
        printf("\n\n\t\t\t\t\t\t\t\tID: %d", E.Id);
        printf("\n\t\t\t\t\t\t\t\tLe nom et le prenom: %s", E.NP);
        printf("\n\t\t\t\t\t\t\t\tNumero de telephone: (+212)%d",E.Ntele);
        printf("\n\t\t\t\t\t\t\t\tNumero de chambre: %d",E.Nchambre);
    }
    fclose(f);
}
void reservationA()
{
 	int counter,id;
 	float prix;
    FILE* f;
    FILE* g;
    FILE* h;
    client E;
    chambre C;
    Reservation R;
    f = fopen("cptclient.txt", "r");
    fscanf(f, "%d", &counter);
    fclose(f);
    h= fopen("cptId.txt", "r");
    fscanf(h, "%d", &id);
    fclose(h);
    printf("\n\n\t\t\t\t\t\t        _________________________________________________________");
    printf("\n\n\t\t\t\t\t\t\t\tSaisir votre nom complet: ");
    fflush(stdin);
    gets(E.NP);
    printf("\t\t\t\t\t\t\t\tSaisir votre numero de telephone:(+212)");
    scanf("%d",&E.Ntele);
    printf("\t\t\t\t\t\t\t\tEntrer le numero de la chambre a reserver :");
    scanf("%d", &R.Nchambre);
    E.Nchambre=R.Nchambre;
    printf("\t\t\t\t\t\t\t\tQuand voulez vous commencer votre reservation ?");
    here:
	printf("\n\t\t\t\t\t\t\t\tDonnez le jour : ");
    scanf("%d", &R.debut.jour);
    printf("\t\t\t\t\t\t\t\tDonnez le mois : ");
    scanf("%d", &R.debut.mois);
    printf("\t\t\t\t\t\t\t\tDonnez l'annee : ");
    scanf("%d", &R.debut.annee);
    printf("\t\t\t\t\t\t\t\tNombre de jours a reserver: ");
    scanf("%d", &R.duree);
    if (!estDateValide(R.debut.jour,R.debut.mois,R.debut.annee))
    {
    	printf("\t\t\t\t\t\t\t\tVeuillez saisie une date valide: ");
    	goto here;
	}
    R.fin.jour = R.debut.jour + R.duree - 1;
   	R.fin.mois = R.debut.mois;
	R.fin.annee = R.debut.annee;
	if(estChambreDisponible(R))
	{
		FILE* i=fopen("chambres.txt","rb");
    	while (fread(&C, sizeof(chambre), 1, i) == 1) 
		{
        	if (C.Numchambre == R.Nchambre) 
			{
	
        		prix = C.prix*R.duree;
				break;
			}	
        }
        fclose(i);        
	    // Afficher les details de la facture
    	system("cls");
		printf("\n\n\t\t\t\t\t\t       _________________________________________________________");
        printf("\n\t\t\t\t\t\t         |                        FACTURE                         |");
		printf("\n\n\t\t\t\t\t\t\t\tClient : %s\n", E.NP);
    	printf("\t\t\t\t\t\t\t\tChambre reservee : %d\n", R.Nchambre);
   		printf("\t\t\t\t\t\t\t\tMontant a payer : %.2f\n", prix);
    	printf("\n\t\t\t\t\t\t         _________________________________________________________\n\n\n");		
	
		g = fopen("clients.txt", "a");
	
		E.Id=id+1;
    	fwrite(&E, sizeof(client), 1, g);
    	fclose(g);
    	printf("\n\n\t\t\t\t\t\t        _________________________________________________________");
    	printf("\n\n\n\t\t\t\t\t\t\t\tVous avez reserve la chambre %d avec succes.\n\t\t\t\t\t\t\t\t\t  == Votre Id est %d ==",R.Nchambre,E.Id);
        
		FILE *j = fopen("reservations.txt", "ab");
    	if (j == NULL) 
		{
        	printf("Erreur lors de l'ouverture du fichier des reservations.\n");
        	return;
    	}
		R.IdClient= E.Id;
    	// Enregistrer la reservation dans le fichier
    	fwrite(&R, sizeof(Reservation), 1, j);
    	fclose(j);
			
		f = fopen("cptclient.txt", "w");
    	fprintf(f, "%d", counter+1);
    	fclose(f);
        h = fopen("cptId.txt", "w");
    	fprintf(h, "%d", id+1);
    	fclose(h);
    	f=fopen("chambres.txt","rb");
    	g=fopen("tppp.txt","wb");
    	do
    	{
    	    fread(&C, sizeof(chambre), 1, f);
       		if (C.Numchambre!=R.Nchambre) fwrite(&C, sizeof(chambre), 1, g);
    	}
    	while(C.Numchambre!=R.Nchambre);
    	C.etat=E.Id;
    	fwrite(&C, sizeof(chambre), 1, g);
    	while(!feof(f))
    	{
    	    fread(&C, sizeof(chambre), 1, f);
        	fwrite(&C, sizeof(chambre), 1, g);
    	}
    	fclose(f);
    	fclose(g);
    	remove("chambres.txt");
    	rename("tppp.txt","chambres.txt");		
	}
	else 
	   	printf("\n\n\n\t\t\t\t\t\t\t\tLa chambre %d est deja reservee.\n",R.Nchambre);	
	
}
//***************admin******************


void menuC()
{
    int a;
back:
    printf("\n\n\n\t\t\t\t\t\t  _________________________________________________________");
    printf("\n\n\t\t\t\t\t\t\t\t  Bonjour cher client\n");
    printf("\t\t\t\t\t\t\t\t  Que voulez-vous faire?\n\n");
    printf("\n\t\t\t\t\t\t  _________________________________________________________");
    printf("\n\n\t\t\t\t\t\t\t\t1- Reserver une chambre\n");
    printf("\t\t\t\t\t\t\t\t2- Mettre a jour une reservation\n");
    printf("\t\t\t\t\t\t\t\t3- Afficher les chambres vides\n");
    printf("\t\t\t\t\t\t\t\t4- Retour au menu\n\n\n");
    printf("\t\t\t\t\t\t\t\t  Choisir une option SVP: ");
l:
    scanf("%d", &a);
    switch(a)
    {
    case 1:
        system("cls");
        reservation();
        int c;
		while ((c = getchar()) != '\n' && c != EOF);
        printf("\n\n\t\t\t\t\t\tCliquer sur entree pour revenir au menu_client");
        getchar();
        system("cls");
        goto back;
        break;
    case 2:
        system("cls");
        met_reservation();
		while ((c = getchar()) != '\n' && c != EOF);
        printf("\n\n\t\t\t\t\t\tCliquer sur entree pour revenir au menu_client");
        getchar();
        system("cls");
        goto back;
        break;
    case 3:
        system("cls");
        aff_vide();
		while ((c = getchar()) != '\n' && c != EOF);
        printf("\n\n\t\t\t\t\t\tCliquer sur entree pour revenir au menu_client");
        getchar();
        system("cls");
        goto back;
        break;
    case 4:
        system("cls");
        menu();
        break;
    default:
    {
        printf("\n\t\t\t\t\t\t\t\tVeuiller choisir une option valide: ");
        goto l;
    }
    }
}


void menuA()
{
k:
    printf("\n\n\n\t\t\t\t\t\t  _________________________________________________________");
    printf("\n\n\t\t\t\t\t\t\t            *  *  *  *  *  *  *  *");
    printf("\n\n\t\t\t\t\t\t\t            *                    *");
    printf("\n\n\t\t\t\t\t\t\t            *    Hello Admin     *");
    printf("\n\n\t\t\t\t\t\t\t            *                    *");
    printf("\n\n\t\t\t\t\t\t\t            *  *  *  *  *  *  *  *");
    Sleep(700);
    printf("\n\n\t\t\t\t\t\t  _________________________________________________________");
    printf("\n\t\t\t\t\t\t\t\t   == Que voulez faire? ==\n\n");
    printf("\t\t\t\t\t\t\t\t  1- Afficher les reservations\n");
    printf("\t\t\t\t\t\t\t\t  2- Afficher les clients\n");
    printf("\t\t\t\t\t\t\t\t  3- Mettre a jour une reservation\n");
    printf("\t\t\t\t\t\t\t\t  4- Afficher les chambres\n");
    printf("\t\t\t\t\t\t\t\t  5- Mettre a jour une chambre\n");
    printf("\t\t\t\t\t\t\t\t  6- Initialiser_DATA\n");
    printf("\t\t\t\t\t\t\t\t  7- Retour au menu\n");
    int a;
    printf("\n\n\t\t\t\t\t\t\t\t    Choisir une option SVP: ");
ll:
    scanf("%d", &a);
    switch(a)
    {
    case 1:
        system("cls");
		affReservations();
		int c;
		while ((c = getchar()) != '\n' && c != EOF);
		printf("\n\n\t\t\t\t\t\t  Cliquer sur entree pour revenir au menu_Admin..");
        getchar();
		system("cls");
        goto k;
        break;
    case 2:
        system("cls");
		affclient();
		while ((c = getchar()) != '\n' && c != EOF);
		printf("\n\n\t\t\t\t\t\t  Cliquer sur entree pour revenir au menu_Admin..");
        getchar();
        system("cls");
        goto k;
        break;
    case 3:
        system("cls");
        met_reserv_admin();
		while ((c = getchar()) != '\n' && c != EOF);
        printf("\n\n\t\t\t\t\t\t  Cliquer sur entree pour revenir au menu_Admin..");
        getchar();
		system("cls");
        goto k;
        break;
    case 4:
        system("cls");
        affchambre();
		while ((c = getchar()) != '\n' && c != EOF);
		printf("\n\n\t\t\t\t\t\t  Cliquer sur entree pour revenir au menu_Admin..");
		getchar();
		system("cls");
        goto k;
        break;
    case 5:
        system("cls");
        met_chambre();
		while ((c = getchar()) != '\n' && c != EOF);
		printf("\n\n\t\t\t\t\t\t  Cliquer sur entree pour revenir au menu_Admin..");
        getchar();
		system("cls");
        goto k;
    case 6:
        system("cls");
        remove("clients.txt");
        remove("chambres.txt");
        remove("cptchambre.txt");
        remove("cptclient.txt");
        chambres();
        printf("\n\n\t\t\t\t\t\t\t   _________________________________________________________");
        printf("\n\n\t\t\t\t\t\t\t      Toutes les reservations sont supprimees avec succes.\n\t\t\t\t\t\t\t   Toutes les chambres sont initialisees a leur etat initiale.");
        printf("\n\n\t\t\t\t\t\t\t   _________________________________________________________");
        printf("\n\n\t\t\t\t\t\t\t  Cliquer sur entree pour revenir au menu_Admin..");
        getchar();
        Sleep(7000);
		system("cls");
        goto k;
    case 7:
        system("cls");
        menu();
        break;
    default:
    {
        printf("\n\t\t\t\t\t\t\t\t    choisir une option valide: ");
        goto ll;
        break;
    }
    }
}


void menu()
{

    printf("\n\n\t\t\t\t\t\t\t   _________________________________________________________");
    printf(" \n\n\t\t\t\t\t\t\t\t           *  *  *  *  *  *  *  *");
    printf("\n\n\t\t\t\t\t\t\t\t           *                    *");
    printf("\n\n\t\t\t\t\t\t\t\t           *     Bienvenue      *");
    printf("\n\n\t\t\t\t\t\t\t\t           *                    *");
    printf("\n\n\t\t\t\t\t\t\t\t           *  *  *  *  *  *  *  *");
    Sleep(1000);
    printf("\n\n\t\t\t\t\t\t\t   ________________________________________________________");
    printf("\n\n\t\t\t\t\t\t\tVoulez-vous se connecter en tant que Admin ou en tant que Client? ");
    printf("\n\n\t\t\t\t\t\t\t\t\t\t 1- Admin\n\t\t\t\t\t\t\t\t\t\t 2- Client");
    printf(" \n\n\t\t\t\t\t\t\t\t\t   Choisir une option(1/2):...");
    int o;
Restart:
    fflush(stdin);
    scanf("%d",&o);
    if(o==1)
    {
        system("cls");
        mdp();
        Sleep(2000);
        system("cls");
        menuA();
    }
    else if(o==2)
    {
        system("cls");
        menuC();
    }
    else
    {
        printf("\t\t\t\t\t\t\t\t\t   Choisir une option valide:.");
        goto Restart;
    }

    getchar();


}
  

void intro()
{
    //Changement de la couleur de l'arriere plan et du text.
    printf("\x1b[104m");
    printf("\x1b[95m");
    Sleep(1000);
    int i;

    printf("\n\n\t\t\t\t\t  ");
    for(i=0; i<74; i++)
    {
        printf("=");
        Sleep(20);
    }
    

    printf("\n\n\t\t\t\t\t  === Application de gestion de reservations des chambres dans un hotel ===");
    Sleep(500);

    printf("\n\n\t\t\t\t\t\t\t         === DATA Engineering 1 ===");
    Sleep(500);
    printf("\n\n\t\t\t\t\t\t\t\t  === Hajar ElKamri ===");
    Sleep(500);
    printf("\n\n\t\t\t\t\t\t\t           === Hanan Gharibi ===");
    Sleep(500);
    printf("\n\t\t\t\t\t  ");
    for(i=0; i<74; i++)
    {
        printf("=");
        Sleep(20);
    }
    printf("\n\n\n\n\n\t\t\t\t\t\t  _________________________________________________________");
    Sleep(200);
    printf("\n\t\t\t\t\t\t     Appuyez sur entree pour continuer... ");
    getchar();
    system("cls");
//Introduction
    printf("\n\n\t\t\t\t\t\t\t   ________________________________________________________");
    printf(" \n\n\n\t\t\t\t\t\t\t\t   <<< chargement, veuillez patienter >>>");

    printf("\n\t\t\t\t\t\t\t\t  ");
    for(i=0; i<41; i++)
    {
        printf(".");
        Sleep(40);
    }
    Sleep(500);
}

main()
{
	system("color FD");
	    system("color FD");
 	printf("  ==  \n =  = \n ====\n =  = \n =  = \n\n\t");
 	Sleep(500);
    printf("===== \n\t=   = \n\t===== \n\t=  =  \n\t=   = \n\n\t\t");
    Sleep(500);
	printf(" ===  \n\t\t  =   \n\t\t  =   \n\t\t  =   \n\t\t ===  \n\n\t\t\t");
    Sleep(500);
    printf("====  \n\t\t\t   =  \n\t\t\t  =   \n\t\t\t =    \n\t\t\t====  \n\n\t\t\t\t");
	Sleep(500);
    printf(" ===  \n\t\t\t\t=   = \n\t\t\t\t=   = \n\t\t\t\t=   = \n\t\t\t\t ===  \n\n\t\t\t\t\t");
    Sleep(500);
	printf("=   = \n\t\t\t\t\t==  = \n\t\t\t\t\t= = = \n\t\t\t\t\t=  == \n\t\t\t\t\t=   = \n\n\t\t\t\t\t\t");
    Sleep(500);
	printf("  ==  \n\t\t\t\t\t\t =  = \n\t\t\t\t\t\t ====\n\t\t\t\t\t\t =  = \n\t\t\t\t\t\t =  = \n\n\t\t\t\t\t\t\t");
	Sleep(500);
	
	
	printf("=   = \n\t\t\t\t\t\t\t=   = \n\t\t\t\t\t\t\t===== \n\t\t\t\t\t\t\t=   = \n\t\t\t\t\t\t\t=   = \n\n\t\t\t\t\t\t\t\t");
	Sleep(500);
	printf(" ===  \n\t\t\t\t\t\t\t\t=   = \n\t\t\t\t\t\t\t\t=   = \n\t\t\t\t\t\t\t\t=   = \n\t\t\t\t\t\t\t\t ===  \n\n\t\t\t\t\t\t\t\t\t");
	Sleep(500);
	printf("===== \n\t\t\t\t\t\t\t\t\t  =   \n\t\t\t\t\t\t\t\t\t  =   \n\t\t\t\t\t\t\t\t\t  =   \n\t\t\t\t\t\t\t\t\t  =   \n\n\t\t\t\t\t\t\t\t\t\t");
    Sleep(500);
	printf("===== \n\t\t\t\t\t\t\t\t\t\t=     \n\t\t\t\t\t\t\t\t\t\t===== \n\t\t\t\t\t\t\t\t\t\t=     \n\t\t\t\t\t\t\t\t\t\t===== \n\n\t\t\t\t\t\t\t\t\t\t\t");
    Sleep(500);
	printf("=     \n\t\t\t\t\t\t\t\t\t\t\t=     \n\t\t\t\t\t\t\t\t\t\t\t=     \n\t\t\t\t\t\t\t\t\t\t\t=     \n\t\t\t\t\t\t\t\t\t\t\t===== \n\n\t\t\t\t\t\t\t\t\t\t\t\t\t");
	Sleep(500);
    
    system("cls");
    intro();
	menu();
    
}
