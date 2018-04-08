#ifndef HEADERWIDGET
#define HEADERWIDGET

#include <QMainWindow>
#include <QFrame>
#include <QMouseEvent>

class HeaderWidget : public QWidget
{
public:
    HeaderWidget(QWidget* _parent)
        :QWidget(_parent)
    {
		isPressed = false;
    }

    HeaderWidget()
        :QWidget(NULL)
    {

    }

    void SetWindow(QMainWindow* _window)
    {
        window_ = _window;
    }

protected:
    void mousePressEvent(QMouseEvent* _event)
    {
        if (_event->button() == Qt::LeftButton)
        {
            isPressed = true;
            dragPosition = _event->globalPos() - window_->frameGeometry().topLeft();
            _event->accept();
        }
    }

    void mouseMoveEvent(QMouseEvent* _event)
    {
        if(!isPressed)
            return;

        if (_event->buttons() == Qt::LeftButton)
        {
            window_->move(_event->globalPos() - dragPosition);
            _event->accept();
        }
    }

    void mouseReleaseEvent(QMouseEvent* )
    {
        isPressed = false;
    }

private:
    QPoint dragPosition;
    QMainWindow* window_;
    bool isPressed;
};

#endif // HEADERWIDGET

