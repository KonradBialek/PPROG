#include "modul.h"  /* z struktura opisujaca obraz, opcje i dokumentacja funkcji */

int czytaj(FILE *plik_we, t_obraz *obrazek) {
  char buf[DL_LINII];  /* bufor pomocniczy do czytania naglowka i komentarzy */
  int znak;            /* zmienna pomocnicza do czytania komentarzy */
  int koniec=0;        /* czy napotkano koniec danych w pliku */
  int i,j;
  
  /*Sprawdzenie czy podano prawidlowy uchwyt pliku */
  if (plik_we==NULL) {
    fprintf(stderr,"Blad asercji: funkcja czytaj.\n");
    fprintf(stderr,"Nie podano uchwytu do pliku\n");
    return(0);
  }
  
  /* Sprawdzenie "numeru magicznego" - powinien byæ P2 */
  if (fgets(buf,DL_LINII,plik_we)==NULL)   /* Wczytanie pierwszej linii pliku do bufora */
    koniec=1;                              /* Nie udalo sie? Koniec danych! */
  if ( (buf[0]!='P') || ((buf[1]!='2'&&buf[1]!='3')) || koniec) {  /* Czy jest magiczne "P2" lub "P3"? */
    fprintf(stderr,"Blad asercji: funkcja czytaj.\n");
    fprintf(stderr,"To nie jest plik PGM ani PPM.\n");
    return(0);
  }
  if(buf[1]=='2')
    obrazek->typ=1; /* dla obrazow ze skala szarosci */
  else
    obrazek->typ=3; /* dla obrazow kolorowych        */
  
  /* Pominiecie komentarzy */
  do {
    if ((znak=fgetc(plik_we))=='#') {         /* Czy linia rozpoczyna sie od znaku '#'? */
      if (fgets(buf,DL_LINII,plik_we)==NULL)  /* Przeczytaj ja do bufora                */
	koniec=1;                   /* Zapamietaj ewentualny koniec danych */
    }  else {
      ungetc(znak,plik_we);                   /* Gdy przeczytany znak z poczatku linii */
    }                                         /* nie jest '#' zwroc go                 */
  } while (znak=='#' && !koniec);   /* Powtarzaj dopoki sa linie komentarza */
  /* i nie nastapil koniec danych         */
  
  /* Pobranie wymiarow obrazu i liczby odcieni */
  if (fscanf(plik_we,"%d %d %d",&(obrazek->wymx),&(obrazek->wymy),&(obrazek->odcieni))!=3) {
    fprintf(stderr,"Blad asercji: funkcja czytaj.\n");
    fprintf(stderr,"Brak wymiarow obrazu lub liczby stopni koloru.\n");
    return(0);
  }
  
  obrazek->piksele = malloc(obrazek->typ*obrazek->wymx*obrazek->wymy*sizeof(int));
  int (*piksele)[obrazek->typ*obrazek->wymx]; /* piksele w strukturze z obrazem */
  piksele=(int(*)[obrazek->typ*obrazek->wymx]) obrazek->piksele;
  
  /* Pobranie obrazu i zapisanie w strukturze obrazek */
  for (i=0;i<obrazek->wymy;i++) {
    for (j=0;j<obrazek->typ*obrazek->wymx;j++) {
      if (fscanf(plik_we,"%d",&(piksele[i][j]))!=1) {
	fprintf(stderr,"Blad asercji: funkcja czytaj.\n");
        fprintf(stderr,"Niewlasciwe wymiary obrazu\n");
	return(0);
      }
    }
  }
  return obrazek->wymx*obrazek->wymy;   /* Czytanie zakonczone sukcesem    */
}                       /* Zwroc liczbe wczytanych pikseli */




int negatyw(t_obraz *obrazek, int *odczytano)
{
  int i, j;
  int (*piksele)[obrazek->wymx];          /* piksele w strukturze z obrazem */
  
  if(*odczytano==0) /* czy plik zostal odczytany i obraz jest w pamieci */
    {
      fprintf(stderr,"Blad asercji: funkcja negatyw.\n");  
      fprintf(stderr,"Brak obrazu w pamieci. Najpierw odczytaj obraz.\n");
      return -1;
    }
  piksele=(int(*)[obrazek->wymx]) obrazek->piksele;
  
  for(i=0;i<obrazek->wymy;i++)
    for(j=0;j<obrazek->wymx;j++) /* odwrocenie wartosci szarosci */
      piksele[i][j]=obrazek->odcieni-piksele[i][j];
  strcat(obrazek->modyfikacje,"negatyw; ");
  /* kontynuacja komentarza modyfikacji */
  
  return obrazek->wymx*obrazek->wymy; 
}



