#include "def.h"

/*Ŀ��������ɣ������м�������У�ѡ����ʵ�mipsָ��*/

/*Ŀǰ��ʱ֧�ֵ�mipsָ��Ϊ*/

/*
add,addi,sub,subi,mul,and,andi,or,ori,beq,bne,bgt,bge,blt,ble,sllv,srlv,lw,sw,j,jal,jr
*/

/*
��Ҫ����Ϊ��·��ԭ�������ָ��Ҳ������R��I��J�������͵�ָ�
*/
/*�Ĵ��������㷨���þֲ��Ĵ��������㷨����������밴�黮�ֵ����Ż��������㷨ʵ�������Ƚ��鷳*/

FILE* fp;

struct codenode* head=NULL;

struct codenode* tail=NULL;

int regState[10];//δ����Ϊ0������Ϊ���ü���
//����Ϊ8-17�żĴ���

//�˶�Ϊ�ھ���������ɹ����е���;

struct registerSymbol regtable[100];

int top;

char targetcode[30];

int cmpreg;
int cmpregindex;//ȡֵֻ��16 17
//���������Ҫָʾ������ǰ���ڱȽ����ĸ�ֵ����ֵĬ��Ϊ��ͬ�Ĵ������������һ�θ�ֵΪ16�ţ��ڶ���ҲΪ16�żĴ���


/*����ԭ��
1.������addr=v����䷭��Ϊlw������Ĵ���
2.������temp=const����䣬����ʱ��¼��const��ֵ�����������ʹ�÷���Ϊaddi�ȣ�����ʹ����û���κ�����
���ճ������tempһ�����õ���
3.��������������Ϊ��Ӧ������ַ���
4.��ֵ��䷭��Ϊsw�����صĽ��Ϊ����ֵ�ļĴ���ֵ
5.arg����Ϊ��ջ
6.parameter��Ϊ�������壬ͬ����ƫ����ֵ
7.function���Ϊ��ţ�
8.call��䷭��Ϊ����ջ֡������ֵ��$v0ȡ
9.return��䷭��Ϊ����ֵд��$v0��jr
10.goto��䷭��Ϊj
11.if��䷭��Ϊbeq $0
12.eq,ne,gt,ge,lt,le�ֱ��Ӧ�����������ж����
13.������ԭ������

������ʱ����ջ����ʶΪ��������������ڳ���ֱ���Գ������㣬���ڱ�������Ҫд�����ջ����ȷ��Ӧ�Ĵ���
*/



