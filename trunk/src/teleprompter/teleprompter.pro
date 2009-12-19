
TARGET = dviz-viewer
TEMPLATE = app
DEPENDPATH += $$PWD ../
INCLUDEPATH += $$PWD ../
VPATH += ../

CONFIG += debug


win32 {
	INCLUDEPATH += \
		../external/ffmpeg/include/msinttypes \
		../external/ffmpeg/include/libswscale \
		../external/ffmpeg/include/libavutil \
		../external/ffmpeg/include/libavdevice \
		../external/ffmpeg/include/libavformat \
		../external/ffmpeg/include/libavcodec \
		../external/ffmpeg/include
	
	LIBS += -L"../external/ffmpeg/lib" \
		-lavcodec-51 \
		-lavformat-52 \
		-lavutil-49 \
		-lavdevice-52 \
		-lswscale-0
}


include(../frames/frames.pri)
include(../items/items.pri)
include(../model/model.pri)
include(../songdb/songdb.pri)
include(../itemlistfilters/itemlistfilters.pri)
include(../3rdparty/richtextedit/richtextedit.pri)
include(../3rdparty/qtgradienteditor/qtgradienteditor.pri)
include(../3rdparty/videocapture/videocapture.pri)
include(../3rdparty/analyzers/analyzers.pri)
include(../qvideo/qvideo.pri)
include(../qtcolorpicker/qtcolorpicker.pri)
include(../qtmultimedia/audio/audio.pri)
include(../3rdparty/rtmidi/rtmidi.pri)
include(../3rdparty/md5/md5.pri)
include(../3rdparty/qjson/qjson.pri)
include(../ppt/ppt.pri)
include(../groupplayer/groupplayer.pri)

MOC_DIR = .build
OBJECTS_DIR = .build
RCC_DIR = .build
UI_DIR = .build

# Input

FORMS += MainWindow.ui \
	outputsetupdialog.ui \
	SingleOutputSetupDialog.ui \
	AppSettingsDialog.ui \
	DocumentSettingsDialog.ui \
	GridDialog.ui \
	SlideSettingsDialog.ui \
	SlideGroupSettingsDialog.ui 
	
RESOURCES += dviz.qrc

HEADERS += \
	MainWindow.h \
	MyGraphicsScene.h \
	RenderOpts.h \
	SlideGroupListModel.h \
	SlideEditorWindow.h \
	DocumentListModel.h \
	SlideGroupViewer.h \
	OutputSetupDialog.h \
	SingleOutputSetupDialog.h \
	AppSettings.h \
	AppSettingsDialog.h \
	DocumentSettingsDialog.h \
	GridDialog.h \
	SlideSettingsDialog.h \
	SlideGroupSettingsDialog.h \
	ImageFilters.h \
	SlideItemListModel.h \
	MediaBrowser.h \
	MediaBrowserDialog.h \
	OutputInstance.h \
	OutputControl.h \
	JpegServer.h \
	DeepProgressIndicator.h \
	OutputServer.h \
	DirectoryListModel.h \
	viewer/NetworkClient.h
	
SOURCES += main.cpp \
	MainWindow.cpp \
	viewer/NetworkClient.cpp \
	MyGraphicsScene.cpp \
	SlideGroupListModel.cpp \
	SlideEditorWindow.cpp \
	DocumentListModel.cpp \
	SlideGroupViewer.cpp \
	OutputViewer.cpp \
	OutputSetupDialog.cpp \
	SingleOutputSetupDialog.cpp \
	AppSettings.cpp \
	AppSettingsDialog.cpp \
	DocumentSettingsDialog.cpp \
	GridDialog.cpp \
	SlideSettingsDialog.cpp \
	SlideGroupSettingsDialog.cpp \
	ImageFilters.cpp \
	SlideItemListModel.cpp \
	MediaBrowser.cpp \
	MediaBrowserDialog.cpp \
	OutputInstance.cpp \
	OutputControl.cpp \
	JpegServer.cpp \
	DeepProgressIndicator.cpp \
	OutputServer.cpp \
	DirectoryListModel.cpp
	

QT += core \
	gui \
	svg \
	network \
	xml \
	sql \
	opengl

unix {
	LIBS += -lavdevice -lavformat -lavcodec -lavutil -lswscale -lbz2
}


# deployment on Linux
unix {
	target.path = /usr/bin
	icon.files = dviz.png
	icon.path = /usr/share/pixmaps
	dfile.files = dviz.desktop
	dfile.path = /usr/share/applications
	man.files = dviz.1
	man.path = /usr/share/man/man1
	INSTALLS += target \
		icon \
		dfile \
		man
}

# static builds
win32|macx {
	contains(CONFIG, static)|contains(CONFIG, qt_no_framework) {
		DEFINES  += STATIC_LINK
		QTPLUGIN += qgif \
			qjpeg \
			qsvg \
			qtiff
	}
}






