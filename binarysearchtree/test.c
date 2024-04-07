// https://github.com/sanori/bstree-lab
#include "binarysearchtree.h"


// new_bstree should return bstree struct with null root node
void test_init(void) {
  bstree *t = new_bstree();
  assert(t != NULL);
#ifdef SENTINEL
  assert(t->nil != NULL);
  assert(t->root == t->nil);
#else
  assert(t->root == NULL);
#endif
  delete_bstree(t);
}

// root node should have proper values and pointers
void test_insert_single(const key_t key) {
  bstree *t = new_bstree();
  node_t *p = bstree_insert(t, key);
  assert(p != NULL);
  assert(t->root == p);
  assert(p->key == key);
  // assert(p->color == BsTREE_BLACK);  // color of root node should be black
#ifdef SENTINEL
  assert(p->left == t->nil);
  assert(p->right == t->nil);
  assert(p->parent == t->nil);
#else
  assert(p->left == NULL);
  assert(p->right == NULL);
  assert(p->parent == NULL);
#endif
  delete_bstree(t);
}

// find should return the node with the key or NULL if no such node exists
void test_find_single(const key_t key, const key_t wrong_key) {
  bstree *t = new_bstree();
  node_t *p = bstree_insert(t, key);

  node_t *q = bstree_find(t, key);
  assert(q != NULL);
  assert(q->key == key);
  assert(q == p);

  q = bstree_find(t, wrong_key);
  assert(q == NULL);

  delete_bstree(t);
}

// erase should delete root node
void test_erase_root(const key_t key) {
  bstree *t = new_bstree();
  node_t *p = bstree_insert(t, key);
  assert(p != NULL);
  assert(t->root == p);
  assert(p->key == key);

  bstree_erase(t, p);
#ifdef SENTINEL
  assert(t->root == t->nil);
#else
  assert(t->root == NULL);
#endif

  delete_bstree(t);
}

static void insert_arr(bstree *t, const key_t *arr, const size_t n) {
  for (size_t i = 0; i < n; i++) {
    bstree_insert(t, arr[i]);
  }
}

static int comp(const void *p1, const void *p2) {
  const key_t *e1 = (const key_t *)p1;
  const key_t *e2 = (const key_t *)p2;
  if (*e1 < *e2) {
    return -1;
  } else if (*e1 > *e2) {
    return 1;
  } else {
    return 0;
  }
};

// min/max should return the min/max value of the tree
void test_minmax(key_t *arr, const size_t n) {
  // null array is not allowed
  assert(n > 0 && arr != NULL);

  bstree *t = new_bstree();
  assert(t != NULL);

  insert_arr(t, arr, n);
  assert(t->root != NULL);
#ifdef SENTINEL
  assert(t->root != t->nil);
#endif

  qsort((void *)arr, n, sizeof(key_t), comp);
  node_t *p = bstree_min(t);
  assert(p != NULL);
  assert(p->key == arr[0]);

  node_t *q = bstree_max(t);
  assert(q != NULL);
  assert(q->key == arr[n - 1]);

  bstree_erase(t, p);
  p = bstree_min(t);
  assert(p != NULL);
  assert(p->key == arr[1]);

  if (n >= 2) {
    bstree_erase(t, q);
    q = bstree_max(t);
    assert(q != NULL);
    assert(q->key == arr[n - 2]);
  }

  delete_bstree(t);
}

void test_to_array(bstree *t, const key_t *arr, const size_t n) {
  assert(t != NULL);

  insert_arr(t, arr, n);
  qsort((void *)arr, n, sizeof(key_t), comp);

  key_t *res = calloc(n, sizeof(key_t));
  bstree_to_array(t, res, n);
  for (int i = 0; i < n; i++) {
    assert(arr[i] == res[i]);
  }
  free(res);
}

