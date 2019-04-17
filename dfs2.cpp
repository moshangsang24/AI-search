/********************************************************

filename:dfs.c
datetime:2019/4/11
version:0.0.0.1
Description: Depth-first algorithm

********************************************************/
//#include "stdafx.h"
#include "stdlib.h"
#include<stdio.h>
#include<malloc.h>
#include<time.h>
struct NODE  *A_star(struct NODE *s);                    //A*�㷨
struct NODE  *Expand(struct NODE *pNode);               //��չ���
struct NODE *Move(struct NODE *pNode, int i1, int j1);        //�ƶ��ո�
int ISGoal(struct NODE *pNode);                 //�жϴ˽���Ƿ�Ŀ��ڵ�
int H_Function(struct NODE *pNode);           //����˽�㵽��Ŀ��Ĵ���
//��ǰ����Ƿ�Ϊ�丸���ĸ��ڵ�
int ISGrandFather(struct NODE *pNode, struct NODE *father);
void printpath(struct NODE *pNode);                       //�������·��
void printNode(struct NODE *pNode);                   //���·���ϵĽ��
//������open��,���β���open��
struct NODE  *AddToopen(struct NODE *pNode, struct NODE *pnode);
//������closed��
struct NODE *AddToclosed(struct NODE *pNode, struct NODE *pnode);
//��������ɾ�����
struct NODE *Del(struct NODE *pNode, struct NODE *plist);
struct NODE *IN(struct NODE *pNode, struct NODE *plist);    //�����������
int Equal(struct NODE *pNode, int a[4][4]);             //�ж�������Ƿ���ͬ
struct NODE *NewNode(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int);          //�����½��

//���ṹ
struct NODE
{
	int  a[4][4];                                       //��Ű������״̬
	int  i0, j0;                                       //�ո�����λ������
	int  g, f;                                                    //����g,f
	struct NODE *father;                                        //�����
	struct NODE *pNext;                                        //�ӽ��
	int father_num;													//��������
};
//ȫ�ֱ���open��closed���ҳ�ʼ�ÿ�
struct NODE *g_popen = NULL, *g_pclosed = NULL;
//int g_Goal[4][4] = { { 1, 2, 3 }, { 8, 0, 4 }, { 7, 6, 5 } };                 //�������Ŀ��״̬
int g_Goal[4][4] = { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 },{ 13, 14, 15, 0} };  //ʮ������Ŀ��״̬
int k = 0;                                           //A*����·���Ľڵ���


//A*�㷨
struct NODE* A_star(struct NODE *s,int depth)
{
	struct NODE *n = NULL, *m = NULL, *pNode = NULL, *psubNodelist = NULL;
	g_popen = s;                                  //��ʼ���s����open��
	g_pclosed = NULL;                                 //closed���ʼ�ÿ�
	while (g_popen)                                  //���open��Ϊ��
	{
		n = g_popen;                         //open���һ����㸳ֵ�����n
		if (ISGoal(n) == 1) return(n);           //���n��Ŀ�꣬�����ɹ�������n
		g_popen = g_popen->pNext;  //���n����Ŀ���㣬���open����ȡ��n
		g_pclosed = AddToclosed(n, g_pclosed);        //�������n����closed��
		psubNodelist = Expand(n);                             //��չn


		while (psubNodelist)                               //���n���ӽ��
		{
			m = psubNodelist;           //psubNodelist�ĵ�һ����㸳ֵ�����m
			psubNodelist = psubNodelist->pNext;        //��psubNodelist��ȡ��m
			//���m����open���У���ʹpNodeָ��open���оɵ�m
			if (pNode = IN(m, g_popen))
			{
				continue;
			}
			//���m����open������closed���У���ʹpNodeָ��closed���оɵ�m
			else if (pNode = IN(m, g_pclosed))
			{
				continue;
			}
			else
			{
				//���m����open��closed����

				if (m->father_num <= depth){
					g_popen = AddToopen(m, g_popen);              //���m����open��
				}


			}
		}
	}
	if (g_popen == NULL)
		return NULL;
	else
		return(s);                            //���open��Ϊ�գ�����s����ֵ
}


