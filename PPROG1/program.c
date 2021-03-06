/* Konrad Bialek */
/* Nr albumu: 248993 */
/* Data wykonania: 08.11.2018r. */

/* Program liczacy czestotliwosc przeciec zera we wczytywanym strumieniu 
   wejsciowym. */

#include <stdio.h>
#define WMIN -10 /* wartosc minimalna */
#define WMAX 10  /* wartosc maksymalna */
#define CMIN 8   /* czestotliwosc minimalna */
#define CMAX 14  /* czestotliwosc maksymalna */
#define GRUPA 99 /* liczba danych w pelnej grupie */
#define STOP 99  /* wartosc konca strumienia danych */

int main()
{
  float w1=0, w2=0; /* pobierane wartosci */
  int cz=0;         /* czestotliwosc przeciec zera */
  int numer=1;      /* numer wartosci z grupy sprawdzanej przez program */
  int grupa=0;      /* numer pelnej grupy wartosci */ 
  
  while(w2!=STOP)
    {
      scanf("%f", &w2);
      
      if(WMIN<=w2&&w2<=WMAX)  /* przedzial poprawnych wartosci */
	{
	  if(w1*w2<0) /* zwiekszenie czestotliwosci */
 	    cz++;

	  if(w2!=0) /* pomijanie zer */
	    w1=w2;
	}
      
      if(numer==GRUPA) /* pelna grupa wartosci */
	{
	  grupa++;
	  if(!(CMIN<=cz&&cz<=CMAX)) /* wymagany komunikat */
	    printf("Grupa %d. Nieprawidlowa czestotliwosc: %d\n", grupa, cz);
	  else /* dla poprawnych czestotliwosci */
	    {
	      printf("%d. %d\n", grupa, cz);
	    }
  	  
	  cz=0;      /* wyzerowanie wartosci czestotliwosci */
	  w1=0;      /* powrot do neutralnej wartosci w1 */
	  numer=0;   /* po instrukcji warunkowej numer zostanie zwiekszony */
	}
      numer++; /* zwiekszenie numeru dla nastepnej sprawdzanej wartosci */
      
    }
  
  return 0;
}
