/*******************************
 ** Autor: Vaclav Martinka    **
 **    ID: xmarti76           **
 ** Nazev: Proj2              **
 *******************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include <limits.h>

///Definice globalnich konstant
enum err_codes {E_OK, E_NUMOFARG, E_BADARG, E_NUM1, E_NUM2, E_NUM3, E_NUMOFITER, E_MINMAX};
#define MIN_DOUBLE -1e101
#define MAX_DOUBLE 1e101
#define MAX_INT UINT_MAX-1
#define MAX_ITERATIONS 1000000


///Prevod string na double
/*povolene tvary:
  +123,123  OK
  123.123   OK
  ,123      OK
  1e2       OK
  1,5e+2    OK   = 1*10^2
  0,1e-2    OK
*/
double string_to_double (char input[], bool *bad_number) {
  double output = 0.0;
  char *err = NULL;

  for (unsigned i = 0 ; input[i] != '\0' ; i ++) {    //prevedu , na .
    if ((input[i] == ',') || (input[i] == '.'))
      input[i] = '.';
  }

  output = strtod(input, &err);

  if (input == err || *err != '\0') {
    *bad_number = true;
    return 0.0;
  }

  return output;
}


///Rozdil mezi 2 cisly
double my_abs (double a, double b) {
  if (a > b)
    return (a - b);
  else
    return (b - a);
}


///Vypocet logaritmu pomoci Taylorova polynomu OK
double taylor_log (double x, unsigned int n) {
  if (x == 0) return -INFINITY;
  if (x <  0) return NAN;
  isnan(x)    return NAN;
  isinf(x)    return INFINITY;

  long double output;
  long double y;

  if (x <= 1) {   //Pocitam pomoci zlomku
    x = 1 - x;
    y = x;
    output = 0 - x;
    for (unsigned int i = 2 ; i <= n ; i ++) {
      y *= x;
      output = output - (y / i);
    }
  }
  else {          //Pocitam pomoci sumy
    y = (x - 1) / x;
    x = y;
    output = x;
    for (unsigned int i = 2 ; i <= n ; i ++) {
      y *= x;
      output = output + (y / i);
    }
  }

  return output;
}


///Vypocet logaritmu pomoci zretezeneho zlomku OK
double cfrac_log (double x, unsigned int n) {
  if (x == 0) return -INFINITY;
  if (x <  0) return NAN;
  isnan(x)    return NAN;
  isinf(x)    return INFINITY;

  long double z = (x - 1) / (x + 1);
  long double output = 2 * z;
              z *= z;
  long double y = (2 * n) - 1;

  for (; n > 1 ; n --)
    y = (2 * n) - 3 - ((n - 1) * (n - 1) / y * z);

  return (output / y);
}


///Vypocet poctu iteraci pro cf
int cf_iter (double ite_min, double ite_max, double ite_eps) {
  unsigned int i = 0;
  unsigned int counter = 1;
           int    step = 100;
  double     number = ite_min;
  double     result = cfrac_log(number, 1);
  double ref_result = log(number);
  bool second_round = false;

  while (1) {    //dokud neni odchylka mensi nez eps
    if (isinf(result) || isnan(result)) {
        if (second_round)
          return 0;
      second_round = true;
      number = ite_max;
      result = taylor_log(number, 1);
      ref_result = log(number);
      //printf("OK\n");
      continue;
    }

    i += step;
    if (counter != 0) counter ++;

    result = cfrac_log(number, i);
    //kazdych 100 vypoctu zvysim krok o 100
    if ((((counter) % 100) == 0) && counter != 0) step += 100;
    //po 100000 vypoctech ukoncim program
    if (counter > MAX_ITERATIONS) return -1;

    if (my_abs(ref_result, result) <= ite_eps) {    //pokud splnim podminku
      if (counter != 0) {     //a jeste jsem to neudelal
        i -= step;            //vratim se o step
        step = 1;             //a spustim vypocet znovu -> hledam presny pocet
        counter = 0;
        result = cfrac_log(number, i);
        continue;
      }
      if (!second_round) {
        number = ite_max;           //az najdu presny pocet iteraci
        ref_result = log(number);   //prepisu promenne a pocitam pro druhe cislo
        //pokud mi i staci -> mam vysledny pocet iteraci
        if (my_abs(ref_result, (result = cfrac_log(number, i))) <= ite_eps) return i;
        //jinak pokracuji dal
        second_round = true;
        counter = 1;
//      step = 100;
      }
      else
        return i;
    }
  }
}