//��չ��㣬���غ�̽ڵ�����
struct NODE *Expand(struct NODE *pNode)
{
	struct NODE *psubNodelist = NULL, *m = NULL;
	int i, j, i1, j1;          //i,j��ʾ�ո��ƶ�������i1,j1��ʾ�ƶ���ո��λ��
	for (i = -1; i <= 1; i++)
	{
		for (j = -1; j <= 1; j++)
		{
			if (i == 0 && j == 0) continue;
			if (i != 0 && j != 0) continue;                       //��֤�ƶ������Ϸ�
			i1 = pNode->i0 + i;                           //ȷ���ƶ����λ��i1,j1
			j1 = pNode->j0 + j;
			if (i1<0 || j1<0 || i1>3 || j1>3)                 //��֤����ƶ����λ�úϷ�
				continue;
			m = Move(pNode, i1, j1);   //�ƶ��ո�λ��(i1,j1)������pNode��չ��m
			if (ISGrandFather(m, pNode))            //���m��pNode�ĸ������
			{
				free(m);                                //���������չ���ͷ�m
				continue;
			}
			m->father = pNode;                           //pNode��m�ĸ����
			if (pNode == NULL){
				m->father_num = 1;
			}
			else {
				m->father_num = pNode->father_num + 1;
			}
			m->g = pNode->g + 1;                           //��g(m)=g(pNode)+1
			m->f = m->g + H_Function(m);                       //f(m)=g(m)+h(m)
			m->pNext = psubNodelist;
			psubNodelist = m;               //m�����̽�������Ϊ��һ��Ԫ��
		}
	}
	return psubNodelist;                             //���غ�̽ڵ㣨����
}

//�ƶ��ո�
struct NODE *Move(struct NODE *pNode, int i1, int j1)
{
	int i2, j2, i, j;
	struct NODE *pTempNode;    //�����½�㣬��Ϊ�ƶ��ո��õ����½��
	//Ϊ�½�����ռ�
	pTempNode = (struct NODE *)malloc(sizeof(struct NODE));
	if (pTempNode == NULL)  return NULL; //����ռ����ʧ���򷵻ؿ�ָ��
	//�����ƶ�ǰ��״̬
	for (i = 0; i<4; i++)
	{
		for (j = 0; j<4; j++)
		{
			pTempNode->a[i][j] = pNode->a[i][j];
			pTempNode->i0 = pNode->i0;
			pTempNode->j0 = pNode->j0;
		}
	}
	//�ո���ָ��λ��(i1,j1)����
	i2 = pTempNode->i0;
	j2 = pTempNode->j0;                          //�ƶ�ǰ�ո��λ��(i0,j0)
	pTempNode->a[i2][j2] = pTempNode->a[i1][j1];
	pTempNode->a[i1][j1] = 0;
	pTempNode->i0 = i1;
	pTempNode->j0 = j1;
	return(pTempNode);                     //�����ƶ��ո�󣬵õ��½��
}

//�жϵ�ǰ�ڵ��Ƿ�ΪĿ��ڵ�
int ISGoal(struct NODE *pNode)
{
	int z;
	z = Equal(pNode, g_Goal);               //�����ж�������Ƿ���ͬ�ĺ���
	return z;                                    //��ͬʱz=1����ͬʱz=0
}

