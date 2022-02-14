//#include <vcl.h>
#include <stdio.h>
//#include "brd.h"
#include "Bsp_common.h"
#include "library_k.h"
#include "Options.h"
#include "my_structs.h"
#include "dsp_param.h"
#include "buf.h"
#include <complex>
#include <ctime>

#include "sundries.h"
using namespace std;
//void bpf0(unsigned m,int mode,double t,complex<double> *x);


#define TIME_OUT 0xffffff
//#include "d:\cpp\model.h"
//void Graf(complex<double> *s,long N0,double FD,int F,char *text,double x1,double x2,...);
//extern Form_data FormData; // не забыть проинициализировать!!!
extern volatile PBsp_Status	status;
extern int d_count;
extern unsigned int k;
extern string ProgPath;
extern string ProgrammPath;
extern BRD_Handle handle;
extern int err;
extern time_t tim;
extern FILE *flog,*ff;
extern int start; // запуск с FOrm4
extern PARAM_DSP dsp_param, dsp_param_1;
extern unsigned int  Daln_HM,Daln_MONO, sizeHM, sizeMONO;
extern int size_HM,size_MONO;
extern char ss[256];
extern int z;
extern unsigned int size;
//extern int mes[4],mes1[4];
extern int *p11,*p21;
extern char fname[256];
extern unsigned int items;// Число записей программы ДДС
extern unsigned int g_aProgArray[1000+8];// Массив для хранения программы ДДС
extern unsigned int g_aProg[1000+8]; // Для проверки правильности ввода

extern complex<float> KU_HM_NU[L_LEN],KU_HM_VU[L_LEN],KU_MONO_NU[M_LEN],KU_MONO_VU[M_LEN];
extern complex<float> KU_HM_NU_1[L_LEN];
extern complex<float> Filter_MTD[2*10*nK*Lmax],Filter_MTD1[2*10*nK*Lmax];
extern complex<float> zz;
extern int Period[10],Period1[10];
int qrt=0; // 1 - работа в автономном режиме (отображение из файла)
int *q1,*q2;
complex<short int> test_sig[500],test_sig1[500];
complex<double> CC;
int k_test_hm1,k_test_hm2,k_test_mono1,k_test_mono2;
//double xx,xxm;
//int M0;long N0;
//extern int otobrajenie;;
extern char fini[256];
extern int LL;
extern int nT,nKK;
extern float sig;
//nach_lo заменил на return 1;
//konez заменил на return 2;
//если все норма, то return 0;
extern struct okno
{
        float mnoj_okna;//,mno;
        int prop,L_Buf,n;
} okno, oknom;
struct
{
        float mnoj_okna;//,mno;
        int prop,L_Buf,n;
}okno1;
extern int tip_obr;
static int wwq=0;
int ShtdnRemSystem(char *name=NULL, bool force_opt=false,
                                    bool reboot_opt=true);