########################################33

VPATH += ../imgtool/exiv2-0.18.2-qtbuild/
DEPENDPATH += ../imgtool/exiv2-0.18.2-qtbuild/

MOC_DIR = .build
OBJECTS_DIR = .build
RCC_DIR = .build
UI_DIR = .build

DEPENDPATH += . \
              config \
              src \
#              contrib/organize \
              xmpsdk/include \
              xmpsdk/src \
              xmpsdk/include/client-glue
INCLUDEPATH += . \
               ../imgtool/exiv2-0.18.2-qtbuild/src \
               ../imgtool/exiv2-0.18.2-qtbuild/xmpsdk/include \
               ../imgtool/exiv2-0.18.2-qtbuild/xmpsdk/include/client-glue \
#               contrib/organize \
               ../imgtool/exiv2-0.18.2-qtbuild/xmpsdk/src

DEFINES += EXV_LOCALEDIR=\"/usr/local/share/locale\" EXV_BUILDING_LIB=1 NDEBUG=1
DEFINES += EXV_HAVE_STDINT_H=1

# Input
HEADERS += src/basicio.hpp \
           src/bmpimage.hpp \
           src/canonmn.hpp \
           src/convert.hpp \
           src/cr2image.hpp \
           src/cr2image_int.hpp \
           src/crwimage.hpp \
           src/crwimage_int.hpp \
           src/datasets.hpp \
           src/doxygen.hpp \
           src/easyaccess.hpp \
           src/error.hpp \
           src/exif.hpp \
           src/exiv2.hpp \
           src/exv_conf.h \
           src/fujimn.hpp \
           src/futils.hpp \
           src/gifimage.hpp \
           src/i18n.h \
           src/image.hpp \
           src/iptc.hpp \
           src/jp2image.hpp \
           src/jpgimage.hpp \
           src/makernote_int.hpp \
           src/metacopy.hpp \
           src/metadatum.hpp \
           src/minoltamn.hpp \
           src/mn.hpp \
           src/mrwimage.hpp \
           src/nikonmn.hpp \
           src/olympusmn.hpp \
           src/orfimage.hpp \
           src/orfimage_int.hpp \
           src/panasonicmn.hpp \
           src/pentaxmn.hpp \
           src/pngchunk_int.hpp \
           src/pngimage.hpp \
           src/preview.hpp \
           src/private.h \
           src/properties.hpp \
           src/psdimage.hpp \
           src/rafimage.hpp \
           src/rcsid.hpp \
           src/rw2image.hpp \
           src/rw2image_int.hpp \
           src/sigmamn.hpp \
           src/sonymn.hpp \
           src/tags.hpp \
           src/tgaimage.hpp \
           src/tiffcomposite_int.hpp \
           src/tifffwd_int.hpp \
           src/tiffimage.hpp \
           src/tiffimage_int.hpp \
           src/tiffvisitor_int.hpp \
           src/timegm.h \
           src/types.hpp \
           src/tzfile.h \
           src/utils.hpp \
           src/value.hpp \
           src/version.hpp \
           src/xmp.hpp \
           src/xmpsidecar.hpp \
#           contrib/organize/helpers.hpp \
#           contrib/organize/MD5.h \
           xmpsdk/include/XMPMD5.h \
           xmpsdk/include/TXMPIterator.hpp \
           xmpsdk/include/TXMPMeta.hpp \
           xmpsdk/include/TXMPUtils.hpp \
           xmpsdk/include/XMP_Const.h \
           xmpsdk/include/XMP_Environment.h \
           xmpsdk/include/XMP_Version.h \
           xmpsdk/include/XMPSDK.hpp \
           xmpsdk/src/ExpatAdapter.hpp \
           xmpsdk/src/UnicodeConversions.hpp \
           xmpsdk/src/XMLParserAdapter.hpp \
           xmpsdk/src/XMP_BuildInfo.h \
           xmpsdk/src/XMPCore_Impl.hpp \
           xmpsdk/src/XMPIterator.hpp \
           xmpsdk/src/XMPMeta.hpp \
           xmpsdk/src/XMPUtils.hpp \
           xmpsdk/include/client-glue/WXMP_Common.hpp \
           xmpsdk/include/client-glue/WXMPFiles.hpp \
           xmpsdk/include/client-glue/WXMPIterator.hpp \
           xmpsdk/include/client-glue/WXMPMeta.hpp \
           xmpsdk/include/client-glue/WXMPUtils.hpp \
           xmpsdk/include/client-glue/TXMPMeta.incl_cpp \
           xmpsdk/include/client-glue/TXMPIterator.incl_cpp \
           xmpsdk/include/client-glue/TXMPUtils.incl_cpp \
           xmpsdk/include/XMP.incl_cpp \
           xmpsdk/src/UnicodeInlines.incl_cpp