void test_multi_instance() {
  bstree *t1 = new_bstree();
  assert(t1 != NULL);
  bstree *t2 = new_bstree();
  assert(t2 != NULL);

  key_t arr1[] = {10, 5, 8, 34, 67, 23, 156, 24, 2, 12, 24, 36, 990, 25};
  const size_t n1 = sizeof(arr1) / sizeof(arr1[0]);
  insert_arr(t1, arr1, n1);
  qsort((void *)arr1, n1, sizeof(key_t), comp);

  key_t arr2[] = {4, 8, 10, 5, 3};
  const size_t n2 = sizeof(arr2) / sizeof(arr2[0]);
  insert_arr(t2, arr2, n2);
  qsort((void *)arr2, n2, sizeof(key_t), comp);

  key_t *res1 = calloc(n1, sizeof(key_t));
  bstree_to_array(t1, res1, n1);
  for (int i = 0; i < n1; i++) {
    assert(arr1[i] == res1[i]);
  }

  key_t *res2 = calloc(n2, sizeof(key_t));
  bstree_to_array(t2, res2, n2);
  for (int i = 0; i < n2; i++) {
    assert(arr2[i] == res2[i]);
  }

  free(res2);
  free(res1);
  delete_bstree(t2);
  delete_bstree(t1);
}

// Search tree constraint
// The values of left subtree should be less than or equal to the current node
// The values of right subtree should be greater than or equal to the current
// node

static bool search_traverse(const node_t *p, key_t *min, key_t *max,
                            node_t *nil) {
  if (p == nil) {
    return true;
  }

  *min = *max = p->key;

  key_t l_min, l_max, r_min, r_max;
  l_min = l_max = r_min = r_max = p->key;

  const bool lr = search_traverse(p->left, &l_min, &l_max, nil);
  if (!lr || l_max > p->key) {
    return false;
  }
  const bool rr = search_traverse(p->right, &r_min, &r_max, nil);
  if (!rr || r_min < p->key) {
    return false;
  }

  *min = l_min;
  *max = r_max;
  return true;
}

void test_search_constraint(const bstree *t) {
  assert(t != NULL);
  node_t *p = t->root;
  key_t min, max;
#ifdef SENTINEL
  node_t *nil = t->nil;
#else
  node_t *nil = NULL;
#endif
  assert(search_traverse(p, &min, &max, nil));
}

// Color constraint
// 1. Each node is either red or black. (by definition)
// 2. All NIL nodes are considered black.
// 3. A red node does not have a red child.
// 4. Every path from a given node to any of its descendant NIL nodes goes
// through the same number of black nodes.

bool touch_nil = false;
int max_black_depth = 0;

static void init_color_traverse(void) {
  touch_nil = false;
  max_black_depth = 0;
}

static bool color_traverse(const node_t *p, const color_t parent_color,
                           const int black_depth, node_t *nil) {
  if (p == nil) {
    if (!touch_nil) {
      touch_nil = true;
      max_black_depth = black_depth;
    } else if (black_depth != max_black_depth) {
      return false;
    }
    return true;
  }
  if (parent_color == BsTREE_RED && p->color == BsTREE_RED) {
    return false;
  }
  int next_depth = ((p->color == BsTREE_BLACK) ? 1 : 0) + black_depth;
  return color_traverse(p->left, p->color, next_depth, nil) &&
         color_traverse(p->right, p->color, next_depth, nil);
}

void test_color_constraint(const bstree *t) {
  assert(t != NULL);
#ifdef SENTINEL
  node_t *nil = t->nil;
#else
  node_t *nil = NULL;
#endif
  node_t *p = t->root;
  assert(p == nil || p->color == BsTREE_BLACK);

  init_color_traverse();
  assert(color_traverse(p, BsTREE_BLACK, 0, nil));
}

// bstree should keep search tree and color constraints
void test_rb_constraints(const key_t arr[], const size_t n) {
  bstree *t = new_bstree();
  assert(t != NULL);

  insert_arr(t, arr, n);
  assert(t->root != NULL);
#ifndef BST
  test_color_constraint(t);
#endif
  test_search_constraint(t);

  delete_bstree(t);
}

// bstree should manage distinct values
void test_distinct_values() {
  const key_t entries[] = {10, 5, 8, 34, 67, 23, 156, 24, 2, 12};
  const size_t n = sizeof(entries) / sizeof(entries[0]);
  test_rb_constraints(entries, n);
}

// bstree should manage values with duplicate
void test_duplicate_values() {
  const key_t entries[] = {10, 5, 5, 34, 6, 23, 12, 12, 6, 12};
  const size_t n = sizeof(entries) / sizeof(entries[0]);
  test_rb_constraints(entries, n);
}

