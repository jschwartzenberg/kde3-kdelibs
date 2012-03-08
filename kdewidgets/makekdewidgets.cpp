/* Copyright (C) 2004-2005 ian reinhart geiser <geiseri@sourcextreme.com> */
#include <kaboutdata.h>
#include <kinstance.h>
#include <kcmdlineargs.h>
#include <kconfig.h>
#include <kmacroexpander.h>
#include <kdebug.h>

#include <qtextstream.h>
#include <qfileinfo.h>
#include <qfile.h>
#include <qbuffer.h>
#include <qimage.h>

static const char description[] = I18N_NOOP( "Builds Qt widget plugins from an ini style description file." );
static const char version[] = "0.2";
static const char classHeader[] = 	"/**\n"
					"* This file was autogenerated by makekdewidgets. Any changes will be lost!\n"
					"* The generated code in this file is licensed under the same license that the\n"
					"* input file.\n"
                                   	"*/\n"
					"#include <qwidgetplugin.h>\n";
static const char classDef[] = "#ifndef EMBED_IMAGES\n"
                                "#include <kstandarddirs.h>\n"
                                "#endif\n"
                                "\n"
                                "class %PluginName : public QWidgetPlugin\n"
                                "{\n"
                                "public:\n"
                                "	%PluginName();\n"
                                "	\n"
                                "	virtual ~%PluginName();\n"
                                "	\n"
                                "	virtual QStringList keys() const\n"
                                "	{\n"
                                "		QStringList result;\n"
                                "		for (WidgetInfos::ConstIterator it = m_widgets.begin(); it != m_widgets.end(); ++it)\n"
                                "			result << it.key();\n"
                                "		return result;\n"
                                "	}\n"
                                "	\n"
                                "	virtual QWidget *create(const QString &key, QWidget *parent = 0, const char *name = 0);\n"
                                "	\n"
                                "	virtual QIconSet iconSet(const QString &key) const\n"
                                "	{\n"
                                "#ifdef EMBED_IMAGES\n"
                                "		QPixmap pix(m_widgets[key].iconSet);\n"
                                "#else\n"
                                "		QPixmap pix(locate( \"data\", \n"
				"			QString::fromLatin1(\"%PluginNameLower/pics/\") + m_widgets[key].iconSet));\n"
                                "#endif\n"
                                "		return QIconSet(pix);\n"
                                "	}\n"
                                "	\n"
                                "	virtual bool isContainer(const QString &key) const { return m_widgets[key].isContainer; }\n"
                                "	\n"
                                "	virtual QString group(const QString &key) const { return m_widgets[key].group; }\n"
                                "	\n"
                                "	virtual QString includeFile(const QString &key) const { return m_widgets[key].includeFile; }\n"
                                "	\n"
                                "	virtual QString toolTip(const QString &key) const { return m_widgets[key].toolTip; }\n"
                                "	\n"
                                "	virtual QString whatsThis(const QString &key) const { return m_widgets[key].whatsThis; }\n"
                                "private:\n"
                                "	struct WidgetInfo\n"
                                "	{\n"
                                "		QString group;\n"
                                "#ifdef EMBED_IMAGES\n"
                                "		QPixmap iconSet;\n"
                                "#else\n"
                                "		QString iconSet;\n"
                                "#endif\n"
                                "		QString includeFile;\n"
                                "		QString toolTip;\n"
                                "		QString whatsThis;\n"
                                "		bool isContainer;\n"
                                "	};\n"
                                "	typedef QMap<QString, WidgetInfo> WidgetInfos;\n"
                                "	WidgetInfos m_widgets;\n"
                                "};\n"
                                "%PluginName::%PluginName()\n"
                                "{\n"
                                "        WidgetInfo widget;\n";
