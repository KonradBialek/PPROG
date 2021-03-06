/* Konrad Bialek */
/* Numer albumu: 248993 */
/* Data wykonania: 28.01.2019r. */

/* Program obslugujacy czarno-biale i kolorowe obrazy zapisane w plikach w formacie PGM lub PPM */

/* Wykorzystane w programie kody bledow, struktura w_opcje, procedura wyzeruj_opcje i funkcja */
/* przetwarzaj_opcje zostaly opracowane na podstawie pliku opcje.c dostepnego na stronie      */
/* kcir.pwr.edu.pl/~mucha/PProg/PProg_lab_05/opcje.c [dostep: 19.01.2019r.]                   */

#include "modul.h"     /* z struktura opisujaca obraz i dokumentacja funkcji */

#define W_OK 0                   /* wartosc oznaczajaca brak bledow */
#define B_NIEPOPRAWNAOPCJA -1    /* kody bledow rozpoznawania opcji */
#define B_BRAKWARTOSCI  -2
#define B_BRAKPLIKU   -3
#define B_BRAKNAZWY_WEJ -4
#define B_BRAKNAZWY_WYJ -5


/*******************************************************/
/* Funkcja inicjuje strukture wskazywana przez wybor   */
/* PRE:                                                */
/*      poprawnie zainicjowany argument wybor (!=NULL) */
/* POST:                                               */
/*      "wyzerowana" struktura wybor wybranych opcji   */
/*******************************************************/

void wyzeruj_opcje(w_opcje * wybor) {
  wybor->plik_we=NULL;
  wybor->plik_wy=NULL;
  wybor->negatyw=0;
  wybor->konturowanie=0;
  wybor->progowanie=0;
  wybor->wyswietlenie=0;
  wybor->odczytanie=0;
  wybor->kolor=0;
}

/************************************************************************/
/* Funkcja rozpoznaje opcje wywolania programu zapisane w tablicy argv  */
/* i zapisuje je w strukturze wybor; zbiera tez informacje o nazwie     */
/* oryginalnego pliku, pliku zapisu i czy recznie wywolano konwersje    */
/* Skladnia opcji wywolania programu                                    */
/*         program {[-i nazwa] [-o nazwa] [-p liczba] [-n] [-k] [-d]    */
/*         [-m]}                                                        */
/* Argumenty funkcji:                                                   */
/*         argc    -  liczba argumentow wywolania wraz z nazwa programu */
/*         argv    -  tablica argumentow wywolania                      */
/*         wybor   -  struktura z informacjami o wywolanych opcjach     */
/*         obrazek -  struktura z informacjami o obrazie                */
/* PRE:                                                                 */
/*      brak                                                            */
/* POST:                                                                */
/*      funkcja otwiera odpowiednie pliki, zwraca uchwyty do nich       */
/*      w strukturze wybor, do tego ustawia na 1 pola, ktore            */
/*	poprawnie wystapily w linii wywolania programu,                 */
/*	pola opcji nie wystepujacych w wywolaniu ustawione sa na 0;     */
/*	zwraca wartosc W_OK, gdy wywolanie bylo poprawne                */
/*	lub kod bledu zdefiniowany stalymi B_* (<0)                     */
/*      pobrane informacje zwiazane z obrazem sa zapisywane do          */
/*      struktury obrazek                                               */
/* UWAGA:                                                               */
/*      funkcja nie sprawdza istnienia i praw dostepu do plikow         */
/*      w takich przypadkach zwracane uchwyty maja wartosc NULL         */
/************************************************************************/

