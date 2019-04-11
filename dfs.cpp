#include "stdafx.h"
#include "stdlib.h"
#include<stdio.h>
#include<malloc.h>
#include<time.h> 
struct NODE  *A_star(struct NODE *s);                    //A*算法
struct NODE  *Expand(struct NODE *pNode);               //扩展结点
struct NODE *Move(struct NODE *pNode, int i1, int j1);        //移动空格
int ISGoal(struct NODE *pNode);                 //判断此结点是否目标节点
int H_Function(struct NODE *pNode);           //计算此结点到达目标的代价
//当前结点是否为其父结点的父节点
int ISGrandFather(struct NODE *pNode, struct NODE *father);
void printpath(struct NODE *pNode);                       //输出解题路径
void printNode(struct NODE *pNode);                   //输出路径上的结点
//结点放入open表,依次查找open表
struct NODE  *AddToopen(struct NODE *pNode, struct NODE *pnode);
//结点放入closed表
struct NODE *AddToclosed(struct NODE *pNode, struct NODE *pnode);
//从链表中删除结点
struct NODE *Del(struct NODE *pNode, struct NODE *plist);
struct NODE *IN(struct NODE *pNode, struct NODE *plist);    //结点在链表内
int Equal(struct NODE *pNode, int a[3][3]);             //判断两结点是否相同
struct NODE *NewNode(int, int, int, int, int, int, int, int, int);          //创建新结点

//结点结构                                                                
struct NODE
{
	int  a[3][3];                                       //存放八数码的状态
	int  i0, j0;                                       //空格所在位置坐标
	int  g, f;                                                    //代价g,f
	struct NODE *father;                                        //父结点
	struct NODE *pNext;                                        //子结点
	int father_num;													//链表的深度
};
//全局变量open表、closed表且初始置空
struct NODE *g_popen = NULL, *g_pclosed = NULL;
int g_Goal[3][3] = { { 1, 2, 3 }, { 8, 0, 4 }, { 7, 6, 5 } };                 //八数码的目标状态
int k = 0;                                           //A*所得路径的节点数


//A*算法
struct NODE* A_star(struct NODE *s,int depth)
{
	struct NODE *n = NULL, *m = NULL, *pNode = NULL, *psubNodelist = NULL;
	g_popen = s;                                  //开始结点s放入open表
	g_pclosed = NULL;                                 //closed表初始置空
	while (g_popen)                                  //如果open表不为空
	{
		n = g_popen;                         //open表第一个结点赋值给结点n
		if (ISGoal(n) == 1) return(n);           //如果n是目标，搜索成功，返回n
		g_popen = g_popen->pNext;  //如果n不是目标结点，则从open表中取出n
		g_pclosed = AddToclosed(n, g_pclosed);        //并将结点n放入closed表
		psubNodelist = Expand(n);                             //扩展n


		while (psubNodelist)                               //如果n有子结点
		{
			m = psubNodelist;           //psubNodelist的第一个结点赋值给结点m
			psubNodelist = psubNodelist->pNext;        //从psubNodelist中取出m
			//如果m已在open表中，则使pNode指向open表中旧的m
			if (pNode = IN(m, g_popen))
			{
				continue;
			}
			//如果m不在open表，已在closed表中，则使pNode指向closed表中旧的m
			else if (pNode = IN(m, g_pclosed))
			{
				continue;
			}
			else 
			{
				//如果m不在open及closed表中
				
				if (m->father_num <= depth){
					g_popen = AddToopen(m, g_popen);              //则把m放入open表
				}
				
					
			}
		}
	}
	if (g_popen == NULL)
		return NULL;
	else 
		return(s);                            //如果open表为空，返回s即空值
}


