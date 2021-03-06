/* Konrad Bialek */
/* Numer albumu: 248993 */
/* Data wykonania: 05.12.2018r. */

/* Program obslugujacy czarno-biale obrazy rastrowe zapisane w plikach w formacie PGM */

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define STOP 8             /* Warunek stopu petli */
#define MAX 512            /* Maksymalny rozmiar wczytywanego obrazu */
#define DL_LINII 1024      /* Dlugosc buforow pomocniczych */

/************************************************************************************
 * Funkcja wczytuje obraz PGM z pliku do tablicy                                    *
 *                                                                                  * 
 * \param[in] plik_we uchwyt do pliku z obrazem w formacie PGM                      *
 * \param[out] obraz_pgm tablica, do ktorej zostanie zapisany obraz                 *
 * \param[out] wymx szerokosc obrazka                                               *
 * \param[out] wymy wysokosc obrazka                                                *
 * \param[out] szarosci liczba odcieni szarosci                                     *
 * \return liczba wczytanych pikseli                                                *
 ************************************************************************************/

int czytaj(FILE *plik_we,int obraz_pgm[][MAX],int *wymx,int *wymy, int *szarosci) {
  char buf[DL_LINII];      /* bufor pomocniczy do czytania naglowka i komentarzy */
  int znak;                /* zmienna pomocnicza do czytania komentarzy */
  int koniec=0;            /* czy napotkano koniec danych w pliku */
  int i,j;

  /*Sprawdzenie czy podano prawidlowy uchwyt pliku */
  if (plik_we==NULL) {
    fprintf(stderr,"Blad: Nie podano uchwytu do pliku\n");
    return(0);
  }

  /* Sprawdzenie "numeru magicznego" - powinien byæ P2 */
  if (fgets(buf,DL_LINII,plik_we)==NULL)   /* Wczytanie pierwszej linii pliku do bufora */
    koniec=1;                              /* Nie udalo sie? Koniec danych! */

  if ( (buf[0]!='P') || (buf[1]!='2') || koniec) {  /* Czy jest magiczne "P2"? */
    fprintf(stderr,"Blad: To nie jest plik PGM\n");
    return(0);
  }

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

  /* Pobranie wymiarow obrazu i liczby odcieni szarosci */
  if (fscanf(plik_we,"%d %d %d",wymx,wymy,szarosci)!=3) {
    fprintf(stderr,"Blad: Brak wymiarow obrazu lub liczby stopni szarosci\n");
    return(0);
  }
  
  /* przekroczenie wymiarow obrazu */
  if(*wymx>MAX||*wymy>MAX)
    {
      fprintf(stderr,"Blad: Zbyt duze wymiary obrazu.\n");
      return (0);
    }
    
  /* Pobranie obrazu i zapisanie w tablicy obraz_pgm*/
  for (i=0;i<*wymy;i++) {
    for (j=0;j<*wymx;j++) {
      if (fscanf(plik_we,"%d",&(obraz_pgm[i][j]))!=1) {
	fprintf(stderr,"Blad: Niewlasciwe wymiary obrazu\n");
	return(0);
      }
    }
  }
  return *wymx**wymy;   /* Czytanie zakonczone sukcesem    */
}                       /* Zwroc liczbe wczytanych pikseli */

/************************************************************************************
 * Funkcja dokonuje modyfikacji negatywu na obrazie PGM w tablicy                   *
 *                                                                                  *
 * \param[in] obraz_pgm tablica, w ktorej obraz jest zapisany                       *
 * \param[in] wymx szerokosc obrazka                                                *
 * \param[in] wymy wysokosc obrazka                                                 *
 * \param[in] szarosci liczba odcieni szarosci                                      *
 * \param[in] modyfikacje zmiany wykonane na obrazie w tablicy przed negatywem      *
 * \param[out] obraz_pgm tablica, do ktorej zostanie zapisany zmodyfikowany obraz   *
 * \param[out] modyfikacje zmiany wykonane na obrazie w tablicy razem z negatywem   *
 * \return liczba pikseli obrazu                                                    *
 ************************************************************************************/

int negatyw(int obraz_pgm[][MAX], int *wymx, int *wymy, int *szarosci, char modyfikacje[100])
{
  int i, j;
  
  for(i=0;i<*wymy;i++)
    {
      for(j=0;j<*wymx;j++) /* odwrocenie wartosci szarosci */
	obraz_pgm[i][j]=*szarosci-obraz_pgm[i][j];
    }
  strcat(modyfikacje,"negatyw; "); /* kontynuacja komentarza modyfikacji */
  printf("Wykonano modyfikacje: Negatyw.\n");
  
  return *wymx**wymy;
}