SOURCES += src/basicio.cpp \
           src/bmpimage.cpp \
           src/canonmn.cpp \
           src/convert.cpp \
           src/cr2image.cpp \
#           src/crwedit.cpp \
           src/crwimage.cpp \
#           src/crwparse.cpp \
           src/datasets.cpp \
           src/easyaccess.cpp \
           src/error.cpp \
           src/exif.cpp \
           src/fujimn.cpp \
           src/futils.cpp \
           src/gifimage.cpp \
           src/image.cpp \
           src/iptc.cpp \
           src/jp2image.cpp \
           src/jpgimage.cpp \
           src/localtime.c \
           src/makernote.cpp \
#           src/metacopy.cpp \
           src/metadatum.cpp \
           src/minoltamn.cpp \
           src/mrwimage.cpp \
#           src/mrwthumb.cpp \
           src/nikonmn.cpp \
           src/olympusmn.cpp \
           src/orfimage.cpp \
           src/panasonicmn.cpp \
#           src/path-test.cpp \
           src/pentaxmn.cpp \
           src/pngchunk.cpp \
           src/pngimage.cpp \
           src/preview.cpp \
           src/properties.cpp \
           src/psdimage.cpp \
           src/rafimage.cpp \
           src/rw2image.cpp \
           src/sigmamn.cpp \
           src/sonymn.cpp \
#           src/taglist.cpp \
           src/tags.cpp \
           src/tgaimage.cpp \
#src/tiff-test.cpp \
           src/tiffcomposite.cpp \
           src/tiffimage.cpp \
#           src/tiffmn-test.cpp \
           src/tiffvisitor.cpp \
           src/types.cpp \
           src/utils.cpp \
#           src/utiltest.cpp \
           src/value.cpp \
           src/version.cpp \
           src/xmp.cpp \
#           src/xmpdump.cpp \
           src/xmpsidecar.cpp \
#           contrib/organize/helpers.cpp \
#           contrib/organize/MD5.cpp \
#           contrib/organize/organize.cpp \
           xmpsdk/src/ExpatAdapter.cpp \
           xmpsdk/src/XMPMD5.cpp \
           xmpsdk/src/ParseRDF.cpp \
           xmpsdk/src/UnicodeConversions.cpp \
           xmpsdk/src/WXMPIterator.cpp \
           xmpsdk/src/WXMPMeta.cpp \
           xmpsdk/src/WXMPUtils.cpp \
           xmpsdk/src/XMPCore_Impl.cpp \
           xmpsdk/src/XMPIterator.cpp \
           xmpsdk/src/XMPMeta-GetSet.cpp \
           xmpsdk/src/XMPMeta-Parse.cpp \
           xmpsdk/src/XMPMeta-Serialize.cpp \
           xmpsdk/src/XMPMeta.cpp \
           xmpsdk/src/XMPUtils-FileInfo.cpp \
           xmpsdk/src/XMPUtils.cpp

win32 {
	HEADERS += ../imgtool/exiv2-0.18.2-qtbuild/src/getopt_win32.h 
	SOURCES += ../imgtool/exiv2-0.18.2-qtbuild/src/getopt_win32.c 
	DEFINES += WIN32
	
	INCLUDEPATH += ../imgtool/exiv2-0.18.2-qtbuild/win32/zlib123 \
		../imgtool/exiv2-0.18.2-qtbuild/win32/expat201/Source/lib 

	DEPENDPATH += ../imgtool/exiv2-0.18.2-qtbuild/win32/zlib123 \
		../imgtool/exiv2-0.18.2-qtbuild/win32/zlib123dll/dll32 \
		../imgtool/exiv2-0.18.2-qtbuild/win32/expat201/Source/lib 
		
	LIBS += -L"../imgtool/exiv2-0.18.2-qtbuild/win32/zlib123dll/dll32" \
		-lzlibwapi \
		-L"../imgtool/exiv2-0.18.2-qtbuild/win32/expat201/Bin" \
		-llibexpat \
		-llibexpatw 
	
}

unix {
	DEFINES += \
		EXV_HAVE_SYS_MMAN_H \
		EXV_HAVE_MMAP \
		EXV_HAVE_MUNMAP \
		EXV_HAVE_S_ISLNK \
		EXV_HAVE_LSTAT \
		EXV_HAVE_LIBINTL_H \	#/* Define if you have the <libintl.h> header file. */
		EXV_ENABLE_NLS \	#/* Define if translation of program messages to the user's native language is requested. */
		EXV_HAVE_TIMEGM	\	#/* Define if you have the `timegm' function. */
		EXV_HAVE_GMTIME_R \	#/* Define if you have the 'gmtime_r' function */		
		EXV_HAVE_STRERROR_R \ 	# 'stderror_r' function
		EXV_HAVE_ICONV		# 'iconv' function
		
}


