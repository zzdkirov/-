#include "def.h"

/*�м�������ɣ�ȥ��������ַ��ͱ���,���д����ֻ�漰��int�ͱ��������������飬�������ṹ*/
/*������0��������󣬼�ʹ���˷�int�ͱ���*/

/*�ⲿ������Ҫ���ƫ���������ڽڵ���ӽڵ��λ��*/

/*�ֲ�������ͬ�ⲿ����*/

/*��䣺�����Ӧ�����*/

/*���ʽ���ο����ʽ�����Զ����´������ȼ����ʽ*/

/*���飬����ֻ֧��һά���飬�����±���ԣ�������Ϊһ����ʱ����������������м���룬��ƫ��������ʱ������ƫ������˫�ӣ�Ҳ��Ϊ���������һ��*/

/*�ṹ��ͬ���������ṹ��ĵ�����Ϊ��ʱ�������ɽṹ�������ƫ��������ƫ��������*/

/*���˵������ǣ��Ƚ����е��м�������ɣ�Ȼ����ڸ������������ӣ����ⲿ���������ٽ������ӣ�*/

/*temp������ָ��������ִ�и�ֵд��ʱ��ָ�����Ǳ����ص�ַ*/

/*
ʵ������ݣ�
��1��ʹ��δ����ı�����
��2������δ�����δ�����ĺ�����
��3����ͬһ���������Ƶ��ظ����壨������������������ṹ�������Լ��ṹ���Ա���ȣ���
Ϊ�����˵�������������Ҳ���Բ�ֳɼ������͵Ĵ���������ظ����塢�����ظ����塢�ṹ���Ա���ظ��ȣ�
��4���ԷǺ��������ú���������ʽ��
��5���Ժ��������÷Ǻ���������ʽ���ʣ�
��6����������ʱ����������ƥ�䣬��ʵ�α��ʽ����̫�ࡢ��ʵ�α��ʽ����̫�٣�
��7����������ʱʵ�κ��β����Ͳ�ƥ�䣻
��8���Է�������������±��������ʽ���ʣ�
��9������������±겻�����ͱ��ʽ��
��10���Էǽṹ�������ó�Աѡ���������.����
��11���ṹ��Ա�����ڣ�
��12����ֵ����߲�����ֵ���ʽ��
��13���Է���ֵ���ʽ�����������Լ����㣻
��14���Խṹ����������������Լ����㣻
��15�����Ͳ�ƥ�䡣����������ṹ������������㣬��Ҫָ�����Ͳ�ƥ�������Щ��Ҫ���ݶ�������Ե��������н��н綨�����磺32+'A'��10*12.3�����ʹ��ǿ����
��������Ҫ���������C���Ե������͹�������������������ģ�����Ҫ�ں����׶���Ҫ��������ת��������ͳһ���ٽ��ж�Ӧ���㣻
��16����������ֵ�����뺯������ķ���ֵ���Ͳ�ƥ�䣻
��17������û�з�����䣨����������ֵ���Ͳ���voidʱ����
��18��break��䲻��ѭ������switch����У�
��19��continue��䲻��ѭ������У�
*/
int NoIRgenerate;

int ifincir;	//������ָʾ�Ƿ���ѭ����

int LV = 0;   //���

int func_size; //1�������Ļ��¼��С

int functype;//��ǰ����������

int ifreturn;

int nowfunc=-1;//ָʾ��ǰ�������±꣬���ڲ��������ж�

struct symbol_table symbolTable;

int globaloff;//ȫ�ֱ���ƫ����

int localoff=4;//�ֲ�����ƫ������ջ֡��ͷ��ebp����˾ֲ�����ƫ������ʼ��Ϊ4

int nowfuncparanum;//ͳ�Ƶ�ǰ��������ֵ

int funcwidth=0;

struct opn op1;
struct opn op2;
struct opn result;

struct codenode* Intermidiate;

int expresultindex;//��¼���ʽ����Ķ�Ӧ���ţ���������ڷ��ű��±꣬���ڱ��ʽ���ԣ����һ���Ǹ���ʱ��������Ϊ��Ӧ���ǼĴ���

struct codenode* root=NULL;

struct codenode* target=NULL;

//#define DISPSYMBOL
//��ʾ���ű�
void dispsymbol()
{
#ifdef DISPSYMBOL
    int i = 0;
    char *name;
    char symboltype[20];
    int lv;
    char type[10];
    short scope;
    char *master;
    printf("%s\t%s\t%s\t%s\t%s\t%s\n", "������","��������", "�� ��", "��  ��", "������","��  ��");
    for (i = 0; i < symbolTable.index; i++)
    {
        name=symbolTable.symbols[i].name;
        switch(symbolTable.symbols[i].flag)
        {
            case 'D':
                strcpy(symboltype,"FuncDeclare");
                master=NULL;
                break;
            case 'F':
                strcpy(symboltype,"FuncDefine");
                master=NULL;
                break;
            case 'V':
                strcpy(symboltype,"Variable");
                master=NULL;
                break;
            case 'A':
                strcpy(symboltype,"ArrayVariable");
                master=NULL;
                break;
            case 'B':
                strcpy(symboltype,"StructVar");
                master=NULL;
                break;
            case 'S':
                strcpy(symboltype,"StructDec");
                master=NULL;
                break;
            case 'P':
                strcpy(symboltype,"FuncParam");
                master=symbolTable.symbols[symbolTable.symbols[i].paramnum].name;
                break;
            case 'N':
                strcpy(symboltype,"StructMumber");
                master=symbolTable.symbols[symbolTable.symbols[i].paramnum].name;
                break;
            default:
                break;
        }
        lv=symbolTable.symbols[i].level;
        switch(symbolTable.symbols[i].type)
        {
            case -2:
                strcpy(type,"void");
                break;
            case -1:
                strcpy(type,"struct");
                break;
            case 0:
                strcpy(type,"int");
                break;
            case 1:
                strcpy(type,"char");
                break;
            case 2:
                strcpy(type,"float");
                break;
            default:
                break;
        }
        scope=symbolTable.symbols[i].local;
        printf("%s",name);
        printf("\t%s",symboltype);
        printf("\t%d",lv);
        printf("\t%s",type);
        if(scope)
            printf("\t%s","Local");
        else
            printf("\t%s","Global");

        if(master)
            printf("\t%s",master);
        else
            printf("\t%s","\\");
        putchar('\n');

    }
#endif // DISPSYMBOL
}
#define DISPINTERMIDIATE

void displayIR(struct codenode* code)
{
#ifdef DISPINTERMIDIATE
    while(code!=NULL)
    {
        switch(code->op)
        {
            case FUNC_IR://��������,result.offset�洢�˺���ջ֡��С
                printf("Function %s\n",code->result.id);
                break;
            case PARAM_IR://��������
                printf("Parameter %s\n",code->result.id);
                break;
            case ASSIGN_IR://��ֵ���㣨������ASSIGN�����temp=op1=op2����һ����temp=op(var/const)��
                printf("%s = %s = %s\n",code->result.id,code->opn1.id,code->opn2.id);
                break;
            case ARG_IR://����
                printf("Arg %s\n",code->result.id);
                break;
            case CALL_IR://��������
                if(code->result.kind)
                    printf("%s = Call %s\n",code->result.id,code->opn2.id);
                else
                    printf("Call %s\n",code->opn2.id);
                break;
            case ARR_IR://�������
                printf("%s = %s[%s]\n",code->result.id,code->opn1.id,code->opn2.id);
                break;
            case STRUCT_IR://�ṹ����
                printf("%s = %s.%s\n",code->result.id,code->opn1.id,code->opn2.id);
                break;
            case RETURN_IR://������䣬һ����д�ֵ��temp��result.offset�洢�˺����������ֽ���
                if(code->result.id)
                    printf("Return %s\n",code->result.id);
                else
                    printf("Return\n");
                break;
            /*
            case ASSIGN_ARR://����д
                printf("%s[%s] = %s\n",code->opn1.id,code->opn2.id,code->result.id);
                break;
            case ASSIGN_STRUCT://�ṹд
                printf("%s.%s = %s\n",code->opn1.id,code->opn2.id,code->result.id);
                break;
            */
            case IF_IR://��ʽif result==0 j op1 else j op2
                printf("IF %s == 0 Goto %s\n",code->result.id,code->opn1.id);
                break;
            case LABEL_IR://������
                printf("Lable %s\n",code->result.id);
                break;
            case GOTO_IR://��ת���
                printf("Goto %s\n",code->result.id);
                break;
            case EQ_IR://��Ӧbeq
                printf("If %s == %s Goto %s\n",code->opn1.id,code->opn2.id,code->result.id);
                break;
            case NE_IR://��Ӧbne
                printf("If %s != %s Goto %s\n",code->opn1.id,code->opn2.id,code->result.id);
                break;
            case GT_IR://��Ӧbgtz
                printf("If %s > %s Goto %s\n",code->opn1.id,code->opn2.id,code->result.id);
                break;
            case GE_IR://��Ӧbgez
                printf("If %s >= %s Goto %s\n",code->opn1.id,code->opn2.id,code->result.id);
                break;
            case LT_IR://��Ӧbltz
                printf("If %s < %s Goto %s\n",code->opn1.id,code->opn2.id,code->result.id);
                break;
            case LE_IR://��Ӧblez
                printf("If %s <= %s Goto %s\n",code->opn1.id,code->opn2.id,code->result.id);
                break;
            case ASSIGN://ǰ��������������ֵ
                if(code->opn2.kind==INT_CONST)
                    printf("%s = %d\n",code->result.id,code->opn2.const_int);
                else
                    printf("%s = %s\n",code->result.id,code->opn2.id);
                break;
            case PLUS://������ʽͳһtemp=op1+op2��tempΪ��ʱ����
                printf("%s = %s + %s\n",code->result.id,code->opn1.id,code->opn2.id);
                break;
            case MINUS:
                printf("%s = %s - %s\n",code->result.id,code->opn1.id,code->opn2.id);
                break;
            case MUL:
                printf("%s = %s * %s\n",code->result.id,code->opn1.id,code->opn2.id);
                break;
            case DIV:
                printf("%s = %s / %s\n",code->result.id,code->opn1.id,code->opn2.id);
                break;
            case MOD:
                printf("%s = %s % %s\n",code->result.id,code->opn1.id,code->opn2.id);
                break;
            case LSHFIT:
                printf("%s = %s << %s\n",code->result.id,code->opn1.id,code->opn2.id);
                break;
            case RSHFIT:
                printf("%s = %s >> %s\n",code->result.id,code->opn1.id,code->opn2.id);
                break;
            case AND:
                printf("%s = %s && %s\n",code->result.id,code->opn1.id,code->opn2.id);
                break;
            case OR:
                printf("%s = %s || %s\n",code->result.id,code->opn1.id,code->opn2.id);
                break;
            case END_IR:
                return ;
            default:
                break;
        }
        code=code->next;
    }
#endif // DISPINTERMIDIATE
}

