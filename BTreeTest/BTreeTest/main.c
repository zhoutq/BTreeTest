//
//  main.c
//  BTreeTest
//
//  Created by zhoutq on 2017/9/16.
//  Copyright © 2017年 zhoutq. All rights reserved.
//

#include <stdio.h>
#include <malloc/malloc.h>
#include <stdlib.h>

/*
 定义二叉树节点类型
 */
typedef struct BNode {
    
    char data;
    struct BNode *lChild, *rChild;
}BTree;

/*
 创建二叉树:先创建根节点，然后递归用同样的方法创建其左右子节点

        a
       / \
      b  e
     / \ /
    c  d f

 如果创建上面二叉树，‘ ’表示结束子节点创建，则输入字符串为:
 ‘a’ - 'b' - 'c' - ' ' - ' ' - 'd' - ' ' - ' ' - 'e' - 'f' - ' ' - ' ' - ' '

 */

// 键盘输入读取
BTree *CreateBTree() {
    
    char c;
    printf("please input btree data:");
    scanf("%c",&c);
    getchar(); // 屏蔽回车
    
    if (c == ' ') {
        return NULL;
    }
    
    BTree *bt = (BTree *)malloc(sizeof(BTree));
    bt->data = c;
    bt->lChild = CreateBTree(); // 递归创建左子节点
    bt->rChild = CreateBTree(); // 递归创建右子节点
    return bt;
}

// 数组读取
BTree *CreateBTree2(char a[]) {

    static int n = 0;
    char c = a[n++];
    
    if (c == ' ') {
        return NULL;
    }
    
    BTree *bt = (BTree *)malloc(sizeof(BTree));
    bt->data = c;
    bt->lChild = CreateBTree2(a); // 递归创建左子节点
    bt->rChild = CreateBTree2(a); // 递归创建右子节点
    return bt;
}

/*
 前序遍历二叉树：遍历顺序 root -> lChild -> rChild，示例结果：abcdef
 */
void PreorderPrintfBTree(BTree *bt) {
    
    if (!bt) {
        return;
    }
    
    printf("%c",bt->data);
    PreorderPrintfBTree(bt->lChild);
    PreorderPrintfBTree(bt->rChild);
}

/*
 中序遍历二叉树：遍历顺序 lChild -> root -> rChild，示例结果：cbdafe
 */
void InorderPrintfBTree(BTree *bt) {
    
    if (!bt) {
        return;
    }
    
    InorderPrintfBTree(bt->lChild);
    printf("%c",bt->data);
    InorderPrintfBTree(bt->rChild);
}

/*
 后序遍历二叉树：遍历顺序 lChild -> rChild -> root，示例结果：cdbfea
 */
void PostorderPrintfBTree(BTree *bt) {
    
    if (!bt) {
        return;
    }
    
    PostorderPrintfBTree(bt->lChild);
    PostorderPrintfBTree(bt->rChild);
    printf("%c",bt->data);
}

/*
 二叉树节点个数：如果二叉树为空，则二叉树节点个数为 0 ，
              如果不为空，则二叉树节点个数为左子节点个数 + 右子节点个数 + 1(该根节点)
 */
int NodeCountBTree(BTree *bt) {
    
    if (!bt) {
        return 0;
    }
    
    return (NodeCountBTree(bt->lChild) + NodeCountBTree(bt->rChild) + 1);
}

/*
 二叉树叶子节点个数：如果二叉树为空，则二叉树叶子节点个数为 0 ，
                 如果不为空，但左右子节点都为空，则二叉树叶子节点个数为 1
                 如果不为空，则二叉树叶子节点个数为左右子节点叶子节点个数之和
 */
int LeafNodeCountBTree(BTree *bt) {
    
    if (!bt) {
        return 0;
    }
    
    if (!bt->lChild && !bt->rChild) {
        return 1;
    }
    return (LeafNodeCountBTree(bt->lChild) + LeafNodeCountBTree(bt->rChild));
}

/*
 二叉树第k层节点个数：如果二叉树为空，则二叉树叶子节点个数为 0 ，
                  如果是第一层，则二叉树第k层节点个数为 1
                  如果不为空，则二叉树第k层节点个数为左右子节点第k-1层节点个数之和
 */
int KLevelNoteCount(BTree * bt, int k)
{
    if (!bt) {
        return 0;
    }
    
    if (1 == k) {
        return 1;
    }
    
    return KLevelNoteCount(bt->lChild, k - 1) + KLevelNoteCount(bt->rChild, k - 1);
}

/*
 二叉树深度：如果二叉树为空，则二叉树深度为 0 ，
           如果不为空，则二叉树深度为子节点深度(左右子节点深度最大值) + 1(该根节点)
 */
int DepthBTree(BTree *bt) {
    
    if (!bt) {
        return 0;
    }
    
    // 定义一个max的block函数 http://fuckingblocksyntax.com/
    int (^max)(int , int ) = ^int(int a, int b){
        
        return a > b ? a:b;
    };
    
    return (max(DepthBTree(bt->lChild),DepthBTree(bt->rChild)) + 1);
}

/*
 二叉树转换为有序双向链表：转换顺序为 lChild + root + rChild
 */

void BTreeToBLinkList(BTree *bt, BTree **head,BTree **end)
{
    if(!bt) {
        *head = NULL;
        *end = NULL;
        return;
    }
    
    BTree *lHead, *lEnd, *rHead, *rEnd;
    
    BTreeToBLinkList(bt->lChild, &lHead, &lEnd);
    BTreeToBLinkList(bt->rChild, &rHead, &rEnd);
    
    bt->lChild = lEnd; // 左右子节点各自转换完成后，根节点的左子节点指向左子节点的end
    bt->rChild = rHead; // 左右子节点各自转换完成后，根节点的右子节点指向右子节点的head
    
    if (lEnd) lEnd->rChild = bt; // 同时，如果左子节点的end存在，则其右子节点指向根节点
    if (rHead) rHead->lChild = bt; // 同时，如果右子节点的head存在，则其左子节点指向根节点
    
    // 返回head和end
    *head = lHead ? : bt; // 如果左子节点lHead存在，则head为lHead,否则为根节点
    if (end) *end = rEnd ? : bt; // 如果右子节点rEnd存在，则end为rEnd,否则为根节点, 加if判断是因为main函数调用的时候end入参为NULL，赋值会奔溃。
}


int main(int argc, const char * argv[]) {
    
    char a[] = {'a','b','c',' ',' ','d',' ',' ','e','f',' ',' ',' '};
    BTree *bt = CreateBTree2(a); //CreateBTree();
    PreorderPrintfBTree(bt);
    printf("\n");
//    InorderPrintfBTree(bt);
//    printf("\n");
//    PostorderPrintfBTree(bt);
//    printf("\n");

//    printf("node count: %d\n",NodeCountBTree(bt));
//    printf("leaf node count: %d\n",LeafNodeCountBTree(bt));
//    printf("k leve node count: %d\n",KLevelNoteCount(bt, 2));
//    printf("depth: %d\n",DepthBTree(bt));
    
    BTree *head;
    BTreeToBLinkList(bt, &head, NULL);
    
    // 定义一个打印转换后双链表的block函数
    void (^PrintfDLinkList)(BTree *) = ^(BTree *head){
        BTree *p = head;
        while (p) {
            printf("%c,",p->data);
            p = p->rChild;
        }
        printf("\n");
    };
    
    PrintfDLinkList(head);

    
    return 0;
}