void generateTargetCode(struct codenode* root)
{

        fp=fopen("F:\\Y3\\compliationexp\\lab4\\TargetCode\\target.asm","w+");

        /*����Ҫ��system�ĵ��ã���Ϊһ������ԭ�ͣ�û��ʲô����*/
        printf("\n\n\n");
        label("system");
        push(31);//���ص�ַ��ջ
        push(29);//ebp��ջ
        addi(29,30,0);//ebp=esp
        //�����޲�������迪��ջ֡
        lw(4,0,8,1);//��ӡ����Ϊebp+8�����ĺżĴ���
        lw(3,0,12,1);//���Ʋ���Ϊebp+12�������żĴ���
        printf("syscall\n");
        fprintf(fp,"syscall\n");

        addi(30,29,0);//esp=ebp
        pop(29);
        pop(31);
        jr(31);

        head=root;
        struct opn *op1;
        struct opn *op2;
        struct opn *res;
        while(head->op!=END_IR)
        {
            op1=&(head->opn1);
            op2=&(head->opn2);
            res=&(head->result);
            switch(head->op)
            {
                case FUNC_IR:
                {
                    printf("\n\n\n");
                    label(res->id);
                    if(!strcmp(res->id,"main"))
                    {
                        addi(28,0,512);
                        addi(29,0,2048);
                        addi(30,0,2048);
                    }
                    //�������߱����ֳ�
                    push(31);//���ص�ַ��ջ
                    push(29);//ebp��ջ
                    addi(29,30,0);//ebp=esp
                    addi(30,30,-getframesize(root,res->id));

                    push(8);
                    push(9);
                    push(10);
                    push(11);
                    push(12);
                    push(13);
                    push(14);
                    push(15);
                    push(16);
                    push(17);
                    break;
                }
                case PARAM_IR:
                {
                    //������
                    break;
                }
                case ASSIGN:
                {
                    //assign�����࣬һ���ǳ�����ֵtemp��һ���Ǳ�����ֵaddr
                    //һ����temp����������ֵv��һ���Ǳ�����addr����ֵv
                    //ע�⣬����һ�������ڷ�assign����Ҫ�����ͷţ�16Ϊר�ŵ�constreg
                    //�г�����ֱ����16�żĴ���д�����ˣ�������ɵ����г������Ǽ�ȡ����
                    if(op2->kind==INT_CONST)//temp=const���
                    {
                        strcpy(regtable[top].tempvar,res->id);
                        if(cmpreg==2)
                        {
                            //������������������Ĵ�������ռ�û���Ҫ��ֵ���������
                            if(regState[8]==1)
                            {
                                regtable[top].reg=17;
                                regState[9]=1;
                                cmpregindex=17;
                            }
                            else
                            {
                                regtable[top].reg=16;
                                regState[8]=1;
                                cmpregindex=16;
                            }
                            cmpreg--;
                        }
                        else if(cmpreg==1)
                        {
                            regtable[top].reg=cmpregindex;
                            cmpreg--;
                            //��ֵʱ������ͬһ�Ĵ���
                        }
                        else//���漰�Ƚϵĳ����Ĵ�������
                        {
                            if(regState[8]==1)//16�żĴ�����ռ��
                            {
                                regtable[top].reg=17;
                                regState[9]=1;
                            }
                            else
                            {
                                regtable[top].reg=16;
                                regState[8]=1;
                            }
                        }

                        addi(regtable[top].reg,0,op2->const_int);
                        regtable[top++].flag=0;
                    }
                    else if(res->id[0]=='a')//addr=v���
                    {
                        strcpy(regtable[top].tempvar,res->id);
                        regtable[top].reg=allocRegister();
                        regtable[top].flag=1;
                        lw(regtable[top].reg,0,-res->offset,res->level>0);
                        regtable[top].addrreg=allocRegister();
                        addi(regtable[top++].addrreg,0,-res->offset);
                        //level>0��ʾΪ�ֲ�������offset��Ҫ��ebpȡ������offset��gpȡ
                    }
                    else if(res->id[0]=='v'&&op2->id[0]=='t')//v=temp������൱�ڱ�������ֵ���������Ĵ������ͷ�op2�Ĵ�������
                    {
                        int regresult=getopnum(op2->id);

                        sw(regtable[regresult].reg,0,-res->offset,res->level>0);

                        revokeRegister(regtable[regresult].reg);//����ֵ�Ĵ�����Ҫ�ͷ�

                    }
                    else if(res->id[0]=='v'&&op2->id[0]=='a')//v=addr������൱�ڱ����Ա�����ֵ����ʱ����ֵ����reg����ַ����addrreg�����ͷ�
                    {
                        int regresult=getopnum(op2->id);

                        sw(regtable[regresult].reg,0,-res->offset,res->level>0);

                        revokeRegister(regtable[regresult].reg);//����ֵ�Ĵ�����Ҫ�ͷ�

                        revokeRegister(regtable[regresult].addrreg);//����ֵ��ַ�Ĵ�����Ҫ�ͷ�

                    }
                    break;
                }
                case ASSIGN_IR:
                {
                    //�Ĵ���ֵд��洢��
                    int regresult=getopnum(op2->id);
                    if(regresult==-1)
                        printf("ASSIGN_IR error\n");

                    int temp=regtable[regresult].reg;//op2�ļĴ������
                    int regresult2=getopnum(op1->id);//������Ҫ�ǻ�ȡ��ַ��ͬʱΪ�ͷżĴ���������׼��
                    int temp2=regtable[regresult2].reg;
                    int temp3=regtable[regresult2].addrreg;//op1�ļĴ������
                    //�ǳ���:op2�Ĵ������ݣ�д��op1��Ӧ��ַ���ݼ���
                    sw(temp,temp3,0,op1->level>0);//�����ƫ������ַд�ڼĴ�����Ҫ������29/28�żĴ������

                    revokeRegister(temp2);//�ͷ�op1����ַ��ֵ���Ĵ���
                    revokeRegister(temp3);//�ͷ�op1����ַ��ֵ���Ĵ���

                    //��op2��Ӧ�ļĴ���д��res��Ӧ����ʱ�����ļĴ���
                    regtable[top].reg=temp;
                    regtable[top].flag=0;
                    strcpy(regtable[top++].tempvar,res->id);

                    break;
                }
                case ARG_IR:
                {
                    //��֮�г�������16,17�żĴ���д
                    int regresult=getopnum(res->id);

                    push(regtable[regresult].reg);
                    revokeRegister(regtable[regresult].reg);
                    break;
                }
                case CALL_IR:
                {
                    //˳��$31��ջ��ebp��ջ��ebp=esp��������ջ֡������������jal
                    //��ʵ����system��Ϊ����ԭ�ͣ�������ֱ�Ӵ�ӡ


                    if(!strcmp(op2->id,"system"))
                    {
                        jal("system");
                    }
                    else
                    {
                        jal(op2->id);

                        if(res->kind!=0)
                        {
                            regtable[top].reg=allocRegister();//��������ֵ�̶������2�żĴ���
                            strcpy(regtable[top].tempvar,res->id);
                            regtable[top].flag=0;
                            addi(regtable[top++].reg,2,0);
                        }
                    }
                    break;
                }
                case ARR_IR:
                {
                    //���������ַ��Ľ������
                    //ĳ����ֵassignʱ�������ṹ�����飩������ֵ����ĳЩ�Ĵ����������ַҲ����ڵ�
                    //18λ��ַ�Ĵ���
                    int offsetresult=getopnum(op2->id);//��ô��op2ֵ�ļĴ�������op2����

                    add(18,0,regtable[offsetresult].reg);
                    sllv(18,18,2);//��ַ������λ����ֽڵ�ַ
                    sub(18,0,18);//ƫ����Ϊ��
                    addi(18,18,-op1->offset);
                    regtable[top].reg=allocRegister();
                    strcpy(regtable[top].tempvar,res->id);
                    lw(regtable[top].reg,18,0,op1->level>0);
                    regtable[top].addrreg=allocRegister();
                    addi(regtable[top++].addrreg,0,18);
                    revokeRegister(regtable[offsetresult].reg);//�ͷ��ݴ��ַ�ļĴ���

                    break;
                }
                case STRUCT_IR:
                {
                    //����ƫ�����Ѿ�д��addr������棬���ﲻ������Ŀ�����
                    /*��ʱ����*/
                    int addrconst=res->offset;

                    regtable[top].reg=allocRegister();
                    strcpy(regtable[top].tempvar,res->id);
                    lw(regtable[top].reg,0,addrconst,op1->level>0);
                    regtable[top].addrreg=allocRegister();
                    add(regtable[top++].addrreg,0,res->offset);
                    break;
                }
                case RETURN_IR:
                {
                    //˳�򣺷���ֵд��2�żĴ������ָ��ֳ���esp=ebp,pop ebp��pop $31��esp��ȥ�����ϣ���������*4���ֽڣ��Ѵ洢�ڽڵ㣩
                    int regresult=getopnum(res->id);
                    add(2,0,regtable[regresult].reg);//����ֵд��2�żĴ���

                    pop(17);
                    pop(16);
                    pop(15);
                    pop(14);
                    pop(13);
                    pop(12);
                    pop(11);
                    pop(10);
                    pop(9);
                    pop(8);

                    addi(30,29,0);//esp=ebp

                    pop(29);//pop(ebp)
                    pop(31);//pop(esp)
                    addi(30,30,res->offset);//��������Ĩ��


                    jr(31);
                    revokeRegister(regtable[regresult].reg);

                    break;
                }
                case IF_IR:
                {
                    int regresult=getopnum(res->id);

                    beq(regtable[regresult].reg,0,op1->id);
                    revokeRegister(regtable[regresult].reg);//�˴����ɣ��Դ����

                    break;
                }
                case LABEL_IR:
                {
                    label(res->id);
                    break;
                }
                case GOTO_IR:
                {
                    j(res->id);
                    break;
                }
                case PLUS:
                case MINUS:
                case LSHFIT:
                case RSHFIT:
                case AND:
                case OR:
                {
                    //�����еļĴ����ͷŹ���
                    //��ֵ�Ĵ�����Ϊ�������Ĵ洢
                    //��ֵ�Ĵ������ǵ�ַ�Ĵ��������ͷ�
                    //����������Ĵ����б���һ�������ͷ�
                    int regresult1=getopnum(op1->id);
                    int reg1,reg2;
                    reg1=regtable[regresult1].reg;

                    int regresult2=getopnum(op2->id);
                    reg2=regtable[regresult2].reg;


                    regtable[top].reg=reg2;
                    strcpy(regtable[top].tempvar,res->id);
                    regtable[top].flag=0;

                    switch(head->op)
                    {
                        case PLUS:
                            add(regtable[top++].reg,reg1,reg2);
                            break;
                        case MINUS:
                            sub(regtable[top++].reg,reg1,reg2);
                            break;
                        case LSHFIT:
                            sllv(regtable[top++].reg,reg1,reg2);
                            break;
                        case RSHFIT:
                            srlv(regtable[top++].reg,reg1,reg2);
                            break;
                        case AND:
                            and(regtable[top++].reg,reg1,reg2);
                            break;
                        case OR:
                            or(regtable[top++].reg,reg1,reg2);
                            break;
                    }
                    if(regtable[regresult1].flag==0)
                    {
                        revokeRegister(reg1);
                    }
                    break;
                }
                case EQ_IR:
                case NE_IR:
                case GT_IR:
                case GE_IR:
                case LT_IR:
                case LE_IR:
                {

                    int regresult1=getopnum(op1->id);
                    int reg1,reg2;
                    reg1=regtable[regresult1].reg;


                    int regresult2=getopnum(op2->id);

                    reg2=regtable[regresult2].reg;

                    cmpreg=2;//ÿ����һ�θ�ֵ��1
                    switch(head->op)
                    {
                        case EQ_IR:
                            beq(reg1,reg2,res->id);
                            break;
                        case NE_IR:
                            bne(reg1,reg2,res->id);
                            break;
                        case GT_IR:
                            bgt(reg1,reg2,res->id);
                            break;
                        case GE_IR:
                            bge(reg1,reg2,res->id);
                            break;
                        case LT_IR:
                            blt(reg1,reg2,res->id);
                            break;
                        case LE_IR:
                            ble(reg1,reg2,res->id);
                            break;
                    }
                    revokeRegister(reg1);
                    revokeRegister(reg2);

                    break;
                }
                case MUL:
                case DIV:
                case MOD:
                {
                    printf("Not support opreator\n");
                    break;
                }
                case END_IR:
                {
                    //���������return
                    return;
                    break;
                }
                default:
                    break;
            }
            head=head->next;

        }


    fclose(fp);
}

