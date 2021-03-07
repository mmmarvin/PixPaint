; -- PixPaint Installer Script
#define AppExeName "PixPaint.exe"
#define UnInstallerIcon "uninst.ico"

[Setup]
AppName = PixPaint
AppVersion = 1.02
WizardStyle = modern
DefaultDirName = {autopf}\PixPaint
DefaultGroupName = PixPaint
UninstallDisplayIcon={app}\{#UnInstallerIcon}
Compression=lzma2
SolidCompression=yes
PrivilegesRequiredOverridesAllowed = commandline dialog
PrivilegesRequired=lowest
ChangesAssociations=yes

[Files]
Source: "changelog.txt"; Destdir: "{app}"
Source: "libboost_python38-mt.dll"; Destdir: "{app}"
Source: "libbrotlicommon.dll"; Destdir: "{app}"
Source: "libbrotlidec.dll"; Destdir: "{app}"
Source: "libbz2-1.dll"; Destdir: "{app}"
Source: "libdouble-conversion.dll"; Destdir: "{app}"
Source: "libfreetype-6.dll"; Destdir: "{app}"
Source: "libgcc_s_seh-1.dll"; Destdir: "{app}"
Source: "libglib-2.0-0.dll"; Destdir: "{app}"
Source: "libgraphite2.dll"; Destdir: "{app}"
Source: "libharfbuzz-0.dll"; Destdir: "{app}"
Source: "libiconv-2.dll"; Destdir: "{app}"
Source: "libicudt68.dll"; Destdir: "{app}"
Source: "libicuin68.dll"; Destdir: "{app}"
Source: "libicuuc68.dll"; Destdir: "{app}"
Source: "libintl-8.dll"; Destdir: "{app}"
Source: "libpcre-1.dll"; Destdir: "{app}"
Source: "libpcre2-16-0.dll"; Destdir: "{app}"
Source: "libpng16-16.dll"; Destdir: "{app}"
Source: "libpython3.8.dll"; Destdir: "{app}"
Source: "libstdc++-6.dll"; Destdir: "{app}"
Source: "libwinpthread-1.dll"; Destdir: "{app}"
Source: "libzstd.dll"; Destdir: "{app}"
Source: "LICENSE.txt"; Destdir: "{app}"
Source: "PixPaint.exe"; Destdir: "{app}"
Source: "platforms\qwindows.dll"; Destdir: "{app}\platforms"
Source: "Qt5Core.dll"; Destdir: "{app}"
Source: "Qt5Gui.dll"; Destdir: "{app}"
Source: "Qt5Widgets.dll"; Destdir: "{app}"
Source: "res\add_btn_icon.png"; Destdir: "{app}\res"
Source: "res\add_cpy_btn_icon.png"; Destdir: "{app}\res"
Source: "res\back_begin_btn_icon.png"; Destdir: "{app}\res"
Source: "res\back_btn_icon.png"; Destdir: "{app}\res"
Source: "res\cursor\color_picker_cursor.png"; Destdir: "{app}\res\cursor"
Source: "res\cursor\eraser_cursor.png"; Destdir: "{app}\res\cursor"
Source: "res\cursor\fill_cursor.png"; Destdir: "{app}\res\cursor"
Source: "res\cursor\hourglass_cursor.png"; Destdir: "{app}\res\cursor"
Source: "res\cursor\magic_wand_cursor.png"; Destdir: "{app}\res\cursor"
Source: "res\cursor\pencil_cursor.png"; Destdir: "{app}\res\cursor"
Source: "res\forward_btn_icon.png"; Destdir: "{app}\res"
Source: "res\forward_end_btn_icon.png"; Destdir: "{app}\res"
Source: "res\icons\brush_tool.png"; Destdir: "{app}\res\icons"
Source: "res\icons\circle_tool.png"; Destdir: "{app}\res\icons"
Source: "res\icons\color_picker_tool.png"; Destdir: "{app}\res\icons"
Source: "res\icons\custom_brush_tool.png"; Destdir: "{app}\res\icons"
Source: "res\icons\dither_fill_tool.png"; Destdir: "{app}\res\icons"
Source: "res\icons\eraser_tool.png"; Destdir: "{app}\res\icons"
Source: "res\icons\fill_tool.png"; Destdir: "{app}\res\icons"
Source: "res\icons\line_tool.png"; Destdir: "{app}\res\icons"
Source: "res\icons\magic_wand_tool.png"; Destdir: "{app}\res\icons"
Source: "res\icons\pencil_tool.png"; Destdir: "{app}\res\icons"
Source: "res\icons\rectselection_tool.png"; Destdir: "{app}\res\icons"
Source: "res\icons\rect_tool.png"; Destdir: "{app}\res\icons"
Source: "res\icons\text_selection_tool.png"; Destdir: "{app}\res\icons"
Source: "res\option_ascale.png"; Destdir: "{app}\res"
Source: "res\option_resize.png"; Destdir: "{app}\res"
Source: "res\option_rotate_clockwise.png"; Destdir: "{app}\res"
Source: "res\option_rotate_counter_clockwise.png"; Destdir: "{app}\res"
Source: "res\option_scale.png"; Destdir: "{app}\res"
Source: "res\pause_btn_icon.png"; Destdir: "{app}\res"
Source: "res\pixpaint.png"; Destdir: "{app}\res"
Source: "res\play_btn_icon.png"; Destdir: "{app}\res"
Source: "res\rem_btn_icon.png"; Destdir: "{app}\res"
Source: "thirdpartylicense.txt"; Destdir: "{app}"
Source: "uninst.ico"; Destdir: "{app}"
Source: "zlib1.dll"; Destdir: "{app}"

[Registry]
Root: HKCR; Subkey: ".ppf";                             ValueData: "{#MyAppName}";             Flags: uninsdeletevalue; ValueType: string;  ValueName: ""
Root: HKCR; Subkey: "{#MyAppName}";                     ValueData: "Program {#MyAppName}";     Flags: uninsdeletekey;   ValueType: string;  ValueName: ""
Root: HKCR; Subkey: "{#MyAppName}\DefaultIcon";         ValueData: "{app}\{#MyAppExeName},0";                           ValueType: string;  ValueName: ""
Root: HKCR; Subkey: "{#MyAppName}\shell\open\command";  ValueData: """{app}\{#MyAppExeName}""  ""%1""";                 ValueType: string;  ValueName: ""

[Dirs]
Name: "{app}\scripts"
Name: "{app}\scripts\tools"
Name: "{app}\scripts\effects"
Name: "{app}\scripts\animation_filetype"
Name: "{app}\scripts\image_filetype"

[Icons]
Name: "{userdesktop}\PixPaint"; Filename: "{app}\{#AppExeName}"
Name: "{userdesktop}\PixPaint NoPython"; Filename: "{app}\{#AppExeName} --nopython"