int przetwarzaj_opcje(int argc, char *argv[], w_opcje *wybor, t_obraz *obrazek) {
  int i, prog;
  
  wyzeruj_opcje(wybor);
  wybor->plik_wy=stdout;        /* na wypadek gdy nie podano opcji "-o" */
  obrazek->nowy_plik="stdout";
  for (i=1; i<argc; i++) {
    if (argv[i][0] != '-')  /* blad: to nie jest opcja - brak znaku "-" */
      return B_NIEPOPRAWNAOPCJA; 
    switch (argv[i][1]) {
    case 'i': {                 /* opcja z nazwa pliku wejsciowego */
      wybor->odczytanie=1;
      if (++i<argc) {   /* wczytujemy kolejny argument jako nazwe pliku */
	obrazek->nazwa=argv[i];
	if (strcmp(obrazek->nazwa,"-")==0) /* gdy nazwa jest "-"        */
	  wybor->plik_we=stdin;            /* ustwiamy wejscie na stdin */
	else                               /* otwieramy wskazany plik   */
	  wybor->plik_we=fopen(obrazek->nazwa,"r");
      } else 
	return B_BRAKNAZWY_WEJ;                   /* blad: brak nazwy pliku */
      break;
    }
    case 'o': {                 /* opcja z nazwa pliku wyjsciowego */
      if (++i<argc) {   /* wczytujemy kolejny argument jako nazwe pliku */
	obrazek->nowy_plik=argv[i];
	if (strcmp(obrazek->nowy_plik,"-")==0)/* gdy nazwa jest "-"         */
	  {
	    wybor->plik_wy=stdout;             /* ustwiamy wyjscie na stdout */
	    obrazek->nowy_plik="stdout";
	  }
	else                                  /* otwieramy wskazany plik    */
	  wybor->plik_wy=fopen(obrazek->nowy_plik,"w");
      } else 
	return B_BRAKNAZWY_WYJ;                   /* blad: brak nazwy pliku */
      
      break;
    }
    case 'p': {
      if (++i<argc) { /* wczytujemy kolejny argument jako wartosc progu */
	if (sscanf(argv[i],"%d",&prog)==1) {
	  wybor->progowanie=1;
	  wybor->w_progu=prog;
	} else
	  return B_BRAKWARTOSCI;     /* blad: niepoprawna wartosc progu */
      } else 
	return B_BRAKWARTOSCI;             /* blad: brak wartosci progu */
      break;
    }
    case 'n': {                 /* mamy wykonac negatyw */
      wybor->negatyw=1;
      break;
    }
    case 'k': {                 /* mamy wykonac konturowanie */
      wybor->konturowanie=1;
      break;
    }
    case 'm': {
      wybor->kolor=1;
      break;
    }
    case 'd': {                 /* mamy wyswietlic obraz */
      wybor->wyswietlenie=1;
      break;
    }
    default:                    /* nierozpoznana opcja */
      return B_NIEPOPRAWNAOPCJA;
    } /*koniec switch */
  } /* koniec for */

  if (wybor->plik_we!=NULL)     /* ok: wej. strumien danych zainicjowany */
    return W_OK;
  else 
    return B_BRAKPLIKU;         /* blad:  nie otwarto pliku wejsciowego  */
}

/************************************************************************************/
/* Procedura wyswietla obraz z pliku za pomoca programu "display"                   */
/* Argumenty funkcji:                                                               */
/*         n_pliku  -   nazwa pliku, z ktorego zostanie wyswietlony obraz           */
/*         zapisano -   informacja czy zapisano obraz i jego parametry do pliku     */
/* PRE:                                                                             */
/*      poprawnie zapisano obraz do nowego pliku i n_pliku musi zawierac nazwe      */
/*      pliku z odpowiednim rozszerzeniem, ktory znajduje sie w biezacym katalogu   */
/* POST:                                                                            */
/*      wyswietla obraz                                                             */
/************************************************************************************/

void wyswietl(char *n_pliku, int *zapisano) {
  char polecenie[DL_LINII];    /* bufor pomocniczy do zestawienia polecenia */
  
  if(*zapisano==0||*zapisano==-1) /* czy plik zostal zapisany */	
    {
      fprintf(stderr,"Blad asercji: procedura wyswietl.\n");  
      fprintf(stderr,"Brak zapisanego obrazu. Najpierw zapisz obraz.\n"); 
      return;
    }
  /* Funkcja zapisz tworzy plik pod podana i ta nazwa (wskazujaca na plik   */
  /* z odpowiednim rozszerzeniem zapisany w biezacym katalogu) zostanie     */
  /* wyslana do funkcji wyswietl (po jej wywolaniu), dlatego zmienna        */
  /* *n_pliku jest poprawna                                                 */
  strcpy(polecenie,"display ");  /* konstrukcja polecenia postaci */
  strcat(polecenie,n_pliku);     /* display "nazwa_pliku" &       */
  strcat(polecenie," &");
  printf("%s\n",polecenie);      /* wydruk kontrolny polecenia */
  system(polecenie);             /* wykonanie polecenia        */
}

/************************************************************************************/
/* Funkcja glowna zarzadza przetwarzaniem obrazu                                    */
/* Argumenty funkcji:                                                               */
/*         argc  -  liczba argumentow wywolania wraz z nazwa programu               */
/*         argv  -  tablica argumentow wywolania                                    */
/* PRE:                                                                             */
/*      opcje wywolania musza zaczynac sie znakiem '-'; wywolanie programu musi     */
/*      zawierac opcje "-i"; w wywolaniu programu powinny zostac odpowiednio        */
/*      okreslone: nazwa pliku wejsciowego z obrazem (za pomoca opcji "-i" nazwa),  */
/*      nazwa pliku wyjsciowego (za pomoca opcji "-o" nazwa) gdy podano opcje "-o", */
/*      wartosc progu funkcji progowania (za pomoca opcji "-p" prog) gdy podano     */
/*      opcje "-p"; nazwa pliku wejsciowego musi wskazywac na plik znajdujacy sie w */
/*      biezacym katalogu; opcja "-m" moze zostac wywolana tylko dla kolorowego     */
/*      obrazu, nie mozna wyswietlic niezapisanego lub zapisanego na stdout obrazu  */
/* POST:                                                                            */
/*      wywoluje funkcje dotyczace obrazu wedlug argumentow wywolania lub           */
/*      wyswietla odpowiedni komunikat o bledzie; zwraca wartosc 0                  */
/************************************************************************************/