///Vypocet poctu iteraci pro taylor (stejne jako cf_iter)
int taylor_iter (double ite_min, double ite_max, double ite_eps) {
  unsigned int i = 0;
  unsigned int counter = 1;
           int    step = 100;
  double     number = ite_min;
  double     result = taylor_log(number, 1);
  double ref_result = log(number);
  bool second_round = false;

  while (1) {    //dokud neni odchylka mensi nez eps
    if (isinf(result) || isnan(result)) {
        if (second_round)
          return 0;
      second_round = true;
      number = ite_max;
      result = taylor_log(number, 1);
      ref_result = log(number);
      continue;
    }

    i += step;
    if (counter != 0) counter ++;

    result = taylor_log(number, i);
    //kazdych 100 vypoctu zvysim krok o 100
    if ((((counter) % 100) == 0) && counter != 0) step += 100;
    //po 100000 vypoctech ukoncim program
    if (counter > MAX_ITERATIONS) return -1;

    if (my_abs(ref_result, result) <= ite_eps) {    //pokud splnim podminku
      if (counter != 0) {     //a jeste jsem to neudelal
        i -= step;            //vratim se o step
        step = 1;             //a spustim vypocet znovu -> hledam presny pocet
        counter = 0;
        result = taylor_log(number, i);
        continue;
      }
      if (!second_round) {
        number = ite_max;           //az najdu presny pocet iteraci
        ref_result = log(number);   //prepisu promenne a pocitam pro druhe cislo
        //pokud mi i staci -> mam vysledny pocet iteraci
        if (my_abs(ref_result, (result = taylor_log(number, i))) <= ite_eps) return i;
        //jinak pokracuji dal
        second_round = true;
        counter = 1;
//      step = 100;
      }
      else
        return i;
    }
  }
}


///Funkce pro chybova hlaseni
void print_error (int x) {
  switch (x) {
    case 0:
      return;
    case 1:
      printf("Byl zadan neplatny pocet argumentu!\n   Pouzijte --help pro napovedu.\n\n");
      break;
    case 2:
      printf("Byl zadan neplatny argument!\n   Pouzijte --help pro napovedu.\n\n");
      break;
    case 3:
      printf("Prvni cislo neni v platnem tvaru nebo rozsahu!\n   Pouzijte --help pro napovedu.\n\n");
      break;
    case 4:
      printf("Druhe cislo neni v platnem tvaru nebo rozsahu!\n   Pouzijte --help pro napovedu.\n\n");
      break;
    case 5:
      printf("Treti cislo neni v platnem tvaru nebo rozsahu!\n   Pouzijte --help pro napovedu.\n\n");
      break;
    case 6:
      printf("Pri vypoctu byl presazen maximalni pocet iteraci!\n   Pouzijte mensi rozsah nebo mensi EPS!\n\n");
      break;
    case 7:
      printf("Bylo zadano MIN vetsi nez MAX!\n\n");
      break;
    default:
      printf("Doslo k nespecifikovane chybe!\n   Pouzijte --help pro napovedu.\n\n");
  }
  return;
}


///Tisk help
void print_help (void) {
  printf(
    "Projekt 2\n"
    "Autor: V. Martinka (xmarti76)\n\n"
    "Program spocita prirozeny logaritmus pomoci dvou metod.\n\n\n"
    "  # Pokud je spusten s parametry --log X N\n"
    "    - Kde X je realne cislo v rozsahu %g - %g a udava z jakeho cisla chceme logaritmus pocitat\n"
    "    - A N je prirozene cislo mensi nez %ul znacici pocet iteraci pri vypoctu\n"
    "  # Pokud je spusten s parametry --iter MIN MAX EPS\n"
    "    - Spocita minimalni pocet iteraci pro cisla v rozsahu MIN - MAX tak, aby se vysledek techto\n"
    "      metod lisil maximalne o EPS oproti skutecnemu\n"
    "    - Cisla MIN, MAX a EPS mohou byt v rozsahu %g - %g\n"
    "    - Provadi se pouze %d vypoctu, proto je mozne, ze vypocet nebude dokoncen...\n\n"
    "  * Povolene tvary pro zapis cisla jsou nasledujici:\n"
    "    1,5\n"
    "    .5\n"
    "    2e3\n"
    "    0.3E+1\n"
    "    ,55e-5\n",
    MIN_DOUBLE, MAX_DOUBLE, MAX_INT, MIN_DOUBLE, MAX_DOUBLE, MAX_ITERATIONS
  );

  return;
}