void test_minmax_suite() {
  key_t entries[] = {10, 5, 8, 34, 67, 23, 156, 24, 2, 12};
  const size_t n = sizeof(entries) / sizeof(entries[0]);
  test_minmax(entries, n);
}

void test_to_array_suite() {
  bstree *t = new_bstree();
  assert(t != NULL);

  key_t entries[] = {10, 5, 8, 34, 67, 23, 156, 24, 2, 12, 24, 36, 990, 25};
  const size_t n = sizeof(entries) / sizeof(entries[0]);
  test_to_array(t, entries, n);

  delete_bstree(t);
}

void test_find_erase(bstree *t, const key_t *arr, const size_t n) {
  for (int i = 0; i < n; i++) {
    node_t *p = bstree_insert(t, arr[i]);
    assert(p != NULL);
  }

  for (int i = 0; i < n; i++) {
    node_t *p = bstree_find(t, arr[i]);
    // printf("arr[%d] = %d\n", i, arr[i]);
    assert(p != NULL);
    assert(p->key == arr[i]);
    bstree_erase(t, p);
  }

  for (int i = 0; i < n; i++) {
    node_t *p = bstree_find(t, arr[i]);
    assert(p == NULL);
  }

  for (int i = 0; i < n; i++) {
    node_t *p = bstree_insert(t, arr[i]);
    assert(p != NULL);
    node_t *q = bstree_find(t, arr[i]);
    assert(q != NULL);
    assert(q->key == arr[i]);
    assert(p == q);
    bstree_erase(t, p);
    q = bstree_find(t, arr[i]);
    assert(q == NULL);
  }
}

void test_find_erase_fixed() {
  const key_t arr[] = {10, 5, 8, 34, 67, 23, 156, 24, 2, 12, 24, 36, 990, 25};
  const size_t n = sizeof(arr) / sizeof(arr[0]);
  bstree *t = new_bstree();
  assert(t != NULL);

  test_find_erase(t, arr, n);

  delete_bstree(t);
}

void test_find_erase_rand(const size_t n, const unsigned int seed) {
  srand(seed);
  bstree *t = new_bstree();
  key_t *arr = calloc(n, sizeof(key_t));
  for (int i = 0; i < n; i++) {
    arr[i] = rand();
  }

  test_find_erase(t, arr, n);

  free(arr);
  delete_bstree(t);
}

int main(void) {
  // tree 초기화 잘 하는지 확인
  test_init();
  // 루트값을 넣고 노드  초기화 잘 하는지 확인
  test_insert_single(1024);
  // 첫번째 값을 넣고 잘찾는지, 두번째 값을 못찾는지 확인
  test_find_single(512, 1024);
  // 루트 노드의 값만 넣고 지웠을 때 잘되는지 확인
  test_erase_root(128);
  // 고정된 값을 전부 트리에 넣고, 넣은 값들을 전부 찾고, 전부 지우는게 잘되는지 확인,
  // 고정된 값들을 하나씩 넣고, 찾고, 지우며 잘 시행하는지 확인 (test_find_erase_rand 의 고정버전)
  test_find_erase_fixed();
  // 정해진 값을 넣고 트리의 min max 함수가 잘되는지 확인, min max 값을 지우고 다시 한번 더 min max 값이 잘 나오는지 확인
  test_minmax_suite();
  // 정해진 값을 넣고 트리의 to_array와 입력한 값의 quicksort한 값과 비교
  test_to_array_suite();    
  // 독립된 값을 넣은 트리에 전위순회하면서 좌우 값이 규칙에 맞는지 확인 (색깔은 확인안하도록 처리함)
  test_distinct_values();   
  // 중복된 값을 넣은 트리에 전위순회하면서 좌우 값이 규칙에 맞는지 확인 (색깔은 확인안하도록 처리함)
  test_duplicate_values();  
  // 여러 트리를 만들어서 정해진 값을 넣고 트리의 to_array와 입력한 값의 quicksort한 값과 비교(test_to_array_suite 두번함)
  test_multi_instance();    
  // 랜덤값을 전부 트리에 넣고, 넣은 값들을 전부 찾고, 전부 지우는게 잘되는지 확인,
  // 랜덤값들을 하나씩 넣고, 찾고, 지우며 잘 시행하는지 확인 (test_find_erase_fixed 의 랜덤 버전)
  test_find_erase_rand(10000, 17); 
  printf("Passed all tests!\n");
}