int main(int argc, char * argv[])
{
  t_obraz *obrazek;
  w_opcje opcje;
  int odczytano = 0, zapisano = 0, blad;

  obrazek = (t_obraz *) malloc(sizeof(t_obraz));
  blad=przetwarzaj_opcje(argc,argv,&opcje, obrazek);
  /* przetwarzanie argumentow wywolania */
  if(blad<0||opcje.odczytanie==0)   
    { /*bledy funkcji przetwarzaj_opcje */
      fprintf(stderr,"Blad asercji: wywolanie programu - funkcja przetwarzaj_opcje.\n");
      if(blad==B_BRAKNAZWY_WEJ)
        fprintf(stderr,"Brak nazwy pliku wejsciowego z obrazem.\n");
      if(blad==B_BRAKNAZWY_WYJ)
        fprintf(stderr,"Brak nazwy pliku wyjsciowego.\n");
      if(blad==B_NIEPOPRAWNAOPCJA)
        fprintf(stderr,"To nie jest opcja.\n");
      if(blad==B_BRAKWARTOSCI)
        fprintf(stderr,"Brak wartosci progu.\n");
      if(blad==B_BRAKPLIKU&&opcje.odczytanie!=0)
        fprintf(stderr,"Nie podano uchwytu do pliku.\n");
      if(opcje.odczytanie==0)
        fprintf(stderr,"Nie podano opcji \"-i\".\n");
      return 0;
    }

  if(opcje.plik_we!=NULL)
    {
      odczytano = czytaj(opcje.plik_we, obrazek);
      fclose(opcje.plik_we);
      strcpy(obrazek->modyfikacje,"Wykonane modyfikacje: ");
      /* Poczatek komentarza modyfikacji */
    } /* schemat wywolania programu przez potoki uniemozliwia zapamietanie modyfikacji  */
     /* z wszystkich potokow, zapamietuje modyfikacje tylko z jednego potoku */
  else /* gdy nie podano uchwytu pliku lub podany uchwyt nie wskazuje na istniejacy plik */
    return 0;
  
  if(obrazek->typ==1&&opcje.kolor==1)
    { /* po wywolaniu "-m" dla obrazu PGM zostaje to pominiete */
      fprintf(stderr,"Blad asercji: wywlanie programu.\n");
      fprintf(stderr,"Konwersje mozna wywolac tylko dla kolorowych obrazow.\n");
      fprintf(stderr,"Podczas przetwarzania pominieto konwersje.\n");
    }
  
  if((opcje.kolor==1||opcje.negatyw==1||opcje.progowanie==1||opcje.konturowanie==1)&&obrazek->typ==3)
    konwersja(obrazek, &odczytano); /* gdy obraz jest kolorowy i wywolano modyfikacje lub konwersje */
                                    /* dla modyfikacji konwersja jest automatyczna                  */
                                    /* wywolanie jednoczesnie konwersji i modyfikacji nie spowoduje */
                                    /* wadliwego dzialania programu                                 */
  /* gdy obraz jest czarno-bialy */
  if(opcje.negatyw==1&&obrazek->typ==1) 
    negatyw(obrazek, &odczytano);
  if(opcje.progowanie==1&&obrazek->typ==1) 
    progowanie(obrazek, &odczytano, opcje.w_progu);
  if(opcje.konturowanie==1&&obrazek->typ==1)
    konturowanie(obrazek, &odczytano);
  
  zapisano=zapisz(obrazek, &odczytano, opcje.plik_wy); 
  if(opcje.wyswietlenie==1&&strcmp(obrazek->nowy_plik,"stdout")!=0)
    wyswietl(obrazek->nowy_plik, &zapisano);
  if(opcje.wyswietlenie==1&&strcmp(obrazek->nowy_plik,"stdout")==0)
    {
      fprintf(stderr,"Blad asercji: wywolanie programu.\n");
      fprintf(stderr,"Nie mozna wyswietlic niezapisanego lub zapisanego na stdout obrazu.\n");
      return 0;
    }
  if(obrazek->piksele)
    free(obrazek->piksele);
  free(obrazek);
  
  return 0;
}