//����h,   ��ǰ��㵽��Ŀ��ڵ�Ĵ���
int H_Function(struct NODE *pNode)
{
	int i, j, n = 0;//nΪ������״̬����ͬ��������̾����ܺͣ�������
	for (i = 0; i<4; i++)
	{
		for (j = 0; j<4; j++)
		{
			for (int a = 0; a < 4; a++){
				for (int b = 0; b < 4; b++){
					if (pNode->a[i][j] == g_Goal[a][b]){
						int ai = (a - i), bj = b - j;//������̾���
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


//��ǰ����Ƿ�Ϊ�丸���ĸ����ڵ�
int  ISGrandFather(struct NODE *pNode, struct NODE *father)
{
	int z;
	while (father)
	{
		z = Equal(pNode, father->a);    //�ж�pNode�Ƿ�����father�ĸ����ڵ�
		if (z = 0) { father = father->father; continue; }
		else break;
	}
	return(z);
}

//��ǰ�ڵ����open��,���ѵ�ʱ����������ķ��ھ��ס�
struct NODE  *AddToopen(struct NODE *pnode, struct NODE *pNode)
{
	//struct NODE *p, *p1;	        //��㰴��fֵ��С�������open��������
	pnode->pNext = pNode;
	return pnode;//����open��
}


//��ǰ������closed��
struct NODE *AddToclosed(struct NODE *pnode, struct NODE *pNode)
{
	pnode->pNext = pNode;
	pNode = pnode;                            //�¼���Ľ���������ǰ��
	return pNode;                                        //����closed��
}



//�ӱ���ɾ�����
struct NODE *Del(struct NODE *plist)
{
	plist = plist->pNext;
	return(plist);
}




//�жϵ�ǰ����Ƿ��ڱ���
struct NODE  *IN(struct NODE *pNode, struct NODE *plist)
{   //ʹ��whileѭ�����αȽ�
	int i, j, flag = 1;
	while (plist)
	{
		flag = 1;
		for (i = 0; i<4; i++)
			for (j = 0; j<4; j++)
			{
				if (plist->a[i][j] != pNode->a[i][j]) flag = 0;
			}
		if (flag == 1) break;//pNode��plist��
		else plist = plist->pNext;
	}
	return plist;//�����Ѵ��ڵľɽ��
}


//�жϵ�ǰ����Ƿ�Ϊ�����Ľ��
int Equal(struct NODE *pNode, int a[4][4])
{
	int flag = 1;
	int i, j;
	for (i = 0; i<4; i++)
		for (j = 0; j<4; j++)
		{
			if (pNode->a[i][j] != a[i][j])
				flag = 0;                 //��ĳһλ�õ����벻ͬ����flag��0
			if (flag == 0) break;                       //��flag=0,���˳�ѭ��
		}
	return flag;                      //��ͬʱ����flag=1����ͬʱ����flag=0
}



//�½����
struct NODE *NewNode(int i00, int i01, int i02, int i03, int i10, int i11, int i12, int i13, int i20, int i21, int i22, int i23, int i30, int i31, int i32, int i33)
{
	struct NODE *pNode = NULL;
	int i, j;
	int bend = 0;
	pNode = (struct NODE *) malloc(sizeof(struct NODE));  //ΪpNode����ռ�
	if (pNode == NULL) return(NULL);         //���ռ����ʧ���򷵻ؿ�ָ��
	pNode->a[0][0] = i00; pNode->a[0][1] = i01; pNode->a[0][2] = i02; pNode->a[0][3] = i03;
	pNode->a[1][0] = i10; pNode->a[1][1] = i11; pNode->a[1][2] = i12; pNode->a[1][3] = i13;
	//��������Ӧλ��
	pNode->a[2][0] = i20; pNode->a[2][1] = i21; pNode->a[2][2] = i22; pNode->a[2][3] = i23;
	pNode->a[3][0] = i30; pNode->a[3][1] = i31; pNode->a[3][2] = i32; pNode->a[3][3] = i33;
	pNode->g = 0;
	pNode->f = 0;                                   //����g,h,fֵ��ʼΪ��
	pNode->father = NULL;
	pNode->pNext = NULL;                                 //�޸����ӽ��
	for (i = 0; i<4; i++)
		for (j = 0; j<4; j++)
		{
			if (pNode->a[i][j] == 0)
			{
				pNode->i0 = i;
				pNode->j0 = j;                     //ʹ��forѭ��ȷ���ո������i0,j0
				bend = 1;
				break;
			}
			if (bend == 1)
				break;
		}
	return(pNode);                                     //�����½��Ľ��
}



//�������·��
void printpath(struct NODE *pNode)
{
	if (pNode == NULL) return;//��Ϊ�գ��򷵻�
	printpath(pNode->father);//�ݹ����printpath
	printNode(pNode);//���·���еĽ��
}

//������
void printNode(struct NODE *pNode)
{
	int i, j;
	k = k + 1;
	printf("setp %d��\n", k);
	for (i = 0; i<4; i++)
	{
		for (j = 0; j<4; j++)
			printf("%d ", pNode->a[i][j]);
		printf("\n");                                             //����
	}                                                //����ʽ���������
	/*printf("�ýڵ���0��λ��:");
	printf("(%d,%d)\n", pNode->i0, pNode->j0);         //����ո�0��λ��
	//�������f,h
	printf("�ýڵ��fֵ��%d, hֵ��%d\n", pNode->f, pNode->g);
	printf("**************************\n");*/
}
int main()
{
	int i;

	clock_t start, end;
	start = clock();


	struct NODE *s;
	//s = NewNode(2, 8, 3, 1, 6, 4, 7, 0, 5);                       //������ĳ�ʼ״̬
	s = NewNode(1, 3, 6, 4, 5, 2, 8, 0, 9, 10, 7, 11, 13, 14, 15, 12);  //ʮ���������״̬
	s->father_num = 1;
	for (i = 1; i <= 30; i++){
		//s = NewNode(2, 8, 3, 1, 6, 4, 7, 0, 5);                       //������ĳ�ʼ״̬
		s = NewNode(1, 3, 6, 4, 5, 2, 8, 0, 9, 10, 7, 11, 13, 14, 15, 12);  //ʮ���������״̬
		s->father_num = 1;
		s = A_star(s,i);                                           //����A*�㷨
		if (s != NULL)
			break;
		else{
			printf("����%d��δ�ҵ����ϵĽ��\n", i);
		}
	}
	end = clock();
	printf("����ʱ��time=%f\n", (double)(end - start) / CLK_TCK);
	if (s)
	{
		printf("����������·���������£�\n");
		printf("\n");
		printpath(s);

	}                                 //���s���գ��������·��
	else printf("No key\n");//�����������·����




	printf("����%d�������ҵ����ϵĽ��\n", i);
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
	printf("���ɽڵ�ĳ��ȣ�%d,��չ�ڵ�ĳ��ȣ�%d", g_open_len, g_close_len);
	system("pause");//�����������·����
	return 0;
}
