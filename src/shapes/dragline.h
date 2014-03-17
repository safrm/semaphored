#ifndef DRAGLINE_H
#define DRAGLINE_H

#include <QWidget>
#include "abstractdraginterface.h"
#include "dragbaseline.h"

class DragLine : public DragBaseLine
{
    Q_OBJECT
    Q_INTERFACES(AbstractDragInterface)

    public:
        enum LineEnding {
            Plain              = 0x0,
            Arrow              = 0x1,
            Association        = Arrow,
            Aggregation        = 0x2,
            Composition        = 0x4,
            AssociationEnd     = 0x8,
            GeneralizationEnd  = 0x16
      };
  //    Q_DECLARE_FLAGS(LineEndings, LineEnding)


      explicit DragLine(const QPoint & p1, const QPoint & p2, DragWidget *parent);
      //virtual ~DragLine () {}
      //const QPoint p1() { return m_p1; }
      //const QPoint p2() { return m_p2; }


      //abstract behaviour
      //virtual QColor currentColor();
      //virtual void changeColor(const QColor &acolor);
      //virtual void select(bool bSelected);
      virtual QMenu* rightClickMenu();

    protected:
      //void paintEvent(QPaintEvent *event);
      virtual void contextMenuEvent ( QContextMenuEvent * event );

    private slots:
      void deleteItemSlot();
      void changeWidthSlot(QAction *action);

    private:
      LineEnding m_LineEnding_p1, m_LineEnding_p2;
      QMenu*   m_RightClickMenu;
      QAction* m_p2pAction;
      QAction* m_p4pAction;
      QAction* m_p8pAction;
};
//Q_DECLARE_OPERATORS_FOR_FLAGS(DragLine::LineEndings);

#endif // DRAGLINE_H
