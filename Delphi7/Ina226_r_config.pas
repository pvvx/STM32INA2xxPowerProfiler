unit Ina226_r_config;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ExtCtrls, Spin;

const
  // Configuration Register (00h) (Read/Write)

  ConfigModeShl = 0;
  ConfigModeMsk = 7;
  ConfigModeCnt = 8;

  ShuntCTimeShl = 3;
  ShuntCTimeMsk = $0038;
  ShuntCTimeCnt = 8;

  BusCTimeShl = 6;
  BusCTimeMsk = $01c0;
  BusCTimeCnt = 8;

  AveragingModeShl = 9;
  AveragingModeMsk = $0e00;
  AveragingModeCnt = 8;

  ConfigResetMsk = $8000;

  // Mask/Enable Register (06h) (Read/Write)

  ShuntOverVoltageShl = 15;
  ShuntUnderVoltageShl = 14;
  BusOverVoltageShl = 13;
  BusUnderVoltageShl = 12;
  PowerOverLimitShl = 11;
  ConversionReadyShl = 10;
  AlertFunctionFlagShl = 4;
  ConversionReadyFlagShl = 3;
  MathOverflowFlagShl = 2;
  AlertPolarityShl = 1;
  AlertLatchEnableShl = 0;

  ShuntOverVoltageMsk = $8000;
  ShuntUnderVoltageMsk = $4000;
  BusOverVoltageMsk = $2000;
  BusUnderVoltageMsk = $1000;
  PowerOverLimitMsk = $0800;
  ConversionReadyMsk = $0400;
  AlertFunctionFlagMsk = $0010;
  ConversionReadyFlagMsk = $0008;
  MathOverflowFlagMsk = $0004;
  AlertPolarityMsk = $0002;
  AlertLatchEnableMsk = $0001;

  //
  SMBus_226_Speed_Max_kHz = 2500; // Chip Max 2940 kHz
  SMBus_226_Speed_kHz = 1000; // default
  SMBus_226_Speed_Min_kHz = 100;

type
  TForm226Config = class(TForm)
    GroupBox1: TGroupBox;
    CheckBoxReset: TCheckBox;
    RadioGroupMode: TRadioGroup;
    ButtonOk: TButton;
    ButtonCancel: TButton;
    RadioGroupBusCTime: TRadioGroup;
    RadioGroupShuntCTime: TRadioGroup;
    RadioGroupAverageMode: TRadioGroup;
    SpinEditCLkKHz: TSpinEdit;
    Label2: TLabel;
    Label1: TLabel;
    EditUz: TEdit;
    EditIz: TEdit;
    LabelUz: TLabel;
    LabelIz: TLabel;
    EditRegConfig: TEdit;
    EditRegCalibration: TEdit;
    EditRegMaskEnable: TEdit;
    EditRegAlertData: TEdit;
    Label3: TLabel;
    Label4: TLabel;
    Label5: TLabel;
    Label6: TLabel;
    ButtonCopyUz: TButton;
    ButtonCopyIz: TButton;
    procedure ChargeReg(Sender: TObject);
    procedure ButtonOkClick(Sender: TObject);
    procedure GetParamIz;
    procedure GetParams;
    procedure GetScrParms;
    procedure ShowScrParms;
    procedure SetParamIU(i : double ; u : double);
    function DevIniCfg(mode : integer) : byte;
    procedure FormActivate(Sender: TObject);
    procedure ButtonCopyUzClick(Sender: TObject);
    procedure ButtonCopyIzClick(Sender: TObject);
  private
    { Private declarations }
    ChgEna : boolean;
    procedure CheckRegValue;
  public
    { Public declarations }
    reg_config : word;
    reg_calibration : word;
    reg_mask_enable : word;
    reg_alert_limit : word;
  end;

var
  Form226Config: TForm226Config;

  TabBusClkTiming : array [0..ShuntCTimeCnt-1] of word =
   (1000, 800, 800, 800,
    800, 800, 800, 800);

  TabTimerCTime : array [0..BusCTimeCnt-1] of word =
   (140, 204, 332, 588,
    1100, 2116, 4156, 8244);

  Ik_226 : double = 0.02314; // 2.5 uV/bit
  Uk_226 : double = 0.00125000; // 1.25 mV/bit
  I_226_zero : double = 0.0;
  U_226_zero : double = 0.0;
  Clk_226 : integer = SMBus_226_Speed_kHz;


implementation

{$R *.dfm}
Uses MainFrm;

function Str2dword(const s: string): Dword;
var
 i: integer;
 o : string;
 hex : boolean;
begin
    i := 1;
    hex := False;
