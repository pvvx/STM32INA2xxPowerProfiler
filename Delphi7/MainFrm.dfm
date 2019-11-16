object frmMain: TfrmMain
  Left = 1386
  Top = 567
  Width = 1139
  Height = 648
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object StatusBar: TStatusBar
    Left = 0
    Top = 590
    Width = 1123
    Height = 19
    Panels = <
      item
        Text = 'COM?'
        Width = 50
      end
      item
        Text = '? sps'
        Width = 80
      end
      item
        Text = 'Open COM port!'
        Width = 50
      end>
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 1123
    Height = 38
    Align = alTop
    TabOrder = 1
    object LabelMXI: TLabel
      Left = 424
      Top = 12
      Width = 13
      Height = 13
      Hint = #1057#1088#1077#1076#1085#1080#1081' '#1058#1086#1082
      Caption = 'I:?'
      ParentShowHint = False
      ShowHint = True
    end
    object LabelMXU: TLabel
      Left = 496
      Top = 12
      Width = 16
      Height = 13
      Hint = #1057#1088#1077#1076#1085#1077#1077' '#1053#1072#1087#1088#1103#1078#1077#1085#1080#1077
      Caption = 'U:?'
      ParentShowHint = False
      ShowHint = True
    end
    object btnQueryDosDevice: TButton
      Left = 8
      Top = 8
      Width = 49
      Height = 21
      Hint = #1055#1077#1088#1077#1089#1082#1072#1085#1080#1088#1086#1074#1072#1090#1100' COM '#1087#1086#1088#1090#1099
      Caption = 'ReScan'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 0
      OnClick = btnReScanComDevices
    end
    object ComboBox: TComboBox
      Left = 64
      Top = 8
      Width = 65
      Height = 21
      Style = csOwnerDrawFixed
      ItemHeight = 15
      ItemIndex = 1
      Sorted = True
      TabOrder = 1
      Text = 'COM3'
      OnChange = ChComName
      Items.Strings = (
        'COM1'
        'COM3'
        'COM4')
    end
    object ButtonOpen: TButton
      Left = 136
      Top = 8
      Width = 49
      Height = 21
      Hint = #1054#1090#1082#1088#1099#1090#1100'/'#1079#1072#1082#1088#1099#1090#1100' '#1074#1099#1073#1088#1072#1085#1085#1099#1081' COM '#1087#1086#1088#1090
      Caption = 'Open'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 2
      OnClick = ButtonOpenClick
    end
    object ButtonConfigReg: TButton
      Left = 196
      Top = 8
      Width = 65
      Height = 21
      Hint = 'Configuration Register'
      Caption = 'Config Reg'
      Enabled = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 3
      OnClick = ButtonConfigRegClick
    end
    object ButtonStart: TButton
      Left = 272
      Top = 8
      Width = 65
      Height = 21
      Hint = 'Start CMMode. Continuous Measurment Mode.'
      Caption = 'Start CMM'
      Enabled = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 4
      OnClick = ButtonStartClick
    end
    object ButtonStore: TButton
      Left = 696
      Top = 8
      Width = 65
      Height = 21
      Hint = #1047#1072#1087#1080#1089#1072#1090#1100' '#1090#1077#1082#1091#1097#1091#1102' '#1082#1086#1085#1092#1080#1075#1091#1088#1072#1094#1080#1102' '#1074#1086' Flash '#1091#1089#1090#1088#1086#1081#1089#1090#1074#1072'.'
      Caption = 'Store CFG'
      Enabled = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 5
      OnClick = ButtonStoreClick
    end
    object ButtonRdAllRegs: TButton
      Left = 768
      Top = 8
      Width = 65
      Height = 21
      Hint = #1057#1095#1080#1090#1072#1090#1100' '#1074#1089#1077' 8-'#1084#1100' '#1088#1077#1075#1080#1089#1090#1088#1086#1074' INA2xx'
      Caption = 'Read Regs'
      Enabled = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 6
      OnClick = ButtonRdAllRegsClick
    end
    object ButtonStop: TButton
      Left = 344
      Top = 8
      Width = 73
      Height = 21
      Hint = #1054#1089#1090#1072#1085#1086#1074#1080#1090#1100' '#1085#1077#1087#1088#1077#1088#1099#1074#1085#1086#1077' '#1095#1090#1077#1085#1080#1077'.'
      Caption = 'Stop'
      Enabled = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 7
      OnClick = ButtonStopClick
    end
    object EditRegs: TEdit
      Left = 844
      Top = 9
      Width = 269
      Height = 21
      TabOrder = 8
      Text = '?'
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 38
    Width = 1123
    Height = 35
    Align = alTop
    TabOrder = 2
    object SetColorI: TLabel
      Left = 6
      Top = 8
      Width = 21
      Height = 19
      Hint = #1053#1072#1089#1090#1088#1086#1081#1082#1072' '#1094#1074#1077#1090#1072'  '#1080'   '#1074#1082#1083'./'#1074#1099#1082#1083'. ('#1082#1085#1086#1087#1082#1080' '#1084#1099#1096#1080')'
      Caption = ' A '
      Color = clRed
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      OnMouseDown = SetColorIMouseDown
    end
    object SetColorU: TLabel
      Left = 38
      Top = 8
      Width = 21
      Height = 19
      Hint = #1053#1072#1089#1090#1088#1086#1081#1082#1072' '#1094#1074#1077#1090#1072'  '#1080'   '#1074#1082#1083'./'#1074#1099#1082#1083'. ('#1082#1085#1086#1087#1082#1080' '#1084#1099#1096#1080')'
      Caption = ' U '
      Color = clBlue
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      OnMouseDown = SetColorUMouseDown
    end
    object ButtonClrGrf: TButton
      Left = 288
      Top = 8
      Width = 73
      Height = 21
      Hint = #1054#1095#1080#1089#1090#1080#1090#1100' '#1075#1088#1072#1092#1080#1082
      Caption = 'ClrGrf'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 0
      OnClick = ButtonClrGrfClick
    end
    object ButtonSaveGRF: TButton
      Left = 752
      Top = 8
      Width = 65
      Height = 21
      Hint = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1076#1072#1085#1085#1099#1077' '#1074' csv '#1092#1072#1081#1083
      Caption = 'Save Data'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 1
      OnClick = ButtonSaveGRFClick
    end
    object ButtonPrtGrf: TButton
      Left = 824
      Top = 8
      Width = 65
      Height = 21
      Hint = #1055#1077#1095#1072#1090#1100' '#1075#1088#1072#1092#1080#1082#1072
      Caption = 'PrintGrf'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 2
      OnClick = ButtonPrtGrfClick
    end
    object ButtonAsize: TButton
      Left = 176
      Top = 8
      Width = 65
      Height = 21
      Hint = #1052#1072#1089#1096#1090#1072#1073#1080#1088#1086#1074#1072#1085#1080#1077
      Caption = 'Manual'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 3
      OnClick = ButtonAsizeClick
    end
    object ButtonScalP: TButton
      Left = 248
      Top = 8
      Width = 25
      Height = 21
      Hint = #1052#1072#1089#1096#1090#1072#1073#1080#1088#1086#1074#1072#1085#1080#1077
      Caption = '+'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 4
      Visible = False
      OnClick = ButtonScalPClick
    end
    object ButtonScalM: TButton
      Left = 144
      Top = 8
      Width = 25
      Height = 21
      Hint = #1052#1072#1089#1096#1090#1072#1073#1080#1088#1086#1074#1072#1085#1080#1077
      Caption = '-'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 5
      Visible = False
      OnClick = ButtonScalMClick
    end
    object CheckBoxOpenGL: TCheckBox
      Left = 72
      Top = 8
      Width = 65
      Height = 17
      Caption = 'OpenGL'
      Checked = True
      State = cbChecked
      TabOrder = 6
      OnClick = CheckBoxOpenGLClick
    end
    object EditSizeGrf: TEdit
      Left = 1040
      Top = 8
      Width = 73
      Height = 21
      Hint = #1052#1072#1082#1089#1080#1084#1072#1083#1100#1085#1086#1077' '#1082#1086#1083'-'#1074#1086' '#1090#1086#1095#1077#1082' '#1085#1072' '#1075#1088#1072#1092#1080#1082#1077' '#1076#1086' '#1086#1089#1090#1072#1085#1086#1074#1072
      ParentShowHint = False
      ShowHint = True
      TabOrder = 7
      Text = '600000'
    end
    object EditUk: TEdit
      Left = 896
      Top = 8
      Width = 65
      Height = 21
      Hint = #1050#1086#1101#1092'. '#1076#1083#1103' U'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 8
      Text = '?.?'
    end
    object EditIk: TEdit
      Left = 968
      Top = 8
      Width = 65
      Height = 21
      Hint = #1050#1086#1101#1092'. '#1076#1083#1103' I'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 9
      Text = '?.?'
    end
    object EditTriggerI: TEdit
      Left = 448
      Top = 6
      Width = 57
      Height = 21
      Hint = #1058#1080#1088#1075#1077#1088': '#1058#1086#1082' (mA)'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 10
      Text = '1.0'
      OnExit = CheckBoxTrigerClick
    end
    object CheckBoxTrigerRiseI: TCheckBox
      Left = 376
      Top = 8
      Width = 73
      Height = 17
      Hint = #1058#1088#1080#1075#1075#1077#1088' '#1089#1090#1072#1088#1090#1072' '#1087#1086' '#1087#1077#1088#1077#1087#1072#1076#1091' '#1090#1086#1082#1072' '#1074#1074#1077#1088#1093
      Caption = #1058#1088#1080#1075#1077#1088' I^'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 11
      OnClick = CheckBoxTrigerClick
    end
    object CheckBoxTrigerRiseU: TCheckBox
      Left = 512
      Top = 8
      Width = 81
      Height = 17
      Hint = #1058#1088#1080#1075#1075#1077#1088' '#1089#1090#1072#1088#1090#1072' '#1087#1086' '#1087#1077#1088#1077#1087#1072#1076#1091' '#1085#1072#1087#1088#1103#1078#1077#1085#1080#1103' '#1074#1074#1077#1088#1093
      Caption = #1058#1088#1080#1075#1077#1088' U^'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 12
      OnClick = CheckBoxTrigerClick
    end
    object EditTriggerU: TEdit
      Left = 592
      Top = 6
      Width = 57
      Height = 21
      Hint = #1058#1080#1088#1075#1077#1088': '#1053#1072#1087#1088#1103#1078#1077#1085#1080#1077' (V)'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 13
      Text = '1.0'
      OnExit = CheckBoxTrigerClick
    end
  end
  object Chart: TChart
    Left = 31
    Top = 73
    Width = 1092
    Height = 517
    BackWall.Brush.Color = clWhite
    BackWall.Brush.Style = bsClear
    Legend.Visible = False
    MarginBottom = 2
    MarginLeft = 2
    MarginRight = 2
    MarginTop = 2
    PrintProportional = False
    Title.Text.Strings = (
      'TChart')
    Title.Visible = False
    Chart3DPercent = 50
    View3D = False
    View3DOptions.Elevation = 357
    View3DOptions.Zoom = 50
    Align = alClient
    TabOrder = 3
    object Series1: TFastLineSeries
      Marks.Arrow.Visible = True
      Marks.Callout.Brush.Color = clBlack
      Marks.Callout.Arrow.Visible = True
      Marks.Shadow.Color = 8487297
      Marks.Visible = False
      SeriesColor = clBlue
      VertAxis = aRightAxis
      LinePen.Color = clBlue
      LinePen.Width = 2
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
    object Series2: TFastLineSeries
      Marks.Arrow.Visible = True
      Marks.Callout.Brush.Color = clBlack
      Marks.Callout.Arrow.Visible = True
      Marks.Shadow.Color = 8487297
      Marks.Visible = False
      LinePen.Color = clRed
      LinePen.Width = 2
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
    object ChartTool1: TSeriesStatsTool
    end
  end
  object TeeCommander: TTeeCommander
    Left = 0
    Top = 73
    Width = 31
    Height = 517
    Panel = Chart
    Vertical = True
    Align = alLeft
    ParentShowHint = False
    TabOrder = 4
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 84
    OnTimer = TimerTimer
    Left = 632
    Top = 78
  end
  object PrintDialog: TPrintDialog
    Left = 600
    Top = 78
  end
  object SaveDialog: TSaveDialog
    Left = 504
    Top = 78
  end
  object PrinterSetupDialog: TPrinterSetupDialog
    Left = 536
    Top = 78
  end
  object ColorDialog: TColorDialog
    Left = 568
    Top = 78
  end
  object TeeOpenGL1: TTeeOpenGL
    Active = True
    AmbientLight = 0
    DrawStyle = tcsWire
    Light.Color = clGray
    Light.Visible = False
    Light.Direction.Z = -1.000000000000000000
    Light.Position.X = -100.000000000000000000
    Light.Position.Y = -100.000000000000000000
    Light.Position.Z = -100.000000000000000000
    Light.SpotDegrees = 180.000000000000000000
    Light1.Color = clGray
    Light1.Visible = True
    Light1.Direction.Z = -1.000000000000000000
    Light1.Position.X = -100.000000000000000000
    Light1.Position.Y = -100.000000000000000000
    Light1.Position.Z = -100.000000000000000000
    Light1.SpotDegrees = 180.000000000000000000
    Light2.Color = clGray
    Light2.Visible = True
    Light2.Direction.Z = -1.000000000000000000
    Light2.Position.X = -100.000000000000000000
    Light2.Position.Y = -100.000000000000000000
    Light2.Position.Z = -100.000000000000000000
    Light2.SpotDegrees = 180.000000000000000000
    Light2.UseDirection = True
    ShadeQuality = False
    Shininess = 0.100000000000000000
    TeePanel = Chart
    Left = 696
    Top = 81
  end
  object ChartPreviewer: TChartPreviewer
    Chart = Chart
    Left = 664
    Top = 80
  end
end