/*
�õ��ļĴ�����
$2��$v0����RETURN�ķ���ֵ
$8-$15����$t0-$t7��:��ʱ�����Ĵ���,8����ȥ������Ӧ�ù��ã�
�����¹涨��8,9,10Ϊ��ʱ�����Ĵ�����rs=8,rt=9,rd=10


$16-$23 :$16Ϊ�����Ĵ��������ڴ洢��ʱ������������
$17Ϊ��ַ�Ĵ������洢������ַ�����飬�ṹ��������
$18Ϊ���������Ĵ���
$28:ȫ�ֱ�����ַ����ʼʱ��Ϊ0x200
$29:%ebp������ʼʱ��Ϊ0x800
$30:%esp������ʼʱ��Ϊ0x800
��main���������Σ����⴦����0x800��Ϊ�洢main�����ߵ�ebp��main�ľֲ�������0x7fc��ʼ��
main�ķ��ص�ַָ��0x0��0x0�洢һ��ͣ��ָ�Ȼ���ٿ�ʼ�������ɣ�
$31:���ص�ַ

*/

int getframesize(struct codenode* root,char* s)
{
    if(!strcmp(s,"system"))
        return 0;

    while(root->op!=END_IR)
    {
        if(root->op==FUNC_IR&&(!strcmp(root->result.id,s)))
            return root->result.offset;
        else
            root=root->next;
    }
    printf("debug error 6\n");
    return -1;
}


