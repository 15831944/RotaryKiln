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
SetupIconFile=RotaryKiln\Setup.ico
Compression=lzma
SolidCompression=yes
PrivilegesRequired=admin      
InfoAfterFile=RotaryKiln\ע��.txt

[Languages]
Name: "chinesesimp"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Files]
Source: "D:\ZhuGF\RotaryKiln\Release\RotaryKiln.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "RotaryKiln\api-ms-win-core-libraryloader-l1-2-0.dll"; DestDir: "{app}"; Flags: ignoreversion       
Source: "RotaryKiln\api-ms-win-crt-runtime-l1-1-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "RotaryKiln\MachineRegistrationClient.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "RotaryKiln\mfc140.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "RotaryKiln\msvcp120.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "RotaryKiln\msvcp140.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "RotaryKiln\msvcp140d.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "RotaryKiln\msvcr100.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "RotaryKiln\msvcr120.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "RotaryKiln\opencv_core2411.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "RotaryKiln\opencv_highgui2411.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "RotaryKiln\opencv_imgproc2411.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "RotaryKiln\pthreadGC2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "RotaryKiln\vcruntime140.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "RotaryKiln\vcruntime140d.dll"; DestDir: "{app}"; Flags: ignoreversion   
Source: "RotaryKiln\YoseenFfmpeg.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "RotaryKiln\YoseenSDK.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "RotaryKiln\oledb32.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "RotaryKiln\sqloledb.dll"; DestDir: "{app}"; Flags: ignoreversion       
Source: "RotaryKiln\region.dat"; DestDir: "{app}"; Flags: ignoreversion
Source: "RotaryKiln\��תҤRotaryKiln��װ�����.pdf"; DestDir: "{app}"; Flags: ignoreversion     
Source: "RotaryKiln\opcdata"; DestDir: "{app}"; Flags: ignoreversion
Source: "RotaryKiln\vc_redist.x86.exe"; DestDir: "{app}"; Flags: ignoreversion    
Source: "RotaryKiln\AccessDatabaseEngine 2010.exe"; DestDir: "{app}"; Flags: ignoreversion 
Source: "RotaryKiln\Engine_install.bat"; DestDir: "{app}"; Flags: ignoreversion 
Source: "RotaryKiln\Engine_install_passive.bat"; DestDir: "{app}"; Flags: ignoreversion  
Source: "RotaryKiln\db\RotaryKiln.accdb"; DestDir: "{app}\db"; Flags: ignoreversion recursesubdirs createallsubdirs
; ע��: ��Ҫ���κι���ϵͳ�ļ���ʹ�á�Flags: ignoreversion��

[Icons]
Name: "{commonprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
Name: "{userdesktop}\{#MyAppName}";Filename: "{app}\{#MyAppExeName}"; WorkingDir: "{app}"


[Run]          
Filename: "{app}\vc_redist.x86.exe";
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait skipifsilent postinstall

