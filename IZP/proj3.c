/*******************************
 ** Autor: Vaclav Martinka    **
 **    ID: xmarti76           **
 ** Nazev: Proj3              **
 *******************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX
#include <string.h> //strcmp

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("%s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif


/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 *   struct objt_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *      misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
  int id;
  float x;
  float y;
};

struct cluster_t {
  int size;
  int capacity;
  struct obj_t *obj;
};

/*****************************************************************
 *                Deklarace potrebnych funkci                    *
 *****************************************************************/
// Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
// Ukazatel NULL u pole objektu znamena kapacitu 0.
void init_cluster (struct cluster_t *c, int cap) {
  assert(c != NULL);
  assert(cap >= 0);

  struct obj_t *p = malloc(cap * sizeof(struct obj_t));   //alokuji pamet
  if(p == NULL) {                                         //v pripade chyby -> cap = 0
    fprintf(stderr, "Allocation of objects were unsuccessful.\n   New capacity: %d\n", cap);
    cap = 0;
  }

  c->size = 0;          //zapisu data
  c->capacity = cap;
  c->obj = p;
}


// Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
void clear_cluster (struct cluster_t *c) {
  assert(c != NULL);

  free(c->obj);       //uvolnim pamet pro objekty
  c->obj = NULL;      //a vynuluji cluster c
  c->capacity = 0;
  c->size = 0;
}


// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;


// Zmena kapacity shluku 'c' na kapacitu 'new_cap'
struct cluster_t *resize_cluster (struct cluster_t *c, int new_cap) {
  assert(c);
  assert(c->capacity >= 0);
  assert(new_cap >= 0);

  if (c->capacity >= new_cap)
    return c;

  size_t size = sizeof(struct obj_t) * new_cap;

  void *arr = realloc(c->obj, size);
  if (arr == NULL)
    return NULL;

  c->obj = arr;
  c->capacity = new_cap;

  return c;
}


// Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt nevejde.
void append_cluster (struct cluster_t *c, struct obj_t obj) {
  assert(c != NULL);

  void *tmp = NULL;

  if(c->size == c->capacity) {                              //pokud je pole plne
    if(c->capacity >= INT_MAX - CLUSTER_CHUNK) {
      fprintf(stderr, "Resize of cluster was unsuccessful.\n");
      return;
    }
    debug("Rozsiruji shluk - je plny...");
    tmp = resize_cluster(c, c->capacity + CLUSTER_CHUNK);   //zvetsim jeho velikost
    if(tmp == NULL) {
      fprintf(stderr, "Resize of cluster was unsuccessful.\n");
      return;
    }
  }

  c->obj[c->size] = obj;    //Ulozim novy objekt na konec pole
  c->size ++;               //a zvysi pocet objektu
}


// Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
void sort_cluster (struct cluster_t *c);


/* Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen. */
void merge_clusters (struct cluster_t *c1, struct cluster_t *c2) {
  assert(c1 != NULL);
  assert(c2 != NULL);

  for(int i = 0 ; i < c2->size ; i ++)
    append_cluster(c1, c2->obj[i]);

  sort_cluster(c1);
}

/**********************************************************************/
/** Prace s polem shluku **/

/* Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli. */
int remove_cluster (struct cluster_t *carr, int narr, int idx) {
  assert(idx < narr);
  assert(narr > 0);

  free(carr[idx].obj);

  carr[idx].obj      = carr[narr - 1].obj;
  carr[idx].size     = carr[narr - 1].size;
  carr[idx].capacity = carr[narr - 1].capacity;

  carr[narr - 1].obj      = NULL;
  carr[narr - 1].size     = 0;
  carr[narr - 1].capacity = 0;

  return narr - 1;
}


// Pocita Euklidovskou vzdalenost mezi dvema objekty.
float obj_distance (struct obj_t *o1, struct obj_t *o2) {
  assert(o1 != NULL);
  assert(o2 != NULL);

  float x = fabs(o1->x - o2->x);
  float y = fabs(o1->y - o2->y);

  return sqrt((x*x) + (y*y));
}


