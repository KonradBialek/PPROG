#include "stos.h"
/* z struktura opisujaca element listy (stosu) i dokumentacja funkcji stosu */

int inicjuj(elem_listy **stos)
{
  *stos=NULL; /* zainicjowanie pustej listy (stosu) */
  if(empty(*stos)==0) /* czy poprawnie zainicjowano */
    return 0;
  else
    return BLAD;
}


int empty(elem_listy *stos)
{
  if(stos==NULL) /* sprawdzenie, czy stos jest pusty */
    return 0;
  else
    return 1;
}


void print(elem_listy *stos)
{
  if(empty(stos)==0) /* gdy stos jest pusty */
    fprintf(stderr,"Stos jest pusty.\n");
  /* to nie jest asercja, tylko informacja o zawartosci stosu */
  while (stos!=NULL) 
    {
      /* dopoki stos nie jest pusty wyswietlane sa jego kolejne elementy */
      printf("%d\n", stos->wartosc);
      stos = stos->nastepny;
    }
} 


int pop(elem_listy **stos, int *arg)
{
  elem_listy *szczyt;
  szczyt=NULL;
  if(empty(*stos)==0) /* gdy stos jest pusty */
    {
      fprintf(stderr,"Blad asercji: funkcja pop.\n");
      fprintf(stderr,"Nie da się zdjac elementu z pustego stosu.\n");
      return BLAD;
    }
  else
    {
      szczyt=*stos; 
      /* szczyt wskazuje na pierwszy element stosu */
      *arg=szczyt->wartosc; 
      /* liczba z pierwszego elementu zostaje pobrana jako wartosc zmiennej */
      /* *arg                                                               */
      *stos=szczyt->nastepny; 
      /* *stos wskazuje na kolejny (drugi) element */
      free(szczyt); 
      /* zwolnienie pamieci elementu szczyt stosu */
      szczyt=NULL;
      return 0;
    }
}


int push(elem_listy **stos, int arg)
{
  elem_listy *nowy;
  nowy=(elem_listy *)malloc(sizeof(elem_listy));
  if(nowy==NULL) /* czy poprawnie zaalokowano pamiec */
    {
      fprintf(stderr,"Blad asercji: funkcja push.\n");
      fprintf(stderr,"Nie udalo się zalokowac potrzebnej pamieci.\n");
      return BLAD;
    }
  else
    {
      nowy->wartosc=arg; 
      /* przypisanie do wartosci w strukturze podanej liczby */
      nowy->nastepny=*stos; 
      /* wskazanie "zerowego" elementu na pierwszy element listy (stosu) */
      *stos=nowy; 
      /* zrobienie "zerowego" elementu pierwszym */
      nowy=NULL;
      return 0;
    }
}