int progowanie(t_obraz *obrazek, int *odczytano, int prog)
{
  int i, j, prog1;
  int (*piksele)[obrazek->wymx];          /* piksele w strukturze z obrazem */

  if(*odczytano==0) /* czy plik zostal odczytany i obraz jest w pamieci */
    {                                 
      fprintf(stderr,"Blad asercji: funkcja progowanie.\n");  
      fprintf(stderr,"Brak obrazu w pamieci. Najpierw odczytaj obraz.\n");
      return -1;
    }
  if(prog>100||prog<0)
    {    /* czy wartosc progu jest wlasciwa */
      fprintf(stderr,"Blad asercji: funkcja progowanie.\n");  
      fprintf(stderr,"Niewlasciwa wartosc progu - spoza przedzialu od 0 do 100.\n");
      return -1;
    }  
  
  piksele=(int(*)[obrazek->wymx]) obrazek->piksele;  
  prog1=prog*obrazek->odcieni/100;
  /* obliczenie progu wyrazonego w odcieniach szarosci */
  for(i=0;i<obrazek->wymy;i++)
    for(j=0;j<obrazek->wymx;j++)
      if(piksele[i][j]<=prog1)
	piksele[i][j]=0;         /* przypisanie czerni */
      else 
	piksele[i][j]=obrazek->odcieni; /* przypisanie bieli */
  strcat(obrazek->modyfikacje,"progowanie; ");
  /* kontynuacja komentarza modyfikacji */
  
  return obrazek->wymx*obrazek->wymy; 
}



int konturowanie(t_obraz *obrazek, int *odczytano)
{
  int i, j;
  int (*piksele)[obrazek->wymx];          /* piksele w strukturze z obrazem */
  int (*tab_kont)[obrazek->wymx];

  if(*odczytano==0) /* czy plik zostal odczytany i obraz jest w pamieci */
    {
      fprintf(stderr,"Blad asercji: funkcja konturowanie.\n");  
      fprintf(stderr,"Brak obrazu w pamieci. Najpierw odczytaj obraz.\n");
      return -1;
    }
  piksele=(int(*)[obrazek->wymx]) obrazek->piksele;
  tab_kont = (int(*)[obrazek->wymx]) malloc(obrazek->wymx*obrazek->wymy*sizeof(int));
  
  for(i=0;i<obrazek->wymy;i++)
    for(j=0;j<obrazek->wymx;j++)
      tab_kont[i][j]=piksele[i][j]; /* kopiowanie obrazu do tablicy tab_kont */

  for(i=0;i<obrazek->wymy;i++) /* zerowanie wspolrzednych tablicy "i" i "j" do wykorzystania */
    for(j=0;j<obrazek->wymx;j++) /* w konturowaniu */
      {  /* 4 wzory dla mozliwych przypadkow */
        if(i==obrazek->wymy-1&&j==obrazek->wymx-1)
	  piksele[i][j]=abs(tab_kont[i][j-1]-tab_kont[i][j])+abs(tab_kont[i-1][j]-tab_kont[i][j]);
	else if(i==obrazek->wymy-1&&j!=obrazek->wymx-1)
	  piksele[i][j]=abs(tab_kont[i][j+1]-tab_kont[i][j])+abs(tab_kont[i-1][j]-tab_kont[i][j]);
	else if(i!=obrazek->wymy-1&&j==obrazek->wymx-1)
	  piksele[i][j]=abs(tab_kont[i][j-1]-tab_kont[i][j])+abs(tab_kont[i+1][j]-tab_kont[i][j]);
	else 
	  piksele[i][j]=abs(tab_kont[i][j+1]-tab_kont[i][j])+abs(tab_kont[i+1][j]-tab_kont[i][j]);
      }
  obrazek->odcieni*=2;
  /* zwiekszenie skali szarosci, aby zmiescic powstale odcienie */
  strcat(obrazek->modyfikacje,"konturowanie; ");
  /* kontynuacja komentarza modyfikacji */
  free(tab_kont);
  
  return obrazek->wymx*obrazek->wymy; 
}



