﻿#include "qbasicformmoveablewidget.h"
#include <QMouseEvent>
#include <QStyleOption>
#include <QPainter>
#include <windows.h>
#include <windowsx.h>

QBasicFormMoveableWidget::QBasicFormMoveableWidget(QWidget *parent) :
    QWidget(parent)
{
    isLeftPressDown_ = false;
    this->dir_ = NONE;
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setMouseTracking(true);
    move_plan_ = PLANA;
}

void QBasicFormMoveableWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && move_plan_ == PLANA) {
        isLeftPressDown_ = true;
        if(dir_ != NONE) {
            this->mouseGrabber();
        } else {
            dragPosition_ = event->globalPos() - this->frameGeometry().topLeft();
        }
    }
    QWidget::mousePressEvent(event);
}

void QBasicFormMoveableWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(move_plan_ == PLANA){
        QPoint gloPoint = event->globalPos();
        QRect rect = this->rect();
        QPoint tl = mapToGlobal(rect.topLeft());
        QPoint rb = mapToGlobal(rect.bottomRight());
        if(!isLeftPressDown_) {
            this->region(gloPoint);
        } else {
            if(dir_ != NONE) {
                QRect rMove(tl, rb);
                switch(dir_) {
                case LEFT:
                    if(rb.x() - gloPoint.x() <= this->minimumWidth())
                        rMove.setX(tl.x());
                    else
                        rMove.setX(gloPoint.x());
                    break;
                case RIGHT:
                    rMove.setWidth(gloPoint.x() - tl.x());
                    break;
                case UP:
                    if(rb.y() - gloPoint.y() <= this->minimumHeight())
                        rMove.setY(tl.y());
                    else
                        rMove.setY(gloPoint.y());
                    break;
                case DOWN:
                    rMove.setHeight(gloPoint.y() - tl.y());
                    break;
                case LEFTTOP:
                    if(rb.x() - gloPoint.x() <= this->minimumWidth())
                        rMove.setX(tl.x());
                    else
                        rMove.setX(gloPoint.x());
                    if(rb.y() - gloPoint.y() <= this->minimumHeight())
                        rMove.setY(tl.y());
                    else
                        rMove.setY(gloPoint.y());
                    break;
                case RIGHTTOP:
                    rMove.setWidth(gloPoint.x() - tl.x());
                    rMove.setY(gloPoint.y());
                    break;
                case LEFTBOTTOM:
                    rMove.setX(gloPoint.x());
                    rMove.setHeight(gloPoint.y() - tl.y());
                    break;
                case RIGHTBOTTOM:
                    rMove.setWidth(gloPoint.x() - tl.x());
                    rMove.setHeight(gloPoint.y() - tl.y());
                    break;
                default:
                    break;
                }
                this->setGeometry(rMove);
            } else {
                move(event->globalPos() - dragPosition_);
                event->accept();
            }
        }
    }
    QWidget::mouseMoveEvent(event);
}

void QBasicFormMoveableWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && move_plan_ == PLANA) {
        isLeftPressDown_ = false;
        if(dir_ != NONE) {
            this->releaseMouse();
            this->setCursor(QCursor(Qt::ArrowCursor));
        }
    }
    QWidget::mouseReleaseEvent(event);
}

void QBasicFormMoveableWidget::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(event);
}

bool QBasicFormMoveableWidget::winEvent(MSG *message, long *result)
{
    if(move_plan_ == PLANB){
        switch(message->message)
        {
        case WM_NCHITTEST:
            int xPos = GET_X_LPARAM(message->lParam) - this->frameGeometry().x();
            int yPos = GET_Y_LPARAM(message->lParam) - this->frameGeometry().y();
            if(this->childAt(xPos,yPos) == 0)
            {
                *result = HTCAPTION;
            }else{
                return false;
            }
            if(xPos > 1 && xPos < 6)
                *result = HTLEFT;
            if(xPos > (this->width() - 6) && xPos < (this->width() - 1))
                *result = HTRIGHT;
            if(yPos > 1 && yPos < 6)
                *result = HTTOP;
            if(yPos > (this->height() - 6) && yPos < (this->height() - 1))
                *result = HTBOTTOM;
            if(xPos > 1 && xPos < 6 && yPos > 1 && yPos < 6)
                *result = HTTOPLEFT;
            if(xPos > (this->width() - 6) && xPos < (this->width() - 1) && yPos > 1 && yPos < 6)
                *result = HTTOPRIGHT;
            if(xPos > 1 && xPos < 6 && yPos > (this->height() - 6) && yPos < (this->height() - 1))
                *result = HTBOTTOMLEFT;
            if(xPos > (this->width() - 6) && xPos < (this->width() - 1) && yPos > (this->height() - 6) && yPos < (this->height() - 1))
                *result = HTBOTTOMRIGHT;
            return true;
        }
        return false;
    } else {
        return false;
    }
}

void QBasicFormMoveableWidget::region(const QPoint &cursorGlobalPoint)
{
    QRect rect = this->rect();
    QPoint tl = mapToGlobal(rect.topLeft());
    QPoint rb = mapToGlobal(rect.bottomRight());
    int x = cursorGlobalPoint.x();
    int y = cursorGlobalPoint.y();

    if(tl.x() + PADDING >= x && tl.x() <= x && tl.y() + PADDING >= y && tl.y() <= y) {
        // 左上角
        dir_ = LEFTTOP;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));
    } else if(x >= rb.x() - PADDING && x <= rb.x() && y >= rb.y() - PADDING && y <= rb.y()) {
        // 右下角
        dir_ = RIGHTBOTTOM;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));
    } else if(x <= tl.x() + PADDING && x >= tl.x() && y >= rb.y() - PADDING && y <= rb.y()) {
        //左下角
        dir_ = LEFTBOTTOM;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    } else if(x <= rb.x() && x >= rb.x() - PADDING && y >= tl.y() && y <= tl.y() + PADDING) {
        // 右上角
        dir_ = RIGHTTOP;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    } else if(x <= tl.x() + PADDING && x >= tl.x()) {
        // 左边
        dir_ = LEFT;
        this->setCursor(QCursor(Qt::SizeHorCursor));
    } else if( x <= rb.x() && x >= rb.x() - PADDING) {
        // 右边
        dir_ = RIGHT;
        this->setCursor(QCursor(Qt::SizeHorCursor));
    }else if(y >= tl.y() && y <= tl.y() + PADDING){
        // 上边
        dir_ = UP;
        this->setCursor(QCursor(Qt::SizeVerCursor));
    } else if(y <= rb.y() && y >= rb.y() - PADDING) {
        // 下边
        dir_ = DOWN;
        this->setCursor(QCursor(Qt::SizeVerCursor));
    }else {
        // 默认
        dir_ = NONE;
        this->setCursor(QCursor(Qt::ArrowCursor));
    }
}
MOVEPLAN QBasicFormMoveableWidget::move_plan() const
{
    return move_plan_;
}

void QBasicFormMoveableWidget::setMove_plan(const MOVEPLAN &move_plan)
{
    move_plan_ = move_plan;
}