/************************************************************************************
 * Funkcja dokonuje modyfikacji progowania na obrazie PGM w tablicy                 *
 *                                                                                  *
 * \param[in] obraz_pgm tablica, w ktorej obraz jest zapisany                       *
 * \param[in] wymx szerokosc obrazka                                                *
 * \param[in] wymy wysokosc obrazka                                                 *
 * \param[in] szarosci liczba odcieni szarosci                                      *
 * \param[in] modyfikacje zmiany wykonane na obrazie w tablicy przed progowaniem    *
 * \param[out] obraz_pgm tablica, do ktorej zostanie zapisany zmodyfikowany obraz   *
 * \param[out] modyfikacje zmiany wykonane na obrazie w tablicy razem z progowaniem *
 * \return liczba pikseli obrazu                                                    *
 ************************************************************************************/

int progowanie(int obraz_pgm[][MAX], int *wymx, int *wymy, int *szarosci, char modyfikacje[100])
{
  int i, j, prog, prog1;

  printf("Podaj wartosc progu w procentach (od 0 do 100): ");
  scanf("%d",&prog);
  while(prog>100||prog<0) /* gdy uzytkownik wpisze niewlasciwy prog */
    {  
      printf("Niewlasciwy prog. Podaj wartosc progu jeszcze raz: ");
      scanf("%d",&prog);
    }
  prog1=prog**szarosci/100; /* obliczenie progu wyrazonego w odcieniach szarosci */
  for(i=0;i<*wymy;i++)
    {
      for(j=0;j<*wymx;j++)
	{
	  if(obraz_pgm[i][j]<=prog1)
	    obraz_pgm[i][j]=0;         /* przypisanie czerni */
	  else
	    obraz_pgm[i][j]=*szarosci; /* przypisanie bieli */
	}
    }
  strcat(modyfikacje,"progowanie; "); /* kontynuacja komentarza modyfikacji */
  printf("Wykonano modyfikacje: Progowanie.\n");
  
  return *wymx**wymy;
}

/***************************************************************************************
 * Funkcja dokonuje modyfikacji konturowania na obrazie PGM w tablicy                  *
 *                                                                                     *
 * \param[in] obraz_pgm tablica, w ktorej obraz jest zapisany                          *
 * \param[in] wymx szerokosc obrazka                                                   *
 * \param[in] wymy wysokosc obrazka                                                    *
 * \param[in] szarosci liczba odcieni szarosci                                         *
 * \param[in] modyfikacje zmiany wykonane na obrazie w tablicy przed konturowaniem     *
 * \param[out] obraz_pgm tablica, do ktorej zostanie zapisany zmodyfikowany obraz      *
 * \param[out] szarosci liczba odcieni szarosci                                        *
 * \param[out] modyfikacje zmiany wykonane na obrazie w tablicy razem z konturowaniem  *
 * \param[out] tab_kont tablica, do ktorej zostanie skopiowany obraz przed modyfikacja *
 * \return liczba pikseli obrazu                                                       *
 **************************************************************************************/

int konturowanie(int obraz_pgm[][MAX], int *wymx, int *wymy, int *szarosci, char modyfikacje[100], int tab_kont[][MAX])
{
  int i, j;
  
  for(i=0;i<*wymy;i++)
    {
      for(j=0;j<*wymx;j++)
	{
	  tab_kont[i][j]=obraz_pgm[i][j];  /* kopiowanie obrazu do tablicy tab_kont */
	}
    }
  for(i=0;i<*wymy;i++) /* zerowanie wspolrzednych tablicy "i" i "j" do wykorzystania w konturowaniu */
    {
      for(j=0;j<*wymx;j++) /* 4 wzory dla mozliwych przypadkow */
	{  
	  if(i==*wymy-1&&j==*wymx-1)
	    obraz_pgm[i][j]=abs(tab_kont[i][j-1]-tab_kont[i][j])+abs(tab_kont[i-1][j]-tab_kont[i][j]);
	  else if(i==*wymy-1&&j!=*wymx-1)
	    obraz_pgm[i][j]=abs(tab_kont[i][j+1]-tab_kont[i][j])+abs(tab_kont[i-1][j]-tab_kont[i][j]);
	  else if(i!=*wymy-1&&j==*wymx-1)
	    obraz_pgm[i][j]=abs(tab_kont[i][j-1]-tab_kont[i][j])+abs(tab_kont[i+1][j]-tab_kont[i][j]);
	  else 
	    obraz_pgm[i][j]=abs(tab_kont[i][j+1]-tab_kont[i][j])+abs(tab_kont[i+1][j]-tab_kont[i][j]);
	}
    }
  *szarosci*=2; /* zwiekszenie skali szarosci, aby zmiescic powstale odcienie */
  strcat(modyfikacje,"konturowanie; "); /* kontynuacja komentarza modyfikacji */
  printf("Wykonano modyfikacje: Konturowanie.\n");

  return *wymx**wymy;
}