//  result := 0;
    while (i <= Length(s)) do begin
      if ((s[i] = '0') and (s[i+1] = 'x')) then begin
       hex := True;
       inc(i);
      end
      else if (s[i] = '$') then  hex := True
      else if (((s[i] >= 'a') and (s[i] <= 'f'))
      or ((s[i] >= 'A') and (s[i] <= 'F'))) then begin
       hex := True;
       o := o + s[i];
      end
      else if ((s[i] >= '0') and (s[i] <= '9')) then o := o + s[i];
      inc(i);
    end;
    if hex then result := StrToIntDef('$'+o,0)
    else result := StrToIntDef(o,0)
end;

procedure TForm226Config.GetScrParms;
begin
    DecimalSeparator := '.';
    U_226_zero := StrToFloat(EditUz.Text);
    I_226_zero := StrToFloat(EditIz.Text);
end;

procedure TForm226Config.ShowScrParms;
begin
    DecimalSeparator := '.';
    EditUz.Text:=FormatFloat('0.00000000', U_226_zero);
    EditIz.Text:=FormatFloat('0.00000000', I_226_zero);
end;

procedure TForm226Config.FormActivate(Sender: TObject);
begin
     ChgEna := False;
     reg_config := ina2xx_reg.config;
//     ScrollBarCLK.Max := SMBus_219_Speed_Max_kHz;
//     ScrollBarCLK.Min := SMBus_219_Speed_Min_kHz;
//     if (blk_cfg.clk_khz < SMBus_219_Speed_Min_kHz) or (blk_cfg.clk_khz > SMBus_219_Speed_Max_kHz) then blk_cfg.clk_khz := SMBus_219_Speed_kHz;
//     ScrollBarCLK.Position := blk_cfg.clk_khz;
//     LabelClk.Caption := 'Bus CLK = ' + IntToStr(ScrollBarCLK.Position) + ' kHz';
     if (reg_config and ConfigResetMsk) <> 0 then
        CheckBoxReset.Checked := True
     else
       CheckBoxReset.Checked := False;

     RadioGroupBusCTime.ItemIndex := (reg_config and BusCTimeMsk) shr BusCTimeShl;
     RadioGroupShuntCTime.ItemIndex := (reg_config and ShuntCTimeMsk) shr ShuntCTimeShl;
     RadioGroupAverageMode.ItemIndex := (reg_config and AveragingModeMsk) shr AveragingModeShl;

     RadioGroupMode.ItemIndex := reg_config and ConfigModeMsk;
     ShowScrParms;

     EditRegConfig.Text := '0x' + IntToHex(reg_config, 4);
     EditRegCalibration.Text := '0x' + IntToHex(ina2xx_reg.calibration, 4);
     EditRegMaskEnable.Text := '0x' + IntToHex(ina2xx_reg.mask_enable, 4);
     EditRegAlertData.Text := '0x' + IntToHex(ina2xx_reg.alert_data, 4);

     ChgEna := True;
end;

procedure TForm226Config.CheckRegValue;
begin
     reg_config := (RadioGroupMode.ItemIndex shl ConfigModeShl) and ConfigModeMsk;
     reg_config := reg_config or ((RadioGroupBusCTime.ItemIndex shl BusCTimeShl) and BusCTimeMsk);
     reg_config := reg_config or ((RadioGroupShuntCTime.ItemIndex shl ShuntCTimeShl) and ShuntCTimeMsk);
     reg_config := reg_config or ((RadioGroupAverageMode.ItemIndex shl AveragingModeShl) and AveragingModeMsk);
     if CheckBoxReset.Checked then reg_config := reg_config or ConfigResetMsk;
     EditRegConfig.Text := '0x' + IntToHex(reg_config, 4);
     reg_calibration := Str2dword(EditRegCalibration.Text);
     reg_mask_enable := Str2dword(EditRegMaskEnable.Text);
     reg_alert_limit := Str2dword(EditRegAlertData.Text);
end;

procedure TForm226Config.ChargeReg(Sender: TObject);
begin
    if ChgEna then CheckRegValue;
end;

procedure TForm226Config.ButtonOkClick(Sender: TObject);
var
 clk_khz : integer;