/*
����ʱ�洢��֯��
�Ӹߵ�ַ���͵�ַ��������ջ֡������n......����1������ջ֡�����˴�Ϊ��ǰ����ebp��������ebp���ֲ�����......
���ص�ַ����$31��espָ��ջ֡ĩβ�����Խ��в�����ջ����
�����X86��Ҫ�Ƿ��ص�ַ�ĸı�
����Ŀ�����ʱҪע���жϺ���ʹ�õļĴ����ĸ���
*/
int allocRegister()
{
    for(int i=0;i<8;i++)
    {
        if(regState[i]==0)
        {
            regState[i]=1;
            return i+8;
        }
    }
    printf("reg is not enough for use\n");
    return -1;
}

/*
�Ĵ�������ԭ��
1.return ��arg������������
2.�Ƚ�ָ������ֵ
3.��ֵָ������ֵ���ǽ��ֵ����������ַ�Ĵ�������ֵ�Ĵ�����

*/
void revokeRegister(int reg)
{
    for(int i=0;i<top;i++)
    {
        if(regtable[i].reg==reg&&regtable[i].flag==1)
        {
            regtable[i].flag=0;
            if(regtable[i].tempvar[0]=='a')//��ַ����
            {
                regState[regtable[i].addrreg-8]=0;
            }
        }

    }
    regState[reg-8]=0;
}