int init_plata(void)
{
    double xx,xxm;
    int chet_err=0;
    int mes[4];
    BSP_RESET(status);
    BSP_SET_STATUS( status, cmdWORK );
    BSP_SET_D_COUNT( status, d_count++ );


        if(sagr()!=0) return 2; // по идее, надо идти в конец
                                //здесь return в никуда !!! Вернуться и проработать, иначе возврат в никуда
        ProgPath = ProgrammPath+"p1.tsx";
    k=0;
    /*
    if(otobrajenie==1)
    {
        Form4->OpenDialog1->Filter="Не сжатые (*.bin)|*.bin|Сжатые (*.dsp)|*.dsp";
        if(Form4->OpenDialog1->Execute())
                ff=fopen(Form4->OpenDialog1->FileName.c_str(),"rb");
        else
                return 2;
    }
    */
    //if(otobrajenie==0)
    for(k=0;k<5;k++)
    {
        if(Ini_Start((char*) ProgPath.c_str())!=0)
        {
            BRD_reset(handle,0);
            BRD_reset(handle,1);
            err=BRD_cleanup();
            chet_err=0;
            while(!BRD_errcmp(err,BRDerr_OK) && chet_err<5)
            {
               err=BRD_cleanup();
               chet_err++;
            }
						time(&tim);// =Now();
            fprintf(flog,"\n%-60s%s","Ошибки при инициализации платы ",DateTimeToStr(tim).c_str());
            fflush(flog);

            //FormData.flag_work_PK = 0; //  Form4->Shape1->Brush->Color=clRed;
        }
        else
            break;
    }

// 1
    //FormData.progress_bar_position = 15;
    if(k==5)
    {
        //BRD_cleanup();
        MessageBox(NULL,"Ошибка инициализации платы","Error",MB_OK);
        //FormData.flag_work_PK = 0; //  Form4->Shape1->Brush->Color=clRed;
        start=0;
        return 2; //goto konez;
    }
    q1=(int*)&test_sig;
    q2=(int*)&test_sig1;
    
    //M0=11;

    //N0=1<<M0;
    //double FD=1.5625e6;
    //complex<double> *s1,*s2;
    //s1=new complex<double>[N0];
    //s2=new complex<double>[N0];
    k_test_hm1=q1[0]>>16;
    k_test_hm2=q1[k_test_hm1+1]>>16;
    k_test_mono1=q1[k_test_hm1+k_test_hm2+2]>>16;
    k_test_mono2=q1[k_test_hm1+k_test_hm2+k_test_mono1+3]>>16;
    //k_test_hm1>>=2;
    //k_test_hm1=(k_test_hm1+1)<<2;
    /*
    for(int i=1;i<k_test_hm1;i++)
    {
        CC=test_sig[i];
        s1[i-1]=CC;
    }
    Graf(s1,N0,FD,0,"Сигнал",0,2*k_test_hm1/FD,0);
    */
    
    xx=dsp_param.aMCLOCK/(dsp_param.aDECIMATION*1.5e8); // Это для ЧМ f=1.5625e6
    if(dsp_param.aDECIMONO==32)
        xxm= xx* dsp_param.aDECIMATION /dsp_param.aDECIMONO;
    else
        xxm=xx;
    //if(dsp_param.aMCLOCK==100000000)
    //    xx/=2;
    Daln_HM=dsp_param.Daln_HM*xx+0.5;	// Число отсчетов дистанции ЧМ
    dsp_param.dal_ogr_HM=dsp_param.dal_ogr_HM*xx+0.5;	// Число отсчетов дистанции ограничения ЧМ
    Daln_MONO=dsp_param.Daln_MONO*xxm+0.5;// Число отсчетов дистанции МОНО
    dsp_param.Daln_VU*=xx;
        //dsp_param.Ddal=dsp_param.Ddal*xx+0.5;
        //dsp_param.DMax=dsp_param.DMax*xx+0.5;
        //dsp_param.DELTAD=dsp_param.DELTAD*xx+0.5;
        //dsp_param.md=dsp_param.md*xx+0.5;
    // Число отсчетов дистанции увеличено за счет задержки
    // Число отсчетов ЧМ сигнала кратная 4  должно быть кратно 8
    sizeHM=(Daln_HM+dsp_param.zad_lhm+k_test_hm1);
    size_HM=(Daln_HM+k_test_hm1);
    //sizeHM += k_test_hm1;
    /*
    if(dsp_param.aMCLOCK==100000000)
    {
        sizeHM += k_test_hm1+dsp_param.zad_lhm;
        size_HM += k_test_hm1;
        size_HM>>=1;  // Подгонка под 50 мГц
    }
    */
    //size_HM+=k_test_hm1;
    sizeHM>>=2;
    sizeHM=(sizeHM+1)<<2;
    /*
    if(dsp_param.aMCLOCK==100000000)
        sizeHM*=2;
    */
    size_HM&=0xfffffffc;
    if(size_HM> L_LEN)//+k_test_hm1>N)
    {
        sprintf(ss,"Дальность ЧМ слишком велика. Мах=%.0f",(L_LEN - k_test_hm1)/xx);
        //MessageBox(NULL,ss,"Ошибка",MB_OK);

        size_HM = L_LEN  - k_test_hm1;
        sprintf(ss,"Исправленная дальность ЧМ=%.0f",(L_LEN - k_test_hm1)/xx);
        WritePrivateProfileString("Дальность","ЧМ", std::to_string(size_HM).c_str() , fini);
        //MessageBox(NULL,ss,"Сообщение",MB_OK);
        fprintf(flog,"\n%s",ss);

       // start=0;
       // goto konez;
    }

// 2
    //FormData.progress_bar_position = 30;

    dsp_param.sizeHM = sizeHM; // Кол отсчетов одного канала, принимаемых из ДДС
    dsp_param.size_HM = size_HM;
    //if(dsp_param.aMCLOCK==100000000)
    //		xx/=2;
    z=dsp_param.d_Wary1;
    z*=xx;
    z=(z+4)>>2;
    z=(z+1)<<2;
    dsp_param.d_Wary1=z;

    z=dsp_param.d_Wary2;
    z*=xx;
    z=(z+4)>>2;
    z=(z+1)<<2;
    dsp_param.d_Wary2=z;

    z=dsp_param.d_Wary1m;
    z*=xxm;
    z=(z+4)>>2;
    z=(z+1)<<2;
    dsp_param.d_Wary1m=z;

    z=dsp_param.d_Wary2m;
    z*=xxm;
    z=(z+4)>>2;
    z=(z+1)<<2;
    dsp_param.d_Wary2m=z;

    sizeMONO=(Daln_MONO+dsp_param.zad_mono+k_test_mono1)>>2; // Число отсчетов дистанции увеличено за счет задержки
    sizeMONO=(sizeMONO+1)<<2;
    size_MONO=sizeMONO-dsp_param.zad_mono;     //Сигнал должен быть соответсвующей длительности
    size_MONO&=0xfffffffc;
    if(size_MONO>M_LEN)//+k_test_mono1>M)
    {
        sprintf(ss,"Дальность МОНО слишком велика. Мах=%.0f", (M_LEN -k_test_mono1)/xxm);
        //MessageBox(NULL,ss,"Error",MB_OK);

        size_MONO = M_LEN - k_test_mono1;
        z=(float)(M_LEN - k_test_mono1)/(float)xxm;
        sprintf(ss,"Исправленная дальность МОНО=%d",z);//(M-k_test_mono1)/xxm);
        WritePrivateProfileString("Дальность","МОНО",std::to_string(z).c_str(),fini);
        //MessageBox(NULL,ss,"Сообщение",MB_OK);
        fprintf(flog,"\n%s",ss);
        //start=0;
        //goto konez;
    }

// 3
    //FormData.progress_bar_position = 40;

    dsp_param.sizeMONO = sizeMONO;  // Кол отсчетов одного канала, принимаемых из ДДС
    dsp_param.size_MONO = size_MONO;
    //MessageBox(NULL,"Нажмите ОК","Message",MB_OK);
    size = sizeof(mes);
    //if(otobrajenie==1)
    //    return 0;
    for(int i=0;i<4;i++)
        mes[i]=0;
    Sleep(1000);
    BRD_getMsg(handle,0, &mes,&size,0x7ffff);//TIME_OUT);
    // !!!! НУЖНА ЛИ ВЕРСИЯ ПРОШИВКИ ВООБЩЕ??? Form1->Label2->Caption="Версия прошивки="+IntToStr(mes[1]);
    if(mes[0]==707)
    {
        //MessageBox(NULL,"Ошибки при открытии линка на передачу","Error",MB_OK);

        BRD_reset(handle,0);
        BRD_reset(handle,1);
        err=BRD_cleanup();
        while(!BRD_errcmp(err,BRDerr_OK))
               err=BRD_cleanup();
        time(&tim);;
        fprintf(flog,"\n%-60s%s","Ошибки при открытии линка на передачу ",DateTimeToStr(tim).c_str());
        fflush(flog);
        //FormData.flag_work_PK=0;
        return 1; //goto nach_lo;

    }

// 4
    //FormData.progress_bar_position=55;

    if(mes[0]==606)
    {
        //MessageBox(NULL,"Ошибки при открытии линка на прием","Error",MB_OK);
        BRD_reset(handle,0);
        BRD_reset(handle,1);
        err=BRD_cleanup();
        while(!BRD_errcmp(err,BRDerr_OK))
               err=BRD_cleanup();
        time(&tim);;
        fprintf(flog,"\n%-60s%s","Ошибки при открытии линка на прием ",DateTimeToStr(tim).c_str());
        fflush(flog);
        //FormData.flag_work_PK=0;   // !!! ОШИБКА  Form1->Shape1->Brush->Color=clRed;
        return 1; //goto nach_lo;
    }
    if(mes[0]!=777)
    {
        //MessageBox(NULL,"Ошибки при передаче сигнала готовности во 2 процессор","Message",MB_OK);
        BRD_reset(handle,0);
        BRD_reset(handle,1);
        err=BRD_cleanup();
        while(!BRD_errcmp(err,BRDerr_OK))
               err=BRD_cleanup();
        time(&tim);;
        fprintf(flog,"\n%-60s%s","Ошибки при передаче сигнала готовности во 2 процессор ",DateTimeToStr(tim).c_str());
        fflush(flog);
        wwq++;
        if(wwq>=3)
        {
                time(&tim);;
                fprintf(flog,"\nОшибки при передаче сигнала готовности во 2 процессор");
                fprintf(flog,"\n%-60s%s","Перезагрузка ОС! ",DateTimeToStr(tim).c_str());
                fflush(flog);
                WritePrivateProfileString("Bsp", "Exit", "1", fname);
                //ExitWindows(EW_RESTARTWINDOWS,0);
                ShtdnRemSystem(NULL,false,true);
        }
        return 1; //goto nach_lo;
    }
    size=sizeof(dsp_param )+16;
    
    size&=0xfffffff0;
    k=size;
    //Sleep(1000);
    BRD_putMsg(handle,0, &dsp_param.Daln_HM,&size,TIME_OUT); // Параметр size может быть испорчен
    //MessageBox(NULL,"Ошибки при передаче коэффициентов KU_MONO_VU","Message",MB_OK);
    size=k;
    BRD_getMsg(handle,0, &dsp_param_1.Daln_HM,&k,TIME_OUT);



    // сравниваем побайтно по XOR то, что передали и то, что считали...
    p11=(int*)&dsp_param ;
    p21=(int*)&dsp_param_1;
    z=0;
     for(unsigned int i=0;i<size/sizeof(int);i++)
        z+=p11[i]^p21[i];

    if(z!=0)
    {
        //MessageBox(NULL,"Ошибки при передаче параметров ДДС","Message",MB_OK);
        BRD_reset(handle,0);
        BRD_reset(handle,1);
        err=BRD_cleanup();
        while(!BRD_errcmp(err,BRDerr_OK))
               err=BRD_cleanup();
        time(&tim);;
        fprintf(flog,"\n%-60s%s","Ошибки при передаче параметров ДДС ",DateTimeToStr(tim).c_str());
        fflush(flog);
        //FormData.flag_work_PK=0;   // !!! ОШИБКА  Form1->Shape1->Brush->Color=clRed;
        return 1; //goto nach_lo;
    }


///////////////////////////////////////////////////////////////
// 5
    //FormData.progress_bar_position=65;

    // Передадим в процессор программу ДДС
    //size=sizeof(items );
    //BRD_putMsg(handle,0,&items ,&size,TIME_OUT);
    //items*=sizeof(int);
    items=dsp_param.items*sizeof(int);

    BRD_putMsg(handle,0, g_aProgArray,&items,TIME_OUT);
    items=dsp_param.items*sizeof(int);
    BRD_getMsg(handle,0, g_aProg,&items,TIME_OUT);
    //p11=(char*)g_aProgArray ;
    //p21=(char*)g_aProg;
    p11=(int*)g_aProgArray ;
    p21=(int*)g_aProg;
    z=0;
    for(unsigned int i=0;i<dsp_param.items;i++)
        z+=p11[i]^p21[i];

    if(z!=0)
    {
        //MessageBox(NULL,"Ошибки при передаче программы ДДС","Message",MB_OK);
        BRD_reset(handle,0);
        BRD_reset(handle,1);
        err=BRD_cleanup();
        while(!BRD_errcmp(err,BRDerr_OK))
               err=BRD_cleanup();
        time(&tim);;
        fprintf(flog,"\n%-60s%s","Ошибки при передаче программы ДДС ",DateTimeToStr(tim).c_str());
        fflush(flog);
        //FormData.flag_work_PK=0;   // !!! ОШИБКА  Form1->Shape1->Brush->Color=clRed;
        return 1; //goto nach_lo;
    }

    // Передадим в процессор тестовые сигналы
    size=sizeof(test_sig);
    size&=0xfffffff0;
    k=size;
    q1=(int*)&test_sig;
    q2=(int*)&test_sig1;

    BRD_putMsg(handle,0,(void*)q1 ,&size,TIME_OUT);
    size=k;
    BRD_getMsg(handle,0,(void*)q2 ,&k,TIME_OUT);
    //k_test_hm1=q1[0]>>16;
    z=0;
    for(unsigned int i=0;i<size/sizeof(float);i++)
    {
        z+=q1[i]^q2[i];
        if(z!=0)
            z=z;
    }

    if(z!=0)
    {

        BRD_reset(handle,0);
        BRD_reset(handle,1);
        err=BRD_cleanup();
        while(!BRD_errcmp(err,BRDerr_OK))
               err=BRD_cleanup();
        time(&tim);;
        fprintf(flog,"\n%-60s%s","Ошибки при передаче тестового сигнала ",DateTimeToStr(tim).c_str());
        fflush(flog);
        //FormData.flag_work_PK=0;   // !!! ОШИБКА  Form1->Shape1->Brush->Color=clRed;
        return 1; //goto nach_lo;
    }

    // Передаем в процессор коэффициенты передачи сжимающих фильтров
    size=sizeof(KU_HM_NU);
    BRD_putMsg(handle,0,KU_HM_NU ,&size,TIME_OUT);
    BRD_getMsg(handle,0,KU_HM_NU_1 ,&size,TIME_OUT);
    q1=(int*)KU_HM_NU;
    q2=(int*)KU_HM_NU_1;
    z=0; zz=0;
    for(unsigned int i=0;i<size/sizeof(float);i++)
    {
        zz+=KU_HM_NU[i]-KU_HM_NU_1[i];
        z+=q1[i]-q2[i];
        if(z!=0)
            z=z;
    }

    if(z!=0)
    {
        //MessageBox(NULL,"Ошибки при передаче коэффициентов KU_HM_NU","Message",MB_OK);
        BRD_reset(handle,0);
        BRD_reset(handle,1);
        err=BRD_cleanup();
        while(!BRD_errcmp(err,BRDerr_OK))
               err=BRD_cleanup();
        time(&tim);;
        fprintf(flog,"\n%-60s%s","Ошибки при передаче коэффициентов KU_HM_NU ",DateTimeToStr(tim).c_str());
        fflush(flog);
        //FormData.flag_work_PK=0;   // !!! ОШИБКА  Form1->Shape1->Brush->Color=clRed;
        return 1; //goto nach_lo;
    }

    BRD_putMsg(handle,0,KU_HM_VU ,&size,TIME_OUT);
    BRD_getMsg(handle,0,KU_HM_NU_1 ,&size,TIME_OUT);
    q1=(int*)KU_HM_VU;
    q2=(int*)KU_HM_NU_1;
    z=0;
    for(unsigned int i=0;i<size/sizeof(float);i++)
        z+=q1[i]-q2[i];

    if(z!=0)
    {
        //MessageBox(NULL,"Ошибки при передаче коэффициентов KU_HM_VU","Message",MB_OK);
        BRD_reset(handle,0);
        BRD_reset(handle,1);
        err=BRD_cleanup();
        while(!BRD_errcmp(err,BRDerr_OK))
               err=BRD_cleanup();
        time(&tim);
        fprintf(flog,"\n%-60s%s","Ошибки при передаче коэффициентов KU_HM_VU ",DateTimeToStr(tim).c_str());
        fflush(flog);
        //FormData.flag_work_PK=0;   // !!! ОШИБКА  Form1->Shape1->Brush->Color=clRed;
        return 1; //goto nach_lo;
    }
/*
// Проверка правильности передачи коэффициента сжимающих фильтров и тестового сигнала
{
FILE *uu,*uy;
uu=fopen("sig.txt","w");
uy=fopen("filt.txt","w");
        complex<double>s1[2048],s2[2048];
        float x,y;
        int qqq1,qqq2,*w;

        w=(int*)test_sig;
        k_test_hm1=w[0]>>16;
        fprintf(uu,"%d\n",k_test_hm1);
        for(int i=0;i<k_test_hm1;i++)
        {
           qqq1=(w[i+1]<<16)>>16;
           qqq2=w[i+1]>>16;
           fprintf(uu,"%d  %d\n",qqq1,qqq2);
           s1[i+200]=complex<double>(qqq1,qqq2);
        }


        for(int i=k_test_hm1+200;i<2048;i++)
                s1[i]=0;
        bpf0(11,-1,1,s1);
        for(int i=0;i<2048;i++)
        {
                s1[i]*= complex<double>(KU_HM_NU[i].real(),KU_HM_NU[i].imag());
                fprintf(uy,"%f  %f\n",KU_HM_NU[i].real(),KU_HM_NU[i].imag() );
        }
        fclose(uu);
        fclose(uy);
        bpf0(11,1,1,s1);
        Graf(s1,2048,1,0,"Сжатый сигнал NU",0,2048,0);
}
*/
    size=sizeof(KU_MONO_NU);
    BRD_putMsg(handle,0,KU_MONO_NU ,&size,TIME_OUT);
    BRD_getMsg(handle,0,KU_HM_NU_1 ,&size,TIME_OUT);
    q1=(int*)KU_MONO_NU;
    q2=(int*)KU_HM_NU_1;
    z=0;
    for(unsigned int i=0;i<size/sizeof(float);i++)
        z+=q1[i]-q2[i];

// 6
    //FormData.progress_bar_position=85;

    if(z!=0)
    {
        //MessageBox(NULL,"Ошибки при передаче коэффициентов KU_MONO_NU","Message",MB_OK);
        BRD_reset(handle,0);
        BRD_reset(handle,1);
        err=BRD_cleanup();
        while(!BRD_errcmp(err,BRDerr_OK))
               err=BRD_cleanup();
        time(&tim);
        fprintf(flog,"\n%-60s%s","Ошибки при передаче коэффициентов KU_HM_NU ",DateTimeToStr(tim).c_str());
        fflush(flog);
        //FormData.flag_work_PK=0;   // !!! ОШИБКА  Form1->Shape1->Brush->Color=clRed;
        return 1; //goto nach_lo;
    }

    //MessageBox(NULL,"Ошибки при передаче коэффициентов KU_MONO_VU","Message",MB_OK);
    BRD_putMsg(handle,0,KU_MONO_VU ,&size,TIME_OUT);
    BRD_getMsg(handle,0,KU_HM_NU_1 ,&size,TIME_OUT);
    q1=(int*)KU_MONO_VU;
    q2=(int*)KU_HM_NU_1;
    z=0;
    for(unsigned int i=0;i<size/sizeof(float);i++)
        z+=q1[i]-q2[i];

    if(z!=0)
    {
        //MessageBox(NULL,"Ошибки при передаче коэффициентов KU_MONO_VU","Message",MB_OK);
        BRD_reset(handle,0);
        BRD_reset(handle,1);
        err=BRD_cleanup();
        while(!BRD_errcmp(err,BRDerr_OK))
               err=BRD_cleanup();
        time(&tim);
        fprintf(flog,"\n%-60s%s","Ошибки при передаче коэффициентов KU_MONO_VU ",DateTimeToStr(tim).c_str());
        fflush(flog);
        //FormData.flag_work_PK=0;   // !!! ОШИБКА  Form1->Shape1->Brush->Color=clRed;
        return 1; //goto nach_lo;
    }

    // Передаем в процессор массив периодов повторения
    size=8*sizeof(int);
    BRD_putMsg(handle,0,Period ,&size,TIME_OUT);
    BRD_getMsg(handle,0,Period1 ,&size,TIME_OUT);
    q1=(int*)Period;
    q2=(int*)Period1;
    z=0;
    for(unsigned int i=0;i<size/sizeof(float);i++)
        z+=q1[i]-q2[i];

    if(z!=0)
    {
        //MessageBox(NULL,"Ошибки при передаче периодов повторения","Message",MB_OK);
        BRD_reset(handle,0);
        BRD_reset(handle,1);
        err=BRD_cleanup();
        while(!BRD_errcmp(err,BRDerr_OK))
               err=BRD_cleanup();
        time(&tim);
        fprintf(flog,"\n%-60s%s","Ошибки при передаче периодов повторения ",DateTimeToStr(tim).c_str());
        fflush(flog);
        //FormData.flag_work_PK=0;   // !!! ОШИБКА  Form1->Shape1->Brush->Color=clRed;
        return 1; //goto nach_lo;
    }
    // Передаем число фильтров карты помех
    int nT1;
    size=sizeof(int);
    BRD_putMsg(handle,0,&nKK ,&size,TIME_OUT);
    BRD_getMsg(handle,0,&nT1 ,&size,TIME_OUT);
    if(nKK!=nT1)
    {
        time(&tim);
        fprintf(flog,"\n%-60s%s","Ошибки при передаче числа фильтров карты помех ",DateTimeToStr(tim).c_str());
        fflush(flog);
        return 1;
    }
    // Передаем в процессор коэффициенты передачи фильтров MTD
    //size=2*nK*LL*dsp_param.per_lhm;
    size=2*nKK*LL*dsp_param.per_lhm;
    //size>>=2;size++;size*=4;
    size*=sizeof(complex<float>);
    BRD_putMsg(handle,0,Filter_MTD ,&size,TIME_OUT);
    BRD_getMsg(handle,0,Filter_MTD1 ,&size,TIME_OUT);
    q1=(int*)Filter_MTD;
    q2=(int*)Filter_MTD1;
    z=0;
    for(unsigned int i=0;i<size/sizeof(float);i++)
        z+=q1[i]-q2[i];

    if(z!=0)
    {
        //MessageBox(NULL,"Ошибки при передаче коэффициентов передачи МТД","Message",MB_OK);
        BRD_reset(handle,0);
        BRD_reset(handle,1);
        err=BRD_cleanup();
        while(!BRD_errcmp(err,BRDerr_OK))
               err=BRD_cleanup();
        time(&tim);
        fprintf(flog,"\n%-60s%s","Ошибки при передаче коэффициентов передачи МТД ",DateTimeToStr(tim).c_str());
        fflush(flog);
        //FormData.flag_work_PK=0;   // !!! ОШИБКА  Form1->Shape1->Brush->Color=clRed;
        return 1; //goto nach_lo;
    }

// Передаем параметры окна обнаружения
    size=sizeof(okno);
    BRD_putMsg(handle,0,&okno ,&size,TIME_OUT);
    BRD_getMsg(handle,0,&okno1 ,&size,TIME_OUT);
    q1=(int*)&okno;
    q2=(int*)&okno1;
    z=0;
    for(unsigned int i=0;i<size/sizeof(float);i++)
        z+=q1[i]-q2[i];

    if(z!=0)
    {
        //MessageBox(NULL,"Ошибки при передаче параметров окна обнаружения","Message",MB_OK);
        BRD_reset(handle,0);
        BRD_reset(handle,1);
        err=BRD_cleanup();
        while(!BRD_errcmp(err,BRDerr_OK))
               err=BRD_cleanup();
        time(&tim);
        fprintf(flog,"\n%-60s%s","Ошибки при передаче параметров окна обнаружения ",DateTimeToStr(tim).c_str());
        fflush(flog);
        //FormData.flag_work_PK=0;   // !!! ОШИБКА  Form1->Shape1->Brush->Color=clRed;
        return 1; //goto nach_lo;
    }
// Передаем параметры окна обнаружения MONO
    size=sizeof(okno);
    BRD_putMsg(handle,0,&oknom ,&size,TIME_OUT);
    BRD_getMsg(handle,0,&okno1 ,&size,TIME_OUT);
    q1=(int*)&oknom;
    q2=(int*)&okno1;
    z=0;
    for(unsigned int i=0;i<size/sizeof(float);i++)
        z+=q1[i]-q2[i];

    if(z!=0)
    {
        //MessageBox(NULL,"Ошибки при передаче параметров окна обнаружения МОНО","Message",MB_OK);
        BRD_reset(handle,0);
        BRD_reset(handle,1);
        err=BRD_cleanup();
        while(!BRD_errcmp(err,BRDerr_OK))
               err=BRD_cleanup();
        time(&tim);
        fprintf(flog,"\n%-60s%s","Ошибки при передаче параметров окна обнаружения МОНО",DateTimeToStr(tim).c_str());
        fflush(flog);
        //FormData.flag_work_PK=0;   // !!! ОШИБКА  Form1->Shape1->Brush->Color=clRed;
        return 1; //goto nach_lo;
    }
    // Передаем тип обработки
    size=sizeof(tip_obr);
    BRD_putMsg(handle,0,&tip_obr ,&size,TIME_OUT);
    BRD_getMsg(handle,0,&nT1 ,&size,TIME_OUT);
    if(tip_obr!=nT1)
    {
        time(&tim);
        fprintf(flog,"\n%-60s%s","Ошибки при передаче типа обработки ",DateTimeToStr(tim).c_str());
        fflush(flog);
        return 1;
    }

/*
// Передаем число фильтров карты помех
    int nT1;
    size=sizeof(int);
    BRD_putMsg(handle,0,&nKK ,&size,TIME_OUT);
    BRD_getMsg(handle,0,&nT1 ,&size,TIME_OUT);
    if(nKK!=nT1)
    {
        time(&tim);
        fprintf(flog,"\n%-60s%s","Ошибки при передаче числа фильтров карты помех ",DateTimeToStr(tim).c_str());
        fflush(flog);
        return 1;
    }
*/
// Передаем параметры коэффициента децимации
    //int nT1;
    size=sizeof(int);
    BRD_putMsg(handle,0,&nT ,&size,TIME_OUT);
    BRD_getMsg(handle,0,&nT1 ,&size,TIME_OUT);
    if(nT!=nT1)
    {
        time(&tim);
        fprintf(flog,"\n%-60s%s","Ошибки при передаче коэффициента децимации ",DateTimeToStr(tim).c_str());
        fflush(flog);
        return 1;
    }
// Передаем параметры мощности сигнала
    float sig1;
    size=sizeof(float);
    BRD_putMsg(handle,0,&sig ,&size,TIME_OUT);
    BRD_getMsg(handle,0,&sig1 ,&size,TIME_OUT);
    if(sig!=sig1)
    {
        time(&tim);
        fprintf(flog,"\n%-60s%s","Ошибки при передаче мощности сигнала ",DateTimeToStr(tim).c_str());
        fflush(flog);
        return 1;
    }
    // Принимаем результат открытия ДДС
    size=sizeof(mes);
    BRD_getMsg(handle,0,mes ,&size,TIME_OUT);
    if(mes[0]<0)
    {
        //MessageBox(NULL,"Ошибки открытия ДДС","Message",MB_OK);
        BRD_reset(handle,0);
        BRD_reset(handle,1);
        err=BRD_cleanup();
        while(!BRD_errcmp(err,BRDerr_OK))
               err=BRD_cleanup();
        time(&tim);
        fprintf(flog,"\n%-60s%s","Ошибки открытия ДДС ",DateTimeToStr(tim).c_str());
        fflush(flog);
        //FormData.flag_work_PK=0;   // !!! ОШИБКА  Form1->Shape1->Brush->Color=clRed;
        return 1; //goto nach_lo;
    }
    // Принимаем результат открытия PIO
    size=sizeof(mes);
    BRD_getMsg(handle,0,mes ,&size,TIME_OUT);
    if(mes[0]<0)
    {
        //MessageBox(NULL,"Ошибки открытия PIO","Message",MB_OK);
        BRD_reset(handle,0);
        BRD_reset(handle,1);
        err=BRD_cleanup();
        while(!BRD_errcmp(err,BRDerr_OK))
               err=BRD_cleanup();
        time(&tim);
        fprintf(flog,"\n%-60s%s","Ошибки открытия PIO ",DateTimeToStr(tim).c_str());
        fflush(flog);
        //FormData.flag_work_PK=0;   // !!! ОШИБКА  Form1->Shape1->Brush->Color=clRed;
        return 1; // goto nach_lo;
    }

// 7
    k=0;

    //FormData.progress_bar_position=100;
    //FormData.progress_bar=0; // и невидима

    // включаем кнопки выбора комплекта
   // Form4->Button15->Enabled = true;
    time(&tim);
    fprintf(flog,"\n%-60s%s","Загрузка данных в процессоры закончена",DateTimeToStr(tim).c_str());
    fflush(flog);
    return 0;
}
