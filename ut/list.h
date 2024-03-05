#ifndef _LIST_H_
#define _LIST_H_

#include "public.h"

node *Create(void *data);                        //创建链表中新的元素
node *findIndex(node *head, int index);          //查找元素所在位置,有多个就全列出来
void addData(node *head, void *data);            //增
void insert(node *head, int index, void *data);  //插
void *delData(node *head, int index);            //删
void *findData(node *head, int index);           //查：查找某个索引的data值
void *update(node *head, int index, void *data); //改：//修改指定索引的值
// int selectData(node *head, ctarr *arr, void *data); //查：查找指定值，返回有多少个该值
node *createList();
void findAllint(node *head);
void findAllchar(node *head);
void findAlldouble(node *head);
// node *loadListFromFile(char *fileName, int typeSize); //保存文件中的内容到链表里面；
// void saveListToFile(char *fileName, node *List, int typeSize);

#endif