/*����tvar��ȡ��Ĵ���������Ϊ�Ĵ�������0��Ϊ��������1*/
int getopnum(char* tvar)
{
    for(int i=0;i<top;i++)
    {
        if(!strcmp(regtable[i].tempvar,tvar))
        {
            return i;
        }
    }
    return -1;
}

int getregtableindex(char* tvar)
{
    for(int i=0;i<top;i++)
    {
        if(!strcmp(regtable[i].tempvar,tvar)&&regtable[i].flag!=0)
        {
            return i;
        }
    }
    return -1;
}

int getBaseBlock(struct codenode* root)
{
    //������
    if(head==NULL)
        head=root;
    else
    {
        if(tail->op!=END_IR)
            head=tail->next;
        else
            return 0;
        while(head->op!=END_IR)
        {
            head=head->next;
            if(head->flag=='H')
                break;
        }
    }

    if(head->op==END_IR)
    {
        return 0;
    }
    //��β���

    tail=head->next;
    while(tail->op!=END_IR)
    {
        if(tail->flag=='H')
        {
            tail=tail->prior;
            break;
        }
        else if((tail->op>=IF_IR&&tail->op<=LE_IR)||tail->op==GOTO_IR)
        {
            break;
        }
        tail=tail->next;
    }
    return 1;
}


