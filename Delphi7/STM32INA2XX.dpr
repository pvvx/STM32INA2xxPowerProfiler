program STM32INA2XX;

uses
  Forms,
  MainFrm in 'MainFrm.pas' {frmMain},
  ComPort in 'ComPort.pas',
  HexUtils in 'HexUtils.pas',
  Ina219_r_config in 'Ina219_r_config.pas' {Form219Config},
  Ina226_r_config in 'Ina226_r_config.pas' {Form226Config};

{$E exe}

{$R *.res}

begin
  Application.Initialize;
  Application.Title := 'STM32-INA2XX';
  Application.CreateForm(TfrmMain, frmMain);
  Application.CreateForm(TForm219Config, Form219Config);
  Application.CreateForm(TForm226Config, Form226Config);
  Application.Run;
end.