/************************************************************************************
 * Funkcja zapisuje obraz PGM z tablicy do pliku                                    *
 *                                                                                  *
 * \param[in] obraz_pgm tablica, w ktorej obraz jest zapisany                       *
 * \param[in] wymx szerokosc obrazka                                                *
 * \param[in] wymy wysokosc obrazka                                                 *
 * \param[in] szarosci liczba odcieni szarosci                                      *
 * \param[in] zmiana informacja czy dokonano modyfikacji na obrazie                 *
 * \param[in] nazwa_pliku nazwa oryginalnego pliku                                  *
 * \param[in] modyfikacje zmiany wykonane na obrazie w tablicy przed zapisem        *
 * \param[out] wymx szerokosc obrazka                                               *
 * \param[out] wymy wysokosc obrazka                                                *
 * \param[out] szarosci liczba odcieni szarosci                                     *
 * \param[out] nazwa_pliku nazwa oryginalnego pliku                                 *
 * \param[out] nowy_plik nazwa pliku, w ktorym obraz zostanie zapisany              *
 * \param[out] modyfikacje zmiany wykonane na obrazie w tablicy                     *
 * \return liczba pikseli obrazu                                                    *
 ************************************************************************************/

int zapisz(int obraz_pgm[][MAX], int *wymx, int *wymy, int *szarosci, int *zmiana, char nazwa_pliku[100], char nowy_plik[100], char modyfikacje[100])
{
  char nazwa[100]; 
  FILE *nowy;
  int i, j, element = 0; /* elelment - numer komorki tabeli */
  
  printf("Uwaga! Jezeli podasz nazwe pliku, ktory juz istnieje, to zostanie on zastapiony.\n");
  printf("Podaj nazwe nowego pliku bez rozszerzenia .pgm:\n");
  scanf("%s", nazwa);
  strcpy(nowy_plik,nazwa);   /* dopisanie rozszerzenia .pgm do nazwy pliku */
  strcat(nowy_plik,".pgm");
  
  nowy=fopen(nowy_plik,"w"); /* plik zostaje zastapiony, jezeli istnieje lub stworzony, jezeli nie istnieje */
  
  /* zapisanie komentarzy i informacji odpowiednich dla formatu pliku */
  if(*zmiana==0)
    strcat(modyfikacje,"brak");
  fprintf(nowy,"P2\n# Nazwa oryginalnego pliku: %s\n# %s\n",nazwa_pliku,modyfikacje);
  fprintf(nowy,"# Nazwa tego pliku: %s\n%d %d\n%d\n", nowy_plik,*wymx,*wymy,*szarosci);
  
  for(i=0;i<*wymy;i++)
    { 
      for(j=0;j<*wymx;j++)
	{
	  fprintf(nowy,"%4d", obraz_pgm[i][j]);  /* zapisywanie obrazu */
	  element++;
	  if(element%17==0)
	    fprintf(nowy,"\n"); /* koniec linii - ograniczenie 70 znakow w linii pliku*/
	}
    }
  fprintf(nowy,"\n"); /* znak konczacy plik - potrzebny do poprawnego otwarcia pliku przez program display */
  fclose(nowy);
  if(*zmiana==0)
    strcpy(modyfikacje,"Wykonane modyfikacje: "); /* Poczatek komentarza modyfikacji */
  printf("Obraz zostal zapisany pomyslnie.\n");
  return *wymx**wymy;
}

/************************************************************************************
 * Funkcja wyswietla obraz PGM z pliku za pomoca programu "display                  *
 *                                                                                  * 
 * \param[in] n_pliku nazwa pliku, z ktorego zostanie wyswietlony obraz             *
 ************************************************************************************/

void wyswietl(char *n_pliku) {
  char polecenie[DL_LINII];      /* bufor pomocniczy do zestawienia polecenia */
  
  strcpy(polecenie,"display ");  /* konstrukcja polecenia postaci */
  strcat(polecenie,n_pliku);     /* display "nazwa_pliku" &       */
  strcat(polecenie," &");
  printf("%s\n",polecenie);      /* wydruk kontrolny polecenia */
  system(polecenie);             /* wykonanie polecenia        */
}