static const char widgetDef[] = "	widget.group = QString::fromLatin1(\"%Group\");\n"
                                 "#ifdef EMBED_IMAGES\n"
                                 "	widget.iconSet = QPixmap(%Pixmap);\n"
                                 "#else\n"
                                 "	widget.iconSet = QString::fromLatin1(\"%IconSet\");\n"
                                 "#endif\n"
                                 "	widget.includeFile = QString::fromLatin1(\"%IncludeFile\");\n"
                                 "	widget.toolTip = QString::fromLatin1(\"%ToolTip\");\n"
                                 "	widget.whatsThis = QString::fromLatin1(\"%WhatsThis\");\n"
                                 "	widget.isContainer = %IsContainer;\n"
                                 "	m_widgets.insert(QString::fromLatin1(\"%Class\"), widget);\n";
static const char endCtor[] = "	%Init\n"
                               "}\n"
                               "%PluginName::~%PluginName()\n"
                               "{\n"
                               "	%Destroy\n"
                               "}\n"
                               "QWidget *%PluginName::create(const QString &key, QWidget *parent, const char *name)\n"
                               "{\n";
static const char widgetCreate[] = "         if (key == QString::fromLatin1(\"%Class\"))\n"
                                    "                return new %ImplClass%ConstructorArgs;\n";
static const char endCreate[] = "	return 0;\n"
                                 "}\n"
                                 "KDE_Q_EXPORT_PLUGIN(%PluginName)\n";


static KCmdLineOptions options[] =
    {
        { "+file", I18N_NOOP( "Input file" ), 0 },
        { "o <file>", I18N_NOOP( "Output file" ), 0 },
        { "n <plugin name>", I18N_NOOP( "Name of the plugin class to generate" ), "WidgetsPlugin" },
        { "g <group>", I18N_NOOP( "Default widget group name to display in designer" ), "Custom" },
        { "p <pixmap dir>", I18N_NOOP( "Embed pixmaps from a source directory" ), 0 },
        KCmdLineLastOption
    };

static QString buildWidgetDef( const QString &name, KConfig &input, const QString &group );
static QString buildWidgetCreate( const QString &name, KConfig &input );
static QString buildWidgetInclude( const QString &name, KConfig &input );
static void buildFile( QTextStream &stream, const QString& group, const QString& fileName, const QString& pluginName, const QString& iconPath );
static QString buildPixmap( const QString &name, KConfig &input, const QString &iconPath );

int main( int argc, char **argv ) {
    new KInstance( "makekdewidgets" );

    KAboutData about( "makekdewidgets", I18N_NOOP( "makekdewidgets" ), version, description, KAboutData::License_GPL, "(C) 2004-2005 ian reinhart geiser", 0, 0, "geiseri@kde.org" );
    about.addAuthor( "ian reinhart geiser", 0, "geiseri@kde.org" );
    KCmdLineArgs::init( argc, argv, &about );
    KCmdLineArgs::addCmdLineOptions( options );
    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
    if ( args->count() < 1 ) {
        args->usage();
        return ( 1 );
    }

    QFileInfo fi( args->arg( args->count() - 1 ) );

    QString outputFile = args->getOption( "o" );
    QString pluginName = args->getOption( "n" );
    QString group = args->getOption( "g" );
    QString iconPath = "";
    if ( args->isSet( "p" ) )
        iconPath = args->getOption( "p" );
    QString fileName = fi.absFilePath();

    if ( args->isSet( "o" ) ) {
        QFile output( outputFile );
        if ( output.open( IO_WriteOnly ) ) {
            QTextStream ts( &output );
            buildFile( ts, group, fileName , pluginName, iconPath );
        }
        output.close();
    } else {
        QTextStream ts( stdout, IO_WriteOnly );
        buildFile( ts, group, fileName , pluginName, iconPath );
    }
}