//扩展结点，返回后继节点链表
struct NODE *Expand(struct NODE *pNode)
{
	struct NODE *psubNodelist = NULL, *m = NULL;
	int i, j, i1, j1;          //i,j表示空格移动向量；i1,j1表示移动后空格的位置
	for (i = -1; i <= 1; i++)
	{
		for (j = -1; j <= 1; j++)
		{
			if (i == 0 && j == 0) continue;
			if (i != 0 && j != 0) continue;                       //保证移动向量合法
			i1 = pNode->i0 + i;                           //确定移动后的位置i1,j1
			j1 = pNode->j0 + j;
			if (i1<0 || j1<0 || i1>2 || j1>2)                 //保证求得移动后的位置合法
				continue;
			m = Move(pNode, i1, j1);   //移动空格到位置(i1,j1)，即由pNode扩展出m
			if (ISGrandFather(m, pNode))            //如果m是pNode的父辈结点
			{
				free(m);                                //则放弃该扩展并释放m
				continue;
			}
			m->father = pNode;                           //pNode是m的父结点
			if (pNode == NULL){
				m->father_num = 1;
			}
			else {
				m->father_num = pNode->father_num + 1;
			}
			m->g = pNode->g + 1;                           //则g(m)=g(pNode)+1
			m->f = m->g + H_Function(m);                       //f(m)=g(m)+h(m)
			m->pNext = psubNodelist;
			psubNodelist = m;               //m放入后继结点表且作为第一个元素
		}
	}
	return psubNodelist;                             //返回后继节点（链表）
}

//移动空格
struct NODE *Move(struct NODE *pNode, int i1, int j1)
{
	int i2, j2, i, j;
	struct NODE *pTempNode;    //生成新结点，作为移动空格后得到的新结点
	//为新结点分配空间
	pTempNode = (struct NODE *)malloc(sizeof(struct NODE));
	if (pTempNode == NULL)  return NULL; //如果空间分配失败则返回空指针
	//复制移动前的状态
	for (i = 0; i<3; i++)
	{
		for (j = 0; j<3; j++)
		{
			pTempNode->a[i][j] = pNode->a[i][j];
			pTempNode->i0 = pNode->i0;
			pTempNode->j0 = pNode->j0;
		}
	}
	//空格与指定位置(i1,j1)交换
	i2 = pTempNode->i0;
	j2 = pTempNode->j0;                          //移动前空格的位置(i0,j0)
	pTempNode->a[i2][j2] = pTempNode->a[i1][j1];
	pTempNode->a[i1][j1] = 0;
	pTempNode->i0 = i1;
	pTempNode->j0 = j1;
	return(pTempNode);                     //返回移动空格后，得到新结点
}

//判断当前节点是否为目标节点
int ISGoal(struct NODE *pNode)
{
	int z;
	z = Equal(pNode, g_Goal);               //调用判断两结点是否相同的函数
	return z;                                    //相同时z=1，不同时z=0
}

//函数h,   当前结点到达目标节点的代价
int H_Function(struct NODE *pNode)
{
	int i, j, n = 0;//n为计算两状态中相同数码的棋盘距离总和，即代价
	for (i = 0; i<3; i++)
	{
		for (j = 0; j<3; j++)
		{
			for (int a = 0; a < 3; a++){
				for (int b = 0; b < 3; b++){
					if (pNode->a[i][j] == g_Goal[a][b]){
						int ai = (a - i), bj = b - j;//求出棋盘距离
						if (ai<0)
							ai = -ai;
						if (bj<0)
							bj = -bj;

						n += ai + bj;
					}
				}
			}


		}
	}
	return(n);
}


//当前结点是否为其父结点的父辈节点
int  ISGrandFather(struct NODE *pNode, struct NODE *father)
{
	int z;
	while (father)
	{
		z = Equal(pNode, father->a);    //判断pNode是否是其father的父辈节点
		if (z = 0) { father = father->father; continue; }
		else break;
	}
	return(z);
}

//当前节点放入open表,深搜的时候把搜索到的放在聚首。
struct NODE  *AddToopen(struct NODE *pnode, struct NODE *pNode)
{
	//struct NODE *p, *p1;	        //结点按其f值从小到大加入open表中排序
	pnode->pNext = pNode;
	return pnode;//返回open表
}


//当前结点放入closed表
struct NODE *AddToclosed(struct NODE *pnode, struct NODE *pNode)
{
	pnode->pNext = pNode;
	pNode = pnode;                            //新加入的结点放在链表前端
	return pNode;                                        //返回closed表
}



//从表中删除结点
struct NODE *Del(struct NODE *plist)
{
	plist = plist->pNext;
	return(plist);
}




//判断当前结点是否在表中
struct NODE  *IN(struct NODE *pNode, struct NODE *plist)
{   //使用while循环依次比较
	int i, j, flag = 1;
	while (plist)
	{
		flag = 1;
		for (i = 0; i<3; i++)
			for (j = 0; j<3; j++)
			{
				if (plist->a[i][j] != pNode->a[i][j]) flag = 0;
			}
		if (flag == 1) break;//pNode在plist内
		else plist = plist->pNext;
	}
	return plist;//返回已存在的旧结点  
}


