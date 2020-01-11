object Form226Config: TForm226Config
  Left = 1030
  Top = 211
  BorderStyle = bsDialog
  Caption = 'INA226: Registor Config'
  ClientHeight = 348
  ClientWidth = 481
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnActivate = FormActivate
  PixelsPerInch = 96
  TextHeight = 13
  object Label2: TLabel
    Left = 144
    Top = 220
    Width = 20
    Height = 13
    Caption = 'KHz'
  end
  object Label1: TLabel
    Left = 8
    Top = 220
    Width = 61
    Height = 13
    Caption = 'SMBUS CLK'
  end
  object LabelUz: TLabel
    Left = 224
    Top = 220
    Width = 37
    Height = 13
    Caption = 'U offset'
  end
  object LabelIz: TLabel
    Left = 224
    Top = 260
    Width = 32
    Height = 13
    Caption = 'I offset'
  end
  object Label3: TLabel
    Left = 8
    Top = 252
    Width = 30
    Height = 13
    Caption = 'Config'
  end
  object Label4: TLabel
    Left = 8
    Top = 276
    Width = 49
    Height = 13
    Caption = 'Calibration'
  end
  object Label5: TLabel
    Left = 8
    Top = 300
    Width = 59
    Height = 13
    Caption = 'MaskEnable'
  end
  object Label6: TLabel
    Left = 8
    Top = 324
    Width = 44
    Height = 13
    Caption = 'AlertData'
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 8
    Width = 185
    Height = 33
    Caption = 'Reset'
    TabOrder = 0
    object CheckBoxReset: TCheckBox
      Left = 8
      Top = 12
      Width = 65
      Height = 17
      Caption = 'Reset'
      TabOrder = 0
      OnClick = ChargeReg
      OnEnter = ChargeReg
    end
  end
  object RadioGroupMode: TRadioGroup
    Left = 200
    Top = 8
    Width = 169
    Height = 201
    Caption = 'Mode '
    ItemIndex = 0
    Items.Strings = (
      'Power-down'
      'Shunt voltage, triggered'
      'Bus voltage, triggered'
      'Shunt and bus, triggered'
      'Power-Down (or Shutdown)'#13
      'Shunt voltage, continuous'
      'Bus voltage, continuous'
      'Shunt and bus, continuous')
    TabOrder = 1
    OnClick = ChargeReg
    OnEnter = ChargeReg
  end
  object ButtonOk: TButton
    Left = 240
    Top = 312
    Width = 75
    Height = 25
    Caption = 'Ok'
    TabOrder = 2
    OnClick = ButtonOkClick
  end
  object ButtonCancel: TButton
    Left = 344
    Top = 312
    Width = 75
    Height = 25
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 3
  end
  object RadioGroupBusCTime: TRadioGroup
    Left = 8
    Top = 48
    Width = 89
    Height = 161
    Caption = 'Bus CTime'
    ItemIndex = 3
    Items.Strings = (
      '140 us'
      '204 us'
      '332 us'
      '588 us'
      '1.1 ms'
      '2.116 ms'
      '4.156 ms'
      '8.244 ms')
    TabOrder = 4
    OnClick = ChargeReg
    OnEnter = ChargeReg
  end
  object RadioGroupShuntCTime: TRadioGroup
    Left = 104
    Top = 48
    Width = 89
    Height = 161
    Caption = 'Shunt CTime'
    ItemIndex = 3
    Items.Strings = (
      '140 us'
      '204 us'
      '332 us'
      '588 us'
      '1.1 ms'
      '2.116 ms'
      '4.156 ms'
      '8.244 ms')
    TabOrder = 5
    OnClick = ChargeReg
    OnEnter = ChargeReg
  end
  object RadioGroupAverageMode: TRadioGroup
    Left = 376
    Top = 8
    Width = 97
    Height = 201
    Caption = 'Averaging Mode'
    ItemIndex = 0
    Items.Strings = (
      '1'
      '4'
      '16'
      '64'
      '128'
      '256'
      '512'
      '1024')
    TabOrder = 6
    OnClick = ChargeReg
    OnEnter = ChargeReg
  end
  object SpinEditCLkKHz: TSpinEdit
    Left = 72
    Top = 216
    Width = 65
    Height = 22
    Increment = 50
    MaxValue = 1100
    MinValue = 400
    TabOrder = 7
    Value = 1000
  end
  object EditUz: TEdit
    Left = 272
    Top = 218
    Width = 89
    Height = 21
    Hint = #1057#1084#1077#1097#1077#1085#1080#1077' '#1076#1083#1103' U'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 8
    Text = '?.?'
  end
  object EditIz: TEdit
    Left = 272
    Top = 258
    Width = 89
    Height = 21
    Hint = #1057#1084#1077#1097#1077#1085#1080#1077' '#1076#1083#1103' U'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 9
    Text = '?.?'
  end
  object EditRegConfig: TEdit
    Left = 72
    Top = 248
    Width = 65
    Height = 21
    Hint = 'Registor Config'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 10
    Text = '?.?'
  end
  object EditRegCalibration: TEdit
    Left = 72
    Top = 272
    Width = 65
    Height = 21
    Hint = 'Registor Calibration'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 11
    Text = '?.?'
  end
  object EditRegMaskEnable: TEdit
    Left = 72
    Top = 296
    Width = 65
    Height = 21
    Hint = 'Registor MaskEnable'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 12
    Text = '?.?'
  end
  object EditRegAlertData: TEdit
    Left = 72
    Top = 320
    Width = 65
    Height = 21
    Hint = #1057#1084#1077#1097#1077#1085#1080#1077' '#1076#1083#1103' U'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 13
    Text = '?.?'
  end
  object ButtonCopyUz: TButton
    Left = 368
    Top = 216
    Width = 89
    Height = 25
    Caption = 'Copy Us to Uz'
    TabOrder = 14
    OnClick = ButtonCopyUzClick
  end
  object ButtonCopyIz: TButton
    Left = 368
    Top = 256
    Width = 89
    Height = 25
    Caption = 'Copy Is to Iz'
    TabOrder = 15
    OnClick = ButtonCopyIzClick
  end
end