void sw(int rd,int rt,int offset,int base)
{
    if(base>0)
    {
        add(18,0,rt);
        add(18,18,29);
        sprintf(targetcode,"sw $%d,%d($18)\n",rd,offset);
    }
    else
    {
        add(18,0,rt);
        add(18,18,28);
        sprintf(targetcode,"sw $%d,%d($18)\n",rd,offset);
    }
    recordcode();
}

void lw(int rd,int rt,int offset,int base)
{
    if(base>0)
    {
        add(18,0,rt);
        add(18,18,29);
        sprintf(targetcode,"lw $%d,%d($18)\n",rd,offset);
    }
    else
    {
        add(18,0,rt);
        add(18,18,28);
        sprintf(targetcode,"lw $%d,%d($18)\n",rd,offset);
    }
    recordcode();
}

void push(int rd)
{
    sprintf(targetcode,"addi $30,$30,-4\n");
    recordcode();
    sprintf(targetcode,"sw $%d,0($30)\n",rd);
    recordcode();
}

void pop(int rd)
{
    sprintf(targetcode,"lw $%d,0($30)\n",rd);
    recordcode();
    sprintf(targetcode,"addi $30,$30,4\n");
    recordcode();
}

void label(char* s)
{
    sprintf(targetcode,"%s:\n",s);
    recordcode();
}

void addi(int rd,int rs,int imm)
{
    sprintf(targetcode,"addi $%d,$%d,%d\n",rd,rs,imm);
    recordcode();
}

void subi(int rd,int rs,int imm)
{
    sprintf(targetcode,"subi $%d,$%d,%d\n",rd,rs,imm);
    recordcode();
}

void add(int rd,int rs,int rt)
{
    sprintf(targetcode,"add $%d,$%d,$%d\n",rd,rs,rt);
    recordcode();
}

void sub(int rd,int rs,int rt)
{
    sprintf(targetcode,"sub $%d,$%d,$%d\n",rd,rs,rt);
    recordcode();
}

void sllv(int rd,int rs,int rt)
{
    sprintf(targetcode,"sllv $%d,$%d,$%d\n",rd,rs,rt);
    recordcode();
}

void srlv(int rd,int rs,int rt)
{
    sprintf(targetcode,"srlv $%d,$%d,$%d\n",rd,rs,rt);
    recordcode();
}

void and(int rd,int rs,int rt)
{
    sprintf(targetcode,"and $%d,$%d,$%d\n",rd,rs,rt);
    recordcode();
}

void or(int rd,int rs,int rt)
{
    sprintf(targetcode,"or $%d,$%d,$%d\n",rd,rs,rt);
    recordcode();
}

void jr(int rd)
{
    sprintf(targetcode,"jr $%d\n",rd);
    recordcode();
}
void j(char* s)
{
    sprintf(targetcode,"j %s\n",s);
    recordcode();
}

void jal(char* s)
{
    sprintf(targetcode,"jal %s\n",s);
    recordcode();
}

void beq(int rd,int rt,char* s)
{
    sprintf(targetcode,"beq $%d,$%d,%s\n",rd,rt,s);
    recordcode();
}

void bne(int rd,int rt,char* s)
{
    sprintf(targetcode,"bne $%d,$%d,%s\n",rd,rt,s);
    recordcode();
}

void bgt(int rd,int rt,char* s)
{
    sprintf(targetcode,"bgt $%d,$%d,%s\n",rd,rt,s);
    recordcode();
}

void bge(int rd,int rt,char* s)
{
    sprintf(targetcode,"bge $%d,$%d,%s\n",rd,rt,s);
    recordcode();
}

void blt(int rd,int rt,char* s)
{
    sprintf(targetcode,"blt $%d,$%d,%s\n",rd,rt,s);
    recordcode();
}

void ble(int rd,int rt,char* s)
{
    sprintf(targetcode,"ble $%d,$%d,%s\n",rd,rt,s);
    recordcode();
}

void recordcode()
{
    printf("%s",targetcode);
    fprintf(fp,"%s",targetcode);
    //�ض����ļ�
}
