/** \mainpage Project #3 - cluster analysis
 *
 * \section about About program
 * This program do cluster analysis on file
 * with objects.
 *
 * \subsection author Author:
 * Václav Martinka<br>
 * xmarti76
 */


/** \defgroup cluster Working with clusters
 * \short
 * These functions work with clusters (initializing, merging, etc.).
 * Usually need pointer to cluster respectively some other data.
 */


/** \defgroup other Other functions
 * \short
 * Thess functions take care of the running program and
 * cluster analysis.
 */


/** \short Structure of objects
 * \details
 * One structure contain unique id
 * and coordinates of object.
 */
struct obj_t {
  /// Unique identifikator of object (integer greater then 0).
  int id;
  /// X-coordinate of object (integer from 0 to 1000).
  float x;
  /// Y-coordinate of object (integer from 0 to 1000).
  float y;
};

/** \short Cluster of objects
 * \details
 * One cluster contain array of objects.
 */
struct cluster_t {
  /// Number of objects in cluster.
  int size;
  /// Size of array.
  int capacity;
  /// Array of objects. This is a poniter to first object.
  struct obj_t *obj;
};

/** \ingroup cluster
 * This function initialize new cluster. It mean, allocate memory
 * for <i>cap</i> of objects and write pointer of this memory <i>as c.obj</i>.
 * Next it sets c.capacity as <i>cap</i> and <i>c.size</i> as 0.<br>
 * In case of error, set <i>c.obj</i> as NULL and <i>c.capacity</i> as 0.
 *
 * \param[in] c Pointer to cluster
 * \param[in] cap Number of objects
 *
 * \pre cap is greater than 0
 * \pre c isn't NULL
 * \post c.size is 0
 * \post c.capacity isn't 0
 * \post c.obj isn't NULL
 */
void init_cluster (struct cluster_t *c, int cap);

/** \ingroup cluster
 * This function destroy cluster. It mean clean memory of array of objects
 * and set size and capacity to 0. <i>c.obj</i> is set as NULL.
 *
 * \param[in] c Pointer to cluster
 *
 * \pre c isn't NULL
 * \post c.size is 0
 * \post c.capacity is 0
 * \post c.obj is NULL
 */
void clear_cluster (struct cluster_t *c);

/** \ingroup cluster
 * Chunk of cluster objects. Value recommended for reallocation.
 */
extern const int CLUSTER_CHUNK;

/** \ingroup cluster
 * This function resize cluster to new capacity. If actually capacity is bigger
 * then new do nothing. In case of error return NULL.
 * Else return pointer to <i>c</i>.
 *
 * \param[in] c Pointer to cluster
 * \param[in] new_cap New capacity as total number of objects
 *
 * \pre c isn't NULL
 * \pre c.capacity is greater then 0
 * \pre new_cap is greater then 0
 * \post c.capacity is new_cap
 *
 * \return Pointer to cluster
 * \return If return NULL, resize is unsuccessful.
 */
struct cluster_t *resize_cluster (struct cluster_t *c, int new_cap);

/** \ingroup cluster
 * This function append a object to end of clusters. In case, that capacity
 * of cluster is insufficient so it resize them.
 *
 * \param[in] c Pointer to cluster
 * \param[in] obj Appended object
 *
 * \pre c insn't NULL
 *
 * \post c.size is greater by one
 */
void append_cluster (struct cluster_t *c, struct obj_t obj);

/** \ingroup cluster
 * This function add objects from cluster <i>c2</i> to end of cluster <i>c1</i>.
 * In case, that capacity of cluster is insufficient so it resize them.
 *
 * \param[in] c1 Pointer to first cluster
 * \param[in] c2 Pointer to second cluster
 *
 * \pre c1 insn't NULL
 * \pre c2 insn't NULL
 *
 * \post New c1.size is equal to c1.size + c2. size
 */
void merge_clusters (struct cluster_t *c1, struct cluster_t *c2);

/** \ingroup cluster
 * This function remove cluster from array <i>carr</i> on index <i>idx</i>.
 * Size of array is <i>narr</i>. After destroy cluster function move last
 * cluster to place after deleted cluster.
 *
 * \param[in] carr Pointer to array of clusters
 * \param[in] narr Number of clusters in array
 * \param[in] idx Index of cluster to destroy
 *
 * \pre carr insn't NULL
 * \pre narr is greater then 0
 * \pre idx is greater or 0
 *
 * \return New number of clusters
 */
int remove_cluster (struct cluster_t *carr, int narr, int idx);

/** \ingroup other
 * This function calculate distance between two objects.
 *
 * \param[in] o1 Pointer to first object
 * \param[in] o2 Pointer to second object
 *
 * \pre o1 insn't NULL
 * \pre o2 insn't NULL
 *
 * \return Distance between first and second objects
 */
float obj_distance (struct obj_t *o1, struct obj_t *o2);

/** \ingroup other
 * This function calculate distance between two clusters.
 * Distance is the smallest possible.
 *
 * \param[in] c1 Pointer to first cluster
 * \param[in] c2 Pointer to second cluster
 *
 * \pre c1 insn't NULL
 * \pre c2 insn't NULL
 *
 * \return Distance between first and second clusters
 */
float cluster_distance (struct cluster_t *c1, struct cluster_t *c2);

/** \ingroup other
 * This function find the two nearest clusters in array <i>carr</i>. Size of
 * array is <i>narr</i>. Index of the nearest clusters are
 * <i>c1</i> and <i>c2</i>.
 *
 * \param[in] carr Array of clusters
 * \param[in] narr Size of array
 * \param[in] c1 Pointer to first object
 * \param[in] c2 Pointer to second object
 *
 * \pre narr is greater than 0
 * \pre carr insn't NULL
 * \pre c1 insn't NULL
 * \pre c2 insn't NULL
 * \post c1 and c2 are indexs of the nearest clusters
 */
void find_neighbours (struct cluster_t *carr, int narr, int *c1, int *c2);

/** \ingroup cluster
 * This function sort objects in cluster by id.
 *
 * \param[in] c Pointer to cluster
 *
 * \pre c insn't NULL
 */
void sort_cluster (struct cluster_t *c);

/** \ingroup other
 * This function print all objects in one cluster.
 *
 * \param[in] c Pointer to cluster
 *
 * \pre c insn't NULL
 */
void print_cluster (struct cluster_t *c);

/** \ingroup other
 * This function loads file with objects. For each object initialize cluster
 * and save it to them.
 *
 * Form of file is:<br>
 * *  first line: count=N<br>
 * *  other lines: id x y <br>
 * N, id, x and y are integeres.<br>
 * File contain N+1 lines, next lines will be ignorated.
 * If doesn't contain N+1 lines it's error.
 *
 * In case of error, function clear memory, set
 * <i>arr</i> to NULL and return 0.
 * \param[in] filename Name of file
 * \param[in] arr Pointer to pointer of array
 *
 * \pre filename insn't NULL
 * \pre arr insn't NULL
 * \post Arr is pointer to array of clusters
 *
 * \return Number of loaded objects
 */
int load_clusters (char *filename, struct cluster_t **arr);

/** \ingroup other
 * This function print objects in array <i>carr</i>. Number of printed
 * objects is <i>narr</i>.
 *
 * \param[in] carr Array of clusters
 * \param[in] narr Number of printed clusters
 *
 * \pre carr insn't NULL
 */
void print_clusters (struct cluster_t *carr, int narr);