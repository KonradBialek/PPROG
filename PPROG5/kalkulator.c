/* Konrad Bialek */
/* Numer albumu: 248993 */
/* Data wykonania: 03.02.2019r. */

/* Zgodnie z Formularzem oceny kalkulatora RPN do pozycji Sprawozdanie  */ 
/* dolaczylem pusty plik, a wlasciwe sprawozdanie do tego programu jest */
/* jednym z plikow archiwum tego programu.                              */

/* Kalkulator RPN */

#include "stos.h"  
/* z struktura opisujaca element listy (stosu) i dokumentacja funkcji stosu */

#define KONIEC 'q' /* zakonczenie programu */

/************************************************************************/
/* Funkcja wykonuje operacje okreslona przez zmienna dane na podanych   */
/* liczbach                                                             */
/* Argumenty funkcji:                                                   */
/*         dane -  zmienna okreslajaca wykonywana operacje              */
/*         arg1 -  pierwszy argument operatora (pobrany jako drugi)     */
/*         arg2 -  drugi argument operatora (pobrany jako pierwszy)     */
/* PRE:                                                                 */
/*      istnieja co najmniej dwa elementy stosu, czyli istnieja co      */
/*      najmniej dwa argumenty, na ktorych mozna wykonywac operacje     */
/* POST:                                                                */
/*      wyznacza wynik wywolanej operacji; zwraca wartosc tego wyniku   */
/************************************************************************/
int operacja(int dane, int arg1, int arg2)
{
  int liczba; /* wynik operacji */
  
  switch(dane)
    {         /* wykonanie                      */
    case '+': /* dodawania                      */
      liczba=arg1+arg2;
      break;
    case '-': /* odejmowania                    */
      liczba=arg1-arg2;
      break;
    case '*': /* mnozenia                       */
      liczba=arg1*arg2;
      break;
    case '/': /* dzielenia calkowitoliczbowego  */
      if(arg2==0)
	{
	  fprintf(stderr,"Nie mozna dzielic przez 0.\n");
	  exit(-2);
	}
      liczba=arg1/arg2;
      break;
    }
  return liczba; /* zwroc wynik operacji */
}

/************************************************************************/
/* Funkcja duplikuje argument znajdujacy sie na szczycie stosu          */
/* Argumenty funkcji:                                                   */
/*         stos  -  pierwszy element stosu                              */
/*         arg2  -  argument operacji (pobrany ze stosu)                */
/* PRE:                                                                 */
/*      istnieje co najmniej jeden element stosu, czyli istnieje co     */
/*      najmniej jeden argument, ktory mozna zduplikowac                */
/* POST:                                                                */
/*      duplikuje pobrany ze szczytu stosu argument i wprowadza go na   */
/*      stos; zwraca wartosc 0, gdy zadzialala poprawnie lub wartosc    */
/*      BLAD, gdy nie ma wystarczajacej ilosci argumentow do duplikacji */
/************************************************************************/
int duplikuj(elem_listy *stos, int *arg2)
{
  if(empty(stos)==0) /* gdy stos jest pusty */
    {
      fprintf(stderr,"Blad asercji: funkcja duplikuj.\n");
      fprintf(stderr,"Do operacji duplikacji potrzebny jest 1 argument.\n");
      fprintf(stderr,"Na stosie jest 0 argumentow.\n");
      return BLAD;
    }
  else 
    {
      pop(&stos,arg2); /* pobranie liczby ze stosu */
      push(&stos,*arg2); /* i wprowadzenie do stosu 2 razy */
      push(&stos->nastepny,*arg2);
      return 0;      
    }
}