int main()
{
  int obraz[MAX][MAX], tablica_konturowania[MAX][MAX];
  int wymx, wymy, odcieni;
  
  int odczytano = 0, zapisano = 0, i = 0;
  int wybor = 0, zmiana = 0;  /* zmiana - zmiana obrazu przez jego modyfikacje */
  FILE *plik;
  char nazwa[100], nowy_plik[100], blad[100], modyfikacje[100], nazwa_pliku[100];
  
  while(wybor != STOP)
    {
      printf("Wybierz operacje:\n");
      printf("   1 - Odczytaj obraz\n");
      printf("   2 - Modyfikuj - Negatyw\n");
      printf("   3 - Modyfikuj - Progowanie\n");
      printf("   4 - Modyfikuj - Konturowanie\n");
      printf("   5 - Zapisz obraz do pliku pod nowa nazwa\n");
      printf("   6 - Wyswietl obraz przed zmianami\n");
      printf("   7 - Wyswietl ostatnio zapisany obraz\n");
      printf("   %d - Zakoncz dzialanie programu\n", STOP);
      printf("Twoj wybor: ");
      if(scanf("%d", &wybor)==0)
	{
	  printf("To nie jest liczba! Wprowadz liczbe.\n");
	  scanf("%s", blad);
	}
      else
	{
	  switch(wybor)
	    {
	    case 1:
	      /* Wczytanie zawartosci wskazanego pliku do pamieci */
	      printf("Odczytywanie obrazu.\n");
	      printf("Odczytywany obraz nie moze przekraczac wymiarow 512x512 pikseli.\n");
	      printf("Podaj nazwe pliku bez rozszerzenia .pgm:\n");
	      scanf("%s",nazwa);
	      strcpy(nazwa_pliku,nazwa);
	      strcat(nazwa_pliku,".pgm"); /* dopisanie rozszerzenia .pgm */
	      
	      plik=fopen(nazwa_pliku,"r");
	      if (plik != NULL)
		{ 
		  odczytano = czytaj(plik, obraz, &wymx, &wymy, &odcieni);
		  fclose(plik);
		  strcpy(modyfikacje,"Wykonane modyfikacje: "); /* Poczatek komentarza modyfikacji */
		  zmiana = 0; /* Wczytany obraz nie przechodzil modyfikacji wzgledem pliku, z ktorego zostal pobrany */
		}
	      else
		fprintf(stderr,"Blad: Nie podano uchwytu do pliku.\n");
	      break;
	    case 2:  /* Modyfikacja: Negatyw */
	      if(odczytano!=0) /* czy plik zostal odczytany i obraz jest w pamieci */
		zmiana = negatyw(obraz, &wymx, &wymy, &odcieni, modyfikacje);
	      else
		printf("Brak obrazu w pamieci. Najpierw odczytaj obraz.\n");
	      break;
	    case 3:  /* Modyfikacja: Progowanie */
	      if(odczytano!=0)  
		zmiana = progowanie(obraz, &wymx, &wymy, &odcieni, modyfikacje);
	      else
		printf("Brak obrazu w pamieci. Najpierw odczytaj obraz.\n");
	      break;
	    case 4:  /* Modyfikacja: Konturowanie */
	      if(odczytano!=0)  
		zmiana = konturowanie(obraz, &wymx, &wymy, &odcieni, modyfikacje, tablica_konturowania);
	      else
		printf("Brak obrazu w pamieci. Najpierw odczytaj obraz.\n");
	      break;
	    case 5:  /* Zapis obrazu do nowego pliku */
	      if(odczytano!=0) 
		zapisano = zapisz(obraz, &wymx, &wymy, &odcieni, &zmiana, nazwa_pliku, nowy_plik, modyfikacje);
	      else
		printf("Brak obrazu w pamieci. Najpierw odczytaj obraz.\n");
	      break;
	    case 6:       /* Wyswietlenie poprawnie wczytanego obrazu zewnetrznym programem */
	      if(odczytano!=0)
		wyswietl(nazwa_pliku);
	      else
		printf("Brak obrazu w pamieci. Najpierw odczytaj obraz.\n");
	      break;
	    case 7:       /* Wyswietlenie zapisanego obrazu */
	      if(zapisano!=0) /* czy plik zostal zapisany */
		wyswietl(nowy_plik);	
	      else
		printf("Brak zapisanego obrazu. Najpierw zapisz obraz.\n");
	      break;
	    case STOP:
	      printf("Koniec dzialania programu.\n");
	      break;
	    default:
	      printf("Nie ma takiej pozycji. Wybierz jeszcze raz.\n");
	      break;
	    }
	}
    }
  return odczytano;
}
