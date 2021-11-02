#ifdef __TIAOBIAO__
/**
  跳表的节点
*/
struct skip_list_node
{
     int key; //key 是唯一的
     int value;  //存储的内容
     //当前节点的最大层次
     int max_level;
     /**
       柔性数组更具节点层次的不同指向大小不通的数组
       *next[0]表示该节点的下一个节点的索引地址
       .
       *next[n]表示该节点的第n层下一节点的索引地址
     */
     struct skip_list_node *next[];
};
/**
  跳表的层次和数目
*/
struct skip_list
{
    int level;//跳表的索引层次
    int num; //节点数目
    struct skip_list_node *head;
};

extern struct skip_list* skip_list_creat(int max_level);
extern int skip_list_insert (struct skip_list *list, int key, int value);
extern int skip_list_delete (struct skip_list *list, int key);
extern int skip_list_modify (struct skip_list *list, int key, int value);
extern int skip_list_search (struct skip_list *list, int key, int *value);
extern int skip_list_destroy(struct skip_list *list);

#endif //  __TIAOBIAO__