void buildFile( QTextStream &ts, const QString& group, const QString& fileName, const QString& pluginName, const QString& iconPath ) {
    KConfig input( fileName, true, false );
    input.setGroup( "Global" );
    QMap<QString, QString> MainMap;
    MainMap.insert( "PluginName", input.readEntry( "PluginName", pluginName ) );
    MainMap.insert( "PluginNameLower", input.readEntry( "PluginName", pluginName ).lower() );
    MainMap.insert( "Init", input.readEntry( "Init", "" ) );
    MainMap.insert( "Destroy", input.readEntry( "Destroy", "" ) );
    ts << classHeader << endl;
    QStringList includes = input.readListEntry( "Includes", ',' );
    for ( uint idx = 0; idx < includes.count(); ++idx )
        ts << "#include <" << includes[ idx ] << ">" << endl;
    QStringList classes = input.groupList();
    classes.remove( classes.find( "Global" ) );
    // Autogenerate widget includes here
    for ( uint idx = 0; idx < classes.count(); ++idx )
        ts << buildWidgetInclude( classes[ idx ], input ) << endl;
    // Generate embedded icons
    if ( !iconPath.isEmpty() ) {
        for ( uint idx = 0; idx < classes.count(); ++idx )
            ts << buildPixmap( classes[ idx ], input, iconPath ) << endl;
        ts << "#define EMBED_IMAGES" << endl;
    }
    // Generate the main class code.
    ts << KMacroExpander::expandMacros( classDef, MainMap ) << endl;
    // Autogenerate widget defs here
    for ( uint idx = 0; idx < classes.count(); ++idx )
        ts << buildWidgetDef( classes[ idx ], input, group ) << endl;
    ts << KMacroExpander::expandMacros( endCtor, MainMap ) << endl;
    // Autogenerate create code here...
    for ( uint idx = 0; idx < classes.count(); ++idx )
        ts << buildWidgetCreate( classes[ idx ], input ) << endl;
    ts << KMacroExpander::expandMacros( endCreate, MainMap ) << endl;

}

QString buildWidgetDef( const QString &name, KConfig &input, const QString &group ) {
    input.setGroup( name );
    QMap<QString, QString> defMap;
    defMap.insert( "Group", input.readEntry( "Group", group ).replace( "\"", "\\\"" ) );
    defMap.insert( "IconSet", input.readEntry( "IconSet", name.lower() + ".png" ).replace( ":", "_" ) );
    defMap.insert( "Pixmap", name.lower().replace( ":", "_" ) + "_xpm" );
    defMap.insert( "IncludeFile", input.readEntry( "IncludeFile", name.lower() + ".h" ).remove( ":" ) );
    defMap.insert( "ToolTip", input.readEntry( "ToolTip", name + " Widget" ).replace( "\"", "\\\"" ) );
    defMap.insert( "WhatsThis", input.readEntry( "WhatsThis", name + " Widget" ).replace( "\"", "\\\"" ) );
    defMap.insert( "IsContainer", input.readEntry( "IsContainer", "false" ) );
    defMap.insert( "Class", name );
    return KMacroExpander::expandMacros( widgetDef, defMap );
}

QString buildWidgetCreate( const QString &name, KConfig &input ) {
    input.setGroup( name );
    QMap<QString, QString> createMap;
    createMap.insert( "ImplClass", input.readEntry( "ImplClass", name ) );
    createMap.insert( "ConstructorArgs", input.readEntry( "ConstructorArgs", "(parent, name)" ) );
    createMap.insert( "Class", name );
    return KMacroExpander::expandMacros( widgetCreate, createMap );
}

QString buildWidgetInclude( const QString &name, KConfig &input ) {
    input.setGroup( name );
    return "#include <" + input.readEntry( "IncludeFile", name.lower() + ".h" ) + ">";
}

QString buildPixmap( const QString &name, KConfig &input, const QString &iconPath ) {
    input.setGroup( name );
    QString cleanName = name.lower().replace( ":", "_" );
    QString iconName = input.readEntry( "IconSet", cleanName + ".png" );

    QFileInfo fi( iconPath + "/" + iconName );
    QImage pix( fi.absFilePath() );
    QCString xpm;
    QBuffer buff( xpm );
    buff.open( IO_WriteOnly );
    QImageIO io( &buff, "XPM" );
    io.setFileName( cleanName + "_xpm" );
    io.setImage( pix );
    io.write();
    buff.close();
    return xpm;
}