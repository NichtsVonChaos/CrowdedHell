#ifndef AVOIDANCEDISPLAYWIDGET_H
#define AVOIDANCEDISPLAYWIDGET_H

#include <QOpenGLWidget>

class CrowdedHellGUI;
enum class MessageType;
enum class Language;

#ifndef CROWDEDHELLGUI_H
#include "crowdedhellgui.h"
#endif

class AvoidanceDisplayWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	AvoidanceDisplayWidget(CrowdedHellGUI *parent = Q_NULLPTR);
};

#endif // AVOIDANCEDISPLAYWIDGET_H
