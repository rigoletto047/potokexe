// объявление структуры для отображения данных на форме

struct Form_data
{
// флаги работы первичного и вторичного каналов, если =0 (авария=красный), 1 (резерв=желтый), 2(норма=зеленый)
int flag_work_PK;   // соотв.   Form1->Shape4->Brush->Color=clYellow; clLime, clRed  + Form4
int flag_work_VK;   // соотв.   Form1->Shape5->Brush->Color=clYellow + Form4

bool progress_bar;  // 0 - не показывать, 1 - показывать процесс инициализации
int  progress_bar_position;
/*  для прогрессбара был код ниже
  Form1->Panel2->Top=2;
  Form1->ProgressBar1->Position=1;
  Form1->Panel2->Visible = true;
  Form1->AutoSize=true;
*/




};