//�����ַ���
char *strcats(char *s1, char *s2)
{
    static char result[10];
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

/*��������ǩ����ʱ������*/
char *createAlias()
{
    static int no = 1;
    char s[10];
    itoa(no++, s, 10);
    return strcats("v", s);
}
char *createLabel() {
    static int no=1;
    char s[10];
    itoa(no++,s,10);
    return strcats("Lable",s);
}

//����һ����ʱ����������
char *createTemp()
{
    static int no = 1;
    char s[10];
    itoa(no++, s, 10);
    return strcats("temp", s);
}

/*����addr������Ŀ���ǽ������ͽṹ��ĸ�ֵ���⣬ͬʱҲ���������࣬addr����Ϊ��temp����*/
char* createAddr()
{
    static int no = 1;
    char s[10];
    itoa(no++, s, 10);
    return strcats("addr", s);
}

/*��ʱ����0ӵ��ͳһ�����֣�temp0����Ӧ��mips��0�żĴ�����������һ������*/



//�����ⲿ���������б�
void ExtDefList(struct node *T)
{
    int start=0;
    while(T)
	{
		switch(T->ptr[0]->kind)
		{
			case VAR:
			{
				//decide type and varlist,including var and array
				VarDef(T->ptr[0]);
				break;
			}
			case FUNCDEF:
			{
				FuncDef(T->ptr[0]);
				root=merge(2,root,T->ptr[0]->code);
				if(start==0)
                {
                    start=1;
                    root->flag='H';
                }
				break;
			}
			case FUNCDEC:
			{
				FuncDec(T->ptr[0]);
				break;
			}
			case STRUCTDEC:
			{
				StructDec(T->ptr[0]);
				break;
			}
			case STRUCTDEF:
			{
				StructDef(T->ptr[0]);
				break;
			}
			default:
				break;
		}
		T=T->ptr[1];

	}
	dispsymbol();
	root=merge(2,root,genIR(END_IR,op1,op2,result));
	if(NoIRgenerate==0)
    {
        displayIR(root);
        putchar('\n');
        //DAGoptimize();
        //displayIR(target);
        generateTargetCode(root);
    }

    else
        printf("Semantic error occured, please check again\n");
}

/*���������鲻�ܺͳ���Ա��������������κ��ⲿ��������*/
void VarDef(struct node *T)
{
	int type;
	struct node *temp;
	int basewidth;

	if(!strcmp(T->ptr[0]->type_id,"int"))
		type=0;
	else if(!strcmp(T->ptr[0]->type_id,"char"))
		type=1;
	else if(!strcmp(T->ptr[0]->type_id,"float"))
		type=2;

    if(type!=0)
    {
        errormsg(0,T->pos,T->ptr[0]->type_id,"Function type should be int, temporarily");
    }
    basewidth=4;//Ĭ��Ϊint

    int dim=basewidth;

	while(T->ptr[1])
	{
		T=T->ptr[1];
		if(T->ptr[0]->kind==VARDEC)
		{
			temp=T->ptr[0];
			if(redec(temp->ptr[0]->type_id,'V',0))//���ṹ��Ա�����ⲻ���б�������
			{
			    temp->ptr[0]->width=basewidth;
                temp->ptr[0]->offset=globaloff;
                globaloff+=basewidth;
				fillSymbolTable(temp->ptr[0]->type_id,createAlias(),LV,type,'V',0,temp->ptr[0]->offset);
				if(temp->ptr[1])
                {//�������ݵ����ƣ����ڱ����ĳ�ʼ�������ڲ�ֱ������Ŀ����룬����д���ڴ�������޷�ֱ�ӹ��ɡ��ⲿ�����ĳ�ʼ����Ҫ��������
                    if(Exp(temp->ptr[1])!=0)//�����ս������ԵĹ淶��ֻҪ����int�ͱ��������
					{
						errormsg(0,temp->ptr[0]->pos,temp->ptr[0]->type_id,"Variable initializer types do not match or initializer element is not constant");
					}
                }
			}
			else
			{
				errormsg(1,temp->ptr[0]->pos,temp->ptr[0]->type_id,"Redefinition of variables");
			}
		}
		else//ARRDEC
		{
			temp=T->ptr[0];
			if(redec(temp->ptr[0]->type_id,'A',0))//���ṹ��Ա�����ⲻ���б�������
			{
			    struct node *temp1=temp->ptr[1];//ά��
			    temp->ptr[0]->offset=globaloff;
				fillSymbolTable(temp->ptr[0]->type_id,createAlias(),LV,type,'A',0,temp->ptr[0]->offset);
				symbolTable.symbols[symbolTable.index].paramnum=0;
				while(temp1)
                {
                    if(Exp(temp1->ptr[0])!=0)//һ��һ��������ά�ȣ�����Ϊ����
					{
						errormsg(2,temp->ptr[0]->pos,temp->ptr[0]->type_id,"Array length is not Int_Const");
					}
					else
                    {
                        dim=dim*getresult(temp1->ptr[0]);
                        symbolTable.symbols[symbolTable.index-1].paramnum++;//ά����¼����
                    }
                    temp1=temp1->ptr[1];
                    if(temp1!=NULL)
                    {
                        errormsg(0,temp->ptr[0]->pos,temp->ptr[0]->type_id,"Array dimension just support 1 dim, temporarily");
                    }
                }
				/*�˴��������ʼ����������Ϊ��Ҫ�Բ���̫�����ˣ������������������*/
				/*�����������ʼ��ȡ��*/
			}
			else
			{
				errormsg(1,temp->ptr[0]->pos,temp->ptr[0]->type_id,"Redefinition of arrays");
			}
			temp->ptr[0]->width=dim;
			globaloff+=dim;
		}
	}

}
/*����ԭ�Ͳ��ܺͳ���Ա�����Ͳ����ͺ��������ͺ���ԭ����ı�������*/
void FuncDec(struct node *T)
{
	int type;
	struct node *temp;
	struct node *ttemp;

	int i,j;

	if(!strcmp(T->ptr[0]->type_id,"int"))
		type=0;
	else if(!strcmp(T->ptr[0]->type_id,"char"))
		type=1;
	else if(!strcmp(T->ptr[0]->type_id,"float"))
		type=2;
	else if(!strcmp(T->ptr[0]->type_id,"void"))
		type=3;

    if(type==1||type==2)
    {
        errormsg(0,T->pos,T->ptr[0]->type_id,"Function type should not be char or float, temporarily");
    }

	temp=T->ptr[1];//ָ��ָ��funcdef

	if(redec(temp->ptr[0]->type_id,'D',0))
	{
		symbolTable.symbols[symbolTable.index].paramnum=0;//��ʱ��Ӧ�ڷ��ű�ļ���д���λ�ã������������дΪ0
		i=symbolTable.index;
		fillSymbolTable(temp->ptr[0]->type_id,NULL,LV,type,'D',0,0);//����ű��Ժ���ԭ�ͣ�ֻ����������׶������ã�����������
		ttemp=temp->ptr[1];//ָ��paralist
		int ttype;
		while(ttemp)//�����б�
		{
			if(redec(ttemp->ptr[0]->ptr[1]->type_id,'P',i))
			{
				if(!strcmp(ttemp->ptr[0]->ptr[0]->type_id,"int"))
					ttype=0;
				else if(!strcmp(ttemp->ptr[0]->ptr[0]->type_id,"char"))
					ttype=1;
				else if(!strcmp(ttemp->ptr[0]->ptr[0]->type_id,"float"))
					ttype=2;

                if(ttype!=0)
                {
                    errormsg(0,ttemp->ptr[0]->ptr[1]->pos,ttemp->ptr[0]->ptr[1]->type_id,"Function param type should be int, temporarily");
                }

				if(ttemp->ptr[0]->ptr[1]->kind==VARNAME)
				{
					fillSymbolTable(ttemp->ptr[0]->ptr[1]->type_id,NULL,LV+1,ttype,'P',0,0);//����ԭ�Ϳ��Բ��ò����м��������
					symbolTable.symbols[symbolTable.index-1].paramnum=i;
					symbolTable.symbols[i].paramnum++;//ͳ�Ʋ�������
				}
				else
				{
					//�˴�Ϊ����ʱ��BUG��Ĭ�϶�����ʵ�Ǻ��������б���֧������
					errormsg(3,ttemp->ptr[1]->pos,ttemp->ptr[0]->ptr[1]->type_id,"Function Paramater do not support array, temporarily");
					/*
					symbolTable.symbols[symboltable.index].paramnum=1;//��������ֻ֧��һά����
					fillSymbolTable(ttemp->ptr[1]->type_id,LV,ttype,'P');

					if(Exp(ttemp->ptr[2]))//������ʽû������
					{
						//û����
					}
					else
					{
						//���������б���ʽ������
					}




					*/
				}
			}
			else
			{
				errormsg(4,temp->ptr[0]->pos,ttemp->ptr[0]->ptr[1]->type_id,"Redeclaration of function paramaters");
			}
			ttemp=ttemp->ptr[1];
		}
		j=searchTarget(temp->ptr[0]->type_id,'F');
		if(j!=-1)
        {
            switch(parammatch(i,j))
            {
                case 0:
                    break;
                case 1:
                    errormsg(5,T->pos,temp->ptr[0]->type_id,"Funcdec and funcdef's type are not match");
                    break;
                case 2:
                    errormsg(6,T->pos,temp->ptr[0]->type_id,"Funcdec and funcdef's paramater num are not match");
                    break;
                case 3:
                    errormsg(7,T->pos,temp->ptr[0]->type_id,"Funcdec and funcdef's paramater type are not match");
                    break;
                default:
                    break;
            }
        }
	}
	else
	{
		errormsg(8,temp->ptr[0]->pos,temp->ptr[0]->type_id,"Redefinition of functions");
	}

}

void FuncDef(struct node *T)
{
	int type;
	struct node *temp;
	struct node *ttemp;
	int i,j;
    struct codenode* tempnode=NULL;
    nowfuncparanum=0;

	if(!strcmp(T->ptr[0]->type_id,"int"))
		type=0;
	else if(!strcmp(T->ptr[0]->type_id,"char"))
		type=1;
	else if(!strcmp(T->ptr[0]->type_id,"float"))
		type=2;
	else if(!strcmp(T->ptr[0]->type_id,"void"))
		type=3;

    if(type!=0)
    {
        errormsg(0,T->pos,temp->ptr[0]->type_id,"Function type should be int, temporarily");
    }

	functype=type;

	temp=T->ptr[1];//ָ��ָ��funchead

	if(redec(temp->ptr[0]->type_id,'F',0))
	{
		symbolTable.symbols[symbolTable.index].paramnum=0;//��ʱ��Ӧ�ڷ��ű�ļ���д���λ�ã������������дΪ0
		i=symbolTable.index;
		fillSymbolTable(temp->ptr[0]->type_id,NULL,LV,type,'D',0,0);//����ű�
		ttemp=temp->ptr[1];//ָ��paralist
		int ttype;

		int baseoffset=8;//����ĳ�ʼֵ���Ǻ�����������ں�����ջ֡��ƫ�������������Ӧ�������������Ϊ����Ҳ�ɣ�
		//���⣬���滹�з��ص�ַ
		//��Ϊջ���һ���洢���Ƿ��ص�ַ

		while(ttemp)//�����б�
		{
			if(redec(ttemp->ptr[0]->ptr[1]->type_id,'P',i))
			{
				if(!strcmp(ttemp->ptr[0]->ptr[0]->type_id,"int"))
					ttype=0;
				else if(!strcmp(ttemp->ptr[0]->ptr[0]->type_id,"char"))
					ttype=1;
				else if(!strcmp(ttemp->ptr[0]->ptr[0]->type_id,"float"))
					ttype=2;

                if(ttype!=0)
                {
                    errormsg(0,ttemp->ptr[0]->ptr[1]->pos,ttemp->ptr[0]->ptr[1]->type_id,"Function param type should be int, temporarily");
                }

				if(ttemp->ptr[0]->ptr[1]->kind==VARNAME)
				{
				    ttemp->ptr[0]->ptr[1]->offset=baseoffset;
				    //�Ժ��������Ĵ�����һ�����У�����������ƫ����������ں���ͷ�ļ��º�����ջ֡��ƫ����
				    //��Ҫ���Ϸ��ص�ַ��ƫ�Ƶ�ַ��Ϊ����mips32��ʹ�ã�int�����ݵĺ㶨ֵ����4
				    fillSymbolTable(ttemp->ptr[0]->ptr[1]->type_id,createAlias(),LV+1,ttype,'P',1,ttemp->ptr[0]->ptr[1]->offset);
				    //�˴��������βε����ֲ�������������Ϊ1
					symbolTable.symbols[symbolTable.index-1].paramnum=i;

					symbolTable.symbols[i].paramnum++;//ͳ�Ʋ�������
					nowfuncparanum++;//ͳ�Ʋ����������������м������
					ttemp->ptr[0]->ptr[1]->width=4;//�������Ⱥ㶨Ϊsizeof(int);
				    result.kind=ID_IR;
                    strcpy(result.id, symbolTable.symbols[symbolTable.index-1].alias);
                    symbolTable.symbols[symbolTable.index-1].offset=-baseoffset;
                    result.offset=-baseoffset;
                    result.level=LV+1;//������Ϊ�ֲ�����
				    ttemp->ptr[0]->ptr[1]->code=genIR(PARAM_IR,op1,op2,result);
				    tempnode=merge(2,tempnode,ttemp->ptr[0]->ptr[1]->code);
				    baseoffset+=4;
				}
				else
				{
					//�˴�Ϊ����ʱ��BUG��Ĭ�϶�����ʵ�Ǻ��������б���֧������
					errormsg(3,ttemp->ptr[1]->pos,ttemp->ptr[0]->ptr[1]->type_id,"Function Paramater do not support array, temporarily");
					/*
					fillSymbolTable(ttemp->ptr[1]->type_id,LV,ttype,'P');
					symbolTable.symbols[symboltable.index-1].paramnum=1;//��������ֻ֧��һά����
					if(Exp(ttemp->ptr[2]))//������ʽû������
					{
						//û����
					}
					else
					{
						//���������б���ʽ������
					}
					*/
				}
			}
			else
			{
				errormsg(4,temp->ptr[0]->pos,ttemp->ptr[0]->ptr[1]->type_id,"Redeclaration of function paramaters");
			}
			ttemp=ttemp->ptr[1];
		}

		j=searchTarget(temp->ptr[0]->type_id,'D');
		if(j!=-1)
        {
            switch(parammatch(i,j))
            {
                case 0:
                    break;
                case 1:
                    errormsg(5,T->pos,temp->ptr[0]->type_id,"Funcdec and funcdef's type are not match");
                    break;
                case 2:
                    errormsg(6,T->pos,temp->ptr[0]->type_id,"Funcdec and funcdef's paramater num are not match");
                    break;
                case 3:
                    errormsg(7,T->pos,temp->ptr[0]->type_id,"Funcdec and funcdef's paramater type are not match");
                    break;
                default:
                    break;
            }
        }

	}
	else
	{
		errormsg(8,temp->ptr[0]->pos,temp->ptr[0]->type_id,"Redefinition of functions");
	}
	localoff=4;
	funcwidth=0;
	ifreturn=0;
	nowfunc=i;
	LV++;
	CompoundStat(T->ptr[2]);//��鸴�����
	LV--;
	if(ifreturn!=1)
    {
        errormsg(9,temp->ptr[0]->pos,temp->ptr[0]->type_id,"No return statment of non-void function");
    }
	nowfunc=-1;
	T->width=funcwidth;
	strcpy(result.id,temp->ptr[0]->type_id);//������
	result.offset=funcwidth;
	T->code=genIR(FUNC_IR,op1,op2,result);//FUNCTION HEAD
	T->code=merge(2,T->code,tempnode);//PARAM
	T->code=merge(2,T->code,T->ptr[2]->code);//Compound Stat
}

void StructDec(struct node *T)
{
	struct node *temp;
	int i;
	int master;//�ṹ���Ӧ�ڷ��ű�Ľڵ㣬���ں����жϳ�Ա�������ظ�


	if(redec(T->type_id,'S',0))
	{
		master=fillSymbolTable(T->type_id,NULL,LV,0,'S',0,0);//����ű�
		//�����Ա�����б�
		temp=T->ptr[0];//ָ���Ա�б�
		int type;
		while(temp)
		{
			if(!strcmp(temp->ptr[0]->type_id,"int"))
				type=0;
			else if(!strcmp(temp->ptr[0]->type_id,"char"))
				type=1;
			else if(!strcmp(temp->ptr[0]->type_id,"float"))
				type=2;

            if(type!=0)
            {
                errormsg(0,temp->ptr[0]->pos,temp->ptr[0]->type_id,"Struct member type should be int, temporarily");
            }

			struct node *ttemp;
			int number;

			int baseoffset=0;

			ttemp=temp->ptr[1];
			while(ttemp)
			{
				if(redec(ttemp->type_id,'N',master))
				{
				    ttemp->offset=baseoffset;//ƫ����
				    ttemp->width=4;//int���ͳ���
				    symbolTable.symbols[master].offset+=ttemp->width;//ͳ�ƽṹ�峤��
				    fillSymbolTable(ttemp->type_id,NULL,LV,type,'N',0,baseoffset);
				    symbolTable.symbols[symbolTable.index-1].paramnum=master;
				    baseoffset+=4;
				}
				else
				{
					//���ִ��󣬳�Ա��������
					errormsg(10,ttemp->pos,temp->ptr[0]->type_id,"Redeclaration of member variables");
				}
				ttemp=ttemp->ptr[0];
			}
			temp=temp->ptr[2];
		}

	}
	else
	{
		errormsg(11,T->pos,T->type_id,"Redeclaration of struct");
	}

}

void StructDef(struct node *T)
{
	struct node *temp;
	int i;
	int master;//�ṹ��������Ӧ�ڷ��ű�Ľڵ㣬ָ���ṹ������Ӧ�Ľṹ��

	master=searchTarget(T->type_id,'S');//�ҽṹ������ֻ���ǽṹ����
	if(master>=0)
	{
		temp=T->ptr[0];
		while(temp)
		{
			if(redec(temp->type_id,'B',0))
			{
			    temp->offset=globaloff;
				fillSymbolTable(temp->type_id,createAlias(),LV,-1,'B',0,temp->offset);
				//�ո�д��ķ��ţ���Ӧ�Ľṹ���������ű��λ��
				symbolTable.symbols[symbolTable.index-1].paramnum=master;
				globaloff+=4;
			}
			else
			{
				errormsg(12,temp->pos,temp->type_id,"Redefinition of struct variables");
			}
			temp=temp->ptr[0];
		}
	}
	else
	{
		errormsg(13,T->pos,T->type_id,"No such declaration of structs");
	}
}


/*���븴������¼����ʱ���ű�ļ�¼��������ʱ��ȫ��ɾ��*/
void CompoundStat(struct node *T)
{
	int record=symbolTable.index;//��¼��ʱ���ű��β��λ��
	struct node *temp=T->ptr[0];//ָ��statlist��һ��
	while(temp)
	{
		switch(temp->ptr[0]->kind)
		{
			case LOCALVAR:
			case LOCALSTRUCTDEF:
				LocalVar(temp->ptr[0]);
				T->code=merge(2,T->code,temp->ptr[0]->code);
				break;
			default:
			{
				Stat(temp->ptr[0]);
				T->code=merge(2,T->code,temp->ptr[0]->code);//������Ӧ���м�������ӵ����������
				break;
			}
		}
		temp=temp->ptr[1];
	}
	dispsymbol();
	//�˳��������ʱ���ű�Ҫ��Ӧ��ȥ��
	symbolTable.index=record;

}

void Stat(struct node *T)
{
	switch(T->kind)
	{
	    /*���ʽ�����м�������з��ڱ��ʽ�﷨���ĸ���*/
		case EXP:
		    {
                if(Exp(T->ptr[0])==-1)//EXP�����������ʽ�Ĺ淶��Ӧ���ǴӶ�Ӧ��������ڵ㿪ʼ������������ʵ��һ�����һ��ʧ������
                    errormsg(14,T->pos,"Expression statement","Semantic error");
                T->code=merge(2,T->code,T->ptr[0]->code);
                //EXP����Ҫע���޸�һ�£��ؼ��Ƿ���ֵ�Ĺ淶��
                //EXP���м���룬��EXP��������������

                break;
		    }
        /*���������м�������з��ڸ������ĸ���*/
		case COMPSTAT://��������м����
		    {
                LV++;
                CompoundStat(T);
                LV--;
                break;
		    }
        /*return�����м�������з���RETURN�ڵ���*/
		case RETURN://�ڴ��жϺ���ƥ�����⣬
		{

			if(T->ptr[0])
			{
			    ifreturn=1;
				if(Exp(T->ptr[0])%3!=functype)
				{
					errormsg(15,T->pos,"return","values do not match function type");
				}
				result.kind=ID_IR;
                strcpy(result.id,symbolTable.symbols[expresultindex].alias);
                T->code=merge(2,T->code,T->ptr[0]->code);
			}
			else
			{
				if(functype!=-2)//����ǰ��������ֵ���Ͳ�Ϊvoid
				{
					errormsg(15,T->pos,"return","values do not match function type");
				}
				result.kind=0;
			}


            result.offset=nowfuncparanum*4;
            /*ʵ�������ƣ����к�������ֵ��ΪInt����ˣ��ⷽ�������˼򻯣�ֱ�ӽ�ƫ����д��result�ڵ�*/

            /*�˴���Ҫ�޸ģ���expresultindex��ʶ�ľ��Ǳ��ʽ��ֵ��Ӧ����ʱ��������˶�������û��ҪŪһ����ֵ���*/
            /*��ʽreturn result��return (result.kind==0)*/
            T->code=merge(2,T->code,genIR(RETURN_IR,op1,op2,result));

			break;
		}

		/*if�����м�������з���IF��*/
		/*���ڱ��ʽ������һ���µı������ж��Ƿ����0*/
		/*if���ṹ��exp�м�������У�truelable��true��䣨������䣩���У�falsetable������ѡ��false��䣨����������У����������*/
		case IF:
		    {
                if(Exp(T->ptr[0])<0)
                    errormsg(14,T->pos,"if","Condition expression semantic error");
                int result1=expresultindex;
                Stat(T->ptr[1]);
                if(T->ptr[2])
                    Stat(T->ptr[2]);

                /*if���Ĵ�����Ҫ�������ȷ�ʹ���ı�ţ�*/
                strcpy(T->Etrue,createLabel());  //����Ϊ�����תλ�ã���ʵû��ɶ�ã������൱��һ���洢��ŵ���ʱ����
                strcpy(T->Efalse,createLabel());//����Ϊ�ٵ���תλ��

                T->code=T->ptr[0]->code;//���Ǳ��ʽ�Ĵ���

                /*if���ڱȽϱ��ʽ��ֵ�Ƿ�Ϊ0����0��ת��false����1��ת��true*/
                /*��ñ��ʽ��ֵ��ǰ���й�*/

                result.kind=ID_IR;
                strcpy(result.id,symbolTable.symbols[result1].alias);

                op1.kind=ID_IR;
                strcpy(op1.id,T->Efalse);
                struct codenode* tempnode=genIR(IF_IR,op1,op2,result);
                T->code=merge(2,T->code,tempnode);//if result==0 goto op1
                //IF�����жϵ��м���룬��Ϊresult��Ӧ�ı����Ƿ����0Ȼ����ת��op1����op2
                //�����Ĵ�������ָ������ȫ��ͬ��ԭ���ж�����һ�Ƕ���C�����Ӽ�����Ĳ�ͬ������Ƕ�������if(a+2)�����ı��ʽ�����ò������ʽ������ȫ����
                /*��true��ǩ��true������У�false��ǩ��false������У���β��ǩ*/

                T->code=merge(2,T->code,T->ptr[1]->code);//true�������
                tempnode->next->flag='H';//����ת���������һ�����
                char endlable[10];

                strcpy(endlable,createLabel());

                T->code=merge(2,T->code,genGoto(endlable));

                T->code=merge(2,T->code,genLabel(T->Efalse));

                if(T->ptr[2])
                {
                    //false�������
                    T->code=merge(2,T->code,T->ptr[2]->code);
                }
                T->code=merge(2,T->code,genLabel(endlable));

                break;

		    }
        /*while�����м�������з���WHILE��*/
        /*���ڱ��ʽ������һ���µı������ж��Ƿ����0*/
		/*while���ṹ����ʼlabel��exp�м�������У�truelable��true��䣨������䣩���У�falsetable���������*/
		case WHILE:
		    {
                ifincir++;
                if(Exp(T->ptr[0])<0)
                    errormsg(14,T->pos,"while","Condition expression semantic error");
                int result1=expresultindex;
                Stat(T->ptr[1]);
                ifincir--;

                strcpy(T->Etrue,createLabel());
                strcpy(T->Efalse,createLabel());

                char startlable[10];
                strcpy(startlable,createLabel());


                T->code=genLabel(startlable);//while�Ŀ�ʼ��ǩ

                T->code=merge(2,T->code,T->ptr[0]->code);

                result.kind=ID_IR;
                strcpy(result.id,symbolTable.symbols[result1].alias);//�Ƚ�ֵ

                op1.kind=ID_IR;
                strcpy(op1.id,T->Efalse);//�٣�������
                struct codenode* tempnode=genIR(IF_IR,op1,op2,result);
                T->code=merge(2,T->code,tempnode);//�����ж�

                T->code=merge(2,T->code,T->ptr[1]->code);//�������
                tempnode->next->flag='H';
                T->code=merge(2,T->code,genGoto(startlable));//��ת�������ж�

                T->code=merge(2,T->code,genLabel(T->Efalse));//����λ��

                break;
		    }
        /*break�����м�������з���BREAK��*/
		case BREAK:
		    {
                if(ifincir==0)
                    errormsg(15,T->pos,"break","Not in circle");
                struct node* temp=T;
                //break���Ĵ���������Ѱ��while����for����Efalse��label��д��goto���ɡ�
                while(temp->parent->kind!=WHILE&&temp->parent->kind!=FOR)
                {
                    temp=temp->parent;
                }

                //tempָ�����break�����ѭ���ڵ�
                //������ת��������goto��伴��

                T->code=genGoto(temp->Efalse);


                break;
		    }
        /*continue�����м�������з���CONTINUE��*/
		case CONTINUE:
		    {
                if(ifincir==0)
                    errormsg(16,T->pos,"continue","Not in circle");

                struct node* temp=T;
                //continue���Ĵ���������Ѱ��while����for���Ŀ�ͷ��label��д��goto���ɡ�
                while(temp->parent->kind!=WHILE&&temp->parent->kind!=FOR)
                {
                    temp=temp->parent;
                }

                //tempָ�����for�����ѭ���ڵ�
                //������ת����ʼ��goto��伴��
                char startlable[10];
                //ѭ�����Ľڵ��Ӧ�ĵ�һ���м����һ���Ǳ��
                strcpy(startlable,temp->code->result.id);
                T->code=genGoto(startlable);
                break;
		    }
        /*for�����м�������з���FOR��*/
		case FOR:
		{
			ifincir++;
			int exporvar=0;
			int record=symbolTable.index;
			if(T->ptr[0]->ptr[0]->kind==DATATYPE)
			{
				//for�ھֲ�����
				struct node *Temp=T->ptr[0];
				int type;
				struct node *temp;

                int basewidth=0;

				if(!strcmp(Temp->ptr[0]->type_id,"int"))
					type=0;
				else if(!strcmp(Temp->ptr[0]->type_id,"char"))
					type=1;
				else if(!strcmp(Temp->ptr[0]->type_id,"float"))
					type=2;

                basewidth=4;

                if(type!=0)
                {
                    errormsg(0,T->pos,temp->ptr[0]->type_id,"Struct member type should be int, temporarily");
                }

				while(Temp->ptr[1])
				{
					Temp=Temp->ptr[1];
					if(Temp->ptr[0]->kind==VARDEC)
					{
						int i;
						temp=Temp->ptr[0];

						if(localRedec(temp->ptr[0]->type_id,LV+1))//���ṹ��Ա�����ⲻ���б�������
						{
							//for�ھֲ�������Ӧ���ǲ��+1��������ɼ��ݶ�Ӧ�ڸ����������
							temp->ptr[0]->width=basewidth;
                            funcwidth+=basewidth;
                            temp->ptr[0]->offset=localoff;
                            localoff+=basewidth;
							i=fillSymbolTable(temp->ptr[0]->type_id,createAlias(),LV+1,type,'V',1,temp->ptr[0]->offset);
							if(temp->ptr[1])
							{
								if(Exp(temp->ptr[1])%3!=type)
									errormsg(0,temp->ptr[0]->pos,temp->ptr[0]->type_id,"Variable initialization types do not match");
                                T->code=merge(2,T->code,temp->ptr[1]->code);
                                int result1=expresultindex;
                                op2.kind=ID_IR;
                                strcpy(op2.id,symbolTable.symbols[result1].alias);

                                result.kind=ID_IR;
                                strcpy(result.id,symbolTable.symbols[i].alias);
                                result.offset=temp->ptr[0]->offset;
                                result.level=LV;
                                //�ֲ�������ʼ���ĸ�ֵ��result(var)=op2(temp)��Ӧ��Ӧ��һ��storeָ��
                                T->code=merge(2,T->code,genIR(ASSIGN,op1,op2,result));
                                //for1Ϊ�ֲ�����
							}
						}
						else
						{
							errormsg(1,temp->ptr[0]->pos,temp->ptr[0]->type_id,"Redefinition of variables");
						}
					}
					else//ARRDEC
					{
						errormsg(17,T->pos,"for","Not support array definition");
						/*
						int i;
						temp=Temp->ptr[0];
						if(redec(temp->ptr[0]->type_id,'A',0))//���ṹ��Ա�����ⲻ���б�������
						{
							struct node *temp1=temp->ptr[1];//ά��
							fillSymbolTable(temp->ptr[0]->type_id,LV,type,'A',1);
							symbolTable.symbols[symboltable.index].paramnum=0;
							while(temp1)
							{
								if(Exp(temp1->ptr[0])!=1)//һ��һ��������ά�ȣ�����Ϊ����
								{
									errormsg(temp->ptr[0]->pos,temp->ptr[0]->type_id,"���鳤�Ȳ�Ϊ������");
								}
								temp1=temp1->ptr[1];
								symbolTable.symbols[symboltable.index].paramnum++;//ά����¼����
							}
						}
						else
						{
							errormsg(temp->ptr[0]->pos,temp->ptr[0]->type_id,"����������ظ�");
						}*/
					}
				}
			}
			else
			{
				if(Exp(T->ptr[0]->ptr[0])<0)
					errormsg(14,T->pos,"for","Condition Expression 1 semantic error");
                T->code=T->ptr[0]->code;//for1Ϊ���ʽ
			}

			if(Exp(T->ptr[1]->ptr[0])<0)
				errormsg(14,T->pos,"for","Condition Expression 2 semantic error");
            int result1=expresultindex;
			if(Exp(T->ptr[2]->ptr[0])<0)
				errormsg(14,T->pos,"for","Condition Expression 3 semantic error");
			Stat(T->ptr[3]);

			strcpy(T->Etrue,createLabel());
            strcpy(T->Efalse,createLabel());

            char startlable[10];
            strcpy(startlable,createLabel());




            T->code=merge(2,T->code,genLabel(startlable));//for�������жϵĿ�ʼ��ǩ

            T->code=merge(2,T->code,T->ptr[1]->ptr[0]->code);

            result.kind=ID_IR;
            strcpy(result.id,symbolTable.symbols[result1].alias);//�Ƚ�ֵ

            op1.kind=ID_IR;
            strcpy(op1.id,T->Efalse);//�٣�������
            struct codenode* tempnode=genIR(IF_IR,op1,op2,result);
            T->code=merge(2,T->code,tempnode);//�����ж�

            T->code=merge(2,T->code,genLabel(T->Etrue));//��ȷ��ǩ
            tempnode->next->flag='H';
            T->code=merge(2,T->code,T->ptr[3]->code);//�������

            T->code=merge(2,T->code,T->ptr[2]->ptr[0]->code);//for3

            T->code=merge(2,T->code,genGoto(startlable));

            T->code=merge(2,T->code,genLabel(T->Efalse));//����λ��

			//�������ޣ��ͷžֲ�����
			symbolTable.index=record;
			ifincir--;
			break;
		}
		default:
			break;
	}
}


/*�����ֲ������ĳ����ܺ�Ӧ��д���Ӧ������ƫ��ֵ��λ�ã��������ʱʹ��*/
/*�����ֲ��������ȵļ��㣬Ӧ��д���ܺ�*/
void LocalVar(struct node *T)
{
    struct node* Troot=T;//�˴�Ϊһ��ʧ�󣬰Ѻ�������ֵ�ı���
	if(T->kind==LOCALVAR)
	{
		int type;
		struct node *temp;
		int basewidth=0;
		int dim=1;

		if(!strcmp(T->ptr[0]->type_id,"int"))
			type=0;
		else if(!strcmp(T->ptr[0]->type_id,"char"))
			type=1;
		else if(!strcmp(T->ptr[0]->type_id,"float"))
			type=2;

        basewidth=4;

        if(type!=0)
        {
            errormsg(0,T->pos,temp->ptr[0]->type_id,"Local variable type should be int, temporarily");
        }

        int i=0;

		while(T->ptr[1])
		{
			T=T->ptr[1];
			if(T->ptr[0]->kind==VARDEC)
			{
				temp=T->ptr[0];
				if(localRedec(temp->ptr[0]->type_id,LV))//���ṹ��Ա�����ⲻ���б�������
				{
				    temp->ptr[0]->width=basewidth;
				    funcwidth+=basewidth;
                    temp->ptr[0]->offset=localoff;
                    localoff+=basewidth;
					i=fillSymbolTable(temp->ptr[0]->type_id,createAlias(),LV,type,'V',1,temp->ptr[0]->offset);
					if(temp->ptr[1])
					{
						if(Exp(temp->ptr[1])%3!=type)
							errormsg(0,temp->ptr[0]->pos,temp->ptr[0]->type_id,"Variable initialization types do not match");

                        Troot->code=merge(2,Troot->code,temp->ptr[1]->code);
                        int result1=expresultindex;
                        op2.kind=ID_IR;
                        strcpy(op2.id,symbolTable.symbols[result1].alias);

                        result.kind=ID_IR;
                        strcpy(result.id,symbolTable.symbols[i].alias);
                        result.offset=temp->ptr[0]->offset;
                        result.level=LV;
                        //�ֲ�������ʼ���ĸ�ֵ��result(var)=op2(temp)��Ӧ��Ӧ��һ��storeָ��
                        Troot->code=merge(2,Troot->code,genIR(ASSIGN,op1,op2,result));
					}
				}
				else
				{
					errormsg(1,temp->ptr[0]->pos,temp->ptr[0]->type_id,"Redefinition of variables");
				}
			}
			else//ARRDEC
			{
				temp=T->ptr[0];
				if(localRedec(temp->ptr[0]->type_id,LV))//���ṹ��Ա�����ⲻ���б�������
				{
				    int index=0;
					struct node *temp1=temp->ptr[1];//ά��
					temp->ptr[0]->offset=localoff;
					index=fillSymbolTable(temp->ptr[0]->type_id,createAlias(),LV,type,'A',1,temp->ptr[0]->offset);
					symbolTable.symbols[index].paramnum=0;
					while(temp1)
					{
						if(Exp(temp1->ptr[0])!=0)//һ��һ��������ά�ȣ�����Ϊ����
						{
							errormsg(2,temp->ptr[0]->pos,temp->ptr[0]->type_id,"Array length is not Int_Const");
						}
						else
                        {
                            dim=dim*getresult(temp1->ptr[0]);
                            symbolTable.symbols[index].paramnum++;//ά����¼����
                        }
						temp1=temp1->ptr[1];
                        if(temp1!=NULL)
                        {
                            errormsg(0,temp->ptr[0]->pos,temp->ptr[0]->type_id,"Array dimension just support 1 dim, temporarily");
                        }
					}
					/*�˴�����ʽ��ʼ��*/
				}
				else
				{
					errormsg(1,temp->ptr[0]->pos,temp->ptr[0]->type_id,"Redefinition of arrays");
				}
                temp->ptr[0]->width=dim;
                funcwidth+=dim;
                localoff+=dim;
			}
		}
	}
	else
	{
		struct node *temp;
		int master;//�ṹ��������Ӧ�ڷ��ű�Ľڵ㣬ָ���ṹ������Ӧ�Ľṹ��

		master=searchTarget(T->type_id,'S');//�ҽṹ������ֻ���ǽṹ����
		if(master)
		{
			temp=T->ptr[0];
			while(temp)
			{
				if(localRedec(temp->type_id,LV))
				{
				    temp->offset=localoff;
				    temp->width=symbolTable.symbols[master].offset;
				    funcwidth+=temp->width;
					fillSymbolTable(temp->type_id,createAlias(),LV,-1,'B',1,temp->offset);
					//�ո�д��ķ��ţ���Ӧ�Ľṹ���������ű��λ��
					symbolTable.symbols[symbolTable.index-1].paramnum=master;
					localoff+=temp->width;//ƫ��������
				}
				else
				{
					errormsg(12,temp->pos,temp->type_id,"Redefinition of struct variables");
				}
				temp=temp->ptr[0];
			}
		}
		else
		{
			errormsg(13,T->pos,T->type_id,"No such declaration of structs");
		}
	}
}

/*���ؼ������� -2:void -1:error 0:int const 1:char const 2:float const 3:int var 4:char var 5:float var*/
/*���ʽ���ؽ�����жϣ�������ֵ���ʽ������<0����ʾ�����ˣ���Ϊvoid���Ͳ�����Ϊֵ������Stat��Expֻ����-1ʱ����*/
/*���ʽ�����࣬��һ���Ǳ������ü����㣬������ʽ�Ľ����д��ĳ�Ĵ�������ĳ��ʱ�������ڣ� �����3+4����û�и�ֵҲҪд��Ĵ���*/
/*�ڶ����ǳ������ã�������д��Ĵ����ڣ�������д�������addi*/
/*�������Ǻ������ã����ؽ��д���Ӧ�Ĵ�ֵ�Ĵ�������Ҳ��д��Ĵ��������ǲ���д��ͨ�üĴ���*/
int Exp(struct node* T)
{
	switch(T->kind)
	{
		//��ֵ������������ǵ���һ��varcall��arraycall��Ϊ�������ʵ�黹�ǽ���Щ�ֿ�дΪ��
		case ASSIGN:
		{
			if(T->ptr[0]->kind!=VARCALL&&T->ptr[0]->kind!=ARRAYCALL&&T->ptr[0]->kind!=STRUCTCALL)
			{
				errormsg(18,T->pos,"Assign","Inappropriate lvalue");
				return -1;
			}
			int lvalue=Exp(T->ptr[0]);
			int result1=expresultindex;
			int rvalue=Exp(T->ptr[1]);//��ֵ��������ֵΪ
			int result2=expresultindex;
			if(lvalue<0)
			{
				errormsg(19,T->pos,"Assign","lvalue error");
				return -1;
			}
			if(rvalue<0)
			{
				errormsg(20,T->pos,"Assign","rvalue error");
				return -1;
			}
			if(lvalue%3!=rvalue%3)
			{
				errormsg(21,T->pos,"Assign","lvalue and rvalue do not match");
				return -1;
			}

			T->code=merge(2,T->code,T->ptr[0]->code);
			T->code=merge(2,T->code,T->ptr[1]->code);

			//�洢����1
			op1.kind=ID_IR;
			strcpy(op1.id,symbolTable.symbols[result1].alias);
			op1.offset=symbolTable.symbols[result1].offset;
			op1.level=symbolTable.symbols[result1].level;
			op2.kind=ID_IR;
			strcpy(op2.id,symbolTable.symbols[result2].alias);

			char tempvar[10];
			strcpy(tempvar,createTemp());
			expresultindex=fillSymbolTable(NULL,tempvar,LV,0,'T',1,0);
			result.kind=ID_IR;
			strcpy(result.id,tempvar);
			T->code=merge(2,T->code,genIR(ASSIGN_IR,op1,op2,result));
			/*�������˼�ǣ�tempX=op1=op2*/
			/*tempX����ʵ�����壬���ǻ�������Ҫ�ģ���if���*/

			return lvalue;
			break;
		}
		case ASSIGN_PLUS:
		{
			if(T->ptr[0]->kind!=VARCALL&&T->ptr[0]->kind!=ARRAYCALL)
			{
				errormsg(18,T->pos,"Assign_plus","Inappropriate lvalue");
				return -1;
			}
			int lvalue=Exp(T->ptr[0]);
			int result1=expresultindex;
			int rvalue=Exp(T->ptr[1]);//��ֵ��������ֵΪ
			int result2=expresultindex;
			if(lvalue<0)
			{
				errormsg(19,T->pos,"Assign_plus","lvalue error");
				return -1;
			}
			if(rvalue<0)
			{
				errormsg(20,T->pos,"Assign_plus","rvalue error");
				return -1;
			}
			if(lvalue%3!=rvalue%3)
			{
				errormsg(21,T->pos,"Assign_plus","lvalue and rvalue do not match");
				return -1;
			}

			/*+=���м���룬��һ����ʱ�����洢�ӷ�������ڶ�����ʱ�����洢������*/

			//�����Ĵ���
			T->code=merge(2,T->code,T->ptr[0]->code);
			T->code=merge(2,T->code,T->ptr[1]->code);

			//����ӷ�����
			char tempvar[10];
			strcpy(tempvar,createTemp());
			fillSymbolTable(NULL,tempvar,LV,0,'T',1,0);
			result.kind=ID_IR;
			strcpy(result.id,tempvar);

			op1.kind=ID_IR;
			strcpy(op1.id,symbolTable.symbols[result1].alias);
			op1.offset=symbolTable.symbols[result1].offset;
			op1.level=symbolTable.symbols[result1].level;
			op2.kind=ID_IR;
			strcpy(op2.id,symbolTable.symbols[result2].alias);

			T->code=merge(2,T->code,genIR(PLUS,op1,op2,result));

			//����ֵ����
			op2.kind=ID_IR;
			strcpy(op2.id,result.id);

			strcpy(tempvar,createTemp());
			expresultindex=fillSymbolTable(NULL,tempvar,LV,0,'T',1,0);
			result.kind=ID_IR;
			strcpy(result.id,tempvar);
			T->code=merge(2,T->code,genIR(ASSIGN_IR,op1,op2,result));
			/*�������˼�ǣ�tempX=op1=op2*/
			/*tempX����ʵ�����壬���ǻ�������Ҫ�ģ���if���*/

			return lvalue;
			break;
		}
		case ASSIGN_MINUS:
		{
			if(T->ptr[0]->kind!=VARCALL&&T->ptr[0]->kind!=ARRAYCALL)
			{
				errormsg(18,T->pos,"Assign_minus","Inappropriate lvalue");
				return -1;
			}
			int lvalue=Exp(T->ptr[0]);
			int result1=expresultindex;
			int rvalue=Exp(T->ptr[1]);//��ֵ��������ֵΪ
			int result2=expresultindex;
			if(lvalue<0)
			{
				errormsg(19,T->pos,"Assign_minus","lvalue error");
				return -1;
			}
			if(rvalue<0)
			{
				errormsg(20,T->pos,"Assign_minus","rvalue error");
				return -1;
			}
			if(lvalue%3!=rvalue%3)
			{
				errormsg(21,T->pos,"Assign_minus","lvalue and rvalue do not match");
				return -1;
			}

			/*-=���м���룬��һ����ʱ�����洢�ӷ�������ڶ�����ʱ�����洢������*/

			//�����Ĵ���
			T->code=merge(2,T->code,T->ptr[0]->code);
			T->code=merge(2,T->code,T->ptr[1]->code);

			//����ӷ�����
			char tempvar[10];
			strcpy(tempvar,createTemp());
			fillSymbolTable(NULL,tempvar,LV,0,'T',1,0);
			result.kind=ID_IR;
			strcpy(result.id,tempvar);

			op1.kind=ID_IR;
			strcpy(op1.id,symbolTable.symbols[result1].alias);
			op1.offset=symbolTable.symbols[result1].offset;
			op1.level=symbolTable.symbols[result1].level;
			op2.kind=ID_IR;
			strcpy(op2.id,symbolTable.symbols[result2].alias);

			T->code=merge(2,T->code,genIR(MINUS,op1,op2,result));

			//����ֵ����
			op2.kind=ID_IR;
			strcpy(op2.id,result.id);

			strcpy(tempvar,createTemp());
			expresultindex=fillSymbolTable(NULL,tempvar,LV,0,'T',1,0);
			result.kind=ID_IR;
			strcpy(result.id,tempvar);
			T->code=merge(2,T->code,genIR(ASSIGN_IR,op1,op2,result));
			/*�������˼�ǣ�tempX=op1=op2*/
			/*tempX����ʵ�����壬���ǻ�������Ҫ�ģ���if���*/

			return lvalue;
			break;
		}
		case ASSIGN_MUL:
		{
			if(T->ptr[0]->kind!=VARCALL&&T->ptr[0]->kind!=ARRAYCALL)
			{
				errormsg(18,T->pos,"Assign_mul","Inappropriate lvalue");
				return -1;
			}
			int lvalue=Exp(T->ptr[0]);
			int result1=expresultindex;
			int rvalue=Exp(T->ptr[1]);//��ֵ��������ֵΪ
			int result2=expresultindex;
			if(lvalue<0)
			{
				errormsg(19,T->pos,"Assign_mul","lvalue error");
				return -1;
			}
			if(rvalue<0)
			{
				errormsg(20,T->pos,"Assign_mul","rvalue error");
				return -1;
			}
			if(lvalue%3!=rvalue%3)
			{
				errormsg(21,T->pos,"Assign_mul","lvalue and rvalue do not match");
				return -1;
			}

			/* *=���м���룬��һ����ʱ�����洢�ӷ�������ڶ�����ʱ�����洢������*/

			//�����Ĵ���
			T->code=merge(2,T->code,T->ptr[0]->code);
			T->code=merge(2,T->code,T->ptr[1]->code);

			//����ӷ�����
			char tempvar[10];
			strcpy(tempvar,createTemp());
			fillSymbolTable(NULL,tempvar,LV,0,'T',1,0);
			result.kind=ID_IR;
			strcpy(result.id,tempvar);

			op1.kind=ID_IR;
			strcpy(op1.id,symbolTable.symbols[result1].alias);
			op1.offset=symbolTable.symbols[result1].offset;
			op1.level=symbolTable.symbols[result1].level;
			op2.kind=ID_IR;
			strcpy(op2.id,symbolTable.symbols[result2].alias);

			T->code=merge(2,T->code,genIR(MUL,op1,op2,result));

			//����ֵ����
			op2.kind=ID_IR;
			strcpy(op2.id,result.id);

			strcpy(tempvar,createTemp());
			expresultindex=fillSymbolTable(NULL,tempvar,LV,0,'T',1,0);
			result.kind=ID_IR;
			strcpy(result.id,tempvar);
			T->code=merge(2,T->code,genIR(ASSIGN_IR,op1,op2,result));
			/*�������˼�ǣ�tempX=op1=op2*/
			/*tempX����ʵ�����壬���ǻ�������Ҫ�ģ���if���*/

			return lvalue;
			break;
		}
		case ASSIGN_DIV:
		{
			if(T->ptr[0]->kind!=VARCALL&&T->ptr[0]->kind!=ARRAYCALL)
			{
				errormsg(18,T->pos,"Assign_div","Inappropriate lvalue");
				return -1;
			}
			int lvalue=Exp(T->ptr[0]);
			int result1=expresultindex;
			int rvalue=Exp(T->ptr[1]);//��ֵ��������ֵΪ
			int result2=expresultindex;
			if(lvalue<0)
			{
				errormsg(19,T->pos,"Assign_div","lvalue error");
				return -1;
			}
			if(rvalue<0)
			{
				errormsg(20,T->pos,"Assign_div","rvalue error");
				return -1;
			}
			if(lvalue%3!=rvalue%3)
			{
				errormsg(21,T->pos,"Assign_div","lvalue and rvalue do not match");
				return -1;
			}

			/* /=���м���룬��һ����ʱ�����洢�ӷ�������ڶ�����ʱ�����洢������*/

			//�����Ĵ���
			T->code=merge(2,T->code,T->ptr[0]->code);
			T->code=merge(2,T->code,T->ptr[1]->code);

			//����ӷ�����
			char tempvar[10];
			strcpy(tempvar,createTemp());
			fillSymbolTable(NULL,tempvar,LV,0,'T',1,0);
			result.kind=ID_IR;
			strcpy(result.id,tempvar);

			op1.kind=ID_IR;
			strcpy(op1.id,symbolTable.symbols[result1].alias);
			op1.offset=symbolTable.symbols[result1].offset;
			op1.level=symbolTable.symbols[result1].level;
			op2.kind=ID_IR;
			strcpy(op2.id,symbolTable.symbols[result2].alias);

			T->code=merge(2,T->code,genIR(DIV,op1,op2,result));

			//����ֵ����
			op2.kind=ID_IR;
			strcpy(op2.id,result.id);

			strcpy(tempvar,createTemp());
			expresultindex=fillSymbolTable(NULL,tempvar,LV,0,'T',1,0);
			result.kind=ID_IR;
			strcpy(result.id,tempvar);
			T->code=merge(2,T->code,genIR(ASSIGN_IR,op1,op2,result));
			/*�������˼�ǣ�tempX=op1=op2*/
			/*tempX����ʵ�����壬���ǻ�������Ҫ�ģ���if���*/

			return lvalue;
			break;
		}
		case ASSIGN_MOD:
		{
			if(T->ptr[0]->kind!=VARCALL&&T->ptr[0]->kind!=ARRAYCALL)
			{
				errormsg(18,T->pos,"Assign_mod","Inappropriate lvalue");
				return -1;
			}
			int lvalue=Exp(T->ptr[0]);
			int result1=expresultindex;
			int rvalue=Exp(T->ptr[1]);//��ֵ��������ֵΪ
			int result2=expresultindex;

			if(lvalue<0||lvalue%3==2)
			{
				errormsg(19,T->pos,"Assign_mod","lvalue error");
				return -1;
			}
			if(rvalue<0||rvalue%3==2)
			{
				errormsg(20,T->pos,"Assign_mod","rvalue error");
				return -1;
			}
			if(lvalue%3!=rvalue%3)
			{
				errormsg(21,T->pos,"Assign_mod","lvalue and rvalue do not match");
				return -1;
			}

			/*%=���м���룬��һ����ʱ�����洢�ӷ�������ڶ�����ʱ�����洢������*/

			//�����Ĵ���
			T->code=merge(2,T->code,T->ptr[0]->code);
			T->code=merge(2,T->code,T->ptr[1]->code);

			//����ӷ�����
			char tempvar[10];
			strcpy(tempvar,createTemp());
			fillSymbolTable(NULL,tempvar,LV,0,'T',1,0);
			result.kind=ID_IR;
			strcpy(result.id,tempvar);

			op1.kind=ID_IR;
			strcpy(op1.id,symbolTable.symbols[result1].alias);
			op1.offset=symbolTable.symbols[result1].offset;
			op1.level=symbolTable.symbols[result1].level;
			op2.kind=ID_IR;
			strcpy(op2.id,symbolTable.symbols[result2].alias);

			T->code=merge(2,T->code,genIR(MOD,op1,op2,result));

			//����ֵ����
			op2.kind=ID_IR;
			strcpy(op2.id,result.id);

			strcpy(tempvar,createTemp());
			expresultindex=fillSymbolTable(NULL,tempvar,LV,0,'T',1,0);
			result.kind=ID_IR;
			strcpy(result.id,tempvar);
			T->code=merge(2,T->code,genIR(ASSIGN_IR,op1,op2,result));
			/*�������˼�ǣ�tempX=op1=op2*/
			/*tempX����ʵ�����壬���ǻ�������Ҫ�ģ���if���*/

			return lvalue;
			break;
		}
		case ASSIGN_LSHFIT:
		{
			if(T->ptr[0]->kind!=VARCALL&&T->ptr[0]->kind!=ARRAYCALL)
			{
				errormsg(18,T->pos,"Assign_lshift","Inappropriate lvalue");
				return -1;
			}
            int lvalue=Exp(T->ptr[0]);
            int result1=expresultindex;
			int rvalue=Exp(T->ptr[1]);//��ֵ��������ֵΪ
			int result2=expresultindex;
			if(lvalue<0||lvalue%3==2)
			{
				errormsg(19,T->pos,"Assign_lshift","lvalue error");
				return -1;
			}
			if(rvalue<0||rvalue%3==2)
			{
				errormsg(20,T->pos,"Assign_lshift","rvalue error");
				return -1;
			}
			if(lvalue%3!=rvalue%3)
			{
				errormsg(21,T->pos,"Assign_lshift","lvalue and rvalue do not match");
				return -1;
			}

			/*<<=���м���룬��һ����ʱ�����洢�ӷ�������ڶ�����ʱ�����洢������*/

			//�����Ĵ���
			T->code=merge(2,T->code,T->ptr[0]->code);
			T->code=merge(2,T->code,T->ptr[1]->code);

			//����ӷ�����
			char tempvar[10];
			strcpy(tempvar,createTemp());
			fillSymbolTable(NULL,tempvar,LV,0,'T',1,0);
			result.kind=ID_IR;
			strcpy(result.id,tempvar);

			op1.kind=ID_IR;
			strcpy(op1.id,symbolTable.symbols[result1].alias);
			op1.offset=symbolTable.symbols[result1].offset;
			op1.level=symbolTable.symbols[result1].level;
			op2.kind=ID_IR;
			strcpy(op2.id,symbolTable.symbols[result2].alias);

			T->code=merge(2,T->code,genIR(LSHFIT,op1,op2,result));

			//����ֵ����
			op2.kind=ID_IR;
			strcpy(op2.id,result.id);

			strcpy(tempvar,createTemp());
			expresultindex=fillSymbolTable(NULL,tempvar,LV,0,'T',1,0);
			result.kind=ID_IR;
			strcpy(result.id,tempvar);
			T->code=merge(2,T->code,genIR(ASSIGN_IR,op1,op2,result));
			/*�������˼�ǣ�tempX=op1=op2*/
			/*tempX����ʵ�����壬���ǻ�������Ҫ�ģ���if���*/

			return lvalue;
			break;
		}
		case ASSIGN_RSHFIT:
		{
			if(T->ptr[0]->kind!=VARCALL&&T->ptr[0]->kind!=ARRAYCALL)
			{
				errormsg(18,T->pos,"Assign_rshift","Inappropriate lvalue");
				return -1;
			}
			int lvalue=Exp(T->ptr[0]);
			int result1=expresultindex;
			int rvalue=Exp(T->ptr[1]);//��ֵ��������ֵΪ
			int result2=expresultindex;

			if(lvalue<0||lvalue%3==2)
			{
				errormsg(19,T->pos,"Assign_rshift","lvalue error");
				return -1;
			}
			if(rvalue<0||rvalue%3==2)
			{
				errormsg(20,T->pos,"Assign_rshift","rvalue error");
				return -1;
			}
			if(lvalue%3!=rvalue%3)
			{
				errormsg(21,T->pos,"Assign_rshift","lvalue and rvalue do not match");
				return -1;
			}

			/*>>=���м���룬��һ����ʱ�����洢�ӷ�������ڶ�����ʱ�����洢������*/

			//�����Ĵ���
			T->code=merge(2,T->code,T->ptr[0]->code);
			T->code=merge(2,T->code,T->ptr[1]->code);

			//����ӷ�����
			char tempvar[10];
			strcpy(tempvar,createTemp());
			fillSymbolTable(NULL,tempvar,LV,0,'T',1,0);
			result.kind=ID_IR;
			strcpy(result.id,tempvar);

			op1.kind=ID_IR;
			strcpy(op1.id,symbolTable.symbols[result1].alias);
			op1.offset=symbolTable.symbols[result1].offset;
			op1.level=symbolTable.symbols[result1].level;
			op2.kind=ID_IR;
			strcpy(op2.id,symbolTable.symbols[result2].alias);

			T->code=merge(2,T->code,genIR(RSHFIT,op1,op2,result));

			//����ֵ����
			op2.kind=ID_IR;
			strcpy(op2.id,result.id);

			strcpy(tempvar,createTemp());
			expresultindex=fillSymbolTable(NULL,tempvar,LV,0,'T',1,0);
			result.kind=ID_IR;
			strcpy(result.id,tempvar);
			T->code=merge(2,T->code,genIR(ASSIGN_IR,op1,op2,result));
			/*�������˼�ǣ�tempX=op1=op2*/
			/*tempX����ʵ�����壬���ǻ�������Ҫ�ģ���if���*/

			return lvalue;
			break;
		}
		case AND:
		{
			int lvalue=Exp(T->ptr[0]);
			int result1=expresultindex;
			int rvalue=Exp(T->ptr[1]);//��ֵ��������ֵΪ
			int result2=expresultindex;
			if(lvalue<0)
			{
				errormsg(19,T->pos,"And","lvalue error");
				return -1;
			}
			if(rvalue<0)
			{
				errormsg(20,T->pos,"And","rvalue error");
				return -1;
			}
			if(lvalue%3!=rvalue%3)
			{
				errormsg(21,T->pos,"And","lvalue and rvalue do not match");
				return -1;
			}
			//�����Ĵ���
			T->code=merge(2,T->code,T->ptr[0]->code);
			T->code=merge(2,T->code,T->ptr[1]->code);

			//����ӷ�����
			char tempvar[10];
			strcpy(tempvar,createTemp());
			expresultindex=fillSymbolTable(NULL,tempvar,LV,0,'T',1,0);
			result.kind=ID_IR;
			strcpy(result.id,tempvar);

			op1.kind=ID_IR;
			strcpy(op1.id,symbolTable.symbols[result1].alias);
			op2.kind=ID_IR;
			strcpy(op2.id,symbolTable.symbols[result2].alias);

			T->code=merge(2,T->code,genIR(AND,op1,op2,result));

			return lvalue;
			break;
		}
		case OR:
		{
			int lvalue=Exp(T->ptr[0]);
			int result1=expresultindex;
			int rvalue=Exp(T->ptr[1]);//��ֵ��������ֵΪ
			int result2=expresultindex;
			if(lvalue<0)
			{
				errormsg(19,T->pos,"Or","lvalue error");
				return -1;
			}
			if(rvalue<0)
			{
				errormsg(21,T->pos,"Or","rvalue error");
				return -1;
			}
			if(lvalue%3!=rvalue%3)
			{
				errormsg(21,T->pos,"Or","lvalue and rvalue do not match");
				return -1;
			}

			//�����Ĵ���
			T->code=merge(2,T->code,T->ptr[0]->code);
			T->code=merge(2,T->code,T->ptr[1]->code);

			//����ӷ�����
			char tempvar[10];
			strcpy(tempvar,createTemp());
			expresultindex=fillSymbolTable(NULL,tempvar,LV,0,'T',1,0);
			result.kind=ID_IR;
			strcpy(result.id,tempvar);

			op1.kind=ID_IR;
			strcpy(op1.id,symbolTable.symbols[result1].alias);
			op2.kind=ID_IR;
			strcpy(op2.id,symbolTable.symbols[result2].alias);

			T->code=merge(2,T->code,genIR(OR,op1,op2,result));

			return lvalue;
			break;
		}
		case RELOP:
		{
		    //��ϵ�������
		    //==,!=,>,>=,<,<=�����ֻ��������0��1
		    //��������ʱ������ֵ��ŵ��ǱȽ�������������
			int lvalue=Exp(T->ptr[0]);
			int result1=expresultindex;
			int rvalue=Exp(T->ptr[1]);//��ֵ��������ֵΪ
			int result2=expresultindex;
			if(lvalue<0)
			{
				errormsg(19,T->pos,"Relop","lvalue error");
				return -1;
			}
			if(rvalue<0)
			{
				errormsg(20,T->pos,"Relop","rvalue error");
				return -1;
			}
			if(lvalue%3!=rvalue%3)
			{
				errormsg(21,T->pos,"Relop","lvalue and rvalue do not match");
				return -1;
			}

			int relationop=0;

			if(!strcmp(T->type_id,"=="))
            {
                relationop=EQ_IR;//if�Ļ����̾����ж��Ƿ�Ϊ0��ת
            }
            else if(!strcmp(T->type_id,"!="))
            {
                relationop=NE_IR;
            }
            else if(!strcmp(T->type_id,">="))
            {
                relationop=GE_IR;
            }
            else if(!strcmp(T->type_id,">"))
            {
                relationop=GT_IR;
            }
            else if(!strcmp(T->type_id,"<="))
            {
                relationop=LE_IR;
            }
            else if(!strcmp(T->type_id,"<"))
            {
                relationop=LT_IR;
            }

            //����ṹ��exp,exp,op(tempvar),#temp=0,goto labend,labtrue,#temp=1,labend

            //�����Ĵ���
			T->code=merge(2,T->code,T->ptr[0]->code);
			T->code=merge(2,T->code,T->ptr[1]->code);

			//������ǩ��true,false,end

			strcpy(T->Etrue,createLabel());
			char tempend[15];
			strcpy(tempend,createLabel());





			result.kind=ID_IR;
			strcpy(result.id,T->Etrue);

            op1.kind=ID_IR;
			strcpy(op1.id,symbolTable.symbols[result1].alias);
			op2.kind=ID_IR;
			strcpy(op2.id,symbolTable.symbols[result2].alias);

			//ĳ���������Ƿ�goto result
			struct codenode* tempnode=genIR(relationop,op1,op2,result);
			T->code=merge(2,T->code,tempnode);

            //temp=0

			char tempvar[10];
			strcpy(tempvar,createTemp());
			expresultindex=fillSymbolTable(NULL,tempvar,LV,0,'T',1,0);

            op2.kind=INT_CONST;
            op2.const_int=0;
            result.kind=ID_IR;
            strcpy(result.id,tempvar);
            T->code=merge(2,T->code,genIR(ASSIGN,op1,op2,result));
            tempnode->next->flag='H';

            T->code=merge(2,T->code,genGoto(tempend));

            T->code=merge(2,T->code,genLabel(T->Etrue));

            //temp=1
            op2.kind=INT_CONST;
            op2.const_int=1;
            result.kind=ID_IR;
            strcpy(result.id,tempvar);
            T->code=merge(2,T->code,genIR(ASSIGN,op1,op2,result));

            T->code=merge(2,T->code,genLabel(tempend));

			return lvalue;
			break;
		}
		case PLUS:
		{
			int lvalue=Exp(T->ptr[0]);
			int result1=expresultindex;
			int rvalue=Exp(T->ptr[1]);//��ֵ��������ֵΪ
			int result2=expresultindex;
			if(lvalue<0)
			{
				errormsg(19,T->pos,"Plus","lvalue error");
				return -1;
			}
			if(rvalue<0)
			{
				errormsg(20,T->pos,"Plus","rvalue error");
				return -1;
			}
			if(lvalue%3!=rvalue%3)
			{
				errormsg(21,T->pos,"Plus","lvalue and rvalue do not match");
				return -1;
			}

			//�����Ĵ���
			T->code=merge(2,T->code,T->ptr[0]->code);
			T->code=merge(2,T->code,T->ptr[1]->code);

			//����ӷ�����
			char tempvar[10];
			strcpy(tempvar,createTemp());
			expresultindex=fillSymbolTable(NULL,tempvar,LV,0,'T',1,0);
			result.kind=ID_IR;
			strcpy(result.id,tempvar);

			op1.kind=ID_IR;
			strcpy(op1.id,symbolTable.symbols[result1].alias);
			op2.kind=ID_IR;
			strcpy(op2.id,symbolTable.symbols[result2].alias);

			T->code=merge(2,T->code,genIR(PLUS,op1,op2,result));

			return lvalue;
			break;
		}
		case MINUS:
		{
			if(T->ptr[1])
			{
				int lvalue=Exp(T->ptr[0]);
				int result1=expresultindex;
                int rvalue=Exp(T->ptr[1]);//��ֵ��������ֵΪ
                int result2=expresultindex;
				if(lvalue<0)
				{
					errormsg(19,T->pos,"Minus","lvalue error");
					return -1;
				}
				if(rvalue<0)
				{
					errormsg(20,T->pos,"Minus","rvalue error");
					return -1;
				}
				if(lvalue%3!=rvalue%3)
				{
					errormsg(21,T->pos,"Minus","lvalue and rvalue do not match");
					return -1;
				}

				//�����Ĵ���
                T->code=merge(2,T->code,T->ptr[0]->code);
                T->code=merge(2,T->code,T->ptr[1]->code);

                //����ӷ�����
                char tempvar[10];
                strcpy(tempvar,createTemp());
                expresultindex=fillSymbolTable(NULL,tempvar,LV,0,'T',1,0);
                result.kind=ID_IR;
                strcpy(result.id,tempvar);

                op1.kind=ID_IR;
                strcpy(op1.id,symbolTable.symbols[result1].alias);
                op2.kind=ID_IR;
                strcpy(op2.id,symbolTable.symbols[result2].alias);

                T->code=merge(2,T->code,genIR(MINUS,op1,op2,result));

				return lvalue;
			}
			else
			{
				int rvalue=Exp(T->ptr[0]);
				int result1=expresultindex;
				if(rvalue<0)
				{
					errormsg(20,T->pos,"Neg","rvalue error");
					return -1;
				}

				//�����Ĵ���
                T->code=merge(2,T->code,T->ptr[0]->code);

                //����ӷ�����
                char tempvar[10];
                strcpy(tempvar,createTemp());
                expresultindex=fillSymbolTable(NULL,tempvar,LV,0,'T',1,0);
                result.kind=ID_IR;
                strcpy(result.id,tempvar);

                op1.kind=INT_CONST;
                op1.const_int=0;
                op2.kind=ID_IR;
                strcpy(op2.id,symbolTable.symbols[result1].alias);

                //�˴�����֮�����ڣ�mipsû��negָ����ֻ����0��
                T->code=merge(2,T->code,genIR(MINUS,op1,op2,result));

				return rvalue;
			}
			break;
		}
		case MUL:
		{
			int lvalue=Exp(T->ptr[0]);
			int result1=expresultindex;
			int rvalue=Exp(T->ptr[1]);//��ֵ��������ֵΪ
			int result2=expresultindex;
			if(lvalue<0)
			{
				errormsg(19,T->pos,"Mul","lvalue error");
				return -1;
			}
			if(rvalue<0)
			{
				errormsg(20,T->pos,"Mul","rvalue error");
				return -1;
			}
			if(lvalue%3!=rvalue%3)
			{
				errormsg(21,T->pos,"Mul","lvalue and rvalue do not match");
				return -1;
			}

			//�����Ĵ���
            T->code=merge(2,T->code,T->ptr[0]->code);
            T->code=merge(2,T->code,T->ptr[1]->code);

            //����ӷ�����
            char tempvar[10];
            strcpy(tempvar,createTemp());
            expresultindex=fillSymbolTable(NULL,tempvar,LV,0,'T',1,0);
            result.kind=ID_IR;
            strcpy(result.id,tempvar);

            op1.kind=ID_IR;
            strcpy(op1.id,symbolTable.symbols[result1].alias);
            op2.kind=ID_IR;
            strcpy(op2.id,symbolTable.symbols[result2].alias);

            T->code=merge(2,T->code,genIR(MUL,op1,op2,result));

			return lvalue;
			break;
		}
		case DIV:
		{
			int lvalue=Exp(T->ptr[0]);
			int result1=expresultindex;
			int rvalue=Exp(T->ptr[1]);//��ֵ��������ֵΪ
			int result2=expresultindex;
			if(lvalue<0)
			{
				errormsg(19,T->pos,"Div","lvalue error");
				return -1;
			}
			if(rvalue<0)
			{
				errormsg(20,T->pos,"Div","rvalue error");
				return -1;
			}
			if(lvalue%3!=rvalue%3)
			{
				errormsg(21,T->pos,"Div","lvalue and rvalue do not match");
				return -1;
			}

			//�����Ĵ���
            T->code=merge(2,T->code,T->ptr[0]->code);
            T->code=merge(2,T->code,T->ptr[1]->code);

            //����ӷ�����
            char tempvar[10];
            strcpy(tempvar,createTemp());
            expresultindex=fillSymbolTable(NULL,tempvar,LV,0,'T',1,0);
            result.kind=ID_IR;
            strcpy(result.id,tempvar);

            op1.kind=ID_IR;
            strcpy(op1.id,symbolTable.symbols[result1].alias);
            op2.kind=ID_IR;
            strcpy(op2.id,symbolTable.symbols[result2].alias);

            T->code=merge(2,T->code,genIR(DIV,op1,op2,result));

			return lvalue;
			break;
		}
		case MOD:
		{
			int lvalue=Exp(T->ptr[0]);
			int result1=expresultindex;
			int rvalue=Exp(T->ptr[1]);//��ֵ��������ֵΪ
			int result2=expresultindex;
			if(lvalue<0||lvalue%3==2)
			{
				errormsg(19,T->pos,"Mod","lvalue error");
				return -1;
			}
			if(rvalue<0||rvalue%3==2)
			{
				errormsg(20,T->pos,"Mod","rvalue error");
				return -1;
			}
			if(lvalue%3!=rvalue%3)
			{
				errormsg(21,T->pos,"Mod","lvalue and rvalue do not match");
				return -1;
			}


			//�����Ĵ���
            T->code=merge(2,T->code,T->ptr[0]->code);
            T->code=merge(2,T->code,T->ptr[1]->code);

            //����ӷ�����
            char tempvar[10];
            strcpy(tempvar,createTemp());
            expresultindex=fillSymbolTable(NULL,tempvar,LV,0,'T',1,0);
            result.kind=ID_IR;
            strcpy(result.id,tempvar);

            op1.kind=ID_IR;
            strcpy(op1.id,symbolTable.symbols[result1].alias);
            op2.kind=ID_IR;
            strcpy(op2.id,symbolTable.symbols[result2].alias);

            T->code=merge(2,T->code,genIR(MOD,op1,op2,result));

			return lvalue;
			break;
		}
		case LSHFIT:
		{
			int lvalue=Exp(T->ptr[0]);
			int result1=expresultindex;
			int rvalue=Exp(T->ptr[1]);//��ֵ��������ֵΪ
			int result2=expresultindex;
			if(lvalue<0||lvalue%3==2)
			{
				errormsg(19,T->pos,"Lshift","lvalue error");
				return -1;
			}
			if(rvalue<0||rvalue%3==2)
			{
				errormsg(20,T->pos,"Lshift","rvalue error");
				return -1;
			}
			if(lvalue%3!=rvalue%3)
			{
				errormsg(21,T->pos,"Lshift","lvalue and rvalue do not match");
				return -1;
			}


			//�����Ĵ���
            T->code=merge(2,T->code,T->ptr[0]->code);
            T->code=merge(2,T->code,T->ptr[1]->code);

            //����ӷ�����
            char tempvar[10];
            strcpy(tempvar,createTemp());
            expresultindex=fillSymbolTable(NULL,tempvar,LV,0,'T',1,0);
            result.kind=ID_IR;
            strcpy(result.id,tempvar);

            op1.kind=ID_IR;
            strcpy(op1.id,symbolTable.symbols[result1].alias);
            op2.kind=ID_IR;
            strcpy(op2.id,symbolTable.symbols[result2].alias);

            T->code=merge(2,T->code,genIR(LSHFIT,op1,op2,result));

			return lvalue;
			break;
		}
		case RSHFIT:
		{
			int lvalue=Exp(T->ptr[0]);
			int result1=expresultindex;
			int rvalue=Exp(T->ptr[1]);//��ֵ��������ֵΪ
			int result2=expresultindex;
			if(lvalue<0||lvalue%3==2)
			{
				errormsg(19,T->pos,"Rshift","lvalue error");
				return -1;
			}
			if(rvalue<0||rvalue%3==2)
			{
				errormsg(20,T->pos,"Rshift","rvalue error");
				return -1;
			}
			if(lvalue%3!=rvalue%3)
			{
				errormsg(21,T->pos,"Rshift","lvalue and rvalue do not match");
				return -1;
			}


			//�����Ĵ���
            T->code=merge(2,T->code,T->ptr[0]->code);
            T->code=merge(2,T->code,T->ptr[1]->code);

            //����ӷ�����
            char tempvar[10];
            strcpy(tempvar,createTemp());
            expresultindex=fillSymbolTable(NULL,tempvar,LV,0,'T',1,0);
            result.kind=ID_IR;
            strcpy(result.id,tempvar);

            op1.kind=ID_IR;
            strcpy(op1.id,symbolTable.symbols[result1].alias);
            op2.kind=ID_IR;
            strcpy(op2.id,symbolTable.symbols[result2].alias);

            T->code=merge(2,T->code,genIR(RSHFIT,op1,op2,result));

			return lvalue;
			break;
		}
		case INC:
		{
			int value;
			if(T->ptr[0])
			{
				if(T->ptr[0]->kind!=VARCALL&&T->ptr[0]->kind!=ARRAYCALL)
				{
					errormsg(22,T->pos,"Inc","Inappropriate opreation value");
					return -1;
				}
				value=Exp(T->ptr[0]);
				int result1=expresultindex;
				if(value<0)
				{
					errormsg(23,T->pos,"Inc","Opreation value error");
					return -1;
				}

                char tempvar[10];

				//��������Ϊ+=1�Ĳ���������mips������INCָ����������Ĳ�������ȫ�ȼ۵�
				//�����Ĵ���
                T->code=merge(2,T->code,T->ptr[0]->code);

                strcpy(tempvar,createTemp());
                expresultindex=fillSymbolTable(NULL,tempvar,LV,0,'T',1,0);

                //��׺����ͨ��������ʱ�������

                op2.kind=ID_IR;
                strcpy(op2.id,symbolTable.symbols[result1].alias);
                result.kind=ID_IR;
                strcpy(result.id,tempvar);

                T->code=merge(2,T->code,genIR(ASSIGN,op1,op2,result));

                //����ӷ�����

                strcpy(tempvar,createTemp());
                fillSymbolTable(NULL,tempvar,LV,0,'T',1,0);
                result.kind=ID_IR;
                strcpy(result.id,tempvar);

                op1.kind=ID_IR;
                strcpy(op1.id,symbolTable.symbols[result1].alias);
                op1.offset=symbolTable.symbols[result1].offset;
                op1.level=symbolTable.symbols[result1].level;


                op2.kind=INT_CONST;
                op2.const_int=1;
                result.kind=ID_IR;
                strcpy(result.id,tempvar);

                //������ʹ��Ҫ�ȴ�����ʱ����
                T->code=merge(2,T->code,genIR(ASSIGN,op1,op2,result));

                op2.kind=ID_IR;
                strcpy(op2.id,result.id);


                T->code=merge(2,T->code,genIR(PLUS,op1,op2,result));

                //����ֵ����
                op2.kind=ID_IR;
                strcpy(op2.id,result.id);

                strcpy(tempvar,createTemp());
                fillSymbolTable(NULL,tempvar,LV,0,'T',1,0);
                result.kind=ID_IR;
                strcpy(result.id,tempvar);
                T->code=merge(2,T->code,genIR(ASSIGN_IR,op1,op2,result));
                /*�������˼�ǣ�tempX=op1=op2*/
                /*tempX����ʵ�����壬���ǻ�������Ҫ�ģ���if���*/

			}
			else if(T->ptr[1])
			{
				if(T->ptr[1]->kind!=VARCALL&&T->ptr[1]->kind!=ARRAYCALL)
				{
					errormsg(22,T->pos,"Inc","Inappropriate opreation value");
					return -1;
				}
				value=Exp(T->ptr[1]);
				int result1=expresultindex;
				if(value<0)
				{
					errormsg(23,T->pos,"Inc","Opreation value error");
					return -1;
				}
				//��׺�����Ĵ���������ǰ��ʱ������Ӧ��ֵ���ɣ���׺��������Ψ��һ������ǰ��ֵ�������
				//��Ȼ��Ҫ��ʱ�����洢
				//��������Ϊ+=1�Ĳ���������mips������INCָ����������Ĳ�������ȫ�ȼ۵�
				//�����Ĵ���
                T->code=merge(2,T->code,T->ptr[1]->code);


                //����ӷ�����



                char tempvar[10];
                strcpy(tempvar,createTemp());
                fillSymbolTable(NULL,tempvar,LV,0,'T',1,0);
                result.kind=ID_IR;
                strcpy(result.id,tempvar);

                op1.kind=ID_IR;
                strcpy(op1.id,symbolTable.symbols[result1].alias);
                op1.offset=symbolTable.symbols[result1].offset;
                op1.level=symbolTable.symbols[result1].level;
                op2.kind=INT_CONST;
                op2.const_int=1;

                result.kind=ID_IR;
                strcpy(result.id,tempvar);

                //������ʹ��Ҫ�ȴ�����ʱ����
                T->code=merge(2,T->code,genIR(ASSIGN,op1,op2,result));

                op2.kind=ID_IR;
                strcpy(op2.id,result.id);


                T->code=merge(2,T->code,genIR(PLUS,op1,op2,result));

                //����ֵ����
                op2.kind=ID_IR;
                strcpy(op2.id,result.id);

                strcpy(tempvar,createTemp());
                expresultindex=fillSymbolTable(NULL,tempvar,LV,0,'T',1,0);
                result.kind=ID_IR;
                strcpy(result.id,tempvar);
                T->code=merge(2,T->code,genIR(ASSIGN_IR,op1,op2,result));
                /*�������˼�ǣ�tempX=op1=op2*/
                /*tempX����ʵ�����壬���ǻ�������Ҫ�ģ���if���*/

			}
			return value;
			break;
		}
		case DEC:
		{
			int value;
			if(T->ptr[0])
			{
				if(T->ptr[0]->kind!=VARCALL&&T->ptr[0]->kind!=ARRAYCALL)
				{
					errormsg(22,T->pos,"Dec","Inappropriate opreation value");
					return -1;
				}
				value=Exp(T->ptr[0]);
				int result1=expresultindex;
				if(value<0)
				{
					errormsg(22,T->pos,"Dec","Opreation value error");
					return -1;
				}

				//���Լ���Ϊ-=1�Ĳ���������mips������DECָ����������Ĳ�������ȫ�ȼ۵�
				//�����Ĵ���
                T->code=merge(2,T->code,T->ptr[0]->code);


                char tempvar[10];

                strcpy(tempvar,createTemp());
                expresultindex=fillSymbolTable(NULL,tempvar,LV,0,'T',1,0);

                //��׺����ͨ��������ʱ�������

                op2.kind=ID_IR;
                strcpy(op2.id,symbolTable.symbols[result1].alias);
                result.kind=ID_IR;
                strcpy(result.id,tempvar);

                T->code=merge(2,T->code,genIR(ASSIGN,op1,op2,result));


                //����ӷ�����

                strcpy(tempvar,createTemp());
                fillSymbolTable(NULL,tempvar,LV,0,'T',1,0);
                result.kind=ID_IR;
                strcpy(result.id,tempvar);

                op1.kind=ID_IR;
                strcpy(op1.id,symbolTable.symbols[result1].alias);
                op1.offset=symbolTable.symbols[result1].offset;
                op1.level=symbolTable.symbols[result1].level;
                op2.kind=INT_CONST;
                op2.const_int=1;


                result.kind=ID_IR;
                strcpy(result.id,tempvar);

                //������ʹ��Ҫ�ȴ�����ʱ����
                T->code=merge(2,T->code,genIR(ASSIGN,op1,op2,result));

                op2.kind=ID_IR;
                strcpy(op2.id,result.id);


                T->code=merge(2,T->code,genIR(MINUS,op1,op2,result));

                //����ֵ����
                op2.kind=ID_IR;
                strcpy(op2.id,result.id);

                strcpy(tempvar,createTemp());
                fillSymbolTable(NULL,tempvar,LV,0,'T',1,0);
                result.kind=ID_IR;
                strcpy(result.id,tempvar);
                T->code=merge(2,T->code,genIR(ASSIGN_IR,op1,op2,result));
                /*�������˼�ǣ�tempX=op1=op2*/
                /*tempX����ʵ�����壬���ǻ�������Ҫ�ģ���if���*/


			}
			else if(T->ptr[1])
			{
				if(T->ptr[1]->kind!=VARCALL&&T->ptr[1]->kind!=ARRAYCALL)
				{
					errormsg(23,T->pos,"Dec","Inappropriate opreation value");
					return -1;
				}
				value=Exp(T->ptr[1]);
				int result1=expresultindex;
				if(value<0)
				{
					errormsg(23,T->pos,"Dec","Opreation value error");
					return -1;
				}

				//��׺�Լ��Ĵ���������ǰ��ʱ������Ӧ��ֵ���ɣ���׺���Լ�-=������Ψ��һ������ǰ��ֵ�������
				//��Ȼ��Ҫ��ʱ�����洢
				//��������Ϊ-=1�Ĳ���������mips������DECָ����������Ĳ�������ȫ�ȼ۵�
				//�����Ĵ���
                T->code=merge(2,T->code,T->ptr[1]->code);


                //�����������



                char tempvar[10];
                strcpy(tempvar,createTemp());
                fillSymbolTable(NULL,tempvar,LV,0,'T',1,0);
                result.kind=ID_IR;
                strcpy(result.id,tempvar);

                op1.kind=ID_IR;
                strcpy(op1.id,symbolTable.symbols[result1].alias);
                op1.offset=symbolTable.symbols[result1].offset;
                op1.level=symbolTable.symbols[result1].level;
                op2.kind=INT_CONST;
                op2.const_int=1;


                result.kind=ID_IR;
                strcpy(result.id,tempvar);

                //������ʹ��Ҫ�ȴ�����ʱ����
                T->code=merge(2,T->code,genIR(ASSIGN,op1,op2,result));

                op2.kind=ID_IR;
                strcpy(op2.id,result.id);


                T->code=merge(2,T->code,genIR(MINUS,op1,op2,result));

                //����ֵ����
                op2.kind=ID_IR;
                strcpy(op2.id,result.id);

                strcpy(tempvar,createTemp());
                expresultindex=fillSymbolTable(NULL,tempvar,LV,0,'T',1,0);
                result.kind=ID_IR;
                strcpy(result.id,tempvar);
                T->code=merge(2,T->code,genIR(ASSIGN_IR,op1,op2,result));
                /*�������˼�ǣ�tempX=op1=op2*/
                /*tempX����ʵ�����壬���ǻ�������Ҫ�ģ���if���*/


			}
			return value;
			break;
		}
		case NOT:
		{
			int value=Exp(T->ptr[0]);
			int result1=expresultindex;
			if(value<0)
			{
				errormsg(23,T->pos,"Not","Opreation value error");
				return -1;
			}
			//�߼��ǵĴ���ͬ�ڰ�λ�ǣ�����0ֵΪ�жϵ�
			//�����Ƿ�Ϊ0�Ƚ�ȡֵ���߼���

			//������� exp ��if ��temp=1��goto end��truelable,temp=0,endlable
            T->code=merge(2,T->code,T->ptr[0]->code);

			//������ǩ��true,end

			strcpy(T->Etrue,createLabel());
			char tempend[15];
			strcpy(tempend,createLabel());

			char tempvar[10];
			strcpy(tempvar,createTemp());
			expresultindex=fillSymbolTable(NULL,tempvar,LV,0,'T',1,0);
			result.kind=ID_IR;
			strcpy(result.id,T->Etrue);

            op1.kind=ID_IR;
			strcpy(op1.id,symbolTable.symbols[result1].alias);
			op2.kind=INT_CONST;
			op2.const_int=0;

			//ĳ���������Ƿ�goto result
			struct codenode* tempnode=genIR(EQ_IR,op1,op2,result);
			T->code=merge(2,T->code,tempnode);

            //temp=0
            strcpy(tempvar,createTemp());
            op2.kind=INT_CONST;
            op2.const_int=0;
            result.kind=ID_IR;
            strcpy(result.id,tempvar);
            T->code=merge(2,T->code,genIR(ASSIGN,op1,op2,result));
            tempnode->next->flag='H';
            //ASSIGN��ʾ������ֵresult=op2
            T->code=merge(2,T->code,genGoto(tempend));

            T->code=merge(2,T->code,genLabel(T->Etrue));

            //temp=1
            strcpy(tempvar,createTemp());
            op2.kind=INT_CONST;
            op2.const_int=1;
            result.kind=ID_IR;
            strcpy(result.id,tempvar);
            T->code=merge(2,T->code,genIR(ASSIGN,op1,op2,result));

            T->code=merge(2,T->code,genLabel(tempend));

			return value;
			break;
		}
		case FUNCCALL:
		{
			/*��������һ���Ժ�������Ϊ׼�����޺�������ֻ�к������������Ժ�������Ϊ׼*/

			int nodestack[20];
			int top=-1;

			int type;
			int i=searchTarget(T->type_id,'F');
			if(i<0)
			{
			    i=searchTarget(T->type_id,'D');
			    if(i<0)
                    errormsg(24,T->pos,T->type_id,"Undeclared function or Undefined function");
			}
			type=symbolTable.symbols[i].type;

			/*�Է��ű��еĲ���Ϊ׼����������Ѱ��*/
			int j=1;//�ӵ�ǰ����ͷ��ʼ����ƥ�����

			int pnum=symbolTable.symbols[i].paramnum;

			int deftype;
			int paratype;

			struct node *temp=T->ptr[0];

			while(j<=pnum)
			{
				deftype=symbolTable.symbols[i+j].type;
				paratype=Exp(temp->ptr[0]);
				//��������ջ��ÿ�ε��ñ��ʽ��������һ��expresult��indexȻ��ֵ������ջ����
				T->code=merge(2,T->code,temp->ptr[0]->code);
                nodestack[++top]=expresultindex;

				if(paratype<0)
				{
					errormsg(26,T->pos,T->type_id,"Real paramaters type error");
					return -1;
				}

				if(deftype%3!=paratype%3)
				{
					errormsg(27,T->pos,T->type_id,"Real paramaters do not match formal paramaters");
					return -1;
				}
				temp=temp->ptr[1];
				j++;
				if(temp==NULL&&j<=pnum)
				{
					//ʵ������С���β�����
					errormsg(28,T->pos,"Function_call","Real paramaters and formal paramaters are not equal");
					return -1;
				}
			}
			if(temp!=NULL)
			{
				//ʵ�����д����β�����
				errormsg(28,T->pos,"Function_call","Real paramaters and formal paramaters are not equal");
				return -1;
			}
			/*�������õĽ���Ƿ���ֵ��*/


			while(top>=0)
            {
                result.kind=ID_IR;
                strcpy(result.id,symbolTable.symbols[nodestack[top--]].alias);
                T->code=merge(2,T->code,genIR(ARG_IR,op1,op2,result));
            }

            result.kind=0;
            if(type!=-2)//��void����
            {
                result.kind=ID_IR;
                strcpy(result.id,createTemp());
                expresultindex=fillSymbolTable(NULL,result.id,LV,0,'T',1,0);//��������ֵ
            }
			op2.kind=ID_IR;
            strcpy(op2.id,T->type_id);
            T->code=merge(2,T->code,genIR(CALL_IR,op1,op2,result));
			//��ʽ��result(temp)=call op2����result�����ڣ�kind==0�����޷���ֵ

			return type+3;
			break;
		}
		case ARRAYCALL:
		{
			int type;
			int i=searchTarget(T->type_id,'A');//��һ������
			if(i<0)
			{
				errormsg(29,T->pos,T->type_id,"Undefined array");
				return -1;
			}
			type=symbolTable.symbols[i].type;

            int result1;//Ĭ�ϴ�ʱֻ��һά����

			struct node *temp=T->ptr[0];
			int dnum=symbolTable.symbols[i].paramnum;

			while(dnum>0)
			{
				if(Exp(temp->ptr[0])%3!=0)
				{
					errormsg(30,T->pos,T->type_id,"Array dimention is not Int");
					return -1;
				}
				T->code=merge(2,T->code,temp->ptr[0]->code);
				result1=expresultindex;

				dnum--;
				temp=temp->ptr[1];

				if(dnum>0&&temp==NULL)
				{
					errormsg(31,T->pos,T->type_id,"Array dimention is greater than call dimention");
					return -1;
				}
			}
			if(temp!=NULL)
			{
				errormsg(32,T->pos,T->type_id,"Call dimention is greater than array dimention");
				return -1;
			}



			op2.kind=ID_IR;//OP2ָʾ�±�
            strcpy(op2.id,symbolTable.symbols[result1].alias);


            op1.kind=ID_IR;//op1ָʾ
            strcpy(op1.id,symbolTable.symbols[i].alias);
            op1.offset=symbolTable.symbols[i].offset;
            op1.level=symbolTable.symbols[i].level;

            result.kind=ID_IR;
            strcpy(result.id,createAddr());
            expresultindex=fillSymbolTable(NULL,result.id,LV,0,'T',1,0);
            result.offset=op1.offset;//�����ַ
            result.level=op1.level;

            T->code=merge(2,T->code,genIR(ARR_IR,op1,op2,result));
			//��ʽ��result(temp)=op1[op2]����op1+op2*4����result�����ڣ�kind==0�����޷���ֵ


			return type+3;
			break;
		}
		case VARCALL:
		{
			int type;
			int i=searchTarget(T->type_id,'V');

			int j=-1;
			if(nowfunc>=0)
				j=searchTarget(T->type_id,'P');

			int varindex;

			if(j>=0&&symbolTable.symbols[j].paramnum==nowfunc)
            {
                if(i>j)//˵��Ϊ�ֲ�����
                {
                    varindex=i;
                }
                else//˵���ǲ������ⲿ����
                {
                    varindex=j;
                }
			}
			else
			{
				if(i<0)
				{
					errormsg(33,T->pos,T->type_id,"Undefined ID");
					return -1;
				}
				varindex=i;
			}

			result.kind=ID_IR;
            strcpy(result.id,createAddr());

			op2.kind=ID_IR;
            strcpy(op2.id,symbolTable.symbols[varindex].alias);
			result.offset=symbolTable.symbols[varindex].offset;
			result.level=symbolTable.symbols[varindex].level;

			expresultindex=fillSymbolTable(NULL,result.id,LV,0,'T',1,0);

			T->code=merge(2,T->code,genIR(ASSIGN,op1,op2,result));
			//�������ã�Ϊ��ʱ�����������ԭ���Ƿ���Ŀ��������ɣ����뽫��������Ĵ���

			return symbolTable.symbols[varindex].type+3;
		}
		case STRUCTCALL:
		{
			int type;
			int i=searchTarget(T->type_id,'B');

			if(i<0)
			{
				errormsg(34,T->pos,T->type_id,"Undeclared struct variable");
				return -1;
			}

            int master=searchTarget(symbolTable.symbols[symbolTable.symbols[i].paramnum].name,'S');

			int j=1;
			struct node *temp=T->ptr[0];

			while(symbolTable.symbols[master+j].flag=='N'&&symbolTable.symbols[master+j].paramnum==master)
			{
			    //��Ա�����ȶ�
				if(!strcmp(symbolTable.symbols[master+j].name,temp->type_id))
				{
				    result.kind=ID_IR;
                    strcpy(result.id,createAddr());
                    expresultindex=fillSymbolTable(NULL,result.id,LV,0,'T',1,0);//�ṹ����ֵ

                    op2.kind=ID_IR;
                    strcpy(op2.id,temp->type_id);//��Ա����
                    op2.offset=temp->offset;//��Ա������Ӧ��ƫ����

                    op1.kind=ID_IR;
                    strcpy(op1.id,symbolTable.symbols[i].name);//�ṹ����
                    op1.offset=symbolTable.symbols[i].offset;
                    op1.level=symbolTable.symbols[i].level;

                    result.offset=op1.offset+op2.offset;
                    result.level=op1.level;
                    T->code=merge(2,T->code,genIR(STRUCT_IR,op1,op2,result));
                    //��ʽ��result(temp)=op1.op2����op1+op2����result�����ڣ�kind==0�����޷���ֵ

					return symbolTable.symbols[master+j].type+3;
				}
				j++;
			}
			errormsg(35,T->pos,temp->type_id,"No such member variable in the struct");
			return -1;
		}
		case INT_CONST:

		    result.kind=ID_IR;
            strcpy(result.id,createTemp());
            expresultindex=fillSymbolTable(NULL,result.id,LV,0,'T',1,0);//�ṹ����ֵ

            op2.kind=INT_CONST;
            op2.const_int=T->type_int;
            T->code=merge(2,T->code,genIR(ASSIGN,op1,op2,result));

			return 0;
			break;
		case CHAR_CONST:
		    errormsg(0,T->pos,"#","Not support char constant, temporarily");
			return -1;
			break;
		case FLOAT_CONST:
		    errormsg(0,T->pos,"#","Not support float constant, temporarily");
			return -2;
			break;
		case STRING_CONST:
			errormsg(36,T->pos,"String_const","not support in the expression, temporarily");
			return -1;

	}

}



/*�ظ������ж�*/
/*�漰ȫ��Ϊ�ⲿ����*/
/*�ֲ������������κ��ⲿ�����������������ⲿ�������ṹ�����ȣ�������Ծֲ�����Ϊ׼*/
/*���������飬�ṹ����ֻ�����������Ա��������*/
/*����ԭ�Ϳ����뺯�����壬��������Ա��������*/
/*������������뺯��ԭ�ͣ���������Ա��������*/
/*�ṹ�����������������ԭ�ͣ� �������壬��Ա��������*/
/*��Ա��������������ṹ���Ա������һ������*/
/*�����������������������һ������*/
/*�˺�����֤�ⲿ���������淶������֤�ֲ����������淶������жϾֲ�������Ҫ�ڽṹ���ڼ�������ж�*/
/*Ϊ�˷�ֹ���������������Ϊ����Ҫһ�����޸ģ��п��ܳ��ֱ���֮ǰ����һ����Ա����������Ա����֮ǰ�ı����ͱ��ڸ���,�ѽ��*/
int redec(char* name,char type,int master)
{
	switch(type)
	{
		int i=0;
		char typef;
		case 'V':
		case 'A':
		case 'B':
			for (i=symbolTable.index-1;i>=0;i--)
				if (!strcmp(symbolTable.symbols[i].name,name))
				{
					typef=symbolTable.symbols[i].flag;
					if(typef!='N'&&typef!='P')
						return 0;
				}
			break;
		case 'D':
			for (i=symbolTable.index-1;i>=0;i--)
				if (!strcmp(symbolTable.symbols[i].name,name))
				{
					typef=symbolTable.symbols[i].flag;
					if(typef!='N'&&typef!='P'&&typef!='F')
						return 0;
				}
			break;
		case 'F':
			for (i=symbolTable.index-1;i>=0;i--)
					if (!strcmp(symbolTable.symbols[i].name,name))
					{
						typef=symbolTable.symbols[i].flag;
						if(typef!='N'&&typef!='P'&&typef!='D')
							return 0;
					}
				break;
		case 'S':
			for(i=symbolTable.index-1;i>=0;i--)
					if (!strcmp(symbolTable.symbols[i].name,name))
					{
						typef=symbolTable.symbols[i].flag;
						if(typef!='N'&&typef!='P'&&typef!='D'&&typef!='F')
							return 0;
					}
				break;
		case 'N'://Ĭ�ϵ��ǴӺ���ǰ�ң����ͬ�ṹ��ĳ�Ա������ͻһ�������ڲ�����������֮ǰ
			for(i=symbolTable.index-1;i>=0;i--)
				if (!strcmp(symbolTable.symbols[i].name,name))
				{
					typef=symbolTable.symbols[i].flag;
					if(symbolTable.symbols[i].paramnum==master&&typef=='N')
						return 0;
				}
			break;
		case 'P'://Ĭ�ϵ��ǴӺ���ǰ�ң����ͬ�����Ĳ�����ͻһ�������ڲ�����������֮ǰ
			for(i=symbolTable.index-1;i>=0;i--)
				if (!strcmp(symbolTable.symbols[i].name,name))
				{
					typef=symbolTable.symbols[i].flag;
					if(symbolTable.symbols[i].paramnum==master&&typef=='P')
						return 0;
				}
			break;
		default:
			break;
	}
	return 1;
}
/*�ֲ������ظ����*/
/*�κξֲ�����ֻҪ������ͬ���û����ͬ��������*/
/*����ͬ�����ζ��ͬһ�������飬�����ܳ����������*/
/*��Ϊ��������˳�ʱ��ɾ����Ӧ����ľֲ�����*/
int localRedec(char* name,int LV)
{
	int i=symbolTable.index-1;
	while(symbolTable.symbols[i].level==LV)
	{
		if(!strcmp(symbolTable.symbols[i].name,name))
			return 0;
		i--;
	}
	return 1;
}

void errormsg(int No,int line, char *msg1, char *msg2)
{
    NoIRgenerate=1;
    printf("No:%d Line:%d, %s %s\n",No, line, msg1, msg2);
}

/*�����ض�����ķ��ű����������ж��Ƿ���������������һ�������������Ƕ�һ�޶���*/
int searchTarget(char *name,char flag)
{
    int i;
    for (i = symbolTable.index - 1; i >= 0; i--)
        if (!strcmp(symbolTable.symbols[i].name, name)&&symbolTable.symbols[i].flag==flag)
            return i;
    return -1;
}


int parammatch(int dec,int def)
{
    int k=0;
    int pnum;

    if(symbolTable.symbols[dec].type!=symbolTable.symbols[def].type)
        return 1;

    if(symbolTable.symbols[dec].paramnum!=symbolTable.symbols[def].paramnum)
        return 2;

    pnum=symbolTable.symbols[dec].paramnum;

    while(k<=pnum)
    {
        if(symbolTable.symbols[dec+k].type!=symbolTable.symbols[def+k].type)
            return 3;
        k++;
    }
    return 0;

}

int getresult(struct node* T)
{
    if(T==NULL)
        return 0;
    if(T->kind==INT_CONST)
    {
        return T->type_int;
    }
    else
    {
        switch(T->kind)
        {
            case PLUS:
                return getresult(T->ptr[0])+getresult(T->ptr[1]);
                break;
            case MINUS:
                if(T->ptr[1]!=NULL)
                    return getresult(T->ptr[0])-getresult(T->ptr[1]);
                else
                    return -getresult(T->ptr[0]);
                break;
            case MUL:
                return getresult(T->ptr[0])*getresult(T->ptr[1]);
                break;
            case DIV:
                return getresult(T->ptr[0])/getresult(T->ptr[1]);
                break;
            case MOD:
                return getresult(T->ptr[0])%getresult(T->ptr[1]);
                break;
            case LSHFIT:
                return getresult(T->ptr[0])<<getresult(T->ptr[1]);
                break;
            case RSHFIT:
                return getresult(T->ptr[0])>>getresult(T->ptr[1]);
                break;
            default:
                errormsg(0,T->pos,"#","Not Int Const");
                return 0;
                break;

        }
    }
}


/*�����ű�*/
int fillSymbolTable(char *name,char *alias, int level, int type, char flag,int local,int offset)
{

    int i;
	i=symbolTable.index;

    //��д���ű�����
    if(name==NULL)
        strcpy(symbolTable.symbols[i].name, "");
    else
        strcpy(symbolTable.symbols[i].name, name);
    if(alias==NULL)
        strcpy(symbolTable.symbols[i].alias, "");
    else
        strcpy(symbolTable.symbols[i].alias, alias);
    symbolTable.symbols[i].level = level;
    symbolTable.symbols[i].type = type;
    symbolTable.symbols[i].flag = flag;
    symbolTable.symbols[i].local = local;
    symbolTable.symbols[i].offset = offset;
    return symbolTable.index++; //���ص��Ƿ����ڷ��ű��е�λ����ţ��м��������ʱ�������ȡ�����ű���
}

struct codenode *genIR(int op,struct opn opn1,struct opn opn2,struct opn result)
{
    struct codenode *h=(struct codenode *)malloc(sizeof(struct codenode));
    h->op=op;
    h->opn1=opn1;
    h->opn2=opn2;
    h->result=result;
    h->flag='N';
    h->next=h;
    h->prior=h;
    return h;
}

//����һ�������䣬����ͷָ��
struct codenode *genLabel(char *label){
    struct codenode *h=(struct codenode *)malloc(sizeof(struct codenode));
    h->op=LABEL_IR;
    strcpy(h->result.id,label);
    h->flag='H';
    h->next=h->prior=h;
    return h;
}

//����GOTO��䣬����ͷָ��
struct codenode *genGoto(char *label){
    struct codenode *h=(struct codenode *)malloc(sizeof(struct codenode));
    h->op=GOTO_IR;
    strcpy(h->result.id,label);
    h->flag='N';
    h->next=h->prior=h;
    return h;
}

//�ϲ�����м�����˫��ѭ��������β����
struct codenode *merge(int num,...){
    struct codenode *h1,*h2,*p,*t1,*t2;
    va_list ap;
    va_start(ap,num);
    h1=va_arg(ap,struct codenode *);
    while (--num>0) {
        h2=va_arg(ap,struct codenode *);
        if (h1==NULL) h1=h2;
        else if (h2){
            t1=h1->prior;
            t2=h2->prior;
            t1->next=h2;
            t2->next=h1;
            h1->prior=t2;
            h2->prior=t1;
            }
        }
    va_end(ap);
    return h1;
}