//判断当前结点是否为给定的结点
int Equal(struct NODE *pNode, int a[3][3])
{
	int flag = 1;
	int i, j;
	for (i = 0; i<3; i++)
		for (j = 0; j<3; j++)
		{
			if (pNode->a[i][j] != a[i][j])
				flag = 0;                 //若某一位置的数码不同，则flag置0
			if (flag == 0) break;                       //若flag=0,则退出循环
		}
	return flag;                      //相同时返回flag=1，不同时返回flag=0
}



//新建结点
struct NODE *NewNode(int i00, int i01, int i02, int i10, int i11, int i12, int i20, int i21, int i22)
{
	struct NODE *pNode = NULL;
	int i, j;
	int bend = 0;
	pNode = (struct NODE *) malloc(sizeof(struct NODE));  //为pNode分配空间
	if (pNode == NULL) return(NULL);         //若空间分配失败则返回空指针
	pNode->a[0][0] = i00; pNode->a[0][1] = i01; pNode->a[0][2] = i02;
	pNode->a[1][0] = i10; pNode->a[1][1] = i11; pNode->a[1][2] = i12;
	//数码放入对应位置
	pNode->a[2][0] = i20; pNode->a[2][1] = i21; pNode->a[2][2] = i22;
	pNode->g = 0;
	pNode->f = 0;                                   //代价g,h,f值初始为零
	pNode->father = NULL;
	pNode->pNext = NULL;                                 //无父、子结点
	for (i = 0; i<3; i++)
		for (j = 0; j<3; j++)
		{
			if (pNode->a[i][j] == 0)
			{
				pNode->i0 = i;
				pNode->j0 = j;                     //使用for循环确定空格的坐标i0,j0
				bend = 1;
				break;
			}
			if (bend == 1)
				break;
		}
	return(pNode);                                     //返回新建的结点
}



//输出解题路径
void printpath(struct NODE *pNode)
{
	if (pNode == NULL) return;//若为空，则返回
	printpath(pNode->father);//递归调用printpath
	printNode(pNode);//输出路径中的结点
}

//输出结点
void printNode(struct NODE *pNode)
{
	int i, j;
	k = k + 1;
	printf("setp %d：\n", k);
	for (i = 0; i<3; i++)
	{
		for (j = 0; j<3; j++)
			printf("%d ", pNode->a[i][j]);
		printf("\n");                                             //换行
	}                                                //按格式输出八数码
	/*printf("该节点中0的位置:");
	printf("(%d,%d)\n", pNode->i0, pNode->j0);         //输出空格即0的位置
	//输出代价f,h
	printf("该节点的f值是%d, h值是%d\n", pNode->f, pNode->g);
	printf("**************************\n");*/
}
void main()
{
	int i; 

	clock_t start, end;
	start = clock();


	struct NODE *s;
	s = NewNode(2, 8, 3, 1, 6, 4, 7, 0, 5);                       //八数码的初始状态
	s->father_num = 1;
	for (i = 1; i <= 30; i++){
		s = NewNode(2, 8, 3, 1, 6, 4, 7, 0, 5);                       //八数码的初始状态
		s->father_num = 1;
		s = A_star(s,i);                                           //调用A*算法
		if (s != NULL)
			break;
		else{
			printf("深搜%d层未找到符合的结果\n", i);
		}
	}
	end = clock();
	printf("运行时间time=%f\n", (double)(end - start) / CLK_TCK);
	if (s)
	{
		printf("八数码所得路径过程如下：\n");
		printf("\n");
		printpath(s);

	}                                 //如果s不空，输出解题路径   
	else printf("No key\n");//否则输出“无路径”

	
	
	
	printf("深搜%d层正好找到符合的结果\n", i);
	int g_open_len = 1;
	while (g_popen->pNext != NULL){
		g_open_len++;
		g_popen = g_popen->pNext;
	}
	int g_close_len = 1;
	while (g_pclosed->pNext != NULL){
		g_close_len++;
		g_pclosed = g_pclosed->pNext;
	}
	printf("生成节点的长度：%d,拓展节点的长度：%d", g_open_len, g_close_len);
	system("pause");//否则输出“无路径”
}