int zapisz(t_obraz *obrazek, int *odczytano, FILE *nowy)
{
  int i, j, element = 0; /* elelment - numer komorki tabeli */
  int (*piksele)[obrazek->typ*obrazek->wymx];/* piksele w strukturze z obrazem */
  
  if(*odczytano==0) /* czy plik zostal odczytany i obraz jest w pamieci */
    {
      fprintf(stderr,"Blad asercji: funkcja zapisz.\n");  
      fprintf(stderr,"Brak obrazu w pamieci. Najpierw odczytaj obraz.\n");
      return -1;
    }
  /* uchwyt do pliku jest poprawny, poniewaz zostal sprawdzony w funkcji */
  /* glownej                                                             */
  
  piksele=(int(*)[obrazek->typ*obrazek->wymx]) obrazek->piksele;
  
  /* zapisanie komentarzy i informacji odpowiednich dla formatu pliku */
  fprintf(nowy,"P%d\n# Nazwa oryginalnego pliku: %s\n# %s\n",obrazek->typ==1?2:3, obrazek->nazwa, obrazek->modyfikacje); 
  fprintf(nowy,"# Nazwa tego pliku: %s\n%d %d\n%d\n", obrazek->nowy_plik,obrazek->wymx,obrazek->wymy,obrazek->odcieni);
  for(i=0;i<obrazek->wymy;i++)
    for(j=0;j<obrazek->typ*obrazek->wymx;j++)
      {
	fprintf(nowy,"%4d", piksele[i][j]);  /* zapisywanie obrazu */
	element++;
	if(element%17==0)
	  fprintf(nowy,"\n");
	/* koniec linii - ograniczenie 70 znakow w linii pliku*/
      } /* obrazy czarno-biale i kolorowe sa zapisywane tak samo */
  fprintf(nowy,"\n");/* znak konczacy plik - potrzebny do poprawnego otwarcia pliku przez program display*/
  if(nowy!=stdin)
    fclose(nowy);
  return obrazek->wymx*obrazek->wymy; 
}



int konwersja(t_obraz *obrazek, int *odczytano)
{
  int i, j;
  int (*piksele)[obrazek->typ*obrazek->wymx];/*piksele w strukturze z obrazem*/
  int (*piksele2)[obrazek->wymx];        /* piksele2 dla obrazu po konwersji */
  int (*zapas)[obrazek->typ*obrazek->wymx];/*zapas przechowuje kolorowy obraz*/
  /* w tej funkcji obrazek->typ=3 do zmiany z 3 na 1 na  koncu */
  
  if(*odczytano==0) /* czy plik zostal odczytany i obraz jest w pamieci */
    {
      fprintf(stderr,"Blad asercji: funkcja konwersja.\n");  
      fprintf(stderr,"Brak obrazu w pamieci. Najpierw odczytaj obraz.\n");
      return -1;
    }

  zapas=(int(*)[obrazek->typ*obrazek->wymx]) malloc(obrazek->typ*obrazek->wymx*obrazek->wymy*sizeof(int));
  piksele=(int(*)[obrazek->typ*obrazek->wymx]) obrazek->piksele;

  for(i=0;i<obrazek->wymy;i++)
    for(j=0;j<obrazek->typ*obrazek->wymx;j++)
      zapas[i][j]=piksele[i][j];  /* kopiowanie obrazu do tablicy zapas */

  free(obrazek->piksele);
  obrazek->piksele = malloc(obrazek->wymx*obrazek->wymy*sizeof(int));
  piksele2=(int(*)[obrazek->wymx]) obrazek->piksele;
                           /* konwersja do szarosci na podstawie  */
  for(i=0;i<obrazek->wymy;i++)  /* sredniej arytmetycznej kolorow */
    for(j=0;j<obrazek->wymx;j++) 
      piksele2[i][j]=(zapas[i][3*j]+zapas[i][3*j+1]+zapas[i][3*j+2])/3;
  /* piksele[i][j] sa typu int, dlatego czesci niecalkowite sa pomijane */
  obrazek->typ=1;
  strcat(obrazek->modyfikacje,"konwersja do szarosci; ");
  /* kontynuacja komentarza modyfikacji */
  free(zapas);
  return obrazek->wymx*obrazek->wymy; 
}
