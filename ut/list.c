#include "list.h"

//#include "adndelnin.h" //引入动态数组的函数
// #include "public.h"
// //-------------------申明函数
// node *Create(void *data);                           //创建链表中新的元素
// node *findIndex(node *head, int index);             //查找元素所在位置,有多个就全列出来
// void addData(node *head, void *data);               //增
// void insert(node *head, int index, void *data);     //插
// void *delData(node *head, int index);               //删
// void *findData(node *head, int index);              //查：查找某个索引的data值
// void *update(node *head, int index, void *data);    //改：//修改指定索引的值
// int selectData(node *head, ctarr *arr, void *data); //查：查找指定值，返回有多少个该值
// node *createList();
// //-------------------

//修改指定索引的值
void *update(node *head, int index, void *data)
{
    node *p = findIndex(head, index);
    void *oldnum = p->data;
    p->data = data;
    return oldnum;
}

//查找指定值，返回有多少个该数
// int selectData(node *head, ctarr *arr, void *data)
// {
//     node *p = head->next;
//     int x = 0;
//     int *INDEX = 0;
//     while (p != head)
//     {
//         if (p->data == data)
//         {
//             addData_ARR(arr, INDEX); //使用动态数组记录索引
//             x++;
//         }
//         p = p->next;
//         *INDEX++;
//     }
//     if (x == 0)
//     {
//         printf("链表中未找到与之的相同数\n");
//     }
//     else
//     {
//         printf("找到 %d 个相同的数，索引为\n", x);
//         Traverse_ARR(arr, Traverse_Arr_int);
//     }
//     return x;
// }

//查找某个索引的data值
void *findData(node *head, int index)
{
    node *p = findIndex(head, index);
    if (p == head || p == NULL)
    {
        puts("Error in findIndex");
        return NULL;
    }
    return p->data;
}

//删除元素
void *delData(node *head, int index)
{
    node *p = findIndex(head, index);
    if (p != head)
    {
        p->next->front = p->front;
        p->front->next = p->next;
        void *del = p->data;
        free(p);
        return del;
    }
    else
    {
        puts("error");
        return NULL;
        // node *del = p;
        // p->next->next->front = &head;
        // p->next = p->next->next;
        // free(del);
    }
}

//在循环链表中插入数据，如果超出索引，则插入到最末尾
void insert(node *head, int index, void *data)
{
    node *p = findIndex(head, index);
    if (p == NULL)
    {
        printf("输入的索引有误或者索引超出范围！！！\n");
    }
    else if (p == head)
    {
        addData(head, data);
    }
    else
    {
        node *newp = Create(data);
        newp->next = p;
        newp->front = p->front;
        p->front->next = newp;
        p->front = newp;
    }
}

//新增
void addData(node *head, void *data)
{
    node *p = Create(data);
    if (head->next == NULL)
    {
        head->next = p;
        head->front = p;
        p->next = head;
        p->front = head;
    }
    else
    {
        p->front = head->front;
        p->next = head;
        head->front->next = p;
        head->front = p;
    }
}
//创建链表中的元素
node *Create(void *data)
{
    node *newp = (node *)malloc(sizeof(node));
    newp->data = data;
    newp->front = NULL;
    newp->next = NULL;
    return newp;
}
//查找元素所在位置
node *findIndex(node *head, int index)
{
    if (index < 0)
    {
        printf("输入的索引有误！！\n");
        return NULL;
    }
    node *p = head->next;
    while (p != head && index > 0)
    {
        p = p->next;
        index--;
    }
    return p;
}
//创建链表
node *createList()
{
    node *head = malloc(sizeof(node));
    head->next = head;
    head->front = head;
    return head;
}
//读取文件(二进制)内容到链表中，需要给出文件位置，变量类型大小；
// node *loadListFromFile(char *fileName, int typeSize)
// {
//     node *head = createList();
//     FILE *fp = openFile(fileName, "rb");
//     void *x = malloc(typeSize);
//     while (fileRead(fp, x, typeSize))
//     {
//         addData(head, x);
//         x = malloc(typeSize);
//     }
//     free(x);

//     fclose(fp);

//     return head;
// }
//保存链表中的元素到指定文件中，需要给出文件名，链表，保存的数据类型大小；
// void saveListToFile(char *fileName, node *List, int typeSize)
// {

//     FILE *fp = openFile(fileName, "wb");
//     node *p = List->next;
//     while (p != List)
//     {
//         fileWrite(fp, p->data, typeSize);
//         p = p->next;
//     }
//     closeFile(fp);
// }
//查整型的数值所有
void findAllint(node *head)
{
    node *p1 = head->next;
    while (p1 != head)
    {
        printf("data = %d , next = %p , front = %p\n", *(int *)(p1->data), p1->next, p1->front);
        p1 = p1->next;
    }
}
void findAllchar(node *head)
{
    node *p1 = head->next;
    while (p1 != head)
    {
        printf("data = %c \n", *(char *)(p1->data));
        p1 = p1->next;
    }
}
void findAlldouble(node *head)
{
    node *p1 = head->next;
    while (p1 != head)
    {
        printf("data = %lf \n", *(double *)(p1->data));
        p1 = p1->next;
    }
}