// Pocita vzdalenost dvou shluku. Vzdalenost je vypoctena na zaklade nejblizsiho souseda.
float cluster_distance (struct cluster_t *c1, struct cluster_t *c2) {
  assert(c1 != NULL);
  assert(c1->size > 0);
  assert(c2 != NULL);
  assert(c2->size > 0);

  float distance = 0, distance_tmp = 0;
  int i = 0, j = 0;

  while(1) {
    distance_tmp = obj_distance(&c1->obj[i], &c2->obj[j]);
    if((i == 0 && j == 0) || (distance_tmp < distance))
      distance = distance_tmp;
    j ++;

    if(j >= c2->size) {
      i ++;
      if(i >= c1->size) break;
      j = 0;
    }
  }

  return distance;
}


/* Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky (podle nejblizsiho souseda). Nalezene shluky
 identifikuje jejich indexy v poli 'carr'. Funkce nalezene shluky (indexy do
 pole 'carr') uklada do pameti na adresu 'c1' resp. 'c2'. */
void find_neighbours (struct cluster_t *carr, int narr, int *c1, int *c2) {
  assert(narr > 0);
  assert(c1 != NULL);
  assert(c2 != NULL);
  assert(carr != NULL);

  int i = 0, j = 1;
  float distance_tmp, distance;

  while(1) {
    distance_tmp = cluster_distance(&carr[i], &carr[j]);
    if((i == 0 && j == 1) || (distance_tmp < distance)) {
      distance = distance_tmp;
      *c1 = i;
      *c2 = j;
    }
    j ++;

    if(j >= narr) {
      i ++;
      if(i >= (narr - 1)) break;
      j = i + 1;
    }
  }
}


// Pomocna funkce pro razeni shluku
static int obj_sort_compar (const void *a, const void *b) {
  const struct obj_t *o1 = a;
  const struct obj_t *o2 = b;
  if (o1->id < o2->id) return -1;
  if (o1->id > o2->id) return 1;
  return 0;
}


// Razeni objektu ve shluku vzestupne podle jejich identifikatoru
void sort_cluster (struct cluster_t *c) {
  qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}


// Tisk shluku 'c' na stdout
void print_cluster (struct cluster_t *c) {
  for (int i = 0 ; i < c->size ; i++) {
    if (i)
      putchar(' ');
    printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
  }
  putchar('\n');
}


/* Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL. */
int load_clusters (char *filename, struct cluster_t **arr) {
  assert(filename != NULL);
  assert(arr != NULL);

  struct obj_t obj_tmp;

  FILE *file_obj = fopen(filename, "rt");
    if (file_obj == NULL) {
      fprintf(stderr, "Opening the file was unsuccessful.\n");
      *arr = NULL;
      return 0;
    }

  int count_of_obj = 0, count_declared, err, tmp_size = 0;
  int id, x, y;
  char tmp_char;

  err = fscanf(file_obj, "count=%d", &count_declared);
  if(err != 1 || count_declared <= 0) {
    fprintf(stderr, "Reading the file header was unsuccessful.\n");
    fclose(file_obj);
    *arr = NULL;
    return 0;
  }

  struct cluster_t *array = malloc(count_declared * sizeof(struct cluster_t));
  if(array == NULL) {
    fprintf(stderr, "Alocation of array of clusters was unsuccessful.\n");
    fclose(file_obj);
    *arr = NULL;
    return 0;
  }
  *arr = array;

  while(count_of_obj < count_declared){   //ctu postupne radky
    if(count_of_obj == INT_MAX) {
      for(int i = 0 ; i < count_of_obj ; i ++)
        clear_cluster(&array[i]);
      free(array);
      *arr = NULL;
      break;
    }

    if(count_of_obj < (count_declared - 1)) {
      err = fscanf(file_obj, "%d %d %d%c", &id, &x, &y, &tmp_char);
      if(err != 4 || !(tmp_char == 10 || tmp_char == 13)) {
        fprintf(stderr, "Reading the %d. line was unsuccessful.\n", count_of_obj + 1);
        fprintf(stderr, "id: %d   x: %d   y: %d   c: '%c' (%d)\n", id, x, y, tmp_char, tmp_char);

        for(int i = 0 ; i < count_of_obj ; i ++)
          clear_cluster(&array[i]);
        free(array);
        *arr = NULL;
        break;
      }
    }
    if(count_of_obj == (count_declared - 1)) {
      err = fscanf(file_obj, "%d %d %d", &id, &x, &y);
      if(err != 3) {
        fprintf(stderr, "Reading the last %d. line was unsuccessful.\n", count_of_obj + 1);
        fprintf(stderr, "id: %d   x: %d   y: %d\n", id, x, y);

        for(int i = 0 ; i < count_of_obj ; i ++)
          clear_cluster(&array[i]);
        free(array);
        *arr = NULL;
        break;
      }
    }

    if(x > 1000 || y > 1000 || x < 0 || y < 0) {
        fprintf(stderr, "Some number on line %d is out of range.\n", count_of_obj + 1);

        for(int i = 0 ; i < count_of_obj ; i ++)
          clear_cluster(&array[i]);
        free(array);
        *arr = NULL;
        break;
    }

    obj_tmp.id = id;
    obj_tmp.x = x;
    obj_tmp.y = y;

    init_cluster(&array[count_of_obj], 1);
    if(array[count_of_obj].obj == NULL) {
      for(int i = 0 ; i < count_of_obj ; i ++)
        clear_cluster(&array[i]);
      free(array);
      *arr = NULL;
      break;
    }

    tmp_size = array[count_of_obj].size;
    append_cluster(&array[count_of_obj], obj_tmp);
    if(array[count_of_obj].size == tmp_size) {
      for(int i = 0 ; i < count_of_obj ; i ++)
        clear_cluster(&array[i]);
      free(array);
      *arr = NULL;
      break;
    }

    count_of_obj ++;
  }

  fclose(file_obj);

  return count_of_obj;
}


// Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
// Tiskne se prvnich 'narr' shluku.
void print_clusters (struct cluster_t *carr, int narr) {
  printf("Clusters:\n");
  for (int i = 0 ; i < narr ; i ++) {
    printf("cluster %d: ", i);
    print_cluster(&carr[i]);
  }
}


int main (int argc, char *argv[]) {
  char *err = NULL;
  long tmp = 0.0;
  int count_of_clusters = 0, loaded_clusters = 0;
  int first_cluster, second_cluster, size_tmp = 0;

  if(argc > 3 || argc < 2) {
    fprintf(stderr, "Bad count of arguments.\n");
    return 1;
  }

  if(argc == 2)
    count_of_clusters = 1;
  else {
    tmp = strtoll(argv[2], &err, 10);
    if(err == argv[2] || *err != '\0') {
      fprintf(stderr, "Second argument isn't number.\n");
      return 1;
    }
    if(tmp < 1 || tmp > INT_MAX) {
      fprintf(stderr, "Second argument is smaller than 1 or bigger than %d.\n", INT_MAX);
      return 1;
    }
    count_of_clusters = tmp;
  }

  struct cluster_t *clusters;

  loaded_clusters = load_clusters(argv[1], &clusters);
  if(clusters == NULL) {
    fprintf(stderr, "Loading of clusters was unsuccessful.\n");
    return 1;
  }
  if(count_of_clusters > loaded_clusters) {
    fprintf(stderr, "Count of clusters is mass then count of loaded clusters.\n");
    for(int i = 0 ; i < count_of_clusters ; i ++)
      clear_cluster(&clusters[i]);
    free(clusters);
    return 1;
  }

  while(count_of_clusters != loaded_clusters) {
    find_neighbours(clusters, loaded_clusters, &first_cluster, &second_cluster);

    size_tmp = clusters[first_cluster].size;
    merge_clusters(&clusters[first_cluster], &clusters[second_cluster]);
    if(clusters[first_cluster].size == size_tmp) {
      fprintf(stderr, "Merge of clusters was unsuccessful.\n");
      return 1;
    }

    remove_cluster(clusters, loaded_clusters, second_cluster);
    loaded_clusters --;
  }

  print_clusters(clusters, count_of_clusters);

  for(int i = 0 ; i < count_of_clusters ; i ++)
    clear_cluster(&clusters[i]);
  free(clusters);

  return 0;
}