#ifndef __DEF_H__
#define __DEF_H__

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdarg.h"

#include "parser.tab.h"

#define MAXLENGTH 300
#define DX sizeof(int)

//DXΪ���ص�ַ

enum node_kind{EXTLIST,VAR,VARDEC,ARRDEC,LOCALVAR,FUNCDEF,FUNCDEC,FUNCHEAD,VARLIST,VARNAME,PARAMLIST,PARAM,ARRNAME,ARRLTH,DIMELIST,STRUCTNAME,
ARRINITIAL,STRUCTDEF,STRUCTDEC,LOCALSTRUCTDEF,NUMBERLIST,STRUCTCALL,FUNCNAME,COMPSTAT,STATLIST,EXP,FOR1,FOR2,FOR3,FUNCCALL,ARRAYCALL,CALLDIMELIST
,VARCALL,ARGS};

enum IR_kind{FUNC_IR=34,PARAM_IR,ASSIGN_IR,ARG_IR,CALL_IR,ARR_IR,STRUCT_IR,RETURN_IR,ID_IR,IF_IR,LABEL_IR,GOTO_IR,
EQ_IR,NE_IR,GT_IR,GE_IR,LT_IR,LE_IR,END_IR};


//�����﷨��������͡�����ַ������͵ȶ�������ο���ʵ��ʱһ��Ҫ�����Լ������������


struct node {

	enum node_kind kind;               //�������
	union {
		  char type_id[33];       //�ɱ�ʶ�����ɵ�Ҷ���
		  int type_int;           //�����������ɵ�Ҷ���
		  char type_char;         //���ַ��������ɵ�Ҷ�ڵ�
		  float type_float;       //�ɸ��㳣�����ɵ�Ҷ���
	      };
    struct node *ptr[4];        //�ӽڵ�
    struct node *parent;		//���ڵ�
    int pos;                     //�﷨��λ����λ���к�
	int type;                      //���Ա�ʶ���ʽ��������
	int num;                      //����������������ͳ���βθ���
	char Etrue[15],Efalse[15];		//�Բ������ʽ�ķ���ʱ�����ת��Ŀ��ı��
	char Snext[15];               //����Ӧ���Sִ�к����һ�����λ�ñ��
	struct codenode *code;          //�ý���м��������ͷָ��
	int offset;                     //ƫ����
	int place;                     //��ţ���ʱ�������ڷ��ű��λ�����
    int width;                     //ռ�����ֽ���
    };



struct symbol{
    char name[33];   //����������
    int level;        //��ţ�������临��ʱָʾ�����Ƿ��ظ�
    char alias[10];     //����
    int type;         //�������ͻ�������ֵ���ͣ�-2:void 0��int��1��char��2��float
    int paramnum;
	//�Ժ������ã���¼��ʽ�������������ڳ�Ա������ָʾ��ṹ��������±꣬�����飬ָʾ��ά�����Խṹ������ָ���������±�
	//�Բ�����ָʾ�亯��ͷ���±�
    char flag;       //���ű�ǣ��������壺'F' ����ԭ�ͣ�'D'  ������'V'  ������'P' ���飺'A' �ṹ����'S' �ṹ��������'B'
					 //��Ա������'N'����֧�ֳ�Ա���飩 ��ʱ������'T'
	int offset;     //��������������ƫ������ȫ�ֱ��������data����ַƫ�ƣ��ֲ��������ջ֡ƫ�ƣ�
	//���ڽṹ�������ǽṹ�峤�ȣ����ں������壬�����оֲ�����֮��
    int local;
};

//���ű�

struct symbol_table{
    struct symbol symbols[ MAXLENGTH ];
    int index;//indexָ�������һ��д����ű��λ��
};

struct opn{
    int kind;                  //��ʶ����������
    int type;                  //��ʶ������������
    int const_int;      //������ֵ��������
    char id[33];        //��������ʱ�����ı��������ַ���
    int level;                 //�����Ĳ�ţ�0��ʾ��ȫ�ֱ��������ݱ����ھ�̬������
    int offset;                 //������Ԫƫ�����������ڷ��ű�Ķ���λ����ţ�Ŀ���������ʱ��
    //���ں���������ԣ�����൱��ջ֡�Ĵ�С����result���
};
struct codenode {   //����ַTAC������,���õ��������м����Դ���
        int  op;
        char flag;
        struct opn opn1;
        struct opn opn2;
        struct opn result;
        struct codenode  *next,*prior;
};

struct registerSymbol{
    char tempvar[10];
    int reg;
    int addrreg;
    int flag;//0:�ǵ�ַ��1:�洢�е�ַ
};
//ʵ��һ����
struct node *mknode(int kind,struct node *first, struct node *second, struct node *third, struct node *forth, int pos );

//ʵ�������
void errormsg(int No,int line, char *msg1, char *msg2);
int localRedec(char* name,int LV);
int redec(char* name,char type,int master);
int Exp(struct node* T);
void LocalVar(struct node *T);
void Stat(struct node *T);
void CompoundStat(struct node *T);
void StructDef(struct node *T);
void StructDec(struct node *T);
void FuncDec(struct node *T);
void FuncDef(struct node *T);
void VarDef(struct node *T);
void ExtDefList(struct node *T);
int fillSymbolTable(char *name,char* alias,int level, int type, char flag,int local,int offset);
int searchTarget(char *name,char flag);
int parammatch(int dec,int def);

//ʵ��������
int getresult(struct node* T);
struct codenode *genIR(int op,struct opn opn1,struct opn opn2,struct opn result);
struct codenode *merge(int num,...);
struct codenode *genGoto(char *label);
struct codenode *genLabel(char *label);

//ʵ���ĺ���
void push(int rd);
void pop(int rd);
void ble(int rd,int rt,char* s);
void blt(int rd,int rt,char* s);
void bge(int rd,int rt,char* s);
void bgt(int rd,int rt,char* s);
void bne(int rd,int rt,char* s);
void beq(int rd,int rt,char* s);
void jal(char* s);
void j(char* s);
void jr(int rd);
void or(int rd,int rs,int rt);
void and(int rd,int rs,int rt);
void srlv(int rd,int rs,int rt);
void sllv(int rd,int rs,int rt);
void sub(int rd,int rs,int rt);
void add(int rd,int rs,int rt);
void subi(int rd,int rs,int imm);
void addi(int rd,int rs,int imm);
void label(char* s);
void sw(int rd,int rt,int offset,int base);
void lw(int rd,int rt,int offset,int base);
int getBaseBlock(struct codenode* root);
int getopnum(char* tvar);
void revokeRegister(int reg);
int allocRegister();
int getframesize(struct codenode* root,char* s);
void generateTargetCode(struct codenode* root);
void recordcode();



#endif // DEF_H_INCLUDED