/************************************************************************/
/* Funkcja zamienia miejscami argumenty znajdujace sie na szczycie stosu*/
/* Argumenty funkcji:                                                   */
/*         stos  -  pierwszy element stosu                              */
/*         arg1  -  pierwszy argument operacji (pobrany jako drugi)     */
/*         arg2  -  drugi argument operacji (pobrany jako pierwszy)     */
/* PRE:                                                                 */
/*      istnieja co najmniej dwa elementy stosu, czyli istnieja co      */
/*      najmniej dwa argumenty, na ktorych mozna wykonac zamiane        */
/* POST:                                                                */
/*      wprowadza pobrane argumenty z powrotem do stosu, ale w odwrotnej*/
/*      kolejnosci; zwraca wartosc 0, gdy zadzialala poprawnie lub      */
/*      wartosc BLAD, gdy nie ma wystarczajacej ilosci argumentow do    */
/*      do zamiany                                                      */
/************************************************************************/
int zamiana(elem_listy *stos, int *arg1, int *arg2)
{
  if(empty(stos)==0||empty(stos->nastepny)==0)
    {
      /* gdy w stosie jest niewystarczajaca ilosc argumentow */
      fprintf(stderr,"Blad asercji: funkcja zamiana.\n");
      fprintf(stderr,"Do operacji zamiany argumentow potrzebne sa 2 argumenty.\n");
      fprintf(stderr,"Na stosie jest %s.\n",stos!=NULL?"1 argument":"0 argumentow");
      return BLAD;
    }
  else
    {
      pop(&stos,arg2); /* pobranie dwoch liczb ze stosu */
      pop(&stos,arg1); 
      push(&stos,*arg2); /* i wprowadzenie do stosu w odwrotnej kolejnosci */
      push(&stos,*arg1);
      return 0;
    }
}

/************************************************************************/
/* Funkcja drukuje na wyjsciu argument znajdujacy sie na szczycie stosu */
/* Argumenty funkcji:                                                   */
/*         stos -  pierwszy element stosu                               */
/* PRE:                                                                 */
/*      istnieje co najmniej jeden element stosu, czyli istnieje co     */
/*      najmniej jeden argument, ktory mozna wydrukowac na wyjsciu      */
/* POST:                                                                */
/*      drukuje na wyjsciu argument znajdujacy sie na szczycie stosu;   */
/*      zwraca wartosc 0, gdy zadzialala poprawnie lub wartosc BLAD,    */
/*      gdy stos jest pusty                                             */
/************************************************************************/
int drukuj_szczyt(elem_listy *stos)
{
  if(empty(stos)==0) /* gdy stos jest pusty */
    {
      fprintf(stderr,"Blad asercji: funkcja drukuj_szczyt.\n");
      fprintf(stderr,"Do operacji drukowania szczytu potrzebny jest 1 argument.\n");
      fprintf(stderr,"Na stosie jest 0 argumentow.\n");
      return BLAD;
    }
  else /* wyswietlenie elementu znajdujacego sie na szczycie stosu */
    { 
      printf("%d\n", stos->wartosc);
      return 0;
    }
}

/************************************************************************/
/* Funkcja usuwa zawartosc stosu                                        */
/* Argumenty funkcji:                                                   */
/*         stos  -  pierwszy element stosu                              */
/*         arg2  -  argument operacji (pobrany ze stosu)                */
/* PRE:                                                                 */
/*      brak (jezeli stos jest pusty przy wywolaniu programu petla nie  */
/*      wykona sie ani razu i program przejdzie do funkcji glownej)     */
/* POST:                                                                */
/*      pobiera wartosci kolejnych elementow stosu i zwalnia ich pamiec */
/*      dopoki w stosie znajduja sie elementy; zwraca wartosc 0         */
/************************************************************************/
int wyczysc(elem_listy **stos,int *arg2)
{
  while(*stos!= NULL) /* pobieranie liczb ze stosu */
    pop(stos, arg2);  /* dopoki stos nie bedzie pusty */
  return 0;
}