///Kontrola a prevod cisel
bool number_control (char *argv[], double numbers[], int count) {
  bool bad_number = false;
  int i = 0;

  while (i < count) {
    numbers[i] = string_to_double(argv[i + 2], &bad_number);
    if (bad_number) break;
    i ++;
  }

  if (bad_number) {
    switch (i) {
      case 0:
        print_error(E_NUM1);
        break;
      case 1:
        print_error(E_NUM2);
        break;
      case 2:
        print_error(E_NUM3);
        break;
      default:
        print_error(-1);
    }
    return false;
  }

  return true;
}


///Kontrola a tisk vysledku logaritmu
void print_log (double log_x, unsigned int log_n) {
  //Zkontroluji spravnost rozsahu
  if (log_x < MIN_DOUBLE || log_x > MAX_DOUBLE) {
    print_error(E_NUM1);
    return;
  }
  if (log_n > (MAX_INT)) {
    print_error(E_NUM2);
    return;
  }

  //Tisk vysledku logaritmu
  printf(
    "       log(%g) = %.12g\n"
    "    cf_log(%g) = %.12g\n"
    "taylor_log(%g) = %.12g\n",
    log_x, log(log_x),
    log_x, cfrac_log(log_x, log_n),
    log_x, taylor_log(log_x, log_n)
  );

  return;
}


///Kontrola a tisk vysledku iteraci
void print_iter (double iter_min, double iter_max, double iter_eps) {
  int cf_iter_result;
  int taylor_iter_result;

  //Zkontroluji spravnost rozsahu
  if (iter_min < MIN_DOUBLE || iter_min > MAX_DOUBLE) {
    print_error(E_NUM1);
    return;
  }
  if (iter_max < MIN_DOUBLE || iter_max > MAX_DOUBLE) {
    print_error(E_NUM2);
    return;
  }
  if (iter_eps < MIN_DOUBLE || iter_eps > MAX_DOUBLE) {
    print_error(E_NUM3);
    return;
  }

  if (iter_min > iter_max) {
    print_error(E_MINMAX);
    return;
  }

  //Zjistim pocet iteraci a zkontroluji vysledky
  cf_iter_result     = cf_iter(iter_min, iter_max, iter_eps);
  taylor_iter_result = taylor_iter(iter_min, iter_max, iter_eps);

  if (cf_iter_result == -1 || taylor_iter_result == -1) {
    print_error(E_NUMOFITER);
    return;
  }

  //Tisknu vysledky
  printf(
    "       log(%g) = %.12g\n"
    "       log(%g) = %.12g\n"
    "continued fraction iterations = %d\n"
    "    cf_log(%g) = %.12g\n"
    "    cf_log(%g) = %.12g\n"
    " taylor polynomial iterations = %d\n"
    " taylor_log(%g) = %.12g\n"
    " taylor_log(%g) = %.12g\n",
    iter_min, log(iter_min),
    iter_max, log(iter_max),
    cf_iter_result,
    iter_min, cfrac_log(iter_min, cf_iter_result),
    iter_max, cfrac_log(iter_max, cf_iter_result),
    taylor_iter_result,
    iter_min, taylor_log(iter_min, taylor_iter_result),
    iter_max, taylor_log(iter_max, taylor_iter_result)
  );

  return;
}


///Hlavni funkce
int main (int argc, char *argv[]) {
  bool   arg_log  = false;
  bool   arg_iter = false;
  double numbers[3] = {0.0, 0.0, 0.0};

  if (argc == 1) {
    print_error(E_NUMOFARG);
    return 0;
  }
  //Prectu si prvni argument
  if (strcmp(argv[1], "--log") == 0) {
    if (argc != 4) {  //a zkontroluji pocet argumentu...
      print_error(E_NUMOFARG);
      return 0;
    }

    arg_log = true;
  }
  else if (strcmp(argv[1], "--iter") == 0) {
    if (argc != 5) {
      print_error(E_NUMOFARG);
      return 0;
    }

    arg_iter = true;
  }
  else if (strcmp(argv[1], "--help") == 0) {
    if (argc != 2) {
      print_error(E_NUMOFARG);
      return 0;
    }

    print_help();
    return 0;
  }
  else {    //Spatny prvni argument
    print_error(E_BADARG);
    return 0;
  }

  //Prevedu si argumenty na promenne, zkontroluji je a spustim vypocet a tisk vysledku
  if (!number_control(&*argv, numbers, argc - 2)) return 0;

  if (arg_log) print_log(numbers[0], numbers[1]);

  else if (arg_iter) print_iter(numbers[0], numbers[1], numbers[2]);

  return 0;
}