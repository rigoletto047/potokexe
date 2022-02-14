//---------------------------------------------------------------------------
#ifndef UTestH
#define UTestH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TTest : public TForm
{
__published:	// IDE-managed Components
	TButton *Button2;
	TTimer *Timer1;
  TButton *Button3;
        TGroupBox *GroupBox1;
        TGroupBox *GroupBox2;
        TEdit *Edit3;
        TEdit *Edit4;
        TLabel *Label4;
        TEdit *Edit7;
        TLabel *Label6;
        TLabel *Label7;
        TLabel *Label9;
        TEdit *Edit8;
        TGroupBox *GroupBox3;
        TEdit *Edit9;
        TEdit *Edit11;
        TLabel *Label10;
        TEdit *Edit12;
        TLabel *Label11;
        TLabel *Label12;
        TLabel *Label13;
        TEdit *Edit13;
        TGroupBox *GroupBox4;
        TGroupBox *GroupBox5;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label5;
        TEdit *Edit1;
        TEdit *Edit2;
        TEdit *Edit5;
        TEdit *Edit6;
        TGroupBox *GroupBox6;
        TLabel *Label8;
        TLabel *Label14;
        TLabel *Label15;
        TLabel *Label18;
        TEdit *Edit10;
        TEdit *Edit14;
        TEdit *Edit15;
        TEdit *Edit16;
        TLabel *Label16;
        TLabel *Label17;
        TEdit *Edit17;
        TEdit *Edit18;
        TLabel *Label19;
        TEdit *Edit19;
        TEdit *Edit20;
        TLabel *Label20;
        TCheckBox *CheckBox1;
        TButton *Button4;
        TLabel *Label21;
        TEdit *Edit21;
        TLabel *Label22;
        TEdit *Edit22;
        TLabel *Label23;
        TEdit *Edit23;
        TLabel *Label24;
        TEdit *Edit24;
        TLabel *Label25;
        TLabel *Label26;
        TEdit *Edit25;
        TEdit *Edit26;
        TEdit *Edit27;
        TLabel *Label27;
        TLabel *Label28;
        TLabel *Label29;
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall CheckBox1Click(TObject *Sender);
        //void __fastcall Edit5Exit(TObject *Sender);
        void __fastcall Edit5KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall Edit6KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall Edit2KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall Edit1KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall Edit15KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall Edit16KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall Edit14KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall Edit10KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall Edit17KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall Edit18KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall Edit19KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall Edit20KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall Button4Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TTest(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TTest *Test;
//---------------------------------------------------------------------------
#endif