/************************************************************************/
/* Funkcja glowna zarzadza kalkulatorem RPN                             */
/* Argumenty funkcji:                                                   */
/*         brak                                                         */
/* PRE:                                                                 */
/*      brak                                                            */
/* POST:                                                                */
/*      wywoluje inicjalizacje stosu (listy), wywoluje funkcje dotyczace*/
/*      kalkulatora wedlug podawanych znakow, wprowadza dane do stosu   */
/*      za pomoca funkcji push, wywoluje czyszczenie stosu przed        */
/*      zakonczeniem programu lub wyswietla odpowiedni komunikat o      */
/*      bledzie; zwraca wartosc 0, gdy zadziala poprawnie, lub wartosc  */
/*      BLAD, gdy stos nie zostanie porawnie zainicjowany               */
/************************************************************************/
int main(){
  elem_listy *stos;
  int arg1, arg2, liczba, dane;
  /* argumenty dzialan, wynik dzialan, zmienna sluzaca do okreslenia typu  */
  /* danych wejsciowych i wykonwanych operacji                             */
  
  /* zainicjowanie pustego stosu i sprawdzenie poprawnosci zainicjowania   */
  if(inicjuj(&stos)!=0)
    {
      fprintf(stderr,"Blad: funkcja inicjuj.\n");
      fprintf(stderr,"Stos nie zostal poprawnie zainicjowany.\n");
      return BLAD;
    }
  
  while (dane!=KONIEC) /* dopoki nie wywolano zakonczenia programu */
    {
      dane=fgetc(stdin); /* wczytywanie znaku danych */
      switch(dane)
        {
	case 'f': /* wyswietlanie calej pamieci kalkulatora */
	  print(stos);
	  break;
	case 'P': /* usuniecie elementu znajdujacego sie na szczycie stosu - */
		  /* bez wyswietlenia tego elementu                          */
	          /* ta operacja nie potrzebuje osobnej funkcji              */
	  if(pop(&stos, &arg2)==BLAD)
	    fprintf(stderr,"Operacja \"P\": Nie mozna usunac nieistniejacego elememtu.\n");
	  break;
	case 'c': /*wyczysc pamiec kalkulatora */
	  wyczysc(&stos,&arg2);
	  break;
	case 'p': /* wyswietl liczbe znajdujaca sie na szczycie stosu */
	  drukuj_szczyt(stos);
	  break;
	case 'r':
	  /* zamien miejscami dwa argumenty znajdujace sie na szczycie stosu */
	  zamiana(stos,&arg1,&arg2);
	  break;
	case 'd': /* duplikuj argument znajdujacy sie na szczycie stosu */
	  duplikuj(stos,&arg2); 
	  break;
	case '+': /* wykonaj odpwiednie dzialanie arytmetyczne */
	case '-':
	case '*':
	case '/':
	  if(empty(stos)==0||empty(stos->nastepny)==0)/* gdy stos jest pusty */
	    {
	      /* gdy w stosie jest niewystarczajaca ilosc argumentow */
	      fprintf(stderr,"Blad asercji: funkcja operacja.\nDo operacji ");
	      switch(dane)
		{
                case '+':
	          fprintf(stderr,"dodawania ");
	          break;
                case '-':
	          fprintf(stderr,"odejmowania ");
	          break;
                case '*':
	          fprintf(stderr,"mnozenia ");
	          break;
                case '/':
	          fprintf(stderr,"dzielenia ");
	          break;
		}
	      fprintf(stderr,"potrzebne sa dwa argumenty.\n");
	      fprintf(stderr,"Na stosie jest ");
	      if(empty(stos)==0)
		fprintf(stderr,"0 argumentow.\n");
	      else if (empty(stos->nastepny)==0)
		fprintf(stderr,"1 argument.\n"); 
	    }
	  else /* gdy wszystko jest w porzadku */
	    {        
	      pop(&stos,&arg2); /* pobranie dwoch argumentow */
	      pop(&stos,&arg1); /* i wykonanie operacji      */
	      liczba=operacja(dane, arg1, arg2);
	      push(&stos,liczba);
	    }
	  break;
	case '0': /* gdy wprowadzono liczbe,    */
	case '1': /* czyli program pobral cyfre */
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	  ungetc(dane,stdin); /* zwroc pobrana cyfre */
	  scanf("%d", &liczba);   /* odczytaj liczbe */
	  push(&stos,liczba); /* wprowadz ta liczbe na stos */
	  break;
	default: /* Nieznany znak nie spowoduje bledu w programie, */
	  break; /* wiec zostanie zignorowany                      */
	}                   
    }
  wyczysc(&stos,&arg2);
  /* konieczne czyszczenie stosu przed zakonczeniem programu */
  return 0;
}
