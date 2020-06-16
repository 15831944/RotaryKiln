; �ű��� Inno Setup �ű��� ���ɣ�
; �йش��� Inno Setup �ű��ļ�����ϸ��������İ����ĵ���

#define MyAppName "RotaryKiln"
#define MyAppVersion "1.0"
#define MyAppPublisher "�Ͼ�ʱ�����ܿƼ����޹�˾"
#define MyAppURL "http://www.sosurer.com/"
#define MyAppExeName "RotaryKiln.exe"

[Setup]
; ע: AppId��ֵΪ������ʶ��Ӧ�ó���
; ��ҪΪ������װ����ʹ����ͬ��AppIdֵ��
; (�����µ�GUID����� ����|��IDE������GUID��)
AppId={{A475A011-D0DB-4CD2-9836-21927BF3D2A5}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DisableProgramGroupPage=yes
OutputBaseFilename=RotaryKilnSetup
SetupIconFile=E:\Visual Studio Application\��תҤ\Package\RotaryKiln\Setup.ico
Compression=lzma
SolidCompression=yes
PrivilegesRequired=admin

[Languages]
Name: "chinesesimp"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Files]
Source: "E:\Visual Studio Application\��תҤ\Package\RotaryKiln\RotaryKiln.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Visual Studio Application\��תҤ\Package\RotaryKiln\api-ms-win-core-libraryloader-l1-2-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Visual Studio Application\��תҤ\Package\RotaryKiln\dailyrecord.conf"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Visual Studio Application\��תҤ\Package\RotaryKiln\MachineRegistrationClient.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Visual Studio Application\��תҤ\Package\RotaryKiln\msvcp120.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Visual Studio Application\��תҤ\Package\RotaryKiln\msvcr120.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Visual Studio Application\��תҤ\Package\RotaryKiln\opcdata"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Visual Studio Application\��תҤ\Package\RotaryKiln\opencv_core2411.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Visual Studio Application\��תҤ\Package\RotaryKiln\opencv_highgui2411.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Visual Studio Application\��תҤ\Package\RotaryKiln\opencv_imgproc2411.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Visual Studio Application\��תҤ\Package\RotaryKiln\pthreadGC2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Visual Studio Application\��תҤ\Package\RotaryKiln\region.dat"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Visual Studio Application\��תҤ\Package\RotaryKiln\YoseenFfmpeg.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Visual Studio Application\��תҤ\Package\RotaryKiln\YoseenSDK.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Visual Studio Application\��תҤ\Package\RotaryKiln\db"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs    
Source: "E:\Visual Studio Application\��תҤ\Package\RotaryKiln\db\RotaryKiln.accdb"; DestDir: "{app}\db"; Flags: ignoreversion recursesubdirs createallsubdirs
; ע��: ��Ҫ���κι���ϵͳ�ļ���ʹ�á�Flags: ignoreversion��

[Icons]
Name: "{commonprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
Name: "{userdesktop}\{#MyAppName}";Filename: "{app}\{#MyAppExeName}"; WorkingDir: "{app}"

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