begin
    CheckRegValue;

    if RadioGroupShuntCTime.ItemIndex < RadioGroupBusCTime.ItemIndex then
      clk_khz := TabBusClkTiming[RadioGroupShuntCTime.ItemIndex]
    else
      clk_khz := TabBusClkTiming[RadioGroupBusCTime.ItemIndex];

    if SpinEditCLkKHz.Value < clk_khz then begin
        ShowMessage('BUS CLK Low!' + #13#10 +'Min CLK ' + IntToStr(clk_khz) + ' kHz!');
        SpinEditCLkKHz.Value := clk_khz;
        ModalResult := mrNone;
        Exit;
    end;

//    I_zero := I_zero_tab[RadioGroupGain.ItemIndex];

//    blk_cfg.clk_khz := 1000;

    ina2xx_reg.config := reg_config;
    ina2xx_reg.calibration := reg_calibration;
    ina2xx_reg.mask_enable := reg_mask_enable;
    ina2xx_reg.alert_data := reg_alert_limit;

    GetScrParms;
    ShowScrParms;

    ModalResult := mrOk;
    Exit;
end;

procedure TForm226Config.GetParamIz;
begin
  I_zero := I_226_zero;
  U_zero := U_226_zero;
//  Uk := Uk_226;
//  Ik := Ik_226;
end;

procedure TForm226Config.GetParams;
begin
  I_zero := I_226_zero;
  U_zero := U_226_zero;
  Uk := Uk_226;
  Ik := Ik_226;
end;

procedure TForm226Config.SetParamIU(i : double ; u : double);
begin
  Ik_226 := i;
  Uk_226 := u;
end;

function TForm226Config.DevIniCfg(mode : integer) : byte;
var
  mask : word;
  u, i, x : dword;
begin
     // инициализировать в INA219 регистры:
     // записать config регистр:
     blk_cfg.init[0].dev_addr := INA2XX_I2C_ADDR;
     blk_cfg.init[0].reg_addr := 0;
     blk_cfg.init[0].data := ina2xx_reg.config;
//   записать другие регистры ?:
   blk_cfg.init[1].dev_addr := INA2XX_I2C_ADDR;
   blk_cfg.init[1].reg_addr := 5;
   blk_cfg.init[1].data := ina2xx_reg.calibration;

   blk_cfg.init[2].dev_addr := INA2XX_I2C_ADDR;
   blk_cfg.init[2].reg_addr := 6;
   blk_cfg.init[2].data := ina2xx_reg.mask_enable;

   blk_cfg.init[3].dev_addr := INA2XX_I2C_ADDR;
   blk_cfg.init[3].reg_addr := 7;
   blk_cfg.init[3].data := ina2xx_reg.alert_data;
     // инициализировать всего 1 регистр:
     blk_cfg.wr_count := 4; // кол-ву regs init

     // задать адреса устройства чтения
     blk_cfg.data[0].dev_addr := INA2XX_I2C_ADDR;
     blk_cfg.data[1].dev_addr := INA2XX_I2C_ADDR;
     // min 140, max 8244*1024 = 8441856 us
     u := (ina2xx_reg.config and BusCTimeMsk) shr BusCTimeShl;
     u := TabTimerCTime[u];
     i := (ina2xx_reg.config and ShuntCTimeMsk) shr ShuntCTimeShl;
     i := TabTimerCTime[i];
     x := (ina2xx_reg.config and AveragingModeMsk) shr AveragingModeShl;
//     u := u shl (x * 2);
//     i := i shl (x * 2);
//     while (u > $ffff) do u := u shr 1;
//     while (i > $ffff) do i := i shr 1;
     mask := ina2xx_reg.config and ConfigModeMsk;
     // задать адреса регистров чтения
     case mask of
         5 : begin // Shunt
             result := CHART_I_MASK;
             if(mode <> 0) then blk_cfg.rd_count := 1;
             blk_cfg.data[0].reg_addr := 1;
             blk_cfg.data[1].reg_addr := 1;
             blk_cfg.time_us := i;
             blk_cfg.multiplier := x;
            end;
         6 : begin // Bus
             result := CHART_U_MASK;
             //i := u;
             if(mode <> 0) then blk_cfg.rd_count := 1;
             blk_cfg.data[0].reg_addr := 2;
             blk_cfg.data[1].reg_addr := 2;
             blk_cfg.time_us := u;
             blk_cfg.multiplier := x;
            end;
         7 : begin // Shunt + Bus
             result := CHART_UI_MASK;
             if(mode <> 0) then blk_cfg.rd_count := 2;
             blk_cfg.data[0].reg_addr := 1;
             blk_cfg.data[1].reg_addr := 2;
             blk_cfg.time_us := (i + u) shr 1;
             blk_cfg.multiplier := x;
             //if i < u then i := u;
            end;
         else begin
            result := 0;
            blk_cfg.rd_count := 0;
            blk_cfg.time_us := 10000;
            blk_cfg.multiplier := 0;
            end;
     end;
     // задать максимум регистров в пакете
     x := blk_cfg.time_us shl blk_cfg.multiplier;
     x := x div (30000 div 30);
     if(x > 29) then x := 29;
     if mask = 7 then x := x and $fffe;
     blk_cfg.pktcnt := 30 - x;
     // задать частоту шины i2c в кГц
     blk_cfg.clk_khz := SpinEditCLkKHz.Value;
     // TabBusClkTiming[i];

end;

procedure TForm226Config.ButtonCopyUzClick(Sender: TObject);
begin
    EditUz.Text:=FormatFloat('0.00000000', OldsU);
end;

procedure TForm226Config.ButtonCopyIzClick(Sender: TObject);
begin
    EditIz.Text:=FormatFloat('0.00000000', OldsI);
end;

end.
